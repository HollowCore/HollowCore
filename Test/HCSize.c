//
//  HCSize.c
//  Test
//
//  Created by Matt Stoker on 10/20/19.
//  Cosyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <math.h>

CTEST(HCSize, Creation) {
    HCSize s0 = HCSizeMake(10.0, -20.0);
    ASSERT_DBL_NEAR(s0.width, 10.0);
    ASSERT_DBL_NEAR(s0.height, -20.0);
    
    HCSize s1 = HCSizeMake(30.0, 20.0);
    ASSERT_DBL_NEAR(s1.width, 30.0);
    ASSERT_DBL_NEAR(s1.height, 20.0);
}

CTEST(HCSize, Equality) {
    ASSERT_FALSE(HCSizeIsInvalid(HCSizeMake(1.0, 2.0)));
    ASSERT_TRUE(HCSizeIsInvalid(HCSizeMake(1.0, NAN)));
    ASSERT_TRUE(HCSizeIsInvalid(HCSizeInvalid));
    ASSERT_TRUE(HCSizeIsSimilar(HCSizeMake(1.0, 2.0), HCSizeMake(1.5, 2.5), 0.6));
    ASSERT_FALSE(HCSizeIsSimilar(HCSizeMake(1.0, 2.0), HCSizeMake(1.5, 2.5), 0.4));
    ASSERT_FALSE(HCSizeIsEqual(HCSizeMake(1.0, -2.5), HCSizeMake(1.0, 2.5)));
    ASSERT_TRUE(HCSizeIsEqual(HCSizeMake(1.0, -2.5), HCSizeMake(1.0, -2.5)));
    ASSERT_EQUAL(HCSizeHashValue(HCSizeMake(1.0, -2.5)), HCSizeHashValue(HCSizeMake(1.0, -2.5)));
}

CTEST(HCSize, Print) {
    HCSize s = HCSizeMake(10.0, -20.0);
    HCSizePrint(s, stdout); // TODO: Not to stdout
}

CTEST(HCSize, Queries) {
    ASSERT_TRUE(HCSizeIsZero(HCSizeZero));
    ASSERT_FALSE(HCSizeIsZero(HCSizeMake(1.0, 0.0)));
    ASSERT_FALSE(HCSizeIsZero(HCSizeInvalid));
    ASSERT_FALSE(HCSizeIsInfinite(HCSizeZero));
    ASSERT_FALSE(HCSizeIsInfinite(HCSizeMake(1.0, 0.0)));
    ASSERT_TRUE(HCSizeIsInfinite(HCSizeMake(INFINITY, 0.0)));
}

CTEST(HCSize, Oserations) {
    HCSize s = HCSizeMake(10.0, -20.0);
    
    // Size outset checks
    HCSize sIncrease = HCSizeIncrease(s, -2.0, 3.0);
    ASSERT_TRUE(HCSizeIsSimilar(sIncrease, HCSizeMake(s.width + -2.0, s.height + 3.0), 0.000001));
    
    // Size inset checks
    HCSize sDecrease = HCSizeDecrease(s, -2.0, 3.0);
    ASSERT_TRUE(HCSizeIsSimilar(sDecrease, HCSizeMake(s.width - -2.0, s.height - 3.0), 0.000001));
    
    // Size scale checks
    HCReal scale = 40.0;
    HCSize sScale = HCSizeScale(s, scale);
    ASSERT_TRUE(HCSizeIsSimilar(sScale, HCSizeMake(s.width * scale, s.height * scale), 0.000001));
}
