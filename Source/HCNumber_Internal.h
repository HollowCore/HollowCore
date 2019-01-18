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
enum HCNumberValueType {
    HCNumberValueTypeBoolean = 0,
    HCNumberValueTypeInteger,
    HCNumberValueTypeReal
};
typedef enum HCNumberValueType HCNumberValueType;

union HCNumberValue {
    HCBoolean boolean;
    HCInteger integer;
    HCReal real;
};
typedef union HCNumberValue HCNumberValue;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
struct HCNumber {
    HCObject base;
    HCNumberValueType type;
    HCNumberValue value;
};
typedef struct HCNumber HCNumber;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCNumberInit(void* memory, HCNumberValueType type, HCNumberValue value);
void HCNumberDestroy(HCNumberRef self);

#endif /* HCNumber_Internal_h */
