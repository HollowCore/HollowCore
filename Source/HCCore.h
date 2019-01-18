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

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value Types
//----------------------------------------------------------------------------------------------------------------------------------
typedef bool HCBoolean;
typedef int64_t HCInteger;
typedef double HCReal;

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
    HCType ancestor;
    HCTypeName name;
} HCTypeData;
typedef void* HCRef;

#endif /* HCCore_h */
