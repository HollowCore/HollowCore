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
    HCPathRef self = HCPathCreateEmpty();
    HCPathParse(self, path);
    return self;
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
                HCPathClose(self);
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
                            HCPathMove(self, subpathStartX, subpathStartY);
                            type = type == 'm' ? 'l' : 'L';
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
    HCReal cosPhi = cos(rotation);
    HCReal sinPhi = sin(rotation);
    
    // Convert from arc end-point parameterization to arc center parameterization and ensure passed radii can span the gap between p0 and p1
    HCReal midX = 0.5 * (p0.x - p1.x);
    HCReal midY = 0.5 * (p0.y - p1.y);
    HCReal x1p = +cosPhi * midX + +sinPhi * midY;
    HCReal y1p = -sinPhi * midX + +cosPhi * midY;
    HCReal radiiAdjustmentSquared = (x1p * x1p) / (xr * xr) + (y1p * y1p) / (yr * yr);
    if (radiiAdjustmentSquared > 1.0) {
        HCReal radiiAdjustment = sqrt(radiiAdjustmentSquared);
        xr *= radiiAdjustment;
        yr *= radiiAdjustment;
    }
    HCReal cc = sqrt((xr*xr*yr*yr - xr*xr*y1p*y1p - yr*yr*x1p*x1p) / (xr*xr*y1p*y1p + yr*yr*x1p*x1p));
    cc = (largeArc == sweep) ? -cc : cc;
    HCReal cxp = cc * +((xr / yr) * y1p);
    HCReal cyp = cc * -((yr / xr) * x1p);
    HCReal cx = +cosPhi * cxp + -sinPhi * cyp + (0.5 * (p0.x + p1.x));
    HCReal cy = +sinPhi * cxp + +cosPhi * cyp + (0.5 * (p0.y + p1.y));
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
    
    HCReal angleSpanRemaining = angleSpan;
    HCReal angleSpanMaximum = M_PI * 0.5;
    HCReal angle = angleStart;
    while (angleSpanRemaining > 0.0) {
        HCReal angleSliceSpan = fmin(angleSpanRemaining, angleSpanMaximum);
        HCReal angleSliceEnd = fmod(angle + angleSliceSpan * (sweep ? 1.0 : -1.0), 2.0 * M_PI);
        HCReal approximation = 4.0 / 3.0 * tan(angleSliceSpan * 0.25);
        HCReal rotatedCosPhi = cos(angleSliceSpan);
        HCReal rotatedSinPhi = sin(angleSliceSpan);
        HCReal cx0Rotated = xr;
        HCReal cy0Rotated = yr * approximation;
        HCReal cx1Rotated = xr * (rotatedCosPhi + approximation * rotatedSinPhi);
        HCReal cy1Rotated = yr * (rotatedSinPhi - approximation * rotatedCosPhi);
        HCReal xRotated = xr * rotatedCosPhi;
        HCReal yRotated = yr * rotatedSinPhi;
        if (true) {
            HCPoint current = HCPathCurrentPoint(self);
            HCReal x0Rotated = xr;
            HCReal y0Rotated = 0.0;
            HCReal x1Rotated = xRotated;
            HCReal y1Rotated = yRotated;
            HCReal a = 10;
            HCReal b = 10;
            HCPathMove(self, -5.0 + a, -5.0 + b);
            HCPathAddLine(self, 5.0 + a, 5.0 + b);
            HCPathMove(self, 5.0 + a, -5.0 + b);
            HCPathAddLine(self, -5.0 + a,  5.0 + b);
            HCPathMove(self, x0Rotated - 5.0 + a, y0Rotated + b);
            HCPathAddLine(self, x0Rotated + 5.0 + a, y0Rotated + b);
            HCPathMove(self, x0Rotated + a, y0Rotated - 5.0 + b);
            HCPathAddLine(self, x0Rotated + a, y0Rotated + 5.0 + b);
            HCPathMove(self, cx0Rotated - 5.0 + a, cy0Rotated + b);
            HCPathAddLine(self, cx0Rotated + 5.0 + a, cy0Rotated + b);
            HCPathMove(self, cx0Rotated + a, cy0Rotated - 5.0 + b);
            HCPathAddLine(self, cx0Rotated + a, cy0Rotated + 5.0 + b);
            HCPathMove(self, cx1Rotated - 5.0 + a, cy1Rotated + b);
            HCPathAddLine(self, cx1Rotated + 5.0 + a, cy1Rotated + b);
            HCPathMove(self, cx1Rotated + a, cy1Rotated - 5.0 + b);
            HCPathAddLine(self, cx1Rotated + a, cy1Rotated + 5.0 + b);
            HCPathMove(self, x1Rotated - 5.0 + a, y1Rotated + b);
            HCPathAddLine(self, x1Rotated + 5.0 + a, y1Rotated + b);
            HCPathMove(self, x1Rotated + a, y1Rotated - 5.0 + b);
            HCPathAddLine(self, x1Rotated + a, y1Rotated + 5.0 + b);
            HCPathMove(self, current.x, current.y);
        }
        HCReal cosPhi = cos(angle);
        HCReal sinPhi = sin(angle);
        HCReal px0 = HCPathCurrentPoint(self).x;
        HCReal py0 = HCPathCurrentPoint(self).y;
        HCReal cx0 = cosPhi * cx0Rotated - sinPhi * cy0Rotated + cx;
        HCReal cy0 = sinPhi * cx0Rotated + cosPhi * cy0Rotated + cy;
        HCReal cx1 = cosPhi * cx1Rotated - sinPhi * cy1Rotated + cx;
        HCReal cy1 = sinPhi * cx1Rotated + cosPhi * cy1Rotated + cy;
        HCReal px1 = cosPhi * xRotated - sinPhi * yRotated + cx;
        HCReal py1 = sinPhi * xRotated + cosPhi * yRotated + cy;
        if (true) {
            HCPoint current = HCPathCurrentPoint(self);
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
        HCPathAddCubicCurve(self, cx0, cy0, cx1, cy1, px1, py1);
        angleSpanRemaining -= angleSliceSpan;
        angle = angleSliceEnd;
    }
    
    // Calculate a cubic bezier approximation of the arc as if it were origin-centered, x-axis oriented
    // TODO: Make non-broken version of this!
//    HCReal rotatedPhi = angleDifference;
//    HCReal approximation = 4.0 / 3.0 * tan(rotatedPhi * 0.25);
//    HCReal rotatedCosPhi = cos(rotatedPhi);
//    HCReal rotatedSinPhi = sin(rotatedPhi);
//    HCPoint rotatedP0 = HCPointMake(
//        xr,
//        0.0);
//    HCPoint rotatedC0 = HCPointMake(
//        xr,
//        yr * approximation);
//    HCPoint rotatedC1 = HCPointMake(
//        xr * (rotatedCosPhi + approximation * rotatedSinPhi),
//        yr * (rotatedSinPhi - approximation * rotatedCosPhi));
//    HCPoint rotatedP1 = HCPointMake(
//        xr * rotatedCosPhi,
//        yr * rotatedSinPhi);
    
//    HCPathAddCubicCurve(self, c1.x, c1.y, c0.x, c0.y, x, y);
    
    // DEBUG: Plot ellipse center using + marker
    if (true) {
        HCPoint current = HCPathCurrentPoint(self);
        HCPathMove(self, cx - 5.0, cy - 5.0);
        HCPathAddLine(self, cx + 5.0, cy + 5.0);
        HCPathMove(self, cx - 5.0, cy + 5.0);
        HCPathAddLine(self, cx + 5.0, cy - 5.0);
        HCPathMove(self, current.x, current.y);
    }
    
    // DEBUG: Plot points of actual ellipse
    if (false) {
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
}
