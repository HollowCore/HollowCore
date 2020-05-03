///
/// @file HCContour+Atlas.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 4/21/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCContour.h"
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
HCContour HCContourMake(HCPoint startPoint, HCInteger curveCount, HCBoolean closed);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCContour HCContourMake(HCPoint startPoint, HCInteger curveCount, HCBoolean closed) {
    HCContour contour;
    contour.curve = HCContourCurveMakeLinear(startPoint);
    contour.count = curveCount;
    contour.closed = closed;
    return contour;
}

HCContour* HCContourInitInCurves(HCContourCurve* curves, HCInteger curveCount, HCBoolean closed) {
    HCContour* contour = (HCContour*)curves;
    *contour = HCContourMake(curves[0].p, curveCount, closed);
    return contour;
}

void HCContourInitWithPolyline(void* memory, HCPoint startPoint, const HCPoint* points, HCInteger pointCount, HCBoolean closed) {
    HCContour* contour = (HCContour*)memory;
    *contour = HCContourMake(startPoint, pointCount + 1, closed);
    HCContourCurve* curves = (HCContourCurve*)memory;
    for (HCInteger pointIndex = 0; pointIndex < pointCount; pointIndex += 1) {
        HCInteger curveIndex = 1 + pointIndex;
        curves[curveIndex] = HCContourCurveMakeLinear(points[pointIndex]);
    }
}

void HCContourInitWithPolyquadratic(void* memory, HCPoint startPoint, const HCPoint* points, HCInteger quadraticCount, HCBoolean closed) {
    HCContour* contour = (HCContour*)memory;
    *contour = HCContourMake(startPoint, quadraticCount + 1, closed);
    HCContourCurve* curves = (HCContourCurve*)memory;
    for (HCInteger quadraticIndex = 0; quadraticIndex < quadraticCount; quadraticIndex++) {
        HCInteger curveIndex = 1 + quadraticIndex;
        curves[curveIndex] = HCContourCurveMakeQuadratic(points[quadraticIndex * 2], points[quadraticIndex * 2 + 1]);
    }
}

void HCContourInitWithPolycubic(void* memory, HCPoint startPoint, const HCPoint* points, HCInteger cubicCount, HCBoolean closed) {
    HCContour* contour = (HCContour*)memory;
    *contour = HCContourMake(startPoint, cubicCount + 1, closed);
    HCContourCurve* curves = (HCContourCurve*)memory;
    for (HCInteger cubicIndex = 0; cubicIndex < cubicCount; cubicIndex++) {
        HCInteger curveIndex = 1 + cubicIndex;
        curves[curveIndex] = HCContourCurveMakeCubic(points[cubicIndex * 3], points[cubicIndex * 3 + 1], points[cubicIndex * 3 + 2]);
    }
}

void HCContourInitWithCurves(void* memory, const HCContourCurve* curves, HCInteger curveCount, HCBoolean closed) {
    memcpy(memory, curves, sizeof(HCContourCurve) * curveCount);
    HCContour* contour = (HCContour*)memory;
    *contour = HCContourMake(curves[0].p, curveCount, closed);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourIsEqual(const HCContour* contour, const HCContour* other) {
    HCInteger curveCount = HCContourCurveCount(contour);
    HCInteger otherCurveCount = HCContourCurveCount(other);
    if (curveCount != otherCurveCount) {
        return false;
    }
    return memcmp(contour, other, curveCount * sizeof(HCContourCurve)) == 0;
}

HCInteger HCContourHashValue(const HCContour* contour) {
    HCInteger curveCount = HCContourCurveCount(contour);
    HCInteger hash = 0;
    for (HCInteger curveIndex = 0; curveIndex < curveCount; curveIndex++) {
        hash ^= HCContourCurveHashValue(HCContourCurveAt(contour, curveIndex));
    }
    return hash;
}

void HCContourPrint(const HCContour* contour, FILE* stream) {
    HCInteger curveCount = HCContourCurveCount(contour);
    fprintf(stream, "<count:%lli", curveCount);
    fprintf(stream, ",start:");
    HCPointPrint(HCContourStartPoint(contour), stream);
    fprintf(stream, ",curves:<");
    for (HCInteger curveIndex = 1; curveIndex < curveCount; curveIndex++) {
        HCContourCurvePrint(HCContourCurveAt(contour, curveIndex), stream);
    }
    fprintf(stream, ">");
    fprintf(stream, ",closed:%s>", HCContourIsClosed(contour) ? "⊨" : "⊭");
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCContourCurveCount(const HCContour* contour) {
    return contour->count;
}

HCBoolean HCContourIsClosed(const HCContour* contour) {
    return contour->closed;
}

HCPoint HCContourStartPoint(const HCContour* contour) {
    return contour->start;
}

HCPoint HCContourEndPoint(const HCContour* contour) {
    return HCContourIsClosed(contour) ? HCContourStartPoint(contour) : HCContourCurveAt(contour, HCContourCurveCount(contour) - 1).p;
}

HCContourCurve HCContourCurveAt(const HCContour* contour, HCInteger curveIndex) {
    return contour[curveIndex].curve;
}

const HCContourCurve* HCContourCurves(const HCContour* contour) {
    return (const HCContourCurve*)contour;
}
