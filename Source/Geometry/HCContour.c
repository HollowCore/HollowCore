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
    HCInteger curveCount = HCContourComponentCount(contour);
    HCInteger otherCurveCount = HCContourComponentCount(other);
    if (curveCount != otherCurveCount) {
        return false;
    }
    return memcmp(contour, other, curveCount * sizeof(HCContourCurve)) == 0;
}

HCInteger HCContourHashValue(const HCContour* contour) {
    HCInteger componentCount = HCContourComponentCount(contour);
    HCInteger hash = 0;
    for (HCInteger componentIndex = 1; componentIndex < componentCount; componentIndex++) {
        hash ^= HCContourCurveHashValue(HCContourComponentAt(contour, componentIndex));
    }
    return hash;
}

void HCContourPrint(const HCContour* contour, FILE* stream) {
    HCInteger componentCount = HCContourComponentCount(contour);
    fprintf(stream, "<count:%lli", componentCount);
    fprintf(stream, ",start:");
    HCPointPrint(HCContourStartPoint(contour), stream);
    fprintf(stream, ",curves:<");
    for (HCInteger componentIndex = 1; componentIndex < componentCount; componentIndex++) {
        HCContourCurvePrint(HCContourComponentAt(contour, componentIndex), stream);
    }
    fprintf(stream, ">");
    fprintf(stream, ",closed:%s>", HCContourIsClosed(contour) ? "⊨" : "⊭");
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourIsEmpty(const HCContour* contour) {
    return HCContourComponentCount(contour) == 0;
}

HCBoolean HCContourIsClosed(const HCContour* contour) {
    return contour->closed;
}

HCPoint HCContourStartPoint(const HCContour* contour) {
    return contour->start;
}

HCPoint HCContourEndPoint(const HCContour* contour) {
    return HCContourIsClosed(contour) ? HCContourStartPoint(contour) : HCContourComponentAt(contour, HCContourComponentCount(contour) - 1).p;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Components
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCContourComponentCount(const HCContour* contour) {
    return contour->count;
}

HCContourCurve HCContourComponentAt(const HCContour* contour, HCInteger componentIndex) {
    return HCContourComponents(contour)[componentIndex];
}

HCContourCurve HCContourComponentContaining(const HCContour* contour, HCReal t) {
    HCInteger componentIndex = HCContourComponentIndexContaining(contour, t);
    return HCContourComponentAt(contour, componentIndex);
}

HCInteger HCContourComponentIndexContaining(const HCContour* contour, HCReal t) {
    return (HCInteger)floor(fmax(1.0, fmin((HCReal)(contour->count - 1), t * (HCReal)contour->count)));
}

HCReal HCContourComponentParameterFor(const HCContour* contour, HCReal t) {
    return fmax(0.0, fmin(1.0, fmod(t * (HCReal)contour->count, 1.0)));
}

const HCContourCurve* HCContourComponents(const HCContour* contour) {
    return (const HCContourCurve*)contour;
}

HCInteger HCContourCurveCount(const HCContour* contour) {
    return HCContourComponentCount(contour) - 1;
}

HCCurve HCContourCurveAt(const HCContour* contour, HCInteger curveIndex) {
    return HCCurveMakeWithContourCurve(HCContourComponentAt(contour, curveIndex).p, HCContourComponentAt(contour, curveIndex + 1));
}

HCCurve HCContourCurveContaining(const HCContour* contour, HCReal t) {
    HCInteger curveIndex = HCContourCurveIndexContaining(contour, t);
    return HCContourCurveAt(contour, curveIndex);
}

HCInteger HCContourCurveIndexContaining(const HCContour* contour, HCReal t) {
    return HCContourComponentIndexContaining(contour, t) - 1;
}

HCReal HCContourCurveParameterFor(const HCContour* contour, HCReal t) {
    return HCContourComponentParameterFor(contour, t);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourValue(const HCContour* contour, HCReal t) {
    return HCCurveValue(HCContourCurveContaining(contour, t), HCContourCurveParameterFor(contour, t));
}

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
