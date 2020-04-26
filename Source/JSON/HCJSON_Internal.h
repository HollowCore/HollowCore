///
/// @file HCJSON_Internal.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 2/18/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCJSON_Internal_h
#define HCJSON_Internal_h

#include "HCJSON.h"
#include "json-parser/json.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCJSONValueRef HCJSONValueCreateWithJSONParserValue(json_value* jsonParserValue);
void HCJSONValueAppendToData(HCJSONValueRef value, HCDataRef data);

#endif /* HCJSON_Internal_h */
