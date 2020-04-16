//
//  HCCondition_Internal.h
//  HollowCore
//
//  Created by Braden Scothern on 3/27/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#ifndef HCCondition_Internal_h
#define HCCondition_Internal_h

#include "../Core/HCObject_Internal.h"
#include "HCLock.h"
#include "HCCondition.h"
#include <pthread.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCCondition {
    HCObject base;
    HCLockRef lock;
    pthread_cond_t condition;
} HCCondition;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCConditionInit(void* memory);
void HCConditionDestroy(HCConditionRef self);

#endif /* HCCondition_Internal_h */
