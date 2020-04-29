///
/// @file HCList_Internal.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 1/21/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCList_Internal_h
#define HCList_Internal_h

#include "../Core/HCObject_Internal.h"
#include "HCList.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCList {
    HCObject base;
    HCInteger count;
    HCInteger capacity;
    HCRef* objects;
} HCList;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCListInit(void* memory, HCInteger capacity);
void HCListDestroy(HCListRef self);

#endif /* HCList_Internal_h */
