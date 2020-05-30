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
HCContourCurve HCContourCurveMakeWithCurve(HCCurve curve);
HCCurve HCCurveMakeWithContourCurve(HCPoint p0, HCContourCurve curve);

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

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveDerivative(HCPoint p0, HCContourCurve curve, HCPoint* dP0, HCContourCurve* dCurve);
HCContourCurve HCContourCurveTangent(HCPoint p0, HCContourCurve curve, HCReal t);
HCContourCurve HCContourCurveTangentUnit(HCPoint p0, HCContourCurve curve, HCReal t);
HCContourCurve HCContourCurveNormal(HCPoint p0, HCContourCurve curve, HCReal t);
HCContourCurve HCContourCurveNormalUnit(HCPoint p0, HCContourCurve curve, HCReal t);
HCReal HCContourCurveCurvature(HCPoint p0, HCContourCurve curve, HCReal t);
void HCContourCurveExtrema(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* extrema);
void HCContourCurveInflections(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* inflections);
HCRectangle HCContourCurveApproximateBounds(HCPoint p0, HCContourCurve curve);
HCRectangle HCContourCurveBounds(HCPoint p0, HCContourCurve curve);
HCReal HCContourCurveLength(HCPoint p0, HCContourCurve curve);
HCReal HCContourCurveParameterAtLength(HCPoint p0, HCContourCurve curve, HCReal d);
HCReal HCContourCurveParameterNearestPoint(HCPoint p0, HCContourCurve curve, HCPoint p);
HCReal HCContourCurveDistanceFromPoint(HCPoint p0, HCContourCurve curve, HCPoint p);
HCPoint HCContourCurveBaselineProjection(HCPoint p0, HCContourCurve curve, HCReal t);
HCContourCurve HCContourCurveInterpolatingPoint(HCPoint p0, HCPoint p1, HCPoint p, HCReal t, HCReal dx, HCReal dy);
HCContourCurve HCContourCurveMould(HCPoint p0, HCContourCurve curve, HCReal t, HCPoint p);
void HCContourCurveSplit(HCPoint p0, HCContourCurve curve, HCReal t, HCPoint* sp0, HCContourCurve* sCurve, HCPoint* ep0, HCContourCurve* eCurve);
HCContourCurve HCContourCurveXAxisAligned(HCPoint p0, HCContourCurve curve);
HCContourCurve HCContourCurveYAxisAligned(HCPoint p0, HCContourCurve curve);
void HCContourCurveIntersection(HCPoint p0, HCContourCurve pCurve, HCPoint q0, HCContourCurve qCurve, HCInteger* count, HCReal* t, HCReal* u);

#endif /* HCContourCurve_h */
