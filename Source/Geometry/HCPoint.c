///
/// @file HCPoint.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 10/19/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCPoint.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
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
HCPoint HCPointTranslate(HCPoint point, HCReal tx, HCReal ty) {
    return HCPointMake(point.x + tx, point.y + ty);
}

HCPoint HCPointScale(HCPoint point, HCReal sx, HCReal sy) {
    return HCPointMake(point.x * sx, point.y * sy);
}

HCPoint HCPointRotate(HCPoint point, HCReal cosAngle, HCReal sinAngle) {
    HCReal x = cosAngle * point.x - sinAngle * point.y;
    HCReal y = sinAngle * point.x + cosAngle * point.y;
    return HCPointMake(x, y);
}

HCPoint HCPointInterpolate(HCPoint point, HCPoint other, HCReal t) {
    HCReal tc = 1.0 - t;
    return HCPointMake(tc * point.x + t * other.x, tc * point.y + t * other.y);
}

HCReal HCPointDistance(HCPoint point, HCPoint other) {
    HCReal dx = point.x - other.x;
    HCReal dy = point.y - other.y;
    return sqrt(dx * dx + dy * dy);
}
