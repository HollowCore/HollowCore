///
/// @file HCCore.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 12/28/18
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCCore.h"
#include <inttypes.h>
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Boolean Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCBooleanIsEqual(HCBoolean self, HCBoolean other) {
    return !self == !other;
}

HCInteger HCBooleanHashValue(HCBoolean self) {
    return self ? 1 : 0;
}

void HCBooleanPrint(HCBoolean self, FILE* stream) {
    fprintf(stream, "%s", self ? "⊨" : "⊭");
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Integer Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCIntegerIsEqual(HCInteger self, HCInteger other) {
    return self == other;
}

HCInteger HCIntegerHashValue(HCInteger self) {
    return self;
}

void HCIntegerPrint(HCInteger self, FILE* stream) {
    fprintf(stream, "%" PRIi64, self);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Real Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRealIsInvalid(HCReal self) {
    return isnan(self);
}

HCBoolean HCRealIsInfinite(HCReal self) {
    return isinf(self);
}

HCBoolean HCRealIsFinite(HCReal self) {
    return isfinite(self);
}

HCBoolean HCRealIsSimilar(HCReal self, HCReal other, HCReal dissimilarity) {
    return fabs(self - other) < dissimilarity;
}

HCBoolean HCRealIsEqual(HCReal self, HCReal other) {
    return self == other; // TODO: NAN?
}

HCInteger HCRealHashValue(HCReal self) {
    // TODO: Does this cause all HCReal values that are exactly equal to HCInteger values to hash to the same value?
    HCReal truncated = floor(self);
    if (truncated - self == 0.0) {
        return (HCInteger)truncated;
    }
    
    // Use the unconverted bits of the real value as the hash
    union {
        HCInteger integer;
        HCReal real;
    } value;
    value.real = self;
    return value.integer;
}

void HCRealPrint(HCReal self, FILE* stream) {
    fprintf(stream, "%f", self);
}
