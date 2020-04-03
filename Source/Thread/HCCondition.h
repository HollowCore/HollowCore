//
//  HCCondition.h
//  HollowCore
//
//  Created by Braden Scothern on 3/27/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

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
typedef void* (HCConditionExecuteAquiredFunction)(void* context);
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
void* HCConditionExecuteAquired(HCConditionRef self, HCConditionExecuteAquiredFunction function, void* context);
void* HCConditionExecuteRaisingEventAquired(HCConditionRef self, HCConditionExecuteAquiredFunction function, void* context, HCConditionEvent event);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Events
//----------------------------------------------------------------------------------------------------------------------------------
void HCConditionSignal(HCConditionRef self);
void HCConditionBroadcast(HCConditionRef self);
void HCConditionRaiseEvent(HCConditionRef self, HCConditionEvent event);
void HCConditionRaiseEventAquired(HCConditionRef self, HCConditionEvent event);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Waiting
//----------------------------------------------------------------------------------------------------------------------------------
void HCConditionWait(HCConditionRef self);
void HCConditionWaitAquired(HCConditionRef self);
void* HCConditionWaitThenExecute(HCConditionRef self, HCConditionExecuteAquiredFunction function, void* context);
void* HCConditionWaitThenExecuteAquired(HCConditionRef self, HCConditionExecuteAquiredFunction function, void* context);

HCBoolean HCConditionWaitTimeout(HCConditionRef self, HCReal secondsToTimeout);
HCBoolean HCConditionWaitTimeoutAquired(HCConditionRef self, HCReal secondsToTimeout);
void HCConditionWaitTimeoutThenExecute(HCConditionRef self, HCReal secondsToTimeout, HCConditionExecuteAquiredFunction function, void* context, void** result, HCBoolean* didTimeout);
void HCConditionWaitTimeoutThenExecuteAquired(HCConditionRef self, HCReal secondsToTimeout, HCConditionExecuteAquiredFunction function, void* context, void** result, HCBoolean* didTimeout);

void HCConditionWaitWhile(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* context, HCReal waitIntervalDuration);
void HCConditionWaitWhileAquired(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* context, HCReal waitIntervalDuration);
void* HCConditionWaitWhileThenExecute(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* waitWhileContext, HCReal waitIntervalDuration, HCConditionExecuteAquiredFunction function, void* functionContext);
void* HCConditionWaitWhileThenExecuteAquired(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* waitWhileContext, HCReal waitIntervalDuration, HCConditionExecuteAquiredFunction function, void* functionContext);

#endif /* HCCondition_h */
