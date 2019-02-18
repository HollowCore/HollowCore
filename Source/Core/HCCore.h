//
//  HCCore.h
//  HollowCore
//
//  Created by Matt Stoker on 12/28/18.
//  Copyright © 2018 HollowCore. All rights reserved.
//

#ifndef HCCore_h
#define HCCore_h

#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value Types
//----------------------------------------------------------------------------------------------------------------------------------
typedef bool HCBoolean;
typedef int64_t HCInteger;
typedef double HCReal;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Extents
//----------------------------------------------------------------------------------------------------------------------------------
#define HCIntegerMin (INT64_MIN)
#define HCIntegerMax (INT64_MAX)

#define HCRealMin (-DOUBLE_MAX)
#define HCRealMax (DOUBLE_MAX)

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Atomic Value Types
//----------------------------------------------------------------------------------------------------------------------------------
typedef _Atomic HCBoolean HCAtomicBoolean;
typedef _Atomic HCInteger HCAtomicInteger;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Types
//----------------------------------------------------------------------------------------------------------------------------------
typedef uint8_t HCByte;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Boolean Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCBooleanIsEqual(HCBoolean self, HCBoolean other);
HCInteger HCBooleanHashValue(HCBoolean self);
void HCBooleanPrint(HCBoolean self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Integer Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCIntegerIsEqual(HCInteger self, HCInteger other);
HCInteger HCIntegerHashValue(HCInteger self);
void HCIntegerPrint(HCInteger self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Real Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRealIsEqual(HCReal self, HCReal other);
HCInteger HCRealHashValue(HCReal self);
void HCRealPrint(HCReal self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Foundation
//----------------------------------------------------------------------------------------------------------------------------------
typedef const char* const HCTypeName;
typedef const struct HCTypeData* HCType;
typedef const struct HCTypeData {
    HCTypeName name;
    HCType ancestor;
} HCTypeData;
typedef void* HCRef;

#endif /* HCCore_h */
