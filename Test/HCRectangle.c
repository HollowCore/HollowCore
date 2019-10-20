//
//  HCRectangle.c
//  Test
//
//  Created by Matt Stoker on 10/20/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <math.h>

CTEST(HCRectangle, Creation) {
    HCRectangle r = HCRectangleMake(HCPointMake(10.0, -20.0), HCSizeMake(30.0, 20.0));
    ASSERT_DBL_NEAR(r.origin.x, 10.0);
    ASSERT_DBL_NEAR(r.origin.y, -20.0);
    ASSERT_DBL_NEAR(r.size.width, 30.0);
    ASSERT_DBL_NEAR(r.size.height, 20.0);
}

CTEST(HCRectangle, Equality) {
    ASSERT_FALSE(HCRectangleIsInvalid(HCRectangleMake(HCPointMake(1.0, 2.0), HCSizeMake(4.0, 5.0))));
    ASSERT_TRUE(HCRectangleIsInvalid(HCRectangleMake(HCPointMake(1.0, 2.0), HCSizeMake(4.0, NAN))));
    ASSERT_TRUE(HCRectangleIsSimilar(HCRectangleMake(HCPointMake(1.0, 2.0), HCSizeMake(4.0, 5.0)), HCRectangleMake(HCPointMake(1.5, 2.5), HCSizeMake(4.5, 5.5)), 0.6));
    ASSERT_FALSE(HCRectangleIsSimilar(HCRectangleMake(HCPointMake(1.0, 2.0), HCSizeMake(4.0, 5.0)), HCRectangleMake(HCPointMake(1.5, 2.5), HCSizeMake(4.5, 5.5)), 0.4));
    ASSERT_FALSE(HCRectangleIsEqual(HCRectangleMake(HCPointMake(1.0, 2.0), HCSizeMake(4.0, 5.0)), HCRectangleMake(HCPointMake(1.0, 2.0), HCSizeMake(4.0, -5.0))));
    ASSERT_TRUE(HCRectangleIsEqual(HCRectangleMake(HCPointMake(1.0, 2.0), HCSizeMake(4.0, 5.0)), HCRectangleMake(HCPointMake(1.0, 2.0), HCSizeMake(4.0, 5.0))));
    ASSERT_EQUAL(HCRectangleHashValue(HCRectangleMake(HCPointMake(1.0, 2.0), HCSizeMake(4.0, 5.0))), HCRectangleHashValue(HCRectangleMake(HCPointMake(1.0, 2.0), HCSizeMake(4.0, 5.0))));
}

CTEST(HCRectangle, Print) {
    HCRectangle r = HCRectangleMake(HCPointMake(1.0, 2.0), HCSizeMake(4.0, 5.0));
    HCRectanglePrint(r, stdout); // TODO: Not to stdout
}

CTEST(HCRectangle, Queries) {
    ASSERT_TRUE(HCRectangleIsZero(HCRectangleZero));
    ASSERT_FALSE(HCRectangleIsZero(HCRectangleMake(HCPointMake(1.0, 0.0), HCSizeZero)));
    ASSERT_FALSE(HCRectangleIsZero(HCRectangleInvalid));
    
    ASSERT_FALSE(HCRectangleIsInfinite(HCRectangleZero));
    ASSERT_FALSE(HCRectangleIsInfinite(HCRectangleMake(HCPointMake(1.0, 0.0), HCSizeZero)));
    ASSERT_TRUE(HCRectangleIsInfinite(HCRectangleMake(HCPointZero, HCSizeMake(INFINITY, 0.0))));
    
    ASSERT_TRUE(HCRectangleIsEmpty(HCRectangleZero));
    ASSERT_TRUE(HCRectangleIsEmpty(HCRectangleMake(HCPointMake(1.0, 0.0), HCSizeMake(1.0, 0.0))));
    ASSERT_FALSE(HCRectangleIsEmpty(HCRectangleMake(HCPointZero, HCSizeMake(1.0, 1.0))));
    ASSERT_FALSE(HCRectangleIsEmpty(HCRectangleMake(HCPointZero, HCSizeMake(1.0, -1.0))));
    ASSERT_FALSE(HCRectangleIsEmpty(HCRectangleMake(HCPointZero, HCSizeMake(INFINITY, 0.0))));
    
    HCRectangle r = HCRectangleMake(HCPointMake(1.0, -2.0), HCSizeMake(5.0, 10.0));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(3.0, 4.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(2.0, 0.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.1, -1.5)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(5.5, 7.5)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointZero));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(-1.0, 0.0)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(2.0, 10.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.0, -2.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.0, -1.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.0, 0.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.0, 7.0)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(1.0, 8.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.0, -2.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(2.0, -2.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(3.0, -2.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(4.0, -2.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(5.0, -2.0)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(6.0, -2.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(5.99, 7.99)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(6.0, 8.0)));
    
    // TODO: Test negative rectangles for point containment
    
    ASSERT_TRUE(HCRectangleContainsRectangle(r, HCRectangleMake(HCPointMake(2.0, 2.0), HCSizeMake(1.0, 1.0))));
    ASSERT_TRUE(HCRectangleContainsRectangle(r, HCRectangleMake(HCPointMake(2.0, 2.0), HCSizeMake(3.0, 5.0))));
    ASSERT_TRUE(HCRectangleContainsRectangle(r, HCRectangleMake(HCPointMake(2.0, -1.0), HCSizeMake(3.0, 5.0))));
    ASSERT_FALSE(HCRectangleContainsRectangle(r, r));
    ASSERT_TRUE(HCRectangleContainsRectangle(r, HCRectangleInset(r, 0.0001, 0.0001)));
    ASSERT_FALSE(HCRectangleContainsRectangle(r, HCRectangleOutset(r, 0.0001, 0.0001)));
    ASSERT_FALSE(HCRectangleContainsRectangle(r, HCRectangleOffset(r, -0.0001, -0.0001)));

    // TODO: Test negative rectangles for rectangle containment
}

CTEST(HCRectangle, Operations) {
    HCRectangle inverted = HCRectangleMake(HCPointMake(15.0, 4.0), HCSizeMake(-3.0, -2.0));
    HCRectangle standardized = HCRectangleStandardize(inverted);
    ASSERT_TRUE(HCRectangleWidth(inverted) == HCRectangleWidth(standardized));
    ASSERT_TRUE(HCRectangleHeight(inverted) == HCRectangleHeight(standardized));
    ASSERT_TRUE(HCRectangleMinX(inverted) == HCRectangleMinX(standardized));
    ASSERT_TRUE(HCRectangleMinY(inverted) == HCRectangleMinY(standardized));
    ASSERT_TRUE(HCRectangleMaxX(inverted) == HCRectangleMaxX(standardized));
    ASSERT_TRUE(HCRectangleMaxY(inverted) == HCRectangleMaxY(standardized));
    ASSERT_TRUE(standardized.size.width >= 0.0);
    ASSERT_TRUE(standardized.size.height >= 0.0);
    
//    HCRectangle HCRectangleIntegral(HCRectangle rectangle);
//    HCRectangle HCRectangleOutset(HCRectangle rectangle, HCReal dx, HCReal dy);
//    HCRectangle HCRectangleInset(HCRectangle rectangle, HCReal dx, HCReal dy);
//    HCRectangle HCRectangleOffset(HCRectangle rectangle, HCReal dx, HCReal dy);
//    HCRectangle HCRectangleUnion(HCRectangle rectangle, HCRectangle other);
//    HCRectangle HCRectangleIntersection(HCRectangle rectangle, HCRectangle other);
//    void HCRectangleDivide(HCRectangle rectangle, HCRectangle* slice, HCRectangle* remainder, HCReal amount, HCRectangleEdge edge);
}
