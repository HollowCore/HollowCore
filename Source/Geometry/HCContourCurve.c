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
    // Determine curve type based on provided information
    if (HCPointIsInvalid(p0) || HCPointIsInvalid(curve.p) || (HCPointIsInvalid(curve.c0) && !HCPointIsInvalid(curve.c1))) {
        // Provided curve data has invalid anchor points or has control point 1 but not control point 0
        return HCCurveTypeInvalid;
    }
    else if (HCPointIsInvalid(curve.c0)) {
        // Linear curve data provided
        return HCCurveCanonicalTypeLinear(p0, curve.p);
    }
    else if (HCPointIsInvalid(curve.c1)) {
        // Quadratic curve data provided
        return HCCurveCanonicalTypeQuadratic(p0, curve.c0, curve.p);
    }
    else {
        // Cubic curve data provided
        return HCCurveCanonicalTypeCubic(p0, curve.c0, curve.c1, curve.p);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourCurveValue(HCPoint p0, HCContourCurve curve, HCReal t) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveValueLinear(p0, curve.p, t);
        }
        else {
            return HCCurveValueQuadratic(p0, curve.c0, curve.p, t);
        }
    }
    else {
        return HCCurveValueCubic(p0, curve.c0, curve.c1, curve.p, t);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------

void HCContourCurveDerivative(HCPoint p0, HCContourCurve curve, HCPoint* dP0, HCContourCurve* dCurve) {
    HCCurveType type = HCContourCurveCanonicalType(p0, curve);
    switch (type) {
        case HCCurveTypeInvalid: {
            if (dP0 != NULL) {
                *dP0 = HCPointInvalid;
            }
            if (dCurve != NULL) {
                *dCurve = HCContourCurveInvalid;
            }
        } break;
        case HCCurveTypePoint:
        case HCCurveTypeLinear: {
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
        case HCCurveTypeQuadratic: {
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
        case HCCurveTypeCubicSimple:
        case HCCurveTypeCubicSingleInflection:
        case HCCurveTypeCubicDoubleInflection:
        case HCCurveTypeCubicLoop:
        case HCCurveTypeCubicLoopAtStart:
        case HCCurveTypeCubicLoopAtEnd:
        case HCCurveTypeCubicLoopClosed:
        case HCCurveTypeCubicCusp: {
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

HCReal HCContourCurveCurvature(HCPoint p0, HCContourCurve curve, HCReal t) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveCurvatureLinear(p0, curve.p, t);
        }
        else {
            return HCCurveCurvatureQuadratic(p0, curve.c0, curve.p, t);
        }
    }
    else {
        return HCCurveCurvatureCubic(p0, curve.c0, curve.c1, curve.p, t);
    }
}

void HCContourCurveExtrema(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* extrema) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCCurveExtremaLinear(p0, curve.p, count, extrema);
        }
        else {
            HCCurveExtremaQuadratic(p0, curve.c0, curve.p, count, extrema);
        }
    }
    else {
        HCCurveExtremaCubic(p0, curve.c0, curve.c1, curve.p, count, extrema);
    }
}

void HCContourCurveInflections(HCPoint p0, HCContourCurve curve, HCInteger* count, HCReal* inflections) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCCurveInflectionsLinear(p0, curve.p, count, inflections);
        }
        else {
            HCCurveInflectionsQuadratic(p0, curve.c0, curve.p, count, inflections);
        }
    }
    else {
        HCCurveInflectionsCubic(p0, curve.c0, curve.c1, curve.p, count, inflections);
    }
}

HCRectangle HCContourCurveApproximateBounds(HCPoint p0, HCContourCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveApproximateBoundsLinear(p0, curve.p);
        }
        else {
            return HCCurveApproximateBoundsQuadratic(p0, curve.c0, curve.p);
        }
    }
    else {
        return HCCurveApproximateBoundsCubic(p0, curve.c0, curve.c1, curve.p);
    }
}

HCRectangle HCContourCurveBounds(HCPoint p0, HCContourCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveBoundsLinear(p0, curve.p);
        }
        else {
            return HCCurveBoundsQuadratic(p0, curve.c0, curve.p);
        }
    }
    else {
        return HCCurveBoundsCubic(p0, curve.c0, curve.c1, curve.p);
    }
}

HCReal HCContourCurveLength(HCPoint p0, HCContourCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveLengthLinear(p0, curve.p);
        }
        else {
            return HCCurveLengthQuadratic(p0, curve.c0, curve.p);
        }
    }
    else {
        return HCCurveLengthCubic(p0, curve.c0, curve.c1, curve.p);
    }
}

HCReal HCContourCurveParameter(HCPoint p0, HCContourCurve curve, HCReal d) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveParameterLinear(p0, curve.p, d);
        }
        else {
            return HCCurveParameterQuadratic(p0, curve.c0, curve.p, d);
        }
    }
    else {
        return HCCurveParameterCubic(p0, curve.c0, curve.c1, curve.p, d);
    }
}

HCReal HCContourCurveParameterNearestPoint(HCPoint p0, HCContourCurve curve, HCPoint p) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveParameterNearestPointLinear(p0, curve.p, p);
        }
        else {
            return HCCurveParameterNearestPointQuadratic(p0, curve.c0, curve.p, p);
        }
    }
    else {
        return HCCurveParameterNearestPointCubic(p0, curve.c0, curve.c1, curve.p, p);
    }
}

HCReal HCContourCurveDistanceFromPoint(HCPoint p0, HCContourCurve curve, HCPoint p) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveDistanceFromPointLinear(p0, curve.p, p);
        }
        else {
            return HCCurveDistanceFromPointQuadratic(p0, curve.c0, curve.p, p);
        }
    }
    else {
        return HCCurveDistanceFromPointCubic(p0, curve.c0, curve.c1, curve.p, p);
    }
}

HCPoint HCContourCurveBaselineProjection(HCPoint p0, HCContourCurve curve, HCReal t) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            return HCCurveBaselineProjectionLinear(p0, curve.p, t);
        }
        else {
            return HCCurveBaselineProjectionQuadratic(p0, curve.p, t);
        }
    }
    else {
        return HCCurveBaselineProjectionCubic(p0, curve.p, t);
    }
}

HCContourCurve HCContourCurveInterpolatingPoint(HCPoint p0, HCPoint p1, HCPoint p, HCReal t, HCReal dx, HCReal dy) {
    HCContourCurve curve = HCContourCurveInvalid;
    HCCurveInterpolatingPointCubic(p0, p1, p, t, dx, dy, &curve.c0, &curve.c1);
    return curve;
}

HCContourCurve HCContourCurveMould(HCPoint p0, HCContourCurve curve, HCReal t, HCPoint p) {
    HCContourCurve moulded = curve;
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            moulded = curve;
        }
        else {
            HCCurveMouldQuadratic(p0, curve.c0, curve.p, t, p, &moulded.c0);
        }
    }
    else {
        HCCurveMouldCubic(p0, curve.c0, curve.c1, curve.p, t, p, &moulded.c0, &moulded.c1);
    }
    return moulded;
}

void HCContourCurveSplit(HCPoint p0, HCContourCurve curve, HCReal t, HCPoint* sp0, HCContourCurve* sCurve, HCPoint* ep0, HCContourCurve* eCurve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCPoint sp1 = HCPointZero;
            HCPoint ep1 = HCPointZero;
            HCCurveSplitLinear(p0, curve.p, t, sp0, &sp1, ep0, &ep1);
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
            HCCurveSplitQuadratic(p0, curve.c0, curve.p, t, sp0, &sc, &sp1, ep0, &ec, &ep1);
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
        HCCurveSplitCubic(p0, curve.c0, curve.c1, curve.p, t, sp0, &sc0, &sc1, &sp1, ep0, &ec0, &ec1, &ep1);
        if (sCurve != NULL) {
            *sCurve = HCContourCurveMakeCubic(sc0, sc1, sp1);
        }
        if (eCurve != NULL) {
            *eCurve = HCContourCurveMakeCubic(ec0, ec1, ep1);
        }
    }
}

HCContourCurve HCContourCurveXAxisAligned(HCPoint p0, HCContourCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCPoint ap1 = HCPointInvalid;
            HCCurveXAxisAlignedLinear(p0, curve.p, NULL, &ap1);
            return HCContourCurveMakeLinear(ap1);
        }
        else {
            HCPoint ac = HCPointInvalid;
            HCPoint ap1 = HCPointInvalid;
            HCCurveXAxisAlignedQuadratic(p0, curve.c0, curve.p, NULL, &ac, &ap1);
            return HCContourCurveMakeQuadratic(ac, ap1);
        }
    }
    else {
        HCPoint ac0 = HCPointInvalid;
        HCPoint ac1 = HCPointInvalid;
        HCPoint ap1 = HCPointInvalid;
        HCCurveXAxisAlignedCubic(p0, curve.c0, curve.c1, curve.p, NULL, &ac0, &ac1, &ap1);
        return HCContourCurveMakeCubic(ac0, ac1, ap1);
    }
}

HCContourCurve HCContourCurveYAxisAligned(HCPoint p0, HCContourCurve curve) {
    if (HCPointIsInvalid(curve.c1)) {
        if (HCPointIsInvalid(curve.c0)) {
            HCPoint ap1 = HCPointInvalid;
            HCCurveYAxisAlignedLinear(p0, curve.p, NULL, &ap1);
            return HCContourCurveMakeLinear(ap1);
        }
        else {
            HCPoint ac = HCPointInvalid;
            HCPoint ap1 = HCPointInvalid;
            HCCurveYAxisAlignedQuadratic(p0, curve.c0, curve.p, NULL, &ac, &ap1);
            return HCContourCurveMakeQuadratic(ac, ap1);
        }
    }
    else {
        HCPoint ac0 = HCPointInvalid;
        HCPoint ac1 = HCPointInvalid;
        HCPoint ap1 = HCPointInvalid;
        HCCurveYAxisAlignedCubic(p0, curve.c0, curve.c1, curve.p, NULL, &ac0, &ac1, &ap1);
        return HCContourCurveMakeCubic(ac0, ac1, ap1);
    }
}

void HCContourCurveIntersection(HCPoint p0, HCContourCurve pCurve, HCPoint q0, HCContourCurve qCurve, HCInteger* count, HCReal* t, HCReal* u) {
    if (HCContourCurveIsLinear(p0, pCurve)) {
        if (HCContourCurveIsLinear(p0, qCurve)) {
            HCCurveIntersectionLinearLinear(p0, pCurve.p, q0, qCurve.p, count, t, u);
        }
        else if (HCContourCurveIsQuadratic(p0, qCurve)) {
            HCCurveIntersectionLinearQuadratic(p0, pCurve.p, q0, qCurve.c0, qCurve.p, count, t, u);
        }
        else {
            HCCurveIntersectionLinearCubic(p0, pCurve.p, q0, qCurve.c0, qCurve.c1, qCurve.p, count, t, u);
        }
    }
    else if (HCContourCurveIsQuadratic(p0, pCurve)) {
        if (HCContourCurveIsLinear(p0, qCurve)) {
            HCCurveIntersectionLinearQuadratic(q0, qCurve.p, p0, pCurve.c0, pCurve.p, count, u, t);
        }
        else if (HCContourCurveIsQuadratic(p0, qCurve)) {
            HCCurveIntersectionQuadraticQuadratic(p0, pCurve.c0, pCurve.p, q0, qCurve.c0, qCurve.p, count, t, u);
        }
        else {
            return HCCurveIntersectionQuadraticCubic(p0, pCurve.c0, pCurve.p, q0, qCurve.c0, qCurve.c1, qCurve.p, count, t, u);
        }
    }
    else {
        return HCCurveIntersectionCubicCubic(p0, pCurve.c0, pCurve.c1, pCurve.p, q0, qCurve.c0, qCurve.c1, qCurve.p, count, t, u);
    }
}
