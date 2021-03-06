///
/// @file HCObject_Internal.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 12/28/18
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCObject_Internal_h
#define HCObject_Internal_h

#include "HCObject.h"
#include <stdatomic.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef _Atomic HCBoolean HCAtomicBoolean;
typedef _Atomic HCInteger HCAtomicInteger;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCObjectTypeData HCObjectTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Instance Data
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCObject {
    HCType type;
    HCAtomicInteger referenceCount;
} HCObject;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCObjectInit(void* memory);
void HCObjectDestroy(HCObjectRef self);

void HCObjectSetType(void* object, HCType type);

#endif /* HCObject_Internal_h */
