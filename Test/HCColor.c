//
//  HCColor.c
//  Test
//
//  Created by Matt Stoker on 3/6/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <math.h>

CTEST(HCColor, Operations) {
    // Color make checks
    HCColor c0 = HCColorMake(0.1, -0.2, 0.3, -0.4);
    ASSERT_DBL_NEAR(c0.a, 0.1);
    ASSERT_DBL_NEAR(c0.r, -0.2);
    ASSERT_DBL_NEAR(c0.g, 0.3);
    ASSERT_DBL_NEAR(c0.b, -0.4);
    
    // Color make checks
    HCColor c1 = HCColorMake(-0.5, 0.6, -0.7, 0.8);
    ASSERT_DBL_NEAR(c1.a, -0.5);
    ASSERT_DBL_NEAR(c1.r, 0.6);
    ASSERT_DBL_NEAR(c1.g, -0.7);
    ASSERT_DBL_NEAR(c1.b, 0.8);
}
