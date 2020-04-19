//
//  HCRaster+Draw.c
//  HollowCore
//
//  Created by Matt Stoker on 4/18/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#include "HCRaster+Draw.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Curve Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawPoint(HCRasterRef self, HCReal x, HCReal y, HCColor color) {
    HCRasterSetPixelAt(self, round(x), round(y), color);
}

void HCRasterDrawLine(HCRasterRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCColor c0, HCColor c1) {
    // Draw using direct evaluation informed by derivitive
    HCReal dx = x1 - x0;
    HCReal dy = y1 - y0;
    for (HCReal t = 0.0; t <= 1.0;) {
        HCReal tc = 1.0 - t;
        HCReal a = tc;
        HCReal b = t;
        HCReal x = a * x0 + b * x1;
        HCReal y = a * y0 + b * y1;
        
        HCRasterDrawPoint(self, x, y, HCColorCombine(c0, c1, t));
        t += fmax(0.00001, 1.00000 / fmax(fabs(dx), fabs(dy)));
    }
    
    // Draw using Bresenham's Algorithm
//    HCReal dx = x1 - x0;
//    HCReal dy = y1 - y0;
//    HCReal step = fmaxf(fabs(dx), fabs(dy));
//    dx = dx / step;
//    dy = dy / step;
//    HCReal x = x0;
//    HCReal y = y0;
//    HCInteger i = 1;
//    do {
//        HCRasterSetPixelAt(self, round(x), round(y), c0);
//        x = x + dx;
//        y = y + dy;
//        i++;
//    } while (i <= step);
    
//    int ix0 = round(x0);
//    int iy0 = round(y0);
//    int ix1 = round(x1);
//    int iy1 = round(y1);
//    int dx = abs(ix1-ix0), sx = ix0<ix1 ? 1 : -1;
//    int dy = abs(iy1-iy0), sy = iy0<iy1 ? 1 : -1;
//    int err = (dx>dy ? dx : -dy)/2, e2;
//
//    while (true) {
//        HCRasterSetPixelAt(self, ix0, iy0, c0);
//        if (ix0==ix1 && iy0==iy1) break;
//        e2 = err;
//        if (e2 >-dx) { err -= dy; ix0 += sx; }
//        if (e2 < dy) { err += dx; iy0 += sy; }
//    }

    // Draw using Xiaolin Wu's Algorithm
//#define fswap(x0, x1) { HCReal t = x0; x0 = x1; x1 = t; }
//#define fpart(x) (x - floor(x))
//#define rfpart(x) (1 - fpart(x))
//
//    HCBoolean steep = fabs(y1 - y0) > fabs(x1 - x0);
//
//    if (steep) {
//        fswap(x0, y0);
//        fswap(x1, y1);
//    }
//    if (x0 > x1) {
//        fswap(x0, x1);
//        fswap(y0, y1);
//    }
//
//    HCReal dx = x1 - x0;
//    HCReal dy = y1 - y0;
//    HCReal gradient = dy / dx;
//    if (dx == 0.0) {
//        gradient = 1.0;
//    }
//
//    // Handle first endpoint
//    HCReal xend = round(x0);
//    HCReal yend = y0 + gradient * (xend - x0);
//    HCReal xgap = rfpart(x0 + 0.5);
//    HCReal xpxl1 = xend;
//    HCReal ypxl1 = floor(yend);
//    if (steep) {
//        HCRasterSetPixelAt(self, ypxl1,   xpxl1, HCColorCombine(HCColorBlack, c1, rfpart(yend) * xgap));
//        HCRasterSetPixelAt(self, ypxl1+1, xpxl1, HCColorCombine(HCColorBlack, c1,  fpart(yend) * xgap));
//    }
//    else {
//        HCRasterSetPixelAt(self, xpxl1, ypxl1  , HCColorCombine(HCColorBlack, c1, rfpart(yend) * xgap));
//        HCRasterSetPixelAt(self, xpxl1, ypxl1+1, HCColorCombine(HCColorBlack, c1,  fpart(yend) * xgap));
//    }
//    HCReal intery = yend + gradient;
//
//    // Handle second endpoint
//    xend = round(x1);
//    yend = y1 + gradient * (xend - x1);
//    xgap = fpart(x1 + 0.5);
//    HCReal xpxl2 = xend; //this will be used in the main loop
//    HCReal ypxl2 = floor(yend);
//    if (steep) {
//        HCRasterSetPixelAt(self, ypxl2  , xpxl2, HCColorCombine(HCColorBlack, c1, rfpart(yend) * xgap));
//        HCRasterSetPixelAt(self, ypxl2+1, xpxl2, HCColorCombine(HCColorBlack, c1,  fpart(yend) * xgap));
//    }
//    else {
//        HCRasterSetPixelAt(self, xpxl2, ypxl2,   HCColorCombine(HCColorBlack, c1, rfpart(yend) * xgap));
//        HCRasterSetPixelAt(self, xpxl2, ypxl2+1, HCColorCombine(HCColorBlack, c1,  fpart(yend) * xgap));
//    }
//
//    // Main loop
//    if (steep) {
//        for (HCReal x = xpxl1 + 1.0; x < xpxl2 - 1.0; x += 1.0) {
//            HCRasterSetPixelAt(self, floor(intery)  , x, HCColorCombine(HCColorBlack, c1, rfpart(intery)));
//            HCRasterSetPixelAt(self, floor(intery)+1, x, HCColorCombine(HCColorBlack, c1,  fpart(intery)));
//            intery = intery + gradient;
//        }
//    }
//    else {
//        for (HCReal x = xpxl1 + 1.0; x < xpxl2 - 1.0; x += 1.0) {
//            HCRasterSetPixelAt(self, x, floor(intery),   HCColorCombine(HCColorBlack, c1, rfpart(intery)));
//            HCRasterSetPixelAt(self, x, floor(intery)+1, HCColorCombine(HCColorBlack, c1,  fpart(intery)));
//            intery = intery + gradient;
//        }
//    }
}

void HCRasterDrawQuadraticCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCColor c0, HCColor c1) {
    // Draw using direct evaluation informed by derivitive
//    HCReal x, y, dx, dy;
//    for (HCReal t = 0.0; t <= 1.0;) {
//        HCRasterEvaluateQuadraticCurve(t, x0, y0, cx, cy, x1, y1, &x, &y, &dx, &dy);
//        HCRasterDrawPoint(self, x, y, HCColorCombine(c0, c1, t));
//        t += fmax(0.00001, 0.50000 / fmax(fabs(dx), fabs(dy)));
//    }
    
    // Draw using De Casteljau's Algorithm
    HCReal flatness =
       (sqrt((cx - x0) * (cx - x0) + (cy - y0) * (cy - y0)) +
        sqrt((x1 - cx) * (x1 - cx) + (y1 - cy) * (y1 - cy))) /
        sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    if (flatness < 1.0001) {
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
    // Draw using direct evaluation informed by derivitive
//    HCReal x, y, dx, dy;
//    for (HCReal t = 0.0; t <= 1.0;) {
//        HCRasterEvaluateCubicCurve(t, x0, y0, cx0, cy0, cx1, cy1, x1, y1, &x, &y, &dx, &dy);
//        HCRasterDrawPoint(self, x, y, HCColorCombine(c0, c1, t));
//        t += fmax(0.00001, 0.33333 / fmax(fabs(dx), fabs(dy)));
//    }
    
    // Draw using De Casteljau's Algorithm
    HCReal flatness =
       (sqrt((cx0 -  x0) * (cx0 -  x0) + (cy0 -  y0) * (cy0 -  y0)) +
        sqrt((cx1 - cx0) * (cx1 - cx0) + (cy1 - cy0) * (cy1 - cy0)) +
        sqrt(( x1 - cx1) * ( x1 - cx1) + ( y1 - cy1) * ( y1 - cy1))) /
        sqrt(( x1 -  x0) * ( x1 -  x0) + ( y1 -  y0) * ( y1 -  y0));
    if (flatness < 1.0001) {
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
                HCRasterSetPixelAt(self, xIndex, yIndex, color);
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
                HCRasterSetPixelAt(self, xIndex, yIndex, color);
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
