///
/// @file HCCondition.h
/// @ingroup HollowCore
///
/// @author Braden Scothern
/// @date 3/27/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCCondition_h
#define HCCondition_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------

/// The @c HCType that describes a @c HCCondition.
extern HCType HCConditionType;

/// The Ref type of @c HCCondition.
typedef struct HCCondition* HCConditionRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef void* (HCConditionExecuteAcquiredFunction)(void* context);
typedef HCBoolean (HCConditionWaitWhileFunction)(void* context);

typedef enum HCConditionEvent {
    HCConditionEventSignal,
    HCConditionEventBroadcast
} HCConditionEvent;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCConditionRef HCConditionCreate(void);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCConditionIsEqual(HCConditionRef self, HCConditionRef other);
HCInteger HCConditionHashValue(HCConditionRef self);
void HCConditionPrint(HCConditionRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Locking and UnLocking
//----------------------------------------------------------------------------------------------------------------------------------
void HCConditionAquire(HCConditionRef self);
void HCConditionRelinquish(HCConditionRef self);
void HCConditionRelinquishRaisingEvent(HCConditionRef self, HCConditionEvent event);
void* HCConditionExecuteAcquired(HCConditionRef self, HCConditionExecuteAcquiredFunction function, void* context);
void* HCConditionExecuteRaisingEventAcquired(HCConditionRef self, HCConditionExecuteAcquiredFunction function, void* context, HCConditionEvent event);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Events
//----------------------------------------------------------------------------------------------------------------------------------
void HCConditionSignal(HCConditionRef self);
void HCConditionBroadcast(HCConditionRef self);
void HCConditionRaiseEvent(HCConditionRef self, HCConditionEvent event);
void HCConditionRaiseEventAcquired(HCConditionRef self, HCConditionEvent event);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Waiting
//----------------------------------------------------------------------------------------------------------------------------------
void HCConditionWait(HCConditionRef self);
void HCConditionWaitAcquired(HCConditionRef self);
void* HCConditionWaitThenExecute(HCConditionRef self, HCConditionExecuteAcquiredFunction function, void* context);
void* HCConditionWaitThenExecuteAcquired(HCConditionRef self, HCConditionExecuteAcquiredFunction function, void* context);

HCBoolean HCConditionWaitTimeout(HCConditionRef self, HCReal secondsToTimeout);
HCBoolean HCConditionWaitTimeoutAcquired(HCConditionRef self, HCReal secondsToTimeout);
void HCConditionWaitTimeoutThenExecute(HCConditionRef self, HCReal secondsToTimeout, HCConditionExecuteAcquiredFunction function, void* context, void** result, HCBoolean* didTimeout);
void HCConditionWaitTimeoutThenExecuteAcquired(HCConditionRef self, HCReal secondsToTimeout, HCConditionExecuteAcquiredFunction function, void* context, void** result, HCBoolean* didTimeout);

void HCConditionWaitWhile(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* context, HCReal waitIntervalDuration);
void HCConditionWaitWhileAcquired(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* context, HCReal waitIntervalDuration);
void* HCConditionWaitWhileThenExecute(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* waitWhileContext, HCReal waitIntervalDuration, HCConditionExecuteAcquiredFunction function, void* functionContext);
void* HCConditionWaitWhileThenExecuteAcquired(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* waitWhileContext, HCReal waitIntervalDuration, HCConditionExecuteAcquiredFunction function, void* functionContext);

#endif /* HCCondition_h */
