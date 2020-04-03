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
    HCPoint p;
    HCPoint c0;
    HCPoint c1;
} HCContourElement;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Element Special Values
//----------------------------------------------------------------------------------------------------------------------------------
#define HCContourElementInvalidStatic {.p = HCPointInvalidStatic, .c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic}
#define HCContourElementZeroStatic    {.p = HCPointZeroStatic, .c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic}

static const HCContourElement HCContourElementInvalid = HCContourElementInvalidStatic;
static const HCContourElement HCContourElementZero = HCContourElementZeroStatic;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Element Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCContourElement HCContourElementMakeLinear(HCPoint p);
HCContourElement HCContourElementMakeQuadratic(HCPoint p, HCPoint c);
HCContourElement HCContourElementMakeCubic(HCPoint p, HCPoint c0, HCPoint c1);

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
HCBoolean HCContourIsEqual(HCContourElement* contour, HCContourElement* other);
HCInteger HCContourHashValue(HCContourElement* contour);
void HCContourPrint(HCContourElement* contour, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCContourElementCount(HCContourElement* contour);
HCBoolean HCContourIsClosed(HCContourElement* contour);
HCPoint HCContourStartPoint(HCContourElement* contour);
HCPoint HCContourEndPoint(HCContourElement* contour);

#endif /* HCContour_h */
