//
//  HCPath.c
//  HollowCore
//
//  Created by Matt Stoker on 12/28/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCPath_Internal.h"
#include "HCContour.h"
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
HCPathRef HCPathCreateEmpty() {
    HCPathRef self = calloc(sizeof(HCPath), 1);
    HCPathInit(self);
    return self;
}

HCPathRef HCPathCreateWithElements(HCPathElement* elements, HCInteger elementCount) {
    HCPathRef self = HCPathCreateEmpty();
    for (HCInteger elementIndex = 0; elementIndex < elementCount; elementIndex++) {
        HCPathElement element = elements[elementIndex];
        switch (element.command) {
            case HCPathCommandMove: HCPathMove(self, element.points[0].x, element.points[0].y); break;
            case HCPathCommandAddLine: HCPathAddLine(self, element.points[0].x, element.points[0].y); break;
            case HCPathCommandAddQuadraticCurve: HCPathAddQuadraticCurve(self, element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y); break;
            case HCPathCommandAddCubicCurve: HCPathAddCubicCurve(self, element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y, element.points[2].x, element.points[2].y); break;
            case HCPathCommandCloseSubpath: HCPathClose(self); break;
        }
    }
    return self;
}

HCPathRef HCPathCreateWithSubpaths(HCListRef subpaths) {
    HCPathRef self = HCPathCreateEmpty();
    for (HCListIterator i = HCListIterationBegin(subpaths); !HCListIterationHasEnded(&i); HCListIterationNext(&i)) {
        HCInteger elementCount = HCPathElementCount(i.object);
        for (HCInteger elementIndex = 0; elementIndex < elementCount; elementIndex++) {
            HCPathElement element = HCPathElementAt(i.object, elementIndex);
            switch (element.command) {
                case HCPathCommandMove: HCPathMove(self, element.points[0].x, element.points[0].y); break;
                case HCPathCommandAddLine: HCPathAddLine(self, element.points[0].x, element.points[0].y); break;
                case HCPathCommandAddQuadraticCurve: HCPathAddQuadraticCurve(self, element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y); break;
                case HCPathCommandAddCubicCurve: HCPathAddCubicCurve(self, element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y, element.points[2].x, element.points[2].y); break;
                case HCPathCommandCloseSubpath: HCPathClose(self); break;
            }
        }
    }
    return self;
}

HCPathRef HCPathCreateRectangle(HCRectangle rectangle) {
    HCPathRef self = HCPathCreateEmpty();
    HCPathMove(self, HCRectangleMinX(rectangle), HCRectangleMinY(rectangle));
    HCPathAddLine(self, HCRectangleMaxX(rectangle), HCRectangleMinY(rectangle));
    HCPathAddLine(self, HCRectangleMaxX(rectangle), HCRectangleMaxY(rectangle));
    HCPathAddLine(self, HCRectangleMinX(rectangle), HCRectangleMaxY(rectangle));
    HCPathClose(self);
    return self;
}

HCPathRef HCPathCreateEllipse(HCRectangle ellipseBounds) {
    static HCReal controlAdjustment = 0.552284749830793; //4.0 / 3.0 * (tan(M_PI * 0.5) * 0.25);
    HCPathRef self = HCPathCreateEmpty();
    
    HCReal p0x = HCRectangleMaxX(ellipseBounds);
    HCReal p0y = HCRectangleMidY(ellipseBounds);
    HCPathMove(self, p0x, p0y);
    
    HCReal c0x = HCRectangleMaxX(ellipseBounds);
    HCReal c0y = HCRectangleMidY(ellipseBounds) + controlAdjustment * HCRectangleHeight(ellipseBounds) * 0.5;
    HCReal c1x = HCRectangleMidX(ellipseBounds) + controlAdjustment * HCRectangleWidth(ellipseBounds) * 0.5;
    HCReal c1y = HCRectangleMaxY(ellipseBounds);
    HCReal p1x = HCRectangleMidX(ellipseBounds);
    HCReal p1y = HCRectangleMaxY(ellipseBounds);
    HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, p1x, p1y);
    
    c0x = HCRectangleMidX(ellipseBounds) - controlAdjustment * HCRectangleWidth(ellipseBounds) * 0.5;
    c0y = HCRectangleMaxY(ellipseBounds);
    c1x = HCRectangleMinX(ellipseBounds);
    c1y = HCRectangleMidY(ellipseBounds) + controlAdjustment * HCRectangleHeight(ellipseBounds) * 0.5;
    p1x = HCRectangleMinX(ellipseBounds);
    p1y = HCRectangleMidY(ellipseBounds);
    HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, p1x, p1y);
    
    c0x = HCRectangleMinX(ellipseBounds);
    c0y = HCRectangleMidY(ellipseBounds) - controlAdjustment * HCRectangleHeight(ellipseBounds) * 0.5;
    c1x = HCRectangleMidX(ellipseBounds) - controlAdjustment * HCRectangleWidth(ellipseBounds) * 0.5;
    c1y = HCRectangleMinY(ellipseBounds);
    p1x = HCRectangleMidX(ellipseBounds);
    p1y = HCRectangleMinY(ellipseBounds);
    HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, p1x, p1y);
    
    c0x = HCRectangleMidX(ellipseBounds) + controlAdjustment * HCRectangleWidth(ellipseBounds) * 0.5;
    c0y = HCRectangleMinY(ellipseBounds);
    c1x = HCRectangleMaxX(ellipseBounds);
    c1y = HCRectangleMidY(ellipseBounds) - controlAdjustment * HCRectangleHeight(ellipseBounds) * 0.5;
    p1x = HCRectangleMaxX(ellipseBounds);
    p1y = HCRectangleMidY(ellipseBounds);
    HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, p1x, p1y);
    
    HCPathClose(self);
    
    return self;
}

void HCPathInit(void* memory) {
    // Construct path object
    HCObjectInit(memory);
    HCPathRef self = memory;
    self->base.type = HCPathType;
    self->elementData = HCDataCreate();
    self->elementPolylines = HCListCreate();
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
    HCRelease(self->elementPolylines);
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
            case HCPathCommandCloseSubpath:
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
            case HCPathCommandCloseSubpath: break;
        }
    }
    return hash;
}

void HCPathPrint(HCPathRef self, FILE* stream) {
    fprintf(stream, "<%s@%p>,path:", self->base.type->name, self);
    HCPathPrintData(self, stream);
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

HCDataRef HCPathElementPolylineData(HCPathRef self, HCInteger elementIndex) {
    return HCListObjectAtIndex(self->elementPolylines, elementIndex);
}

HCInteger HCPathElementPolylinePointCount(HCPathRef self, HCInteger elementIndex) {
    HCDataRef polylineData = HCPathElementPolylineData(self, elementIndex);
    return HCDataSize(polylineData) / sizeof(HCPoint);
}

HCPoint HCPathElementPolylinePointAt(HCPathRef self, HCInteger elementIndex, HCInteger pointIndex) {
    HCDataRef polylineData = HCPathElementPolylineData(self, elementIndex);
    HCPoint* points = (HCPoint*)HCDataBytes(polylineData);
    return points[pointIndex];
}

HCPoint HCPathCurrentPoint(HCPathRef self) {
    if (!HCPathIsEmpty(self)) {
        // Find the end point of the last command
        HCPathElement endElement = HCPathElementAt(self, HCPathElementCount(self) - 1);
        switch (endElement.command) {
            case HCPathCommandMove: return endElement.points[0];
            case HCPathCommandAddLine: return endElement.points[0];
            case HCPathCommandAddQuadraticCurve: return endElement.points[1];
            case HCPathCommandAddCubicCurve:  return endElement.points[2];
            case HCPathCommandCloseSubpath: {
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

HCRectangle HCPathBounds(HCPathRef self) {
    return self->bounds;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Manipulation
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathMove(HCPathRef self, HCReal x, HCReal y) {
    HCPoint points[1];
    points[0].x = x;
    points[0].y = y;
    HCPathAddElement(self, HCPathCommandMove, points);
}

void HCPathAddLine(HCPathRef self, HCReal x, HCReal y) {
    HCPoint points[1];
    points[0].x = x;
    points[0].y = y;
    HCPathAddElement(self, HCPathCommandAddLine, points);
}

void HCPathAddQuadraticCurve(HCPathRef self, HCReal cx, HCReal cy, HCReal x, HCReal y) {
    HCPoint points[2];
    points[0].x = cx;
    points[0].y = cy;
    points[1].x = x;
    points[1].y = y;
    HCPathAddElement(self, HCPathCommandAddQuadraticCurve, points);
}

void HCPathAddCubicCurve(HCPathRef self, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x, HCReal y) {
    HCPoint points[3];
    points[0].x = cx0;
    points[0].y = cy0;
    points[1].x = cx1;
    points[1].y = cy1;
    points[2].x = x;
    points[2].y = y;
    HCPathAddElement(self, HCPathCommandAddCubicCurve, points);
}

void HCPathAddArc(HCPathRef self, HCReal x, HCReal y, HCReal xr, HCReal yr, HCReal theta, HCBoolean largeArc, HCBoolean sweep) {
    // TODO: Add arc as cubic curve
}

void HCPathClose(HCPathRef self) {
    HCPathAddElement(self, HCPathCommandCloseSubpath, NULL);
}

void HCPathAddElement(HCPathRef self, HCPathCommand command, const HCPoint* points) {
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
        case HCPathCommandCloseSubpath:
            element.command = HCPathCommandCloseSubpath;
            element.points = NULL;
            break;
    }
    
    // Add element to path
    HCDataAddBytes(self->elementData, sizeof(element), (const HCByte*)&element);
    
    // Calculate polyline data for element
    HCDataRef elementPolylineData = HCDataCreate();
    switch (element.command) {
        case HCPathCommandMove:
            break;
        case HCPathCommandAddLine:
            HCPathAddLineSegmentPolylineData(self, currentPoint.x, currentPoint.y, element.points[0].x, element.points[0].y, elementPolylineData);
            break;
        case HCPathCommandAddQuadraticCurve:
            HCPathAddQuadraticCurvePolylineData(self, currentPoint.x, currentPoint.y, element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y, HCPathFlatnessNormal, elementPolylineData);
            break;
        case HCPathCommandAddCubicCurve:
            HCPathAddCubicCurvePolylineData(self, currentPoint.x, currentPoint.y, element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y, element.points[2].x, element.points[2].y, HCPathFlatnessNormal, elementPolylineData);
            break;
        case HCPathCommandCloseSubpath: {
            HCPoint closeLineSegmentStart = currentPoint;
            HCPoint closeLineSegmentEnd = HCPathCurrentPoint(self);
            HCPathAddLineSegmentPolylineData(self, closeLineSegmentStart.x, closeLineSegmentStart.y, closeLineSegmentEnd.x, closeLineSegmentEnd.y, elementPolylineData);
        } break;
    }
    
    // Add element polyline data
    HCListAddObjectReleased(self->elementPolylines, elementPolylineData);
    
    // Expand bounds to include polyline data
    if (HCPathElementCount(self) == 1 && element.command == HCPathCommandMove) {
        self->bounds = HCRectangleMake(element.points[0], HCSizeZero);
    }
    else {
        HCInteger elementIndex = HCPathElementCount(self) - 1;
        HCInteger polylinePointCount = HCPathElementPolylinePointCount(self, elementIndex);
        HCRectangle bounds = self->bounds;
        for (HCInteger pointIndex = 0; pointIndex < polylinePointCount; pointIndex++) {
            HCPoint point = HCPathElementPolylinePointAt(self, elementIndex, pointIndex);
            bounds = HCRectangleIncludingPoint(bounds, point);
        }
        self->bounds = bounds;
    }
}

void HCPathRemoveLastElement(HCPathRef self) {
    HCInteger elementCount = HCPathElementCount(self);
    
    // Remove element
    HCPathElement element = HCPathElementAt(self, elementCount - 1);
    if (element.points != NULL) {
        free(element.points);
    }
    HCDataRemoveBytes(self->elementData, sizeof(HCPathElement));
    
    // Remove element polyline data
    HCListRemoveObject(self->elementPolylines);
    
    // Update count
    elementCount = HCPathElementCount(self);
    
    // Re-calculate bounds to exclude the element polylines
    HCRectangle bounds = HCRectangleMake(HCPathCurrentPoint(self), HCSizeZero);
    for (HCInteger elementIndex = 0; elementIndex < elementCount; elementIndex++) {
        HCInteger polylinePointCount = HCPathElementPolylinePointCount(self, elementIndex);
        for (HCInteger pointIndex = 0; pointIndex < polylinePointCount; pointIndex++) {
            HCPoint point = HCPathElementPolylinePointAt(self, elementIndex, pointIndex);
            bounds = HCRectangleIncludingPoint(bounds, point);
        }
    }
    self->bounds = bounds;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Polylines
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathAddLineSegmentPolylineData(HCPathRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCDataRef polylineData) {
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

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Conversion
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathPrintData(HCPathRef self, FILE* stream) {
    // Print SVG command for each element
    HCInteger elementCount = HCPathElementCount(self);
    for (HCInteger elementIndex = 0; elementIndex < elementCount; elementIndex++) {
        HCPathElement element = HCPathElementAt(self, elementIndex);
        switch (element.command) {
            case HCPathCommandMove: fprintf(stream, "M %f %f ", element.points[0].x, element.points[0].y); break;
            case HCPathCommandAddLine: fprintf(stream, "L %f %f ", element.points[0].x, element.points[0].y); break;
            case HCPathCommandAddQuadraticCurve: fprintf(stream, "Q %f %f %f %f ", element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y); break;
            case HCPathCommandAddCubicCurve: fprintf(stream, "C %f %f %f %f %f %f ", element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y, element.points[2].x, element.points[2].y); break;
            case HCPathCommandCloseSubpath: fprintf(stream, "Z "); break;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Subpaths
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPathSubpathContainingElementIsOpen(HCPathRef self, HCInteger elementIndex, HCInteger* startIndex, HCInteger* endIndex) {
    // Search for sub-path start element, if requested (not needed to determine if sub-path is open)
    if (startIndex != NULL) {
        *startIndex = 0;
        for (HCInteger subpathStartSearchIndex = elementIndex; subpathStartSearchIndex >= 0; subpathStartSearchIndex--) {
            HCPathCommand command = HCPathElementAt(self, subpathStartSearchIndex).command;
            if (command == HCPathCommandMove) {
                *startIndex = subpathStartSearchIndex;
                break;
            }
        }
    }
    
    // Search for sub-path end element and determine if it is open
    HCInteger elementCount = HCPathElementCount(self);
    for (HCInteger subpathEndSearchIndex = elementIndex; subpathEndSearchIndex < elementCount; subpathEndSearchIndex++) {
        HCPathCommand command = HCPathElementAt(self, subpathEndSearchIndex).command;
        if (command == HCPathCommandMove && subpathEndSearchIndex != elementIndex) {
            // Sub-path ends with a move, so it is open
            if (endIndex != NULL) {
                *endIndex = subpathEndSearchIndex;
            }
            return true;
        }
        if (command == HCPathCommandCloseSubpath) {
            // Sub-path ends with a close-subpath, so it is closed
            if (endIndex != NULL) {
                *endIndex = subpathEndSearchIndex + 1;
            }
            return false;
        }
    }
    
    // Reached end without finding a move-to or close-subpath command, so the path is open
    if (endIndex != NULL) {
        *endIndex = elementCount;
    }
    return true;
}

HCBoolean HCPathSubpathContainingElementIsClosed(HCPathRef self, HCInteger elementIndex, HCInteger* startIndex, HCInteger* endIndex) {
    return !HCPathSubpathContainingElementIsOpen(self, elementIndex, startIndex, endIndex);
}

HCPathRef HCPathSubpathContainingElementRetained(HCPathRef self, HCInteger elementIndex, HCInteger* startIndex, HCInteger* endIndex, HCBoolean* isOpen) {
    // Determine sub-path indices
    HCInteger subpathStartIndex = 0;
    HCInteger subpathEndIndex = 0;
    HCBoolean open = HCPathSubpathContainingElementIsOpen(self, elementIndex, &subpathStartIndex, &subpathEndIndex);
    if (startIndex != NULL) {
        *startIndex = subpathStartIndex;
    }
    if (endIndex != NULL) {
        *endIndex = subpathEndIndex;
    }
    if (isOpen != NULL) {
        *isOpen = open;
    }
    
    // Create the sub-path with the elements from start to end
    HCPathElement* elements = (HCPathElement*)HCDataBytes(self->elementData);
    HCPathRef subpath = HCPathCreateWithElements(elements + subpathStartIndex, subpathEndIndex - subpathStartIndex);
    return subpath;
}

HCListRef HCPathSubpathsRetained(HCPathRef self) {
    HCListRef subpaths = HCListCreate();
    HCInteger elementCount = HCPathElementCount(self);
    HCInteger elementIndex = 0;
    while (elementIndex < elementCount) {
        HCPathRef subpath = HCPathSubpathContainingElementRetained(self, elementIndex, NULL, &elementIndex, NULL);
        if (!HCPathIsEmpty(subpath)) {
            HCListAddObjectReleased(subpaths, subpath);
        }
    }
    return subpaths;
}

HCListRef HCPathOpenSubpathsRetained(HCPathRef self) {
    HCListRef openSubpaths = HCListCreate();
    HCInteger elementCount = HCPathElementCount(self);
    HCInteger elementIndex = 0;
    while (elementIndex < elementCount) {
        // Find the sub-path start, end, and openness
        HCInteger startIndex;
        HCInteger endIndex;
        HCBoolean isOpen = HCPathSubpathContainingElementIsOpen(self, elementIndex, &startIndex, &endIndex);
        elementIndex = endIndex;
        
        // If open, create the sub-path with the elements from start to end
        if (isOpen) {
            HCPathElement* elements = (HCPathElement*)HCDataBytes(self->elementData);
            HCPathRef subpath = HCPathCreateWithElements(elements + startIndex, endIndex - startIndex);
            if (!HCPathIsEmpty(subpath)) {
                HCListAddObjectReleased(openSubpaths, subpath);
            }
        }
    }
    return openSubpaths;
}

HCListRef HCPathClosedSubpathsRetained(HCPathRef self) {
    HCListRef closedSubpaths = HCListCreate();
    HCInteger elementCount = HCPathElementCount(self);
    HCInteger elementIndex = 0;
    while (elementIndex < elementCount) {
        // Find the sub-path start, end, and openness
        HCInteger startIndex;
        HCInteger endIndex;
        HCBoolean isClosed = HCPathSubpathContainingElementIsClosed(self, elementIndex, &startIndex, &endIndex);
        elementIndex = endIndex;
        
        // If closed, create the sub-path with the elements from start to end
        if (isClosed) {
            HCPathElement* elements = (HCPathElement*)HCDataBytes(self->elementData);
            HCPathRef subpath = HCPathCreateWithElements(elements + startIndex, endIndex - startIndex);
            if (!HCPathIsEmpty(subpath)) {
                HCListAddObjectReleased(closedSubpaths, subpath);
            }
        }
    }
    return closedSubpaths;
}

HCPathRef HCPathOpenSubpathsAsPathRetained(HCPathRef self) {
    HCListRef openSubpaths = HCPathOpenSubpathsRetained(self);
    HCPathRef openPath = HCPathCreateWithSubpaths(openSubpaths);
    HCRelease(openSubpaths);
    return openPath;
}

HCPathRef HCPathClosedSubpathsAsPathRetained(HCPathRef self) {
    HCListRef closedSubpaths = HCPathClosedSubpathsRetained(self);
    HCPathRef closedPath = HCPathCreateWithSubpaths(closedSubpaths);
    HCRelease(closedSubpaths);
    return closedPath;
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
        // Determine if the current sub-path is open, and skip it if it is
        HCInteger endIndex = 0;
        HCBoolean isOpen = HCPathSubpathContainingElementIsOpen(self, elementIndex, NULL, &endIndex);
        if (isOpen) {
            // Move to the next sub-path
            elementIndex = endIndex;
            continue;
        }
        
        // Extract the initial point of the polyline and use it as the start point of the first line segment, or skip it if it is empty (e.g. a move-to element)
        HCInteger polylinePointCount = HCPathElementPolylinePointCount(self, elementIndex);
        if (polylinePointCount <= 0) {
            // Move to the next element
            elementIndex++;
            continue;
        }
        HCPoint p0 = HCPathElementPolylinePointAt(self, elementIndex, 0);
        
        // Compare the line segments of the polyline against the ray
        for (HCInteger pointIndex = 1; pointIndex < polylinePointCount; pointIndex++) {
            // Determine the end point of the line segment from the polyline
            HCPoint p1 = HCPathElementPolylinePointAt(self, elementIndex, pointIndex);
            
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
        HCInteger polylinePointCount = HCPathElementPolylinePointCount(self, elementIndex);
        if (polylinePointCount <= 0) {
            continue;
        }
        HCPoint p0 = HCPathElementPolylinePointAt(self, elementIndex, 0);
        
        // Compare the line segments of the polyline against the other path
        for (HCInteger pointIndex = 1; pointIndex < polylinePointCount; pointIndex++) {
            // Determine the end point of the line segment from the polyline
            HCPoint p1 = HCPathElementPolylinePointAt(self, elementIndex, pointIndex);
            HCInteger otherElementCount = HCPathElementCount(other);
            for (HCInteger otherElementIndex = 0; otherElementIndex < otherElementCount; otherElementIndex++) {
                // Extract the initial point of the other path polyline and use it as the start point of it's first line segment, or skip it if it is empty
                HCInteger otherPolylinePointCount = HCPathElementPolylinePointCount(other, otherElementIndex);
                if (otherPolylinePointCount <= 0) {
                    continue;
                }
                HCPoint q0 = HCPathElementPolylinePointAt(other, otherElementIndex, 0);
                
                // Compare the line segments of the other polyline against the current line segment in the path
                for (HCInteger otherPointIndex = 0; otherPointIndex < otherPolylinePointCount; otherPointIndex++) {
                    // Determine the end point of the line segment from the other polyline
                    HCPoint q1 = HCPathElementPolylinePointAt(other, otherElementIndex, otherPointIndex);
                    
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
    *((HCBoolean*)context) = true;
    *stopSearching = true;
}
