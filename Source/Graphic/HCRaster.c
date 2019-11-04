//
//  HCRaster.c
//  Test
//
//  Created by Matt Stoker on 2/3/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCRaster_Internal.h"
#include <math.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCRasterTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCRaster",
    },
    .isEqual = (void*)HCRasterIsEqual,
    .hashValue = (void*)HCRasterHashValue,
    .print = (void*)HCRasterPrint,
    .destroy = (void*)HCRasterDestroy,
};
HCType HCRasterType = (HCType)&HCRasterTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCRasterSizeMax = 0x2FFFFFFF;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterRef HCRasterCreate(HCInteger width, HCInteger height) {
    if (width * height > HCRasterSizeMax) {
        return NULL;
    }
    
    HCRasterRef self = calloc(sizeof(HCRaster), 1);
    HCRasterInit(self, width, height);
    return self;
}

void HCRasterInit(void* memory,  HCInteger width, HCInteger height) {
    HCObjectInit(memory);
    HCRasterRef self = memory;
    self->base.type = HCRasterType;
    self->width = width;
    self->height = height;
    self->pixels = calloc(sizeof(HCColor), width * height);
}

void HCRasterDestroy(HCRasterRef self) {
    free(self->pixels);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRasterIsEqual(HCRasterRef self, HCRasterRef other) {
    if (self->width != other->width || self->height != other->height) {
        return false;
    }
    //return memcmp(self->pixels, other->pixels, sizeof(HCColor) * self->width * self->height) == 0;
    for (HCInteger yIndex = 0; yIndex < self->height; yIndex++) {
        for (HCInteger xIndex = 0; xIndex < self->width; xIndex++) {
            HCColor c = HCRasterPixelAt(self, xIndex, yIndex);
            HCColor o = HCRasterPixelAt(other, xIndex, yIndex);
            if (c.a != o.a || c.r != o.r || c.g != o.g || c.b != o.b) {
                return false;
            }
        }
    }
    return true;
}

HCInteger HCRasterHashValue(HCRasterRef self) {
    HCInteger hash = 5381;
    for (HCInteger yIndex = 0; yIndex < self->height; yIndex++) {
        for (HCInteger xIndex = 0; xIndex < self->width; xIndex++) {
            HCColor pixel = HCRasterPixelAt(self, xIndex, yIndex);
            // TODO: Better hash
            hash = ((hash << 5) + hash) + pixel.a;
            hash = ((hash << 5) + hash) + pixel.r;
            hash = ((hash << 5) + hash) + pixel.g;
            hash = ((hash << 5) + hash) + pixel.b;
        }
    }
    return hash;
}

void HCRasterPrint(HCRasterRef self, FILE* stream) {
    fprintf(stream, "<%s@%p,size:%lix%li>", self->base.type->name, self, (long)self->width, (long)self->height);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCRasterWidth(HCRasterRef self) {
    return self->width;
}

HCInteger HCRasterHeight(HCRasterRef self) {
    return self->height;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Pixel Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCColor HCRasterPixelAt(HCRasterRef self, HCInteger xIndex, HCInteger yIndex) {
    if (xIndex >= 0 && yIndex >= 0 && xIndex < self->width && yIndex < self->height) {
        return self->pixels[yIndex * self->width + xIndex];
    }
    return HCColorInvalid;
}

void HCRasterSetPixelAt(HCRasterRef self, HCInteger xIndex, HCInteger yIndex, HCColor pixel) {
    if (xIndex >= 0 && yIndex >= 0 && xIndex < self->width && yIndex < self->height) {
        self->pixels[yIndex * self->width + xIndex] = pixel;
    }
}

void HCRasterPixelsAt(HCRasterRef self, HCInteger startXIndex, HCInteger startYIndex, HCInteger endXIndex, HCInteger endYIndex, HCColor* pixels) {
    for (HCInteger yIndex = startYIndex; yIndex < endYIndex; yIndex++) {
        for (HCInteger xIndex = startXIndex; xIndex < endXIndex; xIndex++) {
            *pixels++ = HCRasterPixelAt(self, xIndex, yIndex);
        }
    }
}

void HCRasterSetPixelsAt(HCRasterRef self, HCInteger startXIndex, HCInteger startYIndex, HCInteger endXIndex, HCInteger endYIndex, const HCColor* pixels) {
    for (HCInteger yIndex = startYIndex; yIndex < endYIndex; yIndex++) {
        for (HCInteger xIndex = startXIndex; xIndex < endXIndex; xIndex++) {
            HCColor pixel = *pixels++;
            HCRasterSetPixelAt(self, xIndex, yIndex, pixel);
        }
    }
}

void HCRasterClear(HCRasterRef self) {
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(self); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(self); xIndex++) {
            HCRasterSetPixelAt(self, xIndex, yIndex, HCColorClear);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Lookup Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCColor HCRasterPixelNearest(HCRasterRef self, HCReal x, HCReal y) {
    HCInteger xIndex = fmax(0.0, fmin(self->width - 1.0, round(x)));
    HCInteger yIndex = fmax(0.0, fmin(self->height - 1.0, round(y)));
    return HCRasterPixelAt(self, xIndex, yIndex);
}

HCColor HCRasterPixelFiltered(HCRasterRef self, HCReal x, HCReal y) {
    HCInteger xIndex = floor(x - 0.5);
    HCInteger yIndex = floor(y - 0.5);
    HCReal wp = (x - xIndex) - 0.5;
    HCReal hp = (y - yIndex) - 0.5;
    HCReal w = 1.0 - wp;
    HCReal h = 1.0 - hp;
    HCReal a = w*h;
    HCReal b = wp*h;
    HCReal c = w*hp;
    HCReal d = wp*hp;
    HCInteger xmin = 0;
    HCInteger xmax = HCRasterWidth(self) - 1;
    HCInteger ymin = 0;
    HCInteger ymax = HCRasterHeight(self) - 1;
    HCInteger ax = xIndex + 0; ax = ax < xmin ? xmin : ax; ax = ax > xmax ? xmax : ax;
    HCInteger ay = yIndex + 0; ay = ay < ymin ? ymin : ay; ay = ay > ymax ? ymax : ay;
    HCInteger bx = xIndex + 1; bx = bx < xmin ? xmin : bx; bx = bx > xmax ? xmax : bx;
    HCInteger by = yIndex + 0; by = by < ymin ? ymin : by; by = by > ymax ? ymax : by;
    HCInteger cx = xIndex + 0; cx = cx < xmin ? xmin : cx; cx = cx > xmax ? xmax : cx;
    HCInteger cy = yIndex + 1; cy = cy < ymin ? ymin : cy; cy = cy > ymax ? ymax : cy;
    HCInteger dx = xIndex + 1; dx = dx < xmin ? xmin : dx; dx = dx > xmax ? xmax : dx;
    HCInteger dy = yIndex + 1; dy = dy < ymin ? ymin : dy; dy = dy > ymax ? ymax : dy;
    HCColor ca = HCRasterPixelAt(self, ax, ay);
    HCColor cb = HCRasterPixelAt(self, bx, by);
    HCColor cc = HCRasterPixelAt(self, cx, cy);
    HCColor cd = HCRasterPixelAt(self, dx, dy);
    return HCColorCombine4(ca, a, cb, b, cc, c, cd, d);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Curve Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawPoint(HCRasterRef self, HCReal x, HCReal y, HCColor color) {
    HCRasterSetPixelAt(self, round(x), round(y), color);
}

void HCRasterDrawLine(HCRasterRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCColor c0, HCColor c1) {
    // TODO: Use a non-sampling draw algorithm
    for (HCReal t = 0.0; t <= 1.0; t += 0.01) {
        HCReal tc = 1.0 - t;
        
//        HCReal x = x0 * tc + x1 * t;
//        HCReal y = y0 * tc + y1 * t;
        
        HCReal a = tc;
        HCReal b = t;
        HCReal x = a * x0 + b * x1;
        HCReal y = a * y0 + b * y1;
        
        HCRasterSetPixelAt(self, round(x), round(y), HCColorCombine(c0, c1, t));
    }
}

void HCRasterDrawQuadraticCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCColor c0, HCColor c1) {
    // TODO: Use a non-sampling draw algorithm
    for (HCReal t = 0.0; t <= 1.0; t += 0.01) {
        HCReal tc = 1.0 - t;
        
//        HCReal sx0 = x0 * tc + cx * t;
//        HCReal sy0 = y0 * tc + cy * t;
//        HCReal sx1 = cx * tc + x1 * t;
//        HCReal sy1 = cy * tc + y1 * t;
//        HCReal x = sx0 * tc + sx1 * t;
//        HCReal y = sy0 * tc + sy1 * t;
        
//        HCReal x = (x0 * tc + cx * t) * tc + (cx * tc + x1 * t) * t;
//        HCReal y = (y0 * tc + cy * t) * tc + (cy * tc + y1 * t) * t;

//        HCReal x = x0 * tc * tc + cx * t * tc + cx * tc * t + x1 * t * t;
//        HCReal y = y0 * tc * tc + cy * t * tc + cy * tc * t + y1 * t * t;

//        HCReal t2 = t * t;
//        HCReal tc2 = tc * tc;
//        HCReal x = x0 * tc2 + cx * t * tc + cx * tc * t + x1 * t2;
//        HCReal y = y0 * tc2 + cy * t * tc + cy * tc * t + y1 * t2;
        
//        HCReal t2 = t * t;
//        HCReal tc2 = tc * tc;
//        HCReal x = x0 * tc2 + (2.0 * cx) * t * tc + x1 * t2;
//        HCReal y = y0 * tc2 + (2.0 * cy) * t * tc + y1 * t2;
        
        HCReal a = tc * tc;
        HCReal b = 2.0 * t * tc;
        HCReal c = t * t;
        HCReal x = a * x0 + b * cx + c * x1;
        HCReal y = a * y0 + b * cy + c * y1;
        
        HCRasterSetPixelAt(self, round(x), round(y), HCColorCombine(c0, c1, t));
    }
}

void HCRasterDrawCubicCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCColor c0, HCColor c1) {
    // TODO: Use a non-sampling draw algorithm
    for (HCReal t = 0.0; t <= 1.0; t += 0.01) {
        HCReal tc = 1.0 - t;
        
//        HCReal sx0 = x0 * tc + cx0 * t;
//        HCReal sy0 = y0 * tc + cy0 * t;
//        HCReal scx = cx0 * tc + cx1 * t;
//        HCReal scy = cy0 * tc + cy1 * t;
//        HCReal sx1 = cx1 * tc + x1 * t;
//        HCReal sy1 = cy1 * tc + y1 * t;
//        HCReal ssx0 = sx0 * tc + scx * t;
//        HCReal ssy0 = sy0 * tc + scy * t;
//        HCReal ssx1 = scx * tc + sx1 * t;
//        HCReal ssy1 = scy * tc + sy1 * t;
//        HCReal x = ssx0 * tc + ssx1 * t;
//        HCReal y = ssy0 * tc + ssy1 * t;

//        HCReal ssx0 = (x0 * tc + cx0 * t) * tc + (cx0 * tc + cx1 * t) * t;
//        HCReal ssy0 = (y0 * tc + cy0 * t) * tc + (cy0 * tc + cy1 * t) * t;
//        HCReal ssx1 = (cx0 * tc + cx1 * t) * tc + (cx1 * tc + x1 * t) * t;
//        HCReal ssy1 = (cy0 * tc + cy1 * t) * tc + (cy1 * tc + y1 * t) * t;
//        HCReal x = ssx0 * tc + ssx1 * t;
//        HCReal y = ssy0 * tc + ssy1 * t;

//        HCReal x = ((x0 * tc + cx0 * t) * tc + (cx0 * tc + cx1 * t) * t) * tc + ((cx0 * tc + cx1 * t) * tc + (cx1 * tc + x1 * t) * t) * t;
//        HCReal y = ((y0 * tc + cy0 * t) * tc + (cy0 * tc + cy1 * t) * t) * tc + ((cy0 * tc + cy1 * t) * tc + (cy1 * tc + y1 * t) * t) * t;

//        HCReal x = (x0 * tc + cx0 * t) * tc * tc + (cx0 * tc + cx1 * t) * t * tc + (cx0 * tc + cx1 * t) * t * tc + (cx1 * tc + x1 * t) * t * t;
//        HCReal y = (y0 * tc + cy0 * t) * tc * tc + (cy0 * tc + cy1 * t) * t * tc + (cy0 * tc + cy1 * t) * t * tc + (cy1 * tc + y1 * t) * t * t;

//        HCReal x = x0 * tc * tc * tc + cx0 * tc * tc * t + cx0 * t * tc * tc + cx1 * t * t * tc + cx0 * t * tc * tc + cx1 * t * t * tc + cx1 * t * t * tc + x1 * t * t * t;
//        HCReal y = y0 * tc * tc * tc + cy0 * tc * tc * t + cy0 * t * tc * tc + cy1 * t * t * tc + cy0 * t * tc * tc + cy1 * t * t * tc + cy1 * t * t * tc + y1 * t * t * t;

//        HCReal t2 = t * t;
//        HCReal t3 = t * t * t;
//        HCReal tc2 = tc * tc;
//        HCReal tc3 = tc * tc * tc;
//        HCReal x = x0 * tc3 + cx0 * t * tc2 + cx0 * t * tc2 + cx1 * t2 * tc + cx0 * t * tc2 + cx1 * t2 * tc + cx1 * t2 * tc + x1 * t3;
//        HCReal y = y0 * tc3 + cy0 * t * tc2 + cy0 * t * tc2 + cy1 * t2 * tc + cy0 * t * tc2 + cy1 * t2 * tc + cy1 * t2 * tc + y1 * t3;

//        HCReal t2 = t * t;
//        HCReal t3 = t * t * t;
//        HCReal tc2 = tc * tc;
//        HCReal tc3 = tc * tc * tc;
//        HCReal x = x0 * tc3 + (3.0 * cx0) * t * tc2 + (3.0 * cx1) * t2 * tc + x1 * t3;
//        HCReal y = y0 * tc3 + (3.0 * cy0) * t * tc2 + (3.0 * cy1) * t2 * tc + y1 * t3;
        
        HCReal a = tc * tc * tc;
        HCReal b = 3.0 * t * tc * tc;
        HCReal c = 3.0 * t * t * tc;
        HCReal d = t * t * t;
        HCReal x = a * x0 + b * cx0 + c * cx1 + d * x1;
        HCReal y = a * y0 + b * cy0 + c * cy1 + d * y1;
        
        HCRasterSetPixelAt(self, round(x), round(y), HCColorCombine(c0, c1, t));
    }
}

void HCRasterDrawArc(HCRasterRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCReal xr, HCReal yr, HCReal theta, HCBoolean largeArc, HCBoolean sweep, HCColor c0, HCColor c1) {
    // TODO: Draw using 2-point radius dual ellipse intersection calculation
}

void HCRasterDrawPath(HCRasterRef self, const char* path, HCColor color) {
    // Define state data for parsing the STL path string
    char type = '\0';
    HCReal currentX = 0.0;
    HCReal currentY = 0.0;
    HCReal startX = currentX;
    HCReal startY = currentY;
    HCReal previousControlX = currentX;
    HCReal previousControlY = currentY;
    HCInteger argumentsExpected = 0;
    HCInteger argumentsCount = 0;
    HCReal arguments[10];
    
    // Parse path data according to STL path element data string format
    const char* current = path;
    while (*current != '\0') {
        char c = *current;
        if (c == ' ' || c == '\n' || c == '\r') {
            // Skip whitespace (though it is required to separate numbers)
            current++;
        }
        else if (type == '\0') {
            // Determine path sub-component type and, if valid, setup state to parse arguments
            switch (*current) {
                case 'M': type = 'M'; argumentsCount = 0; argumentsExpected = 2; break;
                case 'm': type = 'm'; argumentsCount = 0; argumentsExpected = 2; break;
                case 'L': type = 'L'; argumentsCount = 0; argumentsExpected = 2; break;
                case 'l': type = 'l'; argumentsCount = 0; argumentsExpected = 2; break;
                case 'H': type = 'H'; argumentsCount = 0; argumentsExpected = 1; break;
                case 'h': type = 'h'; argumentsCount = 0; argumentsExpected = 1; break;
                case 'V': type = 'V'; argumentsCount = 0; argumentsExpected = 1; break;
                case 'v': type = 'v'; argumentsCount = 0; argumentsExpected = 1; break;
                case 'Q': type = 'Q'; argumentsCount = 0; argumentsExpected = 4; break;
                case 'q': type = 'q'; argumentsCount = 0; argumentsExpected = 4; break;
                case 'T': type = 'T'; argumentsCount = 0; argumentsExpected = 2; break;
                case 't': type = 't'; argumentsCount = 0; argumentsExpected = 2; break;
                case 'C': type = 'C'; argumentsCount = 0; argumentsExpected = 6; break;
                case 'c': type = 'c'; argumentsCount = 0; argumentsExpected = 6; break;
                case 'S': type = 'S'; argumentsCount = 0; argumentsExpected = 4; break;
                case 's': type = 's'; argumentsCount = 0; argumentsExpected = 4; break;
                case 'A': type = 'A'; argumentsCount = 0; argumentsExpected = 7; break;
                case 'a': type = 'a'; argumentsCount = 0; argumentsExpected = 7; break;
                case 'Z': // Fallthrough
                case 'z':
                    // Close path has no arguments, so just perform the action and move on
                    HCRasterDrawLine(self, currentX, currentY, startX, startY, color, color);
                    currentX = startX;
                    currentY = startY;
                    break;
            }
            
            // Move to next character
            current++;
        }
        else {
            // Parse an argument number
            char* end = (char*)current;
            double argument = strtod(current, &end);
            if (end <= current) {
                // Invalid argument, ignore it and dump the current path sub-component
                type = '\0';
                current++;
            }
            else {
                // Save the argument and advance to the character after the argument
                arguments[argumentsCount] = argument;
                argumentsCount++;
                current = end;
                
                // When enough arguments have been parsed, execute the draw command specified
                if (argumentsExpected > 0 && argumentsCount == argumentsExpected) {
                    // Execute the draw command
                    switch (type) {
                        case 'm':
                            arguments[0] += currentX;
                            arguments[1] += currentY;
                            // Fallthrough
                        case 'M': {
                            startX = arguments[0];
                            startY = arguments[1];
                            currentX = startX;
                            currentY = startY;
                            previousControlX = currentX;
                            previousControlY = currentY;
                        } break;
                        case 'l':
                            arguments[0] += currentX;
                            arguments[1] += currentY;
                            // Fallthrough
                        case 'L': {
                            HCReal x0 = currentX;
                            HCReal y0 = currentY;
                            HCReal x1 = arguments[0];
                            HCReal y1 = arguments[1];
                            currentX = x1;
                            currentY = y1;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCRasterDrawLine(self, x0, y0, x1, y1, color, color);
                        } break;
                        case 'h':
                            arguments[0] += currentX;
                            // Fallthrough
                        case 'H': {
                            HCReal x0 = currentX;
                            HCReal y0 = currentY;
                            HCReal x1 = arguments[0];
                            HCReal y1 = currentY;
                            currentX = x1;
                            currentY = y1;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCRasterDrawLine(self, x0, y0, x1, y1, color, color);
                        } break;
                        case 'v':
                            arguments[0] += currentY;
                            // Fallthrough
                        case 'V': {
                            HCReal x0 = currentX;
                            HCReal y0 = currentY;
                            HCReal x1 = arguments[0];
                            HCReal y1 = currentY;
                            currentX = x1;
                            currentY = y1;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCRasterDrawLine(self, x0, y0, x1, y1, color, color);
                            currentY = arguments[0];
                        } break;
                        case 'q':
                            arguments[0] += currentX;
                            arguments[1] += currentY;
                            arguments[2] += currentX;
                            arguments[3] += currentY;
                            // Fallthrough
                        case 'Q': {
                            HCReal x0 = currentX;
                            HCReal y0 = currentY;
                            HCReal cx = arguments[0];
                            HCReal cy = arguments[1];
                            HCReal x1 = arguments[2];
                            HCReal y1 = arguments[3];
                            currentX = x1;
                            currentY = y1;
                            previousControlX = cx;
                            previousControlY = cy;
                            HCRasterDrawQuadraticCurve(self, x0, y0, cx, cy, x1, y1, color, color);
                        } break;
                        case 't':
                            arguments[0] += currentX;
                            arguments[1] += currentY;
                            // Fallthrough
                        case 'T': {
                            HCReal x0 = currentX;
                            HCReal y0 = currentY;
                            HCReal cx = 2.0 * x0 - previousControlX;
                            HCReal cy = 2.0 * y0 - previousControlY;
                            HCReal x1 = arguments[0];
                            HCReal y1 = arguments[1];
                            currentX = x1;
                            currentY = y1;
                            previousControlX = cx;
                            previousControlY = cy;
                            HCRasterDrawQuadraticCurve(self, x0, y0, cx, cy, x1, y1, color, color);
                        } break;
                        case 'c':
                            arguments[0] += currentX;
                            arguments[1] += currentY;
                            arguments[2] += currentX;
                            arguments[3] += currentY;
                            arguments[4] += currentX;
                            arguments[5] += currentY;
                            // Fallthrough
                        case 'C': {
                            HCReal x0 = currentX;
                            HCReal y0 = currentY;
                            HCReal c0x = arguments[0];
                            HCReal c0y = arguments[1];
                            HCReal c1x = arguments[2];
                            HCReal c1y = arguments[3];
                            HCReal x1 = arguments[4];
                            HCReal y1 = arguments[5];
                            currentX = x1;
                            currentY = y1;
                            previousControlX = c1x;
                            previousControlY = c1y;
                            HCRasterDrawCubicCurve(self, x0, y0, c0x, c0y, c1x, c1y, x1, y1, color, color);
                        } break;
                        case 's':
                            arguments[0] += currentX;
                            arguments[1] += currentY;
                            arguments[2] += currentX;
                            arguments[3] += currentY;
                            // Fallthrough
                        case 'S': {
                            HCReal x0 = currentX;
                            HCReal y0 = currentY;
                            HCReal c0x = 2.0 * x0 - previousControlX;
                            HCReal c0y = 2.0 * y0 - previousControlY;
                            HCReal c1x = arguments[0];
                            HCReal c1y = arguments[1];
                            HCReal x1 = arguments[2];
                            HCReal y1 = arguments[3];
                            currentX = x1;
                            currentY = y1;
                            previousControlX = c1x;
                            previousControlY = c1y;
                            HCRasterDrawCubicCurve(self, x0, y0, c0x, c0y, c1x, c1y, x1, y1, color, color);
                        } break;
                        case 'a':
                            arguments[5] += currentX;
                            arguments[6] += currentY;
                            // Fallthrough
                        case 'A': {
                            HCReal x0 = currentX;
                            HCReal y0 = currentY;
                            HCReal xr = arguments[0];
                            HCReal yr = arguments[1];
                            HCReal rotation = arguments[2] / 180.0 * M_PI;
                            HCBoolean largeArc = arguments[3] == 0.0 ? false : true;
                            HCBoolean sweep = arguments[4] == 0.0 ? false : true;
                            HCReal x1 = arguments[5];
                            HCReal y1 = arguments[6];
                            currentX = x1;
                            currentY = y1;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCRasterDrawArc(self, x0, y0, x1, y1, xr, yr, rotation, largeArc, sweep, color, color);
                        } break;
                    }
                    
                    // Reset parsing state to look for a new sub-component type
                    argumentsExpected = 0;
                    argumentsCount = 0;
                    type = '\0';
                }
            }
        }
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
// MARK: - Data Extraction
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterExtractARGB8888(HCRasterRef self, uint32_t* pixels) {
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(self); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(self); xIndex++) {
            HCColor pixel = HCRasterPixelAt(self, xIndex, yIndex);
            *pixels = HCColorAsARGB8888(pixel);
            pixels++;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - File Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterRef HCRasterCreateByLoadingPPM(const char* path) {
    // Open a file for reading
    FILE* f = fopen(path, "r");
    if (f == NULL)
        return NULL;
    
    // Verify file magic number
    int magic0 = fgetc(f);
    int magic1 = fgetc(f);
    if (magic0 != 'P' || (magic1 != '3' && magic1 != '6')) {
        return NULL;
    }
    
    // Read and validate the size and channel scale from the header and consume the newline sequence
    int width = 0;
    int height = 0;
    int channelMax = 0;
    if (fscanf(f, "%i %i %i", &width, &height, &channelMax) != 3 || width <= 0 || height <= 0 || channelMax <= 0 || channelMax > 255) {
        return NULL;
    }
    if (fgetc(f) == '\r') {
        char c = fgetc(f);
        if (c != '\n') {
            ungetc(c, f);
        }
    }
    HCBoolean binary = magic1 == '6';
    
    // Trash any comment lines
    if (!binary) {
        while (true) {
            // Read a comment start character or return it to the stream and continue
            int c = fgetc(f);
            if (c != '#') {
                ungetc(c, f);
                break;
            }
            
            // Read until a newline, accounting for possible \r\n sequences
            while (true) {
                c = fgetc(f);
                if (c == '\n' || c == EOF) {
                    break;
                }
                if (c == '\r') {
                    c = fgetc(f);
                    if (c != '\n') {
                        ungetc(c, f);
                    }
                    break;
                }
            }
        }
    }
    
    // Allocate raster
    HCRasterRef self = HCRasterCreate(width, height);
    if (self == NULL) {
        return NULL;
    }
    
    // Read pixel information
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(self); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(self); xIndex++) {
            int r = 0;
            int g = 0;
            int b = 0;
            if (!binary) {
                fscanf(f, "%i %i %i", &r, &g, &b);
            }
            else {
                fread(&r, 1, 1, f);
                fread(&g, 1, 1, f);
                fread(&b, 1, 1, f);
            }
            HCColor color = HCColorMake(1.0, (HCReal)r / (HCReal)channelMax, (HCReal)g / (HCReal)channelMax, (HCReal)b / (HCReal)channelMax);
            HCRasterSetPixelAt(self, xIndex, yIndex, color);
        }
    }
    fclose(f);
    
    return self;
}

void HCRasterSavePPM(HCRasterRef self, const char* path) {
    HCRasterSavePPMWithOptions(self, path, false);
}

void HCRasterSavePPMWithOptions(HCRasterRef self, const char* path, HCBoolean binary) {
    // Open file for writing
    FILE* file = fopen(path, "w");
    if (file == NULL)
        return;
    
    // Write PPM Header
    fprintf(file, "P%c %i %i 255\n", binary ? '6' : '3', (int)HCRasterWidth(self), (int)HCRasterHeight(self));
    
    // Write comment (where not saving binary)
    if (!binary) {
        fprintf(file, "# Created by HollowCore\n");
    }
    
    // Write PPM body, top row first, in red, green, blue byte order
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(self); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(self); xIndex++) {
            HCColor pixel = HCRasterPixelAt(self, xIndex, yIndex);
            HCByte r = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.r * 256.0f)));
            HCByte g = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.g * 256.0f)));
            HCByte b = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.b * 256.0f)));
            if (!binary) {
                fprintf(file, "%i %i %i ", (int)r, (int)g, (int)b);
            }
            else {
                fwrite(&r, sizeof(HCByte), 1, file);
                fwrite(&g, sizeof(HCByte), 1, file);
                fwrite(&b, sizeof(HCByte), 1, file);
            }
        }
        if (!binary) {
            fprintf(file, "\n");
        }
    }
    
    // Cleanup
    fclose(file);
}

HCRasterRef HCRasterCreateByLoadingBMP(const char* path) {
    // BMP (Windows V3)
    // Offset    Size    Description
    // 0         2       the magic number used to identify the BMP file: 0x42 0x4D (Hex code points for B and M in big-endian order)
    // 2         4       the size of the BMP file in bytes
    // 6         2       reserved; actual value depends on the application that creates the image
    // 8         2       reserved; actual value depends on the application that creates the image
    // 10        4       the offset, i.e. starting address, of the byte where the bitmap data can be found.
    // 14        4       the size of this header (40 bytes)
    // 18        4       the bitmap width in pixels (signed integer).
    // 22        4       the bitmap height in pixels (signed integer).
    // 26        2       the number of color planes being used. Must be set to 1.
    // 28        2       the number of bits per pixel, which is the color samplesPerPixel of the image. Typical values are 1, 4, 8, 16, 24 and 32.
    // 30        4       the compression method being used. See the next table for a list of possible values.
    // 34        4       the image size. This is the size of the raw bitmap data (see below), and should not be confused with the file size.
    // 38        4       the horizontal resolution of the image. (pixel per meter, signed integer)
    // 42        4       the vertical resolution of the image. (pixel per meter, signed integer)
    // 46        4       the number of colors in the color palette, or 0 to default to 2n.
    // 50        4       the number of important colors used, or 0 when every color is important; generally ignored.
    // 51        4*w*h   pixel data in XXRRGGBB uint32 format
    
    // Open file for reading
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        return NULL;
    }
    
    // Define header data
    uint16_t magicNumber = 0x4D42;
    uint32_t fileSize = 0;
    uint16_t reserved0 = 0;//0x4D41;
    uint16_t reserved1 = 0;//0x5454;
    uint32_t dataOffset = 54;
    uint32_t infoHeaderSize = 40;
    int32_t width = 0;
    int32_t height = 0;
    //uint16_t colorPlanes = 1;
    //uint16_t bitsPerPixel = 32;
    //uint32_t compression = 0;
    //uint32_t dataSize = width * height * bitsPerPixel / 8;
    //int32_t horizontalResolution = 2835;
    //int32_t verticalResolution = 2835;
    //uint32_t paletteColorCount = 0;
    //uint32_t importantPaletteColorCount = 0;
    
    // Read header data
    fread(&magicNumber, sizeof(uint16_t), 1, f);
    fread(&fileSize, sizeof(uint32_t), 1, f);
    fread(&reserved0, sizeof(uint16_t), 1, f);
    fread(&reserved1, sizeof(uint16_t), 1, f);
    fread(&dataOffset, sizeof(uint32_t), 1, f);
    fread(&infoHeaderSize, sizeof(uint32_t), 1, f);
    fread(&width, sizeof(int32_t), 1, f);
    fread(&height, sizeof(int32_t), 1, f);
    fseek(f, dataOffset, SEEK_SET);
    //fread(&colorPlanes, sizeof(uint16_t), 1, f);
    //fread(&bitsPerPixel, sizeof(uint16_t), 1, f);
    //fread(&compression, sizeof(uint32_t), 1, f);
    //fread(&dataSize, sizeof(uint32_t), 1, f);
    //fread(&horizontalResolution, sizeof(uint32_t), 1, f);
    //fread(&verticalResolution, sizeof(uint32_t), 1, f);
    //fread(&paletteColorCount, sizeof(uint32_t), 1, f);
    //fread(&importantPaletteColorCount, sizeof(uint32_t), 1, f);
    
    //TODO: Check header for sanity, support non-32bpp
    
    // Verify width and height
    HCBoolean reversed = height < 0;
    width = width < 0 ? -width : width;
    height = height < 0 ? -height : height;
    if (width <= 0 || height <= 0) {
        return NULL;
    }
    
    // Allocate raster
    HCRasterRef self = HCRasterCreate(width, height);
    if (self == NULL) {
        return NULL;
    }
    
    // Read BMP body (XXRRGGBB), respecting reversed boolean (bottom-to-top or top-to-bottom line ordering)
    for (HCInteger yIndex = reversed ? 0 : (HCRasterHeight(self) - 1); reversed ? (yIndex < HCRasterHeight(self)) : (yIndex >= 0); reversed ? yIndex++ : yIndex--) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(self); xIndex++) {
            uint32_t colorARGB = 0;
            fread(&colorARGB, sizeof(uint32_t), 1, f);
            HCColor color = HCColorMake(
                ((colorARGB & 0xFF000000) >> 24) / 255.0,
                ((colorARGB & 0x00FF0000) >> 16) / 255.0,
                ((colorARGB & 0x0000FF00) >>  8) / 255.0,
                ((colorARGB & 0x000000FF) >>  0) / 255.0);
            HCRasterSetPixelAt(self, xIndex, yIndex, color);
        }
    }
    
    // Cleanup
    fclose(f);
    
    return self;
}
void HCRasterSaveBMP(HCRasterRef self, const char* path) {
    HCRasterSaveBMPWithOptions(self, path, false);
}

void HCRasterSaveBMPWithOptions(HCRasterRef self, const char* path, HCBoolean reversed) {
    // BMP (Windows V3)
    // Offset    Size    Description
    // 0         2       the magic number used to identify the BMP file: 0x42 0x4D (Hex code points for B and M in big-endian order)
    // 2         4       the size of the BMP file in bytes
    // 6         2       reserved; actual value depends on the application that creates the image
    // 8         2       reserved; actual value depends on the application that creates the image
    // 10        4       the offset, i.e. starting address, of the byte where the bitmap data can be found.
    // 14        4       the size of this header (40 bytes)
    // 18        4       the bitmap width in pixels (signed integer).
    // 22        4       the bitmap height in pixels (signed integer).
    // 26        2       the number of color planes being used. Must be set to 1.
    // 28        2       the number of bits per pixel, which is the color samplesPerPixel of the image. Typical values are 1, 4, 8, 16, 24 and 32.
    // 30        4       the compression method being used. See the next table for a list of possible values.
    // 34        4       the image size. This is the size of the raw bitmap data (see below), and should not be confused with the file size.
    // 38        4       the horizontal resolution of the image. (pixel per meter, signed integer)
    // 42        4       the vertical resolution of the image. (pixel per meter, signed integer)
    // 46        4       the number of colors in the color palette, or 0 to default to 2n.
    // 50        4       the number of important colors used, or 0 when every color is important; generally ignored.
    // 51        4*w*h   pixel data in XXRRGGBB uint32 format
    
    // Open file for writing
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        return;
    }
    
    // Define header data, repecting reversed boolean in the height parameter
    uint16_t magicNumber = 0x4D42;
    uint16_t reserved0 = 0;//0x4D41;
    uint16_t reserved1 = 0;//0x5454;
    uint32_t dataOffset = 54;
    uint32_t infoHeaderSize = 40;
    int32_t width = (int32_t)HCRasterWidth(self);
    int32_t height = (int32_t)HCRasterHeight(self) * (reversed ? -1 : 1);
    uint16_t colorPlanes = 1;
    uint16_t bitsPerPixel = 32;
    uint32_t compression = 0;
    uint32_t dataSize = width * height * bitsPerPixel / 8;
    uint32_t horizontalResolution = 2835;
    uint32_t verticalResolution = 2835;
    uint32_t paletteColorCount = 0;
    uint32_t importantPaletteColorCount = 0;
    uint32_t fileSize = 54 + dataSize;
    
#ifdef __BIG_ENDIAN__
    // Change from big-endian to little-endian byte ordering
#define LE_32(data) (((data & 0x000000FF) << 24) | ((data & 0x0000FF00) << 8) | ((data & 0x00FF0000) >> 8) | ((data >> 24) && 0x000000FF))
#define LE_16(data) (((data & 0x00FF) << 8) | ((data >> 8) & 0x00FF))
#define PACK_PIXEL(alpha, red, green, blue) (0x00000000 | ((uint32_t)blue << 24) | ((uint32_t)green << 16) | ((uint32_t)red << 8) | ((uint32_t)alpha << 0))
    magicNumber = LE_16(magicNumber);
    reserved0 = LE_16(reserved0);
    reserved1 = LE_16(reserved1);
    dataOffset = LE_32(dataOffset);
    infoHeaderSize = LE_32(infoHeaderSize);
    width = LE_32(width);
    height = LE_32(height);
    colorPlanes = LE_16(colorPlanes);
    bitsPerPixel = LE_16(bitsPerPixel);
    compression = LE_32(compression);
    dataSize = LE_32(dataSize);
    horizontalResolution = LE_32(horizontalResolution);
    verticalResolution = LE_32(verticalResolution);
    paletteColorCount = LE_32(paletteColorCount);
    importantPaletteColorCount = LE_32(importantPaletteColorCount);
    fileSize = LE_32(fileSize);
#else
#define PACK_PIXEL(alpha, red, green, blue) (((uint32_t)alpha << 24)  | ((uint32_t)red << 16) | ((uint32_t)green << 8) | ((uint32_t)blue))
#endif //__BIG_ENDIAN__
    
    // Write BMP header (Windows V3, 32bbp)
    fwrite(&magicNumber, sizeof(magicNumber), 1, file);
    fwrite(&fileSize, sizeof(fileSize), 1, file);
    fwrite(&reserved0, sizeof(reserved0), 1, file);
    fwrite(&reserved1, sizeof(reserved1), 1, file);
    fwrite(&dataOffset, sizeof(dataOffset), 1, file);
    fwrite(&infoHeaderSize, sizeof(infoHeaderSize), 1, file);
    fwrite(&width, sizeof(width), 1, file);
    fwrite(&height, sizeof(height), 1, file);
    fwrite(&colorPlanes, sizeof(colorPlanes), 1, file);
    fwrite(&bitsPerPixel, sizeof(bitsPerPixel), 1, file);
    fwrite(&compression, sizeof(compression), 1, file);
    fwrite(&dataSize, sizeof(dataSize), 1, file);
    fwrite(&horizontalResolution, sizeof(horizontalResolution), 1, file);
    fwrite(&verticalResolution, sizeof(verticalResolution), 1, file);
    fwrite(&paletteColorCount, sizeof(paletteColorCount), 1, file);
    fwrite(&importantPaletteColorCount, sizeof(importantPaletteColorCount), 1, file);
    
    // Write BMP body (XXRRGGBB), respecting reversed boolean (bottom-to-top or top-to-bottom line ordering)
    for (HCInteger yIndex = reversed ? 0 : (HCRasterHeight(self) - 1); reversed ? (yIndex < HCRasterHeight(self)) : (yIndex >= 0); reversed ? yIndex++ : yIndex--) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(self); xIndex++) {
            HCColor pixel = HCRasterPixelAt(self, xIndex, yIndex);
            HCByte a = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.a * 256.0f)));
            HCByte r = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.r * 256.0f)));
            HCByte g = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.g * 256.0f)));
            HCByte b = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.b * 256.0f)));
            uint32_t p = PACK_PIXEL(a, r, g, b);
            fwrite(&p, sizeof(p), 1, file);
        }
    }
    
    // Cleanup
    fclose(file);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Color Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCColor HCColorCombine(HCColor c0, HCColor c1, HCReal t) {
    HCReal tc = 1.0 - t;
    HCReal a = tc * c0.a + t * c1.a;
    HCReal r = tc * c0.r + t * c1.r;
    HCReal g = tc * c0.g + t * c1.g;
    HCReal b = tc * c0.b + t * c1.b;
    return HCColorMake(a, r, g, b);
}

HCColor HCColorCombine3(HCColor ca, HCReal ta, HCColor cb, HCReal tb, HCColor cc, HCReal tc) {
    HCReal a = ta * ca.a + tb * cb.a + tc * cc.a;
    HCReal r = ta * ca.r + tb * cb.r + tc * cc.r;
    HCReal g = ta * ca.g + tb * cb.g + tc * cc.g;
    HCReal b = ta * ca.b + tb * cb.b + tc * cc.b;
    return HCColorMake(a, r, g, b);
}

HCColor HCColorCombine4(HCColor ca, HCReal ta, HCColor cb, HCReal tb, HCColor cc, HCReal tc, HCColor cd, HCReal td) {
    HCReal a = ta * ca.a + tb * cb.a + tc * cc.a + td * cd.a;
    HCReal r = ta * ca.r + tb * cb.r + tc * cc.r + td * cd.r;
    HCReal g = ta * ca.g + tb * cb.g + tc * cc.g + td * cd.g;
    HCReal b = ta * ca.b + tb * cb.b + tc * cc.b + td * cd.b;
    return HCColorMake(a, r, g, b);
}
