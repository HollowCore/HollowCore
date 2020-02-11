//
//  HCContourElement.h
//  HollowCore
//
//  Created by Matt Stoker on 2/11/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#ifndef HCContourElement_h
#define HCContourElement_h

#include "../Geometry/HCPoint.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCContourElement {
    HCPoint p;
    HCPoint c0;
    HCPoint c1;
} HCContourElement;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Special Values
//----------------------------------------------------------------------------------------------------------------------------------
#define HCContourElementInvalidStatic {.p = HCPointInvalidStatic, .c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic}
#define HCContourElementZeroStatic    {.p = HCPointZeroStatic, .c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic}

static const HCContourElement HCContourElementInvalid = HCContourElementInvalidStatic;
static const HCContourElement HCContourElementZero = HCContourElementZeroStatic;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCContourElement HCContourElementMakeLinear(HCPoint p);
HCContourElement HCContourElementMakeQuadratic(HCPoint p, HCPoint c);
HCContourElement HCContourElementMakeCubic(HCPoint p, HCPoint c0, HCPoint c1);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourElementIsLinear(HCContourElement element);
HCBoolean HCContourElementIsAlmostLinear(HCContourElement element);
HCBoolean HCContourElementIsQuadratic(HCContourElement element);
HCBoolean HCContourElementIsAlmostQuadratic(HCContourElement element);
HCBoolean HCContourElementIsCubic(HCContourElement element);
HCBoolean HCContourElementIsAlmostCubic(HCContourElement element);
HCBoolean HCContourElementIsSimilar(HCContourElement element, HCContourElement other, HCReal axisDissimilarity);
HCBoolean HCContourElementIsEqual(HCContourElement element, HCContourElement other);
HCInteger HCContourElementHashValue(HCContourElement element);
void HCContourElementPrint(HCContourElement element, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourElementAsLinear(HCContourElement element);
HCBoolean HCContourElementAsQuadratic(HCContourElement element);
HCBoolean HCContourElementAsCubic(HCContourElement element);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Evaluation
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourEvaluateElement(HCReal t, HCPoint p0, HCContourElement element, HCReal* dx, HCReal* dy);
HCPoint HCContourEvaluateLinearCurve(HCReal t, HCPoint p0, HCPoint p1, HCReal* dx, HCReal* dy);
HCPoint HCContourEvaluateQuadraticCurve(HCReal t, HCPoint p0, HCPoint c, HCPoint p1, HCReal* dx, HCReal* dy);
HCPoint HCContourEvaluateCubicCurve(HCReal t, HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal* dx, HCReal* dy);
void HCContourLineLineIntersection(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint q1, HCReal* t, HCReal* u);

#endif /* HCContourElement_h */
