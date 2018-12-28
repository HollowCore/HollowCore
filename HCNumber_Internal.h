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
enum HCNumberValue {
    HCNumberValueBoolean = 0,
    HCNumberValueInteger,
    HCNumberValueReal
};
typedef enum HCNumberValue HCNumberValue;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
struct HCNumber {
    HCObject base;
    HCNumberValue type;
    union {
        HCBoolean boolean;
        HCInteger integer;
        HCReal real;
    } value;
};

#endif /* HCNumber_Internal_h */
