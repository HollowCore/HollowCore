///
/// @file HCContour.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 4/21/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCContour_h
#define HCContour_h

#include "HCContourCurve.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef union HCContour {
    HCContourCurve curve;
    struct {
        HCInteger count;
        HCReal invalidMarker1;
        HCReal invalidMarker2;
        HCBoolean closed;
        HCPoint start;
    }; // Private
} HCContour;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCContour* HCContourInitInCurves(HCContourCurve* curves, HCInteger curveCount, HCBoolean closed);
void HCContourInitWithPolyline(void* memory, HCPoint startPoint, const HCPoint* points, HCInteger pointCount, HCBoolean closed);
void HCContourInitWithPolyquadratic(void* memory, HCPoint startPoint, const HCPoint* points, HCInteger quadraticCount, HCBoolean closed);
void HCContourInitWithPolycubic(void* memory, HCPoint startPoint, const HCPoint* points, HCInteger cubicCount, HCBoolean closed);
void HCContourInitWithCurves(void* memory, const HCContourCurve* curves, HCInteger curveCount, HCBoolean closed);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourIsEqual(const HCContour* contour, const HCContour* other);
HCInteger HCContourHashValue(const HCContour* contour);
void HCContourPrint(const HCContour* contour, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCContourCurveCount(const HCContour* contour);
HCBoolean HCContourIsClosed(const HCContour* contour);
HCPoint HCContourStartPoint(const HCContour* contour);
HCPoint HCContourEndPoint(const HCContour* contour);
HCContourCurve HCContourCurveAt(const HCContour* contour, HCInteger curveIndex);
const HCContourCurve* HCContourCurves(const HCContour* contour);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCCurveValue(HCCurve curve, HCReal t);
HCCurve HCCurveTangent(HCCurve curve, HCReal t);
HCCurve HCCurveTangentUnit(HCCurve curve, HCReal t);
HCCurve HCCurveNormal(HCCurve curve, HCReal t);
HCCurve HCCurveNormalUnit(HCCurve curve, HCReal t);
HCReal HCCurveCurvature(HCCurve curve, HCReal t);
HCCurve HCCurveCurvatureNormal(HCCurve curve, HCReal t);
void HCCurveExtrema(HCCurve curve, HCInteger* count, HCReal* extrema);
void HCCurveInflections(HCCurve curve, HCInteger* count, HCReal* inflections);
HCRectangle HCCurveApproximateBounds(HCCurve curve);
HCRectangle HCCurveBounds(HCCurve curve);
HCReal HCCurveLength(HCCurve curve);
HCReal HCCurveParameterAtLength(HCCurve curve, HCReal d);
HCReal HCCurveParameterNearestPoint(HCCurve curve, HCPoint p);
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
// MARK: - Split
//----------------------------------------------------------------------------------------------------------------------------------
void HCCurveSplit(HCCurve curve, HCReal t, HCCurve* sCurve, HCCurve* eCurve);
void HCCurveSplitLinear(HCPoint p0, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sp1, HCPoint* ep0, HCPoint* ep1);
void HCCurveSplitQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sc, HCPoint* sp1, HCPoint* ep0, HCPoint* ec, HCPoint* ep1);
void HCCurveSplitCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sc0, HCPoint* sc1, HCPoint* sp1, HCPoint* ep0, HCPoint* ec0, HCPoint* ec1, HCPoint* ep1);

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
// MARK: - Intersection
//----------------------------------------------------------------------------------------------------------------------------------
void HCCurveIntersection(HCCurve pCurve, HCCurve qCurve, HCInteger* count, HCReal* t, HCReal* u);
void HCCurveIntersectionLinearLinear(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u);
void HCCurveIntersectionLinearQuadratic(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint qc, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u);
void HCCurveIntersectionLinearCubic(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u);
void HCCurveIntersectionQuadraticQuadratic(HCPoint p0, HCPoint pc, HCPoint p1, HCPoint q0, HCPoint qc, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u);
void HCCurveIntersectionQuadraticCubic(HCPoint p0, HCPoint pc, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u);
void HCCurveIntersectionCubicCubic(HCPoint p0, HCPoint pc0, HCPoint pc1, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u);

#endif /* HCContour_h */
