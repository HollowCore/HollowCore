//
//  HCPoint.c
//  Test
//
//  Created by Matt Stoker on 10/20/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <math.h>

CTEST(HCPoint, Creation) {
    HCPoint p0 = HCPointMake(10.0, -20.0);
    ASSERT_DBL_NEAR( p0.x, 10.0);
    ASSERT_DBL_NEAR( p0.y, -20.0);
    
    HCPoint p1 = HCPointMake(30.0, 20.0);
    ASSERT_DBL_NEAR( p1.x, 30.0);
    ASSERT_DBL_NEAR( p1.y, 20.0);
}

CTEST(HCPoint, Equality) {
    ASSERT_FALSE(HCPointIsInvalid(HCPointMake(1.0, 2.0)));
    ASSERT_TRUE(HCPointIsInvalid(HCPointMake(1.0, NAN)));
    ASSERT_TRUE(HCPointIsInvalid(HCPointInvalid));
    ASSERT_TRUE(HCPointIsSimilar(HCPointMake(1.0, 2.0), HCPointMake(1.5, 2.5), 0.6));
    ASSERT_FALSE(HCPointIsSimilar(HCPointMake(1.0, 2.0), HCPointMake(1.5, 2.5), 0.4));
    ASSERT_FALSE(HCPointIsEqual(HCPointMake(1.0, -2.5), HCPointMake(1.0, 2.5)));
    ASSERT_TRUE(HCPointIsEqual(HCPointMake(1.0, -2.5), HCPointMake(1.0, -2.5)));
    ASSERT_EQUAL(HCPointHashValue(HCPointMake(1.0, -2.5)), HCPointHashValue(HCPointMake(1.0, -2.5)));
}

CTEST(HCPoint, Print) {
    HCPoint p = HCPointMake(10.0, -20.0);
    HCPointPrint(p, stdout); // TODO: Not to stdout
}

CTEST(HCPoint, Queries) {
    ASSERT_TRUE(HCPointIsZero(HCPointZero));
    ASSERT_FALSE(HCPointIsZero(HCPointMake(1.0, 0.0)));
    ASSERT_FALSE(HCPointIsZero(HCPointInvalid));
    ASSERT_FALSE(HCPointIsInfinite(HCPointZero));
    ASSERT_FALSE(HCPointIsInfinite(HCPointMake(1.0, 0.0)));
    ASSERT_TRUE(HCPointIsInfinite(HCPointMake(INFINITY, 0.0)));
}

CTEST(HCPoint, Operations) {
    HCPoint p = HCPointMake(10.0, -20.0);
    
    // Point offset checks
    HCPoint pOffset = HCPointOffset(p, -2.0, 3.0);
    ASSERT_TRUE(HCPointIsSimilar(pOffset, HCPointMake(p.x + -2.0, p.y + 3.0), 0.000001));
    
    // Point scale checks
    HCReal scale = 40.0;
    HCPoint pScale = HCPointScale(p, scale);
    ASSERT_TRUE(HCPointIsSimilar(pScale, HCPointMake(p.x * scale, p.y * scale), 0.000001));
    
    // Point interpolate
    HCPoint p0 = HCPointMake(1.0, -1.0);
    HCPoint p1 = HCPointMake(1.0, -1.0);
    HCReal t = 0.25;
    HCPoint interpolated = HCPointInterpolate(p0, p1, t);
    ASSERT_TRUE(HCPointIsSimilar(interpolated, HCPointMake((1.0 - t) * p0.x + t * p1.x, (1.0 - t) * p0.y + t * p1.y), 0.000001));
}
