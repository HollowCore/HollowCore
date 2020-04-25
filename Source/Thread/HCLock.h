///
/// @file HCLock.h
/// @package HollowCore
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

/// The @c HCType that describes a @c HCLock.
extern HCType HCLockType;

/// The Ref type of @c HCLock.
typedef struct HCLock* HCLockRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef void* (HCLockExecuteAcquiredFunction)(void* context);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCLockRef HCLockCreate(void);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCLockIsEqual(HCLockRef self, HCLockRef other);
HCInteger HCLockHashValue(HCLockRef self);
void HCLockPrint(HCLockRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Locking and Unlocking
//----------------------------------------------------------------------------------------------------------------------------------
void HCLockAquire(HCLockRef self);
HCBoolean HCLockTryAquire(HCLockRef self);
void HCLockRelinquish(HCLockRef self);
void* HCLockExecuteAcquired(HCLockRef self, HCLockExecuteAcquiredFunction function, void* context);

#endif /* HCLock_h */
