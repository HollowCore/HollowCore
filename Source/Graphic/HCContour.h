///
/// @file HCContour.h
/// @package HollowCore
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

#endif /* HCContour_h */
