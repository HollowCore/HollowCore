//
//  HCRaster.h
//  Test
//
//  Created by Matt Stoker on 2/3/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCRaster_h
#define HCRaster_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCRasterType;
typedef struct HCRaster* HCRasterRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCRasterColor {
    HCReal a;
    HCReal r;
    HCReal g;
    HCReal b;
} HCRasterColor;

extern HCRasterColor HCRasterColorInvalid;
extern HCRasterColor HCRasterColorClear;
extern HCRasterColor HCRasterColorBlack;
extern HCRasterColor HCRasterColorRed;
extern HCRasterColor HCRasterColorGreen;
extern HCRasterColor HCRasterColorBlue;
extern HCRasterColor HCRasterColorYellow;
extern HCRasterColor HCRasterColorCyan;
extern HCRasterColor HCRasterColorMagenta;
extern HCRasterColor HCRasterColorWhite;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterRef HCRasterCreate(HCInteger width, HCInteger height);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRasterIsEqual(HCRasterRef self, HCRasterRef other);
HCInteger HCRasterHashValue(HCRasterRef self);
void HCRasterPrint(HCRasterRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCRasterWidth(HCRasterRef self);
HCInteger HCRasterHeight(HCRasterRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Pixel Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterColor HCRasterPixelAt(HCRasterRef self, HCInteger x, HCInteger y);
void HCRasterSetPixelAt(HCRasterRef self, HCInteger x, HCInteger y, HCRasterColor pixel);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawQuadCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCRasterColor color);
void HCRasterDrawCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCRasterColor color);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - File Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterSavePPM(HCRasterRef self, const char* path);
void HCRasterSaveBMP(HCRasterRef self, const char* path);

#endif /* HCRaster_h */
