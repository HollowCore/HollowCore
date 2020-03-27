//
//  HCContour.c
//  Test
//
//  Created by Matt Stoker on 2/11/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"

CTEST(HCPath, LineEvaluation) {
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCPoint s = HCContourEvaluateLinearCurve(0.5, HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), &dx, &dy);
    ASSERT_DBL_NEAR(s.x, 2.0);
    ASSERT_DBL_NEAR(s.y, 3.0);
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
}

CTEST(HCPath, QuadraticEvaluation) {
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCPoint s = HCContourEvaluateQuadraticCurve(0.5, HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0), &dx, &dy);
    ASSERT_DBL_NEAR(s.x, 3.0);
    ASSERT_DBL_NEAR(s.y, 4.0);
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
}

CTEST(HCPath, CubicEvaluation) {
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCPoint s = HCContourEvaluateCubicCurve(0.5, HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0), HCPointMake(7.0, 8.0), &dx, &dy);
    ASSERT_DBL_NEAR(s.x, 4.0);
    ASSERT_DBL_NEAR(s.y, 5.0);
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
}
