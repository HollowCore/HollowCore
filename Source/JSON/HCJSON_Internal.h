//
//  HCJSON_Internal.h
//  HollowCore
//
//  Created by Matt Stoker on 2/18/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

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
