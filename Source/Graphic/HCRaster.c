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
HCRasterColor HCRasterColorInvalid  = { .a = NAN, .r = NAN, .g = NAN, .b = NAN };
HCRasterColor HCRasterColorClear    = { .a = 0.0, .r = 0.0, .g = 0.0, .b = 0.0 };
HCRasterColor HCRasterColorBlack    = { .a = 1.0, .r = 0.0, .g = 0.0, .b = 0.0 };
HCRasterColor HCRasterColorRed      = { .a = 1.0, .r = 1.0, .g = 0.0, .b = 0.0 };
HCRasterColor HCRasterColorGreen    = { .a = 1.0, .r = 0.0, .g = 1.0, .b = 0.0 };
HCRasterColor HCRasterColorBlue     = { .a = 1.0, .r = 0.0, .g = 0.0, .b = 1.0 };
HCRasterColor HCRasterColorYellow   = { .a = 1.0, .r = 1.0, .g = 1.0, .b = 0.0 };
HCRasterColor HCRasterColorCyan     = { .a = 1.0, .r = 0.0, .g = 1.0, .b = 1.0 };
HCRasterColor HCRasterColorMagenta  = { .a = 1.0, .r = 1.0, .g = 0.0, .b = 1.0 };
HCRasterColor HCRasterColorWhite    = { .a = 1.0, .r = 1.0, .g = 1.0, .b = 1.0 };
//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterRef HCRasterCreate(HCInteger width, HCInteger height) {
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
    self->pixels = calloc(sizeof(HCRasterColor), width * height);
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
    return memcmp(self->pixels, other->pixels, sizeof(HCRasterColor) * self->width * self->height) == 0;
}

HCInteger HCRasterHashValue(HCRasterRef self) {
    HCInteger hash = 5381;
    for (HCInteger yIndex = 0; yIndex < self->height; yIndex++) {
        for (HCInteger xIndex = 0; xIndex < self->width; xIndex++) {
            HCRasterColor pixel = HCRasterPixelAt(self, xIndex, yIndex);
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
HCRasterColor HCRasterPixelAt(HCRasterRef self, HCInteger xIndex, HCInteger yIndex) {
    if (xIndex >= 0 && yIndex >= 0 && xIndex < self->width && yIndex < self->height) {
        return self->pixels[yIndex * self->width + xIndex];
    }
    return HCRasterColorInvalid;
}

void HCRasterSetPixelAt(HCRasterRef self, HCInteger xIndex, HCInteger yIndex, HCRasterColor pixel) {
    if (xIndex >= 0 && yIndex >= 0 && xIndex < self->width && yIndex < self->height) {
        self->pixels[yIndex * self->width + xIndex] = pixel;
    }
}

void HCRasterPixelsAt(HCRasterRef self, HCInteger startXIndex, HCInteger startYIndex, HCInteger endXIndex, HCInteger endYIndex, HCRasterColor* pixels) {
    for (HCInteger yIndex = startYIndex; yIndex < endYIndex; yIndex++) {
        for (HCInteger xIndex = startXIndex; xIndex < endXIndex; xIndex++) {
            *pixels++ = HCRasterPixelAt(self, xIndex, yIndex);
        }
    }
}

void HCRasterSetPixelsAt(HCRasterRef self, HCInteger startXIndex, HCInteger startYIndex, HCInteger endXIndex, HCInteger endYIndex, const HCRasterColor* pixels) {
    for (HCInteger yIndex = startYIndex; yIndex < endYIndex; yIndex++) {
        for (HCInteger xIndex = startXIndex; xIndex < endXIndex; xIndex++) {
            HCRasterColor pixel = *pixels++;
            HCRasterSetPixelAt(self, xIndex, yIndex, pixel);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Curve Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawPoint(HCRasterRef self, HCReal x, HCReal y, HCRasterColor color) {
    HCRasterSetPixelAt(self, round(x), round(y), color);
}

void HCRasterDrawLine(HCRasterRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCRasterColor c0, HCRasterColor c1) {
    // TODO: Use a non-sampling draw algorithm
    for (HCReal t = 0.0; t <= 1.0; t += 0.01) {
        HCReal tc = 1.0 - t;
        
//        HCReal x = x0 * tc + x1 * t;
//        HCReal y = y0 * tc + y1 * t;
        
        HCReal a = tc;
        HCReal b = t;
        HCReal x = a * x0 + b * x1;
        HCReal y = a * y0 + b * y1;
        
        HCRasterSetPixelAt(self, round(x), round(y), HCRasterColorCombine(c0, c1, t));
    }
}

void HCRasterDrawQuadraticCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCRasterColor c0, HCRasterColor c1) {
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
        
        HCRasterSetPixelAt(self, round(x), round(y), HCRasterColorCombine(c0, c1, t));
    }
}

void HCRasterDrawCubicCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCRasterColor c0, HCRasterColor c1) {
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
        
        HCRasterSetPixelAt(self, round(x), round(y), HCRasterColorCombine(c0, c1, t));
    }
}

void HCRasterDrawPath(HCRasterRef self, const char* path, HCRasterColor color) {
    // Define state data for parsing the path string
    char type = '\0';
    HCReal currentX = 0.0;
    HCReal currentY = 0.0;
    HCReal startX = currentX;
    HCReal startY = currentY;
    HCInteger argumentsExpected = 0;
    HCInteger argumentsCount = 0;
    HCReal arguments[8];
    
    // Parse path data according to subset of STL path element data string format
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
                case 'M': {
                    type = 'M';
                    argumentsCount = 0;
                    argumentsExpected = 2;
                } break;
                case 'L': {
                    type = 'L';
                    argumentsCount = 0;
                    argumentsExpected = 2;
                } break;
                case 'Q': {
                    type = 'Q';
                    argumentsCount = 0;
                    argumentsExpected = 4;
                } break;
                case 'C': {
                    type = 'C';
                    argumentsCount = 0;
                    argumentsExpected = 6;
                } break;
                case 'Z': {
                    HCRasterDrawLine(self, currentX, currentY, startX, startY, color, color);
                    currentX = startX;
                    currentY = startY;
                } break;
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
                        case 'M': startX = arguments[0]; startY = arguments[1]; break;
                        case 'L': HCRasterDrawLine(self, currentX, currentY, arguments[0], arguments[1], color, color); break;
                        case 'Q': HCRasterDrawQuadraticCurve(self, currentX, currentY, arguments[0], arguments[1], arguments[2], arguments[3], color, color); break;
                        case 'C': HCRasterDrawCubicCurve(self, currentX, currentY, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], color, color); break;
                    }
                    
                    // Save the last two arguments as the first two values of the next draw command
                    currentX = arguments[argumentsCount - 2];
                    currentY = arguments[argumentsCount - 1];
                    
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
void HCRasterDrawTriangle(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCRasterColor ca, HCRasterColor cb, HCRasterColor cc) {
    HCRasterDrawLine(self, ax, ay, bx, by, ca, cb);
    HCRasterDrawLine(self, bx, by, cx, cy, cb, cc);
    HCRasterDrawLine(self, cx, cy, ax, ay, cc, ca);
}

void HCRasterFillTriangle(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCRasterColor ca, HCRasterColor cb, HCRasterColor cc) {
    // Calculate triangle constants for barycentric coordinates
    HCReal determinantInv = 1.0 / ((by - cy)*(ax - cx) + (cx - bx)*(ay - cy));
    HCReal cxbxDifference = cx - bx;
    HCReal axcxDifference = ax - cx;
    HCReal bycyDifference = by - cy;
    HCReal cyayDifference = cy - ay;
    
    // TODO: Use a non-sampling draw algorithm
    HCInteger startXIndex = floor(fmin(ax, fmin(bx, cx)));
    HCInteger startYIndex = floor(fmin(ay, fmin(by, cy)));
    HCInteger endXIndex = ceil(fmax(ax, fmax(bx, cx)));
    HCInteger endYIndex = ceil(fmax(ay, fmax(by, cy)));
    for (HCInteger yIndex = startYIndex; yIndex < endYIndex; yIndex++) {
        for (HCInteger xIndex = startXIndex; xIndex < endXIndex; xIndex++) {
            // Calculate barycenteric coordinates for coordinate
            HCReal x = round(xIndex);
            HCReal y = round(yIndex);
            HCReal lambdaA = (bycyDifference*(x - cx) + cxbxDifference*(y - cy)) * determinantInv;
            HCReal lambdaB = (cyayDifference*(x - cx) + axcxDifference*(y - cy)) * determinantInv;
            HCReal lambdaC = 1.0 - lambdaA - lambdaB;
            
            if (lambdaA >= 0.0 && lambdaB >= 0.0 && lambdaC >= 0.0) {
                HCRasterColor color = HCRasterColorCombine3(ca, lambdaA, cb, lambdaB, cc, lambdaC);
                HCRasterSetPixelAt(self, xIndex, yIndex, color);
            }
        }
    }
}

void HCRasterDrawQuad(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCReal dx, HCReal dy, HCRasterColor ca, HCRasterColor cb, HCRasterColor cc, HCRasterColor cd) {
    HCRasterDrawLine(self, ax, ay, bx, by, ca, cb);
    HCRasterDrawLine(self, bx, by, cx, cy, cb, cc);
    HCRasterDrawLine(self, cx, cy, dx, dy, cc, cd);
    HCRasterDrawLine(self, dx, dy, ax, ay, cd, ca);
}

void HCRasterFillQuad(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCReal dx, HCReal dy, HCRasterColor ca, HCRasterColor cb, HCRasterColor cc, HCRasterColor cd) {
    HCRasterFillTriangle(self, ax, ay, bx, by, cx, cy, ca, cb, cc);
    HCRasterFillTriangle(self, ax, ay, cx, cy, dx, dy, ca, cc, cd);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - File Operat; ons
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterSavePPM(HCRasterRef self, const char* path) {
    // Open file for writing
    FILE* file = fopen(path, "w");
    if (file == NULL)
        return;
    
    // Write PPM Header
    char header[1024];
    sprintf(header, "P6 %i %i 255\n", (int)HCRasterWidth(self), (int)HCRasterHeight(self));
    fputs(header, file);
    
    // Write PPM body, top row first, in red, green, blue byte order
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(self); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(self); xIndex++) {
            HCRasterColor pixel = HCRasterPixelAt(self, xIndex, yIndex);
            HCByte r = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.r * 256.0f)));
            HCByte g = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.g * 256.0f)));
            HCByte b = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.b * 256.0f)));
            fwrite(&r, sizeof(HCByte), 1, file);
            fwrite(&g, sizeof(HCByte), 1, file);
            fwrite(&b, sizeof(HCByte), 1, file);
        }
    }
    
    // Cleanup
    fclose(file);
}

void HCRasterSaveBMP(HCRasterRef self, const char* path) {
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
    if (file == NULL)
        return;
    
    // Define header data
    uint16_t magicNumber = 0x4D42;
    uint16_t reserved0 = 0;//0x4D41;
    uint16_t reserved1 = 0;//0x5454;
    uint32_t dataOffset = 54;
    uint32_t infoHeaderSize = 40;
    uint32_t width = (uint32_t)HCRasterWidth(self);
    uint32_t height = (uint32_t)HCRasterHeight(self);
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
#define PACK_PIXEL(red, green, blue) (0x00000000 | ((uint32_t)blue << 24) | ((uint32_t)green << 16) | ((uint32_t)red << 8))
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
#define PACK_PIXEL(red, green, blue) (0x00000000 | ((uint32_t)red << 16) | ((uint32_t)green << 8) | ((uint32_t)blue))
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
    
    // Write BMP body (XXRRGGBB)
    for (HCInteger yIndex = HCRasterHeight(self) - 1; yIndex >= 0; yIndex--) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(self); xIndex++) {
            HCRasterColor pixel = HCRasterPixelAt(self, xIndex, yIndex);
            HCByte r = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.r * 256.0f)));
            HCByte g = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.g * 256.0f)));
            HCByte b = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.b * 256.0f)));
            uint32_t p = PACK_PIXEL(r, g, b);
            fwrite(&p, sizeof(p), 1, file);
        }
    }
    
    // Cleanup
    fclose(file);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Color Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterColor HCRasterColorMake(HCReal a, HCReal r, HCReal g, HCReal b) {
    return (HCRasterColor){ .a = a, .r = r, .g = g, .b = b };
}

HCRasterColor HCRasterColorCombine(HCRasterColor c0, HCRasterColor c1, HCReal t) {
    HCReal tc = 1.0 - t;
    HCReal a = tc * c0.a + t * c1.a;
    HCReal r = tc * c0.r + t * c1.r;
    HCReal g = tc * c0.g + t * c1.g;
    HCReal b = tc * c0.b + t * c1.b;
    return HCRasterColorMake(a, r, g, b);
}

HCRasterColor HCRasterColorCombine3(HCRasterColor ca, HCReal ta, HCRasterColor cb, HCReal tb, HCRasterColor cc, HCReal tc) {
    HCReal a = ta * ca.a + tb * cb.a + tc * cc.a;
    HCReal r = ta * ca.r + tb * cb.r + tc * cc.r;
    HCReal g = ta * ca.g + tb * cb.g + tc * cc.g;
    HCReal b = ta * ca.b + tb * cb.b + tc * cc.b;
    return HCRasterColorMake(a, r, g, b);
}
