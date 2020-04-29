///
/// @file HCJSON.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 2/17/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCJSON_h
#define HCJSON_h

#include "../Data/HCData.h"
#include "../Data/HCNumber.h"
#include "../Data/HCString.h"
#include "../Container/HCList.h"
#include "../Container/HCMap.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef HCRef HCJSONValueRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef enum HCJSONValueType {
    HCJSONValueTypeUnknown = 0,
    HCJSONValueTypeNull,
    HCJSONValueTypeFalse,
    HCJSONValueTypeTrue,
    HCJSONValueTypeNumber,
    HCJSONValueTypeString,
    HCJSONValueTypeArray,
    HCJSONValueTypeObject,
    HCJSONValueTypeCount
} HCJSONValueType;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCRef HCJSONValueCreateWithData(HCDataRef data);
HCDataRef HCJSONValueAsDataRetained(HCJSONValueRef value);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Query
//----------------------------------------------------------------------------------------------------------------------------------
HCJSONValueType HCJSONValueTypeForObject(HCJSONValueRef object);

#endif /* HCJSON_h */
