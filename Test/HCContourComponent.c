///
/// @file HCContourComponent.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 5/16/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "ctest.h"
#include "../Source/HollowCore.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCContourComponent, Creation) {
    HCContourComponent linear = HCContourComponentMakeLinear(HCPointMake(-1.0, 2.0));
    ASSERT_DBL_NEAR(linear.p.x, -1.0);
    ASSERT_DBL_NEAR(linear.p.y, 2.0);
    
    HCContourComponent quadratic = HCContourComponentMakeQuadratic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0));
    ASSERT_DBL_NEAR(quadratic.c0.x, -1.0);
    ASSERT_DBL_NEAR(quadratic.c0.y, 2.0);
    ASSERT_DBL_NEAR(quadratic.p.x, -3.0);
    ASSERT_DBL_NEAR(quadratic.p.y, 4.0);
    
    HCContourComponent cubic = HCContourComponentMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0));
    ASSERT_DBL_NEAR(cubic.c0.x, -1.0);
    ASSERT_DBL_NEAR(cubic.c0.y, 2.0);
    ASSERT_DBL_NEAR(cubic.c1.x, -3.0);
    ASSERT_DBL_NEAR(cubic.c1.y, 4.0);
    ASSERT_DBL_NEAR(cubic.p.x, -5.0);
    ASSERT_DBL_NEAR(cubic.p.y, 6.0);
    
    HCContourComponent component = HCContourComponentMakeWithCurve(HCCurveMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0), HCPointMake(-7.0, 8.0)));
    ASSERT_DBL_NEAR(component.c0.x, -3.0);
    ASSERT_DBL_NEAR(component.c0.y, 4.0);
    ASSERT_DBL_NEAR(component.c1.x, -5.0);
    ASSERT_DBL_NEAR(component.c1.y, 6.0);
    ASSERT_DBL_NEAR(component.p.x, -7.0);
    ASSERT_DBL_NEAR(component.p.y, 8.0);
    
    HCCurve curve = HCCurveMakeWithContourComponent(HCPointMake(-1.0, 2.0), component);
    ASSERT_DBL_NEAR(curve.p0.x, -1.0);
    ASSERT_DBL_NEAR(curve.p0.y, 2.0);
    ASSERT_DBL_NEAR(curve.c0.x, -3.0);
    ASSERT_DBL_NEAR(curve.c0.y, 4.0);
    ASSERT_DBL_NEAR(curve.c1.x, -5.0);
    ASSERT_DBL_NEAR(curve.c1.y, 6.0);
    ASSERT_DBL_NEAR(curve.p1.x, -7.0);
    ASSERT_DBL_NEAR(curve.p1.y, 8.0);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCContourComponent, Equality) {
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentMakeLinear(HCPointMake(1.0, 2.0)), HCContourComponentMakeLinear(HCPointMake(1.5, 2.5)), 0.6));
    ASSERT_FALSE(HCContourComponentIsSimilar(HCContourComponentMakeLinear(HCPointMake(1.0, 2.0)), HCContourComponentMakeLinear(HCPointMake(1.5, 2.5)), 0.4));
    ASSERT_FALSE(HCContourComponentIsEqual(HCContourComponentMakeLinear(HCPointMake(1.0, -2.5)), HCContourComponentMakeLinear(HCPointMake(1.0, 2.5))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentMakeLinear(HCPointMake(1.0, -2.5)), HCContourComponentMakeLinear(HCPointMake(1.0, -2.5))));
    ASSERT_EQUAL(HCContourComponentHashValue(HCContourComponentMakeLinear(HCPointMake(1.0, -2.5))), HCContourComponentHashValue(HCContourComponentMakeLinear(HCPointMake(1.0, -2.5))));
}

CTEST(HCContourComponent, Print) {
    HCContourComponent e = HCContourComponentMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0));
    HCContourComponentPrint(e, stdout); // TODO: Not to stdout
}

CTEST(HCContourComponent, Queries) {
    HCContourComponent invalid = HCContourComponentInvalid;
    ASSERT_TRUE(HCContourComponentIsInvalid(invalid));
    ASSERT_FALSE(HCContourComponentIsZero(invalid));
    ASSERT_FALSE(HCContourComponentIsInfinite(invalid));
    
    HCContourComponent zero = HCContourComponentZero;
    ASSERT_FALSE(HCContourComponentIsInvalid(zero));
    ASSERT_TRUE(HCContourComponentIsZero(zero));
    ASSERT_FALSE(HCContourComponentIsInfinite(zero));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Order
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCContourComponent, Order) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint c0 = HCPointMake(3.0, -4.0);
    HCPoint c1 = HCPointMake(-5.0, 6.0);
    HCPoint p1 = HCPointMake(7.0, -8.0);
    
    HCContourComponent linear = HCContourComponentMakeLinear(p1);
    ASSERT_FALSE(HCContourComponentIsInvalid(linear));
    ASSERT_FALSE(HCContourComponentIsZero(linear));
    ASSERT_FALSE(HCContourComponentIsInfinite(linear));
    ASSERT_TRUE(HCContourComponentIsLinear(p0, linear));
    ASSERT_FALSE(HCContourComponentIsQuadratic(p0, linear));
    ASSERT_FALSE(HCContourComponentIsCubic(p0, linear));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsLinear(p0, linear), linear, 0.000001));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsQuadratic(p0, linear), HCContourComponentMakeQuadratic(p1, p1), 0.000001));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsCubic(p0, linear), HCContourComponentMakeCubic(p0, p1, p1), 0.000001));
    
    HCContourComponent quadratic = HCContourComponentMakeQuadratic(c0, p1);
    ASSERT_FALSE(HCContourComponentIsInvalid(quadratic));
    ASSERT_FALSE(HCContourComponentIsZero(quadratic));
    ASSERT_FALSE(HCContourComponentIsInfinite(quadratic));
    ASSERT_FALSE(HCContourComponentIsLinear(p0, quadratic));
    ASSERT_TRUE(HCContourComponentIsQuadratic(p0, quadratic));
    ASSERT_FALSE(HCContourComponentIsCubic(p0, quadratic));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsLinear(p0, quadratic), HCContourComponentMakeLinear(quadratic.p), 0.000001));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsQuadratic(p0, quadratic), quadratic, 0.000001));
    HCPoint qc0 = HCPointMake(p0.x + (2.0/3.0) * (c0.x - p0.x), p0.y + (2.0/3.0) * (c0.y - p0.y));
    HCPoint qc1 = HCPointMake(p1.x + (2.0/3.0) * (c0.x - p1.x), p1.y + (2.0/3.0) * (c0.y - p1.y));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsCubic(p0, quadratic), HCContourComponentMakeCubic(qc0, qc1, p1), 0.000001));
    
    HCContourComponent cubic = HCContourComponentMakeCubic(c0, c1, p1);
    ASSERT_FALSE(HCContourComponentIsInvalid(cubic));
    ASSERT_FALSE(HCContourComponentIsZero(cubic));
    ASSERT_FALSE(HCContourComponentIsInfinite(cubic));
    ASSERT_FALSE(HCContourComponentIsLinear(p0, cubic));
    ASSERT_FALSE(HCContourComponentIsQuadratic(p0, cubic));
    ASSERT_TRUE(HCContourComponentIsCubic(p0, cubic));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsLinear(p0, cubic), HCContourComponentMakeLinear(cubic.p), 0.000001));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsQuadratic(p0, cubic), HCContourComponentMakeQuadratic(HCPointInterpolate(cubic.c0, cubic.c1, 0.5), cubic.p), 0.000001));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsCubic(p0, cubic), cubic, 0.000001));
}
