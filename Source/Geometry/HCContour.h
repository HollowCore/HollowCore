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
// MARK: - Construction
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
void HCContourExtrema(const HCContour* contour, HCInteger* count, HCReal* extrema);
void HCContourInflections(const HCContour* contour, HCInteger* count, HCReal* inflections);
HCRectangle HCContourApproximateBounds(const HCContour* contour);
HCRectangle HCContourBounds(const HCContour* contour);
HCReal HCContourLength(const HCContour* contour);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Components
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCContourComponentCount(const HCContour* contour);
HCContourComponent HCContourComponentAt(const HCContour* contour, HCInteger componentIndex);
HCContourComponent HCContourComponentContainingParameter(const HCContour* contour, HCReal t);
HCInteger HCContourComponentIndexContainingParameter(const HCContour* contour, HCReal t);
HCReal HCContourComponentParameterForParameter(const HCContour* contour, HCReal t);
HCReal HCContourParameterForComponentParameter(const HCContour* contour, HCInteger componentIndex, HCReal t);
const HCContourComponent* HCContourComponents(const HCContour* contour);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Components as Curves
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCContourCurveCount(const HCContour* contour);
HCCurve HCContourCurveAt(const HCContour* contour, HCInteger curveIndex);
HCCurve HCContourCurveContainingParameter(const HCContour* contour, HCReal t);
HCInteger HCContourCurveIndexContainingParameter(const HCContour* contour, HCReal t);
HCReal HCContourCurveParameterForParameter(const HCContour* contour, HCReal t);
HCReal HCContourParameterForCurveParameter(const HCContour* contour, HCInteger curveIndex, HCReal t);
HCInteger HCContourCurveIndexForComponentIndex(const HCContour* contour, HCInteger componentIndex);
HCInteger HCContourComponentIndexForCurveIndex(const HCContour* contour, HCInteger curveIndex);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourValue(const HCContour* contour, HCReal t);
HCCurve HCContourTangent(const HCContour* contour, HCReal t);
HCCurve HCContourTangentUnit(const HCContour* contour, HCReal t);
HCCurve HCContourNormal(const HCContour* contour, HCReal t);
HCCurve HCContourNormalUnit(const HCContour* contour, HCReal t);
HCReal HCContourCurvature(const HCContour* contour, HCReal t);
HCCurve HCContourCurvatureNormal(const HCContour* contour, HCReal t);
HCReal HCContourParameterAtLength(const HCContour* contour, HCReal d);
HCReal HCContourParameterNearestPoint(const HCContour* contour, HCPoint p);
HCReal HCContourDistanceFromPoint(const HCContour* contour, HCPoint p);
void HCContourIntersections(const HCContour* pContour, const HCContour* qContour, HCInteger* count, HCReal* t, HCReal* u);

#endif /* HCContour_h */
