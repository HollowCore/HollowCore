//
//  HCList_Internal.h
//  HollowCore
//
//  Created by Matt Stoker on 1/21/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCList_Internal_h
#define HCList_Internal_h

#include "HCObject_Internal.h"
#include "HCList.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCList {
    HCObject base;
    HCInteger count;
    HCInteger capacity;
    HCRef* objects;
} HCList;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCListInit(void* memory, HCInteger capacity);
void HCListDestroy(HCListRef self);

#endif /* HCList_Internal_h */
