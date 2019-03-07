//
//  HCSphere.c
//  HollowCore
//
//  Created by Matt Stoker on 3/5/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCSphere_Internal.h"
#include <math.h>
/*
//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCSphereTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCSphere",
    },
    .isEqual = (void*)HCSphereIsEqual,
    .hashValue = (void*)HCSphereHashValue,
    .print = (void*)HCSpherePrint,
    .destroy = (void*)HCSphereDestroy,
};
HCType HCSphereType = (HCType)&HCSphereTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCSphereRef HCSphereCreate(HCReal center, HCReal radius) {
    HCSphereRef self = calloc(sizeof(HCSphere), 1);
    HCSphereInit(self, center, radius);
    return self;
}

void HCSphereInit(void* memory, HCReal center, HCReal radius) {
    HCObjectInit(memory);
    HCSphereRef self = memory;
    self->base.type = HCSphereType;
    self->type = type;
    self->value = value;
}

void HCSphereDestroy(HCSphereRef self) {
    (void)self; // Unused
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSphereIsEqual(HCSphereRef self, HCSphereRef other) {
    switch (self->type) {
        case HCSphereValueTypeBoolean:
            switch (other->type) {
                case HCSphereValueTypeBoolean: return HCBooleanIsEqual(self->value.boolean, other->value.boolean);
                case HCSphereValueTypeInteger: return !self->value.boolean ? (other->value.integer == 0) : (other->value.integer == 1);
                case HCSphereValueTypeReal: return !self->value.boolean ? (other->value.real == 0.0) : (other->value.real == 1.0); // TODO: Hash matches for this?
            }
        case HCSphereValueTypeInteger:
            switch (other->type) {
                case HCSphereValueTypeBoolean: return !other->value.boolean ? (self->value.integer == 0) : (self->value.integer == 1);
                case HCSphereValueTypeInteger: return HCIntegerIsEqual(self->value.integer, other->value.integer);
                case HCSphereValueTypeReal: return self->value.integer == (HCInteger)floor(other->value.real); // TODO: Hash matches for this?
            }
        case HCSphereValueTypeReal:
            switch (other->type) {
                case HCSphereValueTypeBoolean: return !other->value.boolean ? (self->value.real == 0.0) : (self->value.real == 1.0); // TODO: Hash matches for this?
                case HCSphereValueTypeInteger: return other->value.integer == (HCInteger)floor(self->value.real); // TODO: Hash matches for this?
                case HCSphereValueTypeReal: return HCRealIsEqual(self->value.real, other->value.real); // TODO: Hash matches for this?
            }
    }
    
    return self->value.integer == other->value.integer;
}

HCInteger HCSphereHashValue(HCSphereRef self) {
    switch (self->type) {
        case HCSphereValueTypeBoolean: return HCBooleanHashValue(self->value.boolean);
        case HCSphereValueTypeInteger: return HCIntegerHashValue(self->value.integer);
        case HCSphereValueTypeReal: return HCRealHashValue(self->value.real);
    }
    return self->value.integer;
}

void HCSpherePrint(HCSphereRef self, FILE* stream) {
    switch (self->type) {
        case HCSphereValueTypeBoolean: HCBooleanPrint(self->value.boolean, stream); break;
        case HCSphereValueTypeInteger: HCIntegerPrint(self->value.integer, stream); break;
        case HCSphereValueTypeReal: HCRealPrint(self->value.real, stream); break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Primitive Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSphereIntersect(HCSphereRef self, HCRef other) {
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCSphereGetCenter(HCSphereRef self) {
    return NAN;
}

HCReal HCSphereGetRadius(HCSphereRef self) {
    return NAN;
}
*/
