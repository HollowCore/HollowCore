///
/// @file HCPath.c
/// @package HollowCore
///
/// @author Matt Stoker
/// @date 12/28/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCPath_Internal.h"
#include <math.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCPathTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCPath",
    },
    .isEqual = (void*)HCPathIsEqual,
    .hashValue = (void*)HCPathHashValue,
    .print = (void*)HCPathPrint,
    .destroy = (void*)HCPathDestroy,
};
HCType HCPathType = (HCType)&HCPathTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCPathRef HCPathCreate() {
    HCPathRef self = calloc(sizeof(HCPath), 1);
    HCPathInit(self);
    return self;
}

HCPathRef HCPathCreateWithElements(HCPathElement* elements, HCInteger elementCount) {
    HCPathRef self = HCPathCreate();
    for (HCInteger elementIndex = 0; elementIndex < elementCount; elementIndex++) {
        HCPathElement element = elements[elementIndex];
        switch (element.command) {
            case HCPathCommandMove: HCPathMove(self, element.points[0].x, element.points[0].y); break;
            case HCPathCommandAddLine: HCPathAddLine(self, element.points[0].x, element.points[0].y); break;
            case HCPathCommandAddQuadraticCurve: HCPathAddQuadraticCurve(self, element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y); break;
            case HCPathCommandAddCubicCurve: HCPathAddCubicCurve(self, element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y, element.points[2].x, element.points[2].y); break;
            case HCPathCommandCloseContour: HCPathClose(self); break;
        }
    }
    return self;
}

HCPathRef HCPathCreateWithSubpaths(HCListRef subpaths) {
    // TODO: Can this be done more efficiently, sharing subpath polyline and contour calculations?
    HCPathRef self = HCPathCreate();
    for (HCListIterator i = HCListIterationBegin(subpaths); !HCListIterationHasEnded(&i); HCListIterationNext(&i)) {
        HCInteger elementCount = HCPathElementCount(i.object);
        for (HCInteger elementIndex = 0; elementIndex < elementCount; elementIndex++) {
            HCPathElement element = HCPathElementAt(i.object, elementIndex);
            switch (element.command) {
                case HCPathCommandMove: HCPathMove(self, element.points[0].x, element.points[0].y); break;
                case HCPathCommandAddLine: HCPathAddLine(self, element.points[0].x, element.points[0].y); break;
                case HCPathCommandAddQuadraticCurve: HCPathAddQuadraticCurve(self, element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y); break;
                case HCPathCommandAddCubicCurve: HCPathAddCubicCurve(self, element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y, element.points[2].x, element.points[2].y); break;
                case HCPathCommandCloseContour: HCPathClose(self); break;
            }
        }
    }
    return self;
}

HCPathRef HCPathCreateWithContour(const HCContour* contour) {
    return HCPathCreateWithContourCurves(HCContourCurves(contour), HCContourCurveCount(contour), HCContourIsClosed(contour));
}

HCPathRef HCPathCreateWithContourCurves(const HCContourCurve* curves, HCInteger curveCount, HCBoolean closed) {
    HCPathRef self = HCPathCreate();
    if (curveCount <= 0) {
        return self;
    }
    HCPathMove(self, curves[0].p.x, curves[0].p.y);
    for (HCInteger curveIndex = 1; curveIndex < curveCount; curveIndex++) {
        HCContourCurve contourCurve = curves[curveIndex];
        if (HCContourCurveIsLinear(contourCurve)) {
            HCPathAddLine(self, contourCurve.p.x, contourCurve.p.y);
        }
        else if (HCContourCurveIsQuadratic(contourCurve)) {
            HCPathAddQuadraticCurve(self, contourCurve.c0.x, contourCurve.c0.y, contourCurve.p.x, contourCurve.p.y);
        }
        else if (HCContourCurveIsCubic(contourCurve)) {
            HCPathAddCubicCurve(self, contourCurve.c0.x, contourCurve.c0.y, contourCurve.c1.x, contourCurve.c1.y, contourCurve.p.x, contourCurve.p.y);
        }

    }
    if (closed) {
        HCPathClose(self);
    }
    return self;
}

void HCPathInit(void* memory) {
    // Construct path object
    HCObjectInit(memory);
    HCPathRef self = memory;
    self->base.type = HCPathType;
    self->elementData = HCDataCreate();
    self->polylines = HCListCreate();
    self->contours = HCListCreate();
    self->bounds = HCRectangleZero;
}

void HCPathDestroy(HCPathRef self) {
    HCInteger elementCount = HCPathElementCount(self);
    for (HCInteger elementIndex = 0; elementIndex < elementCount; elementIndex++) {
        HCPathElement element = HCPathElementAt(self, elementIndex);
        if (element.points != NULL) {
            free(element.points);
        }
    }
    HCRelease(self->elementData);
    HCRelease(self->polylines);
    HCRelease(self->contours);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPathIsEqual(HCPathRef self, HCPathRef other) {
    HCInteger elementCount = HCPathElementCount(self);
    HCInteger otherElementCount = HCPathElementCount(other);
    if (elementCount != otherElementCount) {
        return false;
    }
    for (HCInteger elementIndex = 0; elementIndex < elementCount; elementIndex++) {
        HCPathElement element = HCPathElementAt(self, elementIndex);
        HCPathElement otherElement = HCPathElementAt(other, elementIndex);
        if (element.command != otherElement.command) {
            return false;
        }
        switch (element.command) {
            case HCPathCommandMove:
                if (!HCPointIsEqual(element.points[0], otherElement.points[0])) {
                    return false;
                }
                break;
            case HCPathCommandAddLine:
                if (!HCPointIsEqual(element.points[0], otherElement.points[0])) {
                    return false;
                }
                break;
            case HCPathCommandAddQuadraticCurve:
                if (!HCPointIsEqual(element.points[0], otherElement.points[0]) || !HCPointIsEqual(element.points[1], otherElement.points[1])) {
                    return false;
                }
                break;
            case HCPathCommandAddCubicCurve:
                if (!HCPointIsEqual(element.points[0], otherElement.points[0]) || !HCPointIsEqual(element.points[1], otherElement.points[1]) || !HCPointIsEqual(element.points[2], otherElement.points[2])) {
                    return false;
                }
                break;
            case HCPathCommandCloseContour:
                break;
        }
    }
    return true;
}

HCInteger HCPathHashValue(HCPathRef self) {
    HCInteger hash = 5381;
    HCInteger elementCount = HCPathElementCount(self);
    for (HCInteger elementIndex = 0; elementIndex < elementCount; elementIndex++) {
        HCPathElement element = HCPathElementAt(self, elementIndex);
        hash ^= HCIntegerHashValue(element.command);
        switch (element.command) {
            case HCPathCommandMove: hash ^= HCPointHashValue(element.points[0]); break;
            case HCPathCommandAddLine: hash ^= HCPointHashValue(element.points[0]); break;
            case HCPathCommandAddQuadraticCurve: hash ^= HCPointHashValue(element.points[0]) ^ HCPointHashValue(element.points[1]); break;
            case HCPathCommandAddCubicCurve: hash ^= HCPointHashValue(element.points[0]) ^ HCPointHashValue(element.points[1]) ^ HCPointHashValue(element.points[2]); break;
            case HCPathCommandCloseContour: break;
        }
    }
    return hash;
}

void HCPathPrint(HCPathRef self, FILE* stream) {
    fprintf(stream, "<%s@%p>,path:", self->base.type->name, self);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCPathIsEmpty(HCPathRef self) {
    return HCPathElementCount(self) == 0;
}

HCInteger HCPathElementCount(HCPathRef self) {
    return HCDataSize(self->elementData) / sizeof(HCPathElement);
}

HCPathElement HCPathElementAt(HCPathRef self, HCInteger elementIndex) {
    HCPathElement* elements = (HCPathElement*)HCDataBytes(self->elementData);
    return elements[elementIndex];
}

HCRectangle HCPathBounds(HCPathRef self) {
    return self->bounds;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contours
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCPathContourCount(HCPathRef self) {
    return HCListCount(self->contours);
}

HCBoolean HCPathContourIsClosed(HCPathRef self, HCInteger contourIndex) {
    return HCContourIsClosed(HCPathContourAt(self, contourIndex));
}

HCInteger HCPathContourCurveCount(HCPathRef self, HCInteger contourIndex) {
    HCDataRef contourData = HCListObjectAtIndex(self->contours, contourIndex);
    return HCDataSize(contourData) / sizeof(HCContourCurve);
}

HCContourCurve HCPathContourCurveAt(HCPathRef self, HCInteger contourIndex, HCInteger curveIndex) {
    HCDataRef contourData = HCListObjectAtIndex(self->contours, contourIndex);
    HCContourCurve* curves = (HCContourCurve*)HCDataBytes(contourData);
    return curves[curveIndex];
}

const HCContour* HCPathContourAt(HCPathRef self, HCInteger contourIndex) {
    return (HCContour*)HCDataBytes(HCListObjectAtIndex(self->contours, contourIndex));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contour / Path Element Correspondence
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCPathIndexOfContourContainingElement(HCPathRef self, HCInteger elementIndex) {
    // Find contour corresponding to element
    // TODO: Cache?
    HCInteger contourIndex = -1;
    HCInteger endIndex = 0;
    while (elementIndex >= endIndex) {
        HCPathContourContainingElementIsClosed(self, endIndex, NULL, &endIndex);
        contourIndex++;
    }
    return contourIndex;
}

HCBoolean HCPathContourContainingElementIsClosed(HCPathRef self, HCInteger elementIndex, HCInteger* startIndex, HCInteger* endIndex) {
    // Search for contour start element, if requested (not needed to determine if contour is open)
    if (startIndex != NULL) {
        *startIndex = 0;
        for (HCInteger contourStartSearchIndex = elementIndex; contourStartSearchIndex >= 0; contourStartSearchIndex--) {
            HCPathCommand command = HCPathElementAt(self, contourStartSearchIndex).command;
            if (command == HCPathCommandMove) {
                *startIndex = contourStartSearchIndex;
                break;
            }
        }
    }
    
    // Search for contour end element and determine if it is open
    HCInteger elementCount = HCPathElementCount(self);
    for (HCInteger contourEndSearchIndex = elementIndex; contourEndSearchIndex < elementCount; contourEndSearchIndex++) {
        HCPathCommand command = HCPathElementAt(self, contourEndSearchIndex).command;
        if (command == HCPathCommandMove && contourEndSearchIndex != elementIndex) {
            // Contour ends with a move, so it is open
            if (endIndex != NULL) {
                *endIndex = contourEndSearchIndex;
            }
            return false;
        }
        if (command == HCPathCommandCloseContour) {
            // Contour ends with a close-contour, so it is closed
            if (endIndex != NULL) {
                *endIndex = contourEndSearchIndex + 1;
            }
            return true;
        }
    }
    
    // Reached end without finding a move-to or close-contour command, so the path is open
    if (endIndex != NULL) {
        *endIndex = elementCount;
    }
    return false;
}

HCPathRef HCPathContourPathContainingElementRetained(HCPathRef self, HCInteger elementIndex, HCInteger* startIndex, HCInteger* endIndex, HCBoolean* closed) {
    // Determine contour indices
    HCInteger contourStartIndex = 0;
    HCInteger contourEndIndex = 0;
    HCBoolean isClosed = HCPathContourContainingElementIsClosed(self, elementIndex, &contourStartIndex, &contourEndIndex);
    if (startIndex != NULL) {
        *startIndex = contourStartIndex;
    }
    if (endIndex != NULL) {
        *endIndex = contourEndIndex;
    }
    if (closed != NULL) {
        *closed = isClosed;
    }
    
    // Create the contour path with the elements from start to end
    HCPathElement* elements = (HCPathElement*)HCDataBytes(self->elementData);
    HCPathRef contourPath = HCPathCreateWithElements(elements + contourStartIndex, contourEndIndex - contourStartIndex);
    return contourPath;
}

HCListRef HCPathContourPathsRetained(HCPathRef self) {
    HCListRef contourPaths = HCListCreate();
    HCInteger elementCount = HCPathElementCount(self);
    HCInteger elementIndex = 0;
    while (elementIndex < elementCount) {
        HCPathRef contourPath = HCPathContourPathContainingElementRetained(self, elementIndex, NULL, &elementIndex, NULL);
        if (!HCPathIsEmpty(contourPath)) {
            HCListAddObjectReleased(contourPaths, contourPath);
        }
    }
    return contourPaths;
}

HCListRef HCPathOpenContourPathsRetained(HCPathRef self) {
    HCListRef openContourPaths = HCListCreate();
    HCInteger elementCount = HCPathElementCount(self);
    HCInteger elementIndex = 0;
    while (elementIndex < elementCount) {
        // Find the contour start, end, and openness
        HCInteger startIndex;
        HCInteger endIndex;
        HCBoolean closed = HCPathContourContainingElementIsClosed(self, elementIndex, &startIndex, &endIndex);
        elementIndex = endIndex;
        
        // If open, create the contour path with the elements from start to end
        if (!closed) {
            HCPathElement* elements = (HCPathElement*)HCDataBytes(self->elementData);
            HCPathRef contourPath = HCPathCreateWithElements(elements + startIndex, endIndex - startIndex);
            if (!HCPathIsEmpty(contourPath)) {
                HCListAddObjectReleased(openContourPaths, contourPath);
            }
        }
    }
    return openContourPaths;
}

HCListRef HCPathClosedContourPathsRetained(HCPathRef self) {
    HCListRef closedContourPaths = HCListCreate();
    HCInteger elementCount = HCPathElementCount(self);
    HCInteger elementIndex = 0;
    while (elementIndex < elementCount) {
        // Find the contour start, end, and openness
        HCInteger startIndex;
        HCInteger endIndex;
        HCBoolean closed = HCPathContourContainingElementIsClosed(self, elementIndex, &startIndex, &endIndex);
        elementIndex = endIndex;
        
        // If closed, create the contour path with the elements from start to end
        if (closed) {
            HCPathElement* elements = (HCPathElement*)HCDataBytes(self->elementData);
            HCPathRef contourPath = HCPathCreateWithElements(elements + startIndex, endIndex - startIndex);
            if (!HCPathIsEmpty(contourPath)) {
                HCListAddObjectReleased(closedContourPaths, contourPath);
            }
        }
    }
    return closedContourPaths;
}

HCPathRef HCPathOpenContoursAsPathRetained(HCPathRef self) {
    HCListRef openContours = HCPathOpenContourPathsRetained(self);
    HCPathRef openPath = HCPathCreateWithSubpaths(openContours);
    HCRelease(openContours);
    return openPath;
}

HCPathRef HCPathClosedContoursAsPathRetained(HCPathRef self) {
    HCListRef closedContours = HCPathClosedContourPathsRetained(self);
    HCPathRef closedPath = HCPathCreateWithSubpaths(closedContours);
    HCRelease(closedContours);
    return closedPath;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Polylines
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCPathPolylineCount(HCPathRef self) {
    return HCPathElementCount(self);
}

HCInteger HCPathPolylinePointCount(HCPathRef self, HCInteger polylineIndex) {
    HCDataRef polylineData = HCListObjectAtIndex(self->polylines, polylineIndex);
    return HCDataSize(polylineData) / sizeof(HCPoint);
}

HCPoint HCPathPolylinePointAt(HCPathRef self, HCInteger polylineIndex, HCInteger pointIndex) {
    HCDataRef polylineData = HCListObjectAtIndex(self->polylines, polylineIndex);
    HCPoint* points = (HCPoint*)HCDataBytes(polylineData);
    return points[pointIndex];
}

const HCPoint* HCPathPolylineAt(HCPathRef self, HCInteger polylineIndex) {
    return (HCPoint*)HCDataBytes(HCListObjectAtIndex(self->polylines, polylineIndex));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Polyline / Path Element Correspondence
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCPathIndexOfPolylineContainingElement(HCPathRef self, HCInteger elementIndex) {
    (void)self; // unused
    
    // Polyline indices and element indices are currently equivalent, as empty polylines are stored for move commands
    // TODO: Remove empty polylines representing moves
    return elementIndex;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Intersection
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPathContainsPoint(HCPathRef self, HCPoint point) {
    // Determine how many crossings there are for a ray from the point going in the +x direction
    // Rather than construct a path and the intersection function, this function uses custom intersection code similar to the intersection function
    HCInteger intersectionCount = 0;
    HCPoint distantPoint = HCPointMake(self->bounds.size.width * 2.0, point.y);
    HCPoint q0 = point;
    HCPoint q1 = distantPoint;
    HCInteger elementCount = HCPathElementCount(self);
    HCInteger elementIndex = 0;
    while (elementIndex < elementCount) {
        // Determine if the current contour is open, and skip it if it is
        HCInteger endIndex = 0;
        HCBoolean closed = HCPathContourContainingElementIsClosed(self, elementIndex, NULL, &endIndex);
        if (!closed) {
            // Move to the next contour
            elementIndex = endIndex;
            continue;
        }
        
        // Extract the initial point of the polyline and use it as the start point of the first line segment, or skip it if it is empty (e.g. a move-to element)
        HCInteger polylinePointCount = HCPathPolylinePointCount(self, elementIndex);
        if (polylinePointCount <= 0) {
            // Move to the next element
            elementIndex++;
            continue;
        }
        HCPoint p0 = HCPathPolylinePointAt(self, elementIndex, 0);
        
        // Compare the line segments of the polyline against the ray
        for (HCInteger pointIndex = 1; pointIndex < polylinePointCount; pointIndex++) {
            // Determine the end point of the line segment from the polyline
            HCPoint p1 = HCPathPolylinePointAt(self, elementIndex, pointIndex);
            
            // Find the intersection parameters for the lines formed by the line segment points
            // TODO: Could performance be improved using a simpler intersection function that is for line-segment to ray?
            HCReal t = 0.0;
            HCReal u = 0.0;
            HCContourLineLineIntersection(p0, p1, q0, q1, &t, &u);
            
            // Determine if they intersect within the bounds of the segments
            HCBoolean segmentsIntersect = t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0;
            if (segmentsIntersect) {
                intersectionCount++;
            }
            
            // Use the end point of the polyline line segment as the start point of the next
            p0 = p1;
        }
        
        // Move to the next element
        elementIndex++;
    }
    
    // An odd intersection count indicates the point is within the path
    return intersectionCount % 2 == 1;
}

HCBoolean HCPathContainsPointNonZero(HCPathRef self, HCPoint point) {\
    (void)self; // unused
    (void)point; // unused
    
    // TODO: This!
    return false;
}

HCBoolean HCPathIntersectsPath(HCPathRef self, HCPathRef other) {
    HCBoolean intersects = false;
    HCPathIntersections(self, other, HCPathIntersects, &intersects);
    return intersects;
}

void HCPathIntersections(HCPathRef self, HCPathRef other, HCPathIntersectionFunction intersection, void* context) {
    // Compare each polyline segment in the path against those in the other path
    HCInteger elementCount = HCPathElementCount(self);
    for (HCInteger elementIndex = 0; elementIndex < elementCount; elementIndex++) {
        // Extract the initial point of the polyline and use it as the start point of the first line segment, or skip it if it is empty (e.g. a move-to element)
        HCInteger polylinePointCount = HCPathPolylinePointCount(self, elementIndex);
        if (polylinePointCount <= 0) {
            continue;
        }
        HCPoint p0 = HCPathPolylinePointAt(self, elementIndex, 0);
        
        // Compare the line segments of the polyline against the other path
        for (HCInteger pointIndex = 1; pointIndex < polylinePointCount; pointIndex++) {
            // Determine the end point of the line segment from the polyline
            HCPoint p1 = HCPathPolylinePointAt(self, elementIndex, pointIndex);
            HCInteger otherElementCount = HCPathElementCount(other);
            for (HCInteger otherElementIndex = 0; otherElementIndex < otherElementCount; otherElementIndex++) {
                // Extract the initial point of the other path polyline and use it as the start point of it's first line segment, or skip it if it is empty
                HCInteger otherPolylinePointCount = HCPathPolylinePointCount(other, otherElementIndex);
                if (otherPolylinePointCount <= 0) {
                    continue;
                }
                HCPoint q0 = HCPathPolylinePointAt(other, otherElementIndex, 0);
                
                // Compare the line segments of the other polyline against the current line segment in the path
                for (HCInteger otherPointIndex = 0; otherPointIndex < otherPolylinePointCount; otherPointIndex++) {
                    // Determine the end point of the line segment from the other polyline
                    HCPoint q1 = HCPathPolylinePointAt(other, otherElementIndex, otherPointIndex);
                    
                    // Find the intersection parameters for the lines formed by the line segment points
                    HCReal t = 0.0;
                    HCReal u = 0.0;
                    HCContourLineLineIntersection(p0, p1, q0, q1, &t, &u);
                    
                    // Determine if they intersect within the bounds of the segments
                    HCBoolean segmentsIntersect = t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0;
                    if (segmentsIntersect) {
                        // Calculate the intersection point and call the intersection function
                        HCReal tc = 1.0 - t;
                        HCPoint point = HCPointMake(tc * p0.x + t * p1.x, tc * p0.y + t * p1.y);
                        HCBoolean stopSearching = false;
                        intersection(context, &stopSearching, self, other, point);
                        if (stopSearching) {
                            return;
                        }
                    }
                    
                    // Use the end point of the other polyline line segment as the start point of the next
                    q0 = q1;
                }
            }
            
            // Use the end point of the polyline line segment as the start point of the next
            p0 = p1;
        }
    }
}

void HCPathIntersects(void* context, HCBoolean* stopSearching, HCPathRef path, HCPathRef otherPath, HCPoint point) {
    (void)path; // unused
    (void)otherPath; // unused
    (void)point; // unused
    
    *((HCBoolean*)context) = true;
    *stopSearching = true;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Manipulation
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCPathCurrentPoint(HCPathRef self) {
    if (!HCPathIsEmpty(self)) {
        // Find the end point of the last command
        HCPathElement endElement = HCPathElementAt(self, HCPathElementCount(self) - 1);
        switch (endElement.command) {
            case HCPathCommandMove: return endElement.points[0];
            case HCPathCommandAddLine: return endElement.points[0];
            case HCPathCommandAddQuadraticCurve: return endElement.points[1];
            case HCPathCommandAddCubicCurve:  return endElement.points[2];
            case HCPathCommandCloseContour: {
                // Find the move element most recently emitted
                for (HCInteger elementIndex = HCPathElementCount(self) - 1; elementIndex >= 0; elementIndex--) {
                    HCPathElement element = HCPathElementAt(self, elementIndex);
                    if (element.command == HCPathCommandMove) {
                        return element.points[0];
                    }
                }
            }
        }
    }
    
    // No end point found, so return the origin, the defined default current point
    return HCPointZero;
}

const HCContour* HCPathCurrentContour(HCPathRef self) {
    HCDataRef contourData = HCListLastObject(self->contours);
    return contourData == NULL ? NULL : (const HCContour*)HCDataBytes(contourData);
}

void HCPathMove(HCPathRef self, HCReal x, HCReal y) {
    HCPoint points[1];
    points[0].x = x;
    points[0].y = y;
    HCPathAppendElement(self, HCPathCommandMove, points);
}

void HCPathAddLine(HCPathRef self, HCReal x, HCReal y) {
    HCPoint points[1];
    points[0].x = x;
    points[0].y = y;
    HCPathAppendElement(self, HCPathCommandAddLine, points);
}

void HCPathAddQuadraticCurve(HCPathRef self, HCReal cx, HCReal cy, HCReal x, HCReal y) {
    HCPoint points[2];
    points[0].x = cx;
    points[0].y = cy;
    points[1].x = x;
    points[1].y = y;
    HCPathAppendElement(self, HCPathCommandAddQuadraticCurve, points);
}

void HCPathAddCubicCurve(HCPathRef self, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x, HCReal y) {
    HCPoint points[3];
    points[0].x = cx0;
    points[0].y = cy0;
    points[1].x = cx1;
    points[1].y = cy1;
    points[2].x = x;
    points[2].y = y;
    HCPathAppendElement(self, HCPathCommandAddCubicCurve, points);
}

void HCPathClose(HCPathRef self) {
    HCPathAppendElement(self, HCPathCommandCloseContour, NULL);
}

void HCPathAppendElement(HCPathRef self, HCPathCommand command, const HCPoint* points) {
    // Copy the current point of the path so it can be used in polyline computations
    HCPoint currentPoint = HCPathCurrentPoint(self);
    
    // Assemble element and copy points
    HCPathElement element;
    switch (command) {
        case HCPathCommandMove:
            element.command = HCPathCommandMove;
            element.points = (HCPoint*)malloc(sizeof(HCPoint) * 1);
            element.points[0] = points[0];
            break;
        case HCPathCommandAddLine:
            element.command = HCPathCommandAddLine;
            element.points = (HCPoint*)malloc(sizeof(HCPoint) * 1);
            element.points[0] = points[0];
            break;
        case HCPathCommandAddQuadraticCurve:
            element.command = HCPathCommandAddQuadraticCurve;
            element.points = (HCPoint*)malloc(sizeof(HCPoint) * 2);
            element.points[0] = points[0];
            element.points[1] = points[1];
            break;
        case HCPathCommandAddCubicCurve:
            element.command = HCPathCommandAddCubicCurve;
            element.points = (HCPoint*)malloc(sizeof(HCPoint) * 3);
            element.points[0] = points[0];
            element.points[1] = points[1];
            element.points[2] = points[2];
            break;
        case HCPathCommandCloseContour:
            element.command = HCPathCommandCloseContour;
            element.points = NULL;
            break;
    }
    
    // Add element to path
    HCDataAddBytes(self->elementData, sizeof(element), (const HCByte*)&element);
    
    // Add curve data from element to current contour
    switch (element.command) {
        case HCPathCommandMove: {
            // Begin a new contour (and implicitly finish any existing contour) with the move element point
            HCContourCurve start = HCContourCurveMakeLinear(element.points[0]);
            HCDataRef contourData = HCDataCreateWithBytes(sizeof(start), (HCByte*)&start);
            HCListAddObjectReleased(self->contours, contourData);
            HCContourInitInCurves((HCContourCurve*)HCDataBytes(contourData), HCDataSize(contourData) / sizeof(HCContourCurve), false);
        } break;
        case HCPathCommandAddLine:
        case HCPathCommandAddQuadraticCurve:
        case HCPathCommandAddCubicCurve: {
            // Obtain the current contour to add the curve
            HCDataRef contourData = HCListLastObject(self->contours);
            if (contourData == NULL || HCContourIsClosed((HCContour*)HCDataBytes(contourData))) {
                // Contour has not yet been started, so begin a new contour at the current point (default is the origin if path was empty)
                HCContourCurve start = HCContourCurveMakeLinear(currentPoint);
                contourData = HCDataCreateWithBytes(sizeof(start), (HCByte*)&start);
                HCListAddObjectReleased(self->contours, contourData);
            }
            
            // Construct the contour curve that best represents the element
            HCContourCurve contourCurve = HCContourCurveInvalid;
            switch (element.command) {
                case HCPathCommandAddLine: contourCurve = HCContourCurveMakeLinear(element.points[0]); break;
                case HCPathCommandAddQuadraticCurve: contourCurve = HCContourCurveMakeQuadratic(element.points[0], element.points[1]); break;
                case HCPathCommandAddCubicCurve: contourCurve = HCContourCurveMakeCubic(element.points[0], element.points[1], element.points[2]); break;
                default: break;
            }
            
            // Add the contour curve to the current contour and update the contour atlas
            HCDataAddBytes(contourData, sizeof(contourCurve), (HCByte*)&contourCurve);
            HCContourInitInCurves((HCContourCurve*)HCDataBytes(contourData), HCDataSize(contourData) / sizeof(HCContourCurve), false);
        } break;
        case HCPathCommandCloseContour: {
            // Mark the current contour as closed (if there is one to mark, as a path can be closed with no other elements)
            HCDataRef contourData = HCListLastObject(self->contours);
            if (contourData != NULL) {
                HCContourInitInCurves((HCContourCurve*)HCDataBytes(contourData), HCDataSize(contourData) / sizeof(HCContourCurve), true);
            }
        } break;
    }
    
    // Calculate polyline data for element
    HCDataRef polylineData = HCDataCreate();
    switch (element.command) {
        case HCPathCommandMove:
            break;
        case HCPathCommandAddLine:
            HCPathAddLineSegmentPolylineData(self, currentPoint.x, currentPoint.y, element.points[0].x, element.points[0].y, polylineData);
            break;
        case HCPathCommandAddQuadraticCurve:
            HCPathAddQuadraticCurvePolylineData(self, currentPoint.x, currentPoint.y, element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y, HCPathFlatnessNormal, polylineData);
            break;
        case HCPathCommandAddCubicCurve:
            HCPathAddCubicCurvePolylineData(self, currentPoint.x, currentPoint.y, element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y, element.points[2].x, element.points[2].y, HCPathFlatnessNormal, polylineData);
            break;
        case HCPathCommandCloseContour: {
            HCPoint closeLineSegmentStart = currentPoint;
            HCPoint closeLineSegmentEnd = HCPathCurrentPoint(self);
            HCPathAddLineSegmentPolylineData(self, closeLineSegmentStart.x, closeLineSegmentStart.y, closeLineSegmentEnd.x, closeLineSegmentEnd.y, polylineData);
        } break;
    }
    
    // Add element polyline data
    HCListAddObjectReleased(self->polylines, polylineData);
    
    // Expand bounds to include polyline data
    // TODO: Can do this based on bezier curve extrema instead of polyline data? E.g. start point, end point, inflection points
    if (HCPathElementCount(self) == 1 && element.command == HCPathCommandMove) {
        self->bounds = HCRectangleMake(element.points[0], HCSizeZero);
    }
    else {
        HCInteger elementIndex = HCPathElementCount(self) - 1;
        HCInteger polylinePointCount = HCPathPolylinePointCount(self, elementIndex);
        HCRectangle bounds = self->bounds;
        for (HCInteger pointIndex = 0; pointIndex < polylinePointCount; pointIndex++) {
            HCPoint point = HCPathPolylinePointAt(self, elementIndex, pointIndex);
            bounds = HCRectangleIncludingPoint(bounds, point);
        }
        self->bounds = bounds;
    }
}

void HCPathRemoveElement(HCPathRef self) {
    // Determine if there are elements to remove
    HCInteger elementCount = HCPathElementCount(self);
    if (elementCount == 0) {
        return;
    }
    
    // Find the element to be removed
    HCPathElement element = HCPathElementAt(self, elementCount - 1);
    
    // Remove corresponding property of element from current contour
    switch (element.command) {
        case HCPathCommandMove: {
            // Remove contour started by this move element
            HCListRemoveObject(self->contours);
        } break;
        case HCPathCommandAddLine:
        case HCPathCommandAddQuadraticCurve:
        case HCPathCommandAddCubicCurve: {
            // Remove corresponding curve from contour
            HCDataRef contourData = HCListLastObject(self->contours);
            HCDataRemoveBytes(contourData, sizeof(HCContourCurve));
            
            // Remove contour if this was the last curve in it, otherwise update the contour atlas
            HCInteger curveCount = HCDataSize(contourData) / sizeof(HCContourCurve);
            if (curveCount == 0) {
                HCListRemoveObject(self->contours);
            }
            else {
                HCContourInitInCurves((HCContourCurve*)HCDataBytes(contourData), curveCount, false);
            }
        }
        case HCPathCommandCloseContour: {
            // Mark the current contour as open (if there is one to mark, as a path can be closed with no other elements)
            HCDataRef contourData = HCListLastObject(self->contours);
            if (contourData != NULL) {
                HCInteger curveCount = HCDataSize(contourData) / sizeof(HCContourCurve);
                HCContourInitInCurves((HCContourCurve*)HCDataBytes(contourData), curveCount, false);
            }
        } break;
    }
    
    // When this is the last element to be removed in the path, ensure the last contour is removed (accounts for paths starting without a move command)
    if (elementCount <= 1) {
        HCListClear(self->contours);
    }
    
    // Remove element polyline data
    HCListRemoveObject(self->polylines);
    
    // Remove element
    if (element.points != NULL) {
        free(element.points);
    }
    HCDataRemoveBytes(self->elementData, sizeof(HCPathElement));
    elementCount = HCPathElementCount(self);
    
    // Re-calculate bounds to exclude the element polylines
    // TODO: Can do this based on bezier curve extrema instead of polyline data? E.g. start point, end point, inflection points
    HCRectangle bounds = HCRectangleMake(HCPathCurrentPoint(self), HCSizeZero);
    for (HCInteger elementIndex = 0; elementIndex < elementCount; elementIndex++) {
        HCInteger polylinePointCount = HCPathPolylinePointCount(self, elementIndex);
        for (HCInteger pointIndex = 0; pointIndex < polylinePointCount; pointIndex++) {
            HCPoint point = HCPathPolylinePointAt(self, elementIndex, pointIndex);
            bounds = HCRectangleIncludingPoint(bounds, point);
        }
    }
    self->bounds = bounds;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Polyline Conversion
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathAddLineSegmentPolylineData(HCPathRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCDataRef polylineData) {
    (void)self; // unused
    
    // Start the polyline using the segment start point if the polyline is empty
    if (HCDataIsEmpty(polylineData)) {
        HCDataAddReal(polylineData, x0);
        HCDataAddReal(polylineData, y0);
    }
    
    // Add the segment end to the polyline
    HCDataAddReal(polylineData, x1);
    HCDataAddReal(polylineData, y1);
}

void HCPathAddQuadraticCurvePolylineData(HCPathRef self, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCReal flatnessThreshold, HCDataRef polylineData) {
    // Calculate using direct evaluation informed by derivitive
//    HCReal x, y, dx, dy;
//    for (HCReal t = 0.0; t <= 1.0;) {
//        HCPathEvaluateQuadraticCurve(t, x0, y0, cx, cy, x1, y1, &x, &y, &dx, &dy);
//        HCPathDrawPoint(self, x, y, HCColorCombine(c0, c1, t));
//        t += fmax(0.00001, 0.50000 / fmax(fabs(dx), fabs(dy)));
//    }
    
    // Calculate the curve polyline using De Casteljau's Algorithm
    
    // Determine quadratic curve flatness
    HCReal p0cDistance = sqrt((cx - x0) * (cx - x0) + (cy - y0) * (cy - y0));
    HCReal cp1Distance = sqrt((x1 - cx) * (x1 - cx) + (y1 - cy) * (y1 - cy));
    HCReal p0p1Distance = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    HCReal flatness = (p0cDistance + cp1Distance) / p0p1Distance;
    if (flatness < flatnessThreshold || HCRealIsSimilar(p0p1Distance, 0.0, FLT_EPSILON)) {
        // Flatness of the curve is sufficient to approximate it as a line segment, so add it to the
        HCPathAddLineSegmentPolylineData(self, x0, y0, x1, y1, polylineData);
        return;
    }

    // Split the quadratic curve at t = 0.5 and recursively add their polyline data
    HCReal qx0 =  x0 * 0.5 +  cx * 0.5;
    HCReal qy0 =  y0 * 0.5 +  cy * 0.5;
    HCReal qx1 =  cx * 0.5 +  x1 * 0.5;
    HCReal qy1 =  cy * 0.5 +  y1 * 0.5;
    HCReal  sx = qx0 * 0.5 + qx1 * 0.5;
    HCReal  sy = qy0 * 0.5 + qy1 * 0.5;
    HCPathAddQuadraticCurvePolylineData(self, x0, y0, qx0, qy0, sx, sy, flatnessThreshold, polylineData);
    HCPathAddQuadraticCurvePolylineData(self, sx, sy, qx1, qy1, x1, y1, flatnessThreshold, polylineData);
}

void HCPathAddCubicCurvePolylineData(HCPathRef self, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCReal flatnessThreshold, HCDataRef polylineData) {
    // Calculate using direct evaluation informed by derivitive
//    HCReal x, y, dx, dy;
//    for (HCReal t = 0.0; t <= 1.0;) {
//        HCPathEvaluateCubicCurve(t, x0, y0, cx0, cy0, cx1, cy1, x1, y1, &x, &y, &dx, &dy);
//        HCPathDrawPoint(self, x, y, HCColorCombine(c0, c1, t));
//        t += fmax(0.00001, 0.33333 / fmax(fabs(dx), fabs(dy)));
//    }
    
    // Calculate the curve polyline using De Casteljau's Algorithm
    
    // Determine cubic curve flatness
    HCReal p0c0Distance = sqrt((cx0 -  x0) * (cx0 -  x0) + (cy0 -  y0) * (cy0 -  y0));
    HCReal c0c1Distance = sqrt((cx1 - cx0) * (cx1 - cx0) + (cy1 - cy0) * (cy1 - cy0));
    HCReal c1p1Distance = sqrt(( x1 - cx1) * ( x1 - cx1) + ( y1 - cy1) * ( y1 - cy1));
    HCReal p0p1Distance = sqrt(( x1 -  x0) * ( x1 -  x0) + ( y1 -  y0) * ( y1 -  y0));
    HCReal flatness = (p0c0Distance + c0c1Distance + c1p1Distance) / p0p1Distance;
    if (flatness < flatnessThreshold || HCRealIsSimilar(p0p1Distance, 0.0, FLT_EPSILON)) {
        HCPathAddLineSegmentPolylineData(self, x0, y0, x1, y1, polylineData);
        return;
    }

    // Split the cubic curve at t = 0.5 and recursively add their polyline data
    HCReal qx0 =  x0 * 0.5 + cx0 * 0.5;
    HCReal qy0 =  y0 * 0.5 + cy0 * 0.5;
    HCReal qcx = cx0 * 0.5 + cx1 * 0.5;
    HCReal qcy = cy0 * 0.5 + cy1 * 0.5;
    HCReal qx1 = cx1 * 0.5 +  x1 * 0.5;
    HCReal qy1 = cy1 * 0.5 +  y1 * 0.5;
    HCReal rx0 = qx0 * 0.5 + qcx * 0.5;
    HCReal ry0 = qy0 * 0.5 + qcy * 0.5;
    HCReal rx1 = qcx * 0.5 + qx1 * 0.5;
    HCReal ry1 = qcy * 0.5 + qy1 * 0.5;
    HCReal  sx = rx0 * 0.5 + rx1 * 0.5;
    HCReal  sy = ry0 * 0.5 + ry1 * 0.5;
    HCPathAddCubicCurvePolylineData(self, x0, y0, qx0, qy0, rx0, ry0, sx, sy, flatnessThreshold, polylineData);
    HCPathAddCubicCurvePolylineData(self, sx, sy, rx1, ry1, qx1, qy1, x1, y1, flatnessThreshold, polylineData);
}
