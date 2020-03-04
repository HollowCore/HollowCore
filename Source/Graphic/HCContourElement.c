//
//  HCContourElement.c
//  HollowCore
//
//  Created by Matt Stoker on 2/11/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#include "HCContourElement.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCContourElement HCContourElementMakeLinear(HCPoint p) {
    return (HCContourElement){ .p = p, .c0 = HCPointInvalid, .c1 = HCPointInvalid };
}

HCContourElement HCContourElementMakeQuadratic(HCPoint p, HCPoint c) {
    return (HCContourElement){ .p = p, .c0 = c, .c1 = HCPointInvalid };
}

HCContourElement HCContourElementMakeCubic(HCPoint p, HCPoint c0, HCPoint c1) {
    return (HCContourElement){ .p = p, .c0 = c0, .c1 = c1 };
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourElementIsInvalid(HCContourElement element) {
    return
        HCPointIsInvalid(element.p) &&
        HCPointIsInvalid(element.c0) &&
        HCPointIsInvalid(element.c1);
}

HCBoolean HCContourElementIsLinear(HCContourElement element) {
    return
        !HCPointIsInvalid(element.p) &&
        HCPointIsInvalid(element.c0) &&
        HCPointIsInvalid(element.c1);
}

HCBoolean HCContourElementIsQuadratic(HCContourElement element) {
    return
        !HCPointIsInvalid(element.p) &&
        !HCPointIsInvalid(element.c0) &&
        HCPointIsInvalid(element.c1);
}

HCBoolean HCContourElementIsCubic(HCContourElement element) {
    return
        !HCPointIsInvalid(element.p) &&
        !HCPointIsInvalid(element.c0) &&
        !HCPointIsInvalid(element.c1);
}

HCBoolean HCContourElementIsSimilar(HCContourElement element, HCContourElement other, HCReal axisDissimilarity) {
    return
        HCPointIsSimilar(element.p, other.p, axisDissimilarity) &&
        (HCPointIsSimilar(element.c0, other.c0, axisDissimilarity) || (HCPointIsInvalid(element.c0) && HCPointIsInvalid(other.c0))) &&
        (HCPointIsSimilar(element.c1, other.c1, axisDissimilarity) || (HCPointIsInvalid(element.c1) && HCPointIsInvalid(other.c1)));
}

HCBoolean HCContourElementIsZero(HCContourElement element) {
    return
        HCPointIsZero(element.p) &&
        (HCPointIsZero(element.c0) || HCPointIsInvalid(element.c0)) &&
        (HCPointIsZero(element.c1) || HCPointIsInvalid(element.c1));
}

HCBoolean HCContourElementIsInfinite(HCContourElement element) {
    return
        HCPointIsInfinite(element.p) ||
        HCPointIsInfinite(element.c0) ||
        HCPointIsInfinite(element.c1);
}

HCBoolean HCContourElementIsEqual(HCContourElement element, HCContourElement other) {
    return
        HCPointIsEqual(element.p, other.p) &&
        (HCPointIsEqual(element.c0, other.c0) || (HCPointIsInvalid(element.c0) && HCPointIsInvalid(other.c0))) &&
        (HCPointIsEqual(element.c1, other.c1) || (HCPointIsInvalid(element.c1) && HCPointIsInvalid(other.c1)));
}

HCInteger HCContourElementHashValue(HCContourElement element) {
    return
        HCPointHashValue(element.p) ^
        HCPointHashValue(element.c0) ^
        HCPointHashValue(element.c1);
}

void HCContourElementPrint(HCContourElement element, FILE* stream) {
    fprintf(stream, "<p:");
    HCPointPrint(element.p, stream);
    fprintf(stream, ",c0:");
    HCPointPrint(element.c0, stream);
    fprintf(stream, ",c1:");
    HCPointPrint(element.c1, stream);
    fprintf(stream, ">");
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCContourElement HCContourElementAsLinear(HCContourElement element) {
    if (HCContourElementIsLinear(element)) {
        return element;
    }
    if (HCContourElementIsQuadratic(element)) {
        return HCContourElementMakeLinear(element.p);
    }
    return HCContourElementMakeLinear(element.p);
}

HCContourElement HCContourElementAsQuadratic(HCContourElement element) {
    if (HCContourElementIsQuadratic(element)) {
        return element;
    }
    if (HCContourElementIsCubic(element)) {
        return HCContourElementMakeQuadratic(element.p, HCPointInterpolate(element.c0, element.c1, 0.5));
    }
    return HCContourElementMakeQuadratic(element.p, element.p);
}

HCContourElement HCContourElementAsCubic(HCContourElement element) {
    if (HCContourElementIsCubic(element)) {
        return element;
    }
    if (HCContourElementIsQuadratic(element)) {
        return HCContourElementMakeCubic(element.p, element.c0, element.c0);
    }
    return HCContourElementMakeCubic(element.p, element.p, element.p);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Evaluation
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourEvaluateElement(HCReal t, HCPoint p0, HCContourElement element, HCReal* dx, HCReal* dy) {
    if (HCPointIsInvalid(element.c1)) {
        if (HCPointIsInvalid(element.c0)) {
            return HCContourEvaluateLinearCurve(t, p0, element.p, dx, dy);
        }
        return HCContourEvaluateQuadraticCurve(t, p0, element.p, element.c0, dx, dy);
    }
    return HCContourEvaluateCubicCurve(t, p0, element.p, element.c0, element.c1, dx, dy);
}

HCPoint HCContourEvaluateLinearCurve(HCReal t, HCPoint p0, HCPoint p1, HCReal* dx, HCReal* dy) {
    HCReal tc = 1.0 - t;
        
    HCReal x = p0.x * tc + p1.x * t;
    HCReal y = p0.y * tc + p1.y * t;
        
//    HCReal a = tc;
//    HCReal b = t;
//    HCReal x = a * p0.x + b * p1.x;
//    HCReal y = a * p0.y + b * p1.y;
    
    if (dx != NULL) {
        *dx = p1.x - p0.x;
    }
    if (dy != NULL) {
        *dy = p1.y - p0.y;
    }
    return HCPointMake(x, y);
}

HCPoint HCContourEvaluateQuadraticCurve(HCReal t, HCPoint p0, HCPoint c, HCPoint p1, HCReal* dx, HCReal* dy) {
    HCReal tc = 1.0 - t;
    
    HCReal s0x = p0.x * tc + c.x * t;
    HCReal s0y = p0.y * tc + c.y * t;
    HCReal s1x = c.x * tc + p1.x * t;
    HCReal s1y = c.y * tc + p1.y * t;
    HCReal x = s0x * tc + s1x * t;
    HCReal y = s0y * tc + s1y * t;
    
//    HCReal x = (x0 * tc + c.x * t) * tc + (cx * tc + p1.x * t) * t;
//    HCReal y = (y0 * tc + c.y * t) * tc + (cy * tc + p1.y * t) * t;

//    HCReal x = p0.x * tc * tc + c.x * t * tc + c.x * tc * t + p1.x * t * t;
//    HCReal y = p0.y * tc * tc + c.y * t * tc + c.y * tc * t + p1.y * t * t;

//    HCReal t2 = t * t;
//    HCReal tc2 = tc * tc;
//    HCReal x = p0.x * tc2 + c.x * t * tc + c.x * tc * t + p1.x * t2;
//    HCReal y = p0.y * tc2 + c.y * t * tc + c.y * tc * t + p1.y * t2;
    
//    HCReal t2 = t * t;
//    HCReal tc2 = tc * tc;
//    HCReal x = p0.x * tc2 + (2.0 * c.x) * t * tc + p1.x * t2;
//    HCReal y = p0.y * tc2 + (2.0 * c.y) * t * tc + p1.y * t2;
    
//    HCReal a = tc * tc;
//    HCReal b = 2.0 * t * tc;
//    HCReal c = t * t;
//    HCReal x = a * p0.x + b * c.x + c * p1.x;
//    HCReal y = a * p0.y + b * c.y + c * p1.y;
    
    if (dx != NULL) {
        *dx = s1x - s0x;
    }
    if (dy != NULL) {
        *dy = s1y - s0y;
    }
    return HCPointMake(x, y);
}

HCPoint HCContourEvaluateCubicCurve(HCReal t, HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal* dx, HCReal* dy) {
    HCReal tc = 1.0 - t;
        
    HCReal s0x = p0.x * tc + c0.x * t;
    HCReal s0y = p0.y * tc + c0.y * t;
    HCReal scx = c0.x * tc + c1.x * t;
    HCReal scy = c0.y * tc + c1.y * t;
    HCReal s1x = c1.x * tc + p1.x * t;
    HCReal s1y = c1.y * tc + p1.y * t;
    HCReal ss0x = s0x * tc + scx * t;
    HCReal ss0y = s0y * tc + scy * t;
    HCReal ss1x = scx * tc + s1x * t;
    HCReal ss1y = scy * tc + s1y * t;
    HCReal x = ss0x * tc + ss1x * t;
    HCReal y = ss0y * tc + ss1y * t;

//    HCReal ss0x = (x0 * tc + c0.x * t) * tc + (cx0 * tc + c1.x * t) * t;
//    HCReal ss0y = (y0 * tc + c0.y * t) * tc + (cy0 * tc + c1.y * t) * t;
//    HCReal ss1x = (cx0 * tc + c1.x * t) * tc + (cx1 * tc + p1.x * t) * t;
//    HCReal ss1y = (cy0 * tc + c1.y * t) * tc + (cy1 * tc + p1.y * t) * t;
//    HCReal x = ss0x * tc + ss1x * t;
//    HCReal y = ss0y * tc + ss1y * t;

//    HCReal x = ((x0 * tc + c0.x * t) * tc + (cx0 * tc + c1.x * t) * t) * tc + ((cx0 * tc + c1.x * t) * tc + (cx1 * tc + p1.x * t) * t) * t;
//    HCReal y = ((y0 * tc + c0.y * t) * tc + (cy0 * tc + c1.y * t) * t) * tc + ((cy0 * tc + c1.y * t) * tc + (cy1 * tc + p1.y * t) * t) * t;

//    HCReal x = (x0 * tc + c0.x * t) * tc * tc + (cx0 * tc + c1.x * t) * t * tc + (cx0 * tc + c1.x * t) * t * tc + (cx1 * tc + p1.x * t) * t * t;
//    HCReal y = (y0 * tc + c0.y * t) * tc * tc + (cy0 * tc + c1.y * t) * t * tc + (cy0 * tc + c1.y * t) * t * tc + (cy1 * tc + p1.y * t) * t * t;

//    HCReal x = p0.x * tc * tc * tc + c0.x * tc * tc * t + c0.x * t * tc * tc + c1.x * t * t * tc + c0.x * t * tc * tc + c1.x * t * t * tc + c1.x * t * t * tc + p1.x * t * t * t;
//    HCReal y = p0.y * tc * tc * tc + c0.y * tc * tc * t + c0.y * t * tc * tc + c1.y * t * t * tc + c0.y * t * tc * tc + c1.y * t * t * tc + c1.y * t * t * tc + p1.y * t * t * t;

//    HCReal t2 = t * t;
//    HCReal t3 = t * t * t;
//    HCReal tc2 = tc * tc;
//    HCReal tc3 = tc * tc * tc;
//    HCReal x = p0.x * tc3 + c0.x * t * tc2 + c0.x * t * tc2 + c1.x * t2 * tc + c0.x * t * tc2 + c1.x * t2 * tc + c1.x * t2 * tc + p1.x * t3;
//    HCReal y = p0.y * tc3 + c0.y * t * tc2 + c0.y * t * tc2 + c1.y * t2 * tc + c0.y * t * tc2 + c1.y * t2 * tc + c1.y * t2 * tc + p1.y * t3;

//    HCReal t2 = t * t;
//    HCReal t3 = t * t * t;
//    HCReal tc2 = tc * tc;
//    HCReal tc3 = tc * tc * tc;
//    HCReal x = p0.x * tc3 + (3.0 * c0.x) * t * tc2 + (3.0 * c1.x) * t2 * tc + p1.x * t3;
//    HCReal y = p0.y * tc3 + (3.0 * c0.y) * t * tc2 + (3.0 * c1.y) * t2 * tc + p1.y * t3;
    
//    HCReal a = tc * tc * tc;
//    HCReal b = 3.0 * t * tc * tc;
//    HCReal c = 3.0 * t * t * tc;
//    HCReal d = t * t * t;
//    HCReal x = a * p0.x + b * c0.x + c * c1.x + d * p1.x;
//    HCReal y = a * p0.y + b * c0.y + c * c1.y + d * p1.y;
    
    if (dx != NULL) {
        *dx = ss1x - ss0x;
    }
    if (dy != NULL) {
        *dy = ss1y - ss0y;
    }
    return HCPointMake(x, y);
}

void HCContourLineLineIntersection(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint q1, HCReal* t, HCReal* u) {
    HCReal d = (p0.x - p1.x) * (q0.y - q1.y) - (p0.y - p1.y) * (q0.x - q1.x);
    *t =      ((p0.x - q0.x) * (q0.y - q1.y) - (p0.y - q0.y) * (q0.x - q1.x)) / d;
    *u =     -((p0.x - p1.x) * (p0.y - q0.y) - (p0.y - p1.y) * (p0.x - q0.x)) / d;
}
