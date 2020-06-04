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
// MARK: - Construction
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
    HCInteger requestedCount = count == NULL ? HCIntegerMaximum : *count;
    const HCContourComponent* componentsStart = HCContourComponents(contour);
    const HCContourComponent* componentsEnd = componentsStart + HCContourComponentCount(contour);
    const HCContourComponent* previousComponent = componentsStart;
    for (const HCContourComponent* component = componentsStart + 1; component != componentsEnd; component++) {
        const HCCurve* curve = (HCCurve*)&previousComponent->p;
        HCInteger curveExtremaCount = 6;
        HCReal curveExtrema[curveExtremaCount];
        HCCurveExtrema(*curve, &curveExtremaCount, curveExtrema);
        memcpy(contourExtrema + contourExtremaCount, curveExtrema, sizeof(HCReal) * curveExtremaCount);
        contourExtremaCount += curveExtremaCount;
        previousComponent = component;
        if (contourExtremaCount >= requestedCount) {
            break;
        }
    }
    
    // Deliver the results
    HCInteger copyCount = contourExtremaCount < requestedCount ? contourExtremaCount : requestedCount;
    if (extrema != NULL) {
        memcpy(extrema, contourExtrema, sizeof(HCReal) * copyCount);
    }
    if (count != NULL) {
        *count = copyCount;
    }
}

void HCContourInflections(const HCContour* contour, HCInteger* count, HCReal* inflections) {
    // Calculate the inflections of all components
    // TODO: Should find inflections at component boundaries?
    HCInteger contourInflectionCount = 0;
    HCReal contourInflections[HCContourComponentCount(contour) * 2];
    HCInteger requestedCount = count == NULL ? HCIntegerMaximum : *count;
    const HCContourComponent* componentsStart = HCContourComponents(contour);
    const HCContourComponent* componentsEnd = componentsStart + HCContourComponentCount(contour);
    const HCContourComponent* previousComponent = componentsStart;
    for (const HCContourComponent* component = componentsStart + 1; component != componentsEnd; component++) {
        const HCCurve* curve = (HCCurve*)&previousComponent->p;
        HCInteger curveInflectionCount = 6;
        HCReal curveInflections[curveInflectionCount];
        HCCurveInflections(*curve, &curveInflectionCount, curveInflections);
        memcpy(contourInflections + contourInflectionCount, curveInflections, sizeof(HCReal) * curveInflectionCount);
        contourInflectionCount += curveInflectionCount;
        previousComponent = component;
        if (curveInflectionCount >= requestedCount) {
            break;
        }
    }
    
    // Deliver the results
    HCInteger copyCount = contourInflectionCount < requestedCount ? contourInflectionCount : requestedCount;
    if (inflections != NULL) {
        memcpy(inflections, contourInflections, sizeof(HCReal) * copyCount);
    }
    if (count != NULL) {
        *count = copyCount;
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
        minY = fmin(minY, fmin(component->c0.y, fmin(component->c1.y, component->p.y)));
        maxY = fmax(maxY, fmax(component->c0.y, fmax(component->c1.y, component->p.y)));
    }
    return HCRectangleMakeWithEdges(minX, maxX, minY, maxY);
}

HCRectangle HCContourBounds(const HCContour* contour) {
    // Find union rectangle of bounding rectangles of all components
    HCRectangle bounds = HCRectangleMake(contour->start, HCSizeZero);
    const HCContourComponent* componentsStart = HCContourComponents(contour);
    const HCContourComponent* componentsEnd = componentsStart + HCContourComponentCount(contour);
    const HCContourComponent* previousComponent = componentsStart;
    for (const HCContourComponent* component = componentsStart + 1; component != componentsEnd; component++) {
        const HCCurve* curve = (HCCurve*)&previousComponent->p;
        bounds = HCRectangleUnion(bounds, HCCurveBounds(*curve));
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
        const HCCurve* curve = (HCCurve*)&previousComponent->p;
        length += HCCurveLength(*curve);
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

HCContourComponent HCContourComponentContainingParameter(const HCContour* contour, HCReal t) {
    HCInteger componentIndex = HCContourComponentIndexContainingParameter(contour, t);
    return HCContourComponentAt(contour, componentIndex);
}

HCInteger HCContourComponentIndexContainingParameter(const HCContour* contour, HCReal t) {
    return (HCInteger)floor(fmax(1.0, fmin((HCReal)(HCContourComponentCount(contour) - 1), t * (HCReal)(HCContourComponentCount(contour) + 1)) + 1.0));
}

HCReal HCContourComponentParameterForParameter(const HCContour* contour, HCInteger componentIndex, HCReal t) {
    return fmax(0.0, fmin(1.0, t * (HCReal)(HCContourComponentCount(contour) + 1) - (HCReal)(componentIndex - 1)));
}

HCReal HCContourParameterForComponentParameter(const HCContour* contour, HCInteger componentIndex, HCReal t) {
    return fmax(0.0, fmin(1.0, ((HCReal)(componentIndex - 1) + t) / (HCReal)(HCContourComponentCount(contour) + 1)));
}

const HCContourComponent* HCContourComponents(const HCContour* contour) {
    return (const HCContourComponent*)contour;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Components as Curves
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCContourCurveCount(const HCContour* contour) {
    return HCContourComponentCount(contour) - 1;
}

HCCurve HCContourCurveAt(const HCContour* contour, HCInteger curveIndex) {
    return HCCurveMakeWithContourComponent(HCContourComponentAt(contour, curveIndex).p, HCContourComponentAt(contour, curveIndex + 1));
}

HCCurve HCContourCurveContainingParameter(const HCContour* contour, HCReal t) {
    HCInteger curveIndex = HCContourCurveIndexContainingParameter(contour, t);
    return HCContourCurveAt(contour, curveIndex);
}

HCInteger HCContourCurveIndexContainingParameter(const HCContour* contour, HCReal t) {
    HCInteger componentIndex = HCContourComponentIndexContainingParameter(contour, t);
    return HCContourCurveIndexForComponentIndex(contour, componentIndex);
}

HCReal HCContourCurveParameterForParameter(const HCContour* contour, HCInteger curveIndex, HCReal t) {
    HCInteger componentIndex = HCContourComponentIndexForCurveIndex(contour, curveIndex);
    return HCContourComponentParameterForParameter(contour, componentIndex, t);
}

HCReal HCContourParameterForCurveParameter(const HCContour* contour, HCInteger curveIndex, HCReal t) {
    HCInteger componentIndex = HCContourComponentIndexForCurveIndex(contour, curveIndex);
    return HCContourParameterForComponentParameter(contour, componentIndex, t);
}

HCInteger HCContourCurveIndexForComponentIndex(const HCContour* contour, HCInteger componentIndex) {
    (void)contour; // Unused
    return componentIndex - 1;
}

HCInteger HCContourComponentIndexForCurveIndex(const HCContour* contour, HCInteger curveIndex) {
    (void)contour; // Unused
    return curveIndex + 1;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourValue(const HCContour* contour, HCReal t) {
    HCInteger curveIndex = HCContourCurveIndexContainingParameter(contour, t);
    HCReal ct = HCContourCurveParameterForParameter(contour, curveIndex, t);
    HCCurve curve = HCContourCurveAt(contour, curveIndex);
    return HCCurveValue(curve, ct);
}

HCCurve HCContourTangent(const HCContour* contour, HCReal t) {
    HCInteger curveIndex = HCContourCurveIndexContainingParameter(contour, t);
    HCReal ct = HCContourCurveParameterForParameter(contour, curveIndex, t);
    HCCurve curve = HCContourCurveAt(contour, curveIndex);
    return HCCurveTangent(curve, ct);
}

HCCurve HCContourTangentUnit(const HCContour* contour, HCReal t) {
    HCInteger curveIndex = HCContourCurveIndexContainingParameter(contour, t);
    HCReal ct = HCContourCurveParameterForParameter(contour, curveIndex, t);
    HCCurve curve = HCContourCurveAt(contour, curveIndex);
    return HCCurveTangentUnit(curve, ct);
}

HCCurve HCContourNormal(const HCContour* contour, HCReal t) {
    HCInteger curveIndex = HCContourCurveIndexContainingParameter(contour, t);
    HCReal ct = HCContourCurveParameterForParameter(contour, curveIndex, t);
    HCCurve curve = HCContourCurveAt(contour, curveIndex);
    return HCCurveNormal(curve, ct);
}

HCCurve HCContourNormalUnit(const HCContour* contour, HCReal t)  {
    HCInteger curveIndex = HCContourCurveIndexContainingParameter(contour, t);
    HCReal ct = HCContourCurveParameterForParameter(contour, curveIndex, t);
    HCCurve curve = HCContourCurveAt(contour, curveIndex);
    return HCCurveNormalUnit(curve, ct);
}

HCReal HCContourCurvature(const HCContour* contour, HCReal t) {
    HCInteger curveIndex = HCContourCurveIndexContainingParameter(contour, t);
    HCReal ct = HCContourCurveParameterForParameter(contour, curveIndex, t);
    HCCurve curve = HCContourCurveAt(contour, curveIndex);
    return HCCurveCurvature(curve, ct);
}

HCCurve HCContourCurvatureNormal(const HCContour* contour, HCReal t) {
   HCInteger curveIndex = HCContourCurveIndexContainingParameter(contour, t);
    HCReal ct = HCContourCurveParameterForParameter(contour, curveIndex, t);
    HCCurve curve = HCContourCurveAt(contour, curveIndex);
     return HCCurveCurvatureNormal(curve, ct);
}

HCReal HCContourParameterAtLength(const HCContour* contour, HCReal d) {
    // Walk components until the component containing the desired length is reached
    HCReal length = 0.0;
    HCInteger componentIndex;
    for (componentIndex = 1; componentIndex < HCContourComponentCount(contour); componentIndex++) {
        const HCCurve* curve = (HCCurve*)&HCContourComponents(contour)[componentIndex - 1].p;
        HCReal componentLength = HCCurveLength(*curve);
        if (length + componentLength > d) {
            break;
        }
        length += componentLength;
    }
    
    // When the desired length is longer than the contour, return the end parameter
    if (componentIndex >= HCContourComponentCount(contour)) {
        return 1.0;
    }
    
    // Calculate the remaining length, query the component for the component-relative parameter corresponding to that length, then convert it to be contour-relative
    HCReal remaining = d - length;
    const HCCurve* curve = (HCCurve*)&HCContourComponents(contour)[componentIndex - 1].p;
    HCReal ct = HCCurveParameterAtLength(*curve, remaining);
    HCReal t = HCContourParameterForComponentParameter(contour, componentIndex, ct);
    return t;
}

HCReal HCContourParameterNearestPoint(const HCContour* contour, HCPoint p) {
    // Find the nearest parameter for all components
    HCReal nearestDistance = HCRealMaximumPositive;
    HCReal nearestContourParameter = 0.0;
    HCInteger componentIndex;
    for (componentIndex = 1; componentIndex < HCContourComponentCount(contour); componentIndex++) {
        const HCCurve* curve = (HCCurve*)&HCContourComponents(contour)[componentIndex - 1].p;
        HCReal nearestComponentParameter = HCCurveParameterNearestPoint(*curve, p);
        HCPoint nearestComponentPoint = HCCurveValue(*curve, nearestComponentParameter);
        HCReal distance = HCPointDistance(p, nearestComponentPoint);
        if (nearestDistance > distance) {
            nearestDistance = distance;
            nearestContourParameter = HCContourParameterForComponentParameter(contour, componentIndex, nearestComponentParameter);
        }
    }
    return nearestContourParameter;
}

HCReal HCContourDistanceFromPoint(const HCContour* contour, HCPoint p) {
    HCReal t = HCContourParameterNearestPoint(contour, p);
    HCPoint tp = HCContourValue(contour, t);
    return HCPointDistance(p, tp);
}

void HCContourIntersections(const HCContour* pContour, const HCContour* qContour, HCInteger* count, HCReal* t, HCReal* u) {
    // Compare each component curve of each contour against one another for intersections
    HCInteger possibleIntersections = (HCContourComponentCount(pContour) + HCContourComponentCount(qContour)) * 9;
    HCReal contourIntersectionTs[possibleIntersections];
    HCReal contourIntersectionUs[possibleIntersections];
    HCInteger contourIntersectionCount = 0;
    HCInteger requestedCount = count == NULL ? HCIntegerMaximum : *count;
    for (HCInteger pComponentIndex = 1; pComponentIndex < HCContourComponentCount(pContour); pComponentIndex++) {
        // Extract the curve associated with this component
        const HCCurve* pCurve = (HCCurve*)&HCContourComponents(pContour)[pComponentIndex - 1].p;
        
        // Compare this component curve against all component curves of the other contour
        for (HCInteger qComponentIndex = 1; qComponentIndex < HCContourComponentCount(qContour); qComponentIndex++) {
            // Extract the curve associated with this component
            const HCCurve* qCurve = (HCCurve*)&HCContourComponents(qContour)[qComponentIndex - 1].p;
            
            // Determine how many more intersections have been requested to be found
            HCInteger remaining = requestedCount - contourIntersectionCount;
            
            // Find intersections between component curves
            HCInteger curveIntersectionCount = remaining < 9 ? remaining : 9;
            HCReal curveIntersectionTs[curveIntersectionCount];
            HCReal curveIntersectionUs[curveIntersectionCount];
            HCCurveIntersection(*pCurve, *qCurve, &curveIntersectionCount, curveIntersectionTs, curveIntersectionUs);
            
            // Convert intersection parameters from curve-relative 0...1 to contour-relative 0...1
            for (HCInteger curveIntersectionIndex = 0; curveIntersectionIndex < curveIntersectionCount; curveIntersectionIndex++) {
                HCReal curveT = curveIntersectionTs[curveIntersectionIndex];
                HCReal contourT = HCContourParameterForComponentParameter(pContour, pComponentIndex, curveT);
                contourIntersectionTs[contourIntersectionCount] = contourT;
                HCReal curveU = curveIntersectionUs[curveIntersectionIndex];
                HCReal contourU = HCContourParameterForComponentParameter(qContour, qComponentIndex, curveU);
                contourIntersectionUs[contourIntersectionCount] = contourU;
                contourIntersectionCount++;
            }
            
            // Quit searching other contour if enough intersections have been found
            if (contourIntersectionCount >= requestedCount) {
                break;
            }
        }
        
        // Quit searching contour if enough intersections have been found
        if (contourIntersectionCount >= requestedCount) {
            break;
        }
    }
    
    // Deliver results
    HCInteger copyCount = contourIntersectionCount < requestedCount ? contourIntersectionCount : requestedCount;
    if (t != NULL) {
        memcpy(t, contourIntersectionTs, sizeof(HCReal) * copyCount);
    }
    if (u != NULL) {
        memcpy(u, contourIntersectionUs, sizeof(HCReal) * copyCount);
    }
    if (count != NULL) {
        *count = copyCount;
    }
}
