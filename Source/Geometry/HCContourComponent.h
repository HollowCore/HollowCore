///
/// @file HCContourComponent.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 2/11/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCContourComponent_h
#define HCContourComponent_h

#include "HCCurve.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCContourComponent {
    HCPoint c0;
    HCPoint c1;
    HCPoint p;
} HCContourComponent;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contour Curve Special Values
//----------------------------------------------------------------------------------------------------------------------------------

#define HCContourComponentInvalidStatic ((HCContourComponent){.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = HCPointInvalidStatic})
static const HCContourComponent HCContourComponentInvalid = HCContourComponentInvalidStatic;
#define HCContourComponentZeroStatic ((HCContourComponent){.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = HCPointZeroStatic})
static const HCContourComponent HCContourComponentZero = HCContourComponentZeroStatic;
#define HCContourComponentPointAxisDissimilarityStatic (DBL_EPSILON)
static const HCReal HCContourComponentPointAxisDissimilarity = HCContourComponentPointAxisDissimilarityStatic;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCContourComponent HCContourComponentMakeLinear(HCPoint p);
HCContourComponent HCContourComponentMakeQuadratic(HCPoint c, HCPoint p);
HCContourComponent HCContourComponentMakeCubic(HCPoint c0, HCPoint c1, HCPoint p);
HCContourComponent HCContourComponentMakeWithCurve(HCCurve curve);
HCCurve HCCurveMakeWithContourComponent(HCPoint p0, HCContourComponent component);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourComponentIsInvalid(HCContourComponent component);
HCBoolean HCContourComponentIsSimilar(HCContourComponent component, HCContourComponent other, HCReal axisDissimilarity);
HCBoolean HCContourComponentIsZero(HCContourComponent component);
HCBoolean HCContourComponentIsInfinite(HCContourComponent component);
HCBoolean HCContourComponentIsEqual(HCContourComponent component, HCContourComponent other);
HCInteger HCContourComponentHashValue(HCContourComponent component);
void HCContourComponentPrint(HCContourComponent component, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Order
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCContourComponentIsLinear(HCPoint p0, HCContourComponent component);
HCContourComponent HCContourComponentAsLinear(HCPoint p0, HCContourComponent component);
HCBoolean HCContourComponentIsQuadratic(HCPoint p0, HCContourComponent component);
HCContourComponent HCContourComponentAsQuadratic(HCPoint p0, HCContourComponent component);
HCBoolean HCContourComponentIsCubic(HCPoint p0, HCContourComponent component);
HCContourComponent HCContourComponentAsCubic(HCPoint p0, HCContourComponent component);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCCurveType HCContourComponentCanonicalType(HCPoint p0, HCContourComponent component);
void HCContourComponentExtrema(HCPoint p0, HCContourComponent component, HCInteger* count, HCReal* extrema);
void HCContourComponentInflections(HCPoint p0, HCContourComponent component, HCInteger* count, HCReal* inflections);
HCRectangle HCContourComponentApproximateBounds(HCPoint p0, HCContourComponent component);
HCRectangle HCContourComponentBounds(HCPoint p0, HCContourComponent component);
HCReal HCContourComponentLength(HCPoint p0, HCContourComponent component);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCContourComponentValue(HCPoint p0, HCContourComponent component, HCReal t);
void HCContourComponentDerivative(HCPoint p0, HCContourComponent component, HCPoint* dP0, HCContourComponent* dCurve);
HCContourComponent HCContourComponentTangent(HCPoint p0, HCContourComponent component, HCReal t);
HCContourComponent HCContourComponentTangentUnit(HCPoint p0, HCContourComponent component, HCReal t);
HCContourComponent HCContourComponentNormal(HCPoint p0, HCContourComponent component, HCReal t);
HCContourComponent HCContourComponentNormalUnit(HCPoint p0, HCContourComponent component, HCReal t);
HCReal HCContourComponentCurvature(HCPoint p0, HCContourComponent component, HCReal t);
HCReal HCContourComponentParameterAtLength(HCPoint p0, HCContourComponent component, HCReal d);
HCReal HCContourComponentParameterNearestPoint(HCPoint p0, HCContourComponent component, HCPoint p);
HCReal HCContourComponentDistanceFromPoint(HCPoint p0, HCContourComponent component, HCPoint p);
HCPoint HCContourComponentBaselineProjection(HCPoint p0, HCContourComponent component, HCReal t);
HCContourComponent HCContourComponentInterpolatingPoint(HCPoint p0, HCPoint p1, HCPoint p, HCReal t, HCReal dx, HCReal dy);
HCContourComponent HCContourComponentMould(HCPoint p0, HCContourComponent component, HCReal t, HCPoint p);
void HCContourComponentSplit(HCPoint p0, HCContourComponent component, HCReal t, HCPoint* sp0, HCContourComponent* sComponent, HCPoint* ep0, HCContourComponent* eComponent);
HCContourComponent HCContourComponentXAxisAligned(HCPoint p0, HCContourComponent component);
HCContourComponent HCContourComponentYAxisAligned(HCPoint p0, HCContourComponent component);
void HCContourComponentIntersection(HCPoint p0, HCContourComponent pComponent, HCPoint q0, HCContourComponent qComponent, HCInteger* count, HCReal* t, HCReal* u);

#endif /* HCContourComponent_h */
