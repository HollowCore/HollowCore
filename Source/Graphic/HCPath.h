//
//  HCPath.h
//  HollowCore
//
//  Created by Matt Stoker on 12/28/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCPath_h
#define HCPath_h

#include "../Core/HCObject.h"
#include "../Geometry/HCRectangle.h"
#include "../Data/HCData.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCPathType;
typedef struct HCPath* HCPathRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef enum HCPathCommand {
    HCPathCommandMove,
    HCPathCommandAddLine,
    HCPathCommandAddQuadraticCurve,
    HCPathCommandAddCubicCurve,
    HCPathCommandCloseSubpath,
} HCPathCommand;

typedef struct HCPathElement {
    HCPathCommand command;
    HCPoint* points;
} HCPathElement;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCPathRef HCPathCreate(const char* path);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPathIsEqual(HCPathRef self, HCPathRef other);
HCInteger HCPathHashValue(HCPathRef self);
void HCPathPrint(HCPathRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCPathElementCount(HCPathRef self);
HCPathElement HCPathElementAt(HCPathRef self, HCInteger elementIndex);
HCPoint HCPathCurrentPoint(HCPathRef self);
HCRectangle HCPathBounds(HCPathRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Manipulation
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathMoveToPoint(HCPathRef self, HCReal x, HCReal y);
void HCPathAddLine(HCPathRef self, HCReal x, HCReal y);
void HCPathAddQuadraticCurve(HCPathRef self, HCReal cx, HCReal cy, HCReal x, HCReal y);
void HCPathAddCubicCurve(HCPathRef self, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x, HCReal y);
void HCPathCloseSubpath(HCPathRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Conversion
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathPrintData(HCPathRef self, FILE* stream);
HCDataRef HCPathAsLineSegmentDataRetained(HCPathRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Intersection
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPathIntersectsPath(HCPathRef self, HCPathRef other);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Evaluation
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathEvaluateLine(HCReal t, HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCReal* sx, HCReal* sy, HCReal* dx, HCReal* dy);
void HCPathEvaluateQuadraticCurve(HCReal t, HCReal x0, HCReal y0, HCReal cx, HCReal cy, HCReal x1, HCReal y1, HCReal* sx, HCReal* sy, HCReal* dx, HCReal* dy);
void HCPathEvaluateCubicCurve(HCReal t, HCReal x0, HCReal y0, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x1, HCReal y1, HCReal* sx, HCReal* sy, HCReal* dx, HCReal* dy);
void HCPathLineLineIntersection(HCReal x0, HCReal y0, HCReal x1, HCReal y1, HCReal x2, HCReal y2, HCReal x3, HCReal y3, HCReal* t, HCReal* u);

#endif /* HCPath_h */
