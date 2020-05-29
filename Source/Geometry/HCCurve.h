///
/// @file HCCurve.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 5/24/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCCurve_h
#define HCCurve_h

#include "../Geometry/HCRectangle.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCCurve {
    HCPoint p0;
    HCPoint c0;
    HCPoint c1;
    HCPoint p1;
} HCCurve;

typedef enum HCCurveType {
    HCCurveTypeInvalid                   = 0b0000000000,
    HCCurveTypePoint                     = 0b0000000001,
    HCCurveTypeLinear                    = 0b0000000010,
    HCCurveTypeQuadratic                 = 0b0000000100,
    HCCurveTypeCubicSimple               = 0b0000001000,
    HCCurveTypeCubicSingleInflection     = 0b0000011000,
    HCCurveTypeCubicDoubleInflection     = 0b0000111000,
    HCCurveTypeCubicLoop                 = 0b0001001000,
    HCCurveTypeCubicLoopAtStart          = 0b0011001000,
    HCCurveTypeCubicLoopAtEnd            = 0b0101001000,
    HCCurveTypeCubicLoopClosed           = 0b0111001000,
    HCCurveTypeCubicCusp                 = 0b1000001000,
} HCCurveType;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contour Curve Special Values
//----------------------------------------------------------------------------------------------------------------------------------

#define HCCurveInvalidStatic ((HCCurve){.p0 = HCPointInvalidStatic, .c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p1 = HCPointInvalidStatic})
static const HCCurve HCCurveInvalid = HCCurveInvalidStatic;
#define HCCurveZeroStatic ((HCCurve){.p0 = HCPointZeroStatic, .c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p1 = HCPointZeroStatic})
static const HCCurve HCCurveZero = HCCurveZeroStatic;
#define HCCurvePointAxisDissimilarityStatic (DBL_EPSILON)
static const HCReal HCCurvePointAxisDissimilarity = HCCurvePointAxisDissimilarityStatic;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCCurve HCCurveMakeLinear(HCPoint p0, HCPoint p1);
HCCurve HCCurveMakeQuadratic(HCPoint p0, HCPoint c, HCPoint p1);
HCCurve HCCurveMakeCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCCurveIsInvalid(HCCurve curve);
HCBoolean HCCurveIsSimilar(HCCurve curve, HCCurve other, HCReal axisDissimilarity);
HCBoolean HCCurveIsZero(HCCurve curve);
HCBoolean HCCurveIsInfinite(HCCurve curve);
HCBoolean HCCurveIsEqual(HCCurve curve, HCCurve other);
HCInteger HCCurveHashValue(HCCurve curve);
void HCCurvePrint(HCCurve curve, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Order
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCCurveIsLinear(HCCurve curve);
HCCurve HCCurveAsLinear(HCCurve curve);
HCBoolean HCCurveIsQuadratic(HCCurve curve);
HCCurve HCCurveAsQuadratic(HCCurve curve);
HCBoolean HCCurveIsCubic(HCCurve curve);
HCCurve HCCurveAsCubic(HCCurve curve);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Canonical Type
//----------------------------------------------------------------------------------------------------------------------------------
HCCurveType HCCurveCanonicalType(HCCurve curve);
HCCurveType HCCurveCanonicalTypeLinear(HCPoint p0, HCPoint p1);
HCCurveType HCCurveCanonicalTypeQuadratic(HCPoint p0, HCPoint c, HCPoint p1);
HCCurveType HCCurveCanonicalTypeCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1);
HCPoint HCCurveCanonical(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCCurveValue(HCCurve curve, HCReal t);
HCPoint HCCurveValueLinear(HCPoint p0, HCPoint p1, HCReal t);
HCPoint HCCurveValueQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t);
HCPoint HCCurveValueCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Evaluation
//----------------------------------------------------------------------------------------------------------------------------------
void HCCurveEvaluate(HCCurve curve, HCReal t, HCReal* x, HCReal* y, HCReal* dx, HCReal* dy, HCReal* ddx, HCReal* ddy);
void HCCurveEvaluateLinear(HCPoint p0, HCPoint p1, HCReal t, HCReal* x, HCReal* y, HCReal* dx, HCReal* dy, HCReal* ddx, HCReal* ddy);
void HCCurveEvaluateQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t, HCReal* x, HCReal* y, HCReal* dx, HCReal* dy, HCReal* ddx, HCReal* ddy);
void HCCurveEvaluateCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t, HCReal* x, HCReal* y, HCReal* dx, HCReal* dy, HCReal* ddx, HCReal* ddy);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Axis Alignment
//----------------------------------------------------------------------------------------------------------------------------------
HCCurve HCCurveXAxisAligned(HCCurve curve);
void HCCurveXAxisAlignedLinear(HCPoint p0, HCPoint p1, HCPoint* ap0, HCPoint* ap1);
void HCCurveXAxisAlignedQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint* ap0, HCPoint* ac, HCPoint* ap1);
void HCCurveXAxisAlignedCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint* ap0, HCPoint* ac0, HCPoint* ac1, HCPoint* ap1);
HCCurve HCCurveYAxisAligned(HCCurve curve);
void HCCurveYAxisAlignedLinear(HCPoint p0, HCPoint p1, HCPoint* ap0, HCPoint* ap1);
void HCCurveYAxisAlignedQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint* ap0, HCPoint* ac, HCPoint* ap1);
void HCCurveYAxisAlignedCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint* ap0, HCPoint* ac0, HCPoint* ac1, HCPoint* ap1);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Split
//----------------------------------------------------------------------------------------------------------------------------------
void HCCurveSplit(HCCurve curve, HCReal t, HCCurve* sCurve, HCCurve* eCurve);
void HCCurveSplitLinear(HCPoint p0, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sp1, HCPoint* ep0, HCPoint* ep1);
void HCCurveSplitQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sc, HCPoint* sp1, HCPoint* ep0, HCPoint* ec, HCPoint* ep1);
void HCCurveSplitCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sc0, HCPoint* sc1, HCPoint* sp1, HCPoint* ep0, HCPoint* ec0, HCPoint* ec1, HCPoint* ep1);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Curvature
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCCurveCurvature(HCCurve curve, HCReal t);
HCReal HCCurveCurvatureLinear(HCPoint p0, HCPoint p1, HCReal t);
HCReal HCCurveCurvatureQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t);
HCReal HCCurveCurvatureCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Derivative
//----------------------------------------------------------------------------------------------------------------------------------
HCCurve HCCurveDerivative(HCCurve curve);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Extrema
//----------------------------------------------------------------------------------------------------------------------------------
void HCCurveExtrema(HCCurve curve, HCInteger* count, HCReal* extrema);
void HCCurveExtremaLinear(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* extrema);
void HCCurveExtremaQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* extrema);
void HCCurveExtremaCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* extrema);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Inflection
//----------------------------------------------------------------------------------------------------------------------------------
void HCCurveInflections(HCCurve curve, HCInteger* count, HCReal* inflections);
void HCCurveInflectionsLinear(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* inflections);
void HCCurveInflectionsQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* inflections);
void HCCurveInflectionsCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* inflections);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Approximate Bounds
//----------------------------------------------------------------------------------------------------------------------------------
HCRectangle HCCurveApproximateBounds(HCCurve curve);
HCRectangle HCCurveApproximateBoundsLinear(HCPoint p0, HCPoint p1);
HCRectangle HCCurveApproximateBoundsQuadratic(HCPoint p0, HCPoint c, HCPoint p1);
HCRectangle HCCurveApproximateBoundsCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Bounds
//----------------------------------------------------------------------------------------------------------------------------------
HCRectangle HCCurveBounds(HCCurve curve);
HCRectangle HCCurveBoundsLinear(HCPoint p0, HCPoint p1);
HCRectangle HCCurveBoundsQuadratic(HCPoint p0, HCPoint c, HCPoint p1);
HCRectangle HCCurveBoundsCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Length
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCCurveLength(HCCurve curve);
HCReal HCCurveLengthLinear(HCPoint p0, HCPoint p1);
HCReal HCCurveLengthQuadratic(HCPoint p0, HCPoint c, HCPoint p1);
HCReal HCCurveLengthCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameterization by Arc Length
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCCurveParameter(HCCurve curve, HCReal d);
HCReal HCCurveParameterLinear(HCPoint p0, HCPoint p1, HCReal d);
HCReal HCCurveParameterQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal d);
HCReal HCCurveParameterCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal d);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameter Given Point
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCCurveParameterNearestPoint(HCCurve curve, HCPoint p);
HCReal HCCurveParameterNearestPointLinear(HCPoint p0, HCPoint p1, HCPoint p);
HCReal HCCurveParameterNearestPointQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint p);
HCReal HCCurveParameterNearestPointCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint p);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Distance from Point
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCCurveDistanceFromPoint(HCCurve curve, HCPoint p);
HCReal HCCurveDistanceFromPointLinear(HCPoint p0, HCPoint p1, HCPoint p);
HCReal HCCurveDistanceFromPointQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint p);
HCReal HCCurveDistanceFromPointCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint p);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Baseline Projection
//----------------------------------------------------------------------------------------------------------------------------------

HCPoint HCCurveBaselineProjection(HCCurve curve, HCReal t);
HCPoint HCCurveBaselineProjectionLinear(HCPoint p0, HCPoint p1, HCReal t);
HCPoint HCCurveBaselineProjectionQuadratic(HCPoint p0, HCPoint p1, HCReal t);
HCPoint HCCurveBaselineProjectionCubic(HCPoint p0, HCPoint p1, HCReal t);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Interpolation
//----------------------------------------------------------------------------------------------------------------------------------

HCCurve HCCurveInterpolatingPoint(HCPoint p0, HCPoint p1, HCPoint p, HCReal t, HCReal dx, HCReal dy);
void HCCurveInterpolatingPointLinear(HCPoint p0, HCPoint p, HCReal t, HCPoint* p1);
void HCCurveInterpolatingPointQuadratic(HCPoint p0, HCPoint p1, HCPoint p, HCReal t, HCPoint* rc);
void HCCurveInterpolatingPointCubic(HCPoint p0, HCPoint p1, HCPoint p, HCReal t, HCReal dx, HCReal dy, HCPoint* rc0, HCPoint* rc1);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Moulding
//----------------------------------------------------------------------------------------------------------------------------------

HCCurve HCCurveMould(HCCurve curve, HCReal t, HCPoint p);
void HCCurveMouldLinear(HCPoint p0, HCPoint p1, HCReal t, HCPoint p);
void HCCurveMouldQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t, HCPoint p, HCPoint* rc);
void HCCurveMouldCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t, HCPoint p, HCPoint* rc0, HCPoint* rc1);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Intersection
//----------------------------------------------------------------------------------------------------------------------------------
void HCCurveIntersection(HCCurve pCurve, HCCurve qCurve, HCInteger* count, HCReal* t, HCReal* u);
void HCCurveIntersectionLinearLinear(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u);
void HCCurveIntersectionLinearQuadratic(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint qc, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u);
void HCCurveIntersectionLinearCubic(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u);
void HCCurveIntersectionQuadraticQuadratic(HCPoint p0, HCPoint pc, HCPoint p1, HCPoint q0, HCPoint qc, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u);
void HCCurveIntersectionQuadraticCubic(HCPoint p0, HCPoint pc, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u);
void HCCurveIntersectionCubicCubic(HCPoint p0, HCPoint pc0, HCPoint pc1, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u);


#endif /* HCCurve_h */
