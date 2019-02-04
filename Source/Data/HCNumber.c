//
//  HCNumber.c
//  HollowCore
//
//  Created by Matt Stoker on 12/28/18.
//  Copyright © 2018 HollowCore. All rights reserved.
//

#include "HCNumber_Internal.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCNumberTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCNumber",
    },
    .isEqual = (void*)HCNumberIsEqual,
    .hashValue = (void*)HCNumberHashValue,
    .print = (void*)HCNumberPrint,
    .destroy = (void*)HCNumberDestroy,
};
HCType HCNumberType = (HCType)&HCNumberTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCNumberRef HCNumberCreate(void) {
    return HCNumberCreateWithBoolean(false);
}

HCNumberRef HCNumberCreateWithBoolean(HCBoolean value) {
    HCNumberRef self = calloc(sizeof(HCNumber), 1);
    HCNumberValue v = { .boolean = !(!(value)) };
    HCNumberInit(self, HCNumberValueTypeBoolean, v);
    return self;
}

HCNumberRef HCNumberCreateWithInteger(HCInteger value) {
    HCNumberRef self = calloc(sizeof(HCNumber), 1);
    HCNumberValue v = { .integer = value };
    HCNumberInit(self, HCNumberValueTypeInteger, v);
    return self;
}

HCNumberRef HCNumberCreateWithReal(HCReal value) {
    HCNumberRef self = calloc(sizeof(HCNumber), 1);
    HCNumberValue v = { .real = value };
    HCNumberInit(self, HCNumberValueTypeReal, v);
    return self;
}

void HCNumberInit(void* memory, HCNumberValueType type, HCNumberValue value) {
    HCObjectInit(memory);
    HCNumberRef self = memory;
    self->base.type = HCNumberType;
    self->type = type;
    self->value = value;
}

void HCNumberDestroy(HCNumberRef self) {
    (void)self; // Unused
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCNumberIsEqual(HCNumberRef self, HCNumberRef other) {
    switch (self->type) {
        case HCNumberValueTypeBoolean:
            switch (other->type) {
                case HCNumberValueTypeBoolean: return HCBooleanIsEqual(self->value.boolean, other->value.boolean);
                case HCNumberValueTypeInteger: return !self->value.boolean ? (other->value.integer == 0) : (other->value.integer == 1);
                case HCNumberValueTypeReal: return !self->value.boolean ? (other->value.real == 0.0) : (other->value.real == 1.0); // TODO: Hash matches for this?
            }
        case HCNumberValueTypeInteger:
            switch (other->type) {
                case HCNumberValueTypeBoolean: return !other->value.boolean ? (self->value.integer == 0) : (self->value.integer == 1);
                case HCNumberValueTypeInteger: return HCIntegerIsEqual(self->value.integer, other->value.integer);
                case HCNumberValueTypeReal: return self->value.integer == (HCInteger)floor(other->value.real); // TODO: Hash matches for this?
            }
        case HCNumberValueTypeReal:
            switch (other->type) {
                case HCNumberValueTypeBoolean: return !other->value.boolean ? (self->value.real == 0.0) : (self->value.real == 1.0); // TODO: Hash matches for this?
                case HCNumberValueTypeInteger: return other->value.integer == (HCInteger)floor(self->value.real); // TODO: Hash matches for this?
                case HCNumberValueTypeReal: return HCRealIsEqual(self->value.real, other->value.real); // TODO: Hash matches for this?
            }
    }
    
    return self->value.integer == other->value.integer;
}

HCInteger HCNumberHashValue(HCNumberRef self) {
    switch (self->type) {
        case HCNumberValueTypeBoolean: return HCBooleanHashValue(self->value.boolean);
        case HCNumberValueTypeInteger: return HCIntegerHashValue(self->value.integer);
        case HCNumberValueTypeReal: return HCRealHashValue(self->value.real);
    }
    return self->value.integer;
}

void HCNumberPrint(HCNumberRef self, FILE* stream) {
    switch (self->type) {
        case HCNumberValueTypeBoolean: HCBooleanPrint(self->value.boolean, stream); break;
        case HCNumberValueTypeInteger: HCIntegerPrint(self->value.integer, stream); break;
        case HCNumberValueTypeReal: HCRealPrint(self->value.real, stream); break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCNumberIsBoolean(HCNumberRef self) {
    return self->type == HCNumberValueTypeBoolean;
}

HCBoolean HCNumberAsBoolean(HCNumberRef self) {
    switch (self->type) {
        case HCNumberValueTypeBoolean: return self->value.boolean;
        case HCNumberValueTypeInteger: return self->value.integer != 0;
        case HCNumberValueTypeReal: return self->value.real != 0.0;
    }
    return self->value.boolean;
}

HCBoolean HCNumberIsInteger(HCNumberRef self) {
    return self->type == HCNumberValueTypeInteger;
}

HCInteger HCNumberAsInteger(HCNumberRef self) {
    switch (self->type) {
        case HCNumberValueTypeBoolean: return self->value.boolean == false ? 0 : 1;
        case HCNumberValueTypeInteger: return self->value.integer;
        case HCNumberValueTypeReal: return (HCInteger)floor(self->value.real);
    }
    return self->value.integer;
}

HCBoolean HCNumberIsReal(HCNumberRef self) {
    return self->type == HCNumberValueTypeReal;
}

HCReal HCNumberAsReal(HCNumberRef self) {
    switch (self->type) {
        case HCNumberValueTypeBoolean: return self->value.boolean == false ? 0.0 : 1.0;
        case HCNumberValueTypeInteger: return (HCReal)self->value.integer;
        case HCNumberValueTypeReal: return self->value.real;
    }
    return self->value.real;
}
