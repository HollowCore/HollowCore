//
//  HCPath_Internal.h
//  HollowCore
//
//  Created by Matt Stoker on 12/28/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

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
    HCListRef elementPolylines;
    HCRectangle bounds;
} HCPath;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathInit(void* memory);
void HCPathDestroy(HCPathRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Polylines
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathAddLineSegmentPolylineData(HCPathRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCDataRef polylineData);
void HCPathAddQuadraticCurvePolylineData(HCPathRef self, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCReal flatnessThreshold, HCDataRef polylineData);
void HCPathAddCubicCurvePolylineData(HCPathRef self, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCReal flatnessThreshold, HCDataRef polylineData);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Intersection
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathIntersects(void* context, HCBoolean* stopSearching, HCPathRef path, HCPathRef otherPath, HCPoint point);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Subpaths
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPathFilterOpen(void* context, HCPathRef path);
HCBoolean HCPathFilterClosed(void* context, HCPathRef path);

#endif /* HCPath_Internal_h */
