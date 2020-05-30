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

#include "HCContourComponent.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef union HCContour {
    HCContourComponent component;
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
HCContour* HCContourInitInComponents(HCContourComponent* components, HCInteger componentCount, HCBoolean closed);
void HCContourInitWithPolyline(void* memory, HCPoint startPoint, const HCPoint* points, HCInteger pointCount, HCBoolean closed);
void HCContourInitWithPolyquadratic(void* memory, HCPoint startPoint, const HCPoint* points, HCInteger quadraticCount, HCBoolean closed);
void HCContourInitWithPolycubic(void* memory, HCPoint startPoint, const HCPoint* points, HCInteger cubicCount, HCBoolean closed);
void HCContourInitWithComponents(void* memory, const HCContourComponent* curves, HCInteger curveCount, HCBoolean closed);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourIsEqual(const HCContour* contour, const HCContour* other);
HCInteger HCContourHashValue(const HCContour* contour);
void HCContourPrint(const HCContour* contour, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourIsClosed(const HCContour* contour);
HCPoint HCContourStartPoint(const HCContour* contour);
HCPoint HCContourEndPoint(const HCContour* contour);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Components
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCContourComponentCount(const HCContour* contour);
HCContourComponent HCContourComponentAt(const HCContour* contour, HCInteger componentIndex);
HCContourComponent HCContourComponentContaining(const HCContour* contour, HCReal t);
HCInteger HCContourComponentIndexContaining(const HCContour* contour, HCReal t);
HCReal HCContourComponentParameterFor(const HCContour* contour, HCReal t);
const HCContourComponent* HCContourComponents(const HCContour* contour);
HCInteger HCContourCurveCount(const HCContour* contour);
HCCurve HCContourCurveAt(const HCContour* contour, HCInteger curveIndex);
HCCurve HCContourCurveContaining(const HCContour* contour, HCReal t);
HCInteger HCContourCurveIndexContaining(const HCContour* contour, HCReal t);
HCReal HCContourCurveParameterFor(const HCContour* contour, HCReal t);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourValue(const HCContour* contour, HCReal t);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCCurve HCContourTangent(const HCContour* contour, HCReal t);
HCCurve HCContourTangentUnit(const HCContour* contour, HCReal t);
HCCurve HCContourNormal(const HCContour* contour, HCReal t);
HCCurve HCContourNormalUnit(const HCContour* contour, HCReal t);
HCReal HCContourCurvature(const HCContour* contour, HCReal t);
HCCurve HCContourCurvatureNormal(const HCContour* contour, HCReal t);
void HCContourExtrema(const HCContour* contour, HCInteger* count, HCReal* extrema);
void HCContourInflections(const HCContour* contour, HCInteger* count, HCReal* inflections);
HCRectangle HCContourApproximateBounds(const HCContour* contour);
HCRectangle HCContourBounds(const HCContour* contour);
HCReal HCContourLength(const HCContour* contour);
HCReal HCContourParameterAtLength(const HCContour* contour, HCReal d);
HCReal HCContourParameterNearestPoint(const HCContour* contour, HCPoint p);
HCReal HCContourDistanceFromPoint(const HCContour* contour, HCPoint p);
HCReal HCContourDistanceFromPointLinear(HCPoint p0, HCPoint p1, HCPoint p);
HCReal HCContourDistanceFromPointQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint p);
HCReal HCContourDistanceFromPointCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint p);
HCPoint HCContourBaselineProjection(const HCContour* contour, HCReal t);
HCCurve HCContourInterpolatingPoint(HCPoint p0, HCPoint p1, HCPoint p, HCReal t, HCReal dx, HCReal dy);
HCCurve HCContourMould(const HCContour* contour, HCReal t, HCPoint p);
void HCContourSplit(const HCContour* contour, HCReal t, HCContour* sCurve, HCContour* eCurve);
void HCContourIntersection(HCContour pCurve, HCContour qCurve, HCInteger* count, HCReal* t, HCReal* u);

#endif /* HCContour_h */
