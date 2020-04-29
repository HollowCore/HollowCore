///
/// @file HCCondition_Internal.h
/// @ingroup HollowCore
///
/// @author Braden Scothern
/// @date 3/27/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

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
