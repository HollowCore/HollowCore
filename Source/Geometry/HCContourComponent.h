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

#endif /* HCContourComponent_h */
