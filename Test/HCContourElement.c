//
//  HCContourElement.c
//  Test
//
//  Created by Matt Stoker on 3/3/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"

CTEST(HCContourElement, Creation) {
    HCContourElement linear = HCContourElementMakeLinear(HCPointMake(-1.0, 2.0));
    ASSERT_DBL_NEAR(linear.p.x, -1.0);
    ASSERT_DBL_NEAR(linear.p.y, 2.0);
    
    HCContourElement quadratic = HCContourElementMakeQuadratic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0));
    ASSERT_DBL_NEAR(quadratic.p.x, -1.0);
    ASSERT_DBL_NEAR(quadratic.p.y, 2.0);
    ASSERT_DBL_NEAR(quadratic.c0.x, -3.0);
    ASSERT_DBL_NEAR(quadratic.c0.y, 4.0);
    
    HCContourElement cubic = HCContourElementMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0));
    ASSERT_DBL_NEAR(cubic.p.x, -1.0);
    ASSERT_DBL_NEAR(cubic.p.y, 2.0);
    ASSERT_DBL_NEAR(cubic.c0.x, -3.0);
    ASSERT_DBL_NEAR(cubic.c0.y, 4.0);
    ASSERT_DBL_NEAR(cubic.c1.x, -5.0);
    ASSERT_DBL_NEAR(cubic.c1.y, 6.0);
}

CTEST(HCContourElement, Equality) {
    ASSERT_TRUE(HCContourElementIsSimilar(HCContourElementMakeLinear(HCPointMake(1.0, 2.0)), HCContourElementMakeLinear(HCPointMake(1.5, 2.5)), 0.6));
    ASSERT_FALSE(HCContourElementIsSimilar(HCContourElementMakeLinear(HCPointMake(1.0, 2.0)), HCContourElementMakeLinear(HCPointMake(1.5, 2.5)), 0.4));
    ASSERT_FALSE(HCContourElementIsEqual(HCContourElementMakeLinear(HCPointMake(1.0, -2.5)), HCContourElementMakeLinear(HCPointMake(1.0, 2.5))));
    ASSERT_TRUE(HCContourElementIsEqual(HCContourElementMakeLinear(HCPointMake(1.0, -2.5)), HCContourElementMakeLinear(HCPointMake(1.0, -2.5))));
    ASSERT_EQUAL(HCContourElementHashValue(HCContourElementMakeLinear(HCPointMake(1.0, -2.5))), HCContourElementHashValue(HCContourElementMakeLinear(HCPointMake(1.0, -2.5))));
}

CTEST(HCContourElement, Print) {
    HCContourElement e = HCContourElementMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0));
    HCContourElementPrint(e, stdout); // TODO: Not to stdout
}

CTEST(HCContourElement, Queries) {
    HCContourElement invalid = HCContourElementInvalid;
    ASSERT_TRUE(HCContourElementIsInvalid(invalid));
    ASSERT_FALSE(HCContourElementIsLinear(invalid));
    ASSERT_FALSE(HCContourElementIsQuadratic(invalid));
    ASSERT_FALSE(HCContourElementIsCubic(invalid));
    ASSERT_FALSE(HCContourElementIsZero(invalid));
    ASSERT_FALSE(HCContourElementIsInfinite(invalid));
    
    HCContourElement zero = HCContourElementZero;
    ASSERT_FALSE(HCContourElementIsInvalid(zero));
    ASSERT_TRUE(HCContourElementIsLinear(zero));
    ASSERT_FALSE(HCContourElementIsQuadratic(zero));
    ASSERT_FALSE(HCContourElementIsCubic(zero));
    ASSERT_TRUE(HCContourElementIsZero(zero));
    ASSERT_FALSE(HCContourElementIsInfinite(zero));
    
    HCContourElement linear = HCContourElementMakeLinear(HCPointMake(-1.0, 2.0));
    ASSERT_FALSE(HCContourElementIsInvalid(linear));
    ASSERT_TRUE(HCContourElementIsLinear(linear));
    ASSERT_FALSE(HCContourElementIsQuadratic(linear));
    ASSERT_FALSE(HCContourElementIsCubic(linear));
    ASSERT_FALSE(HCContourElementIsZero(linear));
    ASSERT_FALSE(HCContourElementIsInfinite(linear));
    
    HCContourElement quadratic = HCContourElementMakeQuadratic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0));
    ASSERT_FALSE(HCContourElementIsInvalid(quadratic));
    ASSERT_FALSE(HCContourElementIsLinear(quadratic));
    ASSERT_TRUE(HCContourElementIsQuadratic(quadratic));
    ASSERT_FALSE(HCContourElementIsCubic(quadratic));
    ASSERT_FALSE(HCContourElementIsZero(quadratic));
    ASSERT_FALSE(HCContourElementIsInfinite(quadratic));
    
    HCContourElement cubic = HCContourElementMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0));
    ASSERT_FALSE(HCContourElementIsInvalid(cubic));
    ASSERT_FALSE(HCContourElementIsLinear(cubic));
    ASSERT_FALSE(HCContourElementIsQuadratic(cubic));
    ASSERT_TRUE(HCContourElementIsCubic(cubic));
    ASSERT_FALSE(HCContourElementIsZero(cubic));
    ASSERT_FALSE(HCContourElementIsInfinite(cubic));
}

CTEST(HCContourElement, Conversion) {
    HCContourElement linear = HCContourElementMakeLinear(HCPointMake(-1.0, 2.0));
    ASSERT_TRUE(HCContourElementIsSimilar(HCContourElementAsLinear(linear), linear, 0.000001));
    ASSERT_TRUE(HCContourElementIsSimilar(HCContourElementAsQuadratic(linear), HCContourElementMakeQuadratic(linear.p, linear.p), 0.000001));
    ASSERT_TRUE(HCContourElementIsSimilar(HCContourElementAsCubic(linear), HCContourElementMakeCubic(linear.p, linear.p, linear.p), 0.000001));
    HCContourElement quadratic = HCContourElementMakeQuadratic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0));
    ASSERT_TRUE(HCContourElementIsSimilar(HCContourElementAsLinear(quadratic), HCContourElementMakeLinear(quadratic.p), 0.000001));
    ASSERT_TRUE(HCContourElementIsSimilar(HCContourElementAsQuadratic(quadratic), quadratic, 0.000001));
    ASSERT_TRUE(HCContourElementIsSimilar(HCContourElementAsCubic(quadratic), HCContourElementMakeCubic(quadratic.p, quadratic.c0, quadratic.c0), 0.000001));
    HCContourElement cubic = HCContourElementMakeCubic(HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0), HCPointMake(7.0, 8.0));
    ASSERT_TRUE(HCContourElementIsSimilar(HCContourElementAsLinear(cubic), HCContourElementMakeLinear(cubic.p), 0.000001));
    ASSERT_TRUE(HCContourElementIsSimilar(HCContourElementAsQuadratic(cubic), HCContourElementMakeQuadratic(cubic.p, HCPointInterpolate(cubic.c0, cubic.c1, 0.5)), 0.000001));
    ASSERT_TRUE(HCContourElementIsSimilar(HCContourElementAsCubic(cubic), cubic, 0.000001));
}

CTEST(HCContourElement, LineEvaluation) {
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCPoint s = HCContourEvaluateLinearCurve(0.5, HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), &dx, &dy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(2.0, 3.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
}

CTEST(HCContourElement, QuadraticEvaluation) {
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCPoint s = HCContourEvaluateQuadraticCurve(0.5, HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0), &dx, &dy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 4.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
}

CTEST(HCContourElement, CubicEvaluation) {
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCPoint s = HCContourEvaluateCubicCurve(0.5, HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0), HCPointMake(7.0, 8.0), &dx, &dy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(4.0, 5.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
}

CTEST(HCContourElement, ElementEvaluation) {
    HCContourElement cubic = HCContourElementMakeCubic(HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0), HCPointMake(7.0, 8.0));
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCPoint s = HCContourEvaluateElement(0.5, HCPointMake(1.0, 2.0), cubic, &dx, &dy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(4.0, 5.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
}
