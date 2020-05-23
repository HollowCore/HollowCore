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

#include "../Geometry/HCPoint.h"

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
#define HCContourCurveZeroStatic    ((HCContourCurve){.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = HCPointZeroStatic})

static const HCContourCurve HCContourCurveInvalid = HCContourCurveInvalidStatic;
static const HCContourCurve HCContourCurveZero = HCContourCurveZeroStatic;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contour Curve Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCContourCurve HCContourCurveMakeLinear(HCPoint p);
HCContourCurve HCContourCurveMakeQuadratic(HCPoint c, HCPoint p);
HCContourCurve HCContourCurveMakeCubic(HCPoint c0, HCPoint c1, HCPoint p);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contour Curve Equality
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
// MARK: - Contour Curve Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCContourCurve HCContourCurveAsLinear(HCContourCurve curve);
HCContourCurve HCContourCurveAsQuadratic(HCContourCurve curve);
HCContourCurve HCContourCurveAsCubic(HCContourCurve curve);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contour Curve Evaluation
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourEvaluateCurve(HCReal t, HCPoint p0, HCContourCurve curve, HCReal* dx, HCReal* dy);
HCPoint HCContourEvaluateLinearCurve(HCReal t, HCPoint p0, HCPoint p1, HCReal* dx, HCReal* dy);
HCPoint HCContourEvaluateQuadraticCurve(HCReal t, HCPoint p0, HCPoint c, HCPoint p1, HCReal* dx, HCReal* dy);
HCPoint HCContourEvaluateCubicCurve(HCReal t, HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal* dx, HCReal* dy);
void HCContourLineLineIntersection(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint q1, HCReal* t, HCReal* u);

#endif /* HCContourCurve_h */
