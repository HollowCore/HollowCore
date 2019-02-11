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
HCRasterColor HCRasterPixelAt(HCRasterRef self, HCInteger xIndex, HCInteger yIndex);
void HCRasterSetPixelAt(HCRasterRef self, HCInteger xIndex, HCInteger yIndex, HCRasterColor pixel);
void HCRasterPixelsAt(HCRasterRef self, HCInteger startXIndex, HCInteger startYIndex, HCInteger endXIndex, HCInteger endYIndex, HCRasterColor* pixels);
void HCRasterSetPixelsAt(HCRasterRef self, HCInteger startXIndex, HCInteger startYIndex, HCInteger endXIndex, HCInteger endYIndex, const HCRasterColor* pixels);
void HCRasterClear(HCRasterRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Lookup Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterColor HCRasterPixelNearest(HCRasterRef self, HCReal x, HCReal y);
HCRasterColor HCRasterPixelFiltered(HCRasterRef self, HCReal x, HCReal y);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Curve Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawPoint(HCRasterRef self, HCReal x, HCReal y, HCRasterColor color);
void HCRasterDrawLine(HCRasterRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCRasterColor c0, HCRasterColor c1);
void HCRasterDrawQuadraticCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCRasterColor c0, HCRasterColor c1);
void HCRasterDrawCubicCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCRasterColor c0, HCRasterColor c1);
void HCRasterDrawPath(HCRasterRef self, const char* path, HCRasterColor color);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Shape Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawTriangle(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCRasterColor ca, HCRasterColor cb, HCRasterColor cc);
void HCRasterFillTriangle(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCRasterColor ca, HCRasterColor cb, HCRasterColor cc);
void HCRasterFillTexturedTriangle(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCRasterRef texture, HCReal tax, HCReal tay, HCReal tbx, HCReal tby, HCReal tcx, HCReal tcy);
void HCRasterDrawQuad(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCReal dx, HCReal dy, HCRasterColor ca, HCRasterColor cb, HCRasterColor cc, HCRasterColor cd);
void HCRasterFillQuad(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCReal dx, HCReal dy, HCRasterColor ca, HCRasterColor cb, HCRasterColor cc, HCRasterColor cd);
void HCRasterFillTexturedQuad(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCReal dx, HCReal dy, HCRasterRef texture, HCReal tax, HCReal tay, HCReal tbx, HCReal tby, HCReal tcx, HCReal tcy, HCReal tdx, HCReal tdy);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - File Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterSavePPM(HCRasterRef self, const char* path);
HCRasterRef HCRasterCreateByLoadingBMP(const char* path);
void HCRasterSaveBMP(HCRasterRef self, const char* path);

#endif /* HCRaster_h */
