///
/// @file HCContourComponent.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 2/11/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///
/// @see https://pomax.github.io/bezierinfo
///

#include "HCContourComponent.h"
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCContourComponent HCContourComponentMakeLinear(HCPoint p) {
    return (HCContourComponent){ .c0 = HCPointInvalid, .c1 = HCPointInvalid, .p = p };
}

HCContourComponent HCContourComponentMakeQuadratic(HCPoint c, HCPoint p) {
    return (HCContourComponent){ .c0 = c, .c1 = HCPointInvalid, .p = p };
}

HCContourComponent HCContourComponentMakeCubic(HCPoint c0, HCPoint c1, HCPoint p) {
    return (HCContourComponent){ .c0 = c0, .c1 = c1, .p = p };
}

HCContourComponent HCContourComponentMakeWithCurve(HCCurve curve) {
    return (HCContourComponent){ .c0 = curve.c0, .c1 = curve.c1, .p = curve.p1 };
}

HCCurve HCCurveMakeWithContourComponent(HCPoint p0, HCContourComponent component) {
    return (HCCurve){ .p0 = p0, .c0 = component.c0, .c1 = component.c1, .p1 = component.p };
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourComponentIsInvalid(HCContourComponent component) {
    return
        HCPointIsInvalid(component.c0) &&
        HCPointIsInvalid(component.c1) &&
        HCPointIsInvalid(component.p);
}

HCBoolean HCContourComponentIsSimilar(HCContourComponent component, HCContourComponent other, HCReal axisDissimilarity) {
    return
        (HCPointIsSimilar(component.c0, other.c0, axisDissimilarity) || (HCPointIsInvalid(component.c0) && HCPointIsInvalid(other.c0))) &&
        (HCPointIsSimilar(component.c1, other.c1, axisDissimilarity) || (HCPointIsInvalid(component.c1) && HCPointIsInvalid(other.c1))) &&
        HCPointIsSimilar(component.p, other.p, axisDissimilarity);
}

HCBoolean HCContourComponentIsZero(HCContourComponent component) {
    return
        (HCPointIsZero(component.c0) || HCPointIsInvalid(component.c0)) &&
        (HCPointIsZero(component.c1) || HCPointIsInvalid(component.c1)) &&
        HCPointIsZero(component.p);
}

HCBoolean HCContourComponentIsInfinite(HCContourComponent component) {
    return
        HCPointIsInfinite(component.c0) ||
        HCPointIsInfinite(component.c1) ||
        HCPointIsInfinite(component.p);
}

HCBoolean HCContourComponentIsEqual(HCContourComponent component, HCContourComponent other) {
    return
        (HCPointIsEqual(component.c0, other.c0) || (HCPointIsInvalid(component.c0) && HCPointIsInvalid(other.c0))) &&
        (HCPointIsEqual(component.c1, other.c1) || (HCPointIsInvalid(component.c1) && HCPointIsInvalid(other.c1))) &&
        HCPointIsEqual(component.p, other.p);
}

HCInteger HCContourComponentHashValue(HCContourComponent component) {
    return
        HCPointHashValue(component.c0) ^
        HCPointHashValue(component.c1) ^
        HCPointHashValue(component.p);
}

void HCContourComponentPrint(HCContourComponent component, FILE* stream) {
    fprintf(stream, "<c0:");
    HCPointPrint(component.c0, stream);
    fprintf(stream, ",c1:");
    HCPointPrint(component.c1, stream);
    fprintf(stream, ",p:");
    HCPointPrint(component.p, stream);
    fprintf(stream, ">");
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Order
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourComponentIsLinear(HCPoint p0, HCContourComponent component) {
    if (HCPointIsInvalid(p0) || HCPointIsInvalid(component.p)) {
        return false;
    }
    if (HCPointIsInvalid(component.c0) && HCPointIsInvalid(component.c1)) {
        return true;
    }
    return false;
}

HCContourComponent HCContourComponentAsLinear(HCPoint p0, HCContourComponent component) {
    (void)p0; // Unused
    return HCContourComponentMakeLinear(component.p);
}

HCBoolean HCContourComponentIsQuadratic(HCPoint p0, HCContourComponent component) {
    if (HCPointIsInvalid(p0) || HCPointIsInvalid(component.c0) || HCPointIsInvalid(component.p)) {
        return false;
    }
    if (HCPointIsInvalid(component.c1)) {
        return true;
    }
    return false;
}

HCContourComponent HCContourComponentAsQuadratic(HCPoint p0, HCContourComponent component) {
    if (HCContourComponentIsQuadratic(p0, component)) {
        return HCContourComponentMakeQuadratic(component.c0, component.p);
    }
    if (HCContourComponentIsCubic(p0, component)) {
        return HCContourComponentMakeQuadratic(HCPointInterpolate(component.c0, component.c1, 0.5), component.p);
    }
    return HCContourComponentMakeQuadratic(component.p, component.p);
}

HCBoolean HCContourComponentIsCubic(HCPoint p0, HCContourComponent component) {
    if (HCPointIsInvalid(p0) || HCPointIsInvalid(component.c0) || HCPointIsInvalid(component.c1) || HCPointIsInvalid(component.p)) {
        return false;
    }
    return true;
}

HCContourComponent HCContourComponentAsCubic(HCPoint p0, HCContourComponent component) {
    if (HCContourComponentIsCubic(p0, component)) {
        return component;
    }
    if (HCContourComponentIsQuadratic(p0, component)) {
        HCPoint c = component.c0;
        HCPoint p1 = component.p;
        HCPoint c0 = HCPointMake(p0.x + (2.0/3.0) * (c.x - p0.x), p0.y + (2.0/3.0) * (c.y - p0.y));
        HCPoint c1 = HCPointMake(p1.x + (2.0/3.0) * (c.x - p1.x), p1.y + (2.0/3.0) * (c.y - p1.y));
        return HCContourComponentMakeCubic(c0, c1, p1);
    }
    return HCContourComponentMakeCubic(p0, component.p, component.p);
}
