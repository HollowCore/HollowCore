///
/// @file HCThread.c
/// @package HollowCore
///
/// @author Braden Scothern
/// @date 2/14/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCThread_Internal.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCThreadTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCThread",
    },
    .isEqual = (void*)HCThreadIsEqual,
    .hashValue = (void*)HCThreadHashValue,
    .print = (void*)HCThreadPrint,
    .destroy = (void*)HCThreadDestroy,
};
HCType HCThreadType = (HCType)&HCThreadTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCThreadRef HCThreadCreate(HCThreadFunction function, void* context) {
    return HCThreadCreateWithOptions(function, context, HCThreadOptionNone);
}

HCThreadRef HCThreadCreateWithOptions(HCThreadFunction function, void* context, HCThreadOption options) {
    if (function == NULL) {
        return NULL;
    }
    HCThreadRef self = calloc(sizeof(HCThread), 1);
    HCThreadInit(self, function, context, options);
    return self;
}

void HCThreadInit(void* memory, HCThreadFunction function, void* context, HCThreadOption options) {
    HCObjectInit(memory);
    HCThreadRef self = memory;
    self->function = function;
    self->context = context;
    self->options = options;

    HCObjectSetType(self, HCThreadType);
}

void HCThreadDestroy(HCThreadRef self) {
    if (self->options & HCThreadOptionCancelOnDestroy) {
        HCThreadCancel(self);
    }

    // Prevent joining on its own thread since
    if (self->options & HCThreadOptionJoinOnDestroy && !pthread_equal(self->pthread, pthread_self())) {
        HCThreadJoin(self);
    }

    if (self->context != NULL) {
        if (self->options & HCThreadOptionReleaseContextOnDestroy) {
            HCRelease(self->context);
        }
        else if (self->options & HCThreadOptionFreeContextOnDestroy) {
            free(self->context);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCThreadIsEqual(HCThreadRef self, HCThreadRef other) {
    return self == other;
}

HCInteger HCThreadHashValue(HCThreadRef self) {
    return (HCInteger)self;
}

void HCThreadPrint(HCThreadRef self, FILE* stream) {
    HCObjectPrint((HCObjectRef)self, stream);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Current Thread
//----------------------------------------------------------------------------------------------------------------------------------

/// The @c pthread_once_t used ensure that @c HCThreadCurrentThreadKey is only initialized once.
static pthread_once_t HCThreadCurrentThreadKeyOnce = PTHREAD_ONCE_INIT;
/// The @c pthread_key_t used to lookup the current @c HCThread.
static pthread_key_t HCThreadCurrentThreadKey;

/// The function called when @c HCThreadCurrentThreadKeyOnce hasn't been run.
static void HCThreadSetupCurrentThreadKey(void) {
    pthread_key_create(&HCThreadCurrentThreadKey, NULL);
}

/// Used to set the current thread as the current thread.
static void HCThreadSetCurrentThread(HCThreadRef self) {
    pthread_once(&HCThreadCurrentThreadKeyOnce, HCThreadSetupCurrentThreadKey);
    pthread_setspecific(HCThreadCurrentThreadKey, self);
}

HCThreadRef HCThreadGetCurrent(void) {
    pthread_once(&HCThreadCurrentThreadKeyOnce, HCThreadSetupCurrentThreadKey);
    return pthread_getspecific(HCThreadCurrentThreadKey);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Execution State
//----------------------------------------------------------------------------------------------------------------------------------
void HCThreadStart(HCThreadRef self) {
    pthread_create(&self->pthread, NULL, (void*)HCThreadStartEntry, self);
    atomic_store(&self->isJoined, false);
}

void HCThreadCancel(HCThreadRef self) {
    atomic_store(&self->isCanceled, true);
}

HCBoolean HCThreadIsExecuting(HCThreadRef self) {
    return atomic_load(&self->isExecuting);
}

HCBoolean HCThreadIsCanceled(HCThreadRef self) {
    return atomic_load(&self->isCanceled);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Management
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCThreadIsJoined(HCThreadRef self) {
    return atomic_load(&self->isJoined);
}

void HCThreadJoin(HCThreadRef self) {
    if (!atomic_exchange(&self->isJoined, true)) {
        pthread_join(self->pthread, NULL);
    }
    // TODO: Log error if called when not needed?
}

void* HCThreadGetContext(HCThreadRef self) {
    return self->context;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Entry
//----------------------------------------------------------------------------------------------------------------------------------
void* HCThreadStartEntry(HCThreadRef self) {
    HCThreadSetCurrentThread(self);

    atomic_store(&self->isExecuting, true);
    self->function(self->context);
    atomic_store(&self->isExecuting, false);

    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Options
//----------------------------------------------------------------------------------------------------------------------------------
HCThreadOption HCThreadGetOptions(HCThreadRef self) {
    return self->options;
}

// TODO: Should these options exist? If so enable them and expose them in the public header.
#if 0
void HCThreadSetOptions(HCThreadRef self, HCThreadOption options) {
    self->options |= options;
}

void HCThreadUnsetOptions(HCThreadRef self, HCThreadOption options) {
    // Determine the options that are currently enabled so they can be removed via XOR
    HCThreadOption removeMask = self->options & options;
    self->options ^= removeMask;
}

void HCThreadClearOptions(HCThreadRef self) {
    self->options = HCThreadOptionNone;
}
#endif
