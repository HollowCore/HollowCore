///
/// @file HCContourCurve.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 2/11/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///
/// @see https://pomax.github.io/bezierinfo
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
// MARK: - Order
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourCurveIsLinear(HCPoint p0, HCContourCurve curve) {
    if (HCPointIsInvalid(p0) || HCPointIsInvalid(curve.p)) {
        return false;
    }
    if (HCPointIsInvalid(curve.c0) && HCPointIsInvalid(curve.c1)) {
        return true;
    }
    return false;
}

HCContourCurve HCContourCurveAsLinear(HCPoint p0, HCContourCurve curve) {
    return HCContourCurveMakeLinear(curve.p);
}

HCBoolean HCContourCurveIsQuadratic(HCPoint p0, HCContourCurve curve) {
    if (HCPointIsInvalid(p0) || HCPointIsInvalid(curve.c0) || HCPointIsInvalid(curve.p)) {
        return false;
    }
    if (HCPointIsInvalid(curve.c1)) {
        return true;
    }
    return false;
}

HCContourCurve HCContourCurveAsQuadratic(HCPoint p0, HCContourCurve curve) {
    if (HCContourCurveIsQuadratic(p0, curve)) {
        return HCContourCurveMakeQuadratic(curve.c0, curve.p);
    }
    if (HCContourCurveIsCubic(p0, curve)) {
        return HCContourCurveMakeQuadratic(HCPointInterpolate(curve.c0, curve.c1, 0.5), curve.p);
    }
    return HCContourCurveMakeQuadratic(curve.p, curve.p);
}

HCBoolean HCContourCurveIsCubic(HCPoint p0, HCContourCurve curve) {
    if (HCPointIsInvalid(p0) || HCPointIsInvalid(curve.c0) || HCPointIsInvalid(curve.c1) || HCPointIsInvalid(curve.p)) {
        return false;
    }
    return true;
}

HCContourCurve HCContourCurveAsCubic(HCPoint p0, HCContourCurve curve) {
    if (HCContourCurveIsCubic(p0, curve)) {
        return curve;
    }
    if (HCContourCurveIsQuadratic(p0, curve)) {
        HCPoint c = curve.c0;
        HCPoint p1 = curve.p;
        HCPoint c0 = HCPointMake(p0.x + (2.0/3.0) * (c.x - p0.x), p0.y + (2.0/3.0) * (c.y - p0.y));
        HCPoint c1 = HCPointMake(p1.x + (2.0/3.0) * (c.x - p1.x), p1.y + (2.0/3.0) * (c.y - p1.y));
        return HCContourCurveMakeCubic(c0, c1, p1);
    }
    return HCContourCurveMakeCubic(p0, curve.p, curve.p);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Canonical Type
//----------------------------------------------------------------------------------------------------------------------------------
HCContourCurveType HCContourCurveCanonicalType(HCPoint p0, HCContourCurve curve) {
    // Determine curve type based on provided information
    if (HCPointIsInvalid(p0) || HCPointIsInvalid(curve.p) || (HCPointIsInvalid(curve.c0) && !HCPointIsInvalid(curve.c1))) {
        // Provided curve data has invalid anchor points or has control point 1 but not control point 0
        return HCContourCurveTypeInvalid;
    }
    else if (HCPointIsInvalid(curve.c0)) {
        // Linear curve data provided
        return HCContourCurveCanonicalTypeLinear(p0, curve.p);
    }
    else if (HCPointIsInvalid(curve.c1)) {
        // Quadratic curve data provided
        return HCContourCurveCanonicalTypeQuadratic(p0, curve.c0, curve.p);
    }
    else {
        // Cubic curve data provided
        return HCContourCurveCanonicalTypeCubic(p0, curve.c0, curve.c1, curve.p);
    }
}

HCContourCurveType HCContourCurveCanonicalTypeLinear(HCPoint p0, HCPoint p1) {
    // Determine if linear curve is a point or a line segment
    return HCPointIsEqual(p0, p1) ? HCContourCurveTypePoint : HCContourCurveTypeLinear;
}

HCContourCurveType HCContourCurveCanonicalTypeQuadratic(HCPoint p0, HCPoint c, HCPoint p1) {
    // Determine if quadratic curve has degenerate points
    HCBoolean p0p1 = HCPointIsEqual(p0, p1);
    HCBoolean p0c = HCPointIsEqual(p0, c);
    HCBoolean p1c = HCPointIsEqual(p1, c);
    if (p0p1 && p0c) {
        return HCContourCurveTypePoint;
    }
    if (p0c || p1c) {
        return HCContourCurveTypeLinear;
    }
    
    // Determine if quadratic curve points are co-linear
    HCBoolean xOrdered = (p0.x <= c.x && c.x <= p1.x) || (p0.x >= c.x && c.x >= p1.x);
    if (xOrdered) {
        HCBoolean yOrdered = (p0.y <= c.y && c.y <= p1.y) || (p0.y >= c.y && c.y >= p1.y);
        if (yOrdered) {
            HCReal p0p1Slope = (p1.y - p0.y) / (p1.x - p0.x);
            HCReal p0cSlope = (c.y - p0.y) / (c.x - p0.x);
            if (p0cSlope == p0p1Slope) {
                return HCContourCurveTypeLinear;
            }
        }
    }
    
    // Curve is quadratic
    return HCContourCurveTypeQuadratic;
}

HCContourCurveType HCContourCurveCanonicalTypeCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
    // Determine if cubic curve has degenerate points
    HCBoolean p0p1 = HCPointIsEqual(p0, p1);
    HCBoolean p0c0 = HCPointIsEqual(p0, c0);
    HCBoolean p0c1 = HCPointIsEqual(p0, c1);
    HCBoolean p1c0 = HCPointIsEqual(p1, c0);
    HCBoolean p1c1 = HCPointIsEqual(p1, c1);
    HCBoolean c0c1 = HCPointIsEqual(c0, c1);
    if (p0p1 && p0c0 && p0c1) {
        return HCContourCurveTypePoint;
    }
    if ((p0c0 && p1c1) || (p0c1 && p1c0)) {
        return HCContourCurveTypeLinear;
    }
    if (p0c1 || p1c0) {
        return HCContourCurveTypeCubicSimple;
    }
    if (c0c1) {
        return HCContourCurveTypeCubicSimple;
    }
    
    // Determine if cubic curve points are co-linear
    HCBoolean xOrdered = (p0.x <= c0.x && c0.x <= c1.x && c1.x <= p1.x) || (p0.x >= c0.x && c0.x >= c1.x && c1.x >= p1.x);
    if (xOrdered) {
        HCBoolean yOrdered = (p0.y <= c0.y && c0.y <= c1.y && c1.y <= p1.y) || (p0.y >= c0.y && c0.y >= c1.y && c1.y >= p1.y);
        if (yOrdered) {
            HCReal p0p1Slope = (p1.y - p0.y) / (p1.x - p0.x);
            HCReal p0c0Slope = (c0.y - p0.y) / (c0.x - p0.x);
            if (p0c0Slope == p0p1Slope) {
                HCReal c1p1Slope = (p1.y - c1.y) / (p1.x - c1.x);
                if (c1p1Slope == p0p1Slope) {
                    return HCContourCurveTypeLinear;
                }
            }
        }
    }
    
    // Determine if control points form quadratic curve
    if (p0.x + 1.5 * (c0.x - p0.x) == p1.x + 1.5 * (c1.x - p1.x) && p0.y + 1.5 * (c0.y - p0.y) == p1.y + 1.5 * (c1.y - p1.y)) {
        return HCContourCurveTypeQuadratic;
    }
    
    // Calculate canonical curve end point (other points are p0: (0, 0), c0: (0, 1), c1: (1,1))
    HCPoint p = HCContourCurveCanonical(p0, c0, c1, p1);
    
    // Determine cubic curve type based on canonical curve end point
    // See http://graphics.pixar.com/people/derose/publications/CubicClassification/paper.pdf
    if (p.x == 1.0 && p.y == 1.0) {
        return HCContourCurveTypeCubicSimple;
    }
    if (p.x == 0.0 && p.y == 0.0) {
        return HCContourCurveTypeCubicLoopClosed;
    }
    if (p.y > 1.0) {
        return HCContourCurveTypeCubicSingleInflection;
    }
    if (p.x > 1.0) {
        return HCContourCurveTypeCubicSimple;
    }
    HCReal cuspEdge = (-1.0 * p.x * p.x + 2.0 * p.x + 3.0) * 0.25;
    if (p.y == cuspEdge) {
        return HCContourCurveTypeCubicCusp;
    }
    if (p.y > cuspEdge) {
        return HCContourCurveTypeCubicDoubleInflection;
    }
    if (p.x > 0.0) {
        HCReal t1LoopEdge = (sqrt(-3.0 * p.x * p.x + 12.0 * p.x) - p.x) * 0.5;
        if (p.y > t1LoopEdge) {
            return HCContourCurveTypeCubicLoop;
        }
        if (p.y == t1LoopEdge) {
            return HCContourCurveTypeCubicLoopAtEnd;
        }
    }
    else {
        HCReal t0LoopEdge = (-1.0 * p.x * p.x + 3.0 * p.x) * (1.0 / 3.0);
        if (p.y > t0LoopEdge) {
            return HCContourCurveTypeCubicLoop;
        }
        if (p.y == t0LoopEdge) {
            return HCContourCurveTypeCubicLoopAtStart;
        }
    }
    return HCContourCurveTypeCubicSimple;
}

HCPoint HCContourCurveCanonical(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
    // Translate curve to origin
    c0.x -= p0.x;
    c0.y -= p0.y;
    c1.x -= p0.x;
    c1.y -= p0.y;
    p1.x -= p0.x;
    p1.y -= p0.y;

    // Calculate ratio of curve y coordinates
    HCReal yci = 1.0 / c0.y;
    HCReal ycc = c1.y * yci;
    HCReal ypc = p1.y * yci;

    // Calculate canonical curve end point given p0: (0, 0), c0: (0, 1), c1: (1,1)
    HCReal p1x = (p1.x - c0.x * ypc) / (c1.x - c0.x * ycc);
    HCReal p1y = p1x * (1.0 - ycc) + ypc;
    return HCPointMake(p1x, p1y);
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
        
    // Calculate linear interpolations along linear curve at t
    HCPoint sp = HCPointMake(tc * p0.x + t * p1.x, tc * p0.y + t * p1.y);
    
    // Provide solution point and derivative at t
    if (x != NULL) {
        *x = sp.x;
    }
    if (y != NULL) {
        *y = sp.y;
    }
}

void HCContourCurveEvaluateQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t, HCReal* x, HCReal* y, HCReal* dx, HCReal* dy) {
    HCReal tc = 1.0 - t;
    
    // Calculate linear interpolations along quadratic curve anchor and control point polyline at t
    HCPoint qp0 = HCPointMake(tc *  p0.x + t *   c.x, tc *  p0.y + t *   c.y);
    HCPoint qp1 = HCPointMake(tc *   c.x + t *  p1.x, tc *   c.y + t *  p1.y);
    HCPoint  sp = HCPointMake(tc * qp0.x + t * qp1.x, tc * qp0.y + t * qp1.y);
    
    // Provide solution point and derivative at t
    if (x != NULL) {
        *x = sp.x;
    }
    if (y != NULL) {
        *y = sp.y;
    }
    if (dx != NULL) {
        *dx = qp1.x - qp0.x;
    }
    if (dy != NULL) {
        *dy = qp1.y - qp0.y;
    }
}

void HCContourCurveEvaluateCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t, HCReal* x, HCReal* y, HCReal* dx, HCReal* dy, HCReal* ddx, HCReal* ddy) {
    HCReal tc = 1.0 - t;
        
    // Calculate linear interpolations along cubic curve anchor and control point polyline at t
    HCPoint qp0 = HCPointMake(tc *  p0.x + t *  c0.x, tc *  p0.y + t *  c0.y);
    HCPoint  qc = HCPointMake(tc *  c0.x + t *  c1.x, tc *  c0.y + t *  c1.y);
    HCPoint qp1 = HCPointMake(tc *  c1.x + t *  p1.x, tc *  c1.y + t *  p1.y);
    HCPoint rp0 = HCPointMake(tc * qp0.x + t *  qc.x, tc * qp0.y + t *  qc.y);
    HCPoint rp1 = HCPointMake(tc *  qc.x + t * qp1.x, tc *  qc.y + t * qp1.y);
    HCPoint  sp = HCPointMake(tc * rp0.x + t * rp1.x, tc * rp0.y + t * rp1.y);
    
    // Provide solution point and derivatives at t
    if (x != NULL) {
        *x = sp.x;
    }
    if (y != NULL) {
        *y = sp.y;
    }
    if (dx != NULL) {
        *dx = rp1.x - rp0.x;
    }
    if (dy != NULL) {
        *dy = rp1.y - rp0.y;
    }
    if (ddx != NULL) {
        // TODO: Correct?
        *ddx = qp1.x - qp0.x;
    }
    if (ddy != NULL) {
        // TODO: Correct?
        *ddy = qp1.y - qp0.y;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Axis Alignment
//----------------------------------------------------------------------------------------------------------------------------------
HCContourCurve HCContourCurveXAxisAligned(HCPoint p0, HCContourCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
       if (HCPointIsInvalid(curve.c0)) {
           HCPoint ap1 = HCPointInvalid;
           HCContourCurveXAxisAlignedLinear(p0, curve.p, NULL, &ap1);
           return HCContourCurveMakeLinear(ap1);
       }
        HCPoint ac = HCPointInvalid;
        HCPoint ap1 = HCPointInvalid;
        HCContourCurveXAxisAlignedQuadratic(p0, curve.c0, curve.p, NULL, &ac, &ap1);
        return HCContourCurveMakeQuadratic(ac, ap1);
    }
    HCPoint ac0 = HCPointInvalid;
    HCPoint ac1 = HCPointInvalid;
    HCPoint ap1 = HCPointInvalid;
    HCContourCurveXAxisAlignedCubic(p0, curve.c0, curve.c1, curve.p, NULL, &ac0, &ac1, &ap1);
    return HCContourCurveMakeCubic(ac0, ac1, ap1);
}

void HCContourCurveXAxisAlignedLinear(HCPoint p0, HCPoint p1, HCPoint* ap0, HCPoint* ap1) {
    // Translate curve to origin
    p1.x -= p0.x;
    p1.y -= p0.y;
    
    // Calculate angle of end point
    HCReal angle = -atan2(p1.y, p1.x);
    HCReal cosAngle = cos(angle);
    HCReal sinAngle = sin(angle);
    
    // Rotate curve by angle to align the end point to the x-axis
    HCReal p1x = cosAngle * p1.x - sinAngle * p1.y;
    HCReal p1y = sinAngle * p1.x + cosAngle * p1.y;
    
    // Deliver result
    if (ap0 != NULL) {
        *ap0 = HCPointZero;
    }
    if (ap1 != NULL) {
        *ap1 = HCPointMake(p1x, p1y);
    }
}

void HCContourCurveXAxisAlignedQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint* ap0, HCPoint* ac, HCPoint* ap1) {
    // Translate curve to origin
    c.x -= p0.x;
    c.y -= p0.y;
    p1.x -= p0.x;
    p1.y -= p0.y;
    
    // Calculate angle of end point
    HCReal angle = -atan2(p1.y, p1.x);
    HCReal cosAngle = cos(angle);
    HCReal sinAngle = sin(angle);
    
    // Rotate curve by angle to align the end point to the x-axis
    HCReal cx = cosAngle * c.x - sinAngle * c.y;
    HCReal cy = sinAngle * c.x + cosAngle * c.y;
    HCReal p1x = cosAngle * p1.x - sinAngle * p1.y;
    HCReal p1y = sinAngle * p1.x + cosAngle * p1.y;
    
    // Deliver result
    if (ap0 != NULL) {
        *ap0 = HCPointZero;
    }
    if (ac != NULL) {
        *ac = HCPointMake(cx, cy);
    }
    if (ap1 != NULL) {
        *ap1 = HCPointMake(p1x, p1y);
    }
}

void HCContourCurveXAxisAlignedCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint* ap0, HCPoint* ac0, HCPoint* ac1, HCPoint* ap1) {
    // Translate curve to origin
    c0.x -= p0.x;
    c0.y -= p0.y;
    c1.x -= p0.x;
    c1.y -= p0.y;
    p1.x -= p0.x;
    p1.y -= p0.y;
    
    // Calculate angle of end point
    HCReal angle = -atan2(p1.y, p1.x);
    HCReal cosAngle = cos(angle);
    HCReal sinAngle = sin(angle);
    
    // Rotate curve by angle to align the end point to the x-axis
    HCReal c0x = cosAngle * c0.x - sinAngle * c0.y;
    HCReal c0y = sinAngle * c0.x + cosAngle * c0.y;
    HCReal c1x = cosAngle * c1.x - sinAngle * c1.y;
    HCReal c1y = sinAngle * c1.x + cosAngle * c1.y;
    HCReal p1x = cosAngle * p1.x - sinAngle * p1.y;
    HCReal p1y = sinAngle * p1.x + cosAngle * p1.y;
    
    // Deliver result
    if (ap0 != NULL) {
        *ap0 = HCPointZero;
    }
    if (ac0 != NULL) {
        *ac0 = HCPointMake(c0x, c0y);
    }
    if (ac1 != NULL) {
        *ac1 = HCPointMake(c1x, c1y);
    }
    if (ap1 != NULL) {
        *ap1 = HCPointMake(p1x, p1y);
    }
}

HCContourCurve HCContourCurveYAxisAligned(HCPoint p0, HCContourCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
       if (HCPointIsInvalid(curve.c0)) {
           HCPoint ap1 = HCPointInvalid;
           HCContourCurveYAxisAlignedLinear(p0, curve.p, NULL, &ap1);
           return HCContourCurveMakeLinear(ap1);
       }
        HCPoint ac = HCPointInvalid;
        HCPoint ap1 = HCPointInvalid;
        HCContourCurveYAxisAlignedQuadratic(p0, curve.c0, curve.p, NULL, &ac, &ap1);
        return HCContourCurveMakeQuadratic(ac, ap1);
    }
    HCPoint ac0 = HCPointInvalid;
    HCPoint ac1 = HCPointInvalid;
    HCPoint ap1 = HCPointInvalid;
    HCContourCurveYAxisAlignedCubic(p0, curve.c0, curve.c1, curve.p, NULL, &ac0, &ac1, &ap1);
    return HCContourCurveMakeCubic(ac0, ac1, ap1);
}

void HCContourCurveYAxisAlignedLinear(HCPoint p0, HCPoint p1, HCPoint* ap0, HCPoint* ap1) {
    // Align to x-axis then rotate PI/2 to y-axis
    HCPoint xAlignedP1 = HCPointInvalid;
    HCContourCurveXAxisAlignedLinear(p0, p1, NULL, &xAlignedP1);
    HCReal p1x = -xAlignedP1.y;
    HCReal p1y = +xAlignedP1.x;
    
    // Deliver result
    if (ap0 != NULL) {
        *ap0 = HCPointZero;
    }
    if (ap1 != NULL) {
        *ap1 = HCPointMake(p1x, p1y);
    }
}

void HCContourCurveYAxisAlignedQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint* ap0, HCPoint* ac, HCPoint* ap1) {
    // Align to x-axis then rotate PI/2 to y-axis
    HCPoint xAlignedC = HCPointInvalid;
    HCPoint xAlignedP1 = HCPointInvalid;
    HCContourCurveXAxisAlignedQuadratic(p0, c, p1, NULL, &xAlignedC, &xAlignedP1);
    HCReal cx = -xAlignedC.y;
    HCReal cy = +xAlignedC.x;
    HCReal p1x = -xAlignedP1.y;
    HCReal p1y = +xAlignedP1.x;
    
    // Deliver result
    if (ap0 != NULL) {
        *ap0 = HCPointZero;
    }
    if (ac != NULL) {
        *ac = HCPointMake(cx, cy);
    }
    if (ap1 != NULL) {
        *ap1 = HCPointMake(p1x, p1y);
    }
}

void HCContourCurveYAxisAlignedCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint* ap0, HCPoint* ac0, HCPoint* ac1, HCPoint* ap1) {
    // Align to x-axis then rotate PI/2 to y-axis
    HCPoint xAlignedC0 = HCPointInvalid;
    HCPoint xAlignedC1 = HCPointInvalid;
    HCPoint xAlignedP1 = HCPointInvalid;
    HCContourCurveXAxisAlignedCubic(p0, c0, c1, p1, NULL, &xAlignedC0, &xAlignedC1, &xAlignedP1);
    HCReal c0x = -xAlignedC0.y;
    HCReal c0y = +xAlignedC0.x;
    HCReal c1x = -xAlignedC1.y;
    HCReal c1y = +xAlignedC1.x;
    HCReal p1x = -xAlignedP1.y;
    HCReal p1y = +xAlignedP1.x;
    
    // Deliver result
    if (ap0 != NULL) {
        *ap0 = HCPointZero;
    }
    if (ac0 != NULL) {
        *ac0 = HCPointMake(c0x, c0y);
    }
    if (ac1 != NULL) {
        *ac1 = HCPointMake(c1x, c1y);
    }
    if (ap1 != NULL) {
        *ap1 = HCPointMake(p1x, p1y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Split
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveSplit(HCPoint p0, HCContourCurve curve, HCReal t, HCPoint* sp0, HCContourCurve* sCurve, HCPoint* ep0, HCContourCurve* eCurve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCPoint sp1 = HCPointZero;
            HCPoint ep1 = HCPointZero;
            HCContourCurveSplitLinear(p0, curve.p, t, sp0, &sp1, ep0, &ep1);
            if (sCurve != NULL) {
                *sCurve = HCContourCurveMakeLinear(sp1);
            }
            if (eCurve != NULL) {
                *eCurve = HCContourCurveMakeLinear(ep1);
            }
        }
        else {
            HCPoint sc = HCPointZero;
            HCPoint sp1 = HCPointZero;
            HCPoint ec = HCPointZero;
            HCPoint ep1 = HCPointZero;
            HCContourCurveSplitQuadratic(p0, curve.c0, curve.p, t, sp0, &sc, &sp1, ep0, &ec, &ep1);
            if (sCurve != NULL) {
                *sCurve = HCContourCurveMakeQuadratic(sc, sp1);
            }
            if (eCurve != NULL) {
                *eCurve = HCContourCurveMakeQuadratic(ec, ep1);
            }
        }
    }
    else {
        HCPoint sc0 = HCPointZero;
        HCPoint sc1 = HCPointZero;
        HCPoint sp1 = HCPointZero;
        HCPoint ec0 = HCPointZero;
        HCPoint ec1 = HCPointZero;
        HCPoint ep1 = HCPointZero;
        HCContourCurveSplitCubic(p0, curve.c0, curve.c1, curve.p, t, sp0, &sc0, &sc1, &sp1, ep0, &ec0, &ec1, &ep1);
        if (sCurve != NULL) {
            *sCurve = HCContourCurveMakeCubic(sc0, sc1, sp1);
        }
        if (eCurve != NULL) {
            *eCurve = HCContourCurveMakeCubic(ec0, ec1, ep1);
        }
    }
}

void HCContourCurveSplitLinear(HCPoint p0, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sp1, HCPoint* ep0, HCPoint* ep1) {
    HCReal tc = 1.0 - t;
    
    // Calculate linear interpolations along linear curve at t
    HCPoint  sp = HCPointMake(tc * p0.x + t * p1.x, tc * p0.y + t * p1.y);
    
    // Provide intermediates that correspond to a t0-side split
    *sp0 = p0;
    *sp1 = sp;
    
    // Provide intermediates that correspond to a t1-side split
    *ep0 = sp;
    *ep1 = p1;
}

void HCContourCurveSplitQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sc, HCPoint* sp1, HCPoint* ep0, HCPoint* ec, HCPoint* ep1) {
    HCReal tc = 1.0 - t;
    
    // Calculate linear interpolations along quadratic curve anchor and control point polyline at t
    HCPoint qp0 = HCPointMake(tc *  p0.x + t *   c.x, tc *  p0.y + t *   c.y);
    HCPoint qp1 = HCPointMake(tc *   c.x + t *  p1.x, tc *   c.y + t *  p1.y);
    HCPoint  sp = HCPointMake(tc * qp0.x + t * qp1.x, tc * qp0.y + t * qp1.y);
    
    // Provide intermediates that correspond to a t0-side split
    *sp0 = p0;
    *sc = qp0;
    *sp1 = sp;
    
    // Provide intermediates that correspond to a t1-side split
    *ep0 = sp;
    *ec = qp1;
    *ep1 = p1;
}

void HCContourCurveSplitCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sc0, HCPoint* sc1, HCPoint* sp1, HCPoint* ep0, HCPoint* ec0, HCPoint* ec1, HCPoint* ep1) {
    HCReal tc = 1.0 - t;
    
    // Calculate linear interpolations along cubic curve anchor and control point polyline at t
    HCPoint qp0 = HCPointMake(tc *  p0.x + t *  c0.x, tc *  p0.y + t *  c0.y);
    HCPoint  qc = HCPointMake(tc *  c0.x + t *  c1.x, tc *  c0.y + t *  c1.y);
    HCPoint qp1 = HCPointMake(tc *  c1.x + t *  p1.x, tc *  c1.y + t *  p1.y);
    HCPoint rp0 = HCPointMake(tc * qp0.x + t *  qc.x, tc * qp0.y + t *  qc.y);
    HCPoint rp1 = HCPointMake(tc *  qc.x + t * qp1.x, tc *  qc.y + t * qp1.y);
    HCPoint  sp = HCPointMake(tc * rp0.x + t * rp1.x, tc * rp0.y + t * rp1.y);
    
    // Provide intermediates that correspond to a t0-side split
    *sp0 = p0;
    *sc0 = qp0;
    *sc1 = rp0;
    *sp1 = sp;
    
    // Provide intermediates that correspond to a t1-side split
    *ep0 = sp;
    *ec0 = rp1;
    *ec1 = qp1;
    *ep1 = p1;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Curvature
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCContourCurveCurvature(HCPoint p0, HCContourCurve curve, HCReal t) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCContourCurveCurvatureLinear(p0, curve.p, t);
        }
        return HCContourCurveCurvatureQuadratic(p0, curve.c0, curve.p, t);
    }
    return HCContourCurveCurvatureCubic(p0, curve.c0, curve.c1, curve.p, t);
}

HCReal HCContourCurveCurvatureLinear(HCPoint p0, HCPoint p1, HCReal t) {
    // Linear curve has zero curvature
    return 0.0;
}

HCReal HCContourCurveCurvatureQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t) {
    // TODO: Second derivative is zero. How does this work?!?
//    HCReal dx = 0.0;
//    HCReal dy = 0.0;
//    HCContourCurveEvaluateQuadratic(p0, c, p1, t, NULL, NULL, &dx, &dy);
//    HCReal numerator = dx * ddy - ddx * dy
//    HCReal denominator = pow(dx*dx + dy*dy, 1.5)
//    return numerator / denominator
    return NAN;
}

HCReal HCContourCurveCurvatureCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t) {
    HCReal dx = 0.0;
    HCReal dy = 0.0;
    HCReal ddx = 0.0;
    HCReal ddy = 0.0;
    HCContourCurveEvaluateCubic(p0, c0, c1, p1, t, NULL, NULL, &dx, &dy, &ddx, &ddy);
    HCReal numerator = dx * ddy - ddx * dy;
    HCReal denominator = pow(dx*dx + dy*dy, 1.5);
    return numerator / denominator;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Derivative
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveDerivative(HCPoint p0, HCContourCurve curve, HCPoint* dP0, HCContourCurve* dCurve) {
    HCContourCurveType type = HCContourCurveCanonicalType(p0, curve);
    
    switch (type) {
        case HCContourCurveTypeInvalid: {
            if (dP0 != NULL) {
                *dP0 = HCPointInvalid;
            }
            if (dCurve != NULL) {
                *dCurve = HCContourCurveInvalid;
            }
        } break;
        case HCContourCurveTypePoint:
        case HCContourCurveTypeLinear: {
            // Calculate linear derivative weights
            HCPoint p1 = curve.p;
            HCPoint pd = HCPointMake(1.0 * (p1.x - p0.x), 1.0 * (p1.y - p0.y));
            if (dP0 != NULL) {
                *dP0 = pd;
            }
            if (dCurve != NULL) {
                *dCurve = HCContourCurveMakeLinear(pd);
            }
        } break;
        case HCContourCurveTypeQuadratic: {
            // Calculate quadratic derivative weights
            HCPoint c = curve.c0;
            HCPoint p1 = curve.p;
            HCPoint p0d = HCPointMake(2.0 * (c.x - p0.x), 2.0 * (c.y - p0.y));
            HCPoint p1d = HCPointMake(2.0 * (p1.x - c.x), 2.0 * (p1.y - c.y));
            if (dP0 != NULL) {
                *dP0 = p0d;
            }
            if (dCurve != NULL) {
                *dCurve = HCContourCurveMakeLinear(p1d);
            }
        } break;
        case HCContourCurveTypeCubicSimple:
        case HCContourCurveTypeCubicSingleInflection:
        case HCContourCurveTypeCubicDoubleInflection:
        case HCContourCurveTypeCubicLoop:
        case HCContourCurveTypeCubicLoopAtStart:
        case HCContourCurveTypeCubicLoopAtEnd:
        case HCContourCurveTypeCubicLoopClosed:
        case HCContourCurveTypeCubicCusp: {
            // Calculate cubic derivative weights
            HCPoint c0 = curve.c0;
            HCPoint c1 = curve.c1;
            HCPoint p1 = curve.p;
            HCPoint p0d = HCPointMake(3.0 * (c0.x - p0.x), 3.0 * (c0.y - p0.y));
            HCPoint  cd = HCPointMake(3.0 * (c1.x - c0.x), 3.0 * (c1.y - c0.y));
            HCPoint p1d = HCPointMake(3.0 * (p1.x - c1.x), 3.0 * (p1.y - c1.y));
            if (dP0 != NULL) {
                *dP0 = p0d;
            }
            if (dCurve != NULL) {
                *dCurve = HCContourCurveMakeQuadratic(cd, p1d);
            }
        } break;
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
    // Linear curve has no extrema
    if (count != NULL) {
        *count = 0;
    }
}

void HCContourCurveExtremaQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* extrema) {
    HCInteger tCount = 0;
    HCReal t[2];
    
    // Calculate quadratic derivative weights
    HCPoint p0d = HCPointMake(2.0 * (c.x - p0.x), 2.0 * (c.y - p0.y));
    HCPoint p1d = HCPointMake(2.0 * (p1.x - c.x), 2.0 * (p1.y - c.y));
    
    // Calculate zero crossing of derivative in x and add as extrema if in range
    HCReal ax = -1.0 * p0d.x + 1.0 * p1d.x;
    HCReal bx = +1.0 * p0d.x;
    HCReal xt = -bx / ax;
    if (xt >= 0.0 && xt <= 1.0) {
        t[tCount++] = xt;
    }
    
    // Calculate zero crossing of derivative in y and add as extrema if in range
    HCReal ay = -1.0 * p0d.y + 1.0 * p1d.y;
    HCReal by = +1.0 * p0d.y;
    HCReal yt = -by / ay;
    if (yt >= 0.0 && yt <= 1.0) {
        t[tCount++] = yt;
    }
    
    // Deliver results
    if (count != NULL) {
        *count = tCount;
    }
    if (extrema != NULL) {
        memcpy(extrema, t, tCount * sizeof(HCReal));
    }
}

void HCContourCurveExtremaCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* extrema) {
    HCInteger tCount = 0;
    HCReal t[6];
    
    // Calculate cubic derivative weights
    HCPoint p0d = HCPointMake(3.0 * (c0.x - p0.x), 3.0 * (c0.y - p0.y));
    HCPoint  cd = HCPointMake(3.0 * (c1.x - c0.x), 3.0 * (c1.y - c0.y));
    HCPoint p1d = HCPointMake(3.0 * (p1.x - c1.x), 3.0 * (p1.y - c1.y));
    
    // Calculate zero crossing of derivative in x and add as extrema if real and in range
    HCReal ax = +1.0 * p0d.x - 2.0 * cd.x + 1.0 * p1d.x;
    HCReal bx = -2.0 * p0d.x + 2.0 * cd.x;
    HCReal cx = +1.0 * p0d.x;
    HCReal discriminantX = bx * bx - 4.0 * ax * cx;
    if (discriminantX >= 0.0) {
        HCReal sqrtDiscriminant = sqrt(discriminantX);
        HCReal denominatorInverse = 1.0 / (2.0 * ax);
        HCReal xt0 = (-bx + sqrtDiscriminant) * denominatorInverse;
        if (xt0 >= 0.0 && xt0 <= 1.0) {
            t[tCount++] = xt0;
        }
        HCReal xt1 = (-bx - sqrtDiscriminant) * denominatorInverse;
        if (xt1 >= 0.0 && xt1 <= 1.0) {
            t[tCount++] = xt1;
        }
    }
    
    // Calculate zero crossing of derivative in y and add as extrema if real and in range
    HCReal ay = +1.0 * p0d.y - 2.0 * cd.y + 1.0 * p1d.y;
    HCReal by = -2.0 * p0d.y + 2.0 * cd.y;
    HCReal cy = +1.0 * p0d.y;
    HCReal discriminantY = by * by - 4.0 * ay * cy;
    if (discriminantY >= 0.0) {
        HCReal sqrtDiscriminant = sqrt(discriminantY);
        HCReal denominatorInverse = 1.0 / (2.0 * ay);
        HCReal yt0 = (-by + sqrtDiscriminant) * denominatorInverse;
        if (yt0 >= 0.0 && yt0 <= 1.0) {
            t[tCount++] = yt0;
        }
        HCReal yt1 = (-by - sqrtDiscriminant) * denominatorInverse;
        if (yt1 >= 0.0 && yt1 <= 1.0) {
            t[tCount++] = yt1;
        }
    }
    
    // Add extrema of second derivative
    HCInteger quadraticCount = 0;
    HCContourCurveExtremaQuadratic(p0d, cd, p1d, &quadraticCount, &t[tCount]);
    tCount += quadraticCount;
    
    // Deliver results
    if (count != NULL) {
        *count = tCount;
    }
    if (extrema != NULL) {
        memcpy(extrema, t, tCount * sizeof(HCReal));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Inflection
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveInflections(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* inflections) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCContourCurveInflectionsLinear(p0, curve.p, count, inflections);
        }
        HCContourCurveInflectionsQuadratic(p0, curve.c0, curve.p, count, inflections);
    }
    HCContourCurveInflectionsCubic(p0, curve.c0, curve.c1, curve.p, count, inflections);
}

void HCContourCurveInflectionsLinear(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* inflections) {
    // Linear curves have no inflections
    if (count != NULL) {
        *count = 0;
    }
}

void HCContourCurveInflectionsQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* inflections) {
    // Quadratic curves have no inflections
    if (count != NULL) {
        *count = 0;
    }
}

void HCContourCurveInflectionsCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* inflections) {
    // Axis-align the curve to make computation of inflections more straightforward
    HCContourCurve aligned = HCContourCurveXAxisAligned(p0, HCContourCurveMakeCubic(c0, c1, p1));
    p0 = HCPointZero;
    c0 = aligned.c0;
    c1 = aligned.c1;
    p1 = aligned.p;
    
    // Compute zero-crossings of curvature given curve is axis-aligned
    HCReal a = c1.x * c0.y;
    HCReal b = p1.x * c0.y;
    HCReal c = c0.x * c1.y;
    HCReal d = p1.x * c1.y;
    HCReal x = 18.0 * (-3.0 * a + 2.0 * b + 3.0 * c - d);
    HCReal y = 18.0 * (+3.0 * a - 1.0 * b - 3.0 * c);
    HCReal z = 18.0 * (-1.0 * a + 0.0 * b + 1.0 * c);
    HCReal determinant = y * y - 4.0 * x * z;
    if (determinant < 0.0) {
        if (count != NULL) {
            *count = 0;
        }
    }
    HCReal sqrtDeterminant = sqrt(determinant);
    HCReal denominatorInverse = 1.0 / (2.0 * x);
    HCReal inflection0 = (-y + sqrtDeterminant) * denominatorInverse;
    HCReal inflection1 = (-y - sqrtDeterminant) * denominatorInverse;
    
    // Determine if inflections occur in-range and deliver them
    HCInteger inflectionCount = 0;
    if (inflection0 >= 0.0 && inflection0 <= 1.0) {
        if (inflections != NULL) {
            inflections[inflectionCount] = inflection0;
        }
        inflectionCount++;
    }
    if (inflection1 >= 0.0 && inflection1 <= 1.0) {
        if (inflections != NULL) {
            inflections[inflectionCount] = inflection1;
        }
        inflectionCount++;
    }
    if (count != NULL) {
        *count = inflectionCount;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Approximate Bounds
//----------------------------------------------------------------------------------------------------------------------------------
HCRectangle HCContourCurveApproximateBounds(HCPoint p0, HCContourCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCContourCurveApproximateBoundsLinear(p0, curve.p);
        }
        return HCContourCurveApproximateBoundsQuadratic(p0, curve.c0, curve.p);
    }
    return HCContourCurveApproximateBoundsCubic(p0, curve.c0, curve.c1, curve.p);
}

HCRectangle HCContourCurveApproximateBoundsLinear(HCPoint p0, HCPoint p1) {
    return HCRectangleMakeWithEdges(
        fmin(p0.x, p1.x),
        fmin(p0.y, p1.y),
        fmax(p0.x, p1.x),
        fmax(p0.y, p1.y));
}

HCRectangle HCContourCurveApproximateBoundsQuadratic(HCPoint p0, HCPoint c, HCPoint p1) {
    return HCRectangleMakeWithEdges(
        fmin(p0.x, fmin(c.x, p1.x)),
        fmin(p0.y, fmin(c.y, p1.y)),
        fmax(p0.x, fmax(c.x, p1.x)),
        fmax(p0.y, fmax(c.y, p1.y)));
}

HCRectangle HCContourCurveApproximateBoundsCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
    return HCRectangleMakeWithEdges(
        fmin(p0.x, fmin(c0.x, fmin(c1.x, p1.x))),
        fmin(p0.y, fmin(c0.y, fmin(c1.y, p1.y))),
        fmax(p0.x, fmax(c0.x, fmax(c1.x, p1.x))),
        fmax(p0.y, fmax(c0.y, fmax(c1.y, p1.y))));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Bounds
//----------------------------------------------------------------------------------------------------------------------------------
HCRectangle HCContourCurveBounds(HCPoint p0, HCContourCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCContourCurveBoundsLinear(p0, curve.p);
        }
        return HCContourCurveBoundsQuadratic(p0, curve.c0, curve.p);
    }
    return HCContourCurveBoundsCubic(p0, curve.c0, curve.c1, curve.p);
}

HCRectangle HCContourCurveBoundsLinear(HCPoint p0, HCPoint p1) {
    // Find min/max of end points
    HCReal minX = fmin(p0.x, p1.x);
    HCReal minY = fmin(p0.y, p1.y);
    HCReal maxX = fmax(p0.x, p1.x);
    HCReal maxY = fmax(p0.y, p1.y);
    
    // Calculate bounding rectangle from min/max of end points
    return HCRectangleMakeWithEdges(minX, minY, maxX, maxY);
}

HCRectangle HCContourCurveBoundsQuadratic(HCPoint p0, HCPoint c, HCPoint p1) {
    // Calculate quadratic curve extrema
    HCReal extrema[2];
    HCInteger count = 0;
    HCContourCurveExtremaQuadratic(p0, c, p1, &count, extrema);
    
    // Find min/max of extrema and end points
    HCReal minX = fmin(p0.x, p1.x);
    HCReal minY = fmin(p0.y, p1.y);
    HCReal maxX = fmax(p0.x, p1.x);
    HCReal maxY = fmax(p0.y, p1.y);
    for (HCInteger i = 0; i < count; i++) {
        HCReal t = extrema[i];
        HCPoint p = HCContourCurveValueQuadratic(p0, c, p1, t);
        minX = fmin(minX, p.x);
        minY = fmin(minY, p.y);
        maxX = fmax(maxX, p.x);
        maxY = fmax(maxY, p.y);
    }
    
    // Calculate bounding rectangle from min/max of extrema and end points
    return HCRectangleMakeWithEdges(minX, minY, maxX, maxY);
}

HCRectangle HCContourCurveBoundsCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
    // Calculate cubic curve extrema
    HCReal extrema[6];
    HCInteger count = 0;
    HCContourCurveExtremaCubic(p0, c0, c1, p1, &count, extrema);
    
    // Find min/max of extrema and end points
    HCReal minX = fmin(p0.x, p1.x);
    HCReal minY = fmin(p0.y, p1.y);
    HCReal maxX = fmax(p0.x, p1.x);
    HCReal maxY = fmax(p0.y, p1.y);
    for (HCInteger i = 0; i < count; i++) {
        HCReal t = extrema[i];
        HCPoint p = HCContourCurveValueCubic(p0, c0, c1, p1, t);
        minX = fmin(minX, p.x);
        minY = fmin(minY, p.y);
        maxX = fmax(maxX, p.x);
        maxY = fmax(maxY, p.y);
    }
    
    // Calculate bounding rectangle from min/max of extrema and end points
    return HCRectangleMakeWithEdges(minX, minY, maxX, maxY);
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
    // Calculate using linear distance equation
    return HCPointDistance(p0, p1);
}

HCReal HCContourCurveLengthQuadratic(HCPoint p0, HCPoint c, HCPoint p1) {
    // Calculate using closed-form quadratic bezier solution
    // See https://malczak.linuxpl.com/blog/quadratic-bezier-curve-length
    HCReal ax = p0.x - 2.0 * c.x + p1.x;
    HCReal ay = p0.y - 2.0 * c.y + p1.y;
    HCReal bx = 2.0 * c.x - 2.0 * p0.x;
    HCReal by = 2.0 * c.y - 2.0 * p0.y;
    HCReal A = 4.0 * (ax * ax + ay * ay);
    HCReal B = 4.0 * (ax * bx + ay * by);
    HCReal C = bx * bx + by * by;

    HCReal Sabc = 2.0 * sqrt(A+B+C);
    HCReal Sa = sqrt(A);
    HCReal Saa = 2.0 * A * Sa;
    HCReal Sc = 2.0 * sqrt(C);
    HCReal BSa = B / Sa;

    return (Saa * Sabc + Sa * B * (Sabc - Sc) + (4.0 * C * A - B * B) * log((2.0 * Sa + BSa + Sabc) / (BSa + Sc))) / (4.0 * Saa);
}

HCReal HCContourCurveLengthCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
    // TODO: Use Legendre-Gauss to numerically calculate length
    // Calculate using polyline
    HCReal length = 0.0;
    HCPoint ps = p0;
    for (HCReal t = 0.0; t <= 1.0; t += 0.01) {
        HCPoint pe = HCContourCurveValueCubic(p0, c0, c1, p1, t);
        HCReal segmentLength = HCPointDistance(ps, pe);
        length += segmentLength;
        ps = pe;
    }
    return length;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameterization by Arc Length
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCContourCurveParameter(HCPoint p0, HCContourCurve curve, HCReal d) {
    // TODO: This!
    return NAN;
}

HCReal HCContourCurveParameterLinear(HCPoint p0, HCPoint p1, HCReal d) {
    // TODO: This!
    return NAN;
}

HCReal HCContourCurveParameterQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal d) {
    // TODO: This!
    return NAN;
}

HCReal HCContourCurveParameterCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal d) {
    // TODO: This!
    return NAN;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameter Given Point
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCContourCurveParameterNearestPoint(HCPoint p0, HCContourCurve curve, HCPoint p) {
    // TODO: This!
    return NAN;
}

HCReal HCContourCurveParameterNearestPointLinear(HCPoint p0, HCPoint p1, HCPoint p) {
    // TODO: This!
    return NAN;
}

HCReal HCContourCurveParameterNearestPointQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint p) {
    // TODO: This!
    return NAN;
}

HCReal HCContourCurveParameterNearestPointCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint p) {
    // TODO: This!
    return NAN;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameter Given Axis
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveParametersFromXAxis(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* ts) {
    // TODO: This!
}

void HCContourCurveParametersFromYAxis(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* ts) {
    // TODO: This!
}

void HCContourCurveParametersLinearFromXAxis(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* ts) {
    // TODO: This!
}

void HCContourCurveParametersLinearFromYAxis(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* ts) {
    // TODO: This!
}

void HCContourCurveParametersQuadraticFromXAxis(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* ts) {
    // TODO: This!
}

void HCContourCurveParametersQuadraticFromYAxis(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* ts) {
    // TODO: This!
}

void HCContourCurveParametersCubicFromXAxis(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* ts) {
    // TODO: This!
}

void HCContourCurveParametersCubicFromYAxis(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* ts) {
    // TODO: This!
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Projection
//----------------------------------------------------------------------------------------------------------------------------------

HCPoint HCContourCurveProjection(HCPoint p0, HCContourCurve curve) {
    // TODO: This!
    return HCPointInvalid;
}

HCPoint HCContourCurveProjectionLinear(HCPoint p0, HCPoint p1) {
    // TODO: This!
    return HCPointInvalid;
}

HCPoint HCContourCurveProjectionQuadratic(HCPoint p0, HCPoint c, HCPoint p1) {
    // TODO: This!
    return HCPointInvalid;
}

HCPoint HCContourCurveProjectionCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
    // TODO: This!
    return HCPointInvalid;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Moulding
//----------------------------------------------------------------------------------------------------------------------------------

HCContourCurve HCContourCurveMould(HCPoint p0, HCContourCurve curve, HCPoint p, HCReal t) {
    // TODO: This!
    return HCContourCurveInvalid;
}

HCContourCurve HCContourCurveMouldLinear(HCPoint p0, HCPoint p1, HCPoint p, HCReal t) {
    // TODO: This!
    return HCContourCurveInvalid;
}

HCContourCurve HCContourCurveMouldQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint p, HCReal t) {
    // TODO: This!
    return HCContourCurveInvalid;
}

HCContourCurve HCContourCurveMouldCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint p, HCReal t) {
    // TODO: This!
    return HCContourCurveInvalid;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Interpolation
//----------------------------------------------------------------------------------------------------------------------------------

HCContourCurve HCContourCurveInterpolating(HCPoint p0, HCPoint p1, HCPoint p, HCReal t) {
    // TODO: This!
    return HCContourCurveInvalid;
}

HCContourCurve HCContourCurveInterpolatingLinear(HCPoint p0, HCPoint p, HCReal t) {
    // TODO: This!
    return HCContourCurveInvalid;
}

HCContourCurve HCContourCurveInterpolatingQuadratic(HCPoint p0, HCPoint p1, HCPoint p, HCReal t) {
    // TODO: This!
    return HCContourCurveInvalid;
}

HCContourCurve HCContourCurveInterpolatingCubic(HCPoint p0, HCPoint p1, HCPoint p, HCReal t, HCReal dx, HCReal dy) {
    // TODO: This!
    return HCContourCurveInvalid;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Fitting
//----------------------------------------------------------------------------------------------------------------------------------

void HCContourCurveFitting(HCInteger count, const HCPoint* points, HCPoint* p0, HCContourCurve* curve) {
    // TODO: This!
    *p0 = HCPointInvalid;
    *curve = HCContourCurveInvalid;
}

void HCContourCurveFittingLinear(HCInteger count, const HCPoint* points, HCPoint* p0, HCContourCurve* curve) {
    // TODO: This!
    *p0 = HCPointInvalid;
    *curve = HCContourCurveInvalid;
}

void HCContourCurveFittingQuadratic(HCInteger count, const HCPoint* points, HCPoint* p0, HCContourCurve* curve) {
    // TODO: This!
    *p0 = HCPointInvalid;
    *curve = HCContourCurveInvalid;
}

void HCContourCurveFittingCubic(HCInteger count, const HCPoint* points, HCPoint* p0, HCContourCurve* curve) {
    // TODO: This!
    *p0 = HCPointInvalid;
    *curve = HCContourCurveInvalid;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Intersection
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveIntersection(HCPoint p0, HCContourCurve pCurve, HCPoint q0, HCContourCurve qCurve, HCInteger* count, HCReal* t, HCReal* u) {
    if (HCContourCurveIsLinear(p0, pCurve)) {
        if (HCContourCurveIsLinear(p0, qCurve)) {
            HCContourCurveIntersectionLinearLinear(p0, pCurve.p, q0, qCurve.p, count, t, u);
        }
        if (HCContourCurveIsQuadratic(p0, qCurve)) {
            HCContourCurveIntersectionLinearQuadratic(p0, pCurve.p, q0, qCurve.c0, qCurve.p, count, t, u);
        }
        HCContourCurveIntersectionLinearCubic(p0, pCurve.p, q0, qCurve.c0, qCurve.c1, qCurve.p, count, t, u);
    }
    if (HCContourCurveIsQuadratic(p0, pCurve)) {
        if (HCContourCurveIsLinear(p0, qCurve)) {
            HCContourCurveIntersectionLinearQuadratic(q0, qCurve.p, p0, pCurve.c0, pCurve.p, count, t, u);
        }
        if (HCContourCurveIsQuadratic(p0, qCurve)) {
            HCContourCurveIntersectionQuadraticQuadratic(p0, pCurve.c0, pCurve.p, q0, qCurve.c0, qCurve.p, count, t, u);
        }
        return HCContourCurveIntersectionQuadraticCubic(p0, pCurve.c0, pCurve.p, q0, qCurve.c0, qCurve.c1, qCurve.p, count, t, u);
    }
    return HCContourCurveIntersectionCubicCubic(p0, pCurve.c0, pCurve.c1, pCurve.p, q0, qCurve.c0, qCurve.c1, qCurve.p, count, t, u);
}

void HCContourCurveIntersectionLinearLinear(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u) {
    // Find intersection of lines formed by linear curve anchor points
    HCReal d = (p0.x - p1.x) * (q0.y - q1.y) - (p0.y - p1.y) * (q0.x - q1.x);
    if (d == 0) {
        if (count != NULL) {
            *count = 0;
        }
        return;
    }
    
    HCReal intersectionT = ((p0.x - q0.x) * (q0.y - q1.y) - (p0.y - q0.y) * (q0.x - q1.x)) / d;
    HCReal intersectionU = -((p0.x - p1.x) * (p0.y - q0.y) - (p0.y - p1.y) * (p0.x - q0.x)) / d;
        
    // Validate intersection for use as t and u values
    if (intersectionT < 0.0 || intersectionT > 1.0 || intersectionU < 0.0 || intersectionU > 1.0) {
        if (count != NULL) {
            *count = 0;
        }
    }
    else {
        if (count != NULL) {
            *count = 1;
        }
        if (t != NULL) {
            *t = intersectionT;
        }
        if (u != NULL) {
            *u = intersectionU;
        }
    }
}

void HCContourCurveIntersectionLinearQuadratic(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint qc, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u) {
    // Translate points so the linear curve t0 anchor point is at the origin
    q0 = HCPointOffset(q0, -p0.x, -p0.y);
    qc = HCPointOffset(qc, -p0.x, -p0.y);
    q1 = HCPointOffset(q1, -p0.x, -p0.y);
    p1 = HCPointOffset(p1, -p0.x, -p0.y);
    p0 = HCPointZero;
    
    // Find the angle that rotates the linear curve to the x-axis
    HCReal angle = -atan2(p1.y, p1.x);
    HCReal cosAngle = cos(angle);
    HCReal sinAngle = sin(angle);
    
    // Rotate the quadratic curve by the angle to align it to the linear curve
    HCReal q0x = cosAngle * q0.x - sinAngle * q0.y;
    HCReal q0y = sinAngle * q0.x + cosAngle * q0.y;
    q0 = HCPointMake(q0x, q0y);
    HCReal qcx = cosAngle * qc.x - sinAngle * qc.y;
    HCReal qcy = sinAngle * qc.x + cosAngle * qc.y;
    qc = HCPointMake(qcx, qcy);
    HCReal q1x = cosAngle * q1.x - sinAngle * q1.y;
    HCReal q1y = sinAngle * q1.x + cosAngle * q1.y;
    q1 = HCPointMake(q1x, q1y);
    HCReal p1x = cosAngle * p1.x - sinAngle * p1.y;
    HCReal p1y = sinAngle * p1.x + cosAngle * p1.y;
    p1 = HCPointMake(p1x, p1y);
    
    // Calculate zero crossing of the quadratic's y component function, as these are the intersection points with the curves aligned to the x-axis
    HCReal a = +1.0 * q0.y - 2.0 * qc.y + 1.0 * q1.y;
    HCReal b = -2.0 * q0.y + 2.0 * qc.y;
    HCReal c = +1.0 * q0.y;
    HCReal roots[2] = { NAN, NAN };
    HCReal discriminant = b * b - 4.0 * a * c;
    if (discriminant >= 0.0) {
        HCReal sqrtDiscriminant = sqrt(discriminant);
        HCReal denominatorInverse = 1.0 / (2.0 * a);
        roots[0] = (-b + sqrtDiscriminant) * denominatorInverse;
        roots[1] = (-b - sqrtDiscriminant) * denominatorInverse;
    }
    
    // Validate roots for use as t and u values
    HCInteger rCount = 0;
    HCReal ts[2];
    HCReal us[2];
    for (HCInteger rootIndex = 0; rootIndex < 2; rootIndex++) {
        HCReal root = roots[rootIndex];
        if (isnan(root)) {
            continue;
        }
        if (root < 0.0 || root > 1.0) {
            continue;
        }
        HCPoint intersection = HCContourCurveValueQuadratic(q0, qc, q1, root);
        HCReal intersectionT = intersection.x / p1.x;
        if (intersectionT < 0.0 || intersectionT > 1.0) {
            continue;
        }
        ts[rCount] = intersectionT;
        us[rCount] = root;
        rCount++;
    }
    
    // Deliver results
    if (count != NULL) {
        *count = rCount;
    }
    if (t != NULL) {
        memcpy(t, ts, rCount * sizeof(HCReal));
    }
    if (u != NULL) {
        memcpy(u, us, rCount * sizeof(HCReal));
    }
}

void HCContourCurveIntersectionLinearCubic(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u) {
    // Translate points so the linear curve t0 anchor point is at the origin
    q0 = HCPointOffset(q0, -p0.x, -p0.y);
    qc0 = HCPointOffset(qc0, -p0.x, -p0.y);
    qc1 = HCPointOffset(qc1, -p0.x, -p0.y);
    q1 = HCPointOffset(q1, -p0.x, -p0.y);
    p1 = HCPointOffset(p1, -p0.x, -p0.y);
    p0 = HCPointZero;
    
    // Find the angle that rotates the linear curve to the x-axis
    HCReal angle = -atan2(p1.y, p1.x);
    HCReal cosAngle = cos(angle);
    HCReal sinAngle = sin(angle);
    
    // Rotate the cubic curve by the angle to align it to the linear curve
    HCReal q0x = cosAngle * q0.x - sinAngle * q0.y;
    HCReal q0y = sinAngle * q0.x + cosAngle * q0.y;
    q0 = HCPointMake(q0x, q0y);
    HCReal qc0x = cosAngle * qc0.x - sinAngle * qc0.y;
    HCReal qc0y = sinAngle * qc0.x + cosAngle * qc0.y;
    qc0 = HCPointMake(qc0x, qc0y);
    HCReal qc1x = cosAngle * qc1.x - sinAngle * qc1.y;
    HCReal qc1y = sinAngle * qc1.x + cosAngle * qc1.y;
    qc1 = HCPointMake(qc1x, qc1y);
    HCReal q1x = cosAngle * q1.x - sinAngle * q1.y;
    HCReal q1y = sinAngle * q1.x + cosAngle * q1.y;
    q1 = HCPointMake(q1x, q1y);
    HCReal p1x = cosAngle * p1.x - sinAngle * p1.y;
    HCReal p1y = sinAngle * p1.x + cosAngle * p1.y;
    p1 = HCPointMake(p1x, p1y);
    
    // Calculate zero crossing of the cubic's y component function, as these are the intersection points with the curves aligned to the x-axis
//    HCReal a = -1.0 * q0.y + 1.0 * qc0.y - 1.0 * qc1.y + 1.0 * q1.y;
//    HCReal b = +3.0 * q0.y - 2.0 * qc0.y + 1.0 * qc1.y;
//    HCReal c = -3.0 * q0.y + 1.0 * qc0.y;
//    HCReal d = +1.0 * q0.y;
    HCReal a = +3.0 * q0.y - 6.0 * qc0.y + 3.0 * qc1.y;
    HCReal b = -3.0 * q0.y + 3.0 * qc0.y;
    HCReal c = +1.0 * q0.y;
    HCReal d = -1.0 * q0.y + 3.0 * qc0.y - 3.0 * qc1.y + 1.0 * q1.y;
    HCReal roots[3] = { NAN, NAN, NAN };
    if (d == 0.0 && a != 0.0 && b != 0.0) {
        // Component function is quadratic, so solve that for t=0 using quadratic equation
        HCReal discriminant = b * b - 4.0 * a * c;
        if (discriminant >= 0.0) {
            HCReal sqrtDiscriminant = sqrt(discriminant);
            HCReal denominatorInverse = 1.0 / (2.0 * a);
            roots[0] = (-b + sqrtDiscriminant) * denominatorInverse;
            roots[1] = (-b - sqrtDiscriminant) * denominatorInverse;
        }
    }
    else if (d == 0.0 && a == 0.0 && b != 0.0) {
        // Component function is linear, so solve that for t=0
        roots[0] = -c / b;
    }
    else if (d == 0.0 && a == 0.0 && b == 0.0) {
        // No solutions
    }
    else {
        // Component function is a true cubic, solve that for t=0 using modified Cardano's Method
        a /= d;
        b /= d;
        c /= d;
        HCReal p = (3.0 * b - a * a) / 3.0;
        HCReal p3 = p / 3;
        HCReal q = (2.0 * a * a * a - 9.0 * a * b + 27.0 * c) / 27.0;
        HCReal q2 = q / 2.0;
        HCReal discriminant = q2 * q2 + p3 * p3 * p3;
        if (discriminant < 0.0) {
            HCReal mp3 = -p / 3.0;
            HCReal mp33 = mp3 * mp3 * mp3;
            HCReal r = sqrt(mp33);
            HCReal s = -q / (2.0 * r);
            HCReal cosphi = s < -1.0 ? -1.0 : s > 1.0 ? 1.0 : s;
            HCReal phi = acos(cosphi);
            HCReal crtr = cbrt(r);
            HCReal s1 = 2.0 * crtr;
            roots[0] = s1 * cos(phi / 3.0) - a / 3.0;
            roots[1] = s1 * cos((phi + 2.0 * M_PI) / 3.0) - a / 3.0;
            roots[2] = s1 * cos((phi + 4.0 * M_PI) / 3.0) - a / 3.0;
        }
        else if (discriminant == 0.0) {
            HCReal u1 = q2 < 0.0 ? cbrt(-q2) : -cbrt(q2);
            roots[0] = 2.0 * u1 - a / 3.0;
            roots[1] = -u1 - a / 3.0;
        }
        else {
            HCReal sd = sqrt(discriminant);
            HCReal u1 = cbrt(sd - q2);
            HCReal v1 = cbrt(sd + q2);
            roots[0] = u1 - v1 - a / 3.0;
        }
    }
    
    // Validate roots for use as t and u values
    HCInteger rCount = 0;
    HCReal ts[3];
    HCReal us[3];
    for (HCInteger rootIndex = 0; rootIndex < 3; rootIndex++) {
        HCReal root = roots[rootIndex];
        if (isnan(root)) {
            continue;
        }
        if (root < 0.0 || root > 1.0) {
            continue;
        }
        HCPoint intersection = HCContourCurveValueCubic(q0, qc0, qc1, q1, root);
        HCReal intersectionT = intersection.x / p1.x;
        if (intersectionT < 0.0 || intersectionT > 1.0) {
            continue;
        }
        ts[rCount] = intersectionT;
        us[rCount] = root;
        rCount++;
    }
    
    // Deliver results
    if (count != NULL) {
        *count = rCount;
    }
    if (t != NULL) {
        memcpy(t, ts, rCount * sizeof(HCReal));
    }
    if (u != NULL) {
        memcpy(u, us, rCount * sizeof(HCReal));
    }
}

void HCContourCurveIntersectionQuadraticQuadraticRecursive(HCPoint p0, HCPoint pc, HCPoint p1, HCReal pts, HCReal pte, HCPoint q0, HCPoint qc, HCPoint q1, HCReal qts, HCReal qte, HCInteger* count, HCReal* t, HCReal* u) {
    // Stop searching if the maximum number of intersections for quadratic-quadratic pairings has been reached
    if (*count >= 2) {
        return;
    }
    
    // Determine if quadratic curve bounds rectangles overlap
    HCRectangle pr = HCContourCurveBoundsQuadratic(p0, pc, p1);
    HCRectangle qr = HCContourCurveBoundsQuadratic(q0, qc, q1);
    if (!HCRectangleOverlapsRectangle(pr, qr)) {
        return;
    }
    
    // When the parameter span being considered for intersection is small enough, call its center an intersection
    // TODO: How small?
    HCReal ptSpan = (pte - pts);
    HCReal pSplit = 0.5;
    HCReal pt = pts + ptSpan * pSplit;
    HCReal qtSpan = (qte - qts);
    HCReal qSplit = 0.5;
    HCReal qt = qts + qtSpan * qSplit;
    HCReal tSpan = fmin(ptSpan, qtSpan);
    HCReal tPrecision = 0.001;
    if (tSpan < tPrecision) {
        // Filter intersections close to intersections already found
        // TODO: How close?
        if (count <= 0 || fabs(pt - t[*count - 1]) > tPrecision || fabs(qt - u[*count - 1]) > tPrecision) {
            t[*count] = pt;
            u[*count] = qt;
            *count = *count + 1;
        }
        return;
    }
    
    // Split the curves
    HCPoint psp0 = HCPointInvalid;
    HCPoint psc = HCPointInvalid;
    HCPoint psp1 = HCPointInvalid;
    HCPoint pep0 = HCPointInvalid;
    HCPoint pec = HCPointInvalid;
    HCPoint pep1 = HCPointInvalid;
    HCContourCurveSplitQuadratic(p0, pc, p1, pSplit, &psp0, &psc, &psp1, &pep0, &pec, &pep1);
    HCPoint qsp0 = HCPointInvalid;
    HCPoint qsc = HCPointInvalid;
    HCPoint qsp1 = HCPointInvalid;
    HCPoint qep0 = HCPointInvalid;
    HCPoint qec = HCPointInvalid;
    HCPoint qep1 = HCPointInvalid;
    HCContourCurveSplitQuadratic(q0, qc, q1, qSplit, &qsp0, &qsc, &qsp1, &qep0, &qec, &qep1);
    
    // Search for overlaps among the 4 resulting curves
    HCContourCurveIntersectionQuadraticQuadraticRecursive(psp0, psc, psp1, pts, pt, qsp0, qsc, qsp1, qts, qt, count, t, u);
    HCContourCurveIntersectionQuadraticQuadraticRecursive(psp0, psc, psp1, pts, pt, qep0, qec, qep1, qt, qte, count, t, u);
    HCContourCurveIntersectionQuadraticQuadraticRecursive(pep0, pec, pep1, pt, pte, qsp0, qsc, qsp1, qts, qt, count, t, u);
    HCContourCurveIntersectionQuadraticQuadraticRecursive(pep0, pec, pep1, pt, pte, qep0, qec, qep1, qt, qte, count, t, u);
}

void HCContourCurveIntersectionQuadraticQuadratic(HCPoint p0, HCPoint pc, HCPoint p1, HCPoint q0, HCPoint qc, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u) {
    // Determine if quadratic curve bounds rectangles overlap at all
    HCRectangle pr = HCContourCurveBoundsQuadratic(p0, pc, p1);
    HCRectangle qr = HCContourCurveBoundsQuadratic(q0, qc, q1);
    if (!HCRectangleOverlapsRectangle(pr, qr)) {
        if (count != NULL) {
            *count = 0;
        }
        return;
    }
    
    // Search for intersections
    HCInteger intersectionCount = 0;
    HCReal ts[2];
    HCReal us[2];
    HCContourCurveIntersectionQuadraticQuadraticRecursive(p0, pc, p1, 0.0, 1.0, q0, qc, q1, 0.0, 1.0, &intersectionCount, ts, us);
    
    // Deliver results
    if (count != NULL) {
        *count = intersectionCount;
    }
    if (t != NULL) {
        memcpy(t, ts, intersectionCount * sizeof(HCReal));
    }
    if (u != NULL) {
        memcpy(u, us, intersectionCount * sizeof(HCReal));
    }
}

void HCContourCurveIntersectionQuadraticCubicRecursive(HCPoint p0, HCPoint pc, HCPoint p1, HCReal pts, HCReal pte, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCReal qts, HCReal qte, HCInteger* count, HCReal* t, HCReal* u) {
    // Stop searching if the maximum number of intersections for quadratic-cubic pairings has been reached
    if (*count >= 6) {
        return;
    }
    
    // Determine if quadratic and cubic curve bounds rectangles overlap
    HCRectangle pr = HCContourCurveBoundsQuadratic(p0, pc, p1);
    HCRectangle qr = HCContourCurveBoundsCubic(q0, qc0, qc1, q1);
    if (!HCRectangleOverlapsRectangle(pr, qr)) {
        return;
    }
    
    // When the parameter span being considered for intersection is small enough, call its center an intersection
    // TODO: How small?
    HCReal ptSpan = (pte - pts);
    HCReal pSplit = 0.5;
    HCReal pt = pts + ptSpan * pSplit;
    HCReal qtSpan = (qte - qts);
    HCReal qSplit = 0.5;
    HCReal qt = qts + qtSpan * qSplit;
    HCReal tSpan = fmin(ptSpan, qtSpan);
    HCReal tPrecision = 0.001;
    if (tSpan < tPrecision) {
        // Filter intersections close to intersections already found
        // TODO: How close?
        if (count <= 0 || fabs(pt - t[*count - 1]) > tPrecision || fabs(qt - u[*count - 1]) > tPrecision) {
            t[*count] = pt;
            u[*count] = qt;
            *count = *count + 1;
        }
        return;
    }
    
    // Split the curves
    HCPoint psp0 = HCPointInvalid;
    HCPoint psc = HCPointInvalid;
    HCPoint psp1 = HCPointInvalid;
    HCPoint pep0 = HCPointInvalid;
    HCPoint pec = HCPointInvalid;
    HCPoint pep1 = HCPointInvalid;
    HCContourCurveSplitQuadratic(p0, pc, p1, pSplit, &psp0, &psc, &psp1, &pep0, &pec, &pep1);
    HCPoint qsp0 = HCPointInvalid;
    HCPoint qsc0 = HCPointInvalid;
    HCPoint qsc1 = HCPointInvalid;
    HCPoint qsp1 = HCPointInvalid;
    HCPoint qep0 = HCPointInvalid;
    HCPoint qec0 = HCPointInvalid;
    HCPoint qec1 = HCPointInvalid;
    HCPoint qep1 = HCPointInvalid;
    HCContourCurveSplitCubic(q0, qc0, qc1, q1, qSplit, &qsp0, &qsc0, &qsc1, &qsp1, &qep0, &qec0, &qec1, &qep1);
    
    // Search for overlaps among the 4 resulting curves
    HCContourCurveIntersectionQuadraticCubicRecursive(psp0, psc, psp1, pts, pt, qsp0, qsc0, qsc1, qsp1, qts, qt, count, t, u);
    HCContourCurveIntersectionQuadraticCubicRecursive(psp0, psc, psp1, pts, pt, qep0, qec0, qec1, qep1, qt, qte, count, t, u);
    HCContourCurveIntersectionQuadraticCubicRecursive(pep0, pec, pep1, pt, pte, qsp0, qsc0, qsc1, qsp1, qts, qt, count, t, u);
    HCContourCurveIntersectionQuadraticCubicRecursive(pep0, pec, pep1, pt, pte, qep0, qec0, qec1, qep1, qt, qte, count, t, u);
}

void HCContourCurveIntersectionQuadraticCubic(HCPoint p0, HCPoint pc, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u) {
    // Determine if quadratic and cubic curve bounds rectangles overlap at all
    HCRectangle pr = HCContourCurveBoundsQuadratic(p0, pc, p1);
    HCRectangle qr = HCContourCurveBoundsCubic(q0, qc0, qc1, q1);
    if (!HCRectangleOverlapsRectangle(pr, qr)) {
        if (count != NULL) {
            *count = 0;
        }
        return;
    }
    
    // Search for intersections
    HCInteger intersectionCount = 0;
    HCReal ts[6];
    HCReal us[6];
    HCContourCurveIntersectionQuadraticCubicRecursive(p0, pc, p1, 0.0, 1.0, q0, qc0, qc1, q1, 0.0, 1.0, &intersectionCount, ts, us);
    
    // Deliver results
    if (count != NULL) {
        *count = intersectionCount;
    }
    if (t != NULL) {
        memcpy(t, ts, intersectionCount * sizeof(HCReal));
    }
    if (u != NULL) {
        memcpy(u, us, intersectionCount * sizeof(HCReal));
    }
}

void HCContourCurveIntersectionCubicCubicRecursive(HCPoint p0, HCPoint pc0, HCPoint pc1, HCPoint p1, HCReal pts, HCReal pte, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCReal qts, HCReal qte, HCInteger* count, HCReal* t, HCReal* u) {
    // Stop searching if the maximum number of intersections for cubic-cubic pairings has been reached
    if (*count >= 9) {
        return;
    }
    
    // Determine if cubic curve bounds rectangles overlap
    HCRectangle pr = HCContourCurveBoundsCubic(p0, pc0, pc1, p1);
    HCRectangle qr = HCContourCurveBoundsCubic(q0, qc0, qc1, q1);
    if (!HCRectangleOverlapsRectangle(pr, qr)) {
        return;
    }
    
    // When the parameter span being considered for intersection is small enough, call its center an intersection
    // TODO: How small?
    HCReal ptSpan = (pte - pts);
    HCReal pSplit = 0.5;
    HCReal pt = pts + ptSpan * pSplit;
    HCReal qtSpan = (qte - qts);
    HCReal qSplit = 0.5;
    HCReal qt = qts + qtSpan * qSplit;
    HCReal tSpan = fmin(ptSpan, qtSpan);
    HCReal tPrecision = 0.001;
    if (tSpan < tPrecision) {
        // Filter intersections close to intersections already found
        // TODO: How close?
        if (count <= 0 || fabs(pt - t[*count - 1]) > tPrecision || fabs(qt - u[*count - 1]) > tPrecision) {
            t[*count] = pt;
            u[*count] = qt;
            *count = *count + 1;
        }
        return;
    }
    
    // Split the curves
    HCPoint psp0 = HCPointInvalid;
    HCPoint psc0 = HCPointInvalid;
    HCPoint psc1 = HCPointInvalid;
    HCPoint psp1 = HCPointInvalid;
    HCPoint pep0 = HCPointInvalid;
    HCPoint pec0 = HCPointInvalid;
    HCPoint pec1 = HCPointInvalid;
    HCPoint pep1 = HCPointInvalid;
    HCContourCurveSplitCubic(p0, pc0, pc1, p1, pSplit, &psp0, &psc0, &psc1, &psp1, &pep0, &pec0, &pec1, &pep1);
    HCPoint qsp0 = HCPointInvalid;
    HCPoint qsc0 = HCPointInvalid;
    HCPoint qsc1 = HCPointInvalid;
    HCPoint qsp1 = HCPointInvalid;
    HCPoint qep0 = HCPointInvalid;
    HCPoint qec0 = HCPointInvalid;
    HCPoint qec1 = HCPointInvalid;
    HCPoint qep1 = HCPointInvalid;
    HCContourCurveSplitCubic(q0, qc0, qc1, q1, qSplit, &qsp0, &qsc0, &qsc1, &qsp1, &qep0, &qec0, &qec1, &qep1);
    
    // Search for overlaps among the 4 resulting curves
    HCContourCurveIntersectionCubicCubicRecursive(psp0, psc0, psc1, psp1, pts, pt, qsp0, qsc0, qsc1, qsp1, qts, qt, count, t, u);
    HCContourCurveIntersectionCubicCubicRecursive(psp0, psc0, psc1, psp1, pts, pt, qep0, qec0, qec1, qep1, qt, qte, count, t, u);
    HCContourCurveIntersectionCubicCubicRecursive(pep0, pec0, pec1, pep1, pt, pte, qsp0, qsc0, qsc1, qsp1, qts, qt, count, t, u);
    HCContourCurveIntersectionCubicCubicRecursive(pep0, pec0, pec1, pep1, pt, pte, qep0, qec0, qec1, qep1, qt, qte, count, t, u);
}

void HCContourCurveIntersectionCubicCubic(HCPoint p0, HCPoint pc0, HCPoint pc1, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u) {
    // Determine if cubic curve bounds rectangles overlap at all
    HCRectangle pr = HCContourCurveBoundsCubic(p0, pc0, pc1, p1);
    HCRectangle qr = HCContourCurveBoundsCubic(q0, qc0, qc1, q1);
    if (!HCRectangleOverlapsRectangle(pr, qr)) {
        if (count != NULL) {
            *count = 0;
        }
        return;
    }
    
    // Search for intersections
    HCInteger intersectionCount = 0;
    HCReal ts[9];
    HCReal us[9];
    HCContourCurveIntersectionCubicCubicRecursive(p0, pc0, pc1, p1, 0.0, 1.0, q0, qc0, qc1, q1, 0.0, 1.0, &intersectionCount, ts, us);
    
    // Deliver results
    if (count != NULL) {
        *count = intersectionCount;
    }
    if (t != NULL) {
        memcpy(t, ts, intersectionCount * sizeof(HCReal));
    }
    if (u != NULL) {
        memcpy(u, us, intersectionCount * sizeof(HCReal));
    }
}
