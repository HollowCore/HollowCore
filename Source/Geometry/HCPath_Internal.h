///
/// @file HCPath_Internal.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 12/28/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCPath_Internal_h
#define HCPath_Internal_h

#include "../Core/HCObject_Internal.h"
#include "HCPath.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCPath {
    HCObject base;
    HCDataRef elementData;
    HCListRef polylines;
    HCListRef contours;
    HCRectangle bounds;
} HCPath;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathInit(void* memory);
void HCPathDestroy(HCPathRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Intersection
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathIntersects(void* context, HCBoolean* stopSearching, HCPathRef path, HCPathRef otherPath, HCPoint point);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Polyline Conversion
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathAddLineSegmentPolylineData(HCPathRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCDataRef polylineData);
void HCPathAddQuadraticCurvePolylineData(HCPathRef self, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCReal flatnessThreshold, HCDataRef polylineData);
void HCPathAddCubicCurvePolylineData(HCPathRef self, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCReal flatnessThreshold, HCDataRef polylineData);

#endif /* HCPath_Internal_h */
