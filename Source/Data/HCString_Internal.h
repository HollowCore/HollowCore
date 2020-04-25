///
/// @file HCString_Internal.h
/// @package HollowCore
///
/// @author Matt Stoker
/// @date 1/17/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCString_Internal_h
#define HCString_Internal_h

#include "../Core/HCObject_Internal.h"
#include "HCString.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
struct HCString {
    HCObject base;
    HCInteger codeUnitCount;
    HCStringCodeUnit* codeUnits;
} HCString;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCStringInit(void* memory, HCInteger codeUnitCount, HCStringCodeUnit* codeUnits);
void HCStringInitWithoutCopying(void* memory, HCInteger codeUnitCount, HCStringCodeUnit* codeUnits);
void HCStringDestroy(HCStringRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCStringCodeUnitSequeceIsValid(const HCStringCodeUnit* source, HCInteger count);
void HCStringConvertCodeUnits(HCStringRef self, HCStringCodeUnit** sourceStart, HCStringCodeUnit* sourceEnd, HCStringCodePoint** targetStart, HCStringCodePoint* targetEnd);

#endif /* HCString_Internal_h */
