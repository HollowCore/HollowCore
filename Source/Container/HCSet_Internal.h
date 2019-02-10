//
//  HCSet_Internal.h
//  HollowCore
//
//  Created by Matt Stoker on 1/23/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCSet_Internal_h
#define HCSet_Internal_h

#include "../Core/HCObject_Internal.h"
#include "HCSet.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCSetSlot {
    HCRef object;
    struct HCSetSlot* next;
} HCSetSlot;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCSet {
    HCObject base;
    HCInteger count;
    HCInteger capacity;
    HCSetSlot* slots;
} HCSet;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCSetInit(void* memory, HCInteger capacity);
void HCSetDestroy(HCSetRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Searching
//----------------------------------------------------------------------------------------------------------------------------------
void HCSetFindSlotContainingObject(HCSetRef self, HCRef object, HCInteger* objectSlotIndex, HCBoolean* found, HCSetSlot** resultSlot);
HCInteger HCSetNextSlotFromSlot(HCSetRef self, HCInteger index);

#endif /* HCSet_Internal_h */
