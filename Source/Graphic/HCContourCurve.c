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
// MARK: - Order / Conversion
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
        // TODO: Does this match curvature with the original cubic?
        return HCContourCurveMakeCubic(curve.c0, curve.c0, curve.p);
    }
    return HCContourCurveMakeCubic(curve.p, curve.p, curve.p);
}

HCContourCurve HCContourCurveXAxisAligned(HCPoint p0, HCContourCurve curve) {
    // Translate curve to origin
    HCContourCurve translated = curve;
    translated.c0.x -= p0.x;
    translated.c0.y -= p0.y;
    translated.c1.x -= p0.x;
    translated.c1.y -= p0.y;
    translated.p.x -= p0.x;
    translated.p.y -= p0.y;
    
    // Calculate angle of end point
    HCReal angle = atan2(translated.p.y, translated.p.x);
    
    // Rotate curve by negated angle to align the end point to the x-axis
    HCReal cosAngle = cos(-angle);
    HCReal sinAngle = sin(-angle);
    HCReal c0x = cosAngle * translated.c0.x - sinAngle * translated.c0.y;
    HCReal c0y = sinAngle * translated.c0.x + cosAngle * translated.c0.y;
    HCReal c1x = cosAngle * translated.c1.x - sinAngle * translated.c1.y;
    HCReal c1y = sinAngle * translated.c1.x + cosAngle * translated.c1.y;
    HCReal p1x = cosAngle * translated.p.x - sinAngle * translated.p.y;
    HCReal p1y = sinAngle * translated.p.x + cosAngle * translated.p.y;

    return HCContourCurveMakeCubic(HCPointMake(c0x, c0y), HCPointMake(c1x, c1y), HCPointMake(p1x, p1y));
}

HCContourCurve HCContourCurveYAxisAligned(HCPoint p0, HCContourCurve curve) {
    // Align to x-axis then rotate PI/2 to y-axis
    HCContourCurve xAligned = HCContourCurveXAxisAligned(p0, curve);
    HCReal c0x = -xAligned.c0.y;
    HCReal c0y = +xAligned.c0.x;
    HCReal c1x = -xAligned.c1.y;
    HCReal c1y = +xAligned.c1.x;
    HCReal p1x = -xAligned.p.y;
    HCReal p1y = +xAligned.p.x;
    return HCContourCurveMakeCubic(HCPointMake(c0x, c0y), HCPointMake(c1x, c1y), HCPointMake(p1x, p1y));
}

HCContourCurveType HCContourCurveCanonicalType(HCPoint p0, HCContourCurve curve) {
    // Determine curve type based on provided information
    if (HCPointIsInvalid(p0) || HCPointIsInvalid(curve.p) || (HCPointIsInvalid(curve.c0) && !HCPointIsInvalid(curve.c1))) {
        // Provided curve data has invalid anchor points or control point 1 but not control point 0
        return HCContourCurveTypeInvalid;
    }
    else if (HCPointIsInvalid(curve.c0)) {
        // Linear curve data provided
        // Determine if linear curve is a point or a line segment
        return HCPointIsEqual(p0, curve.p) ? HCContourCurveTypePoint : HCContourCurveTypeLinear;
    }
    else if (HCPointIsInvalid(curve.c1)) {
        // Quadratic curve data provided
        
        // Determine if quadratic curve has degenerate points
        HCBoolean p0p1 = HCPointIsEqual(p0, curve.p);
        HCBoolean p0c = HCPointIsEqual(p0, curve.c0);
        HCBoolean p1c = HCPointIsEqual(curve.p, curve.c0);
        if (p0p1 && p0c) {
            return HCContourCurveTypePoint;
        }
        if (p0c || p1c) {
            return HCContourCurveTypeLinear;
        }
        
        // Determine if quadratic curve points are co-linear
        HCPoint c = curve.c0;
        HCPoint p1 = curve.p;
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
    else {
        // Determine if cubic curve has degenerate points
        HCBoolean p0p1 = HCPointIsEqual(p0, curve.p);
        HCBoolean p0c0 = HCPointIsEqual(p0, curve.c0);
        HCBoolean p0c1 = HCPointIsEqual(p0, curve.c1);
        HCBoolean p1c0 = HCPointIsEqual(curve.p, curve.c0);
        HCBoolean p1c1 = HCPointIsEqual(curve.p, curve.c1);
        HCBoolean c0c1 = HCPointIsEqual(curve.c0, curve.c1);
        if (p0p1 && p0c0 && p0c1) {
            return HCContourCurveTypePoint;
        }
        if ((p0c0 && p1c1) || (p0c1 && p1c0)) {
            return HCContourCurveTypeLinear;
        }
        if (p0c1 || p1c0) {
            // TODO: Is quadratic?
            return HCContourCurveTypeCubicSimple;
        }
        if (c0c1) {
            return HCContourCurveTypeCubicSimple;
        }
        
        // Determine if cubic curve points are co-linear
        HCPoint c0 = curve.c0;
        HCPoint c1 = curve.c1;
        HCPoint p1 = curve.p;
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
        // TODO: Care about co-quadratic curves? Can use canonical curve coordinates for this?
//        if ((curve.c0.x == (3.0/2.0) * curve.p.x + curve.c1.x) && (curve.c0.y == (3.0/2.0) * curve.p.y + curve.c1.y)) {
//            return HCContourCurveTypeQuadratic;
//        }
        
        // Calculate canonical curve end point (other points are p0: (0, 0), c0: (0, 1), c1: (1,1))
        HCPoint p = HCContourCurveCanonical(p0, curve);
        
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
}

HCPoint HCContourCurveCanonical(HCPoint p0, HCContourCurve curve) {
    // Translate curve to origin
    HCContourCurve translated = curve;
    translated.c0.x -= p0.x;
    translated.c0.y -= p0.y;
    translated.c1.x -= p0.x;
    translated.c1.y -= p0.y;
    translated.p.x -= p0.x;
    translated.p.y -= p0.y;

    // Calculate ratio of curve y coordinates
    HCReal yci = 1.0 / translated.c0.y;
    HCReal ycc = translated.c1.y * yci;
    HCReal ypc = translated.p.y * yci;

    // Calculate canonical curve end point given p0: (0, 0), c0: (0, 1), c1: (1,1)
    HCReal p1x = (translated.p.x - translated.c0.x * ypc) / (translated.c1.x - translated.c0.x * ycc);
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
// MARK: - Curvature
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourCurveCurvature(HCPoint p0, HCContourCurve curve, HCReal t) {
    // TODO: This!
    return HCPointInvalid;
}

HCPoint HCContourCurveCurvatureLinear(HCPoint p0, HCPoint p1, HCReal t) {
    // TODO: This!
    return HCPointInvalid;
}

HCPoint HCContourCurveCurvatureQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t) {
    // TODO: This!
    return HCPointInvalid;
}

HCPoint HCContourCurveCurvatureCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t) {
    // TODO: This!
    return HCPointInvalid;
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
    // Add end points as extrema
    extrema[(*count)++] = 0.0;
    extrema[(*count)++] = 1.0;
}

void HCContourCurveExtremaQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCInteger* count, HCReal* extrema) {
    // Calculate derivative weights
    HCPoint p0d = HCPointMake(2.0 * (c.x - p0.x), 2.0 * (c.y - p0.y));
    HCPoint p1d = HCPointMake(2.0 * (p1.x - c.x), 2.0 * (p1.y - c.y));
    
    // Calculate zero crossing of derivative in x and add as extrema if in range
    HCReal ax = 1.0 * p0d.x - 1.0 * p1d.x;
    HCReal bx = 1.0 * p1d.x;
    HCReal xt = -bx / ax;
    if (xt >= 0.0 && xt <= 1.0) {
        extrema[(*count)++] = xt;
    }
    
    // Calculate zero crossing of derivative in y and add as extrema if in range
    HCReal ay = 1.0 * p0d.y - 1.0 * p1d.y;
    HCReal by = 1.0 * p1d.y;
    HCReal yt = -by / ay;
    if (yt >= 0.0 && yt <= 1.0) {
        extrema[(*count)++] = yt;
    }
    
    // Add extrema of second derivative
    HCContourCurveExtremaLinear(p0d, p1d, count, extrema);
}

void HCContourCurveExtremaCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCInteger* count, HCReal* extrema) {
    // Calculate derivative weights
    HCPoint p0d = HCPointMake(3.0 * (c0.x - p0.x), 3.0 * (c0.y - p0.y));
    HCPoint  cd = HCPointMake(3.0 * (c1.x - c0.x), 3.0 * (c1.y - c0.y));
    HCPoint p1d = HCPointMake(3.0 * (p1.x - c1.x), 3.0 * (p1.y - c1.y));
    
    // Calculate zero crossing of derivative in x add as extrema if real and in range
    HCReal ax = +1.0 * p0d.x - 2.0 *  cd.x + 1.0 * p1d.x;
    HCReal bx = -2.0 *  cd.x + 2.0 * p1d.x;
    HCReal cx = +1.0 * p1d.x;
    HCReal discriminantX = bx * bx - 4.0 * ax * cx;
    if (discriminantX >= 0.0) {
        HCReal sqrtDiscriminantX = sqrt(discriminantX);
        HCReal denominatorInverse = 1.0 / (2.0 * ax);
        HCReal xt0 = (-bx + sqrtDiscriminantX) * denominatorInverse;
        if (xt0 >= 0.0 && xt0 <= 1.0) {
            extrema[(*count)++] = xt0;
        }
        HCReal xt1 = (-bx - sqrtDiscriminantX) * denominatorInverse;
        if (xt1 >= 0.0 && xt1 <= 1.0) {
            extrema[(*count)++] = xt1;
        }
    }
    
    // Calculate zero crossing of derivative in y add as extrema if real and in range
    HCReal ay = +1.0 * p0d.y - 2.0 *  cd.y + 1.0 * p1d.y;
    HCReal by = -2.0 *  cd.y + 2.0 * p1d.y;
    HCReal cy = +1.0 * p1d.y;
    HCReal discriminantY = by * by - 4.0 * ay * cy;
    if (discriminantY >= 0.0) {
        HCReal sqrtDiscriminantY = sqrt(discriminantY);
        HCReal denominatorInverse = 1.0 / (2.0 * ay);
        HCReal yt0 = (-by + sqrtDiscriminantY) * denominatorInverse;
        if (yt0 >= 0.0 && yt0 <= 1.0) {
            extrema[(*count)++] = yt0;
        }
        HCReal yt1 = (-by - sqrtDiscriminantY) * denominatorInverse;
        if (yt1 >= 0.0 && yt1 <= 1.0) {
            extrema[(*count)++] = yt1;
        }
    }
    
    // Add extrema of second derivative
    HCContourCurveExtremaQuadratic(p0d, cd, p1d, count, extrema);
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
    // Find min/max of linear extrema (always p0 and p1)
    HCReal minX = fmin(p0.x, p1.x);
    HCReal minY = fmin(p0.y, p1.y);
    HCReal maxX = fmax(p0.x, p1.x);
    HCReal maxY = fmax(p0.y, p1.y);
    
    // Calculate bounding rectangle from min/max of extrema
    return HCRectangleMake(HCPointMake(minX, minY), HCSizeMake(maxX - minX, maxY - minY));
}

HCRectangle HCContourCurveBoundsQuadratic(HCPoint p0, HCPoint c, HCPoint p1) {
    // Calculate quadratic curve extrema
    HCReal extrema[4];
    HCInteger count = 0;
    HCContourCurveExtremaQuadratic(p0, c, p1, &count, extrema);
    
    // Find min/max of extrema
    HCReal minX = DBL_MAX;
    HCReal minY = DBL_MAX;
    HCReal maxX = -DBL_MAX;
    HCReal maxY = -DBL_MAX;
    for (HCInteger i = 0; i < count; i++) {
        HCReal t = extrema[i];
        HCPoint p = HCContourCurveValueQuadratic(p0, c, p1, t);
        minX = fmin(minX, p.x);
        minY = fmin(minY, p.y);
        maxX = fmax(maxX, p.x);
        maxY = fmax(maxY, p.y);
    }
    
    // Calculate bounding rectangle from min/max of extrema
    return HCRectangleMake(HCPointMake(minX, minY), HCSizeMake(maxX - minX, maxY - minY));
}

HCRectangle HCContourCurveBoundsCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1) {
    // Calculate cubic curve extrema
    HCReal extrema[8];
    HCInteger count = 0;
    HCContourCurveExtremaCubic(p0, c0, c1, p1, &count, extrema);
    
    // Find min/max of extrema
    HCReal minX = DBL_MAX;
    HCReal minY = DBL_MAX;
    HCReal maxX = -DBL_MAX;
    HCReal maxY = -DBL_MAX;
    for (HCInteger i = 0; i < count; i++) {
        HCReal t = extrema[i];
        HCPoint p = HCContourCurveValueCubic(p0, c0, c1, p1, t);
        minX = fmin(minX, p.x);
        minY = fmin(minY, p.y);
        maxX = fmax(maxX, p.x);
        maxY = fmax(maxY, p.y);
    }
    
    // Calculate bounding rectangle from min/max of extrema
    return HCRectangleMake(HCPointMake(minX, minY), HCSizeMake(maxX - minX, maxY - minY));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Length
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCContourCurveLength(HCPoint p0, HCContourCurve curve, HCReal t) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCContourCurveLengthLinear(p0, curve.p, t);
        }
        return HCContourCurveLengthQuadratic(p0, curve.c0, curve.p, t);
    }
    return HCContourCurveLengthCubic(p0, curve.c0, curve.c1, curve.p, t);
}

HCReal HCContourCurveLengthLinear(HCPoint p0, HCPoint p1, HCReal t) {
    // Calculate using linear distance equation
    return sqrt(p0.x * p0.x + p1.x * p1.x) * t;
}

HCReal HCContourCurveLengthQuadratic(HCPoint p0, HCPoint c, HCPoint p1, HCReal t) {
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

HCReal HCContourCurveLengthCubic(HCPoint p0, HCPoint c0, HCPoint c1, HCPoint p1, HCReal t) {
    // TODO: Use Legendre-Gauss to numerically calculate length
    return NAN;
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
// MARK: - Intersection
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveIntersection(HCPoint p0, HCContourCurve pCurve, HCPoint q0, HCContourCurve qCurve, HCReal* t, HCReal* u) {
    if (HCContourCurveIsLinear(p0, pCurve)) {
        if (HCContourCurveIsLinear(p0, qCurve)) {
            HCContourCurveLinearLinearIntersection(p0, pCurve.p, q0, qCurve.p, t, u);
        }
        if (HCContourCurveIsQuadratic(p0, qCurve)) {
            HCContourCurveLinearQuadraticIntersection(p0, pCurve.p, q0, qCurve.c0, qCurve.p, t, u);
        }
        HCContourCurveLinearCubicIntersection(p0, pCurve.p, q0, qCurve.c0, qCurve.c1, qCurve.p, t, u);
    }
    if (HCContourCurveIsQuadratic(p0, pCurve)) {
        if (HCContourCurveIsLinear(p0, qCurve)) {
            HCContourCurveLinearQuadraticIntersection(q0, qCurve.p, p0, pCurve.c0, pCurve.p, u, t);
        }
        if (HCContourCurveIsQuadratic(p0, qCurve)) {
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
// MARK: - Projectioon
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
// MARK: - Split
//----------------------------------------------------------------------------------------------------------------------------------
void HCContourCurveSplit(HCPoint p0, HCContourCurve curve, HCReal t, HCPoint* sp0, HCContourCurve* sCurve, HCPoint* ep0, HCContourCurve* eCurve) {
    // TODO: This!
    *sp0 = HCPointInvalid;
    *sCurve = HCContourCurveInvalid;
    *ep0 = HCPointInvalid;
    *eCurve = HCContourCurveInvalid;
}
