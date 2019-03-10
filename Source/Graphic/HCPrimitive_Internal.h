//
//  HCPrimitive_Internal.h
//  HollowCore
//
//  Created by Matt Stoker on 3/5/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCPrimitive_Internal_h
#define HCPrimitive_Internal_h

#include "HCPrimitive.h"
#include "../Core/HCObject_Internal.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCPrimitiveTypeData HCPrimitiveTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Instance Data
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCPrimitive {
    HCObject base;
} HCPrimitive;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCPrimitiveInit(void* memory);
void HCPrimitiveDestroy(HCPrimitiveRef self);

#endif /* HCPrimitive_Internal_h */
