//
//  HCPrimitive.h
//  HollowCore
//
//  Created by Matt Stoker on 3/5/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCPrimitive_h
#define HCPrimitive_h

#include "../Core/HCObject.h"
#include "HCRay.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef HCBoolean (*HCPrimitiveIntersectFunction)(HCRef self, HCRay ray);

typedef const struct HCPrimitiveTypeData {
    HCObjectTypeData base;
    HCPrimitiveIntersectFunction intersect;
} HCPrimitiveTypeData;
extern HCType HCPrimitiveType;

typedef struct HCPrimitive* HCPrimitiveRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
// NOTE: Instances of HCPrimitive should be created using sub-types

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Primitive Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCPrimitiveIntersect(HCPrimitiveRef self, HCRay ray);

#endif /* HCPrimitive_h */
