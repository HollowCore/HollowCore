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

typedef void (*HCPathIntersectionFunction)(void* context, HCBoolean* continueSearching, HCPathRef path, HCPathRef otherPath, HCPoint point);
#define HCPathFlatnessCoarse 1.01
#define HCPathFlatnessNormal 1.001
#define HCPathFlatnessFine 1.0001

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCPathRef HCPathCreate(const char* path);
HCPathRef HCPathCreateEmpty(void);
HCPathRef HCPathCreateWithElements(HCPathElement* elements, HCInteger elementCount);
HCPathRef HCPathCreateWithSubpaths(HCListRef subpaths);

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
HCDataRef HCPathElementPolylineDataRetained(HCPathRef self, HCInteger elementIndex);
HCInteger HCPathElementPolylinePointCount(HCPathRef self, HCInteger elementIndex);
HCPoint HCPathElementPolylinePointAt(HCPathRef self, HCInteger elementIndex, HCInteger pointIndex);
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
void HCPathAddElement(HCPathRef self, HCPathCommand command, const HCPoint* points);
void HCPathRemoveLastElement(HCPathRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Conversion
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathPrintData(HCPathRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Subpaths
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPathSubpathContainingElementIsOpen(HCPathRef self, HCInteger elementIndex, HCInteger* startIndex, HCInteger* endIndex);
HCBoolean HCPathSubpathContainingElementIsClosed(HCPathRef self, HCInteger elementIndex, HCInteger* startIndex, HCInteger* endIndex);
HCPathRef HCPathSubpathContaingElementRetained(HCPathRef self, HCInteger elementIndex, HCInteger* startIndex, HCInteger* endIndex, HCBoolean* isOpen);
HCListRef HCPathSubpathsRetained(HCPathRef self);
HCListRef HCPathOpenSubpathsRetained(HCPathRef self);
HCListRef HCPathClosedSubpathsRetained(HCPathRef self);
HCPathRef HCPathOpenSubpathsAsPathRetained(HCPathRef self);
HCPathRef HCPathClosedSubpathsAsPathRetained(HCPathRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Intersection
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCPathContainsPoint(HCPathRef self, HCPoint point);
HCBoolean HCPathContainsPointNonZero(HCPathRef self, HCPoint point);
HCBoolean HCPathIntersectsPath(HCPathRef self, HCPathRef other);
void HCPathIntersections(HCPathRef self, HCPathRef other, HCPathIntersectionFunction intersection, void* context);


#endif /* HCPath_h */
