///
/// @file HCCondition.h
/// @ingroup HollowCore
///
/// @author Braden Scothern
/// @date 3/27/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///
/// @brief Thread mutex and signaling mechanism for data access control with signaling.
///

#ifndef HCCondition_h
#define HCCondition_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------

/// Type of @c HCCondition instances.
extern HCType HCConditionType;

/// A reference to an @c HCCondition instance.
typedef struct HCCondition* HCConditionRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------

/// Function executed while a condition's lock is acquired using @c HCConditionExecuteAcquired() or a related function.
/// @param context The context value passed to @c HCConditionExecuteAcquired() or related function.
/// @returns The value to be returned from @c HCConditionExecuteAcquired() or related function.
typedef void* (HCConditionExecuteAcquiredFunction)(void* context);

/// Function used to determine if a wait operation such as @c HCConditionWaitWhile() should continue.
/// @param context The context value passed to @c HCConditionWaitWhile() or related function.
/// @returns @c true if the wait operation should continue, or @c false if it should terminate.
typedef HCBoolean (HCConditionWaitWhileFunction)(void* context);

/// Event type of a condition.
typedef enum HCConditionEvent {
    HCConditionEventSignal,
    HCConditionEventBroadcast
} HCConditionEvent;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------

/// Creates a condition object.
/// @returns A reference to a condition object that may be acquired, signaled, or waited on.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCConditionRef HCConditionCreate(void);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a condition represents the same underlying signaling mechanism as another condition.
/// @param self A reference to the condition to examine.
/// @param other The other condition to evaluate equality against.
/// @returns @c true if @c self and @c other represent the same internal signaling mechanism and would both be signaled if one was signaled.
HCBoolean HCConditionIsEqual(HCConditionRef self, HCConditionRef other);

/// Calculates a hash value for a condition.
/// @param self A reference to the condition.
/// @returns A hash value determined using only the internal locking mechanism and signaling mechanism of the condition.
HCInteger HCConditionHashValue(HCConditionRef self);

/// Prints a condition to a stream.
/// @param self A reference to the condition.
/// @param stream The stream to which the condition should be printed.
void HCConditionPrint(HCConditionRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Locking and UnLocking
//----------------------------------------------------------------------------------------------------------------------------------

/// Acquires a condition's lock, blocking until it is available to acquire.
///
/// Using this function along with @c HCConditionRelinquish() causes a condition to operate in an analogous way to a simple lock.
///
/// @param self A reference to the condition.
void HCConditionAquire(HCConditionRef self);

/// Relinquishes a condition's lock, allowing other threads to acquire it.
///
/// Using this function along with @c HCConditionAquire() causes a condition to operate in an analogous way to a simple lock.
///
/// @param self A reference to the condition.
void HCConditionRelinquish(HCConditionRef self);

/// Relinquishes a condition's lock and raises a signal, notifying other threads that the condition's lock is available to acquire.
///
/// Using this function along with @c HCConditionAquire() and @c HCConditionWait() causes a condition to operate in an analogous way to a simple lock, but notifying waiting threads when the lock is again available to acquire.
///
/// @param self A reference to the condition.
/// @param event The type of event to raise when the condition's lock is relinquished.
void HCConditionRelinquishRaisingEvent(HCConditionRef self, HCConditionEvent event);

/// Acquires a condition's lock blocking until it is available, executes a function, then relinquishes the lock.
///
/// This function operates similarly to an acquired execution operation on a simple lock.
///
/// @param self A reference to the condition.
/// @param function The function to execute when the condition's lock is acquired.
/// @param context A context value to pass unmodified to @c function.
void* HCConditionExecuteAcquired(HCConditionRef self, HCConditionExecuteAcquiredFunction function, void* context);

/// Acquires a condition's lock blocking until it is available, executes a function, raises a signal notifying other threads that the condition's lock is nearly available to acquire, then relinquishes the lock.
///
/// This function allows a caller to safely multi-thread execute a function and notify one or more waiting threads of the completed execution.
///
/// @param self A reference to the condition.
/// @param function The function to execute when the condition's lock is acquired.
/// @param context A context value to pass unmodified to @c function.
void* HCConditionExecuteRaisingEventAcquired(HCConditionRef self, HCConditionExecuteAcquiredFunction function, void* context, HCConditionEvent event);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Events
//----------------------------------------------------------------------------------------------------------------------------------

/// Signals a single thread waiting on a condition.
///
/// This function ignores of the state of the condition's lock.
///
/// @param self A reference to the condition.
void HCConditionSignal(HCConditionRef self);

/// Broadcasts to all threads waiting on a condition.
///
/// This function ignores of the state of the condition's lock.
///
/// @param self A reference to the condition.
void HCConditionBroadcast(HCConditionRef self);

/// Raises an event on threads waiting on a condition.
///
/// This function ignores of the state of the condition's lock.
///
/// @param self A reference to the condition.
/// @param event The type of event to raise.
void HCConditionRaiseEvent(HCConditionRef self, HCConditionEvent event);

/// Raises an event on threads waiting on a condition in a lock-acquired way.
///
/// Before raising the event, the lock associated with the condition is acquired.
/// The event is then raised, signaling thread or threads waiting on the condition.
/// Once the event has been raised, the condition's lock is relinquished.
///
/// @param self A reference to the condition.
/// @param event The type of event to raise.
void HCConditionRaiseEventAcquired(HCConditionRef self, HCConditionEvent event);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Waiting
//----------------------------------------------------------------------------------------------------------------------------------

/// Places the current thread into a wait state on a condition until the condition is signaled.
///
/// This function places the calling thread into a blocking wait state on the condition until the condition is signaled.
/// The state of the condition's lock is not considered before, during, or after the wait.
/// For a lock-protected wait, see @c HCConditionWaitAcquired().
///
/// @param self A reference to the condition.
void HCConditionWait(HCConditionRef self);

/// Places the current thread into a wait state on a condition until the condition is signaled in a lock-acquired way.
///
/// This function places the calling thread into a blocking wait state on the condition until the condition is signaled.
/// Before starting the wait, the condition's associated lock is acquired, and will be reqlinquished when the wait completes.
/// Because the caller has acquired the condition's lock, the condition must be signaled using @c HCConditionSignal() or a related non-acquiring function, or a deadlock will result.
///
/// @param self A reference to the condition.
void HCConditionWaitAcquired(HCConditionRef self);

/// Executes a function when a condition is signaled.
///
/// This function places the calling thread into a blocking wait state on the condition until the condition is signaled.
/// When the condition is signaled, @c function is executed.
/// The state of the condition's associated lock is not considered before, during, or after the wait, or as part of the execution of @c function.
/// For a lock-protected wait-execute, see @c HCConditionWaitThenExecuteAcquired().
///
/// @param self A reference to the condition.
/// @param function The function to execute when the condition is signaled.
/// @param context A context value to pass unmodified to @c function.
/// @returns The return value of @c function.
void* HCConditionWaitThenExecute(HCConditionRef self, HCConditionExecuteAcquiredFunction function, void* context);

/// Executes a function when a condition is signaled in a lock-acquired way.
///
/// This function places the calling thread into a blocking wait state on the condition until the condition is signaled.
/// Before starting the wait, the condition's associated lock is acquired.
/// When the condition is signaled, @c function is executed.
/// After @c function finishes executing, the condition's associated lock is acquired.
/// Because the caller has acquired the condition's lock, the condition must be signaled using @c HCConditionSignal() or a related non-acquiring function, or a deadlock will result.
///
/// @param self A reference to the condition.
/// @param function The function to execute when the condition is signaled.
/// @param context A context value to pass unmodified to @c function.
/// @returns The return value of @c function.
void* HCConditionWaitThenExecuteAcquired(HCConditionRef self, HCConditionExecuteAcquiredFunction function, void* context);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Waiting with Timeout
//----------------------------------------------------------------------------------------------------------------------------------

/// Places the current thread into a wait state on a condition until the condition is signaled or a timeout elapses.
/// @see @c HCConditionWait().
/// @param self A reference to the condition.
/// @param timeout The timeout value in seconds.
/// @returns @c false if the condition was signaled before the timeout elapsed, or @c true if the timeout elapsed before the condition was signaled.
HCBoolean HCConditionWaitTimeout(HCConditionRef self, HCReal timeout);

/// Places the current thread into a wait state on a condition until the condition is signaled or a timeout elapses in a lock-acquired way.
/// @see @c HCConditionWaitAcquired().
/// @param self A reference to the condition.
/// @param timeout The timeout value in seconds.
/// @returns @c false if the condition was signaled before the timeout elapsed, or @c true if the timeout elapsed before the condition was signaled.
HCBoolean HCConditionWaitTimeoutAcquired(HCConditionRef self, HCReal timeout);

/// Executes a function when a condition is signaled or a timeout elapses.
/// @see @c HCConditionWaitThenExecute().
/// @param self A reference to the condition.
/// @param timeout The timeout value in seconds.
/// @param function The function to execute when the condition is signaled.
/// @param context A context value to pass unmodified to @c function.
/// @param result A pointer to a pointer-sized buffer to receive the result of @c function.
/// @param didTimeout A pointer to be populated with @c false if the condition was signaled before the timeout elapsed, or @c true if the timeout elapsed before the condition was signaled.
void HCConditionWaitTimeoutThenExecute(HCConditionRef self, HCReal timeout, HCConditionExecuteAcquiredFunction function, void* context, void** result, HCBoolean* didTimeout);

/// Executes a function when a condition is signaled or a timeout elapses in a lock-acquired way.
/// @see @c HCConditionWaitThenExecuteAcquired().
/// @param self A reference to the condition.
/// @param timeout The timeout value in seconds.
/// @param function The function to execute when the condition is signaled.
/// @param context A context value to pass unmodified to @c function.
/// @param result A pointer to a pointer-sized buffer to receive the result of @c function.
/// @param didTimeout A pointer to be populated with @c false if the condition was signaled before the timeout elapsed, or @c true if the timeout elapsed before the condition was signaled.
void HCConditionWaitTimeoutThenExecuteAcquired(HCConditionRef self, HCReal timeout, HCConditionExecuteAcquiredFunction function, void* context, void** result, HCBoolean* didTimeout);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Waiting with Timeout Query
//----------------------------------------------------------------------------------------------------------------------------------

/// Places the current thread into a wait state on a condition until the condition is signaled or a timeout elapses, querying if the wait should continue.
/// @see @c HCConditionWait().
/// @param self A reference to the condition.
/// @param waitWhile The function to execute before the wait is started, and each time the wait interval elapses while waiting for the condition to be signaled, to determine if the wait should be performed.
/// @param context A context value to pass unmodified to @c waitWhile.
/// @param waitIntervalDuration The interval value in seconds that should be waited between calls to @c waitWhile.
void HCConditionWaitWhile(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* context, HCReal waitIntervalDuration);

/// Places the current thread into a wait state on a condition until the condition is signaled or a timeout elapses in a lock-acquired way.
/// @see @c HCConditionWaitAcquired().
/// @param self A reference to the condition.
/// @param waitWhile The function to execute before the wait is started, and each time the wait interval elapses while waiting for the condition to be signaled, to determine if the wait should be performed.
/// @param context A context value to pass unmodified to @c waitWhile.
/// @param waitIntervalDuration The interval value in seconds that should be waited between calls to @c waitWhile.
void HCConditionWaitWhileAcquired(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* context, HCReal waitIntervalDuration);

/// Executes a function when a condition is signaled or a timeout elapses.
/// @see @c HCConditionWaitThenExecute().
/// @param self A reference to the condition.
/// @param waitWhile The function to execute before the wait is started, and each time the wait interval elapses while waiting for the condition to be signaled, to determine if the wait should be performed.
/// @param waitWhileContext A context value to pass unmodified to @c waitWhile.
/// @param waitIntervalDuration The interval value in seconds that should be waited between calls to @c waitWhile.
/// @param function The function to execute when the condition is signaled.
/// @param functionContext A context value to pass unmodified to @c function.
/// @returns The return value of @c function.
void* HCConditionWaitWhileThenExecute(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* waitWhileContext, HCReal waitIntervalDuration, HCConditionExecuteAcquiredFunction function, void* functionContext);

/// Executes a function when a condition is signaled or a timeout elapses in a lock-acquired way.
/// @see @c HCConditionWaitThenExecuteAcquired().
/// @param self A reference to the condition.
/// @param waitWhile The function to execute before the wait is started, and each time the wait interval elapses while waiting for the condition to be signaled, to determine if the wait should be performed.
/// @param waitWhileContext A context value to pass unmodified to @c waitWhile.
/// @param waitIntervalDuration The interval value in seconds that should be waited between calls to @c waitWhile.
/// @param function The function to execute when the condition is signaled.
/// @param functionContext A context value to pass unmodified to @c function.
/// @returns The return value of @c function.
void* HCConditionWaitWhileThenExecuteAcquired(HCConditionRef self, HCConditionWaitWhileFunction waitWhile, void* waitWhileContext, HCReal waitIntervalDuration, HCConditionExecuteAcquiredFunction function, void* functionContext);

#endif /* HCCondition_h */
