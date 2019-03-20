//
//  HCVector.c
//  Test
//
//  Created by Matt Stoker on 3/6/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <math.h>

CTEST(HCVector, Creation) {
    HCVector v0 = HCVectorMake(10.0, -20.0, 30.0);
    ASSERT_DBL_NEAR(v0.x, 10.0);
    ASSERT_DBL_NEAR(v0.y, -20.0);
    ASSERT_DBL_NEAR(v0.z, 30.0);
    
    HCVector v1 = HCVectorMake(30.0, 20.0, -10.0);
    ASSERT_DBL_NEAR(v1.x, 30.0);
    ASSERT_DBL_NEAR(v1.y, 20.0);
    ASSERT_DBL_NEAR(v1.z, -10.0);
}

CTEST(HCVector, Equality) {
    ASSERT_FALSE(HCVectorIsInvalid(HCVectorMake(1.0, 2.0, 3.0)));
    ASSERT_TRUE(HCVectorIsInvalid(HCVectorMake(1.0, 2.0, NAN)));
    ASSERT_TRUE(HCVectorIsSimilar(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(1.5, 2.5, 3.5), 0.6));
    ASSERT_FALSE(HCVectorIsSimilar(HCVectorMake(1.0, 2.0, 3.0), HCVectorMake(1.5, 2.5, 3.5), 0.4));
    ASSERT_FALSE(HCVectorIsEqual(HCVectorMake(1.0, -2.5, 3.0), HCVectorMake(1.0, 2.5, 3.0)));
    ASSERT_TRUE(HCVectorIsEqual(HCVectorMake(1.0, -2.5, 3.0), HCVectorMake(1.0, -2.5, 3.0)));
    ASSERT_EQUAL(HCVectorHashValue(HCVectorMake(1.0, -2.5, 3.0)), HCVectorHashValue(HCVectorMake(1.0, -2.5, 3.0)));
}

CTEST(HCVector, Print) {
    HCVector v = HCVectorMake(10.0, -20.0, 30.0);
    HCVectorPrint(v, stdout); // TODO: Not to stdout
}

CTEST(HCVector, Operations) {
    HCVector v0 = HCVectorMake(10.0, -20.0, 30.0);
    HCVector v1 = HCVectorMake(30.0, 20.0, -10.0);
    
    // Vector add checks
    HCVector vAdd = HCVectorAdd(v0, v1);
    ASSERT_DBL_NEAR(vAdd.x, v0.x + v1.x);
    ASSERT_DBL_NEAR(vAdd.y, v0.y + v1.y);
    ASSERT_DBL_NEAR(vAdd.z, v0.z + v1.z);
    
    // Vector subtract checks
    HCVector vSubtract = HCVectorSubtract(v0, v1);
    ASSERT_DBL_NEAR(vSubtract.x, v0.x - v1.x);
    ASSERT_DBL_NEAR(vSubtract.y, v0.y - v1.y);
    ASSERT_DBL_NEAR(vSubtract.z, v0.z - v1.z);
    
    // Vector scale checks
    HCReal scale = 40.0;
    HCVector vScale = HCVectorScale(v0, scale);
    ASSERT_DBL_NEAR(vScale.x, v0.x * scale);
    ASSERT_DBL_NEAR(vScale.y, v0.y * scale);
    ASSERT_DBL_NEAR(vScale.z, v0.z * scale);
    
    // Vector dot checks
    HCReal vDot = HCVectorDot(v0, v1);
    ASSERT_DBL_NEAR(vDot, v0.x * v1.x + v0.y * v1.y + v0.z * v1.z);
    ASSERT_DBL_NEAR(sqrt(fabs(HCVectorDot(v0, v0))), HCVectorMagnitude(v0));
    
    // Vector cross checks
    HCVector vCross = HCVectorCross(v0, v1);
    ASSERT_DBL_NEAR(fabs(HCVectorDot(vCross, v0)), 0.0);
    ASSERT_DBL_NEAR(fabs(HCVectorDot(vCross, v1)), 0.0);
    
    // Vector magnitude and normalize checks
    ASSERT_DBL_NEAR(HCVectorMagnitude(HCVectorNormalize(v0)), 1.0);
    ASSERT_DBL_NEAR(HCVectorMagnitude(HCVectorNormalize(v1)), 1.0);
}
