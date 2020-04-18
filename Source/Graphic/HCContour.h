//
//  HCContour.h
//  HollowCore
//
//  Created by Matt Stoker on 2/11/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#ifndef HCContour_h
#define HCContour_h

#include "../Geometry/HCPoint.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Element Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCContourElement {
    HCPoint c0;
    HCPoint c1;
    HCPoint p;
} HCContourElement;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Element Special Values
//----------------------------------------------------------------------------------------------------------------------------------
#define HCContourElementInvalidStatic {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = HCPointInvalidStatic}
#define HCContourElementZeroStatic    {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = HCPointZeroStatic}

static const HCContourElement HCContourElementInvalid = HCContourElementInvalidStatic;
static const HCContourElement HCContourElementZero = HCContourElementZeroStatic;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Element Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCContourElement HCContourElementMakeLinear(HCPoint p);
HCContourElement HCContourElementMakeQuadratic(HCPoint c, HCPoint p);
HCContourElement HCContourElementMakeCubic(HCPoint c0, HCPoint c1, HCPoint p);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Element Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourElementIsInvalid(HCContourElement element);
HCBoolean HCContourElementIsLinear(HCContourElement element);
HCBoolean HCContourElementIsQuadratic(HCContourElement element);
HCBoolean HCContourElementIsCubic(HCContourElement element);
HCBoolean HCContourElementIsSimilar(HCContourElement element, HCContourElement other, HCReal axisDissimilarity);
HCBoolean HCContourElementIsZero(HCContourElement element);
HCBoolean HCContourElementIsInfinite(HCContourElement element);
HCBoolean HCContourElementIsEqual(HCContourElement element, HCContourElement other);
HCInteger HCContourElementHashValue(HCContourElement element);
void HCContourElementPrint(HCContourElement element, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Element Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCContourElement HCContourElementAsLinear(HCContourElement element);
HCContourElement HCContourElementAsQuadratic(HCContourElement element);
HCContourElement HCContourElementAsCubic(HCContourElement element);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Element Evaluation
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourEvaluateElement(HCReal t, HCPoint p0, HCContourElement element, HCReal* dx, HCReal* dy);
HCPoint HCContourEvaluateLinearCurve(HCReal t, HCPoint p0, HCPoint p1, HCReal* dx, HCReal* dy);
HCPoint HCContourEvaluateQuadraticCurve(HCReal t, HCPoint p0, HCPoint c, HCPoint p1, HCReal* dx, HCReal* dy);
HCPoint HCContourEvaluateCubicCurve(HCReal t, HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal* dx, HCReal* dy);
void HCContourLineLineIntersection(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint q1, HCReal* t, HCReal* u);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourInitWithElementsNoCopy(HCContourElement* elements, HCInteger elementCount, HCBoolean closed);
void HCContourInitWithElements(void* memory, HCContourElement* elements, HCInteger elementCount, HCBoolean closed);
void HCContourInitWithPolyline(void* memory, HCPoint startPoint, HCPoint* points, HCInteger pointCount, HCBoolean closed);
void HCContourInitWithPolyQuadratic(void* memory, HCPoint startPoint, HCPoint* points, HCInteger count, HCBoolean closed);
void HCContourInitWithPolyCubic(void* memory, HCPoint startPoint, HCPoint* points, HCInteger count, HCBoolean closed);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourIsEqual(const HCContourElement* contour, const HCContourElement* other);
HCInteger HCContourHashValue(const HCContourElement* contour);
void HCContourPrint(const HCContourElement* contour, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCContourElementCount(const HCContourElement* contour);
HCBoolean HCContourIsClosed(const HCContourElement* contour);
HCPoint HCContourStartPoint(const HCContourElement* contour);
HCPoint HCContourEndPoint(const HCContourElement* contour);

#endif /* HCContour_h */
