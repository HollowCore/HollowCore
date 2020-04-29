///
/// @file HCString.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 1/17/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCString_Internal.h"
#include <inttypes.h>
#include <string.h>
#include <float.h>
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCStringTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCString",
    },
    .isEqual = (void*)HCStringIsEqual,
    .hashValue = (void*)HCStringHashValue,
    .print = (void*)HCStringPrint,
    .destroy = (void*)HCStringDestroy,
};
HCType HCStringType = (HCType)&HCStringTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------
static const char HCStringCodePointCodeUnitCount[256] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4,5,5,5,5,6,6,6,6
};

static const HCStringCodePoint HCStringCodePointOffsets[6] = { 0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL, 0xFA082080UL, 0x82082080UL };
static const HCStringCodePoint HCStringCodePointMax = 0x0010FFFFUL;
static const HCStringCodePoint HCStringSurrogateHighStart = 0x0000D800UL;
//static const HCStringCodePoint HCStringSurrogateHighEnd = 0x0000DBFFUL;
//static const HCStringCodePoint HCStringSurrogateLowStart = 0x0000DC00UL;
static const HCStringCodePoint HCStringSurrogateLowEnd = 0x0000DFFFUL;
static const HCStringCodePoint HCStringCodePointReplacement = 0x0000FFFDUL;

static const char* HCStringFalseBooleans[] = {
    "⊭",
    "0",
    "f",
    "false",
};

static const char* HCStringTrueBooleans[] = {
    "⊨",
    "1",
    "t",
    "true",
};

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCStringRef HCStringCreate(void) {
    HCStringRef self = calloc(sizeof(HCString), 1);
    HCStringInit(self, 0, NULL);
    return self;
}

HCStringRef HCStringCreateWithBytes(HCStringEncoding encoding, HCInteger size, const HCByte* bytes) {
    // Convert passed bytes to UTF-8 code units
    HCStringCodeUnit* codeUnits;
    switch (encoding) {
        case HCStringEncodingUTF8: codeUnits = (HCStringCodeUnit*)bytes; break;
        case HCStringEncodingUTF16: return NULL; // TODO: Support for UTF-16
        case HCStringEncodingUTF32: return NULL; // TODO: Support for UTF-32
    }
    
    // Initialize the string object with the code unit data
    HCStringRef self = calloc(sizeof(HCString), 1);
    HCStringInit(self, size, codeUnits);
    return self;
}

HCStringRef HCStringCreateWithCString(const char* value) {
    // Determine the length of the null-terminated string
    size_t length = strlen(value);
    return HCStringCreateWithBytes(HCStringEncodingUTF8, length * sizeof(char), (const HCByte*)value);
}

HCStringRef HCStringCreateWithBoolean(HCBoolean value) {
    HCStringRef self = calloc(sizeof(HCString), 1);
    // TODO: What string values should be used to represent true and false? "true" & "false"? "1" & "0"? "t" & "f"? "⊨" & "⊭"?
    const char* stringValue = value ? "⊨" : "⊭";
    HCStringInit(self, strlen(stringValue), (HCStringCodeUnit*)stringValue);
    return self;
}

HCStringRef HCStringCreateWithInteger(HCInteger value) {
    // Determine the length of the printed integer value, allocate memory to store it, and print the integer value
    ssize_t length = snprintf(NULL, 0, "%" PRIi64, value);
    char* string = malloc(length + 1);
    snprintf(string, length + 1, "%" PRIi64, value);
    
    // Initialize the string object with the integer string
    HCStringRef self = calloc(sizeof(HCString), 1);
    HCStringInitWithoutCopying(self, length, (HCStringCodeUnit*)string);
    return self;
}

HCStringRef HCStringCreateWithReal(HCReal value) {
    // Determine the length of the printed real value, allocate memory to store it, and print the real value
    ssize_t length = snprintf(NULL, 0, "%.17g", value);
    char* string = malloc(length + 1);
    snprintf(string, length + 1, "%.17g", value);
    
    // Initialize the string object with the real value string
    HCStringRef self = calloc(sizeof(HCString), 1);
    HCStringInitWithoutCopying(self, length, (HCStringCodeUnit*)string);
    return self;
}

void HCStringInit(void* memory, HCInteger codeUnitCount, HCStringCodeUnit* codeUnits) {
    // Copy the passed code units, ensuring a null terminating character follows them, allowing for ease use as a C string
    // TODO: Check that the allocation and copy proceed successfully, determine how to pass the error otherwise
    HCStringCodeUnit* copiedCodeUnits = malloc(codeUnitCount + 1 * sizeof(HCStringCodeUnit));
    memcpy(copiedCodeUnits, codeUnits, codeUnitCount * sizeof(HCStringCodeUnit));
    copiedCodeUnits[codeUnitCount] = '\0';
    
    // Initialize the string object with the copied code units
    HCStringInitWithoutCopying(memory, codeUnitCount, copiedCodeUnits);
}

void HCStringInitWithoutCopying(void* memory, HCInteger codeUnitCount, HCStringCodeUnit* codeUnits) {
    // Initialize the string object with the passed code unit data
    // NOTE: The codeUnits must point to memory of at least length codeUnitCount + 1 and the byte at codeUnits[codeUnitCount] must contain a null terminating character
    HCObjectInit(memory);
    HCStringRef self = memory;
    self->base.type = HCStringType;
    self->codeUnitCount = codeUnitCount;
    self->codeUnits = codeUnits;
}

void HCStringDestroy(HCStringRef self) {
    free(self->codeUnits);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCStringIsEqual(HCStringRef self, HCStringRef other) {
    return self->codeUnitCount == other->codeUnitCount && memcmp(self->codeUnits, other->codeUnits, self->codeUnitCount * sizeof(HCStringCodeUnit)) == 0;
}

HCInteger HCStringHashValue(HCStringRef self) {
    HCInteger hash = 5381;
    for (HCInteger codeUnitIndex = 0; codeUnitIndex < self->codeUnitCount; codeUnitIndex++) {
        HCStringCodeUnit c = self->codeUnits[codeUnitIndex];
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void HCStringPrint(HCStringRef self, FILE* stream) {
    fprintf(stream, "\"%s\"", HCStringAsCString(self));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCStringIsEmpty(HCStringRef self) {
    return self->codeUnitCount == 0;
}

HCInteger HCStringCodeUnitCount(HCStringRef self) {
    return self->codeUnitCount;
}

HCStringCodeUnit HCStringCodeUnitAtIndex(HCStringRef self, HCInteger codeUnitIndex) {
    return self->codeUnits[codeUnitIndex];
}

void HCStringExtractCodeUnits(HCStringRef self, HCInteger codeUnitIndex, HCInteger count, HCStringCodeUnit* destination) {
    memcpy(destination, self->codeUnits + codeUnitIndex, count * sizeof(HCStringCodeUnit));
}

HCInteger HCStringCodePointCount(HCStringRef self) {
    HCInteger size = 0;
    HCStringCodePoint* end = (HCStringCodePoint*)(-1);
    HCStringConvertCodeUnits(self, NULL, NULL, (HCStringCodePoint**)&size, end);
    return (HCInteger)(size) / sizeof(HCStringCodePoint);
}

HCStringCodePoint HCStringCodePointAtIndex(HCStringRef self, HCInteger codePointIndex) {
    // Convert the requested code point
    HCStringCodePoint result[1] = {0};
    HCStringExtractCodePoints(self, codePointIndex, 1, result);
    return *result;
}

void HCStringExtractCodePoints(HCStringRef self, HCInteger codePointIndex, HCInteger count, HCStringCodePoint* destination) {
    // Walk to the requested code point index
    HCInteger start = 0;
    HCInteger end = sizeof(HCStringCodePoint) * codePointIndex;
    HCStringCodeUnit* codeUnitIndex = self->codeUnits;
    HCStringConvertCodeUnits(self, &codeUnitIndex, NULL, (HCStringCodePoint**)&start, (HCStringCodePoint*)end);
    if (codeUnitIndex >= self->codeUnits + self->codeUnitCount) {
        return;
    }
    
    // Convert code points until the count is reached, or the source is exausted
    HCStringCodePoint* codePoints = destination;
    HCStringConvertCodeUnits(self, &codeUnitIndex, NULL, &codePoints, codePoints + count);
    
    // Fill any remaining memory in the destination
    for (; codePoints < destination; codePoints++) {
        *codePoints = HCStringCodePointReplacement;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCStringCodeUnitSequeceIsValid(const HCStringCodeUnit* source, HCInteger count) {
    HCStringCodePoint a;
    const HCStringCodeUnit* c = source + count;
    switch (count) {
        default: return false;
        case 4: if ((a = (*--c)) < 0x80 || a > 0xBF) return false; // Fallthrough on true
        case 3: if ((a = (*--c)) < 0x80 || a > 0xBF) return false; // Fallthrough on true
        case 2: if ((a = (*--c)) > 0xBF) return false; // Fallthrough on true
        switch (*source) {
            case 0xE0: if (a < 0xA0) return false;
            case 0xED: if (a > 0x9F) return false;
            case 0xF0: if (a < 0x90) return false;
            case 0xF4: if (a > 0x8F) return false;
            default:   if (a < 0x80) return false;
        }
        case 1: if (*source >= 0x80 && *source < 0xC2) return false;
    }
    if (*source > 0xF4) return false;
    return true;
}

void HCStringConvertCodeUnits(HCStringRef self, HCStringCodeUnit** sourceStart, HCStringCodeUnit* sourceEnd, HCStringCodePoint** targetStart, HCStringCodePoint* targetEnd) {
    HCStringCodeUnit* source = sourceStart == NULL ? self->codeUnits : *sourceStart;
    if (sourceEnd == NULL) {
        sourceEnd = self->codeUnits + self->codeUnitCount;
    }
    
    HCBoolean writeTarget = (targetStart == NULL || *targetStart == NULL || targetEnd == NULL) ? false : true;
    HCStringCodePoint* target = targetStart == NULL ? NULL : *targetStart;
    
    while (source < sourceEnd) {
        HCStringCodePoint codePoint = 0;
        unsigned short extraBytesToRead = HCStringCodePointCodeUnitCount[*source]-1;
        if (source + extraBytesToRead >= sourceEnd) {
            if (writeTarget) {
                *target = HCStringCodePointReplacement;
            }
            target++;
            break;
        }
        if (!HCStringCodeUnitSequeceIsValid(source, extraBytesToRead+1)) {
            source++;
            if (writeTarget) {
                *target = HCStringCodePointReplacement;
            }
            target++;
            continue;
        }
        switch (extraBytesToRead) {
            case 5: codePoint += *source++; codePoint <<= 6; // Fallthrough
            case 4: codePoint += *source++; codePoint <<= 6; // Fallthrough
            case 3: codePoint += *source++; codePoint <<= 6; // Fallthrough
            case 2: codePoint += *source++; codePoint <<= 6; // Fallthrough
            case 1: codePoint += *source++; codePoint <<= 6; // Fallthrough
            case 0: codePoint += *source++;
        }
        codePoint -= HCStringCodePointOffsets[extraBytesToRead];
        if (target >= targetEnd) {
            source -= (extraBytesToRead+1);
            break;
        }
        if (writeTarget) {
            if (codePoint <= HCStringCodePointMax) {
                if (codePoint >= HCStringSurrogateHighStart && codePoint <= HCStringSurrogateLowEnd) {
                    *target = HCStringCodePointReplacement;
                } else {
                    *target = codePoint;
                }
            } else {
                *target = HCStringCodePointReplacement;
            }
        }
        target++;
    }
    
    if (sourceStart != NULL) {
        *sourceStart = source;
    }
    if (targetStart != NULL) {
        *targetStart = target;
    }
}

HCBoolean HCStringIsCString(HCStringRef self) {
    // NOTE: Direct conversion is guaranteed by HCStringInit()
    // TODO: Should check if string is valid UTF-8 as well?
    return self->codeUnitCount == (HCInteger)strlen(HCStringAsCString(self));
}

const char* HCStringAsCString(HCStringRef self) {
    // NOTE: Direct conversion is guaranteed by HCStringInit()
    return (char*)self->codeUnits;
}

HCBoolean HCStringIsBoolean(HCStringRef self) {
    // TODO: Use HCStringIsEqual() with literal string booleans
    for (HCInteger booleanIndex = 0; booleanIndex < (HCInteger)(sizeof(HCStringFalseBooleans) / sizeof(const char*)); booleanIndex++) {
        if (HCStringIsEqualToCString(self, HCStringFalseBooleans[booleanIndex])) {
            return true;
        }
    }
    for (HCInteger booleanIndex = 0; booleanIndex < (HCInteger)(sizeof(HCStringTrueBooleans) / sizeof(const char*)); booleanIndex++) {
        if (HCStringIsEqualToCString(self, HCStringTrueBooleans[booleanIndex])) {
            return true;
        }
    }
    return false;
}

HCBoolean HCStringAsBoolean(HCStringRef self) {
    // TODO: Use HCStringHasPrefix() with literal string booleans
    for (HCInteger booleanIndex = 0; booleanIndex < (HCInteger)(sizeof(HCStringTrueBooleans) / sizeof(const char*)); booleanIndex++) {
        const char* boolean = HCStringTrueBooleans[booleanIndex];
        size_t booleanLength = strlen(boolean);
        if (self->codeUnitCount >= (HCInteger)booleanLength && strncmp(HCStringAsCString(self), boolean, booleanLength) == 0) {
            if (self->codeUnits[booleanLength] == '\0' || self->codeUnits[booleanLength] == ' ') {
                return true;
            }
        }
    }
    return false;
}

HCBoolean HCStringIsInteger(HCStringRef self) {
    const char* string = HCStringAsCString(self);
    char* end = (char*)string;
    long long integer = strtoll(string, &end, 10);
    if (sizeof(long long) != sizeof(HCInteger)) {
        if (integer > HCIntegerMaximum) {
            return false;
        }
        if (integer < HCIntegerMinimum) {
            return false;
        }
    }
    return end == string + self->codeUnitCount;
}

HCInteger HCStringAsInteger(HCStringRef self) {
    const char* string = HCStringAsCString(self);
    long long integer = strtoll(string, NULL, 10);
    if (sizeof(long long) == sizeof(HCInteger)) {
        return integer;
    }
    if (integer > HCIntegerMaximum) {
        return HCIntegerMaximum;
    }
    if (integer < HCIntegerMinimum) {
        return HCIntegerMinimum;
    }
    return integer;
}

HCBoolean HCStringIsReal(HCStringRef self) {
    const char* string = HCStringAsCString(self);
    char* end = (char*)string;
    HCReal real = strtod(string, &end);
    return !isnan(real) && end == string + self->codeUnitCount;
}

HCReal HCStringAsReal(HCStringRef self) {
    return strtod(HCStringAsCString(self), NULL);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Comparison
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCStringIsEqualToCString(HCStringRef self, const char* string) {
    return strncmp(HCStringAsCString(self), string, self->codeUnitCount) == 0 && string[self->codeUnitCount] == '\0';
}
