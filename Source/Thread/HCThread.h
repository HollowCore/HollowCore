///
/// @file HCThread.h
/// @package HollowCore
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

/// The @c HCType that describes a @c HCThread.
extern HCType HCThreadType;

/// The Ref type of @c HCThread.
typedef struct HCThread* HCThreadRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------

/// The function that is run by a @c HCThread.
///
/// @param context The @c context value provided at the creation of the @c HCThread calling this function.
typedef void (*HCThreadFunction)(void* context);

/// The options that can be applied to a @c HCThread to change its behavior.
typedef enum HCThreadOption {
    /// The @a HCThreadOption value representing the absence of other options.
    HCThreadOptionNone                      = 0b0,

    /// When this option is set the context given to the @a HCThread is released when the thread is being drestoryed.
    ///
    /// Exclusive from @c HCThreadOptionFreeContextOnDestroy
    HCThreadOptionReleaseContextOnDestroy   = 0b1,

    /// When this option is set the context given to the @c HCThread should be freed when the thread is being destroyed.
    ///
    /// Exclusive from @c HCThreadOptionReleaseContextOnDestroy
    HCThreadOptionFreeContextOnDestroy      = 0b10,

    /// When this option is set the @c HCThread will be joined as part of its destrution.
    HCThreadOptionJoinOnDestroy             = 0b100,
    
    /// When this option is set the @c HCThread is sent a cancel signal before being destoryed.
    ///
    /// This option implies @c HCThreadOptionJoinOnDestroy
    ///
    /// See @c HCThreadCancel()
    HCThreadOptionCancelOnDestroy           = 0b1100,
} HCThreadOption;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------

/// Creates a @c HCThread with no options set.
///
/// @param function The function that is the entry point for the thread once it has started.
/// @param context The context to provide to the @c function when the thread starts.
/// @return A @c HCThread that will run the specified @c function when started.
HCThreadRef HCThreadCreate(HCThreadFunction function, void* context);

/// Creates a @c HCThread with options.
///
/// @param function The function that is the entry point for the thread once it has started.
/// @param context The context to provide to the @c function when the thread starts.
/// @param options A mask of @c HCThreadOption values that should be enabled on the thread.
/// @return A @c HCThread that will run the specified @c function when started.
HCThreadRef HCThreadCreateWithOptions(HCThreadFunction function, void* context, HCThreadOption options);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCThreadIsEqual(HCThreadRef self, HCThreadRef other);
HCInteger HCThreadHashValue(HCThreadRef self);
void HCThreadPrint(HCThreadRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Current Thread
//----------------------------------------------------------------------------------------------------------------------------------

/// Gets the currently running @c HCThread.
///
/// If this returns @c NULL then the current thread is not a @c HCThread.
HCThreadRef HCThreadGetCurrent(void);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Execution State
//----------------------------------------------------------------------------------------------------------------------------------

/// Starts a @c HCThread 's execution.
///
/// See @c HCThreadIsExecuting()
///
/// @param self The @c HCThread to start.
void HCThreadStart(HCThreadRef self);

/// Sets the canceled state for the @c HCThread.
///
/// See @c HCThreadIsCanceled()
///
/// @param self The @c HCThread to cancel.
void HCThreadCancel(HCThreadRef self);

/// Checks if the thread is currently running.
///
/// @param self The @c HCThread to inspect.
HCBoolean HCThreadIsExecuting(HCThreadRef self);

/// Checks if the thread is canceled.
///
/// @param self The @c HCThread to inspect.
HCBoolean HCThreadIsCanceled(HCThreadRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Management
//----------------------------------------------------------------------------------------------------------------------------------

/// Checks if the thread is joined.
///
/// @param self The @c HCThread to inspect.
HCBoolean HCThreadIsJoined(HCThreadRef self);

/// Joins the @c HCThread.
///
/// @param self The @c HCThread to join.
void HCThreadJoin(HCThreadRef self);

/// Gets the context pointer of the @c HCThread.
///
/// See @c HCThreadOptionReleaseContextOnDestroy and @c HCThreadOptionFreeContextOnDestroy to simplify management of the context.
///
/// @param self The @c HCThread to inspect.
void* HCThreadGetContext(HCThreadRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Options
//----------------------------------------------------------------------------------------------------------------------------------
HCThreadOption HCThreadGetOptions(HCThreadRef self);

#endif /* HCThread_h */
