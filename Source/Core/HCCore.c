//
//  HCCore.c
//  HollowCore
//
//  Created by Matt Stoker on 12/28/18.
//  Copyright © 2018 HollowCore. All rights reserved.
//

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
    fprintf(stream, "%s", self ? "true" : "false");
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
