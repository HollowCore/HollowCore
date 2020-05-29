///
/// @file HCCurve.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 5/24/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///
/// @see https://pomax.github.io/bezierinfo
///

#include "HCCurve.h"
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCCurve HCCurveMakeLinear(HCPoint p0, HCPoint p1) {
    return (HCCurve){ .p0 = p0, .c0 = HCPointInvalid, .c1 = HCPointInvalid, .p1 = p1 };
}

HCCurve HCCurveMakeQuadratic(HCPoint p0, HCPoint c, HCPoint p1) {
    return (HCCurve){ .p0 = p0, .c0 = c, .c1 = HCPointInvalid, .p1 = p1 };
}

HCCurve HCCurveMakeCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
    return (HCCurve){ .p0 = p0, .c0 = c0, .c1 = c1, .p1 = p1 };
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCCurveIsInvalid(HCCurve curve) {
    return
        HCPointIsInvalid(curve.p0) &&
        HCPointIsInvalid(curve.c0) &&
        HCPointIsInvalid(curve.c1) &&
        HCPointIsInvalid(curve.p1);
}

HCBoolean HCCurveIsSimilar(HCCurve curve, HCCurve other, HCReal axisDissimilarity) {
    return
        HCPointIsSimilar(curve.p0, other.p0, axisDissimilarity) &&
        (HCPointIsSimilar(curve.c0, other.c0, axisDissimilarity) || (HCPointIsInvalid(curve.c0) && HCPointIsInvalid(other.c0))) &&
        (HCPointIsSimilar(curve.c1, other.c1, axisDissimilarity) || (HCPointIsInvalid(curve.c1) && HCPointIsInvalid(other.c1))) &&
        HCPointIsSimilar(curve.p1, other.p1, axisDissimilarity);
}

HCBoolean HCCurveIsZero(HCCurve curve) {
    return
        HCPointIsZero(curve.p0) &&
        (HCPointIsZero(curve.c0) || HCPointIsInvalid(curve.c0)) &&
        (HCPointIsZero(curve.c1) || HCPointIsInvalid(curve.c1)) &&
        HCPointIsZero(curve.p1);
}

HCBoolean HCCurveIsInfinite(HCCurve curve) {
    return
        HCPointIsInfinite(curve.p0) ||
        HCPointIsInfinite(curve.c0) ||
        HCPointIsInfinite(curve.c1) ||
        HCPointIsInfinite(curve.p1);
}

HCBoolean HCCurveIsEqual(HCCurve curve, HCCurve other) {
    return
        HCPointIsEqual(curve.p0, other.p0) &&
        (HCPointIsEqual(curve.c0, other.c0) || (HCPointIsInvalid(curve.c0) && HCPointIsInvalid(other.c0))) &&
        (HCPointIsEqual(curve.c1, other.c1) || (HCPointIsInvalid(curve.c1) && HCPointIsInvalid(other.c1))) &&
        HCPointIsEqual(curve.p1, other.p1);
}

HCInteger HCCurveHashValue(HCCurve curve) {
    return
        HCPointHashValue(curve.p0) ^
        HCPointHashValue(curve.c0) ^
        HCPointHashValue(curve.c1) ^
        HCPointHashValue(curve.p1);
}

void HCCurvePrint(HCCurve curve, FILE* stream) {
    fprintf(stream, "<p0:");
    HCPointPrint(curve.c0, stream);
    fprintf(stream, ",c0:");
    HCPointPrint(curve.c0, stream);
    fprintf(stream, ",c1:");
    HCPointPrint(curve.c1, stream);
    fprintf(stream, ",p1:");
    HCPointPrint(curve.p1, stream);
    fprintf(stream, ">");
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Order
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCCurveIsLinear(HCCurve curve) {
    if (HCPointIsInvalid(curve.p0) || HCPointIsInvalid(curve.p1)) {
        return false;
    }
    if (HCPointIsInvalid(curve.c0) && HCPointIsInvalid(curve.c1)) {
        return true;
    }
    return false;
}

HCCurve HCCurveAsLinear(HCCurve curve) {
    return HCCurveMakeLinear(curve.p0, curve.p1);
}

HCBoolean HCCurveIsQuadratic(HCCurve curve) {
    if (HCPointIsInvalid(curve.p0) || HCPointIsInvalid(curve.c0) || HCPointIsInvalid(curve.p1)) {
        return false;
    }
    if (HCPointIsInvalid(curve.c1)) {
        return true;
    }
    return false;
}

HCCurve HCCurveAsQuadratic(HCCurve curve) {
    if (HCCurveIsQuadratic(curve)) {
        return HCCurveMakeQuadratic(curve.p0, curve.c0, curve.p1);
    }
    if (HCCurveIsCubic(curve)) {
        return HCCurveMakeQuadratic(curve.p0, HCPointInterpolate(curve.c0, curve.c1, 0.5), curve.p1);
    }
    return HCCurveMakeQuadratic(curve.p0, HCPointInterpolate(curve.p0, curve.p1, 0.5), curve.p1);
}

HCBoolean HCCurveIsCubic(HCCurve curve) {
    if (HCPointIsInvalid(curve.p0) || HCPointIsInvalid(curve.c0) || HCPointIsInvalid(curve.c1) || HCPointIsInvalid(curve.p1)) {
        return false;
    }
    return true;
}

HCCurve HCCurveAsCubic(HCCurve curve) {
    if (HCCurveIsCubic(curve)) {
        return curve;
    }
    if (HCCurveIsQuadratic(curve)) {
        HCPoint p0 = curve.p0;
        HCPoint  c = curve.c0;
        HCPoint p1 = curve.p1;
        HCPoint c0 = HCPointMake(p0.x + (2.0/3.0) * (c.x - p0.x), p0.y + (2.0/3.0) * (c.y - p0.y));
        HCPoint c1 = HCPointMake(p1.x + (2.0/3.0) * (c.x - p1.x), p1.y + (2.0/3.0) * (c.y - p1.y));
        return HCCurveMakeCubic(p0, c0, c1, p1);
    }
    return HCCurveMakeCubic(curve.p0, curve.p0, curve.p1, curve.p1);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Canonical Type
//----------------------------------------------------------------------------------------------------------------------------------
HCCurveType HCCurveCanonicalType(HCCurve curve) {
    // Determine curve type based on provided information
    if (HCPointIsInvalid(curve.p0) || HCPointIsInvalid(curve.p1) || (HCPointIsInvalid(curve.c0) && !HCPointIsInvalid(curve.c1))) {
        // Provided curve data has invalid anchor points or has control point 1 but not control point 0
        return HCCurveTypeInvalid;
    }
    else if (HCPointIsInvalid(curve.c0)) {
        // Linear curve data provided
        return HCCurveCanonicalTypeLinear(curve.p0, curve.p1);
    }
    else if (HCPointIsInvalid(curve.c1)) {
        // Quadratic curve data provided
        return HCCurveCanonicalTypeQuadratic(curve.p0, curve.c0, curve.p1);
    }
    else {
        // Cubic curve data provided
        return HCCurveCanonicalTypeCubic(curve.p0, curve.c0, curve.c1, curve.p1);
    }
}

HCCurveType HCCurveCanonicalTypeLinear(HCPoint p0, HCPoint p1) {
    // Determine if linear curve is a point or a line segment
    return HCPointIsEqual(p0, p1) ? HCCurveTypePoint : HCCurveTypeLinear;
}

HCCurveType HCCurveCanonicalTypeQuadratic(HCPoint p0, HCPoint c, HCPoint p1) {
    // Determine if quadratic curve has degenerate points
    HCBoolean p0p1 = HCPointIsEqual(p0, p1);
    HCBoolean p0c = HCPointIsEqual(p0, c);
    HCBoolean p1c = HCPointIsEqual(p1, c);
    if (p0p1 && p0c) {
        return HCCurveTypePoint;
    }
    if (p0c || p1c) {
        return HCCurveTypeLinear;
    }
    
    // Determine if quadratic curve points are co-linear
    HCBoolean xOrdered = (p0.x <= c.x && c.x <= p1.x) || (p0.x >= c.x && c.x >= p1.x);
    if (xOrdered) {
        HCBoolean yOrdered = (p0.y <= c.y && c.y <= p1.y) || (p0.y >= c.y && c.y >= p1.y);
        if (yOrdered) {
            HCReal p0p1Slope = (p1.y - p0.y) / (p1.x - p0.x);
            HCReal p0cSlope = (c.y - p0.y) / (c.x - p0.x);
            if (p0cSlope == p0p1Slope) {
                return HCCurveTypeLinear;
            }
        }
    }
    
    // Curve is quadratic
    return HCCurveTypeQuadratic;
}

HCCurveType HCCurveCanonicalTypeCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
    // Determine if cubic curve has degenerate points
    HCBoolean p0p1 = HCPointIsEqual(p0, p1);
    HCBoolean p0c0 = HCPointIsEqual(p0, c0);
    HCBoolean p0c1 = HCPointIsEqual(p0, c1);
    HCBoolean p1c0 = HCPointIsEqual(p1, c0);
    HCBoolean p1c1 = HCPointIsEqual(p1, c1);
    HCBoolean c0c1 = HCPointIsEqual(c0, c1);
    if (p0p1 && p0c0 && p0c1) {
        return HCCurveTypePoint;
    }
    if ((p0c0 && p1c1) || (p0c1 && p1c0)) {
        return HCCurveTypeLinear;
    }
    if (p0c1 || p1c0) {
        return HCCurveTypeCubicSimple;
    }
    if (c0c1) {
        return HCCurveTypeCubicSimple;
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
                    return HCCurveTypeLinear;
                }
            }
        }
    }
    
    // Determine if control points form quadratic curve
    if (p0.x + 1.5 * (c0.x - p0.x) == p1.x + 1.5 * (c1.x - p1.x) && p0.y + 1.5 * (c0.y - p0.y) == p1.y + 1.5 * (c1.y - p1.y)) {
        return HCCurveTypeQuadratic;
    }
    
    // Calculate canonical curve end point (other points are p0: (0, 0), c0: (0, 1), c1: (1,1))
    HCPoint p = HCCurveCanonical(p0, c0, c1, p1);
    
    // Determine cubic curve type based on canonical curve end point
    // See http://graphics.pixar.com/people/derose/publications/CubicClassification/paper.pdf
    if (p.x == 1.0 && p.y == 1.0) {
        return HCCurveTypeCubicSimple;
    }
    if (p.x == 0.0 && p.y == 0.0) {
        return HCCurveTypeCubicLoopClosed;
    }
    if (p.y > 1.0) {
        return HCCurveTypeCubicSingleInflection;
    }
    if (p.x > 1.0) {
        return HCCurveTypeCubicSimple;
    }
    HCReal cuspEdge = (-1.0 * p.x * p.x + 2.0 * p.x + 3.0) * 0.25;
    if (p.y == cuspEdge) {
        return HCCurveTypeCubicCusp;
    }
    if (p.y > cuspEdge) {
        return HCCurveTypeCubicDoubleInflection;
    }
    if (p.x > 0.0) {
        HCReal t1LoopEdge = (sqrt(-3.0 * p.x * p.x + 12.0 * p.x) - p.x) * 0.5;
        if (p.y > t1LoopEdge) {
            return HCCurveTypeCubicLoop;
        }
        if (p.y == t1LoopEdge) {
            return HCCurveTypeCubicLoopAtEnd;
        }
    }
    else {
        HCReal t0LoopEdge = (-1.0 * p.x * p.x + 3.0 * p.x) * (1.0 / 3.0);
        if (p.y > t0LoopEdge) {
            return HCCurveTypeCubicLoop;
        }
        if (p.y == t0LoopEdge) {
            return HCCurveTypeCubicLoopAtStart;
        }
    }
    return HCCurveTypeCubicSimple;
}

HCPoint HCCurveCanonical(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
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
HCPoint HCCurveValue(HCCurve curve, HCReal t) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveValueLinear(curve.p0, curve.p1, t);
        }
        else {
            return HCCurveValueQuadratic(curve.p0, curve.c0, curve.p1, t);
        }
    }
    else {
        return HCCurveValueCubic(curve.p0, curve.c0, curve.c1, curve.p1, t);
    }
}

HCPoint HCCurveValueLinear(HCPoint p0, HCPoint p1, HCReal t) {
    HCReal tc = 1.0 - t;
        
    HCReal a = tc;
    HCReal b = t;
    HCReal x = a * p0.x + b * p1.x;
    HCReal y = a * p0.y + b * p1.y;
    
    return HCPointMake(x, y);
}

HCPoint HCCurveValueQuadratic(HCPoint p0, HCPoint c0, HCPoint p1, HCReal t) {
    HCReal tc = 1.0 - t;
    
    HCReal a = tc * tc;
    HCReal b = 2.0 * t * tc;
    HCReal c = t * t;
    HCReal x = a * p0.x + b * c0.x + c * p1.x;
    HCReal y = a * p0.y + b * c0.y + c * p1.y;
    
    return HCPointMake(x, y);
}

HCPoint HCCurveValueCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t) {
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
// MARK: - Tangent
//----------------------------------------------------------------------------------------------------------------------------------
HCCurve HCCurveTangent(HCCurve curve, HCReal t) {
    HCReal tx = HCRealInvalid;
    HCReal ty = HCRealInvalid;
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCCurveTangentLinear(curve.p0, curve.p1, t, &tx, &ty);
        }
        else {
            HCCurveTangentQuadratic(curve.p0, curve.c0, curve.p1, t, &tx, &ty);
        }
    }
    else {
        HCCurveTangentCubic(curve.p0, curve.c0, curve.c1, curve.p1, t, &tx, &ty);
    }
    HCPoint p = HCCurveValue(curve, t);
    HCPoint tp = HCPointOffset(p, tx, ty);
    return HCCurveMakeLinear(p, tp);
}

void HCCurveTangentLinear(HCPoint p0, HCPoint p1, HCReal t, HCReal* tx, HCReal* ty) {
    // Calculate linear derivative weight
    HCPoint dp = HCPointInvalid;
    HCCurveDerivativeLinear(p0, p1, &dp);
    
    // Evaluate derivative curve at t (constant, so that's easy!)
    HCPoint d = dp;
    
    // Deliver result as tangent vector
    if (tx != NULL) {
        *tx = d.x;
    }
    if (ty != NULL) {
        *ty = d.y;
    }
}

void HCCurveTangentQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t, HCReal* tx, HCReal* ty) {
    // Calculate quadratic derivative weights
    HCPoint dp0 = HCPointInvalid;
    HCPoint dp1 = HCPointInvalid;
    HCCurveDerivativeQuadratic(p0, c, p1, &dp0, &dp1);
    
    // Evaluate derivative curve at t
    HCPoint d = HCCurveValueLinear(dp0, dp1, t);
    
    // Deliver result as tangent vector
    if (tx != NULL) {
        *tx = d.x;
    }
    if (ty != NULL) {
        *ty = d.y;
    }
}

void HCCurveTangentCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t, HCReal* tx, HCReal* ty) {
    // Calculate cubic derivative weights
    HCPoint dp0 = HCPointInvalid;
    HCPoint  dc = HCPointInvalid;
    HCPoint dp1 = HCPointInvalid;
    HCCurveDerivativeCubic(p0, c0, c1, p1, &dp0, &dc, &dp1);
    
    // Evaluate derivative curve at t
    HCPoint d = HCCurveValueQuadratic(dp0, dc, dp1, t);
    
    // Deliver result as tangent vector
    if (tx != NULL) {
        *tx = d.x;
    }
    if (ty != NULL) {
        *ty = d.y;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Normal
//----------------------------------------------------------------------------------------------------------------------------------
HCCurve HCCurveNormal(HCCurve curve, HCReal t) {
    HCReal nx = HCRealInvalid;
    HCReal ny = HCRealInvalid;
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCCurveNormalLinear(curve.p0, curve.p1, t, &nx, &ny);
        }
        else {
            HCCurveNormalQuadratic(curve.p0, curve.c0, curve.p1, t, &nx, &ny);
        }
    }
    else {
        HCCurveNormalCubic(curve.p0, curve.c0, curve.c1, curve.p1, t, &nx, &ny);
    }
    HCPoint p = HCCurveValue(curve, t);
    HCPoint np = HCPointOffset(p, nx, ny);
    return HCCurveMakeLinear(p, np);
}

void HCCurveNormalLinear(HCPoint p0, HCPoint p1, HCReal t, HCReal* nx, HCReal* ny) {
    // Obtain tangent vector to linear curve at t
    HCReal tx = HCRealInvalid;
    HCReal ty = HCRealInvalid;
    HCCurveTangentLinear(p0, p1, t, &tx, &ty);
    
    // Rotate by pi/2 and deliver result as normal vector
    if (nx != NULL) {
        *nx = -ty;
    }
    if (ny != NULL) {
        *ny = +tx;
    }
}

void HCCurveNormalQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t, HCReal* nx, HCReal* ny) {
    // Obtain tangent vector to quadratic curve at t
    HCReal tx = HCRealInvalid;
    HCReal ty = HCRealInvalid;
    HCCurveTangentQuadratic(p0, c, p1, t, &tx, &ty);
    
    // Rotate by pi/2 and deliver result as normal vector
    if (nx != NULL) {
        *nx = -ty;
    }
    if (ny != NULL) {
        *ny = +tx;
    }
}

void HCCurveNormalCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t, HCReal* nx, HCReal* ny) {
    // Obtain tangent vector to cubic curve at t
    HCReal tx = HCRealInvalid;
    HCReal ty = HCRealInvalid;
    HCCurveTangentCubic(p0, c0, c1, p1, t, &tx, &ty);
    
    // Rotate by pi/2 and deliver result as normal vector
    if (nx != NULL) {
        *nx = -ty;
    }
    if (ny != NULL) {
        *ny = +tx;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Derivative
//----------------------------------------------------------------------------------------------------------------------------------
HCCurve HCCurveDerivative(HCCurve curve) {
    HCCurveType type = HCCurveCanonicalType(curve);
    switch (type) {
        case HCCurveTypeInvalid: {
            return HCCurveInvalid;
        } break;
        case HCCurveTypePoint:
        case HCCurveTypeLinear: {
            HCPoint dp = HCPointInvalid;
            HCCurveDerivativeLinear(curve.p0, curve.p1, &dp);
            return HCCurveMakeLinear(dp, dp);
        } break;
        case HCCurveTypeQuadratic: {
            HCPoint dp0 = HCPointInvalid;
            HCPoint dp1 = HCPointInvalid;
            HCCurveDerivativeQuadratic(curve.p0, curve.c0, curve.p1, &dp0, &dp1);
            return HCCurveMakeLinear(dp0, dp1);
        } break;
        case HCCurveTypeCubicSimple:
        case HCCurveTypeCubicSingleInflection:
        case HCCurveTypeCubicDoubleInflection:
        case HCCurveTypeCubicLoop:
        case HCCurveTypeCubicLoopAtStart:
        case HCCurveTypeCubicLoopAtEnd:
        case HCCurveTypeCubicLoopClosed:
        case HCCurveTypeCubicCusp: {
            HCPoint dp0 = HCPointInvalid;
            HCPoint dc = HCPointInvalid;
            HCPoint dp1 = HCPointInvalid;
            HCCurveDerivativeCubic(curve.p0, curve.c0, curve.c1, curve.p1, &dp0, &dc, &dp1);
            return HCCurveMakeQuadratic(dp0, dc, dp1);
        } break;
    }
}

void HCCurveDerivativeLinear(HCPoint p0, HCPoint p1, HCPoint* dp) {
    // Calculate linear derivative weights
    if (dp != NULL) {
        *dp = HCPointMake(1.0 * (p1.x - p0.x), 1.0 * (p1.y - p0.y));
    }
}

void HCCurveDerivativeQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint* dp0, HCPoint* dp1) {
    // Calculate quadratic derivative weights
    if (dp0 != NULL) {
        *dp0 = HCPointMake(2.0 * (c.x - p0.x), 2.0 * (c.y - p0.y));
    }
    if (dp1 != NULL) {
        *dp1 = HCPointMake(2.0 * (p1.x - c.x), 2.0 * (p1.y - c.y));
    }
}

void HCCurveDerivativeCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint* dp0, HCPoint* dc, HCPoint* dp1) {
    // Calculate cubic derivative weights
    if (dp0 != NULL) {
        *dp0 = HCPointMake(3.0 * (c0.x - p0.x), 3.0 * (c0.y - p0.y));
    }
    if (dc != NULL) {
        *dc =  HCPointMake(3.0 * (c1.x - c0.x), 3.0 * (c1.y - c0.y));
    }
    if (dp1 != NULL) {
        *dp1 = HCPointMake(3.0 * (p1.x - c1.x), 3.0 * (p1.y - c1.y));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Curvature
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCCurveCurvature(HCCurve curve, HCReal t) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveCurvatureLinear(curve.p0, curve.p1, t);
        }
        else {
            return HCCurveCurvatureQuadratic(curve.p0, curve.c0, curve.p1, t);
        }
    }
    else {
        return HCCurveCurvatureCubic(curve.p0, curve.c0, curve.c1, curve.p1, t);
    }
}

HCReal HCCurveCurvatureLinear(HCPoint p0, HCPoint p1, HCReal t) {
    // Linear curve has zero curvature
    return 0.0;
}

HCReal HCCurveCurvatureQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t) {
    // Calculate quadratic first and second derivative weights
    HCPoint dp0 = HCPointInvalid;
    HCPoint dp1 = HCPointInvalid;
    HCCurveDerivativeQuadratic(p0, c, p1, &dp0, &dp1);
    HCPoint ddp = HCPointInvalid;
    HCCurveDerivativeLinear(dp0, dp1, &ddp);
    
    // Evaluate quadratic derivative curves at t
    HCPoint d = HCCurveValueLinear(dp0, dp1, t);
    HCPoint dd = ddp;
    
    // Calculate curvature at t
    HCReal numerator = d.x * dd.y - dd.x * d.y;
    HCReal denominator = pow(d.x * d.x + d.y * d.y, 1.5);
    return numerator / denominator;
}

HCReal HCCurveCurvatureCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t) {
    // Calculate cubic first and second derivative weights
    HCPoint dp0 = HCPointInvalid;
    HCPoint  dc = HCPointInvalid;
    HCPoint dp1 = HCPointInvalid;
    HCCurveDerivativeCubic(p0, c0, c1, p1, &dp0, &dc, &dp1);
    HCPoint ddp0 = HCPointInvalid;
    HCPoint ddp1 = HCPointInvalid;
    HCCurveDerivativeQuadratic(dp0, dc, dp1, &ddp0, &ddp1);
    
    // Evaluate cubic derivative curves at t
    HCPoint d = HCCurveValueQuadratic(dp0, dc, dp1, t);
    HCPoint dd = HCCurveValueLinear(ddp0, ddp1, t);
    
    // Calculate curvature at t
    HCReal numerator = d.x * dd.y - dd.x * d.y;
    HCReal denominator = pow(d.x * d.x + d.y * d.y, 1.5);
    return numerator / denominator;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Extrema
//----------------------------------------------------------------------------------------------------------------------------------
void HCCurveExtrema(HCCurve curve, HCInteger* count, HCReal* extrema) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCCurveExtremaLinear(curve.p0, curve.p1, count, extrema);
        }
        else {
            HCCurveExtremaQuadratic(curve.p0, curve.c0, curve.p1, count, extrema);
        }
    }
    else {
        HCCurveExtremaCubic(curve.p0, curve.c0, curve.c1, curve.p1, count, extrema);
    }
}

void HCCurveExtremaLinear(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* extrema) {
    // Linear curve has no extrema
    if (count != NULL) {
        *count = 0;
    }
}

void HCCurveExtremaQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* extrema) {
    HCInteger tCount = 0;
    HCReal t[2];
    
    // Calculate quadratic derivative weights
    HCPoint dp0 = HCPointMake(2.0 * (c.x - p0.x), 2.0 * (c.y - p0.y));
    HCPoint dp1 = HCPointMake(2.0 * (p1.x - c.x), 2.0 * (p1.y - c.y));
    HCCurveDerivativeQuadratic(p0, c, p1, &dp0, &dp1);
    
    // Calculate zero crossing of derivative in x and add as extrema if in range
    HCReal ax = -1.0 * dp0.x + 1.0 * dp1.x;
    HCReal bx = +1.0 * dp0.x;
    HCReal xt = -bx / ax;
    if (xt >= 0.0 && xt <= 1.0) {
        t[tCount++] = xt;
    }
    
    // Calculate zero crossing of derivative in y and add as extrema if in range
    HCReal ay = -1.0 * dp0.y + 1.0 * dp1.y;
    HCReal by = +1.0 * dp0.y;
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

void HCCurveExtremaCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* extrema) {
    HCInteger tCount = 0;
    HCReal t[6];
    
    // Calculate cubic derivative weights
    HCPoint dp0 = HCPointInvalid;
    HCPoint  dc = HCPointInvalid;
    HCPoint dp1 = HCPointInvalid;
    HCCurveDerivativeCubic(p0, c0, c1, p1, &dp0, &dc, &dp1);
    
    // Calculate zero crossing of derivative in x and add as extrema if real and in range
    HCReal ax = +1.0 * dp0.x - 2.0 * dc.x + 1.0 * dp1.x;
    HCReal bx = -2.0 * dp0.x + 2.0 * dc.x;
    HCReal cx = +1.0 * dp0.x;
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
    HCReal ay = +1.0 * dp0.y - 2.0 * dc.y + 1.0 * dp1.y;
    HCReal by = -2.0 * dp0.y + 2.0 * dc.y;
    HCReal cy = +1.0 * dp0.y;
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
    HCCurveExtremaQuadratic(dp0, dc, dp1, &quadraticCount, &t[tCount]);
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
void HCCurveInflections(HCCurve curve, HCInteger* count, HCReal* inflections) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCCurveInflectionsLinear(curve.p0, curve.p1, count, inflections);
        }
        else {
            HCCurveInflectionsQuadratic(curve.p0, curve.c0, curve.p1, count, inflections);
        }
    }
    else {
        HCCurveInflectionsCubic(curve.p0, curve.c0, curve.c1, curve.p1, count, inflections);
    }
}

void HCCurveInflectionsLinear(HCPoint p0, HCPoint p1, HCInteger* count, HCReal* inflections) {
    // Linear curves have no inflections
    if (count != NULL) {
        *count = 0;
    }
}

void HCCurveInflectionsQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* inflections) {
    // Quadratic curves have no inflections
    if (count != NULL) {
        *count = 0;
    }
}

void HCCurveInflectionsCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* inflections) {
    // Axis-align the curve to make computation of inflections more straightforward
    HCCurveXAxisAlignedCubic(p0, c0, c1, p1, &p0, &c0, &c1, &p1);
    
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
HCRectangle HCCurveApproximateBounds(HCCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveApproximateBoundsLinear(curve.p0, curve.p1);
        }
        else {
            return HCCurveApproximateBoundsQuadratic(curve.p0, curve.c0, curve.p1);
        }
    }
    else {
        return HCCurveApproximateBoundsCubic(curve.p0, curve.c0, curve.c1, curve.p1);
    }
}

HCRectangle HCCurveApproximateBoundsLinear(HCPoint p0, HCPoint p1) {
    return HCRectangleMakeWithEdges(
        fmin(p0.x, p1.x),
        fmin(p0.y, p1.y),
        fmax(p0.x, p1.x),
        fmax(p0.y, p1.y));
}

HCRectangle HCCurveApproximateBoundsQuadratic(HCPoint p0, HCPoint c, HCPoint p1) {
    return HCRectangleMakeWithEdges(
        fmin(p0.x, fmin(c.x, p1.x)),
        fmin(p0.y, fmin(c.y, p1.y)),
        fmax(p0.x, fmax(c.x, p1.x)),
        fmax(p0.y, fmax(c.y, p1.y)));
}

HCRectangle HCCurveApproximateBoundsCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
    return HCRectangleMakeWithEdges(
        fmin(p0.x, fmin(c0.x, fmin(c1.x, p1.x))),
        fmin(p0.y, fmin(c0.y, fmin(c1.y, p1.y))),
        fmax(p0.x, fmax(c0.x, fmax(c1.x, p1.x))),
        fmax(p0.y, fmax(c0.y, fmax(c1.y, p1.y))));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Bounds
//----------------------------------------------------------------------------------------------------------------------------------
HCRectangle HCCurveBounds(HCCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveBoundsLinear(curve.p0, curve.p1);
        }
        else {
            return HCCurveBoundsQuadratic(curve.p0, curve.c0, curve.p1);
        }
    }
    else {
        return HCCurveBoundsCubic(curve.p0, curve.c0, curve.c1, curve.p1);
    }
}

HCRectangle HCCurveBoundsLinear(HCPoint p0, HCPoint p1) {
    // Find min/max of end points
    HCReal minX = fmin(p0.x, p1.x);
    HCReal minY = fmin(p0.y, p1.y);
    HCReal maxX = fmax(p0.x, p1.x);
    HCReal maxY = fmax(p0.y, p1.y);
    
    // Calculate bounding rectangle from min/max of end points
    return HCRectangleMakeWithEdges(minX, minY, maxX, maxY);
}

HCRectangle HCCurveBoundsQuadratic(HCPoint p0, HCPoint c, HCPoint p1) {
    // Calculate quadratic curve extrema
    HCReal extrema[2];
    HCInteger count = 0;
    HCCurveExtremaQuadratic(p0, c, p1, &count, extrema);
    
    // Find min/max of extrema and end points
    HCReal minX = fmin(p0.x, p1.x);
    HCReal minY = fmin(p0.y, p1.y);
    HCReal maxX = fmax(p0.x, p1.x);
    HCReal maxY = fmax(p0.y, p1.y);
    for (HCInteger i = 0; i < count; i++) {
        HCReal t = extrema[i];
        HCPoint p = HCCurveValueQuadratic(p0, c, p1, t);
        minX = fmin(minX, p.x);
        minY = fmin(minY, p.y);
        maxX = fmax(maxX, p.x);
        maxY = fmax(maxY, p.y);
    }
    
    // Calculate bounding rectangle from min/max of extrema and end points
    return HCRectangleMakeWithEdges(minX, minY, maxX, maxY);
}

HCRectangle HCCurveBoundsCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
    // Calculate cubic curve extrema
    HCReal extrema[6];
    HCInteger count = 0;
    HCCurveExtremaCubic(p0, c0, c1, p1, &count, extrema);
    
    // Find min/max of extrema and end points
    HCReal minX = fmin(p0.x, p1.x);
    HCReal minY = fmin(p0.y, p1.y);
    HCReal maxX = fmax(p0.x, p1.x);
    HCReal maxY = fmax(p0.y, p1.y);
    for (HCInteger i = 0; i < count; i++) {
        HCReal t = extrema[i];
        HCPoint p = HCCurveValueCubic(p0, c0, c1, p1, t);
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
HCReal HCCurveLength(HCCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveLengthLinear(curve.p0, curve.p1);
        }
        else {
            return HCCurveLengthQuadratic(curve.p0, curve.c0, curve.p1);
        }
    }
    else {
        return HCCurveLengthCubic(curve.p0, curve.c0, curve.c1, curve.p1);
    }
}

HCReal HCCurveLengthLinear(HCPoint p0, HCPoint p1) {
    // Calculate using linear distance equation
    return HCPointDistance(p0, p1);
}

HCReal HCCurveLengthQuadratic(HCPoint p0, HCPoint c, HCPoint p1) {
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

HCReal HCCurveLengthCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
    // TODO: Use Legendre-Gauss to numerically calculate length
    // Calculate using polyline segment lengths
    HCReal length = 0.0;
    HCPoint ps = p0;
    HCReal t = 0.0;
    HCReal tStep = 0.01;
    while (true) {
        // Calculate segment length and aggregate to length
        // TODO: Cache polyline?
        HCPoint pe = HCCurveValueCubic(p0, c0, c1, p1, t);
        HCReal segmentLength = HCPointDistance(ps, pe);
        length += segmentLength;
        ps = pe;
        
        // Move to next parameter sample, or finish if the end has been reached
        if (t == 1.0) {
            break;
        }
        t += tStep;
        if (t > 1.0) {
            t = 1.0;
        }
    }
    return length;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameterization by Arc Length
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCCurveParameter(HCCurve curve, HCReal d) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveParameterLinear(curve.p0, curve.p1, d);
        }
        else {
            return HCCurveParameterQuadratic(curve.p0, curve.c0, curve.p1, d);
        }
    }
    else {
        return HCCurveParameterCubic(curve.p0, curve.c0, curve.c1, curve.p1, d);
    }
}

HCReal HCCurveParameterLinear(HCPoint p0, HCPoint p1, HCReal d) {
    // Linear curve parameterized by distance is a simple division
    return HCPointDistance(p0, p1) / d;
}

HCReal HCCurveParameterQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal d) {
    if (d < 0.0) {
        return 0.0;
    }
    
    // TODO: Can do this faster by calling HCCurveLengthQuadratic()?
    // Walk the curve polyline until the segment contains the desired distance
    HCReal length = 0.0;
    HCPoint ps = p0;
    HCPoint pe = ps;
    HCReal t = 0.0;
    HCReal tStep = 0.01;
    for (t = 0.0; t <= 1.0; t += tStep) {
        // TODO: Cache polyline?
        pe = HCCurveValueQuadratic(p0, c, p1, t);
        HCReal segmentLength = HCPointDistance(ps, pe);
        if (length + segmentLength >= d) {
            break;
        }
        length += segmentLength;
        ps = pe;
    }
    
    // Unless the end of the curve was reached, walk partially along the last segment to d
    HCReal remaining = d - length;
    if (remaining <= 0.0) {
        return t;
    }
    HCReal segmentLength = HCPointDistance(ps, pe);
    HCReal tRemaining = (remaining / segmentLength) * tStep;
    t += tRemaining;
    return t;
}

HCReal HCCurveParameterCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal d) {
    if (d < 0.0) {
        return 0.0;
    }
    
    // TODO: Use Legendre-Gauss to numerically calculate length
    // Walk the curve polyline until the segment contains the desired distance
    HCReal length = 0.0;
    HCPoint ps = p0;
    HCPoint pe = ps;
    HCReal t = 0.0;
    HCReal tStep = 0.01;
    for (t = 0.0; t <= 1.0; t += tStep) {
        // TODO: Cache polyline?
        pe = HCCurveValueCubic(p0, c0, c1, p1, t);
        HCReal segmentLength = HCPointDistance(ps, pe);
        if (length + segmentLength >= d) {
            break;
        }
        length += segmentLength;
        ps = pe;
    }
    
    // Unless the end of the curve was reached, walk partially along the last segment to d
    HCReal remaining = d - length;
    if (remaining <= 0.0) {
        return t;
    }
    HCReal segmentLength = HCPointDistance(ps, pe);
    HCReal tRemaining = (remaining / segmentLength) * tStep;
    t += tRemaining;
    return t;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameter Nearest Point
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCCurveParameterNearestPoint(HCCurve curve, HCPoint p) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveParameterNearestPointLinear(curve.p0, curve.p1, p);
        }
        else {
            return HCCurveParameterNearestPointQuadratic(curve.p0, curve.c0, curve.p1, p);
        }
    }
    else {
        return HCCurveParameterNearestPointCubic(curve.p0, curve.c0, curve.c1, curve.p1, p);
    }
}

HCReal HCCurveParameterNearestPointLinear(HCPoint p0, HCPoint p1, HCPoint p) {
    // Translate the curve and point to origin then rotate them to the x axis
    p1.x -= p0.x;
    p1.y -= p0.y;
    p.x -= p0.x;
    p.y -= p0.y;
    HCReal angle = -atan2(p1.y, p1.x);
    HCReal cosAngle = cos(angle);
    HCReal sinAngle = sin(angle);
    HCReal p1x = cosAngle * p1.x - sinAngle * p1.y;
    HCReal px = cosAngle * p.x - sinAngle * p.y;
    
    // Project point to x-axis by ignoring y value and determine t value by linearization
    HCReal t = px / p1x;
    
    // Clamp to start and end as the point is closest to one of the end points if beyond the edges
    return fmax(0.0, fmin(1.0, t));
}

HCReal HCCurveParameterNearestPointQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint p) {
    // TODO: Can avoid using polyline?
    // Use polyline to find parameter with nearest evaluated quadratic curve point
    HCReal nearestT = 0.0;
    HCReal nearestDistance = HCRealMaximumPositive;
    HCReal tStep = 0.01;
    HCReal t = 0.0;
    while (true) {
        // Sample distance and compare to current nearest
        HCPoint pt = HCCurveValueQuadratic(p0, c, p1, t);
        HCReal distance = HCPointDistance(p, pt);
        if (nearestDistance > distance) {
            nearestDistance = distance;
            nearestT = t;
        }
        
        // Move to next parameter sample, or finish if the end has been reached
        if (t == 1.0) {
            break;
        }
        t += tStep;
        if (t > 1.0) {
            t = 1.0;
        }
    }
    return nearestT;
}

HCReal HCCurveParameterNearestPointCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint p) {
    // TODO: Can avoid using polyline?
    // Use polyline to find parameter with nearest evaluated quadratic curve point
    HCReal nearestT = 0.0;
    HCReal nearestDistance = HCRealMaximumPositive;
    HCReal tStep = 0.01;
    HCReal t = 0.0;
    while (true) {
        // Sample distance and compare to current nearest
        HCPoint pt = HCCurveValueCubic(p0, c0, c1, p1, t);
        HCReal distance = HCPointDistance(p, pt);
        if (nearestDistance > distance) {
            nearestDistance = distance;
            nearestT = t;
        }
        
        // Move to next parameter sample, or finish if the end has been reached
        if (t == 1.0) {
            break;
        }
        t += tStep;
        if (t > 1.0) {
            t = 1.0;
        }
    }
    return nearestT;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Distance from Point
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCCurveDistanceFromPoint(HCCurve curve, HCPoint p) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveDistanceFromPointLinear(curve.p0, curve.p1, p);
        }
        else {
            return HCCurveDistanceFromPointQuadratic(curve.p0, curve.c0, curve.p1, p);
        }
    }
    else {
        return HCCurveDistanceFromPointCubic(curve.p0, curve.c0, curve.c1, curve.p1, p);
    }
}

HCReal HCCurveDistanceFromPointLinear(HCPoint p0, HCPoint p1, HCPoint p) {
    HCReal nearest = HCCurveParameterNearestPointLinear(p0, p1, p);
    HCPoint nearestPoint = HCCurveValueLinear(p0, p1, nearest);
    return HCPointDistance(nearestPoint, p);
}

HCReal HCCurveDistanceFromPointQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint p) {
    HCReal nearest = HCCurveParameterNearestPointQuadratic(p0, c, p1, p);
    HCPoint nearestPoint = HCCurveValueQuadratic(p0, c, p1, nearest);
    return HCPointDistance(nearestPoint, p);
}

HCReal HCCurveDistanceFromPointCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint p) {
    HCReal nearest = HCCurveParameterNearestPointCubic(p0, c0, c1, p1, p);
    HCPoint nearestPoint = HCCurveValueCubic(p0, c0, c1, p1, nearest);
    return HCPointDistance(nearestPoint, p);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Baseline Projection
//----------------------------------------------------------------------------------------------------------------------------------

HCPoint HCCurveBaselineProjection(HCCurve curve, HCReal t) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveBaselineProjectionLinear(curve.p0, curve.p1, t);
        }
        else {
            return HCCurveBaselineProjectionQuadratic(curve.p0, curve.p1, t);
        }
    }
    else {
        return HCCurveBaselineProjectionCubic(curve.p0, curve.p1, t);
    }
}

HCPoint HCCurveBaselineProjectionLinear(HCPoint p0, HCPoint p1, HCReal t) {
    // Linear projection onto "baseline" is the point itself
    return HCCurveValueLinear(p0, p1, t);
}

HCPoint HCCurveBaselineProjectionQuadratic(HCPoint p0, HCPoint p1, HCReal t) {
    // Compute quadratic baseline projection point for t
    HCReal tc = 1.0 - t;
    HCReal u = (tc * tc) / (t * t + tc * tc);
    HCReal uc = (1.0 - u);
    HCReal cx = u * p0.x + uc * p1.x;
    HCReal cy = u * p0.y + uc * p1.y;
    return HCPointMake(cx, cy);
}

HCPoint HCCurveBaselineProjectionCubic(HCPoint p0, HCPoint p1, HCReal t) {
    // Compute cubic baseline projection point for t
    HCReal tc = 1.0 - t;
    HCReal u = (tc * tc * tc) / (t * t * t + tc * tc * tc);
    HCReal uc = (1.0 - u);
    HCReal cx = u * p0.x + uc * p1.x;
    HCReal cy = u * p0.y + uc * p1.y;
    return HCPointMake(cx, cy);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Interpolation
//----------------------------------------------------------------------------------------------------------------------------------

HCCurve HCCurveInterpolatingPoint(HCPoint p0, HCPoint p1, HCPoint p, HCReal t, HCReal dx, HCReal dy) {
    HCCurve curve = HCCurveMakeLinear(p0, p1);
    HCCurveInterpolatingPointCubic(p0, p1, p, t, dx, dy, &curve.c0, &curve.c1);
    return curve;
}

void HCCurveInterpolatingPointLinear(HCPoint p0, HCPoint p, HCReal t, HCPoint* p1) {
    HCReal tc = 1.0 - t;
    
    // Find the end point that causes p to correspond to t
    if (p1 != NULL) {
        *p1 = HCPointMake((p.x - tc * p0.x) / t, (p.y - tc * p0.y) / t);
    }
}

void HCCurveInterpolatingPointQuadratic(HCPoint p0, HCPoint p1, HCPoint p, HCReal t, HCPoint* rc) {
    HCReal tc = 1.0 - t;
    
    // Find the projection of the quadratic curve point at t on its baseline
    HCPoint b = HCCurveBaselineProjectionQuadratic(p0, p1, t);
    
    // Given the baseline point and the desired on-curve point, calculate the control point projection whose relationship should remain fixed in the interpolating curve
    // With a quadratic curve, this projected point is the control point of the interpolated curve that makes the desired on-curve point be on-curve at t
    HCReal ratio = fabs((t * t + tc * tc - 1.0) / (t * t + tc * tc));
    HCReal cpx = p.x + (p.x - b.x) / ratio;
    HCReal cpy = p.y + (p.y - b.y) / ratio;
    
    // Deliver result
    if (rc != NULL) {
        *rc = HCPointMake(cpx, cpy);
    }
}

void HCCurveInterpolatingPointCubic(HCPoint p0, HCPoint p1, HCPoint p, HCReal t, HCReal dx, HCReal dy, HCPoint* rc0, HCPoint* rc1) {
    HCReal tc = 1.0 - t;
    
    // Find the projection of the cubic curve point at t on its baseline
    HCPoint b = HCCurveBaselineProjectionCubic(p0, p1, t);
    
    // Given the baseline point and the desired on-curve point, calculate the control point segment projection whose relationship should remain fixed in the interpolating curve
    HCReal ratio = fabs((t * t * t + tc * tc * tc - 1.0) / (t * t * t + tc * tc * tc));
    HCReal apx = p.x + (p.x - b.x) / ratio;
    HCReal apy = p.y + (p.y - b.y) / ratio;
    
    // Calculate the points on the curve bezier sub-control polygon of the desired curve just above the on-curve point according to the desired dx/dy
    HCPoint rpp0 = HCPointOffset(p, -t * dx / 3.0, -t * dy / 3.0);
    HCPoint rpp1 = HCPointOffset(rpp0, dx / 3.0, dy / 3.0);
    
    // Calculate the bezier sub-control polygon points leading to control points that make the desired on-curve point be on-curve at t
    HCReal qpp0x = (rpp0.x - t * apx) / tc;
    HCReal qpp0y = (rpp0.y - t * apy) / tc;
    HCReal qpp1x = (rpp1.x - tc * apx) / t;
    HCReal qpp1y = (rpp1.y - tc * apy) / t;
    HCReal cp0x = (qpp0x - tc * p0.x) / t;
    HCReal cp0y = (qpp0y - tc * p0.y) / t;
    HCReal cp1x = (qpp1x - t * p1.x) / tc;
    HCReal cp1y = (qpp1y - t * p1.y) / tc;
    
    // Deliver result
    if (rc0 != NULL) {
        *rc0 = HCPointMake(cp0x, cp0y);
    }
    if (rc1 != NULL) {
        *rc1 = HCPointMake(cp1x, cp1y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Moulding
//----------------------------------------------------------------------------------------------------------------------------------

HCCurve HCCurveMould(HCCurve curve, HCReal t, HCPoint p) {
    HCCurve moulded = curve;
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            moulded = curve;
        }
        else {
            HCCurveMouldQuadratic(curve.p0, curve.c0, curve.p1, t, p, &moulded.c0);
        }
    }
    else {
        HCCurveMouldCubic(curve.p0, curve.c0, curve.c1, curve.p1, t, p, &moulded.c0, &moulded.c1);
    }
    return moulded;
}

void HCCurveMouldLinear(HCPoint p0, HCPoint p1, HCReal t, HCPoint p) {
    // Linear curve cannot be moulded since its anchor points are the only contributing elements to its form
}

void HCCurveMouldQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t, HCPoint p, HCPoint* rc) {
    // Calculate the interpolating quadratic with the desired on-curve point and associatted t value
    HCCurveInterpolatingPointQuadratic(p0, p1, p, t, rc);
}

void HCCurveMouldCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t, HCPoint p, HCPoint* rc0, HCPoint* rc1) {
    // Find the derivative of the existing curve at t for use in calculating a matching interpolating curve
    HCPoint dp0 = HCPointInvalid;
    HCPoint  dc = HCPointInvalid;
    HCPoint dp1 = HCPointInvalid;
    HCCurveDerivativeCubic(p0, c0, c1, p1, &dp0, &dc, &dp1);
    HCPoint d = HCCurveValueQuadratic(dp0, dc, dp1, t);
    
    // Calculate the interpolating cubic with the desired on-curve point, associated t value, and derivative
    return HCCurveInterpolatingPointCubic(p0, p1, p, t, d.x, d.y, rc0, rc1);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Split
//----------------------------------------------------------------------------------------------------------------------------------
void HCCurveSplit(HCCurve curve, HCReal t, HCCurve* sCurve, HCCurve* eCurve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCPoint sp0 = HCPointZero;
            HCPoint sp1 = HCPointZero;
            HCPoint ep0 = HCPointZero;
            HCPoint ep1 = HCPointZero;
            HCCurveSplitLinear(curve.p0, curve.p1, t, &sp0, &sp1, &ep0, &ep1);
            if (sCurve != NULL) {
                *sCurve = HCCurveMakeLinear(sp0, sp1);
            }
            if (eCurve != NULL) {
                *eCurve = HCCurveMakeLinear(ep0, ep1);
            }
        }
        else {
            HCPoint sp0 = HCPointZero;
            HCPoint sc = HCPointZero;
            HCPoint sp1 = HCPointZero;
            HCPoint ep0 = HCPointZero;
            HCPoint ec = HCPointZero;
            HCPoint ep1 = HCPointZero;
            HCCurveSplitQuadratic(curve.p0, curve.c0, curve.p1, t, &sp0, &sc, &sp1, &ep0, &ec, &ep1);
            if (sCurve != NULL) {
                *sCurve = HCCurveMakeQuadratic(sp0, sc, sp1);
            }
            if (eCurve != NULL) {
                *eCurve = HCCurveMakeQuadratic(ep0, ec, ep1);
            }
        }
    }
    else {
        HCPoint sp0 = HCPointZero;
        HCPoint sc0 = HCPointZero;
        HCPoint sc1 = HCPointZero;
        HCPoint sp1 = HCPointZero;
        HCPoint ep0 = HCPointZero;
        HCPoint ec0 = HCPointZero;
        HCPoint ec1 = HCPointZero;
        HCPoint ep1 = HCPointZero;
        HCCurveSplitCubic(curve.p0, curve.c0, curve.c1, curve.p1, t, &sp0, &sc0, &sc1, &sp1, &ep0, &ec0, &ec1, &ep1);
        if (sCurve != NULL) {
            *sCurve = HCCurveMakeCubic(sp0, sc0, sc1, sp1);
        }
        if (eCurve != NULL) {
            *eCurve = HCCurveMakeCubic(ep0, ec0, ec1, ep1);
        }
    }
}

void HCCurveSplitLinear(HCPoint p0, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sp1, HCPoint* ep0, HCPoint* ep1) {
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

void HCCurveSplitQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sc, HCPoint* sp1, HCPoint* ep0, HCPoint* ec, HCPoint* ep1) {
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

void HCCurveSplitCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t, HCPoint* sp0, HCPoint* sc0, HCPoint* sc1, HCPoint* sp1, HCPoint* ep0, HCPoint* ec0, HCPoint* ec1, HCPoint* ep1) {
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
// MARK: - Axis Alignment
//----------------------------------------------------------------------------------------------------------------------------------
HCCurve HCCurveXAxisAligned(HCCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCPoint ap0 = HCPointInvalid;
            HCPoint ap1 = HCPointInvalid;
            HCCurveXAxisAlignedLinear(curve.p0, curve.p1, &ap0, &ap1);
            return HCCurveMakeLinear(ap0, ap1);
        }
        else {
            HCPoint ap0 = HCPointInvalid;
            HCPoint ac = HCPointInvalid;
            HCPoint ap1 = HCPointInvalid;
            HCCurveXAxisAlignedQuadratic(curve.p0, curve.c0, curve.p1, &ap0, &ac, &ap1);
            return HCCurveMakeQuadratic(ap0, ac, ap1);
        }
    }
    else {
        HCPoint ap0 = HCPointInvalid;
        HCPoint ac0 = HCPointInvalid;
        HCPoint ac1 = HCPointInvalid;
        HCPoint ap1 = HCPointInvalid;
        HCCurveXAxisAlignedCubic(curve.p0, curve.c0, curve.c1, curve.p1, &ap0, &ac0, &ac1, &ap1);
        return HCCurveMakeCubic(ap0, ac0, ac1, ap1);
    }
}

void HCCurveXAxisAlignedLinear(HCPoint p0, HCPoint p1, HCPoint* ap0, HCPoint* ap1) {
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

void HCCurveXAxisAlignedQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint* ap0, HCPoint* ac, HCPoint* ap1) {
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

void HCCurveXAxisAlignedCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint* ap0, HCPoint* ac0, HCPoint* ac1, HCPoint* ap1) {
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

HCCurve HCCurveYAxisAligned(HCCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCPoint ap0 = HCPointInvalid;
            HCPoint ap1 = HCPointInvalid;
            HCCurveYAxisAlignedLinear(curve.p0, curve.p1, &ap0, &ap1);
            return HCCurveMakeLinear(ap0, ap1);
        }
        else {
            HCPoint ap0 = HCPointInvalid;
            HCPoint ac = HCPointInvalid;
            HCPoint ap1 = HCPointInvalid;
            HCCurveYAxisAlignedQuadratic(curve.p0, curve.c0, curve.p1, &ap0, &ac, &ap1);
            return HCCurveMakeQuadratic(ap0, ac, ap1);
        }
    }
    else {
        HCPoint ap0 = HCPointInvalid;
        HCPoint ac0 = HCPointInvalid;
        HCPoint ac1 = HCPointInvalid;
        HCPoint ap1 = HCPointInvalid;
        HCCurveYAxisAlignedCubic(curve.p0, curve.c0, curve.c1, curve.p1, &ap0, &ac0, &ac1, &ap1);
        return HCCurveMakeCubic(ap0, ac0, ac1, ap1);
    }
}

void HCCurveYAxisAlignedLinear(HCPoint p0, HCPoint p1, HCPoint* ap0, HCPoint* ap1) {
    // Align to x-axis then rotate PI/2 to y-axis
    HCPoint xAlignedP1 = HCPointInvalid;
    HCCurveXAxisAlignedLinear(p0, p1, NULL, &xAlignedP1);
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

void HCCurveYAxisAlignedQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCPoint* ap0, HCPoint* ac, HCPoint* ap1) {
    // Align to x-axis then rotate PI/2 to y-axis
    HCPoint xAlignedC = HCPointInvalid;
    HCPoint xAlignedP1 = HCPointInvalid;
    HCCurveXAxisAlignedQuadratic(p0, c, p1, NULL, &xAlignedC, &xAlignedP1);
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

void HCCurveYAxisAlignedCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCPoint* ap0, HCPoint* ac0, HCPoint* ac1, HCPoint* ap1) {
    // Align to x-axis then rotate PI/2 to y-axis
    HCPoint xAlignedC0 = HCPointInvalid;
    HCPoint xAlignedC1 = HCPointInvalid;
    HCPoint xAlignedP1 = HCPointInvalid;
    HCCurveXAxisAlignedCubic(p0, c0, c1, p1, NULL, &xAlignedC0, &xAlignedC1, &xAlignedP1);
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
// MARK: - Intersection
//----------------------------------------------------------------------------------------------------------------------------------
void HCCurveIntersection(HCCurve pCurve, HCCurve qCurve, HCInteger* count, HCReal* t, HCReal* u) {
    if (HCCurveIsLinear(pCurve)) {
        if (HCCurveIsLinear(qCurve)) {
            HCCurveIntersectionLinearLinear(pCurve.p0, pCurve.p1, qCurve.p0, qCurve.p1, count, t, u);
        }
        else if (HCCurveIsQuadratic(qCurve)) {
            HCCurveIntersectionLinearQuadratic(pCurve.p0, pCurve.p1, qCurve.p0, qCurve.c0, qCurve.p1, count, t, u);
        }
        else {
            HCCurveIntersectionLinearCubic(pCurve.p0, pCurve.p1, qCurve.p0, qCurve.c0, qCurve.c1, qCurve.p1, count, t, u);
        }
    }
    else if (HCCurveIsQuadratic(pCurve)) {
        if (HCCurveIsLinear(qCurve)) {
            HCCurveIntersectionLinearQuadratic(qCurve.p0, qCurve.p1, pCurve.p0, pCurve.c0, pCurve.p1, count, u, t);
        }
        else if (HCCurveIsQuadratic(qCurve)) {
            HCCurveIntersectionQuadraticQuadratic(pCurve.p0, pCurve.c0, pCurve.p1, qCurve.p0, qCurve.c0, qCurve.p1, count, t, u);
        }
        else {
            return HCCurveIntersectionQuadraticCubic(pCurve.p0, pCurve.c0, pCurve.p1, qCurve.p0, qCurve.c0, qCurve.c1, qCurve.p1, count, t, u);
        }
    }
    else {
        return HCCurveIntersectionCubicCubic(pCurve.p0, pCurve.c0, pCurve.c1, pCurve.p1, qCurve.p0, qCurve.c0, qCurve.c1, qCurve.p1, count, t, u);
    }
}

void HCCurveIntersectionLinearLinear(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u) {
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

void HCCurveIntersectionLinearQuadratic(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint qc, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u) {
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
        HCPoint intersection = HCCurveValueQuadratic(q0, qc, q1, root);
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

void HCCurveIntersectionLinearCubic(HCPoint p0, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u) {
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
        HCPoint intersection = HCCurveValueCubic(q0, qc0, qc1, q1, root);
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

void HCCurveIntersectionQuadraticQuadraticRecursive(HCPoint p0, HCPoint pc, HCPoint p1, HCReal pts, HCReal pte, HCPoint q0, HCPoint qc, HCPoint q1, HCReal qts, HCReal qte, HCInteger* count, HCReal* t, HCReal* u) {
    // Stop searching if the maximum number of intersections for quadratic-quadratic pairings has been reached
    if (*count >= 2) {
        return;
    }
    
    // Determine if quadratic curve bounds rectangles overlap
    HCRectangle pr = HCCurveBoundsQuadratic(p0, pc, p1);
    HCRectangle qr = HCCurveBoundsQuadratic(q0, qc, q1);
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
    HCCurveSplitQuadratic(p0, pc, p1, pSplit, &psp0, &psc, &psp1, &pep0, &pec, &pep1);
    HCPoint qsp0 = HCPointInvalid;
    HCPoint qsc = HCPointInvalid;
    HCPoint qsp1 = HCPointInvalid;
    HCPoint qep0 = HCPointInvalid;
    HCPoint qec = HCPointInvalid;
    HCPoint qep1 = HCPointInvalid;
    HCCurveSplitQuadratic(q0, qc, q1, qSplit, &qsp0, &qsc, &qsp1, &qep0, &qec, &qep1);
    
    // Search for overlaps among the 4 resulting curves
    HCCurveIntersectionQuadraticQuadraticRecursive(psp0, psc, psp1, pts, pt, qsp0, qsc, qsp1, qts, qt, count, t, u);
    HCCurveIntersectionQuadraticQuadraticRecursive(psp0, psc, psp1, pts, pt, qep0, qec, qep1, qt, qte, count, t, u);
    HCCurveIntersectionQuadraticQuadraticRecursive(pep0, pec, pep1, pt, pte, qsp0, qsc, qsp1, qts, qt, count, t, u);
    HCCurveIntersectionQuadraticQuadraticRecursive(pep0, pec, pep1, pt, pte, qep0, qec, qep1, qt, qte, count, t, u);
}

void HCCurveIntersectionQuadraticQuadratic(HCPoint p0, HCPoint pc, HCPoint p1, HCPoint q0, HCPoint qc, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u) {
    // Determine if quadratic curve bounds rectangles overlap at all
    HCRectangle pr = HCCurveBoundsQuadratic(p0, pc, p1);
    HCRectangle qr = HCCurveBoundsQuadratic(q0, qc, q1);
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
    HCCurveIntersectionQuadraticQuadraticRecursive(p0, pc, p1, 0.0, 1.0, q0, qc, q1, 0.0, 1.0, &intersectionCount, ts, us);
    
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

void HCCurveIntersectionQuadraticCubicRecursive(HCPoint p0, HCPoint pc, HCPoint p1, HCReal pts, HCReal pte, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCReal qts, HCReal qte, HCInteger* count, HCReal* t, HCReal* u) {
    // Stop searching if the maximum number of intersections for quadratic-cubic pairings has been reached
    if (*count >= 6) {
        return;
    }
    
    // Determine if quadratic and cubic curve bounds rectangles overlap
    HCRectangle pr = HCCurveBoundsQuadratic(p0, pc, p1);
    HCRectangle qr = HCCurveBoundsCubic(q0, qc0, qc1, q1);
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
    HCCurveSplitQuadratic(p0, pc, p1, pSplit, &psp0, &psc, &psp1, &pep0, &pec, &pep1);
    HCPoint qsp0 = HCPointInvalid;
    HCPoint qsc0 = HCPointInvalid;
    HCPoint qsc1 = HCPointInvalid;
    HCPoint qsp1 = HCPointInvalid;
    HCPoint qep0 = HCPointInvalid;
    HCPoint qec0 = HCPointInvalid;
    HCPoint qec1 = HCPointInvalid;
    HCPoint qep1 = HCPointInvalid;
    HCCurveSplitCubic(q0, qc0, qc1, q1, qSplit, &qsp0, &qsc0, &qsc1, &qsp1, &qep0, &qec0, &qec1, &qep1);
    
    // Search for overlaps among the 4 resulting curves
    HCCurveIntersectionQuadraticCubicRecursive(psp0, psc, psp1, pts, pt, qsp0, qsc0, qsc1, qsp1, qts, qt, count, t, u);
    HCCurveIntersectionQuadraticCubicRecursive(psp0, psc, psp1, pts, pt, qep0, qec0, qec1, qep1, qt, qte, count, t, u);
    HCCurveIntersectionQuadraticCubicRecursive(pep0, pec, pep1, pt, pte, qsp0, qsc0, qsc1, qsp1, qts, qt, count, t, u);
    HCCurveIntersectionQuadraticCubicRecursive(pep0, pec, pep1, pt, pte, qep0, qec0, qec1, qep1, qt, qte, count, t, u);
}

void HCCurveIntersectionQuadraticCubic(HCPoint p0, HCPoint pc, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u) {
    // Determine if quadratic and cubic curve bounds rectangles overlap at all
    HCRectangle pr = HCCurveBoundsQuadratic(p0, pc, p1);
    HCRectangle qr = HCCurveBoundsCubic(q0, qc0, qc1, q1);
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
    HCCurveIntersectionQuadraticCubicRecursive(p0, pc, p1, 0.0, 1.0, q0, qc0, qc1, q1, 0.0, 1.0, &intersectionCount, ts, us);
    
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

void HCCurveIntersectionCubicCubicRecursive(HCPoint p0, HCPoint pc0, HCPoint pc1, HCPoint p1, HCReal pts, HCReal pte, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCReal qts, HCReal qte, HCInteger* count, HCReal* t, HCReal* u) {
    // Stop searching if the maximum number of intersections for cubic-cubic pairings has been reached
    if (*count >= 9) {
        return;
    }
    
    // Determine if cubic curve bounds rectangles overlap
    HCRectangle pr = HCCurveBoundsCubic(p0, pc0, pc1, p1);
    HCRectangle qr = HCCurveBoundsCubic(q0, qc0, qc1, q1);
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
    HCCurveSplitCubic(p0, pc0, pc1, p1, pSplit, &psp0, &psc0, &psc1, &psp1, &pep0, &pec0, &pec1, &pep1);
    HCPoint qsp0 = HCPointInvalid;
    HCPoint qsc0 = HCPointInvalid;
    HCPoint qsc1 = HCPointInvalid;
    HCPoint qsp1 = HCPointInvalid;
    HCPoint qep0 = HCPointInvalid;
    HCPoint qec0 = HCPointInvalid;
    HCPoint qec1 = HCPointInvalid;
    HCPoint qep1 = HCPointInvalid;
    HCCurveSplitCubic(q0, qc0, qc1, q1, qSplit, &qsp0, &qsc0, &qsc1, &qsp1, &qep0, &qec0, &qec1, &qep1);
    
    // Search for overlaps among the 4 resulting curves
    HCCurveIntersectionCubicCubicRecursive(psp0, psc0, psc1, psp1, pts, pt, qsp0, qsc0, qsc1, qsp1, qts, qt, count, t, u);
    HCCurveIntersectionCubicCubicRecursive(psp0, psc0, psc1, psp1, pts, pt, qep0, qec0, qec1, qep1, qt, qte, count, t, u);
    HCCurveIntersectionCubicCubicRecursive(pep0, pec0, pec1, pep1, pt, pte, qsp0, qsc0, qsc1, qsp1, qts, qt, count, t, u);
    HCCurveIntersectionCubicCubicRecursive(pep0, pec0, pec1, pep1, pt, pte, qep0, qec0, qec1, qep1, qt, qte, count, t, u);
}

void HCCurveIntersectionCubicCubic(HCPoint p0, HCPoint pc0, HCPoint pc1, HCPoint p1, HCPoint q0, HCPoint qc0, HCPoint qc1, HCPoint q1, HCInteger* count, HCReal* t, HCReal* u) {
    // Determine if cubic curve bounds rectangles overlap at all
    HCRectangle pr = HCCurveBoundsCubic(p0, pc0, pc1, p1);
    HCRectangle qr = HCCurveBoundsCubic(q0, qc0, qc1, q1);
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
    HCCurveIntersectionCubicCubicRecursive(p0, pc0, pc1, p1, 0.0, 1.0, q0, qc0, qc1, q1, 0.0, 1.0, &intersectionCount, ts, us);
    
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
