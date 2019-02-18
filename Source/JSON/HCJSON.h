//
//  HCJSON.h
//  HollowCore
//
//  Created by Matt Stoker on 2/17/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

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
    HCJSONValueTypeNone,
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
HCJSONValueType HCJSONValueTypeForObject(HCRef object);

#endif /* HCJSON_h */
