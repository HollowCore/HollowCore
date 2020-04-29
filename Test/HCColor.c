///
/// @file HCColor.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 3/6/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <math.h>

CTEST(HCColor, Creation) {
    HCColor c0 = HCColorMake(0.1, -0.2, 0.3, -0.4);
    ASSERT_DBL_NEAR(c0.a, 0.1);
    ASSERT_DBL_NEAR(c0.r, -0.2);
    ASSERT_DBL_NEAR(c0.g, 0.3);
    ASSERT_DBL_NEAR(c0.b, -0.4);
    
    HCColor c1 = HCColorMake(-0.5, 0.6, -0.7, 0.8);
    ASSERT_DBL_NEAR(c1.a, -0.5);
    ASSERT_DBL_NEAR(c1.r, 0.6);
    ASSERT_DBL_NEAR(c1.g, -0.7);
    ASSERT_DBL_NEAR(c1.b, 0.8);
    
    HCColor redAHSB = HCColorMakeWithAHSB(1.0, 0.0 / 360.0, 1.0, 1.0);
    ASSERT_DBL_NEAR(redAHSB.a, 1.0);
    ASSERT_DBL_NEAR(redAHSB.r, 1.0);
    ASSERT_DBL_NEAR(redAHSB.g, 0.0);
    ASSERT_DBL_NEAR(redAHSB.b, 0.0);
    
    HCColor greenAHSB = HCColorMakeWithAHSB(1.0, 120.0 / 360.0, 1.0, 1.0);
    ASSERT_DBL_NEAR(greenAHSB.a, 1.0);
    ASSERT_DBL_NEAR(greenAHSB.r, 0.0);
    ASSERT_DBL_NEAR(greenAHSB.g, 1.0);
    ASSERT_DBL_NEAR(greenAHSB.b, 0.0);
    
    HCColor blueAHSB = HCColorMakeWithAHSB(1.0, 240.0 / 360.0, 1.0, 1.0);
    ASSERT_DBL_NEAR(blueAHSB.a, 1.0);
    ASSERT_DBL_NEAR(blueAHSB.r, 0.0);
    ASSERT_DBL_NEAR(blueAHSB.g, 0.0);
    ASSERT_DBL_NEAR(blueAHSB.b, 1.0);
    
    HCColor redARGB = HCColorMakeWithARGB(0xFFFF0000);
    ASSERT_DBL_NEAR(redARGB.a, 1.0);
    ASSERT_DBL_NEAR(redARGB.r, 1.0);
    ASSERT_DBL_NEAR(redARGB.g, 0.0);
    ASSERT_DBL_NEAR(redARGB.b, 0.0);
    ASSERT_EQUAL(HCColorAsARGB8888(redARGB), 0xFFFF0000);
    
    HCColor greenARGB = HCColorMakeWithARGB(0xFF00FF00);
    ASSERT_DBL_NEAR(greenARGB.a, 1.0);
    ASSERT_DBL_NEAR(greenARGB.r, 0.0);
    ASSERT_DBL_NEAR(greenARGB.g, 1.0);
    ASSERT_DBL_NEAR(greenARGB.b, 0.0);
    ASSERT_EQUAL(HCColorAsARGB8888(greenARGB), 0xFF00FF00);
    
    HCColor blueARGB = HCColorMakeWithARGB(0xFF0000FF);
    ASSERT_DBL_NEAR(blueARGB.a, 1.0);
    ASSERT_DBL_NEAR(blueARGB.r, 0.0);
    ASSERT_DBL_NEAR(blueARGB.g, 0.0);
    ASSERT_DBL_NEAR(blueARGB.b, 1.0);
    ASSERT_EQUAL(HCColorAsARGB8888(blueARGB), 0xFF0000FF);
}

CTEST(HCColor, Equality) {
    ASSERT_FALSE(HCColorIsInvalid(HCColorMake(0.1, 0.2, 0.3, 0.4)));
    ASSERT_TRUE(HCColorIsInvalid(HCColorMake(0.1, 0.2, NAN, 0.4)));
    ASSERT_TRUE(HCColorIsClamped(HCColorMake(0.1, 0.2, 0.3, 0.4)));
    ASSERT_FALSE(HCColorIsClamped(HCColorMake(0.1, 0.2, 3.0, 0.4)));
    ASSERT_TRUE(HCColorIsSimilar(HCColorMake(0.1, 0.2, 0.3, 0.4), HCColorMake(0.15, 0.25, 0.35, 0.45), 0.06));
    ASSERT_FALSE(HCColorIsSimilar(HCColorMake(0.1, 0.2, 0.3, 0.4), HCColorMake(0.15, 0.25, 0.35, 0.45), 0.04));
    ASSERT_FALSE(HCColorIsEqual(HCColorMake(0.1, 0.2, 0.3, 0.4), HCColorMake(0.1, 0.2, 0.4, 0.3)));
    ASSERT_TRUE(HCColorIsEqual(HCColorMake(0.1, 0.2, 0.3, 0.4), HCColorMake(0.1, 0.2, 0.3, 0.4)));
    ASSERT_EQUAL(HCColorHashValue(HCColorMake(0.1, 0.2, 0.3, 0.4)), HCColorHashValue(HCColorMake(0.1, 0.2, 0.3, 0.4)));
}

CTEST(HCColor, Print) {
    HCColor c = HCColorMake(0.1, 0.2, 0.3, 0.4);
    HCColorPrint(c, stdout); // TODO: Not to stdout
}

CTEST(HCColor, Operations) {
    HCColor c0 = HCColorMake(0.1, 0.2, 0.3, 0.4);
    HCColor c1 = HCColorMake(0.4, 0.3, 0.2, 0.1);

    // Color add checks
    HCColor cAdd = HCColorAdd(c0, c1);
    ASSERT_DBL_NEAR(cAdd.a, c0.a + c1.a);
    ASSERT_DBL_NEAR(cAdd.r, c0.r + c1.r);
    ASSERT_DBL_NEAR(cAdd.g, c0.g + c1.g);
    ASSERT_DBL_NEAR(cAdd.b, c0.b + c1.b);

    // Color subtract checks
    HCColor cSubtract = HCColorSubtract(c0, c1);
    ASSERT_DBL_NEAR(cSubtract.a, c0.a - c1.a);
    ASSERT_DBL_NEAR(cSubtract.r, c0.r - c1.r);
    ASSERT_DBL_NEAR(cSubtract.g, c0.g - c1.g);
    ASSERT_DBL_NEAR(cSubtract.b, c0.b - c1.b);

    // Color multiply checks
    HCColor cMultiply = HCColorMultiply(c0, c1);
    ASSERT_DBL_NEAR(cMultiply.a, c0.a * c1.a);
    ASSERT_DBL_NEAR(cMultiply.r, c0.r * c1.r);
    ASSERT_DBL_NEAR(cMultiply.g, c0.g * c1.g);
    ASSERT_DBL_NEAR(cMultiply.b, c0.b * c1.b);

    // Color scale checks
    HCReal scale = 40.0;
    HCColor cScale = HCColorScale(c0, scale);
    ASSERT_DBL_NEAR(cScale.a, c0.a * scale);
    ASSERT_DBL_NEAR(cScale.r, c0.r * scale);
    ASSERT_DBL_NEAR(cScale.g, c0.g * scale);
    ASSERT_DBL_NEAR(cScale.b, c0.b * scale);

    // Color clamp checks
    ASSERT_DBL_NEAR(HCColorClamp(c0).a, c0.a);
    ASSERT_DBL_NEAR(HCColorClamp(c0).r, c0.r);
    ASSERT_DBL_NEAR(HCColorClamp(c0).g, c0.g);
    ASSERT_DBL_NEAR(HCColorClamp(c0).b, c0.b);
    ASSERT_DBL_NEAR(HCColorClamp(c1).a, c1.a);
    ASSERT_DBL_NEAR(HCColorClamp(c1).r, c1.r);
    ASSERT_DBL_NEAR(HCColorClamp(c1).g, c1.g);
    ASSERT_DBL_NEAR(HCColorClamp(c1).b, c1.b);
    ASSERT_DBL_NEAR(HCColorClamp(HCColorMake(10.0, -10.0, 10.0, -10.0)).a, 1.0);
    ASSERT_DBL_NEAR(HCColorClamp(HCColorMake(10.0, -10.0, 10.0, -10.0)).r, 0.0);
    ASSERT_DBL_NEAR(HCColorClamp(HCColorMake(10.0, -10.0, 10.0, -10.0)).g, 1.0);
    ASSERT_DBL_NEAR(HCColorClamp(HCColorMake(10.0, -10.0, 10.0, -10.0)).b, 0.0);
    
    // Color interpolate checks
    HCReal t = 0.333;
    HCColor cInterpolate = HCColorInterpolate(c0, c1, t);
    ASSERT_DBL_NEAR(cInterpolate.a, (1.0 - t) * c0.a + t * c1.a);
    ASSERT_DBL_NEAR(cInterpolate.r, (1.0 - t) * c0.r + t * c1.r);
    ASSERT_DBL_NEAR(cInterpolate.g, (1.0 - t) * c0.g + t * c1.g);
    ASSERT_DBL_NEAR(cInterpolate.b, (1.0 - t) * c0.b + t * c1.b);
}
