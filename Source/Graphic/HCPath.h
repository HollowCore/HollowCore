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
#include "../Container/HCList.h"
#include "HCContour.h"

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
    HCPathCommandCloseContour,
} HCPathCommand;

typedef struct HCPathElement {
    HCPathCommand command;
    HCPoint* points;
} HCPathElement;

typedef void (*HCPathIntersectionFunction)(void* context, HCBoolean* continueSearching, HCPathRef path, HCPathRef otherPath, HCPoint point);
#define HCPathFlatnessCoarse 1.01
#define HCPathFlatnessNormal 1.001
#define HCPathFlatnessFine 1.0001

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCPathRef HCPathCreateEmpty(void);
HCPathRef HCPathCreateWithElements(HCPathElement* elements, HCInteger elementCount);
HCPathRef HCPathCreateWithSubpaths(HCListRef subpaths);
HCPathRef HCPathCreateWithContourElements(HCContourElement* contourElements, HCInteger contourElementCount);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPathIsEqual(HCPathRef self, HCPathRef other);
HCInteger HCPathHashValue(HCPathRef self);
void HCPathPrint(HCPathRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCPathIsEmpty(HCPathRef self);
HCInteger HCPathElementCount(HCPathRef self);
HCPathElement HCPathElementAt(HCPathRef self, HCInteger elementIndex);
HCRectangle HCPathBounds(HCPathRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contours
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCPathContourCount(HCPathRef self);
HCInteger HCPathContourCurveCount(HCPathRef self, HCInteger contourIndex);
HCContourElement HCPathContourCurveAt(HCPathRef self, HCInteger contourIndex, HCInteger curveIndex);
const HCContourElement* HCPathContourAt(HCPathRef self, HCInteger contourIndex);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contour / Path Element Correspondence
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCPathIndexOfContourContainingElement(HCPathRef self, HCInteger elementIndex);
HCBoolean HCPathContourContainingElementIsOpen(HCPathRef self, HCInteger elementIndex, HCInteger* startIndex, HCInteger* endIndex);
HCBoolean HCPathContourContainingElementIsClosed(HCPathRef self, HCInteger elementIndex, HCInteger* startIndex, HCInteger* endIndex);
HCPathRef HCPathContourPathContaingElementRetained(HCPathRef self, HCInteger elementIndex, HCInteger* startIndex, HCInteger* endIndex, HCBoolean* isOpen);
HCListRef HCPathContourPathsRetained(HCPathRef self);
HCListRef HCPathOpenContourPathsRetained(HCPathRef self);
HCListRef HCPathClosedContourPathsRetained(HCPathRef self);
HCPathRef HCPathOpenContoursAsPathRetained(HCPathRef self);
HCPathRef HCPathClosedContoursAsPathRetained(HCPathRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Polylines
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCPathPolylineCount(HCPathRef self);
HCInteger HCPathPolylinePointCount(HCPathRef self, HCInteger polylineIndex);
HCPoint HCPathPolylinePointAt(HCPathRef self, HCInteger polylineIndex, HCInteger pointIndex);
const HCPoint* HCPathPolylineAt(HCPathRef self, HCInteger polylineIndex);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Polyline / Path Element Correspondence
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCPathIndexOfPolylineContainingElement(HCPathRef self, HCInteger elementIndex);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Intersection
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPathContainsPoint(HCPathRef self, HCPoint point);
HCBoolean HCPathContainsPointNonZero(HCPathRef self, HCPoint point);
HCBoolean HCPathIntersectsPath(HCPathRef self, HCPathRef other);
void HCPathIntersections(HCPathRef self, HCPathRef other, HCPathIntersectionFunction intersection, void* context);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Manipulation
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCPathCurrentPoint(HCPathRef self);
HCDataRef HCPathCurrentContour(HCPathRef self);
void HCPathMove(HCPathRef self, HCReal x, HCReal y);
void HCPathAddLine(HCPathRef self, HCReal x, HCReal y);
void HCPathAddQuadraticCurve(HCPathRef self, HCReal cx, HCReal cy, HCReal x, HCReal y);
void HCPathAddCubicCurve(HCPathRef self, HCReal cx0, HCReal cy0, HCReal cx1, HCReal cy1, HCReal x, HCReal y);
void HCPathClose(HCPathRef self);
void HCPathAppendElement(HCPathRef self, HCPathCommand command, const HCPoint* points);
void HCPathRemoveElement(HCPathRef self);

#endif /* HCPath_h */
