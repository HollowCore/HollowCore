///
/// @file HCLock.h
/// @ingroup HollowCore
///
/// @author Braden Scothern
/// @date 4/12/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCLock_h
#define HCLock_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------

/// Type of @c HCLock instances.
extern HCType HCLockType;

/// A reference to an @c HCLock instance.
typedef struct HCLock* HCLockRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------

/// Function executed while a lock is acquired using @c HCLockExecuteAcquired().
/// @param context The context value passed to @c HCLockExecuteAcquired().
/// @returns The value to be returned from @c HCLockExecuteAcquired().
typedef void* (HCLockExecuteAcquiredFunction)(void* context);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------

/// Creates a lock object.
/// @returns A reference to a lock object that may be subsequently acquired.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCLockRef HCLockCreate(void);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a lock represents the same underlying locking mechanism as another lock.
/// @param self A reference to the lock to examine.
/// @param other The other lock to evaluate equality against.
/// @returns @c true if @c self and @c other represent the same internal locking mechanism and would both be acquired if one was acquired.
HCBoolean HCLockIsEqual(HCLockRef self, HCLockRef other);

/// Calculates a hash value for a lock.
/// @param self A reference to the lock.
/// @returns A hash value determined using only the internal locking mechanism of the lock.
HCInteger HCLockHashValue(HCLockRef self);

/// Prints a lock to a stream.
/// @param self A reference to the lock.
/// @param stream The stream to which the lock should be printed.
void HCLockPrint(HCLockRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Locking and Unlocking
//----------------------------------------------------------------------------------------------------------------------------------

/// Acquires a lock, blocking until it is available.
/// @param self A reference to the lock.
void HCLockAquire(HCLockRef self);

/// Acquires a lock, returning immediately if it is not available.
/// @param self A reference to the lock.
/// @returns @c true if the lock was acquired, or @c false if it was not available.
HCBoolean HCLockTryAquire(HCLockRef self);

/// Relinquishes a lock, allowing other threads to acquire it.
/// @param self A reference to the lock.
void HCLockRelinquish(HCLockRef self);

/// Acquires a lock, blocking until it is available, executes a function, then relinquishes the lock.
/// @param self A reference to the lock.
/// @param function The function to execute when the lock is acquired.
/// @param context A context value to pass unmodified to @c function.
void* HCLockExecuteAcquired(HCLockRef self, HCLockExecuteAcquiredFunction function, void* context);

#endif /* HCLock_h */
