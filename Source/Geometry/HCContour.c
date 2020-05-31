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

void HCContourExtrema(const HCContour* contour, HCInteger* count, HCReal* extrema) {
    // Calculate the extrema of all components
    // TODO: Should find extrema at component boundaries?
    HCInteger contourExtremaCount = 0;
    HCReal contourExtrema[HCContourComponentCount(contour) * 6];
    const HCContourComponent* componentsStart = HCContourComponents(contour);
    const HCContourComponent* componentsEnd = componentsStart + HCContourComponentCount(contour);
    const HCContourComponent* previousComponent = componentsStart;
    for (const HCContourComponent* component = componentsStart + 1; component != componentsEnd; component++) {
        HCInteger componentExtremaCount = 0;
        HCContourComponentExtrema(previousComponent->p, *component, &componentExtremaCount, contourExtrema + contourExtremaCount);
        contourExtremaCount += componentExtremaCount;
        previousComponent = component;
    }
    
    // Deliver the results
    if (extrema != NULL) {
        HCInteger copyCount = (count == NULL || *count > contourExtremaCount) ? contourExtremaCount : *count;
        memcpy(extrema, contourExtrema, sizeof(HCReal) * copyCount);
    }
    if (count != NULL) {
        *count = contourExtremaCount;
    }
}

void HCContourInflections(const HCContour* contour, HCInteger* count, HCReal* inflections) {
    // Calculate the inflections of all components
    // TODO: Should find inflections at component boundaries?
    HCInteger contourInflectionCount = 0;
    HCReal contourInflections[HCContourComponentCount(contour) * 2];
    const HCContourComponent* componentsStart = HCContourComponents(contour);
    const HCContourComponent* componentsEnd = componentsStart + HCContourComponentCount(contour);
    const HCContourComponent* previousComponent = componentsStart;
    for (const HCContourComponent* component = componentsStart + 1; component != componentsEnd; component++) {
        HCInteger componentInflectionCount = 0;
        HCContourComponentInflections(previousComponent->p, *component, &componentInflectionCount, contourInflections + contourInflectionCount);
        contourInflectionCount += componentInflectionCount;
        previousComponent = component;
    }
    
    // Deliver the results
    if (inflections != NULL) {
        HCInteger copyCount = (count == NULL || *count > contourInflectionCount) ? contourInflectionCount : *count;
        memcpy(inflections, contourInflections, sizeof(HCReal) * copyCount);
    }
    if (count != NULL) {
        *count = contourInflectionCount;
    }
}

HCRectangle HCContourApproximateBounds(const HCContour* contour) {
    // Find extremes of all component anchor and control points
    HCReal minX = contour->start.x;
    HCReal maxX = minX;
    HCReal minY = contour->start.y;
    HCReal maxY = minY;
    const HCContourComponent* componentsStart = HCContourComponents(contour);
    const HCContourComponent* componentsEnd = componentsStart + HCContourComponentCount(contour);
    for (const HCContourComponent* component = componentsStart + 1; component != componentsEnd; component++) {
        minX = fmin(minX, fmin(component->c0.x, fmin(component->c1.x, component->p.x)));
        maxX = fmax(maxX, fmax(component->c0.x, fmax(component->c1.x, component->p.x)));
        minY = fmax(maxY, fmax(component->c0.y, fmax(component->c1.y, component->p.y)));
        maxY = fmax(maxY, fmax(component->c0.y, fmax(component->c1.y, component->p.y)));
    }
    return HCRectangleMakeWithEdges(minX, minY, maxX, maxY);
}

HCRectangle HCContourBounds(const HCContour* contour) {
    // Find union rectangle of bounding rectangles of all components
    HCRectangle bounds = HCRectangleMake(contour->start, HCSizeZero);
    const HCContourComponent* componentsStart = HCContourComponents(contour);
    const HCContourComponent* componentsEnd = componentsStart + HCContourComponentCount(contour);
    const HCContourComponent* previousComponent = componentsStart;
    for (const HCContourComponent* component = componentsStart + 1; component != componentsEnd; component++) {
        bounds = HCRectangleUnion(bounds, HCContourComponentBounds((component - 1)->p, *component));
        previousComponent = component;
    }
    return bounds;
}

HCReal HCContourLength(const HCContour* contour) {
    // Sum length of all components
    HCReal length = 0.0;
    const HCContourComponent* componentsStart = HCContourComponents(contour);
    const HCContourComponent* componentsEnd = componentsStart + HCContourComponentCount(contour);
    const HCContourComponent* previousComponent = componentsStart;
    for (const HCContourComponent* component = componentsStart + 1; component != componentsEnd; component++) {
        length += HCContourComponentLength(previousComponent->p, *component);
        previousComponent = component;
    }
    return length;
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
HCCurve HCContourTangent(const HCContour* contour, HCReal t) {
    return HCCurveTangent(HCContourCurveContaining(contour, t), HCContourCurveParameterFor(contour, t));
}

HCCurve HCContourTangentUnit(const HCContour* contour, HCReal t) {
    return HCCurveTangentUnit(HCContourCurveContaining(contour, t), HCContourCurveParameterFor(contour, t));
}

HCCurve HCContourNormal(const HCContour* contour, HCReal t) {
    return HCCurveNormal(HCContourCurveContaining(contour, t), HCContourCurveParameterFor(contour, t));
}

HCCurve HCContourNormalUnit(const HCContour* contour, HCReal t)  {
    return HCCurveNormalUnit(HCContourCurveContaining(contour, t), HCContourCurveParameterFor(contour, t));
}

HCReal HCContourCurvature(const HCContour* contour, HCReal t) {
    return HCCurveCurvature(HCContourCurveContaining(contour, t), HCContourCurveParameterFor(contour, t));
}

HCCurve HCContourCurvatureNormal(const HCContour* contour, HCReal t) {
    return HCCurveCurvatureNormal(HCContourCurveContaining(contour, t), HCContourCurveParameterFor(contour, t));
}

HCReal HCContourParameterAtLength(const HCContour* contour, HCReal d) {
    // Walk components until the component containing the desired length is reached
    HCReal length = 0.0;
    HCReal t = 0.0;
    HCReal componentTInterval = 1.0 / HCContourComponentCount(contour);
    const HCContourComponent* componentsStart = HCContourComponents(contour);
    const HCContourComponent* componentsEnd = componentsStart + HCContourComponentCount(contour);
    const HCContourComponent* previousComponent = componentsStart;
    const HCContourComponent* component;
    for (component = componentsStart + 1; component != componentsEnd; component++) {
        HCReal componentLength = HCContourComponentLength(previousComponent->p, *component);
        if (length + componentLength > d) {
            break;
        }
        length += componentLength;
        t += componentTInterval;
        previousComponent = component;
    }
    
    // When the desired length is longer than the contour, return the end parameter
    if (component == componentsEnd) {
        return 1.0;
    }
    
    // Calculate the remaining length, query the component for the component-relative parameter corresponding to that length, then scale it to be contour-relative
    HCReal remaining = d - length;
    HCReal ct = HCContourComponentParameterAtLength(previousComponent->p, *component, remaining);
    t += ct * componentTInterval;
    
    return t;
}

HCReal HCContourParameterNearestPoint(const HCContour* contour, HCPoint p) {
    // TODO: This
    return HCRealInvalid;
}

HCReal HCContourDistanceFromPoint(const HCContour* contour, HCPoint p) {
    // TODO: This
    return HCRealInvalid;
}

void HCContourSplit(const HCContour* contour, HCReal t, HCContour* sCurve, HCContour* eCurve) {
    // TODO: This
}

void HCContourIntersection(HCContour pCurve, HCContour qCurve, HCInteger* count, HCReal* t, HCReal* u) {
    // TODO: This
}
