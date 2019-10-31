//
//  HCSphere.c
//  HollowCore
//
//  Created by Matt Stoker on 3/5/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCSphere_Internal.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCPrimitiveTypeData HCSphereTypeDataInstance = {
    .base = {
        .base = {
            .ancestor = (HCType)&HCPrimitiveTypeDataInstance,
            .name = "HCSphere",
        },
        .isEqual = (void*)HCSphereIsEqual,
        .hashValue = (void*)HCSphereHashValue,
        .print = (void*)HCSpherePrint,
        .destroy = (void*)HCSphereDestroy,
    },
    .intersect = (void*)HCSphereIntersect,
};
HCType HCSphereType = (HCType)&HCSphereTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCSphereRef HCSphereCreate(HCVector center, HCReal radius) {
    HCSphereRef self = calloc(sizeof(HCSphere), 1);
    HCSphereInit(self, center, radius);
    return self;
}

void HCSphereInit(void* memory, HCVector center, HCReal radius) {
    HCPrimitiveInit(memory);
    HCSphereRef self = memory;
    self->base.base.type = HCSphereType;
    self->center = center;
    self->radius = radius;
}

void HCSphereDestroy(HCSphereRef self) {
    (void)self; // Unused
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSphereIsEqual(HCSphereRef self, HCSphereRef other) {
    return
        HCVectorIsEqual(self->center, other->center) &&
        HCRealIsEqual(self->radius, other->radius);
}

HCInteger HCSphereHashValue(HCSphereRef self) {
    return
        HCVectorHashValue(self->center) ^
        HCRealHashValue(self->radius);
}

void HCSpherePrint(HCSphereRef self, FILE* stream) {
    HCObjectPrint((HCObjectRef)self, stream);
    fprintf(stream, "(");
    fprintf(stream, "c:");
    HCVectorPrint(self->center, stream);
    fprintf(stream, ",");
    fprintf(stream, "r:");
    HCRealPrint(self->radius, stream);
    fprintf(stream, ")");
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Primitive Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCSphereIntersect(HCSphereRef self, HCRay ray) {
    HCVector o = HCVectorSubtract(ray.origin, HCSphereCenter(self));
    HCReal a = HCVectorDot(ray.direction, ray.direction);
    HCReal b = HCVectorDot(o, ray.direction) * 2.0;
    HCReal c = HCVectorDot(o, o) - HCSphereRadius(self) * HCSphereRadius(self);
    
    HCReal discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0.0f) {
        return NAN;
    }
    
    HCReal sqrtDiscriminant = sqrt(discriminant);
    HCReal rootMax = (-b + sqrtDiscriminant) / (2.0 * a);
    HCReal rootMin = (-b - sqrtDiscriminant) / (2.0 * a);
    
    if (rootMin > 0.000001) {
//        HCRayAddIntersection(ray, self, rootMin);
        return rootMin;
    }
    if (rootMax > 0.000001) {
//        HCRayAddIntersection(ray, self, rootMax);
        return rootMax;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCVector HCSphereCenter(HCSphereRef self) {
    return self->center;
}

HCReal HCSphereRadius(HCSphereRef self) {
    return self->radius;
}
