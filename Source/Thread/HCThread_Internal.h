//
//  HCThread_Internal.h
//  HollowCore
//
//  Created by Braden Scothern on 2/14/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

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
