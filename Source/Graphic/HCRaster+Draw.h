///
/// @file HCRaster+Draw.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 4/18/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCRaster_Draw_h
#define HCRaster_Draw_h

#include "HCRaster.h"
#include "../Geometry/HCContour.h"
#include "../Geometry/HCPath.h"
#include "../Geometry/HCPath+SVG.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
#define HCRasterColorRotatingStatic ((HCColor){.a = -1000000.0, .r = 0.0, .g = 0.0, .b = 0.0})
static const HCColor HCRasterColorRotating = HCRasterColorRotatingStatic;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Line Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawPoint(HCRasterRef self, HCReal x, HCReal y, HCColor color);
void HCRasterDrawLine(HCRasterRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCColor c0, HCColor c1);
void HCRasterDrawQuadraticCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCColor c0, HCColor c1);
void HCRasterDrawCubicCurve(HCRasterRef self, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCColor c0, HCColor c1);
void HCRasterDrawArc(HCRasterRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCReal xr, HCReal yr, HCReal theta, HCBoolean largeArc, HCBoolean sweep, HCColor c0, HCColor c1);

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
// MARK: - Contour Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawPolyline(HCRasterRef self, HCPoint startPoint, const HCPoint* points, HCInteger pointCount, HCBoolean closed, HCColor color);
void HCRasterDrawPolyquadratic(HCRasterRef self, HCPoint startPoint, const HCPoint* points, HCInteger quadraticCount, HCBoolean closed, HCColor color);
void HCRasterDrawPolycubic(HCRasterRef self, HCPoint startPoint, const HCPoint* points, HCInteger cubicCount, HCBoolean closed, HCColor color);
void HCRasterDrawContourCurves(HCRasterRef self, const HCContourCurve* curves, HCInteger count, HCBoolean closed, HCColor color);
void HCRasterDrawContour(HCRasterRef self, const HCContour* contour, HCColor color);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterDrawPath(HCRasterRef self, HCPathRef path, HCColor color);
void HCRasterDrawPathData(HCRasterRef self, const char* pathData, HCColor color);

#endif /* HCRaster_Draw_h */
