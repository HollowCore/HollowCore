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
    HCInteger elementCount;
    HCPathElement* elements;
} HCPath;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathInit(void* memory, const char* path);
void HCPathDestroy(HCPathRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Conversion
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathAddLineSegment(HCPathRef self, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCDataRef segmentData);
void HCPathAddQuadraticCurveSegments(HCPathRef self, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCDataRef segmentData);
void HCPathAddCubicCurveSegments(HCPathRef self, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCDataRef segmentData);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Intersection
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathIntersects(void* context, HCBoolean* stopSearching, HCPathRef path, HCPathRef otherPath, HCPoint point);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Parsing
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathParse(HCPathRef self, const char* path);

#endif /* HCPath_Internal_h */
