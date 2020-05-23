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

/// A reference to an instance of a JSON value type: @c HCNumber, @c HCString, @c HCList, or @c HCMap.
typedef HCRef HCJSONValueRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------

/// A JSON data type: one of @c null, @c false, @c true, @c number, @c string, @c array, or @c object.
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

/// Parses a JSON text to create an @c HCJSONValue.
/// @param data A data object containing a JSON text in UTF-8 format. See http://www.json.org.
/// @returns An object that is one of the JSON value types. Use @c HCJSONValueTypeForObject() or @c HCObjectTypeOf() to determine the specific type of the returned object.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCJSONValueRef HCJSONValueCreateWithData(HCDataRef data);

/// Produces a JSON text representing an @c HCJSONValue.
/// @param value A reference to the JSON value to convert.
/// @returns A data object containing a representation of @c value as a UTF-8 encoded JSON text. See http://www.json.org.
HCDataRef HCJSONValueAsDataRetained(HCJSONValueRef value);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Query
//----------------------------------------------------------------------------------------------------------------------------------

/// Examines a JSON value to determine its specific type.
/// @param value A reference to the JSON value to examine.
/// @returns The JSON data type of the value. @c value is guaranteed to be an instance of @c HCNumber if the returned type is @c HCJSONValueTypeFalse, @c HCJSONValueTypeTrue, or @c HCJSONValueTypeNumber. @c value is an instance of @c HCString if the returned type is @c HCJSONValueTypeString. @c value is an instance of @c HCList if the returned type is @c HCJSONValueTypeArray. @c value is an instance of @c HCMap if the returned type is @c HCJSONValueTypeObject. Otherwise, @c value is not an @c HCJSONValue.
HCJSONValueType HCJSONValueTypeForObject(HCJSONValueRef value);

#endif /* HCJSON_h */
