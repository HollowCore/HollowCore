///
/// @file HCLock_Internal.h
/// @package HollowCore
///
/// @author Braden Scothern
/// @date 4/12/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCLock_Internal_h
#define HCLock_Internal_h

#include "../Core/HCObject_Internal.h"
#include "HCLock.h"
#include <pthread.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCLock {
    HCObject base;
    pthread_mutex_t mutex;
} HCLock;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCLockInit(void* memory);
void HCLockDestroy(HCLockRef self);

#endif /* HCLock_Internal_h */
