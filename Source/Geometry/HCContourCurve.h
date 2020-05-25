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

#include "HCCurve.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCContourCurve {
    HCPoint c0;
    HCPoint c1;
    HCPoint p;
} HCContourCurve;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contour Curve Special Values
//----------------------------------------------------------------------------------------------------------------------------------

#define HCContourCurveInvalidStatic ((HCContourCurve){.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = HCPointInvalidStatic})
static const HCContourCurve HCContourCurveInvalid = HCContourCurveInvalidStatic;
#define HCContourCurveZeroStatic ((HCContourCurve){.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = HCPointZeroStatic})
static const HCContourCurve HCContourCurveZero = HCContourCurveZeroStatic;
#define HCContourCurvePointAxisDissimilarityStatic (DBL_EPSILON)
static const HCReal HCContourCurvePointAxisDissimilarity = HCContourCurvePointAxisDissimilarityStatic;

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
HCBoolean HCContourCurveIsSimilar(HCContourCurve curve, HCContourCurve other, HCReal axisDissimilarity);
HCBoolean HCContourCurveIsZero(HCContourCurve curve);
HCBoolean HCContourCurveIsInfinite(HCContourCurve curve);
HCBoolean HCContourCurveIsEqual(HCContourCurve curve, HCContourCurve other);
HCInteger HCContourCurveHashValue(HCContourCurve curve);
void HCContourCurvePrint(HCContourCurve curve, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Order
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourCurveIsLinear(HCPoint p0, HCContourCurve curve);
HCContourCurve HCContourCurveAsLinear(HCPoint p0, HCContourCurve curve);
HCBoolean HCContourCurveIsQuadratic(HCPoint p0, HCContourCurve curve);
HCContourCurve HCContourCurveAsQuadratic(HCPoint p0, HCContourCurve curve);
HCBoolean HCContourCurveIsCubic(HCPoint p0, HCContourCurve curve);
HCContourCurve HCContourCurveAsCubic(HCPoint p0, HCContourCurve curve);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Canonical Type
//----------------------------------------------------------------------------------------------------------------------------------
HCCurveType HCContourCurveCanonicalType(HCPoint p0, HCContourCurve curve);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourCurveValue(HCPoint p0, HCContourCurve curve, HCReal t);
void HCContourCurveEvaluate(HCPoint p0, HCContourCurve curve, HCReal t, HCReal* x, HCReal* y, HCReal* dx, HCReal* dy, HCReal* ddx, HCReal* ddy);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCContourCurve HCContourCurveXAxisAligned(HCPoint p0, HCContourCurve curve);
HCContourCurve HCContourCurveYAxisAligned(HCPoint p0, HCContourCurve curve);
void HCContourCurveSplit(HCPoint p0, HCContourCurve curve, HCReal t, HCPoint* sp0, HCContourCurve* sCurve, HCPoint* ep0, HCContourCurve* eCurve);
HCReal HCContourCurveCurvature(HCPoint p0, HCContourCurve curve, HCReal t);
void HCContourCurveDerivative(HCPoint p0, HCContourCurve curve, HCPoint* dP0, HCContourCurve* dCurve);
void HCContourCurveExtrema(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* extrema);
void HCContourCurveInflections(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* inflections);
HCRectangle HCContourCurveApproximateBounds(HCPoint p0, HCContourCurve curve);
HCRectangle HCContourCurveBounds(HCPoint p0, HCContourCurve curve);
HCReal HCContourCurveLength(HCPoint p0, HCContourCurve curve);
HCReal HCContourCurveParameter(HCPoint p0, HCContourCurve curve, HCReal d);
HCReal HCContourCurveParameterNearestPoint(HCPoint p0, HCContourCurve curve, HCPoint p);
HCPoint HCContourCurveProjection(HCPoint p0, HCContourCurve curve);
HCContourCurve HCContourCurveMould(HCPoint p0, HCContourCurve curve, HCPoint p, HCReal t);
HCContourCurve HCContourCurveInterpolating(HCPoint p0, HCPoint p1, HCPoint p, HCReal t);
void HCContourCurveFitting(HCInteger count, const HCPoint* points, HCPoint* p0, HCContourCurve* curve);
void HCContourCurveIntersection(HCPoint p0, HCContourCurve pCurve, HCPoint q0, HCContourCurve qCurve, HCInteger* count, HCReal* t, HCReal* u);

#endif /* HCContourCurve_h */
