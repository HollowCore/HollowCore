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
    for (HCInteger y = 0; y < self->height; y++) {
        for (HCInteger x = 0; x < self->width; x++) {
            HCRasterColor pixel = HCRasterPixelAt(self, x, y);
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
HCRasterColor HCRasterPixelAt(HCRasterRef self, HCInteger x, HCInteger y) {
    if (x >= 0 && y >= 0 && x < self->width && y < self->height) {
        return self->pixels[y * self->width + x];
    }
    return HCRasterColorInvalid;
}

void HCRasterSetPixelAt(HCRasterRef self, HCInteger x, HCInteger y, HCRasterColor pixel) {
    if (x >= 0 && y >= 0 && x < self->width && y < self->height) {
        self->pixels[y * self->width + x] = pixel;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCRasterColor color) {
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
        
        HCRasterSetPixelAt(self, round(x), round(y), color);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - File Operations
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
    for (HCInteger y = HCRasterHeight(self) - 1; y >= 0; y--) {
        for (HCInteger x = 0; x < HCRasterWidth(self); x++) {
            HCRasterColor pixel = HCRasterPixelAt(self, x, y);
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
    for (HCInteger y = HCRasterHeight(self) - 1; y >= 0; y--) {
        for (HCInteger x = 0; x < HCRasterWidth(self); x++) {
            HCRasterColor pixel = HCRasterPixelAt(self, x, y);
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
