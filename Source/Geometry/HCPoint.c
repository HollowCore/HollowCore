//
//  HCPoint.c
//  HollowCore
//
//  Created by Matt Stoker on 10/19/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCPoint.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCPointMake(HCReal x, HCReal y) {
    return (HCPoint){.x = x, .y = y};
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPointIsInvalid(HCPoint point) {
    return isnan(point.x) || isnan(point.y);
}

HCBoolean HCPointIsSimilar(HCPoint point, HCPoint other, HCReal dimensionDissimilarity) {
    return
        HCRealIsSimilar(point.x, other.x, dimensionDissimilarity) &&
        HCRealIsSimilar(point.y, other.y, dimensionDissimilarity);
}

HCBoolean HCPointIsEqual(HCPoint point, HCPoint other) {
    return
        HCRealIsEqual(point.x, other.x) &&
        HCRealIsEqual(point.y, other.y);
}

HCInteger HCPointHashValue(HCPoint point) {
    return
        HCRealHashValue(point.x) ^
        HCRealHashValue(point.y);
}

void HCPointPrint(HCPoint point, FILE* stream) {
    fprintf(stream, "<x:%f,y:%f>", point.x, point.y);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Queries
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPointIsZero(HCPoint point) {
    return HCPointIsEqual(point, HCPointZero);
}

HCBoolean HCPointIsInfinite(HCPoint point) {
    return isinf(point.x) || isinf(point.y);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCPointOffset(HCPoint point, HCReal dx, HCReal dy) {
    return HCPointMake(point.x + dx, point.y + dy);
}

HCPoint HCPointScale(HCPoint point, HCReal scalar) {
    return HCPointMake(point.x * scalar, point.y * scalar);
}

HCPoint HCPointInterpolate(HCPoint point, HCPoint other, HCReal t) {
    HCReal tc = 1.0 - t;
    return HCPointMake(tc * point.x + t * other.x, tc * point.y + t * other.y);
}
