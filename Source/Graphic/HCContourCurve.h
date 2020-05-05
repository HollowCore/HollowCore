///
/// @file HCContourCurve.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 2/11/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCContourCurve_h
#define HCContourCurve_h

#include "../Geometry/HCRectangle.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCContourCurve {
    HCPoint c0;
    HCPoint c1;
    HCPoint p;
} HCContourCurve;

typedef enum HCContourCurveType {
    HCContourCurveTypePoint,
    HCContourCurveTypeLinear,
    HCContourCurveTypeSimple,
    HCContourCurveTypeSingleInflection,
    HCContourCurveTypeDoubleInflection,
    HCContourCurveTypeLoop,
    HCContourCurveTypeLoopAtStart,
    HCContourCurveTypeLoopAtEnd,
    HCContourCurveTypeCusp,
} HCContourCurveType;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contour Curve Special Values
//----------------------------------------------------------------------------------------------------------------------------------
#define HCContourCurveInvalidStatic ((HCContourCurve){.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = HCPointInvalidStatic})
#define HCContourCurveZeroStatic    ((HCContourCurve){.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = HCPointZeroStatic})

static const HCContourCurve HCContourCurveInvalid = HCContourCurveInvalidStatic;
static const HCContourCurve HCContourCurveZero = HCContourCurveZeroStatic;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCContourCurve HCContourCurveMakeLinear(HCPoint p);
HCContourCurve HCContourCurveMakeQuadratic(HCPoint c, HCPoint p);
HCContourCurve HCContourCurveMakeCubic(HCPoint c0, HCPoint c1, HCPoint p);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourCurveIsInvalid(HCContourCurve curve);
HCBoolean HCContourCurveIsLinear(HCContourCurve curve);
HCBoolean HCContourCurveIsQuadratic(HCContourCurve curve);
HCBoolean HCContourCurveIsCubic(HCContourCurve curve);
HCBoolean HCContourCurveIsSimilar(HCContourCurve curve, HCContourCurve other, HCReal axisDissimilarity);
HCBoolean HCContourCurveIsZero(HCContourCurve curve);
HCBoolean HCContourCurveIsInfinite(HCContourCurve curve);
HCBoolean HCContourCurveIsEqual(HCContourCurve curve, HCContourCurve other);
HCInteger HCContourCurveHashValue(HCContourCurve curve);
void HCContourCurvePrint(HCContourCurve curve, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCContourCurve HCContourCurveAsLinear(HCContourCurve curve);
HCContourCurve HCContourCurveAsQuadratic(HCContourCurve curve);
HCContourCurve HCContourCurveAsCubic(HCContourCurve curve);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourCurveValue(HCPoint p0, HCContourCurve curve, HCReal t);
HCPoint HCContourCurveValueLinear(HCPoint p0, HCPoint p1, HCReal t);
HCPoint HCContourCurveValueQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t);
HCPoint HCContourCurveValueCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Evaluation
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveEvaluate(HCPoint p0, HCContourCurve curve, HCReal t, HCReal* x, HCReal* y, HCReal* dx, HCReal* dy, HCReal* ddx, HCReal* ddy);
void HCContourCurveEvaluateLinear(HCPoint p0, HCPoint p1, HCReal t, HCReal* x, HCReal* y);
void HCContourCurveEvaluateQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t, HCReal* x, HCReal* y, HCReal* dx, HCReal* dy);
void HCContourCurveEvaluateCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t, HCReal* x, HCReal* y, HCReal* dx, HCReal* dy, HCReal* ddx, HCReal* ddy);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Extrema
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveExtrema(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* extrema);
void HCContourCurveExtremaLinear(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* extrema);
void HCContourCurveExtremaQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* extrema);
void HCContourCurveExtremaCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* extrema);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Bounds
//----------------------------------------------------------------------------------------------------------------------------------
HCRectangle HCContourCurveBounds(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* extrema);
HCRectangle HCContourCurveBoundsLinear(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* extrema);
HCRectangle HCContourCurveBoundsQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* extrema);
HCRectangle HCContourCurveBoundsCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* extrema);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Length
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCContourCurveLength(HCPoint p0, HCContourCurve curve);
HCReal HCContourCurveLengthLinear(HCPoint p0, HCPoint p1);
HCReal HCContourCurveLengthQuadratic(HCPoint p0, HCPoint c, HCPoint p1);
HCReal HCContourCurveLengthCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Intersection
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveIntersection(HCPoint p0, HCContourCurve pCurve, HCPoint q0, HCContourCurve qCurve, HCReal* t, HCReal* u);
void HCContourCurveLinearLinearIntersection(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint q1, HCReal* t, HCReal* u);
void HCContourCurveLinearQuadraticIntersection(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint qc, HCPoint q1, HCReal* t, HCReal* u);
void HCContourCurveLinearCubicIntersection(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCReal* t, HCReal* u);
void HCContourCurveQuadraticQuadraticIntersection(HCPoint p0, HCPoint pc, HCPoint p1, HCPoint q0, HCPoint qc, HCPoint q1, HCReal* t, HCReal* u);
void HCContourCurveQuadraticCubicIntersection(HCPoint p0, HCPoint pc, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCReal* t, HCReal* u);
void HCContourCurveCubicCubicIntersection(HCPoint p0, HCPoint pc0, HCPoint pc1, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCReal* t, HCReal* u);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Canonical Forms
//----------------------------------------------------------------------------------------------------------------------------------

HCContourCurve HCContourCurveAlign(HCPoint p0, HCContourCurve curve);
HCPoint HCContourCurveCanonical(HCPoint p0, HCContourCurve curve);
HCContourCurveType HCContourCurveCanonicalType(HCPoint p0, HCContourCurve curve);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Split
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveSplit(HCPoint p0, HCContourCurve curve, HCContourCurve* sCurve, HCContourCurve* eCurve);

#endif /* HCContourCurve_h */
