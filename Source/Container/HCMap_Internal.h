//
//  HCMap_Internal.h
//  HollowCore
//
//  Created by Matt Stoker on 2/2/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCMap_Internal_h
#define HCMap_Internal_h

#include "../Core/HCObject_Internal.h"
#include "HCMap.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCMap {
    HCObject base;
    HCSetRef keys;
    HCSetRef pairs;
} HCMap;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCMapInit(void* memory, HCInteger capacity);
void HCMapDestroy(HCMapRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Pair Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCMapPair {
    HCObject base;
    HCRef key;
    HCRef object;
} HCMapPair;
extern HCType HCMapPairType;
typedef struct HCMapPair* HCMapPairRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Pair Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCMapPairRef HCMapPairCreate(HCRef key, HCRef object);
void HCMapPairInit(void* memory, HCRef key, HCRef object);
void HCMapPairDestroy(HCMapPairRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Pair Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCMapPairIsEqual(HCMapPairRef self, HCMapPairRef other);
HCInteger HCMapPairHashValue(HCMapPairRef self);

#endif /* HCMap_Internal_h */
