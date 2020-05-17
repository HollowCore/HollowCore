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
    HCContourCurveTypeInvalid                   = 0b0000000000,
    HCContourCurveTypePoint                     = 0b0000000001,
    HCContourCurveTypeLinear                    = 0b0000000010,
    HCContourCurveTypeQuadratic                 = 0b0000000100,
    HCContourCurveTypeCubicSimple               = 0b0000001000,
    HCContourCurveTypeCubicSingleInflection     = 0b0000011000,
    HCContourCurveTypeCubicDoubleInflection     = 0b0000111000,
    HCContourCurveTypeCubicLoop                 = 0b0001001000,
    HCContourCurveTypeCubicLoopAtStart          = 0b0011001000,
    HCContourCurveTypeCubicLoopAtEnd            = 0b0101001000,
    HCContourCurveTypeCubicLoopClosed           = 0b0111001000,
    HCContourCurveTypeCubicCusp                 = 0b1001001000,
} HCContourCurveType;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contour Curve Special Values
//----------------------------------------------------------------------------------------------------------------------------------

#define HCContourCurveInvalidStatic ((HCContourCurve){.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = HCPointInvalidStatic})
static const HCContourCurve HCContourCurveInvalid = HCContourCurveInvalidStatic;
#define HCContourCurveZeroStatic    ((HCContourCurve){.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = HCPointZeroStatic})
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
// MARK: - Order / Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourCurveIsLinear(HCPoint p0, HCContourCurve curve);
HCContourCurve HCContourCurveAsLinear(HCPoint p0, HCContourCurve curve);
HCBoolean HCContourCurveIsQuadratic(HCPoint p0, HCContourCurve curve);
HCContourCurve HCContourCurveAsQuadratic(HCPoint p0, HCContourCurve curve);
HCBoolean HCContourCurveIsCubic(HCPoint p0, HCContourCurve curve);
HCContourCurve HCContourCurveAsCubic(HCPoint p0, HCContourCurve curve);
HCContourCurve HCContourCurveXAxisAligned(HCPoint p0, HCContourCurve curve);
HCContourCurve HCContourCurveYAxisAligned(HCPoint p0, HCContourCurve curve);
HCContourCurveType HCContourCurveCanonicalType(HCPoint p0, HCContourCurve curve);
HCPoint HCContourCurveCanonical(HCPoint p0, HCContourCurve curve);

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
// MARK: - Curvature
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCContourCurveCurvature(HCPoint p0, HCContourCurve curve, HCReal t);
HCReal HCContourCurveCurvatureLinear(HCPoint p0, HCPoint p1, HCReal t);
HCReal HCContourCurveCurvatureQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t);
HCReal HCContourCurveCurvatureCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Derivative
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveDerivative(HCPoint p0, HCContourCurve curve, HCPoint* dP0, HCContourCurve* dCurve);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Extrema
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveExtrema(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* extrema);
void HCContourCurveExtremaLinear(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* extrema);
void HCContourCurveExtremaQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* extrema);
void HCContourCurveExtremaCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* extrema);

void HCContourCurveInflections(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* inflections);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Bounds
//----------------------------------------------------------------------------------------------------------------------------------
HCRectangle HCContourCurveBounds(HCPoint p0, HCContourCurve curve);
HCRectangle HCContourCurveBoundsLinear(HCPoint p0, HCPoint p1);
HCRectangle HCContourCurveBoundsQuadratic(HCPoint p0, HCPoint c, HCPoint p1);
HCRectangle HCContourCurveBoundsCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Length
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCContourCurveLength(HCPoint p0, HCContourCurve curve, HCReal t);
HCReal HCContourCurveLengthLinear(HCPoint p0, HCPoint p1, HCReal t);
HCReal HCContourCurveLengthQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t);
HCReal HCContourCurveLengthCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameterization by Arc Length
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCContourCurveParameter(HCPoint p0, HCContourCurve curve, HCReal d);
HCReal HCContourCurveParameterLinear(HCPoint p0, HCPoint p1, HCReal d);
HCReal HCContourCurveParameterQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal d);
HCReal HCContourCurveParameterCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal d);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameter Given Point
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCContourCurveParameterNearestPoint(HCPoint p0, HCContourCurve curve, HCPoint p);
HCReal HCContourCurveParameterNearestPointLinear(HCPoint p0, HCPoint p1, HCPoint p);
HCReal HCContourCurveParameterNearestPointQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint p);
HCReal HCContourCurveParameterNearestPointCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint p);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameter Given Axis
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveParametersFromXAxis(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* extrema);
void HCContourCurveParametersFromYAxis(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* extrema);
void HCContourCurveParametersLinearFromXAxis(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* extrema);
void HCContourCurveParametersLinearFromYAxis(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* extrema);
void HCContourCurveParametersQuadraticFromXAxis(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* extrema);
void HCContourCurveParametersQuadraticFromYAxis(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* extrema);
void HCContourCurveParametersCubicFromXAxis(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* extrema);
void HCContourCurveParametersCubicFromYAxis(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* extrema);

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
// MARK: - Projection
//----------------------------------------------------------------------------------------------------------------------------------

HCPoint HCContourCurveProjection(HCPoint p0, HCContourCurve curve);
HCPoint HCContourCurveProjectionLinear(HCPoint p0, HCPoint p1);
HCPoint HCContourCurveProjectionQuadratic(HCPoint p0, HCPoint c, HCPoint p1);
HCPoint HCContourCurveProjectionCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Moulding
//----------------------------------------------------------------------------------------------------------------------------------

HCContourCurve HCContourCurveMould(HCPoint p0, HCContourCurve curve, HCPoint p, HCReal t);
HCContourCurve HCContourCurveMouldLinear(HCPoint p0, HCPoint p1, HCPoint p, HCReal t);
HCContourCurve HCContourCurveMouldQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint p, HCReal t);
HCContourCurve HCContourCurveMouldCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint p, HCReal t);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Interpolation
//----------------------------------------------------------------------------------------------------------------------------------

HCContourCurve HCContourCurveInterpolating(HCPoint p0, HCPoint p1, HCPoint p, HCReal t);
HCContourCurve HCContourCurveInterpolatingLinear(HCPoint p0, HCPoint p, HCReal t);
HCContourCurve HCContourCurveInterpolatingQuadratic(HCPoint p0, HCPoint p1, HCPoint p, HCReal t);
HCContourCurve HCContourCurveInterpolatingCubic(HCPoint p0, HCPoint p1, HCPoint p, HCReal t, HCReal dx, HCReal dy);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Fitting
//----------------------------------------------------------------------------------------------------------------------------------

void HCContourCurveFitting(HCInteger count, const HCPoint* points, HCPoint* p0, HCContourCurve* curve);
void HCContourCurveFittingLinear(HCInteger count, const HCPoint* points, HCPoint* p0, HCContourCurve* curve);
void HCContourCurveFittingQuadratic(HCInteger count, const HCPoint* points, HCPoint* p0, HCContourCurve* curve);
void HCContourCurveFittingCubic(HCInteger count, const HCPoint* points, HCPoint* p0, HCContourCurve* curve);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Split
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveSplit(HCPoint p0, HCContourCurve curve, HCReal t, HCPoint* sp0, HCContourCurve* sCurve, HCPoint* ep0, HCContourCurve* eCurve);
void HCContourCurveSplitLinear(HCPoint p0, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sp1, HCPoint* ep0, HCPoint* ep1);
void HCContourCurveSplitQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sc, HCPoint* sp1, HCPoint* ep0, HCPoint* ec, HCPoint* ep1);
void HCContourCurveSplitCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sc0, HCPoint* sc1, HCPoint* sp1, HCPoint* ep0, HCPoint* ec0, HCPoint* ec1, HCPoint* ep1);

#endif /* HCContourCurve_h */
