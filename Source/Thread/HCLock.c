//
//  HCLock.c
//  HollowCore
//
//  Created by Braden Scothern on 4/12/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCLock_Internal.h"
#include <errno.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCLockTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCLock",
    },
    .isEqual = (void*)HCLockIsEqual,
    .hashValue = (void*)HCLockHashValue,
    .print = (void*)HCLockPrint,
    .destroy = (void*)HCLockDestroy,
};
HCType HCLockType = (HCType)&HCLockTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCLockRef HCLockCreate(void) {
    HCLockRef self = calloc(sizeof(HCLock), 1);
    HCLockInit(self);
    return self;
}

void HCLockInit(void* memory) {
    HCObjectInit(memory);
    HCLockRef self = memory;

    pthread_mutexattr_t attribute;
    pthread_mutexattr_init(&attribute);
    pthread_mutexattr_settype(&attribute, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&self->mutex, &attribute);

    HCObjectSetType(self, HCLockType);
}

void HCLockDestroy(HCLockRef self) {
    pthread_mutex_destroy(&self->mutex);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCLockIsEqual(HCLockRef self, HCLockRef other) {
    return HCObjectIsEqual((HCObjectRef)self, (HCObjectRef)other);
}

HCInteger HCLockHashValue(HCLockRef self) {
    return HCObjectHashValue((HCObjectRef)self);
}

void HCLockPrint(HCLockRef self, FILE* stream) {
    HCObjectPrint((HCObjectRef)self, stream);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Locking and Unlocking
//----------------------------------------------------------------------------------------------------------------------------------
void HCLockAquire(HCLockRef self) {
    int lockResult = pthread_mutex_lock(&self->mutex);
    if (lockResult == EDEADLK) {
        // Deadlock detected
    }
}

HCBoolean HCLockTryAquire(HCLockRef self) {
    int tryLockResult = pthread_mutex_trylock(&self->mutex);
    if (tryLockResult == EBUSY) {
        // Lock already Acquired
    }
    return tryLockResult == 0;
}

void HCLockRelinquish(HCLockRef self) {
    int unlockResult = pthread_mutex_unlock(&self->mutex);
    if (unlockResult == EPERM) {
        // Lock was not owned by the current thread
    }
}

void* HCLockExecuteAcquired(HCLockRef self, HCLockExecuteAcquiredFunction function, void* context) {
    if (function == NULL) {
        return NULL;
    }
    HCLockAquire(self);
    void* functionResult = function(context);
    HCLockRelinquish(self);
    return functionResult;
}
