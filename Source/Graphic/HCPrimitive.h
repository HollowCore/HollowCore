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

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef HCBoolean (*HCPrimitiveIntersectFunction)(HCRef self, HCRef other);

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
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPrimitiveIntersect(HCPrimitiveRef self, HCPrimitiveRef other);

#endif /* HCPrimitive_h */
