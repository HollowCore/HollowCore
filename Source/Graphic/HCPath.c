//
//  HCPath.c
//  HollowCore
//
//  Created by Matt Stoker on 12/28/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

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
HCPathRef HCPathCreate(const char* path) {
    HCPathRef self = calloc(sizeof(HCPath), 1);
    HCPathInit(self, path);
    return self;
}

void HCPathInit(void* memory, const char* path) {
    // Construct path object
    HCObjectInit(memory);
    HCPathRef self = memory;
    self->base.type = HCPathType;
    self->elementCount = 0;
    self->elements = NULL;
    
    // Parse path data into path object
    HCPathParse(self, path);
}

void HCPathDestroy(HCPathRef self) {
    free(self->elements);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPathIsEqual(HCPathRef self, HCPathRef other) {
    if (self->elementCount != other->elementCount) {
        return false;
    }
    for (HCInteger elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
        HCPathElement element = self->elements[elementIndex];
        HCPathElement otherElement = other->elements[elementIndex];
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
    for (HCInteger elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
        HCPathElement element = self->elements[elementIndex];
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
HCInteger HCPathElementCount(HCPathRef self) {
    return self->elementCount;
}

HCPathElement HCPathElementAt(HCPathRef self, HCInteger elementIndex) {
    return self->elements[elementIndex];
}

HCSize HCPathSize(HCPathRef self) {
    // Convert the path to line segments
    HCDataRef segmentData = HCPathAsLineSegmentDataRetained(self);
    HCInteger pointCount = HCDataSize(segmentData) / sizeof(HCPoint);
    HCPoint* points = (HCPoint*)HCDataBytes(segmentData);
    
    // Find the maxima of the line segment points
    HCReal minX = HCRealMax;
    HCReal minY = HCRealMax;
    HCReal maxX = HCRealMin;
    HCReal maxY = HCRealMin;
    for (HCInteger pointIndex = 0; pointIndex < pointCount; pointIndex++) {
        HCPoint point = points[pointIndex];
        minX = fmin(minX, point.x);
        minY = fmin(minY, point.y);
        maxX = fmax(maxX, point.x);
        maxY = fmax(maxY, point.y);
    }
    HCRelease(segmentData);
    
    // Calculate the size of the path
    return HCSizeMake(maxX - minX, maxY - minY);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Manipulation
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathMoveToPoint(HCPathRef self, HCReal x, HCReal y) {
    // Create move element
    HCPathElement element;
    element.command = HCPathCommandMove;
    element.points = malloc(sizeof(HCPoint) * 1);
    element.points[0].x = x;
    element.points[0].y = y;
    
    // Add element to path
    self->elementCount++;
    self->elements = realloc(self->elements, sizeof(HCPoint) * self->elementCount);
    self->elements[self->elementCount - 1] = element;
}

void HCPathAddLine(HCPathRef self, HCReal x, HCReal y) {
    // Create add line element
    HCPathElement element;
    element.command = HCPathCommandAddLine;
    element.points = malloc(sizeof(HCPoint) * 1);
    element.points[0].x = x;
    element.points[0].y = y;
    
    // Add element to path
    self->elementCount++;
    self->elements = realloc(self->elements, sizeof(HCPoint) * self->elementCount);
    self->elements[self->elementCount - 1] = element;
}

void HCPathAddQuadraticCurve(HCPathRef self, HCReal cx, HCReal cy, HCReal x, HCReal y) {
    // Create add quadratic curve element
    HCPathElement element;
    element.command = HCPathCommandAddQuadraticCurve;
    element.points = malloc(sizeof(HCPoint) * 2);
    element.points[0].x = cx;
    element.points[0].y = cy;
    element.points[1].x = x;
    element.points[1].y = y;
    
    // Add element to path
    self->elementCount++;
    self->elements = realloc(self->elements, sizeof(HCPoint) * self->elementCount);
    self->elements[self->elementCount - 1] = element;
}

void HCPathAddCubicCurve(HCPathRef self, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x, HCReal y) {
    // Create add cubic curve element
    HCPathElement element;
    element.command = HCPathCommandAddCubicCurve;
    element.points = malloc(sizeof(HCPoint) * 3);
    element.points[0].x = cx0;
    element.points[0].y = cy0;
    element.points[1].x = cx1;
    element.points[1].y = cy1;
    element.points[2].x = x;
    element.points[2].y = y;
    
    // Add element to path
    self->elementCount++;
    self->elements = realloc(self->elements, sizeof(HCPoint) * self->elementCount);
    self->elements[self->elementCount - 1] = element;
}

void HCPathAddArc(HCPathRef self, HCReal x, HCReal y, HCReal xr, HCReal yr, HCReal theta, HCBoolean largeArc, HCBoolean sweep) {
    // TODO: Add arc as cubic curve
}

void HCPathCloseSubpath(HCPathRef self) {
    // Create close sub-path element
    HCPathElement element;
    element.command = HCPathCommandCloseSubpath;
    element.points = NULL;
    
    // Add element to path
    self->elementCount++;
    self->elements = realloc(self->elements, sizeof(HCPoint) * self->elementCount);
    self->elements[self->elementCount - 1] = element;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Conversion
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathPrintData(HCPathRef self, FILE* stream) {
    // Print SVG command for each element
    for (HCInteger elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
        HCPathElement element = self->elements[elementIndex];
        switch (element.command) {
            case HCPathCommandMove: fprintf(stream, "M %f %f ", element.points[0].x, element.points[0].y); break;
            case HCPathCommandAddLine: fprintf(stream, "L %f %f ", element.points[0].x, element.points[0].y); break;
            case HCPathCommandAddQuadraticCurve: fprintf(stream, "Q %f %f %f %f ", element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y); break;
            case HCPathCommandAddCubicCurve: fprintf(stream, "C %f %f %f %f %f %f ", element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y, element.points[2].x, element.points[2].y); break;
            case HCPathCommandCloseSubpath: fprintf(stream, "Z "); break;
        }
    }
}

HCDataRef HCPathAsLineSegmentDataRetained(HCPathRef self) {
    // Convert each element to line segments
    HCDataRef segmentData = HCDataCreate();
    HCPoint startPoint = HCPointZero;
    HCPoint currentPoint = startPoint;
    HCPoint controlPoint0 = currentPoint;
    HCPoint controlPoint1 = currentPoint;
    HCPoint endPoint = currentPoint;
    for (HCInteger elementIndex = 0; elementIndex < self->elementCount; elementIndex++) {
        HCPathElement element = self->elements[elementIndex];
        switch (element.command) {
            case HCPathCommandMove:
                endPoint = HCPointMake(element.points[0].x, element.points[0].y);
                currentPoint = endPoint;
                startPoint = currentPoint;
                break;
            case HCPathCommandAddLine:
                endPoint = HCPointMake(element.points[0].x, element.points[0].y);
                HCPathAddLineSegment(self, currentPoint.x, currentPoint.y, endPoint.x, endPoint.y, segmentData);
                currentPoint = endPoint;
                break;
            case HCPathCommandAddQuadraticCurve:
                controlPoint0 = HCPointMake(element.points[0].x, element.points[0].y);
                endPoint = HCPointMake(element.points[1].x, element.points[1].y);
                HCPathAddQuadraticCurveSegments(self, currentPoint.x, currentPoint.y, controlPoint0.x, controlPoint0.y, endPoint.x, endPoint.y, segmentData);
                currentPoint = endPoint;
                break;
            case HCPathCommandAddCubicCurve:
                controlPoint0 = HCPointMake(element.points[0].x, element.points[0].y);
                controlPoint1 = HCPointMake(element.points[1].x, element.points[1].y);
                endPoint = HCPointMake(element.points[2].x, element.points[2].y);
                HCPathAddCubicCurveSegments(self, currentPoint.x, currentPoint.y, controlPoint0.x, controlPoint0.y, controlPoint1.x, controlPoint1.y, endPoint.x, endPoint.y, segmentData);
                currentPoint = endPoint;
                break;
            case HCPathCommandCloseSubpath:
                endPoint = startPoint;
                HCPathAddLineSegment(self, currentPoint.x, currentPoint.y, endPoint.x, endPoint.y, segmentData);
                currentPoint = endPoint;
                break;
        }
    }
    
    return segmentData;
}

void HCPathAddLineSegment(HCPathRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCDataRef segmentData) {
    // Add the segment start and end points
    HCDataAddReal(segmentData, x0);
    HCDataAddReal(segmentData, y0);
    HCDataAddReal(segmentData, x1);
    HCDataAddReal(segmentData, y1);
}

void HCPathAddQuadraticCurveSegments(HCPathRef self, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCDataRef segmentData) {
    // Draw using direct evaluation informed by derivitive
//    HCReal x, y, dx, dy;
//    for (HCReal t = 0.0; t <= 1.0;) {
//        HCPathEvaluateQuadraticCurve(t, x0, y0, cx, cy, x1, y1, &x, &y, &dx, &dy);
//        HCPathDrawPoint(self, x, y, HCColorCombine(c0, c1, t));
//        t += fmax(0.00001, 0.50000 / fmax(fabs(dx), fabs(dy)));
//    }
    
    // Draw using De Casteljau's Algorithm
    HCReal flatness =
       (sqrt((cx - x0) * (cx - x0) + (cy - y0) * (cy - y0)) +
        sqrt((x1 - cx) * (x1 - cx) + (y1 - cy) * (y1 - cy))) /
        sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    if (flatness < 1.0001) {
        HCPathAddLineSegment(self, x0, y0, x1, y1, segmentData);
        return;
    }

    HCReal qx0 =  x0 * 0.5 +  cx * 0.5;
    HCReal qy0 =  y0 * 0.5 +  cy * 0.5;
    HCReal qx1 =  cx * 0.5 +  x1 * 0.5;
    HCReal qy1 =  cy * 0.5 +  y1 * 0.5;
    HCReal  sx = qx0 * 0.5 + qx1 * 0.5;
    HCReal  sy = qy0 * 0.5 + qy1 * 0.5;
    HCPathAddQuadraticCurveSegments(self, x0, y0, qx0, qy0, sx, sy, segmentData);
    HCPathAddQuadraticCurveSegments(self, sx, sy, qx1, qy1, x1, y1, segmentData);
}

void HCPathAddCubicCurveSegments(HCPathRef self, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCDataRef segmentData) {
    // Draw using direct evaluation informed by derivitive
//    HCReal x, y, dx, dy;
//    for (HCReal t = 0.0; t <= 1.0;) {
//        HCPathEvaluateCubicCurve(t, x0, y0, cx0, cy0, cx1, cy1, x1, y1, &x, &y, &dx, &dy);
//        HCPathDrawPoint(self, x, y, HCColorCombine(c0, c1, t));
//        t += fmax(0.00001, 0.33333 / fmax(fabs(dx), fabs(dy)));
//    }
    
    // Draw using De Casteljau's Algorithm
    HCReal flatness =
       (sqrt((cx0 -  x0) * (cx0 -  x0) + (cy0 -  y0) * (cy0 -  y0)) +
        sqrt((cx1 - cx0) * (cx1 - cx0) + (cy1 - cy0) * (cy1 - cy0)) +
        sqrt(( x1 - cx1) * ( x1 - cx1) + ( y1 - cy1) * ( y1 - cy1))) /
        sqrt(( x1 -  x0) * ( x1 -  x0) + ( y1 -  y0) * ( y1 -  y0));
    if (flatness < 1.0001) {
        HCPathAddLineSegment(self, x0, y0, x1, y1, segmentData);
        return;
    }

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
    HCPathAddCubicCurveSegments(self, x0, y0, qx0, qy0, rx0, ry0, sx, sy, segmentData);
    HCPathAddCubicCurveSegments(self, sx, sy, rx1, ry1, qx1, qy1, x1, y1, segmentData);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Parsing
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathParse(HCPathRef self, const char* path) {
    // Define state data for parsing the SVG path string
    char type = '\0';
    HCReal subpathStartX = 0.0;
    HCReal subpathStartY = 0.0;
    HCReal subpathEndX = subpathStartX;
    HCReal subpathEndY = subpathStartY;
    HCReal currentX = subpathEndX;
    HCReal currentY = subpathEndY;
    HCReal previousControlX = currentX;
    HCReal previousControlY = currentY;
    HCInteger argumentsExpected = 0;
    HCInteger argumentsCount = 0;
    HCReal arguments[10];
    
    // Parse path data according to SVG path element data string format
    const char* current = path;
    while (*current != '\0') {
        char c = *current;
        switch (c) {
            case '\n': // Fallthrough
            case '\r': // Fallthrough
            case ',': // Fallthrough
            case ' ': current++; break;
            case 'M': type = 'M'; argumentsExpected = 2; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'm': type = 'm'; argumentsExpected = 2; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'L': type = 'L'; argumentsExpected = 2; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'l': type = 'l'; argumentsExpected = 2; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'H': type = 'H'; argumentsExpected = 1; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'h': type = 'h'; argumentsExpected = 1; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'V': type = 'V'; argumentsExpected = 1; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'v': type = 'v'; argumentsExpected = 1; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'Q': type = 'Q'; argumentsExpected = 4; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'q': type = 'q'; argumentsExpected = 4; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'T': type = 'T'; argumentsExpected = 2; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 't': type = 't'; argumentsExpected = 2; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'C': type = 'C'; argumentsExpected = 6; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'c': type = 'c'; argumentsExpected = 6; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'S': type = 'S'; argumentsExpected = 4; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 's': type = 's'; argumentsExpected = 4; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'A': type = 'A'; argumentsExpected = 7; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'a': type = 'a'; argumentsExpected = 7; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'Z': // Fallthrough
            case 'z':
                // Close path has no arguments, so just perform the action and move on
                HCPathCloseSubpath(self);
                subpathEndX = subpathStartX;
                subpathEndY = subpathStartY;
                currentX = subpathEndX;
                currentY = subpathEndY;
                current++;
                break;
            default: {
                // Parse an argument number
                char* end = (char*)current;
                double argument = strtod(current, &end);
                if (end <= current) {
                    // Invalid argument, ignore it and dump the current path sub-component
                    type = '\0';
                    current++;
                    break;
                }
                
                // Save the argument and advance to the character after the argument
                arguments[argumentsCount] = argument;
                argumentsCount++;
                current = end;
                
                // When enough arguments have been parsed, execute the draw command specified
                if (argumentsCount == argumentsExpected) {
                    // Consume arguments
                    argumentsCount = 0;
                    
                    // Execute the command
                    switch (type) {
                        case 'm':
                            arguments[0] += subpathEndX;
                            arguments[1] += subpathEndY;
                            // Fallthrough
                        case 'M': {
                            subpathStartX = arguments[0];
                            subpathStartY = arguments[1];
                            subpathEndX = subpathStartX;
                            subpathEndY = subpathStartY;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathMoveToPoint(self, subpathStartX, subpathStartY);
                        } break;
                        case 'l':
                            arguments[0] += subpathEndX;
                            arguments[1] += subpathEndY;
                            // Fallthrough
                        case 'L': {
                            HCReal x = arguments[0];
                            HCReal y = arguments[1];
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathAddLine(self, x, y);
                        } break;
                        case 'h':
                            arguments[0] += subpathEndX;
                            // Fallthrough
                        case 'H': {
                            HCReal x = arguments[0];
                            HCReal y = subpathEndY;
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathAddLine(self, x, y);
                        } break;
                        case 'v':
                            arguments[0] += subpathEndY;
                            // Fallthrough
                        case 'V': {
                            HCReal x = subpathEndX;
                            HCReal y = arguments[0];
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathAddLine(self, x, y);
                        } break;
                        case 'q':
                            arguments[0] += subpathEndX;
                            arguments[1] += subpathEndY;
                            arguments[2] += subpathEndX;
                            arguments[3] += subpathEndY;
                            // Fallthrough
                        case 'Q': {
                            HCReal cx = arguments[0];
                            HCReal cy = arguments[1];
                            HCReal x = arguments[2];
                            HCReal y = arguments[3];
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = cx;
                            previousControlY = cy;
                            HCPathAddQuadraticCurve(self, cx, cy, x, y);
                        } break;
                        case 't':
                            arguments[0] += subpathEndX;
                            arguments[1] += subpathEndY;
                            // Fallthrough
                        case 'T': {
                            HCReal cx = 2.0 * subpathEndX - previousControlX;
                            HCReal cy = 2.0 * subpathEndY - previousControlY;
                            HCReal x = arguments[0];
                            HCReal y = arguments[1];
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = cx;
                            previousControlY = cy;
                            HCPathAddQuadraticCurve(self, cx, cy, x, y);
                        } break;
                        case 'c':
                            arguments[0] += subpathEndX;
                            arguments[1] += subpathEndY;
                            arguments[2] += subpathEndX;
                            arguments[3] += subpathEndY;
                            arguments[4] += subpathEndX;
                            arguments[5] += subpathEndY;
                            // Fallthrough
                        case 'C': {
                            HCReal c0x = arguments[0];
                            HCReal c0y = arguments[1];
                            HCReal c1x = arguments[2];
                            HCReal c1y = arguments[3];
                            HCReal x = arguments[4];
                            HCReal y = arguments[5];
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = c1x;
                            previousControlY = c1y;
                            HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, x, y);
                        } break;
                        case 's':
                            arguments[0] += subpathEndX;
                            arguments[1] += subpathEndY;
                            arguments[2] += subpathEndX;
                            arguments[3] += subpathEndY;
                            // Fallthrough
                        case 'S': {
                            HCReal c0x = 2.0 * subpathEndX - previousControlX;
                            HCReal c0y = 2.0 * subpathEndY - previousControlY;
                            HCReal c1x = arguments[0];
                            HCReal c1y = arguments[1];
                            HCReal x = arguments[2];
                            HCReal y = arguments[3];
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = c1x;
                            previousControlY = c1y;
                            HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, x, y);
                        } break;
                        case 'a':
                            arguments[5] += subpathEndX;
                            arguments[6] += subpathEndY;
                            // Fallthrough
                        case 'A': {
                            HCReal xr = arguments[0];
                            HCReal yr = arguments[1];
                            HCReal rotation = arguments[2] / 180.0 * M_PI;
                            HCBoolean largeArc = arguments[3] == 0.0 ? false : true;
                            HCBoolean sweep = arguments[4] == 0.0 ? false : true;
                            HCReal x = arguments[5];
                            HCReal y = arguments[6];
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathAddArc(self, x, y, xr, yr, rotation, largeArc, sweep);
                        } break;
                    }
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Evaluation
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathEvaluateLine(HCReal t, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCReal* sx, HCReal* sy, HCReal* dx, HCReal* dy) {
    HCReal tc = 1.0 - t;
        
//    HCReal x = x0 * tc + x1 * t;
//    HCReal y = y0 * tc + y1 * t;
        
    HCReal a = tc;
    HCReal b = t;
    HCReal x = a * x0 + b * x1;
    HCReal y = a * y0 + b * y1;
    
    *sx = x;
    *sy = y;
    *dx = x1 - x0;
    *dy = y1 - y0;
}

void HCPathEvaluateQuadraticCurve(HCReal t, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCReal* sx, HCReal* sy, HCReal* dx, HCReal* dy) {
    HCReal tc = 1.0 - t;
    
    HCReal sx0 = x0 * tc + cx * t;
    HCReal sy0 = y0 * tc + cy * t;
    HCReal sx1 = cx * tc + x1 * t;
    HCReal sy1 = cy * tc + y1 * t;
    HCReal x = sx0 * tc + sx1 * t;
    HCReal y = sy0 * tc + sy1 * t;
    
//    HCReal x = (x0 * tc + cx * t) * tc + (cx * tc + x1 * t) * t;
//    HCReal y = (y0 * tc + cy * t) * tc + (cy * tc + y1 * t) * t;

//    HCReal x = x0 * tc * tc + cx * t * tc + cx * tc * t + x1 * t * t;
//    HCReal y = y0 * tc * tc + cy * t * tc + cy * tc * t + y1 * t * t;

//    HCReal t2 = t * t;
//    HCReal tc2 = tc * tc;
//    HCReal x = x0 * tc2 + cx * t * tc + cx * tc * t + x1 * t2;
//    HCReal y = y0 * tc2 + cy * t * tc + cy * tc * t + y1 * t2;
    
//    HCReal t2 = t * t;
//    HCReal tc2 = tc * tc;
//    HCReal x = x0 * tc2 + (2.0 * cx) * t * tc + x1 * t2;
//    HCReal y = y0 * tc2 + (2.0 * cy) * t * tc + y1 * t2;
    
//    HCReal a = tc * tc;
//    HCReal b = 2.0 * t * tc;
//    HCReal c = t * t;
//    HCReal x = a * x0 + b * cx + c * x1;
//    HCReal y = a * y0 + b * cy + c * y1;
    
    *sx = x;
    *sy = y;
    *dx = sx1 - sx0;
    *dy = sy1 - sy0;
}

void HCPathEvaluateCubicCurve(HCReal t, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCReal* sx, HCReal* sy, HCReal* dx, HCReal* dy) {
    HCReal tc = 1.0 - t;
        
    HCReal sx0 = x0 * tc + cx0 * t;
    HCReal sy0 = y0 * tc + cy0 * t;
    HCReal scx = cx0 * tc + cx1 * t;
    HCReal scy = cy0 * tc + cy1 * t;
    HCReal sx1 = cx1 * tc + x1 * t;
    HCReal sy1 = cy1 * tc + y1 * t;
    HCReal ssx0 = sx0 * tc + scx * t;
    HCReal ssy0 = sy0 * tc + scy * t;
    HCReal ssx1 = scx * tc + sx1 * t;
    HCReal ssy1 = scy * tc + sy1 * t;
    HCReal x = ssx0 * tc + ssx1 * t;
    HCReal y = ssy0 * tc + ssy1 * t;

//    HCReal ssx0 = (x0 * tc + cx0 * t) * tc + (cx0 * tc + cx1 * t) * t;
//    HCReal ssy0 = (y0 * tc + cy0 * t) * tc + (cy0 * tc + cy1 * t) * t;
//    HCReal ssx1 = (cx0 * tc + cx1 * t) * tc + (cx1 * tc + x1 * t) * t;
//    HCReal ssy1 = (cy0 * tc + cy1 * t) * tc + (cy1 * tc + y1 * t) * t;
//    HCReal x = ssx0 * tc + ssx1 * t;
//    HCReal y = ssy0 * tc + ssy1 * t;

//    HCReal x = ((x0 * tc + cx0 * t) * tc + (cx0 * tc + cx1 * t) * t) * tc + ((cx0 * tc + cx1 * t) * tc + (cx1 * tc + x1 * t) * t) * t;
//    HCReal y = ((y0 * tc + cy0 * t) * tc + (cy0 * tc + cy1 * t) * t) * tc + ((cy0 * tc + cy1 * t) * tc + (cy1 * tc + y1 * t) * t) * t;

//    HCReal x = (x0 * tc + cx0 * t) * tc * tc + (cx0 * tc + cx1 * t) * t * tc + (cx0 * tc + cx1 * t) * t * tc + (cx1 * tc + x1 * t) * t * t;
//    HCReal y = (y0 * tc + cy0 * t) * tc * tc + (cy0 * tc + cy1 * t) * t * tc + (cy0 * tc + cy1 * t) * t * tc + (cy1 * tc + y1 * t) * t * t;

//    HCReal x = x0 * tc * tc * tc + cx0 * tc * tc * t + cx0 * t * tc * tc + cx1 * t * t * tc + cx0 * t * tc * tc + cx1 * t * t * tc + cx1 * t * t * tc + x1 * t * t * t;
//    HCReal y = y0 * tc * tc * tc + cy0 * tc * tc * t + cy0 * t * tc * tc + cy1 * t * t * tc + cy0 * t * tc * tc + cy1 * t * t * tc + cy1 * t * t * tc + y1 * t * t * t;

//    HCReal t2 = t * t;
//    HCReal t3 = t * t * t;
//    HCReal tc2 = tc * tc;
//    HCReal tc3 = tc * tc * tc;
//    HCReal x = x0 * tc3 + cx0 * t * tc2 + cx0 * t * tc2 + cx1 * t2 * tc + cx0 * t * tc2 + cx1 * t2 * tc + cx1 * t2 * tc + x1 * t3;
//    HCReal y = y0 * tc3 + cy0 * t * tc2 + cy0 * t * tc2 + cy1 * t2 * tc + cy0 * t * tc2 + cy1 * t2 * tc + cy1 * t2 * tc + y1 * t3;

//    HCReal t2 = t * t;
//    HCReal t3 = t * t * t;
//    HCReal tc2 = tc * tc;
//    HCReal tc3 = tc * tc * tc;
//    HCReal x = x0 * tc3 + (3.0 * cx0) * t * tc2 + (3.0 * cx1) * t2 * tc + x1 * t3;
//    HCReal y = y0 * tc3 + (3.0 * cy0) * t * tc2 + (3.0 * cy1) * t2 * tc + y1 * t3;
    
//    HCReal a = tc * tc * tc;
//    HCReal b = 3.0 * t * tc * tc;
//    HCReal c = 3.0 * t * t * tc;
//    HCReal d = t * t * t;
//    HCReal x = a * x0 + b * cx0 + c * cx1 + d * x1;
//    HCReal y = a * y0 + b * cy0 + c * cy1 + d * y1;
    
    *sx = x;
    *sy = y;
    *dx = ssx1 - ssx0;
    *dy = ssy1 - ssy0;
}
