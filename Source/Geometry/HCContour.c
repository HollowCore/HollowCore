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
HCContour HCContourMake(HCPoint startPoint, HCInteger componentCount, HCBoolean closed);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCContour HCContourMake(HCPoint startPoint, HCInteger componentCount, HCBoolean closed) {
    HCContour contour;
    contour.component = HCContourComponentMakeLinear(startPoint);
    contour.count = componentCount;
    contour.closed = closed;
    return contour;
}

HCContour* HCContourInitInComponents(HCContourComponent* components, HCInteger componentCount, HCBoolean closed) {
    HCContour* contour = (HCContour*)components;
    *contour = HCContourMake(components[0].p, componentCount, closed);
    return contour;
}

void HCContourInitWithPolyline(void* memory, HCPoint startPoint, const HCPoint* points, HCInteger pointCount, HCBoolean closed) {
    HCContour* contour = (HCContour*)memory;
    *contour = HCContourMake(startPoint, pointCount + 1, closed);
    HCContourComponent* components = (HCContourComponent*)memory;
    for (HCInteger pointIndex = 0; pointIndex < pointCount; pointIndex += 1) {
        HCInteger componentIndex = 1 + pointIndex;
        components[componentIndex] = HCContourComponentMakeLinear(points[pointIndex]);
    }
}

void HCContourInitWithPolyquadratic(void* memory, HCPoint startPoint, const HCPoint* points, HCInteger quadraticCount, HCBoolean closed) {
    HCContour* contour = (HCContour*)memory;
    *contour = HCContourMake(startPoint, quadraticCount + 1, closed);
    HCContourComponent* components = (HCContourComponent*)memory;
    for (HCInteger quadraticIndex = 0; quadraticIndex < quadraticCount; quadraticIndex++) {
        HCInteger componentIndex = 1 + quadraticIndex;
        components[componentIndex] = HCContourComponentMakeQuadratic(points[quadraticIndex * 2], points[quadraticIndex * 2 + 1]);
    }
}

void HCContourInitWithPolycubic(void* memory, HCPoint startPoint, const HCPoint* points, HCInteger cubicCount, HCBoolean closed) {
    HCContour* contour = (HCContour*)memory;
    *contour = HCContourMake(startPoint, cubicCount + 1, closed);
    HCContourComponent* components = (HCContourComponent*)memory;
    for (HCInteger cubicIndex = 0; cubicIndex < cubicCount; cubicIndex++) {
        HCInteger componentIndex = 1 + cubicIndex;
        components[componentIndex] = HCContourComponentMakeCubic(points[cubicIndex * 3], points[cubicIndex * 3 + 1], points[cubicIndex * 3 + 2]);
    }
}

void HCContourInitWithComponents(void* memory, const HCContourComponent* components, HCInteger componentCount, HCBoolean closed) {
    memcpy(memory, components, sizeof(HCContourComponent) * componentCount);
    HCContour* contour = (HCContour*)memory;
    *contour = HCContourMake(components[0].p, componentCount, closed);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourIsEqual(const HCContour* contour, const HCContour* other) {
    HCInteger componentCount = HCContourComponentCount(contour);
    HCInteger otherComponentCount = HCContourComponentCount(other);
    if (componentCount != otherComponentCount) {
        return false;
    }
    return memcmp(contour, other, componentCount * sizeof(HCContourComponent)) == 0;
}

HCInteger HCContourHashValue(const HCContour* contour) {
    HCInteger componentCount = HCContourComponentCount(contour);
    HCInteger hash = 0;
    for (HCInteger componentIndex = 1; componentIndex < componentCount; componentIndex++) {
        hash ^= HCContourComponentHashValue(HCContourComponentAt(contour, componentIndex));
    }
    return hash;
}

void HCContourPrint(const HCContour* contour, FILE* stream) {
    HCInteger componentCount = HCContourComponentCount(contour);
    fprintf(stream, "<count:%lli", componentCount);
    fprintf(stream, ",start:");
    HCPointPrint(HCContourStartPoint(contour), stream);
    fprintf(stream, ",components:<");
    for (HCInteger componentIndex = 1; componentIndex < componentCount; componentIndex++) {
        HCContourComponentPrint(HCContourComponentAt(contour, componentIndex), stream);
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

HCContourComponent HCContourComponentAt(const HCContour* contour, HCInteger componentIndex) {
    return HCContourComponents(contour)[componentIndex];
}

HCContourComponent HCContourComponentContaining(const HCContour* contour, HCReal t) {
    HCInteger componentIndex = HCContourComponentIndexContaining(contour, t);
    return HCContourComponentAt(contour, componentIndex);
}

HCInteger HCContourComponentIndexContaining(const HCContour* contour, HCReal t) {
    return (HCInteger)floor(fmax(1.0, fmin((HCReal)(contour->count - 1), t * (HCReal)contour->count)));
}

HCReal HCContourComponentParameterFor(const HCContour* contour, HCReal t) {
    return fmax(0.0, fmin(1.0, fmod(t * (HCReal)contour->count, 1.0)));
}

const HCContourComponent* HCContourComponents(const HCContour* contour) {
    return (const HCContourComponent*)contour;
}

HCInteger HCContourCurveCount(const HCContour* contour) {
    return HCContourComponentCount(contour) - 1;
}

HCCurve HCContourCurveAt(const HCContour* contour, HCInteger curveIndex) {
    return HCCurveMakeWithContourComponent(HCContourComponentAt(contour, curveIndex).p, HCContourComponentAt(contour, curveIndex + 1));
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
