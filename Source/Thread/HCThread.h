///
/// @file HCThread.h
/// @ingroup HollowCore
///
/// @author Braden Scothern
/// @date 2/14/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCThread_h
#define HCThread_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------

/// Type of @c HCThread instances.
extern HCType HCThreadType;

/// A reference to an @c HCThread instance.
typedef struct HCThread* HCThreadRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------

/// Function run by a thread.
/// @param context The @c context value provided at the creation of the thread calling this function.
typedef void (*HCThreadFunction)(void* context);

/// Options that can be applied to a thread to change its behavior.
typedef enum HCThreadOption {
    /// The @a HCThreadOption value representing the absence of other options.
    HCThreadOptionNone = 0b0,

    /// When this option is set the context given to the thread is released when the thread is being destroyed.
    ///
    /// Exclusive from @c HCThreadOptionFreeContextOnDestroy
    HCThreadOptionReleaseContextOnDestroy = 0b1,

    /// When this option is set the context given to the thread should be freed when the thread is being destroyed.
    ///
    /// Exclusive from @c HCThreadOptionReleaseContextOnDestroy
    HCThreadOptionFreeContextOnDestroy = 0b10,

    /// When this option is set the thread will be joined as part of its destrution.
    HCThreadOptionJoinOnDestroy = 0b100,
    
    /// When this option is set the thread is sent a cancel signal before being destoryed.
    ///
    /// This option implies @c HCThreadOptionJoinOnDestroy
    ///
    /// See @c HCThreadCancel()
    HCThreadOptionCancelOnDestroy = 0b1100,
} HCThreadOption;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------

/// Creates a thread with no options.
///
/// @param function The function that is the entry point for the thread.
/// @param context The context to provide to @c function when the thread is executed.
/// @return A reference to the created thread.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCThreadRef HCThreadCreate(HCThreadFunction function, void* context);

/// Creates a thread with options.
///
/// @param function The function that is the entry point for the thread.
/// @param context The context to provide to @c function when the thread is executed.
/// @param options A bitmask of @c HCThreadOption values that should be enabled on the thread.
/// @return A reference to the created thread.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCThreadRef HCThreadCreateWithOptions(HCThreadFunction function, void* context, HCThreadOption options);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a thread represents the same execution unit as another thread.
/// @param self A reference to the thread to examine.
/// @param other The other thread to evaluate equality against.
/// @returns @c true if @c self and @c other represent the same unit of execution.
HCBoolean HCThreadIsEqual(HCThreadRef self, HCThreadRef other);

/// Calculates a hash value for a thread.
/// @param self A reference to the thread.
/// @returns A hash value determined using only the thread context.
HCInteger HCThreadHashValue(HCThreadRef self);

/// Prints a thread to a stream.
/// @param self A reference to the thread.
/// @param stream The stream to which the thread should be printed.
void HCThreadPrint(HCThreadRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------

/// Obtains the context value of the thread.
/// @see @c HCThreadOptionReleaseContextOnDestroy and @c HCThreadOptionFreeContextOnDestroy to simplify management of the context.
/// @param self The thread to inspect.
/// @returns The thread context value provided when the thread was created.
void* HCThreadContext(HCThreadRef self);

/// Obtains the options set on the thread.
/// @param self The thread to inspect.
/// @returns The thread options provided when the thread was created.
HCThreadOption HCThreadOptions(HCThreadRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Current Thread
//----------------------------------------------------------------------------------------------------------------------------------

/// Obtains the currently executing thread.
///
/// @returns The thread of the calling execution context. Returns @c NULL if the current execution context is not an @c HCThread.
HCThreadRef HCThreadGetCurrent(void);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Execution State
//----------------------------------------------------------------------------------------------------------------------------------

/// Starts a thread 's execution.
/// @see @c HCThreadIsExecuting()
/// @param self The thread to start.
void HCThreadExecute(HCThreadRef self);

/// Marks thread as cancelled, notifying the executing thread function that it should terminate.
/// @see @c HCThreadIsCancelled()
/// @param self The thread to cancel.
void HCThreadCancel(HCThreadRef self);

/// Determines if the thread is currently executing.
/// @param self The thread to inspect.
HCBoolean HCThreadIsExecuting(HCThreadRef self);

/// Determines if the thread has been cancelled.
/// @param self The thread to inspect.
HCBoolean HCThreadIsCancelled(HCThreadRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Joining Threads
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if the thread has been joined to another thread.
/// @param self The thread to inspect.
HCBoolean HCThreadIsJoined(HCThreadRef self);

/// Joins the thread to the thread of the caller, blocking until this occurs.
/// @param self The thread to join.
void HCThreadJoin(HCThreadRef self);

#endif /* HCThread_h */
