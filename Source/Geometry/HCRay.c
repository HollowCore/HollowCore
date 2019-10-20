//
//  HCRay.c
//  HollowCore
//
//  Created by Matt Stoker on 3/10/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCRay.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCRay HCRayMake(HCVector origin, HCVector direction) {
    return (HCRay){.origin = origin, .direction = direction};
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRayIsInvalid(HCRay ray) {
    return
        HCVectorIsInvalid(ray.origin) ||
        HCVectorIsInvalid(ray.direction);
}

HCBoolean HCRayIsSimilar(HCRay ray, HCRay other, HCReal axisDissimilarity) {
    return
        HCVectorIsSimilar(ray.origin, other.origin, axisDissimilarity) &&
        HCVectorIsSimilar(ray.direction, other.direction, axisDissimilarity);
}

HCBoolean HCRayIsEqual(HCRay ray, HCRay other) {
    return
        HCVectorIsEqual(ray.origin, other.origin) &&
        HCVectorIsEqual(ray.direction, other.direction);
}

HCInteger HCRayHashValue(HCRay ray) {
    return
        HCVectorHashValue(ray.origin) ^
        HCVectorHashValue(ray.direction);
}

void HCRayPrint(HCRay ray, FILE* stream) {
    fprintf(stream, "<");
    fprintf(stream, "o:");
    HCVectorPrint(ray.origin, stream);
    fprintf(stream, ",");
    fprintf(stream, "d:");
    HCVectorPrint(ray.direction, stream);
    fprintf(stream, ">");
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Queries
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRayIsZero(HCRay ray) {
    return HCRayIsEqual(ray, HCRayZero);
}

HCBoolean HCRayIsInfinite(HCRay ray) {
    return HCVectorIsInfinite(ray.direction);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCVector HCRayPointAt(HCRay ray, HCReal scale) {
    return HCVectorAdd(ray.origin, HCVectorScale(ray.direction, scale));
}
