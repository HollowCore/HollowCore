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
    
}

HCNumberRef HCNumberCreateWithBoolean(HCBoolean value) {
    
}

HCNumberRef HCNumberCreateWithInteger(HCInteger value) {
    
}

HCNumberRef HCNumberCreateWithReal(HCReal value) {
    
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCNumberIsEqual(HCNumberRef self, HCNumberRef other) {
    switch (self->type) {
        case HCNumberValueBoolean:
            switch (other->type) {
                case HCNumberValueBoolean:
                case HCNumberValueInteger:
                case HCNumberValueReal:
            }
        case HCNumberValueInteger:
            switch (other->type) {
                case HCNumberValueBoolean:
                case HCNumberValueInteger:
                case HCNumberValueReal:
            }
        case HCNumberValueReal:
            switch (other->type) {
                case HCNumberValueBoolean:
                case HCNumberValueInteger:
                case HCNumberValueReal:
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
