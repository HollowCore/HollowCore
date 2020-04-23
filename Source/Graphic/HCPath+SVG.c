//
//  HCPath+SVG.c
//  HollowCore
//
//  Created by Matt Stoker on 3/21/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#include "HCPath+SVG.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCPathRef HCPathCreateWithSVGPathData(const char* path) {
    HCPathRef self = HCPathCreate();
    HCPathParseSVGPathData(self, path);
    return self;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Conversion
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathPrintSVGPathData(HCPathRef self, FILE* stream) {
    // Print SVG command for each element
    HCInteger elementCount = HCPathElementCount(self);
    for (HCInteger elementIndex = 0; elementIndex < elementCount; elementIndex++) {
        HCPathElement element = HCPathElementAt(self, elementIndex);
        switch (element.command) {
            case HCPathCommandMove: fprintf(stream, "M %f %f ", element.points[0].x, element.points[0].y); break;
            case HCPathCommandAddLine: fprintf(stream, "L %f %f ", element.points[0].x, element.points[0].y); break;
            case HCPathCommandAddQuadraticCurve: fprintf(stream, "Q %f %f %f %f ", element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y); break;
            case HCPathCommandAddCubicCurve: fprintf(stream, "C %f %f %f %f %f %f ", element.points[0].x, element.points[0].y, element.points[1].x, element.points[1].y, element.points[2].x, element.points[2].y); break;
            case HCPathCommandCloseContour: fprintf(stream, "Z "); break;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Parsing
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathParseSVGPathData(HCPathRef self, const char* path) {
    // Define state data for parsing the SVG path string
    char type = '\0';
    HCReal contourStartX = 0.0;
    HCReal contourStartY = 0.0;
    HCReal contourEndX = contourStartX;
    HCReal contourEndY = contourStartY;
    HCReal currentX = contourEndX;
    HCReal currentY = contourEndY;
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
            case 'M': type = 'M'; argumentsExpected = 2; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'm': type = 'm'; argumentsExpected = 2; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'L': type = 'L'; argumentsExpected = 2; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'l': type = 'l'; argumentsExpected = 2; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'H': type = 'H'; argumentsExpected = 1; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'h': type = 'h'; argumentsExpected = 1; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'V': type = 'V'; argumentsExpected = 1; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'v': type = 'v'; argumentsExpected = 1; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'Q': type = 'Q'; argumentsExpected = 4; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'q': type = 'q'; argumentsExpected = 4; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'T': type = 'T'; argumentsExpected = 2; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 't': type = 't'; argumentsExpected = 2; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'C': type = 'C'; argumentsExpected = 6; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'c': type = 'c'; argumentsExpected = 6; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'S': type = 'S'; argumentsExpected = 4; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 's': type = 's'; argumentsExpected = 4; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'A': type = 'A'; argumentsExpected = 7; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'a': type = 'a'; argumentsExpected = 7; currentX = contourEndX; currentY = contourEndY; current++; break;
            case 'Z': // Fallthrough
            case 'z':
                // Close path has no arguments, so just perform the action and move on
                HCPathClose(self);
                contourEndX = contourStartX;
                contourEndY = contourStartY;
                currentX = contourEndX;
                currentY = contourEndY;
                current++;
                break;
            default: {
                // Parse an argument number
                char* end = (char*)current;
                double argument = strtod(current, &end);
                if (end <= current) {
                    // Invalid argument, terminate parsing and return path as far as parsed to this point as directed by SVG specification
                    return;
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
                            arguments[0] += contourEndX;
                            arguments[1] += contourEndY;
                            // Fallthrough
                        case 'M': {
                            contourStartX = arguments[0];
                            contourStartY = arguments[1];
                            contourEndX = contourStartX;
                            contourEndY = contourStartY;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathMove(self, contourStartX, contourStartY);
                            type = type == 'm' ? 'l' : 'L';
                        } break;
                        case 'l':
                            arguments[0] += contourEndX;
                            arguments[1] += contourEndY;
                            // Fallthrough
                        case 'L': {
                            HCReal x = arguments[0];
                            HCReal y = arguments[1];
                            contourEndX = x;
                            contourEndY = y;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathAddLine(self, x, y);
                        } break;
                        case 'h':
                            arguments[0] += contourEndX;
                            // Fallthrough
                        case 'H': {
                            HCReal x = arguments[0];
                            HCReal y = contourEndY;
                            contourEndX = x;
                            contourEndY = y;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathAddLine(self, x, y);
                        } break;
                        case 'v':
                            arguments[0] += contourEndY;
                            // Fallthrough
                        case 'V': {
                            HCReal x = contourEndX;
                            HCReal y = arguments[0];
                            contourEndX = x;
                            contourEndY = y;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathAddLine(self, x, y);
                        } break;
                        case 'q':
                            arguments[0] += contourEndX;
                            arguments[1] += contourEndY;
                            arguments[2] += contourEndX;
                            arguments[3] += contourEndY;
                            // Fallthrough
                        case 'Q': {
                            HCReal cx = arguments[0];
                            HCReal cy = arguments[1];
                            HCReal x = arguments[2];
                            HCReal y = arguments[3];
                            contourEndX = x;
                            contourEndY = y;
                            previousControlX = cx;
                            previousControlY = cy;
                            HCPathAddQuadraticCurve(self, cx, cy, x, y);
                        } break;
                        case 't':
                            arguments[0] += contourEndX;
                            arguments[1] += contourEndY;
                            // Fallthrough
                        case 'T': {
                            HCReal cx = 2.0 * contourEndX - previousControlX;
                            HCReal cy = 2.0 * contourEndY - previousControlY;
                            HCReal x = arguments[0];
                            HCReal y = arguments[1];
                            contourEndX = x;
                            contourEndY = y;
                            previousControlX = cx;
                            previousControlY = cy;
                            HCPathAddQuadraticCurve(self, cx, cy, x, y);
                        } break;
                        case 'c':
                            arguments[0] += contourEndX;
                            arguments[1] += contourEndY;
                            arguments[2] += contourEndX;
                            arguments[3] += contourEndY;
                            arguments[4] += contourEndX;
                            arguments[5] += contourEndY;
                            // Fallthrough
                        case 'C': {
                            HCReal c0x = arguments[0];
                            HCReal c0y = arguments[1];
                            HCReal c1x = arguments[2];
                            HCReal c1y = arguments[3];
                            HCReal x = arguments[4];
                            HCReal y = arguments[5];
                            contourEndX = x;
                            contourEndY = y;
                            previousControlX = c1x;
                            previousControlY = c1y;
                            HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, x, y);
                        } break;
                        case 's':
                            arguments[0] += contourEndX;
                            arguments[1] += contourEndY;
                            arguments[2] += contourEndX;
                            arguments[3] += contourEndY;
                            // Fallthrough
                        case 'S': {
                            HCReal c0x = 2.0 * contourEndX - previousControlX;
                            HCReal c0y = 2.0 * contourEndY - previousControlY;
                            HCReal c1x = arguments[0];
                            HCReal c1y = arguments[1];
                            HCReal x = arguments[2];
                            HCReal y = arguments[3];
                            contourEndX = x;
                            contourEndY = y;
                            previousControlX = c1x;
                            previousControlY = c1y;
                            HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, x, y);
                        } break;
                        case 'a':
                            arguments[5] += contourEndX;
                            arguments[6] += contourEndY;
                            // Fallthrough
                        case 'A': {
                            HCReal xr = arguments[0];
                            HCReal yr = arguments[1];
                            HCReal rotation = arguments[2] / 180.0 * M_PI;
                            HCBoolean largeArc = arguments[3] == 0.0 ? false : true;
                            HCBoolean sweep = arguments[4] == 0.0 ? false : true;
                            HCReal x = arguments[5];
                            HCReal y = arguments[6];
                            contourEndX = x;
                            contourEndY = y;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathAddCubicCurvesApproximatingArc(self, xr, yr, rotation, largeArc, sweep, x, y);
                        } break;
                    }
                }
            }
        }
    }
}

void HCPathAddCubicCurvesApproximatingArc(HCPathRef self, HCReal xr, HCReal yr, HCReal rotation, HCBoolean largeArc, HCBoolean sweep, HCReal x, HCReal y) {
    // Correct radii or rotation that are malformed
    xr = fabs(xr);
    yr = fabs(yr);
    rotation = fmod(rotation, 2.0 * M_PI);
    if (xr == 0.0 || yr == 0.0) {
        HCPathAddLine(self, x, y);
        return;
    }
    
    // Obtain the current point for the path as the arc start point, and package the arc end point
    HCPoint p0 = HCPathCurrentPoint(self);
    HCPoint p1 = HCPointMake(x, y);
    if (HCPointIsEqual(p0, p1)) {
        return;
    }
    HCReal cosRotation = cos(rotation);
    HCReal sinRotation = sin(rotation);
    
    // Convert from arc end-point parameterization to arc center parameterization and ensure passed radii can span the gap between p0 and p1
    HCReal midX = 0.5 * (p0.x - p1.x);
    HCReal midY = 0.5 * (p0.y - p1.y);
    HCReal x1p = +cosRotation * midX + +sinRotation * midY;
    HCReal y1p = -sinRotation * midX + +cosRotation * midY;
    HCReal radiiAdjustmentSquared = (x1p * x1p) / (xr * xr) + (y1p * y1p) / (yr * yr);
    if (radiiAdjustmentSquared > 1.0) {
        HCReal radiiAdjustment = sqrt(radiiAdjustmentSquared);
        xr *= radiiAdjustment;
        yr *= radiiAdjustment;
    }
    HCReal ccn = (xr*xr*yr*yr - xr*xr*y1p*y1p - yr*yr*x1p*x1p);
    HCReal ccd = (xr*xr*y1p*y1p + yr*yr*x1p*x1p);
    HCReal cc = sqrt(fabs(ccn) / ccd);
    cc = (largeArc == sweep) ? -cc : cc;
    HCReal cxp = cc * +((xr / yr) * y1p);
    HCReal cyp = cc * -((yr / xr) * x1p);
    HCReal cx = +cosRotation * cxp + -sinRotation * cyp + (0.5 * (p0.x + p1.x));
    HCReal cy = +sinRotation * cxp + +cosRotation * cyp + (0.5 * (p0.y + p1.y));
    HCReal arcStartX = (x1p - cxp) / xr;
    HCReal arcStartY = (y1p - cyp) / yr;
    HCReal arcEndX = (-x1p - cxp) / xr;
    HCReal arcEndY = (-y1p - cyp) / yr;
    HCReal angleStart = atan2(arcStartY, arcStartX);
    HCReal angleEnd = atan2(arcEndY, arcEndX);
    HCReal angleSpan = fabs(angleEnd - angleStart);
    if (largeArc && angleSpan < M_PI) {
        angleSpan = fabs(angleSpan - 2.0 * M_PI);
    }
    if (!largeArc && angleSpan > M_PI) {
        angleSpan = fabs(angleSpan - 2.0 * M_PI);
    }
    
    // Calculate cubic curves to represent the arc, splitting the angle span when it exceeds the defined maximum (typically pi/2)
    HCReal angleSpanRemaining = angleSpan;
    HCReal angleSpanMaximum = M_PI * 0.5;
    HCReal angle = angleStart;
    while (angleSpanRemaining > 0.0) {
        // Calculate the angle slice to be represented by this curve
        HCReal angleSliceSpan = fmin(angleSpanRemaining, angleSpanMaximum);
        HCReal angleSlice = angleSliceSpan * (sweep ? 1.0 : -1.0);
        HCReal angleSliceEnd = fmod(angle + angleSlice , 2.0 * M_PI);
        
        // Calculate a representation of the angle slice on the unit circle using a cubic curve
        HCReal approximation = 4.0 / 3.0 * tan(angleSlice * 0.25);
        HCReal rotatedCosPhi = cos(angleSlice);
        HCReal rotatedSinPhi = sin(angleSlice);
        HCReal cx0Rotated = 1.0;
        HCReal cy0Rotated = approximation;
        HCReal cx1Rotated = rotatedCosPhi + approximation * rotatedSinPhi;
        HCReal cy1Rotated = rotatedSinPhi - approximation * rotatedCosPhi;
        HCReal xRotated = rotatedCosPhi;
        HCReal yRotated = rotatedSinPhi;

        // Rotate and scale the unit circle approximation curve to fix the desired ellipse size and eccentricity
        HCReal cosAngle = cos(angle);
        HCReal sinAngle = sin(angle);
        HCReal cx0p = xr * (cosAngle * cx0Rotated - sinAngle * cy0Rotated);
        HCReal cy0p = yr * (sinAngle * cx0Rotated + cosAngle * cy0Rotated);
        HCReal cx1p = xr * (cosAngle * cx1Rotated - sinAngle * cy1Rotated);
        HCReal cy1p = yr * (sinAngle * cx1Rotated + cosAngle * cy1Rotated);
        HCReal px1p = xr * (cosAngle * xRotated - sinAngle * yRotated);
        HCReal py1p = yr * (sinAngle * xRotated + cosAngle * yRotated);
        
        // Rotate the ellipse by the desired rotation angle
        HCReal cx0 = cosRotation * cx0p - sinRotation * cy0p + cx;
        HCReal cy0 = sinRotation * cx0p + cosRotation * cy0p + cy;
        HCReal cx1 = cosRotation * cx1p - sinRotation * cy1p + cx;
        HCReal cy1 = sinRotation * cx1p + cosRotation * cy1p + cy;
        HCReal px1 = cosRotation * px1p - sinRotation * py1p + cx;
        HCReal py1 = sinRotation * px1p + cosRotation * py1p + cy;

        // Determine how much more of the arc needs to be approximated
        angleSpanRemaining -= angleSliceSpan;
        angle = angleSliceEnd;
        
        // Where this is the last slice of the arc, ensure the final point is exactly as requested
        if (angleSpanRemaining <= 0.0) {
            px1 = x;
            py1 = y;
        }
        
        // Add the completed approximation curve for the angle slice to the path
        HCPathAddCubicCurve(self, cx0, cy0, cx1, cy1, px1, py1);
        
#define VISUALIZE_ARC false
#if (VISUALIZE_ARC)
        {
            // Plot + marks to visualize the control points of the completed approximation curve
            HCPoint current = HCPathCurrentPoint(self);
            HCReal px0 = current.x;
            HCReal py0 = current.y;
            HCPathMove(self, px0 - 5.0, py0);
            HCPathAddLine(self, px0 + 5.0, py0);
            HCPathMove(self, px0, py0 - 5.0);
            HCPathAddLine(self, px0, py0 + 5.0);
            HCPathMove(self, cx0 - 5.0, cy0);
            HCPathAddLine(self, cx0 + 5.0, cy0);
            HCPathMove(self, cx0, cy0 - 5.0);
            HCPathAddLine(self, cx0, cy0 + 5.0);
            HCPathMove(self, cx1 - 5.0, cy1);
            HCPathAddLine(self, cx1 + 5.0, cy1);
            HCPathMove(self, cx1, cy1 - 5.0);
            HCPathAddLine(self, cx1, cy1 + 5.0);
            HCPathMove(self, px1 - 5.0, py1);
            HCPathAddLine(self, px1 + 5.0, py1);
            HCPathMove(self, px1, py1 - 5.0);
            HCPathAddLine(self, px1, py1 + 5.0);
            HCPathMove(self, current.x, current.y);
        }
    }
    
    {
        // Plot ellipse center using x marker
        HCPoint current = HCPathCurrentPoint(self);
        HCPathMove(self, cx - 5.0, cy - 5.0);
        HCPathAddLine(self, cx + 5.0, cy + 5.0);
        HCPathMove(self, cx - 5.0, cy + 5.0);
        HCPathAddLine(self, cx + 5.0, cy - 5.0);
        HCPathMove(self, current.x, current.y);
    }
    
    {
        // Plot points of actual ellipse
        HCPoint current = HCPathCurrentPoint(self);
        HCInteger stepCount = 100;
        HCReal angleStep = angleSpan / (HCReal)stepCount;
        angleStep *= (sweep ? 1.0 : -1.0);
        HCReal angle = angleStart;
        HCPathMove(self, p0.x, p0.y);
        for (HCInteger step = 0; step < stepCount; step++) {
            HCReal tx = xr * cos(angle);
            HCReal ty = yr * sin(angle);
            HCReal x = cosPhi * tx - sinPhi * ty + cx;
            HCReal y = sinPhi * tx + cosPhi * ty + cy;
            HCPathAddLine(self, x, y);
            angle = fmod(angle + angleStep, 2.0 * M_PI);
        }
        HCPathAddLine(self, p1.x, p1.y);
        HCPathMove(self, current.x, current.y);
    }
#else
    }
#endif
}
