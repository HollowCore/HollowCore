//
//  HCString.c
//  Test
//
//  Created by Matt Stoker on 1/17/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCString_Internal.h"
#include <inttypes.h>
#include <string.h>
#include <float.h>

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
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCStringRef HCStringCreate(void) {
    HCStringRef self = calloc(sizeof(HCString), 1);
    HCStringInit(self, 0, NULL);
    return self;
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

HCStringRef HCStringCreateWithCString(char* value) {
    // Determine the length of the null-terminated string
    size_t length = strlen(value);
    
    // Initialize the string object with the null-terminated string value
    HCStringRef self = calloc(sizeof(HCString), 1);
    HCStringInit(self, length, (HCStringCodeUnit*)value);
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
    return self->codeUnitCount == other->codeUnitCount && memcmp(self->codeUnits, other->codeUnits, self->codeUnitCount * sizeof(HCStringCodeUnit));
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
    fprintf(stream, "%s", HCStringAsCString(self));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCStringIsEmpty(HCStringRef self) {
    return self->codeUnitCount == 0;
}

HCInteger HCStringGetCodeUnitCount(HCStringRef self) {
    return self->codeUnitCount;
}

HCStringCodeUnit HCStringGetCodeUnit(HCStringRef self, HCInteger codeUnitIndex) {
    return self->codeUnits[codeUnitIndex];
}

HCInteger HCStringGetCodePointCount(HCStringRef self) {
    // TODO: This
    return 0;
}

HCStringCodePoint HCStringGetCodePoint(HCStringRef self, HCInteger codePointIndex) {
    // TODO: This
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCStringCodePoint HCStringConvertCodeUnits(HCStringRef self, HCInteger codeUnitIndex, HCInteger* nextCodeUnitIndex) {
    // TODO: This
    return 0;
}

HCBoolean HCStringAsBoolean(HCStringRef self) {
    // TODO: This
    return false;
}

HCInteger HCStringAsInteger(HCStringRef self) {
    // TODO: This
    return 0;
}

HCReal HCStringAsReal(HCStringRef self) {
    // TODO: This
    return 0.0;
}

const char* HCStringAsCString(HCStringRef self) {
    // NOTE: Direct conversion is guaranteed by HCStringInit()
    return (char*)self->codeUnits;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Comparison
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCStringIsEqualToCString(HCStringRef self, const char* string) {
    return strncmp(HCStringAsCString(self), string, self->codeUnitCount) == 0 && string[self->codeUnitCount] == '\0';
}
