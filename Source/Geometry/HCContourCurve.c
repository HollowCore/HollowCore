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

#include "HCContourCurve.h"
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

HCContourCurve HCContourCurveMakeWithCurve(HCCurve curve) {
    return (HCContourCurve){ .c0 = curve.c0, .c1 = curve.c1, .p = curve.p1 };
}

HCCurve HCCurveMakeWithContourCurve(HCPoint p0, HCContourCurve curve) {
    return (HCCurve){ .p0 = p0, .c0 = curve.c0, .c1 = curve.c1, .p1 = curve.p };
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
HCCurveType HCContourCurveCanonicalType(HCPoint p0, HCContourCurve curve) {
    return HCCurveCanonicalType(HCCurveMakeWithContourCurve(p0, curve));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourCurveValue(HCPoint p0, HCContourCurve curve, HCReal t) {
    return HCCurveValue(HCCurveMakeWithContourCurve(p0, curve), t);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------

HCContourCurve HCContourCurveTangent(HCPoint p0, HCContourCurve curve, HCReal t) {
    return HCContourCurveMakeWithCurve(HCCurveTangent(HCCurveMakeWithContourCurve(p0, curve), t));
}

HCContourCurve HCContourCurveTangentUnit(HCPoint p0, HCContourCurve curve, HCReal t) {
    return HCContourCurveMakeWithCurve(HCCurveTangentUnit(HCCurveMakeWithContourCurve(p0, curve), t));
}

HCContourCurve HCContourCurveNormal(HCPoint p0, HCContourCurve curve, HCReal t) {
    return HCContourCurveMakeWithCurve(HCCurveNormal(HCCurveMakeWithContourCurve(p0, curve), t));
}

HCContourCurve HCContourCurveNormalUnit(HCPoint p0, HCContourCurve curve, HCReal t) {
   return HCContourCurveMakeWithCurve(HCCurveNormalUnit(HCCurveMakeWithContourCurve(p0, curve), t));
}

void HCContourCurveDerivative(HCPoint p0, HCContourCurve curve, HCPoint* dP0, HCContourCurve* dCurve) {
    HCCurve derivative = HCCurveDerivative(HCCurveMakeWithContourCurve(p0, curve));
    if (dP0 != NULL) {
        *dP0 = derivative.p0;
    }
    if (dCurve != NULL) {
        *dCurve = HCContourCurveMakeWithCurve(derivative);
    }
}

HCReal HCContourCurveCurvature(HCPoint p0, HCContourCurve curve, HCReal t) {
    return HCCurveCurvature(HCCurveMakeWithContourCurve(p0, curve), t);
}

void HCContourCurveExtrema(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* extrema) {
    HCCurveExtrema(HCCurveMakeWithContourCurve(p0, curve), count, extrema);
}

void HCContourCurveInflections(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* inflections) {
    HCCurveInflections(HCCurveMakeWithContourCurve(p0, curve), count, inflections);
}

HCRectangle HCContourCurveApproximateBounds(HCPoint p0, HCContourCurve curve) {
    return HCCurveApproximateBounds(HCCurveMakeWithContourCurve(p0, curve));
}

HCRectangle HCContourCurveBounds(HCPoint p0, HCContourCurve curve) {
    return HCCurveBounds(HCCurveMakeWithContourCurve(p0, curve));
}

HCReal HCContourCurveLength(HCPoint p0, HCContourCurve curve) {
    return HCCurveLength(HCCurveMakeWithContourCurve(p0, curve));
}

HCReal HCContourCurveParameterAtLength(HCPoint p0, HCContourCurve curve, HCReal d) {
    return HCCurveParameterAtLength(HCCurveMakeWithContourCurve(p0, curve), d);
}

HCReal HCContourCurveParameterNearestPoint(HCPoint p0, HCContourCurve curve, HCPoint p) {
    return HCCurveParameterNearestPoint(HCCurveMakeWithContourCurve(p0, curve), p);
}

HCReal HCContourCurveDistanceFromPoint(HCPoint p0, HCContourCurve curve, HCPoint p) {
    return HCCurveDistanceFromPoint(HCCurveMakeWithContourCurve(p0, curve), p);
}

HCPoint HCContourCurveBaselineProjection(HCPoint p0, HCContourCurve curve, HCReal t) {
    return HCCurveBaselineProjection(HCCurveMakeWithContourCurve(p0, curve), t);
}

HCContourCurve HCContourCurveInterpolatingPoint(HCPoint p0, HCPoint p1, HCPoint p, HCReal t, HCReal dx, HCReal dy) {
    HCContourCurve curve = HCContourCurveInvalid;
    HCCurveInterpolatingPointCubic(p0, p1, p, t, dx, dy, &curve.c0, &curve.c1);
    return curve;
}

HCContourCurve HCContourCurveMould(HCPoint p0, HCContourCurve curve, HCReal t, HCPoint p) {
    return HCContourCurveMakeWithCurve(HCCurveMould(HCCurveMakeWithContourCurve(p0, curve), t, p));
}

void HCContourCurveSplit(HCPoint p0, HCContourCurve curve, HCReal t, HCPoint* sp0, HCContourCurve* sCurve, HCPoint* ep0, HCContourCurve* eCurve) {
    HCCurve s = HCCurveInvalid;
    HCCurve e = HCCurveInvalid;
    HCCurveSplit(HCCurveMakeWithContourCurve(p0, curve), t, &s, &e);
    if (sp0 != NULL) {
        *sp0 = s.p0;
    }
    if (sCurve != NULL) {
        *sCurve = HCContourCurveMakeWithCurve(s);
    }
    if (ep0 != NULL) {
        *ep0 = e.p0;
    }
    if (eCurve != NULL) {
        *eCurve = HCContourCurveMakeWithCurve(e);
    }
}

HCContourCurve HCContourCurveXAxisAligned(HCPoint p0, HCContourCurve curve) {
    return HCContourCurveMakeWithCurve(HCCurveXAxisAligned(HCCurveMakeWithContourCurve(p0, curve)));
}

HCContourCurve HCContourCurveYAxisAligned(HCPoint p0, HCContourCurve curve) {
    return HCContourCurveMakeWithCurve(HCCurveYAxisAligned(HCCurveMakeWithContourCurve(p0, curve)));
}

void HCContourCurveIntersection(HCPoint p0, HCContourCurve pCurve, HCPoint q0, HCContourCurve qCurve, HCInteger* count, HCReal* t, HCReal* u) {
    HCCurveIntersection(HCCurveMakeWithContourCurve(p0, pCurve), HCCurveMakeWithContourCurve(q0, qCurve), count, t, u);
}
