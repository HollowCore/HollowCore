//
//  HCData_Internal.h
//  HollowCore
//
//  Created by Matt Stoker on 1/19/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCData_Internal_h
#define HCData_Internal_h

#include "../Core/HCObject_Internal.h"
#include "HCData.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCData {
    HCObject base;
    HCInteger size;
    HCByte* data;
} HCData;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCDataInit(void* memory, HCInteger size, const HCByte* data);
void HCDataInitWithoutCopying(void* memory, HCInteger size, HCByte* data);
void HCDataDestroy(HCDataRef self);

#endif /* HCData_Internal_h */
