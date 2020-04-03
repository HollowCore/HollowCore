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
#include "HCColor.h"
#include "HCPath.h"
#include "HCPath+SVG.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCRasterType;
typedef struct HCRaster* HCRasterRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
extern HCInteger HCRasterSizeMax;
#define HCRasterColorRotatingStatic {.a = -1000000.0, .r = 0.0, .g = 0.0, .b = 0.0}
static const HCColor HCRasterColorRotating = HCRasterColorRotatingStatic;

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
HCColor HCRasterPixelAt(HCRasterRef self, HCInteger xIndex, HCInteger yIndex);
void HCRasterSetPixelAt(HCRasterRef self, HCInteger xIndex, HCInteger yIndex, HCColor pixel);
void HCRasterPixelsAt(HCRasterRef self, HCInteger startXIndex, HCInteger startYIndex, HCInteger endXIndex, HCInteger endYIndex, HCColor* pixels);
void HCRasterSetPixelsAt(HCRasterRef self, HCInteger startXIndex, HCInteger startYIndex, HCInteger endXIndex, HCInteger endYIndex, const HCColor* pixels);
void HCRasterClear(HCRasterRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Lookup Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCColor HCRasterPixelNearest(HCRasterRef self, HCReal x, HCReal y);
HCColor HCRasterPixelFiltered(HCRasterRef self, HCReal x, HCReal y);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Curve Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawPoint(HCRasterRef self, HCReal x, HCReal y, HCColor color);
void HCRasterDrawLine(HCRasterRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCColor c0, HCColor c1);
void HCRasterDrawQuadraticCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCColor c0, HCColor c1);
void HCRasterDrawCubicCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCColor c0, HCColor c1);
void HCRasterDrawArc(HCRasterRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCReal xr, HCReal yr, HCReal theta, HCBoolean largeArc, HCBoolean sweep, HCColor c0, HCColor c1);
void HCRasterDrawPathData(HCRasterRef self, const char* pathData, HCColor color);
void HCRasterDrawPath(HCRasterRef self, HCPathRef path, HCColor color);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Shape Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawTriangle(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCColor ca, HCColor cb, HCColor cc);
void HCRasterFillTriangle(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCColor ca, HCColor cb, HCColor cc);
void HCRasterFillTexturedTriangle(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCRasterRef texture, HCReal tax, HCReal tay, HCReal tbx, HCReal tby, HCReal tcx, HCReal tcy);
void HCRasterDrawQuad(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCReal dx, HCReal dy, HCColor ca, HCColor cb, HCColor cc, HCColor cd);
void HCRasterFillQuad(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCReal dx, HCReal dy, HCColor ca, HCColor cb, HCColor cc, HCColor cd);
void HCRasterFillTexturedQuad(HCRasterRef self, HCReal ax, HCReal ay, HCReal bx, HCReal by, HCReal cx, HCReal cy, HCReal dx, HCReal dy, HCRasterRef texture, HCReal tax, HCReal tay, HCReal tbx, HCReal tby, HCReal tcx, HCReal tcy, HCReal tdx, HCReal tdy);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Data Extraction
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterExtractARGB8888(HCRasterRef self, uint32_t* pixels);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - File Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterRef HCRasterCreateByLoadingPPM(const char* path);
void HCRasterSavePPM(HCRasterRef self, const char* path);
void HCRasterSavePPMWithOptions(HCRasterRef self, const char* path, HCBoolean binary);
HCRasterRef HCRasterCreateByLoadingBMP(const char* path);
void HCRasterSaveBMP(HCRasterRef self, const char* path);
void HCRasterSaveBMPWithOptions(HCRasterRef self, const char* path, HCBoolean reverse);

#endif /* HCRaster_h */
