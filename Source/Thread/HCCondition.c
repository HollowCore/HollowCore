///
/// @file HCCondition.c
/// @ingroup HollowCore
///
/// @author Braden Scothern
/// @date 3/27/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCCondition_Internal.h"
#include "HCLock_Internal.h"
#include <errno.h>
#include <sys/time.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCConditionTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCCondition",
    },
    .isEqual = (void*)HCConditionIsEqual,
    .hashValue = (void*)HCConditionHashValue,
    .print = (void*)HCConditionPrint,
    .destroy = (void*)HCConditionDestroy,
};
HCType HCConditionType = (HCType)&HCConditionTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCConditionRef HCConditionCreate(void) {
    HCConditionRef self = calloc(sizeof(HCCondition), 1);
    HCConditionInit(self);
    return self;
}

void HCConditionInit(void* memory) {
    HCObjectInit(memory);
    HCConditionRef self = memory;
    
    self->lock = HCLockCreate();
    pthread_cond_init(&self->condition, NULL);
    
    HCObjectSetType(self, HCConditionType);
}

void HCConditionDestroy(HCConditionRef self) {
    HCRelease(self->lock);
    pthread_cond_destroy(&self->condition);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCConditionIsEqual(HCConditionRef self, HCConditionRef other) {
    return HCObjectIsEqual((HCObjectRef)self, (HCObjectRef)other);
}

HCInteger HCConditionHashValue(HCConditionRef self) {
    return HCObjectHashValue((HCObjectRef)self);
}

void HCConditionPrint(HCConditionRef self, FILE* stream) {
    HCObjectPrint((HCObjectRef)self, stream);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Locking and UnLocking
//----------------------------------------------------------------------------------------------------------------------------------
void HCConditionAquire(HCConditionRef self) {
    HCLockAquire(self->lock);
}

void HCConditionRelinquish(HCConditionRef self) {
    HCLockRelinquish(self->lock);
}

void HCConditionRelinquishRaisingEvent(HCConditionRef self, HCConditionEvent event) {
    HCConditionRaiseEvent(self, event);
    HCConditionRelinquish(self);
}
void* HCConditionExecuteAcquired(HCConditionRef self, HCConditionExecuteAcquiredFunction function, void* context) {
    return HCLockExecuteAcquired(self->lock, function, context);
}
void* HCConditionExecuteRaisingEventAcquired(HCConditionRef self, HCConditionExecuteAcquiredFunction function, void* context, HCConditionEvent event) {
    if (function == NULL) {
        return NULL;
    }
    HCLockAquire(self->lock);
    void* functionResult = function(context);
    HCConditionRaiseEvent(self, event);
    HCLockRelinquish(self->lock);
    return functionResult;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Events
//----------------------------------------------------------------------------------------------------------------------------------
void HCConditionSignal(HCConditionRef self) {
    pthread_cond_signal(&self->condition);
}

void HCConditionBroadcast(HCConditionRef self) {
    pthread_cond_broadcast(&self->condition);
}

void HCConditionRaiseEvent(HCConditionRef self, HCConditionEvent event) {
    switch (event) {
        case HCConditionEventSignal:
            HCConditionSignal(self);
            break;
        case HCConditionEventBroadcast:
            HCConditionBroadcast(self);
            break;
    }
}

void HCConditionRaiseEventAcquired(HCConditionRef self, HCConditionEvent event) {
    HCConditionAquire(self);
    HCConditionRelinquishRaisingEvent(self, event);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Waiting
//----------------------------------------------------------------------------------------------------------------------------------
void HCConditionWait(HCConditionRef self) {
    pthread_cond_wait(&self->condition, &self->lock->mutex);
}

void HCConditionWaitAcquired(HCConditionRef self) {
    HCConditionAquire(self);
    HCConditionWait(self);
    HCConditionRelinquish(self);
}

void* HCConditionWaitThenExecute(HCConditionRef self, HCConditionExecuteAcquiredFunction function, void* context) {
    HCConditionWait(self);
    if (function == NULL) {
        return NULL;
    }
    return function(context);
}

void* HCConditionWaitThenExecuteAcquired(HCConditionRef self, HCConditionExecuteAcquiredFunction function, void* context) {
    HCConditionAquire(self);
    void* functionResult = HCConditionWaitThenExecute(self, function, context);
    HCConditionRelinquish(self);
    return functionResult;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Waiting with Timeout
//----------------------------------------------------------------------------------------------------------------------------------

HCBoolean HCConditionWaitTimeout(HCConditionRef self, HCReal timeout) {
    long timeoutWholeSeconds = (long)timeout;
    double timeoutNanoseconds = (timeout - (double)((long)timeoutWholeSeconds)) * 1000000000.0;
    
    struct timeval timeval;
    struct timespec timeoutSpec;
    gettimeofday(&timeval, NULL);
    timeoutSpec.tv_sec = timeval.tv_sec + timeoutWholeSeconds;
    timeoutSpec.tv_nsec = timeval.tv_usec * 1000 + (long)timeoutNanoseconds;

    HCBoolean result = pthread_cond_timedwait(&self->condition, &self->lock->mutex, &timeoutSpec) == ETIMEDOUT;
    return result;
}

HCBoolean HCConditionWaitTimeoutAcquired(HCConditionRef self, HCReal timeout) {
    HCConditionAquire(self);
    HCBoolean didTimeout = HCConditionWaitTimeout(self, timeout);
    HCConditionRelinquish(self);
    return didTimeout;
}

void HCConditionWaitTimeoutThenExecute(HCConditionRef self, HCReal timeout, HCConditionExecuteAcquiredFunction function, void* context, void** result, HCBoolean* didTimeout) {
    HCBoolean timeoutResult = HCConditionWaitTimeout(self, timeout);
    if (didTimeout != NULL) {
        *didTimeout = timeoutResult;
    }
    if (function != NULL && !timeoutResult) {
        void* functionResult = function(context);
        if (result != NULL) {
            *result = functionResult;
        }
    }
}

void HCConditionWaitTimeoutThenExecuteAcquired(HCConditionRef self, HCReal timeout, HCConditionExecuteAcquiredFunction function, void* context, void** result, HCBoolean* didTimeout) {
    HCConditionAquire(self);
    HCConditionWaitTimeoutThenExecute(self, timeout, function, context, result, didTimeout);
    HCConditionRelinquish(self);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Waiting with Timeout Query
//----------------------------------------------------------------------------------------------------------------------------------

void HCConditionWaitWhile(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* context, HCReal waitIntervalDuration) {
    if (waitWhile == NULL) {
        return;
    }
    while (waitWhile(context)) {
        HCConditionWaitTimeout(self, waitIntervalDuration);
    }
}

void HCConditionWaitWhileAcquired(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* context, HCReal waitIntervalDuration) {
    HCConditionAquire(self);
    HCConditionWaitWhile(self, waitWhile, context, waitIntervalDuration);
    HCConditionRelinquish(self);
}


void* HCConditionWaitWhileThenExecute(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* waitWhileContext, HCReal waitIntervalDuration, HCConditionExecuteAcquiredFunction function, void* functionContext) {
    HCConditionWaitWhile(self, waitWhile, waitWhileContext, waitIntervalDuration);
    if (function == NULL) {
        return NULL;
    }
    // TODO: Shouldn't the waitWhile function have some way to indicate that the function should not be executed?
    return function(functionContext);
}

void* HCConditionWaitWhileThenExecuteAcquired(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* waitWhileContext, HCReal waitIntervalDuration, HCConditionExecuteAcquiredFunction function, void* functionContext) {
    HCConditionAquire(self);
    void* functionResult = HCConditionWaitWhileThenExecute(self, waitWhile, waitWhileContext, waitIntervalDuration, function, functionContext);
    HCConditionRelinquish(self);
    return functionResult;
}
