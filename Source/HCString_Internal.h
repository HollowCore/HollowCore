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
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
struct HCString {
    HCObject base;
    HCInteger codeUnitCount;
    HCStringCodeUnit* codeUnits;
} HCString;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCStringInit(void* memory, HCInteger codeUnitCount, HCStringCodeUnit* codeUnits);
void HCStringInitWithoutCopying(void* memory, HCInteger codeUnitCount, HCStringCodeUnit* codeUnits);
void HCStringDestroy(HCStringRef self);

#endif /* HCString_Internal_h */
