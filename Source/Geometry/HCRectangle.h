//
//  HCRectangle.h
//  HollowCore
//
//  Created by Matt Stoker on 10/19/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCRectangle_h
#define HCRectangle_h

#include "HCPoint.h"
#include "HCSize.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCRectangle {
    HCPoint origin;
    HCSize size;
} HCRectangle;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Special Values
//----------------------------------------------------------------------------------------------------------------------------------
#define HCRectangleInvalidStatic   {.origin = HCPointInvalidStatic, .size = HCSizeInvalidStatic}
#define HCRectangleZeroStatic      {.origin = HCPointZeroStatic, .size = HCSizeZeroStatic}

static const HCRectangle HCRectangleInvalid = HCRectangleInvalidStatic;
static const HCRectangle HCRectangleZero = HCRectangleZeroStatic;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef enum HCRectangleEdge {
    HCRectangleEdgeMinX,
    HCRectangleEdgeMinY,
    HCRectangleEdgeMaxX,
    HCRectangleEdgeMaxY,
} HCRectangleEdge;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCRectangle HCRectangleMake(HCPoint origin, HCSize size);
HCRectangle HCRectangleMakeWithComponents(HCReal x, HCReal y, HCReal width, HCReal height);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRectangleIsInvalid(HCRectangle rectangle);
HCBoolean HCRectangleIsSimilar(HCRectangle rectangle, HCRectangle other, HCReal elementDissimilarity);
HCBoolean HCRectangleIsEqual(HCRectangle rectangle, HCRectangle other);
HCInteger HCRectangleHashValue(HCRectangle rectangle);
void HCRectanglePrint(HCRectangle rectangle, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Queries
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRectangleIsZero(HCRectangle rectangle);
HCBoolean HCRectangleIsInfinite(HCRectangle rectangle);
HCBoolean HCRectangleIsEmpty(HCRectangle rectangle);
HCBoolean HCRectangleContainsPoint(HCRectangle rectangle, HCPoint point);
HCBoolean HCRectangleContainsRectangle(HCRectangle rectangle, HCRectangle other);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Calculated Properties
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCRectangleWidth(HCRectangle rectangle);
HCReal HCRectangleHeight(HCRectangle rectangle);
HCReal HCRectangleMinX(HCRectangle rectangle);
HCReal HCRectangleMinY(HCRectangle rectangle);
HCReal HCRectangleMidX(HCRectangle rectangle);
HCReal HCRectangleMidY(HCRectangle rectangle);
HCReal HCRectangleMaxX(HCRectangle rectangle);
HCReal HCRectangleMaxY(HCRectangle rectangle);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCRectangle HCRectangleStandardize(HCRectangle rectangle);
HCRectangle HCRectangleIntegral(HCRectangle rectangle);
HCRectangle HCRectangleOutset(HCRectangle rectangle, HCReal dx, HCReal dy);
HCRectangle HCRectangleInset(HCRectangle rectangle, HCReal dx, HCReal dy);
HCRectangle HCRectangleOffset(HCRectangle rectangle, HCReal dx, HCReal dy);
HCRectangle HCRectangleUnion(HCRectangle rectangle, HCRectangle other);
HCRectangle HCRectangleIntersection(HCRectangle rectangle, HCRectangle other);
HCRectangle HCRectangleIncludingPoint(HCRectangle rectangle, HCPoint point);
void HCRectangleDivide(HCRectangle rectangle, HCRectangle* slice, HCRectangle* remainder, HCReal amount, HCRectangleEdge edge);

#endif /* HCRectangle_h */
