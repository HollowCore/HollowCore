//
//  HCRay.c
//  Test
//
//  Created by Matt Stoker on 3/10/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <math.h>

CTEST(HCRay, Creation) {
    HCRay r = HCRayMake(HCVectorMake(10.0, -20.0, 30.0), HCVectorMake(30.0, 20.0, -10.0));
    ASSERT_DBL_NEAR(r.origin.x, 10.0);
    ASSERT_DBL_NEAR(r.origin.y, -20.0);
    ASSERT_DBL_NEAR(r.origin.z, 30.0);
    ASSERT_DBL_NEAR(r.direction.x, 30.0);
    ASSERT_DBL_NEAR(r.direction.y, 20.0);
    ASSERT_DBL_NEAR(r.direction.z, -10.0);
}

CTEST(HCRay, Equality) {
    ASSERT_FALSE(HCRayIsInvalid(HCRayMake(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(4.0, 5.0, 6.0))));
    ASSERT_TRUE(HCRayIsInvalid(HCRayMake(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(4.0, NAN, 6.0))));
    ASSERT_TRUE(HCRayIsSimilar(HCRayMake(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(4.0, 5.0, 6.0)), HCRayMake(HCVectorMake(1.5, 2.5, 3.5), HCVectorMake(4.5, 5.5, 6.5)), 0.6));
    ASSERT_FALSE(HCRayIsSimilar(HCRayMake(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(4.0, 5.0, 6.0)), HCRayMake(HCVectorMake(1.5, 2.5, 3.5), HCVectorMake(4.5, 5.5, 6.5)), 0.4));
    ASSERT_FALSE(HCRayIsEqual(HCRayMake(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(4.0, 5.0, 6.0)), HCRayMake(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(4.0, -5.0, 6.0))));
    ASSERT_TRUE(HCRayIsEqual(HCRayMake(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(4.0, 5.0, 6.0)), HCRayMake(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(4.0, 5.0, 6.0))));
    ASSERT_EQUAL(HCRayHashValue(HCRayMake(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(4.0, 5.0, 6.0))), HCRayHashValue(HCRayMake(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(4.0, 5.0, 6.0))));
}

CTEST(HCRay, Print) {
    HCRay r = HCRayMake(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(4.0, 5.0, 6.0));
    HCRayPrint(r, stdout); // TODO: Not to stdout
}

CTEST(HCRay, Queries) {
    ASSERT_TRUE(HCRayIsZero(HCRayZero));
    ASSERT_FALSE(HCRayIsZero(HCRayMake(HCVectorMake(1.0, 0.0, 0.0), HCVectorZero)));
    ASSERT_FALSE(HCRayIsZero(HCRayInvalid));
    ASSERT_FALSE(HCRayIsInfinite(HCRayZero));
    ASSERT_FALSE(HCRayIsInfinite(HCRayMake(HCVectorMake(1.0, 0.0, 0.0), HCVectorZero)));
    ASSERT_TRUE(HCRayIsInfinite(HCRayMake(HCVectorZero, HCVectorMake(INFINITY, 0.0, 0.0))));
}

CTEST(HCRay, Operations) {
    HCRay r = HCRayMake(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(4.0, 5.0, 6.0));
    ASSERT_TRUE(HCVectorIsSimilar(HCRayPointAt(r, 0.0), r.origin, 0.00001));
    ASSERT_TRUE(HCVectorIsSimilar(HCRayPointAt(r, 1.0), HCVectorAdd(r.origin, r.direction), 0.00001));
    ASSERT_TRUE(HCVectorIsSimilar(HCRayPointAt(r, 2.0), HCVectorAdd(r.origin, HCVectorScale(r.direction, 2.0)), 0.00001));
}
