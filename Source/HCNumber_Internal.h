//
//  HCNumber_Internal.h
//  HollowCore
//
//  Created by Matt Stoker on 12/28/18.
//  Copyright © 2018 HollowCore. All rights reserved.
//

#ifndef HCNumber_Internal_h
#define HCNumber_Internal_h

#include "HCObject_Internal.h"
#include "HCNumber.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef enum HCNumberValueType {
    HCNumberValueTypeBoolean = 0,
    HCNumberValueTypeInteger,
    HCNumberValueTypeReal
} HCNumberValueType;

typedef union HCNumberValue {
    HCBoolean boolean;
    HCInteger integer;
    HCReal real;
} HCNumberValue;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCNumber {
    HCObject base;
    HCNumberValueType type;
    HCNumberValue value;
} HCNumber;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCNumberInit(void* memory, HCNumberValueType type, HCNumberValue value);
void HCNumberDestroy(HCNumberRef self);

#endif /* HCNumber_Internal_h */
