//
//  HCString_Internal.h
//  HollowCore
//
//  Created by Matt Stoker on 1/17/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCString_Internal_h
#define HCString_Internal_h

#include "HCObject_Internal.h"
#include "HCString.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
enum HCStringValue {
    HCStringValueBoolean = 0,
    HCStringValueInteger,
    HCStringValueReal
};
typedef enum HCStringValue HCStringValue;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
struct HCString {
    HCObject base;
    HCInteger count;
    HCStringCodeUnit* data;
} HCString;

#endif /* HCString_Internal_h */
