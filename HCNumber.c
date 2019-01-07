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
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCNumberRef HCNumberCreate(void) {
    HCNumberRef self = calloc(sizeof(HCNumber), 1);
    HCObjectInit(self);
    self->value.boolean = false;
    return self;
}

HCNumberRef HCNumberCreateWithBoolean(HCBoolean value) {
    HCNumberRef self = calloc(sizeof(HCNumber), 1);
    HCObjectInit(self);
    self->value.boolean = !(!(value));
    return self;
}

HCNumberRef HCNumberCreateWithInteger(HCInteger value) {
    HCNumberRef self = calloc(sizeof(HCNumber), 1);
    HCObjectInit(self);
    self->value.integer = value;
    return self;
}

HCNumberRef HCNumberCreateWithReal(HCReal value) {
    HCNumberRef self = calloc(sizeof(HCNumber), 1);
    HCObjectInit(self);
    self->value.real = value;
    return self;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCNumberIsEqual(HCNumberRef self, HCNumberRef other) {
    switch (self->type) {
        case HCNumberValueBoolean:
            switch (other->type) {
                case HCNumberValueBoolean: return !self->value.boolean == !other->value.boolean;
                case HCNumberValueInteger: return !self->value.boolean ? (other->value.integer == 0) : (other->value.integer == 1);
                case HCNumberValueReal: return !self->value.boolean ? (other->value.real == 0.0) : (other->value.real == 1.0); // TODO: Hash matches for this?
            }
        case HCNumberValueInteger:
            switch (other->type) {
                case HCNumberValueBoolean: return !other->value.boolean ? (self->value.integer == 0) : (self->value.integer == 1);
                case HCNumberValueInteger: return self->value.integer == other->value.integer;
                case HCNumberValueReal: return self->value.real == (HCInteger)floor(other->value.real); // TODO: Hash matches for this?
            }
        case HCNumberValueReal:
            switch (other->type) {
                case HCNumberValueBoolean: return !other->value.boolean ? (self->value.real == 0.0) : (self->value.real == 1.0); // TODO: Hash matches for this?
                case HCNumberValueInteger: return other->value.integer == (HCInteger)floor(self->value.real); // TODO: Hash matches for this?
                case HCNumberValueReal: return self->value.real == other->value.real; // TODO: NAN?
            }
    }
    
    return self->value.integer == other->value.integer;
}

HCInteger HCNumberHashValue(HCNumberRef self) {
    return self->value.integer;
}

void HCNumberPrint(HCNumberRef self, FILE* stream) {
    HCObjectPrint((HCObjectRef)self, stream);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCNumberGetBoolean(HCNumberRef self) {
    switch (self->type) {
        case HCNumberValueBoolean: return self->value.boolean;
        case HCNumberValueInteger: return self->value.integer != 0;
        case HCNumberValueReal: return self->value.real != 0.0;
    }
    return self->value.boolean;
}

HCInteger HCNumberGetInteger(HCNumberRef self) {
    switch (self->type) {
        case HCNumberValueBoolean: return self->value.boolean == false ? 0 : 1;
        case HCNumberValueInteger: return self->value.integer;
        case HCNumberValueReal: return floor(self->value.real);
    }
    return self->value.integer;
}

HCReal HCNumberGetReal(HCNumberRef self) {
    switch (self->type) {
        case HCNumberValueBoolean: return self->value.boolean == false ? 0.0 : 1.0;
        case HCNumberValueInteger: return self->value.integer;
        case HCNumberValueReal: return self->value.real;
    }
    return self->value.real;
}
