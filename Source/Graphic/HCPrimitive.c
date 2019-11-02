//
//  HCPrimitive.c
//  HollowCore
//
//  Created by Matt Stoker on 3/5/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCPrimitive_Internal.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
HCPrimitiveTypeData HCPrimitiveTypeDataInstance = {
    .base = {
        .base = {
            .ancestor = NULL,
            .name = "HCPrimitive",
        },
        .isEqual = (void*)HCObjectIsEqual,
        .hashValue = (void*)HCObjectHashValue,
        .print = (void*)HCObjectPrint,
        .destroy = (void*)HCObjectDestroy,
    }
};
HCType HCPrimitiveType = &HCPrimitiveTypeDataInstance.base.base;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCPrimitiveInit(void* memory) {
    HCObjectInit(memory);
    HCPrimitiveRef self = memory;
    self->base.type = HCPrimitiveType;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Primitive Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCPrimitiveIntersect(HCPrimitiveRef self, HCRay ray) {
    (void)self; (void)ray; // Unused
    return NAN;
}
