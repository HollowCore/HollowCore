///
/// @file HCContourCurve.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 2/11/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCContour.h"
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCContourCurve HCContourCurveMakeLinear(HCPoint p) {
    return (HCContourCurve){ .c0 = HCPointInvalid, .c1 = HCPointInvalid, .p = p };
}

HCContourCurve HCContourCurveMakeQuadratic(HCPoint c, HCPoint p) {
    return (HCContourCurve){ .c0 = c, .c1 = HCPointInvalid, .p = p };
}

HCContourCurve HCContourCurveMakeCubic(HCPoint c0, HCPoint c1, HCPoint p) {
    return (HCContourCurve){ .c0 = c0, .c1 = c1, .p = p };
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourCurveIsInvalid(HCContourCurve curve) {
    return
        HCPointIsInvalid(curve.c0) &&
        HCPointIsInvalid(curve.c1) &&
        HCPointIsInvalid(curve.p);
}

HCBoolean HCContourCurveIsLinear(HCContourCurve curve) {
    return
        HCPointIsInvalid(curve.c0) &&
        HCPointIsInvalid(curve.c1) &&
        !HCPointIsInvalid(curve.p);
}

HCBoolean HCContourCurveIsQuadratic(HCContourCurve curve) {
    return
        !HCPointIsInvalid(curve.c0) &&
        HCPointIsInvalid(curve.c1) &&
        !HCPointIsInvalid(curve.p);
}

HCBoolean HCContourCurveIsCubic(HCContourCurve curve) {
    return
        !HCPointIsInvalid(curve.c0) &&
        !HCPointIsInvalid(curve.c1) &&
        !HCPointIsInvalid(curve.p);
}

HCBoolean HCContourCurveIsSimilar(HCContourCurve curve, HCContourCurve other, HCReal axisDissimilarity) {
    return
        (HCPointIsSimilar(curve.c0, other.c0, axisDissimilarity) || (HCPointIsInvalid(curve.c0) && HCPointIsInvalid(other.c0))) &&
        (HCPointIsSimilar(curve.c1, other.c1, axisDissimilarity) || (HCPointIsInvalid(curve.c1) && HCPointIsInvalid(other.c1))) &&
        HCPointIsSimilar(curve.p, other.p, axisDissimilarity);
}

HCBoolean HCContourCurveIsZero(HCContourCurve curve) {
    return
        (HCPointIsZero(curve.c0) || HCPointIsInvalid(curve.c0)) &&
        (HCPointIsZero(curve.c1) || HCPointIsInvalid(curve.c1)) &&
        HCPointIsZero(curve.p);
}

HCBoolean HCContourCurveIsInfinite(HCContourCurve curve) {
    return
        HCPointIsInfinite(curve.c0) ||
        HCPointIsInfinite(curve.c1) ||
        HCPointIsInfinite(curve.p);
}

HCBoolean HCContourCurveIsEqual(HCContourCurve curve, HCContourCurve other) {
    return
        (HCPointIsEqual(curve.c0, other.c0) || (HCPointIsInvalid(curve.c0) && HCPointIsInvalid(other.c0))) &&
        (HCPointIsEqual(curve.c1, other.c1) || (HCPointIsInvalid(curve.c1) && HCPointIsInvalid(other.c1))) &&
        HCPointIsEqual(curve.p, other.p);
}

HCInteger HCContourCurveHashValue(HCContourCurve curve) {
    return
        HCPointHashValue(curve.c0) ^
        HCPointHashValue(curve.c1) ^
        HCPointHashValue(curve.p);
}

void HCContourCurvePrint(HCContourCurve curve, FILE* stream) {
    fprintf(stream, "<c0:");
    HCPointPrint(curve.c0, stream);
    fprintf(stream, ",c1:");
    HCPointPrint(curve.c1, stream);
    fprintf(stream, ",p:");
    HCPointPrint(curve.p, stream);
    fprintf(stream, ">");
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCContourCurve HCContourCurveAsLinear(HCContourCurve curve) {
    if (HCContourCurveIsLinear(curve)) {
        return curve;
    }
    if (HCContourCurveIsQuadratic(curve)) {
        return HCContourCurveMakeLinear(curve.p);
    }
    return HCContourCurveMakeLinear(curve.p);
}

HCContourCurve HCContourCurveAsQuadratic(HCContourCurve curve) {
    if (HCContourCurveIsQuadratic(curve)) {
        return curve;
    }
    if (HCContourCurveIsCubic(curve)) {
        return HCContourCurveMakeQuadratic(HCPointInterpolate(curve.c0, curve.c1, 0.5), curve.p);
    }
    return HCContourCurveMakeQuadratic(curve.p, curve.p);
}

HCContourCurve HCContourCurveAsCubic(HCContourCurve curve) {
    if (HCContourCurveIsCubic(curve)) {
        return curve;
    }
    if (HCContourCurveIsQuadratic(curve)) {
        return HCContourCurveMakeCubic(curve.c0, curve.c0, curve.p);
    }
    return HCContourCurveMakeCubic(curve.p, curve.p, curve.p);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourCurveValue(HCPoint p0, HCContourCurve curve, HCReal t) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCContourCurveValueLinear(p0, curve.p, t);
        }
        return HCContourCurveValueQuadratic(p0, curve.c0, curve.p, t);
    }
    return HCContourCurveValueCubic(p0, curve.c0, curve.c1, curve.p, t);
}

HCPoint HCContourCurveValueLinear(HCPoint p0, HCPoint p1, HCReal t) {
    HCReal tc = 1.0 - t;
        
    HCReal a = tc;
    HCReal b = t;
    HCReal x = a * p0.x + b * p1.x;
    HCReal y = a * p0.y + b * p1.y;
    
    return HCPointMake(x, y);
}

HCPoint HCContourCurveValueQuadratic(HCPoint p0, HCPoint c0, HCPoint p1, HCReal t) {
    HCReal tc = 1.0 - t;
    
    HCReal a = tc * tc;
    HCReal b = 2.0 * t * tc;
    HCReal c = t * t;
    HCReal x = a * p0.x + b * c0.x + c * p1.x;
    HCReal y = a * p0.y + b * c0.y + c * p1.y;
    
    return HCPointMake(x, y);
}

HCPoint HCContourCurveValueCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t) {
    HCReal tc = 1.0 - t;
    
    HCReal a = tc * tc * tc;
    HCReal b = 3.0 * t * tc * tc;
    HCReal c = 3.0 * t * t * tc;
    HCReal d = t * t * t;
    HCReal x = a * p0.x + b * c0.x + c * c1.x + d * p1.x;
    HCReal y = a * p0.y + b * c0.y + c * c1.y + d * p1.y;
    
    return HCPointMake(x, y);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Evaluation
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveEvaluate(HCPoint p0, HCContourCurve curve, HCReal t, HCReal* x, HCReal* y, HCReal* dx, HCReal* dy, HCReal* ddx, HCReal* ddy) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCContourCurveEvaluateLinear(p0, curve.p, t, x, y);
            if (dx != NULL) {
                *dx = 0.0;
            }
            if (dy != NULL) {
                *dy = 0.0;
            }
            if (ddx != NULL) {
                *ddx = 0.0;
            }
            if (ddy != NULL) {
                *ddy = 0.0;
            }
            return;
        }
        HCContourCurveEvaluateQuadratic(p0, curve.c0, curve.p, t, x, y, dx, dy);
        if (ddx != NULL) {
            *ddx = 0.0;
        }
        if (ddy != NULL) {
            *ddy = 0.0;
        }
        return;
    }
    HCContourCurveEvaluateCubic(p0, curve.c0, curve.c1, curve.p, t, x, y, dx, dy, ddx, ddy);
}

void HCContourCurveEvaluateLinear(HCPoint p0, HCPoint p1, HCReal t, HCReal* x, HCReal* y) {
    HCReal tc = 1.0 - t;
        
    HCReal sx = p0.x * tc + p1.x * t;
    HCReal sy = p0.y * tc + p1.y * t;
        
    if (x != NULL) {
        *x = sx;
    }
    if (y != NULL) {
        *y = sy;
    }
}

void HCContourCurveEvaluateQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t, HCReal* x, HCReal* y, HCReal* dx, HCReal* dy) {
    HCReal tc = 1.0 - t;
    
    HCReal s0x = p0.x * tc + c.x * t;
    HCReal s0y = p0.y * tc + c.y * t;
    HCReal s1x = c.x * tc + p1.x * t;
    HCReal s1y = c.y * tc + p1.y * t;
    HCReal ssx = s0x * tc + s1x * t;
    HCReal ssy = s0y * tc + s1y * t;
    
    if (x != NULL) {
        *x = ssx;
    }
    if (y != NULL) {
        *y = ssy;
    }
    if (dx != NULL) {
        *dx = s1x - s0x;
    }
    if (dy != NULL) {
        *dy = s1y - s0y;
    }
}

void HCContourCurveEvaluateCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t, HCReal* x, HCReal* y, HCReal* dx, HCReal* dy, HCReal* ddx, HCReal* ddy) {
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
    HCReal sssx = ss0x * tc + ss1x * t;
    HCReal sssy = ss0y * tc + ss1y * t;
    
    if (x != NULL) {
        *x = sssx;
    }
    if (y != NULL) {
        *y = sssy;
    }
    //3((1−t)2(C0−A0)+2(1−t)t(C1−C0)+t2(A1−C1))
    if (dx != NULL) {
        *dx = ss1x - ss0x;
    }
    if (dy != NULL) {
        *dy = ss1y - ss0y;
    }
    //6((1−t)(C1−2C0+A0)+t(A1−2C1+C0))
    if (ddx != NULL) {
        *ddx = s1x - s0x;
    }
    if (ddy != NULL) {
        *ddy = s1y - s0y;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Extrema
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveExtrema(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* extrema) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCContourCurveExtremaLinear(p0, curve.p, count, extrema);
        }
        HCContourCurveExtremaQuadratic(p0, curve.c0, curve.p, count, extrema);
    }
    HCContourCurveExtremaCubic(p0, curve.c0, curve.c1, curve.p, count, extrema);
}

void HCContourCurveExtremaLinear(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* extrema) {
    if (p0.x == p1.x) {
        *count = 1;
        extrema[0] = p0.y / (p1.y - p0.y);
        return;
    }
    if (p0.y == p1.y) {
        if (p0.y == 0.0) {
            *count = 1;
            extrema[0] = 0.0;
            return;
        }
        *count = 0;
        return;
    }
    HCReal m = (p1.y - p0.y) / (p1.x - p0.x);
    HCReal b = p0.y / (m * p0.x);
    HCReal x = -m / b;
    HCReal t = x / (p1.x - p0.x);
    *count = 1;
    extrema[0] = t;
}

void HCContourCurveExtremaQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* extrema) {
    // TODO: This!
    *count = 0;
    *extrema = NAN;
}

void HCContourCurveExtremaCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* extrema) {
    // TODO: This!
    *count = 0;
    *extrema = NAN;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Bounds
//----------------------------------------------------------------------------------------------------------------------------------
HCRectangle HCContourCurveBounds(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* extrema) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCContourCurveBoundsLinear(p0, curve.p, count, extrema);
        }
        return HCContourCurveBoundsQuadratic(p0, curve.c0, curve.p, count, extrema);
    }
    return HCContourCurveBoundsCubic(p0, curve.c0, curve.c1, curve.p, count, extrema);
}

HCRectangle HCContourCurveBoundsLinear(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* extrema) {
    // TODO: This!
    return HCRectangleInvalid;
}

HCRectangle HCContourCurveBoundsQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* extrema) {
    // TODO: This!
    return HCRectangleInvalid;
}

HCRectangle HCContourCurveBoundsCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* extrema) {
    // TODO: This!
    return HCRectangleInvalid;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Length
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCContourCurveLength(HCPoint p0, HCContourCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCContourCurveLengthLinear(p0, curve.p);
        }
        return HCContourCurveLengthQuadratic(p0, curve.c0, curve.p);
    }
    return HCContourCurveLengthCubic(p0, curve.c0, curve.c1, curve.p);
}

HCReal HCContourCurveLengthLinear(HCPoint p0, HCPoint p1) {
    return NAN;
}

HCReal HCContourCurveLengthQuadratic(HCPoint p0, HCPoint c, HCPoint p1) {
    return NAN;
}

HCReal HCContourCurveLengthCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
    return NAN;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Intersection
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveIntersection(HCPoint p0, HCContourCurve pCurve, HCPoint q0, HCContourCurve qCurve, HCReal* t, HCReal* u) {
    if (HCContourCurveIsLinear(pCurve)) {
        if (HCContourCurveIsLinear(qCurve)) {
            HCContourCurveLinearLinearIntersection(p0, pCurve.p, q0, qCurve.p, t, u);
        }
        if (HCContourCurveIsQuadratic(qCurve)) {
            HCContourCurveLinearQuadraticIntersection(p0, pCurve.p, q0, qCurve.c0, qCurve.p, t, u);
        }
        HCContourCurveLinearCubicIntersection(p0, pCurve.p, q0, qCurve.c0, qCurve.c1, qCurve.p, t, u);
    }
    if (HCContourCurveIsQuadratic(pCurve)) {
        if (HCContourCurveIsLinear(qCurve)) {
            HCContourCurveLinearQuadraticIntersection(q0, qCurve.p, p0, pCurve.c0, pCurve.p, u, t);
        }
        if (HCContourCurveIsQuadratic(qCurve)) {
            HCContourCurveQuadraticQuadraticIntersection(p0, pCurve.c0, pCurve.p, q0, qCurve.c0, qCurve.p, t, u);
        }
        return HCContourCurveQuadraticCubicIntersection(p0, pCurve.c0, pCurve.p, q0, qCurve.c0, qCurve.c1, qCurve.p, t, u);
    }
    return HCContourCurveCubicCubicIntersection(p0, pCurve.c0, pCurve.c1, pCurve.p, q0, qCurve.c0, qCurve.c1, qCurve.p, t, u);
}

void HCContourCurveLinearLinearIntersection(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint q1, HCReal* t, HCReal* u) {
    HCReal d = (p0.x - p1.x) * (q0.y - q1.y) - (p0.y - p1.y) * (q0.x - q1.x);
    *t =      ((p0.x - q0.x) * (q0.y - q1.y) - (p0.y - q0.y) * (q0.x - q1.x)) / d;
    *u =     -((p0.x - p1.x) * (p0.y - q0.y) - (p0.y - p1.y) * (p0.x - q0.x)) / d;
}

void HCContourCurveLinearQuadraticIntersection(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint qc, HCPoint q1, HCReal* t, HCReal* u) {
    // TODO: This!
    *t = NAN;
    *u = NAN;
}

void HCContourCurveLinearCubicIntersection(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCReal* t, HCReal* u) {
    // TODO: This!
    *t = NAN;
    *u = NAN;
}

void HCContourCurveQuadraticQuadraticIntersection(HCPoint p0, HCPoint pc, HCPoint p1, HCPoint q0, HCPoint qc, HCPoint q1, HCReal* t, HCReal* u) {
    // TODO: This!
    *t = NAN;
    *u = NAN;
}

void HCContourCurveQuadraticCubicIntersection(HCPoint p0, HCPoint pc, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCReal* t, HCReal* u) {
    // TODO: This!
    *t = NAN;
    *u = NAN;
}

void HCContourCurveCubicCubicIntersection(HCPoint p0, HCPoint pc0, HCPoint pc1, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCReal* t, HCReal* u) {
    // TODO: This!
    *t = NAN;
    *u = NAN;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Canonical Forms
//----------------------------------------------------------------------------------------------------------------------------------

HCContourCurve HCContourCurveAlign(HCPoint p0, HCContourCurve curve) {
    // TODO: This!
    return HCContourCurveInvalid;
}

HCPoint HCContourCurveCanonical(HCPoint p0, HCContourCurve curve) {
    // TODO: This!
    return HCPointInvalid;
}

HCContourCurveType HCContourCurveCanonicalType(HCPoint p0, HCContourCurve curve) {
    // TODO: This!
    return HCContourCurveTypePoint;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Split
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveSplit(HCPoint p0, HCContourCurve curve, HCContourCurve* sCurve, HCContourCurve* eCurve) {
    // TODO: This!
    *sCurve = HCContourCurveInvalid;
    *eCurve = HCContourCurveInvalid;
}
