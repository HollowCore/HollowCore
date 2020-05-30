///
/// @file HCRaster+Draw.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 4/18/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCRaster+Draw.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Curve Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawPoint(HCRasterRef self, HCReal x, HCReal y, HCColor color) {
    HCInteger xIndex = round(x);
    HCInteger yIndex = round(y);
    HCColor pixel = HCRasterPixelAt(self, xIndex, yIndex);
    pixel = HCColorCombine(pixel, color, color.a);
    HCRasterSetPixelAt(self, xIndex, yIndex, pixel);
}

void HCRasterDrawLine(HCRasterRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCColor c0, HCColor c1) {
    if (!isfinite(x0) || !isfinite(y0) || !isfinite(x1) || !isfinite(y1)) {
        return;
    }
    
    // Draw using Bresenham's Algorithm with color interpolation
    HCInteger ix0 = round(x0);
    HCInteger iy0 = round(y0);
    HCInteger ix1 = round(x1);
    HCInteger iy1 = round(y1);
    HCInteger dx = llabs(ix1-ix0), sx = ix0<ix1 ? 1 : -1;
    HCInteger dy = llabs(iy1-iy0), sy = iy0<iy1 ? 1 : -1;
    HCInteger err = (dx>dy ? dx : -dy)/2, e2;
    HCReal xspaninv = 1.0 / (x1 - x0);
    HCReal yspaninv = 1.0 / (y1 - y0);
    while (true) {
        HCReal tx = isfinite(xspaninv) ? ((double)ix0 - x0) * xspaninv : NAN;
        HCReal ty = isfinite(yspaninv) ? ((double)iy0 - y0) * yspaninv : NAN;
        HCReal t = (!isnan(tx) && !isnan(ty)) ? ((tx + ty) * 0.5) : !isnan(tx) ? tx : !isnan(ty) ? ty : 0.0;
        HCColor color = HCColorCombine(c0, c1, t);
        HCColor pixel = HCRasterPixelAt(self, ix0, iy0);
        pixel = HCColorCombine(pixel, color, color.a);
        HCRasterSetPixelAt(self, ix0, iy0, pixel);
        if (ix0==ix1 && iy0==iy1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; ix0 += sx; }
        if (e2 < dy) { err += dx; iy0 += sy; }
    }
}

void HCRasterDrawQuadraticCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCColor c0, HCColor c1) {
    // Draw using De Casteljau's Algorithm
    // Determine quadratic curve flatness
    HCReal p0cDistance = sqrt((cx - x0) * (cx - x0) + (cy - y0) * (cy - y0));
    HCReal cp1Distance = sqrt((x1 - cx) * (x1 - cx) + (y1 - cy) * (y1 - cy));
    HCReal p0p1Distance = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    if (HCRealIsSimilar(p0cDistance, 0.0, FLT_EPSILON) &&
        HCRealIsSimilar(cp1Distance, 0.0, FLT_EPSILON) &&
        HCRealIsSimilar(p0p1Distance, 0.0, FLT_EPSILON)) {
        return;
    }
    HCReal flatness = (p0cDistance + cp1Distance) / p0p1Distance;
    HCReal flatnessThreshold = HCPathFlatnessNormal;
    if (flatness < flatnessThreshold) {
         HCRasterDrawLine(self, x0, y0, x1, y1, c0, c1);
        return;
    }

    HCReal qx0 =  x0 * 0.5 +  cx * 0.5;
    HCReal qy0 =  y0 * 0.5 +  cy * 0.5;
    HCReal qx1 =  cx * 0.5 +  x1 * 0.5;
    HCReal qy1 =  cy * 0.5 +  y1 * 0.5;
    HCReal  sx = qx0 * 0.5 + qx1 * 0.5;
    HCReal  sy = qy0 * 0.5 + qy1 * 0.5;
    HCColor cs = HCColorCombine(c0, c1, 0.5);
    HCRasterDrawQuadraticCurve(self, x0, y0, qx0, qy0, sx, sy, c0, cs);
    HCRasterDrawQuadraticCurve(self, sx, sy, qx1, qy1, x1, y1, cs, c1);
}

void HCRasterDrawCubicCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCColor c0, HCColor c1) {
    // Draw using De Casteljau's Algorithm
    // Determine cubic curve flatness
    HCReal p0c0Distance = sqrt((cx0 -  x0) * (cx0 -  x0) + (cy0 -  y0) * (cy0 -  y0));
    HCReal c0c1Distance = sqrt((cx1 - cx0) * (cx1 - cx0) + (cy1 - cy0) * (cy1 - cy0));
    HCReal c1p1Distance = sqrt(( x1 - cx1) * ( x1 - cx1) + ( y1 - cy1) * ( y1 - cy1));
    HCReal p0p1Distance = sqrt(( x1 -  x0) * ( x1 -  x0) + ( y1 -  y0) * ( y1 -  y0));
    if (HCRealIsSimilar(p0p1Distance, 0.0, FLT_EPSILON) &&
        HCRealIsSimilar(p0c0Distance, 0.0, FLT_EPSILON) &&
        HCRealIsSimilar(c0c1Distance, 0.0, FLT_EPSILON) &&
        HCRealIsSimilar(p0p1Distance, 0.0, FLT_EPSILON)) {
        return;
    }
    HCReal flatness = (p0c0Distance + c0c1Distance + c1p1Distance) / p0p1Distance;
    HCReal flatnessThreshold = HCPathFlatnessNormal;
    if (flatness < flatnessThreshold) {
        HCRasterDrawLine(self, x0, y0, x1, y1, c0, c1);
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
    HCColor cs = HCColorCombine(c0, c1, 0.5);
    HCRasterDrawCubicCurve(self, x0, y0, qx0, qy0, rx0, ry0, sx, sy, c0, cs);
    HCRasterDrawCubicCurve(self, sx, sy, rx1, ry1, qx1, qy1, x1, y1, cs, c1);
}

void HCRasterDrawArc(HCRasterRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCReal xr, HCReal yr, HCReal rotation, HCBoolean largeArc, HCBoolean sweep, HCColor c0, HCColor c1) {
    // Correct radii or rotation that are malformed
    xr = fabs(xr);
    yr = fabs(yr);
    rotation = fmod(rotation, 2.0 * M_PI);
    if (xr == 0.0 || yr == 0.0) {
        HCRasterDrawLine(self, x0, y0, x1, y1, c0, c1);
        return;
    }
    
    // Obtain the current point for the path as the arc start point, and package the arc end point
    HCPoint p0 = HCPointMake(x0, y0);
    HCPoint p1 = HCPointMake(x1, y1);
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
        HCReal x0Rotated = 1.0;
        HCReal y0Rotated = 0.0;
        HCReal cx0Rotated = 1.0;
        HCReal cy0Rotated = approximation;
        HCReal cx1Rotated = rotatedCosPhi + approximation * rotatedSinPhi;
        HCReal cy1Rotated = rotatedSinPhi - approximation * rotatedCosPhi;
        HCReal x1Rotated = rotatedCosPhi;
        HCReal y1Rotated = rotatedSinPhi;

        // Rotate and scale the unit circle approximation curve to fix the desired ellipse size and eccentricity
        HCReal cosAngle = cos(angle);
        HCReal sinAngle = sin(angle);
        HCReal px0p = xr * (cosAngle * x0Rotated - sinAngle * y0Rotated);
        HCReal py0p = yr * (sinAngle * x0Rotated + cosAngle * y0Rotated);
        HCReal cx0p = xr * (cosAngle * cx0Rotated - sinAngle * cy0Rotated);
        HCReal cy0p = yr * (sinAngle * cx0Rotated + cosAngle * cy0Rotated);
        HCReal cx1p = xr * (cosAngle * cx1Rotated - sinAngle * cy1Rotated);
        HCReal cy1p = yr * (sinAngle * cx1Rotated + cosAngle * cy1Rotated);
        HCReal px1p = xr * (cosAngle * x1Rotated - sinAngle * y1Rotated);
        HCReal py1p = yr * (sinAngle * x1Rotated + cosAngle * y1Rotated);
        
        // Rotate the ellipse by the desired rotation angle
        HCReal px0 = cosRotation * px0p - sinRotation * py0p + cx;
        HCReal py0 = sinRotation * px0p + cosRotation * py0p + cy;
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
            px1 = x1;
            py1 = y1;
        }
        
        // Draw the completed approximation curve for the angle slice
        HCColor c0p = HCColorInterpolate(c0, c1, (angleSpan - (angleSpanRemaining + angleSliceSpan)) / angleSpan);
        HCColor c1p = HCColorInterpolate(c0, c1, (angleSpan - angleSpanRemaining) / angleSpan);
        HCRasterDrawCubicCurve(self, px0, py0, cx0, cy0, cx1, cy1, px1, py1, c0p, c1p);
    }
}

void HCRasterDrawEllipse(HCRasterRef self, HCReal cx, HCReal cy, HCReal xr, HCReal yr, HCReal rotation, HCColor c0, HCColor c1) {
    HCRasterDrawArc(self, cx - xr, cy, cx + xr, cy, xr, yr, 0.0, false, false, c0, c1);
    HCRasterDrawArc(self, cx - xr, cy, cx + xr, cy, xr, yr, 0.0, false, true, c0, c1);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Shape Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawTriangle(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCColor ca, HCColor cb, HCColor cc) {
    HCRasterDrawLine(self, ax, ay, bx, by, ca, cb);
    HCRasterDrawLine(self, bx, by, cx, cy, cb, cc);
    HCRasterDrawLine(self, cx, cy, ax, ay, cc, ca);
}

void HCRasterFillTriangle(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCColor ca, HCColor cb, HCColor cc) {
    // Calculate triangle constants for barycentric coordinates
    HCReal determinantInv = 1.0 / ((by - cy)*(ax - cx) + (cx - bx)*(ay - cy));
    HCReal cxbxDifference = cx - bx;
    HCReal axcxDifference = ax - cx;
    HCReal bycyDifference = by - cy;
    HCReal cyayDifference = cy - ay;
    
    // Iterate over the pixels the triangle could cover, filling the pixels that are covered
    // TODO: Use a non-sampling draw algorithm
    HCInteger startXIndex = floor(fmin(ax, fmin(bx, cx)));
    HCInteger startYIndex = floor(fmin(ay, fmin(by, cy)));
    HCInteger endXIndex = ceil(fmax(ax, fmax(bx, cx)));
    HCInteger endYIndex = ceil(fmax(ay, fmax(by, cy)));
    for (HCInteger yIndex = startYIndex; yIndex < endYIndex; yIndex++) {
        for (HCInteger xIndex = startXIndex; xIndex < endXIndex; xIndex++) {
            // Calculate barycenteric coordinates for the coordinate
            HCReal x = round(xIndex);
            HCReal y = round(yIndex);
            HCReal lambdaA = (bycyDifference*(x - cx) + cxbxDifference*(y - cy)) * determinantInv;
            HCReal lambdaB = (cyayDifference*(x - cx) + axcxDifference*(y - cy)) * determinantInv;
            HCReal lambdaC = 1.0 - lambdaA - lambdaB;
            
            // Fill the pixel if it is covered
            if (lambdaA >= 0.0 && lambdaB >= 0.0 && lambdaC >= 0.0) {
                HCColor color = HCColorCombine3(ca, lambdaA, cb, lambdaB, cc, lambdaC);
                HCColor pixel = HCRasterPixelAt(self, xIndex, yIndex);
                pixel = HCColorCombine(pixel, color, color.a);
                HCRasterSetPixelAt(self, xIndex, yIndex, pixel);
            }
        }
    }
}

void HCRasterFillTexturedTriangle(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCRasterRef texture, HCReal tax, HCReal tay, HCReal tbx, HCReal tby, HCReal tcx, HCReal tcy) {
    // Calculate triangle constants for barycentric coordinates
    HCReal determinantInv = 1.0 / ((by - cy)*(ax - cx) + (cx - bx)*(ay - cy));
    HCReal cxbxDifference = cx - bx;
    HCReal axcxDifference = ax - cx;
    HCReal bycyDifference = by - cy;
    HCReal cyayDifference = cy - ay;
    
    // Iterate over the pixels the triangle could cover, filling the pixels that are covered
    // TODO: Use a non-sampling draw algorithm
    HCInteger startXIndex = floor(fmin(ax, fmin(bx, cx)));
    HCInteger startYIndex = floor(fmin(ay, fmin(by, cy)));
    HCInteger endXIndex = ceil(fmax(ax, fmax(bx, cx)));
    HCInteger endYIndex = ceil(fmax(ay, fmax(by, cy)));
    for (HCInteger yIndex = startYIndex; yIndex < endYIndex; yIndex++) {
        for (HCInteger xIndex = startXIndex; xIndex < endXIndex; xIndex++) {
            // Calculate barycenteric coordinates for the coordinate
            HCReal x = xIndex + 0.5;
            HCReal y = yIndex + 0.5;
            HCReal lambdaA = (bycyDifference*(x - cx) + cxbxDifference*(y - cy)) * determinantInv;
            HCReal lambdaB = (cyayDifference*(x - cx) + axcxDifference*(y - cy)) * determinantInv;
            HCReal lambdaC = 1.0 - lambdaA - lambdaB;
            
            // Fill the pixel if it is covered
            if (lambdaA >= 0.0 && lambdaB >= 0.0 && lambdaC >= 0.0) {
                HCReal tx = lambdaA * tax + lambdaB * tbx + lambdaC * tcx;
                HCReal ty = lambdaA * tay + lambdaB * tby + lambdaC * tcy;
                HCColor color = HCRasterPixelFiltered(texture, tx, ty);
                HCColor pixel = HCRasterPixelAt(self, xIndex, yIndex);
                pixel = HCColorCombine(pixel, color, color.a);
                HCRasterSetPixelAt(self, xIndex, yIndex, pixel);
            }
        }
    }
}

void HCRasterDrawQuad(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCReal dx, HCReal dy, HCColor ca, HCColor cb, HCColor cc, HCColor cd) {
    HCRasterDrawLine(self, ax, ay, bx, by, ca, cb);
    HCRasterDrawLine(self, bx, by, cx, cy, cb, cc);
    HCRasterDrawLine(self, cx, cy, dx, dy, cc, cd);
    HCRasterDrawLine(self, dx, dy, ax, ay, cd, ca);
}

void HCRasterFillQuad(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCReal dx, HCReal dy, HCColor ca, HCColor cb, HCColor cc, HCColor cd) {
    HCRasterFillTriangle(self, ax, ay, bx, by, cx, cy, ca, cb, cc);
    HCRasterFillTriangle(self, ax, ay, cx, cy, dx, dy, ca, cc, cd);
}

void HCRasterFillTexturedQuad(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCReal dx, HCReal dy, HCRasterRef texture, HCReal tax, HCReal tay, HCReal tbx, HCReal tby, HCReal tcx, HCReal tcy, HCReal tdx, HCReal tdy) {
    HCRasterFillTexturedTriangle(self, ax, ay, bx, by, cx, cy, texture, tax, tay, tbx, tby, tcx, tcy);
    HCRasterFillTexturedTriangle(self, ax, ay, cx, cy, dx, dy, texture, tax, tay, tcx, tcy, tdx, tdy);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contour Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawPolyline(HCRasterRef self, HCPoint startPoint, const HCPoint* points, HCInteger pointCount, HCBoolean closed, HCColor color) {
    // Drawing empty polylines does nothing
    if (points == NULL || pointCount <= 0) {
        return;
    }
    
    // Draw each polyline segment
    HCBoolean rotatingColor = HCColorIsEqual(color, HCRasterColorRotating);
    HCPoint currentPoint = startPoint;
    for (HCInteger pointIndex = 0; pointIndex < pointCount; pointIndex++) {
        // When the rotating color is requested, change the color with each segment
        if (rotatingColor) {
            color = HCColorMake(1.0, 0.25 + fmod(color.r + 0.1, 0.75), 0.25 + fmod(color.g + 0.2, 0.75), 0.25 + fmod(color.b + 0.3, 0.75));
        }
        
        // Draw the segment
        HCPoint p = points[pointIndex];
        HCRasterDrawLine(self, currentPoint.x, currentPoint.y, p.x, p.y, color, color);
        currentPoint = p;
    }
    
    // Close the polyline, if requested and required
    HCPoint endPoint = currentPoint;
    if (closed && !HCPointIsEqual(endPoint, startPoint)) {
        HCRasterDrawLine(self, endPoint.x, endPoint.y, startPoint.x, startPoint.y, color, color);
    }
}

void HCRasterDrawPolyquadratic(HCRasterRef self, HCPoint startPoint, const HCPoint* points, HCInteger quadraticCount, HCBoolean closed, HCColor color) {
    // Drawing empty polylines does nothing
    if (points == NULL || quadraticCount <= 0) {
        return;
    }
    
    // Draw each quadratic curve
    HCBoolean rotatingColor = HCColorIsEqual(color, HCRasterColorRotating);
    HCPoint currentPoint = startPoint;
    for (HCInteger quadraticIndex = 0; quadraticIndex < quadraticCount; quadraticIndex++) {
        // When the rotating color is requested, change the color with each segment
        if (rotatingColor) {
            color = HCColorMake(1.0, 0.25 + fmod(color.r + 0.1, 0.75), 0.25 + fmod(color.g + 0.2, 0.75), 0.25 + fmod(color.b + 0.3, 0.75));
        }
        
        // Draw the quadratic curve
        HCPoint c = points[quadraticIndex * 2];
        HCPoint p = points[quadraticIndex * 2 + 1];
        if (HCPointIsInvalid(c)) {
            c = p;
        }
        HCRasterDrawQuadraticCurve(self, currentPoint.x, currentPoint.y, c.x, c.y, p.x, p.y, color, color);
        currentPoint = p;
    }
    
    // Close the polyquadratic, if requested and required
    HCPoint endPoint = currentPoint;
    if (closed && !HCPointIsEqual(endPoint, startPoint)) {
        HCRasterDrawLine(self, endPoint.x, endPoint.y, startPoint.x, startPoint.y, color, color);
    }
}

void HCRasterDrawPolycubic(HCRasterRef self, HCPoint startPoint, const HCPoint* points, HCInteger cubicCount, HCBoolean closed, HCColor color) {
    // Drawing empty polylines does nothing
    if (points == NULL || cubicCount <= 0) {
        return;
    }
    
    // Draw each cubic curve
    HCBoolean rotatingColor = HCColorIsEqual(color, HCRasterColorRotating);
    HCPoint currentPoint = startPoint;
    for (HCInteger cubicIndex = 0; cubicIndex < cubicCount; cubicIndex++) {
        // When the rotating color is requested, change the color with each segment
        if (rotatingColor) {
            color = HCColorMake(1.0, 0.25 + fmod(color.r + 0.1, 0.75), 0.25 + fmod(color.g + 0.2, 0.75), 0.25 + fmod(color.b + 0.3, 0.75));
        }
        
        // Draw the cubic curve
        HCPoint c0 = points[cubicIndex * 3];
        HCPoint c1 = points[cubicIndex * 3 + 1];
        HCPoint p = points[cubicIndex * 3 + 2];
        if (HCPointIsInvalid(c0)) {
            c0 = p;
        }
        if (HCPointIsInvalid(c1)) {
            c1 = c0;
        }
        HCRasterDrawCubicCurve(self, currentPoint.x, currentPoint.y, c0.x, c0.y, c1.x, c1.y, p.x, p.y, color, color);
        currentPoint = p;
    }
    
    // Close the polycubic, if requested and required
    HCPoint endPoint = currentPoint;
    if (closed && !HCPointIsEqual(endPoint, startPoint)) {
        HCRasterDrawLine(self, endPoint.x, endPoint.y, startPoint.x, startPoint.y, color, color);
    }
}

void HCRasterDrawContourCurves(HCRasterRef self, const HCContourCurve* curves, HCInteger curveCount, HCBoolean closed, HCColor color) {
    // Drawing empty contours does nothing
    if (curves == NULL || curveCount <= 0) {
        return;
    }
    
    // Draw each contour curve
    HCBoolean rotatingColor = HCColorIsEqual(color, HCRasterColorRotating);
    HCPoint startPoint = curves[0].p;
    HCPoint currentPoint = startPoint;
    for (HCInteger curveIndex = 1; curveIndex < curveCount; curveIndex++) {
        // When the rotating color is requested, change the color with each curve
        if (rotatingColor) {
            color = HCColorMake(1.0, 0.25 + fmod(color.r + 0.1, 0.75), 0.25 + fmod(color.g + 0.2, 0.75), 0.25 + fmod(color.b + 0.3, 0.75));
        }
        
        // Draw the curve
        HCPoint p0 = curves[curveIndex - 1].p;
        HCContourCurve curve = curves[curveIndex];
        if (HCContourCurveIsLinear(p0, curve)) {
            HCRasterDrawLine(self, currentPoint.x, currentPoint.y, curve.p.x, curve.p.y, color, color);
        }
        else if (HCContourCurveIsQuadratic(p0, curve)) {
            HCRasterDrawQuadraticCurve(self, currentPoint.x, currentPoint.y, curve.c0.x, curve.c0.y, curve.p.x, curve.p.y, color, color);
        }
        else if (HCContourCurveIsCubic(p0, curve)) {
            HCRasterDrawCubicCurve(self, currentPoint.x, currentPoint.y, curve.c0.x, curve.c0.y, curve.c1.x, curve.c1.y, curve.p.x, curve.p.y, color, color);
        }
        currentPoint = curve.p;
    }
    
    // Close the curve, if requested and required
    HCPoint endPoint = currentPoint;
    if (closed && !HCPointIsEqual(endPoint, startPoint)) {
        HCRasterDrawLine(self, endPoint.x, endPoint.y, startPoint.x, startPoint.y, color, color);
    }
}

void HCRasterDrawContour(HCRasterRef self, const HCContour* contour, HCColor color) {
    HCRasterDrawContourCurves(self, HCContourComponents(contour), HCContourComponentCount(contour), HCContourIsClosed(contour), color);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawPath(HCRasterRef self, HCPathRef path, HCColor color) {
    // Draw each element
    HCBoolean rotatingColor = HCColorIsEqual(color, HCRasterColorRotating);
    HCPoint startPoint = HCPointZero;
    HCPoint currentPoint = startPoint;
    HCPoint controlPoint0 = currentPoint;
    HCPoint controlPoint1 = currentPoint;
    HCPoint endPoint = currentPoint;
    for (HCInteger elementIndex = 0; elementIndex < HCPathElementCount(path); elementIndex++) {
        // When the rotating color is requested, change the color with each segment
        if (rotatingColor) {
            color = HCColorMake(1.0, 0.25 + fmod(color.r + 0.1, 0.75), 0.25 + fmod(color.g + 0.2, 0.75), 0.25 + fmod(color.b + 0.3, 0.75));
        }
        
        // Draw the element
        HCPathElement element = HCPathElementAt(path, elementIndex);
        switch (element.command) {
            case HCPathCommandMove:
                endPoint = HCPointMake(element.points[0].x, element.points[0].y);
                currentPoint = endPoint;
                startPoint = currentPoint;
                break;
            case HCPathCommandAddLine:
                endPoint = HCPointMake(element.points[0].x, element.points[0].y);
                HCRasterDrawLine(self, currentPoint.x, currentPoint.y, endPoint.x, endPoint.y, color, color);
                currentPoint = endPoint;
                break;
            case HCPathCommandAddQuadraticCurve:
                controlPoint0 = HCPointMake(element.points[0].x, element.points[0].y);
                endPoint = HCPointMake(element.points[1].x, element.points[1].y);
                HCRasterDrawQuadraticCurve(self, currentPoint.x, currentPoint.y, controlPoint0.x, controlPoint0.y, endPoint.x, endPoint.y, color, color);
                currentPoint = endPoint;
                break;
            case HCPathCommandAddCubicCurve:
                controlPoint0 = HCPointMake(element.points[0].x, element.points[0].y);
                controlPoint1 = HCPointMake(element.points[1].x, element.points[1].y);
                endPoint = HCPointMake(element.points[2].x, element.points[2].y);
                HCRasterDrawCubicCurve(self, currentPoint.x, currentPoint.y, controlPoint0.x, controlPoint0.y, controlPoint1.x, controlPoint1.y, endPoint.x, endPoint.y, color, color);
                currentPoint = endPoint;
                break;
            case HCPathCommandCloseContour:
                endPoint = startPoint;
                HCRasterDrawLine(self, currentPoint.x, currentPoint.y, endPoint.x, endPoint.y, color, color);
                currentPoint = endPoint;
                break;
        }
    }
}

void HCRasterDrawPathData(HCRasterRef self, const char* pathData, HCColor color) {
    HCPathRef path = HCPathCreateWithSVGPathData(pathData);
    HCRasterDrawPath(self, path, color);
    HCRelease(path);
}
