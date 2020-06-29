///
/// @file HCPoint.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 10/19/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCPoint_h
#define HCPoint_h

#include "../Core/HCCore.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCPoint {
    HCReal x;
    HCReal y;
} HCPoint;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Special Values
//----------------------------------------------------------------------------------------------------------------------------------
#define HCPointInvalidStatic   ((HCPoint){.x = NAN, .y = NAN})
#define HCPointZeroStatic      ((HCPoint){.x = 0.0, .y = 0.0})

static const HCPoint HCPointInvalid = HCPointInvalidStatic;
static const HCPoint HCPointZero = HCPointZeroStatic;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCPointMake(HCReal x, HCReal y);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPointIsInvalid(HCPoint point);
HCBoolean HCPointIsSimilar(HCPoint point, HCPoint other, HCReal axisDissimilarity);
HCBoolean HCPointIsEqual(HCPoint point, HCPoint other);
HCInteger HCPointHashValue(HCPoint point);
void HCPointPrint(HCPoint point, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Queries
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPointIsZero(HCPoint point);
HCBoolean HCPointIsInfinite(HCPoint point);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCPointTranslate(HCPoint point, HCReal tx, HCReal ty);
HCPoint HCPointScale(HCPoint point, HCReal sx, HCReal sy);
HCPoint HCPointRotate(HCPoint point, HCReal cosAngle, HCReal sinAngle);
HCPoint HCPointInterpolate(HCPoint point, HCPoint other, HCReal t);
HCReal HCPointDistance(HCPoint point, HCPoint other);

#endif /* HCPoint_h */
