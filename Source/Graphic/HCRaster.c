///
/// @file HCRaster.c
/// @package Test
///
/// @author Matt Stoker
/// @date 2/3/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

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
