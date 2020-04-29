///
/// @file HCThread_Internal.h
/// @ingroup HollowCore
///
/// @author Braden Scothern
/// @date 2/14/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCThread_Internal_h
#define HCThread_Internal_h

#include "../Core/HCObject_Internal.h"
#include "HCThread.h"
#include <pthread.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCThread {
    HCObject base;

    pthread_t pthread;
    HCThreadFunction function;
    void* context;

    HCThreadOption options;

    HCAtomicBoolean isExecuting;
    HCAtomicBoolean isFinished;
    HCAtomicBoolean isCanceled;
    HCAtomicBoolean isJoined;
} HCThread;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCThreadInit(void* memory, HCThreadFunction function, void* context, HCThreadOption options);
void HCThreadDestroy(HCThreadRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Entry
//----------------------------------------------------------------------------------------------------------------------------------
void* HCThreadStartEntry(HCThreadRef self);

#endif /* HCThread_Internal_h */
