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
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.0, 8.0)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(1.0, 8.00001)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.0, -2.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(2.0, -2.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(3.0, -2.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(4.0, -2.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(5.0, -2.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(6.0, -2.0)));
    
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.0, -2.0)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(0.99999, -2.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.00001, -2.0)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(1.0, -2.00001)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.0, -1.99999)));
    
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(6.0, -2.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(5.99999, -2.0)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(6.00001, -2.0)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(6.0, -2.00001)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(6.0, -1.99999)));
    
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.0, 8.0)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(0.99999, 8.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.00001, 8.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(1.0, 7.99999)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(1.0, 8.00001)));
    
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(6.0, 8.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(5.99999, 8.0)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(6.00001, 8.0)));
    ASSERT_TRUE(HCRectangleContainsPoint(r, HCPointMake(6.0, 7.99999)));
    ASSERT_FALSE(HCRectangleContainsPoint(r, HCPointMake(6.0, 8.00001)));
    
    // TODO: Test negative rectangles for point containment
    
    ASSERT_TRUE(HCRectangleContainsRectangle(r, HCRectangleMake(HCPointMake(2.0, 2.0), HCSizeMake(1.0, 1.0))));
    ASSERT_TRUE(HCRectangleContainsRectangle(r, HCRectangleMake(HCPointMake(2.0, 2.0), HCSizeMake(3.0, 5.0))));
    ASSERT_TRUE(HCRectangleContainsRectangle(r, HCRectangleMake(HCPointMake(2.0, -1.0), HCSizeMake(3.0, 5.0))));
    ASSERT_TRUE(HCRectangleContainsRectangle(r, r));
    ASSERT_TRUE(HCRectangleContainsRectangle(r, HCRectangleInset(r, 0.0001, 0.0001)));
    ASSERT_FALSE(HCRectangleContainsRectangle(r, HCRectangleOutset(r, 0.0001, 0.0001)));
    ASSERT_FALSE(HCRectangleContainsRectangle(r, HCRectangleOffset(r, -0.0001, -0.0001)));

    // TODO: Test negative rectangles for rectangle containment
}

CTEST(HCRectangle, Standardize) {
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
}

CTEST(HCRectangle, Integral) {
    HCRectangle nonIntegral = HCRectangleMake(HCPointMake(1.1, -2.2), HCSizeMake(3.3, 4.6));
    HCRectangle integral = HCRectangleIntegral(nonIntegral);
    ASSERT_FALSE(HCRectangleIsEqual(nonIntegral, integral));
    ASSERT_DBL_NEAR(integral.origin.x, floor(integral.origin.x));
    ASSERT_DBL_NEAR(integral.origin.y, floor(integral.origin.y));
    ASSERT_DBL_NEAR(integral.size.width, floor(integral.size.width));
    ASSERT_DBL_NEAR(integral.size.height, floor(integral.size.height));
}

CTEST(HCRectangle, Outset) {
    HCRectangle small = HCRectangleMake(HCPointMake(1.0, 2.0), HCSizeMake(3.0, 4.0));
    HCReal outsetDx = 1.5;
    HCReal outsetDy = 3.25;
    HCRectangle big = HCRectangleOutset(small, outsetDx, outsetDy);
    ASSERT_TRUE(HCRectangleContainsRectangle(big, small));
    ASSERT_DBL_NEAR(HCRectangleWidth(big), HCRectangleWidth(small) + outsetDx * 2.0);
    ASSERT_DBL_NEAR(HCRectangleHeight(big), HCRectangleHeight(small) + outsetDy * 2.0);
    ASSERT_DBL_NEAR(HCRectangleMinX(big), HCRectangleMinX(small) - outsetDx);
    ASSERT_DBL_NEAR(HCRectangleMinY(big), HCRectangleMinY(small) - outsetDy);
    ASSERT_DBL_NEAR(HCRectangleMaxX(big), HCRectangleMaxX(small) + outsetDx);
    ASSERT_DBL_NEAR(HCRectangleMaxY(big), HCRectangleMaxY(small) + outsetDy);
    ASSERT_DBL_NEAR(HCRectangleMidX(big), HCRectangleMidX(small));
    ASSERT_DBL_NEAR(HCRectangleMidY(big), HCRectangleMidY(small));
}

CTEST(HCRectangle, Inset) {
    HCRectangle large = HCRectangleMake(HCPointMake(10.0, 22.0), HCSizeMake(30.0, 40.0));
    HCReal insetDx = 5.5;
    HCReal insetDy = 7.25;
    HCRectangle medium = HCRectangleInset(large, insetDx, insetDy);
    ASSERT_TRUE(HCRectangleContainsRectangle(large, medium));
    ASSERT_DBL_NEAR(HCRectangleWidth(large), HCRectangleWidth(medium) + insetDx * 2.0);
    ASSERT_DBL_NEAR(HCRectangleHeight(large), HCRectangleHeight(medium) + insetDy * 2.0);
    ASSERT_DBL_NEAR(HCRectangleMinX(large), HCRectangleMinX(medium) - insetDx);
    ASSERT_DBL_NEAR(HCRectangleMinY(large), HCRectangleMinY(medium) - insetDy);
    ASSERT_DBL_NEAR(HCRectangleMaxX(large), HCRectangleMaxX(medium) + insetDx);
    ASSERT_DBL_NEAR(HCRectangleMaxY(large), HCRectangleMaxY(medium) + insetDy);
    ASSERT_DBL_NEAR(HCRectangleMidX(large), HCRectangleMidX(medium));
    ASSERT_DBL_NEAR(HCRectangleMidY(large), HCRectangleMidY(medium));
}

CTEST(HCRectangle, Union) {
    HCRectangle left = HCRectangleMake(HCPointMake(10.0, 5.0), HCSizeMake(20.0, 30.0));
    HCRectangle right = HCRectangleMake(HCPointMake(23.0, 29.0), HCSizeMake(10.0, 40.0));
    HCRectangle unionRectangle = HCRectangleUnion(left, right);
    ASSERT_TRUE(HCRectangleIsSimilar(unionRectangle, HCRectangleUnion(right, left), 0.00001));
    ASSERT_TRUE(HCRectangleContainsRectangle(unionRectangle, left));
    ASSERT_TRUE(HCRectangleContainsRectangle(unionRectangle, right));
    ASSERT_FALSE(HCRectangleContainsRectangle(left, unionRectangle));
    ASSERT_FALSE(HCRectangleContainsRectangle(right, unionRectangle));
}

CTEST(HCRectangle, Intersection) {
    HCRectangle left = HCRectangleMake(HCPointMake(10.0, 5.0), HCSizeMake(20.0, 30.0));
    HCRectangle right = HCRectangleMake(HCPointMake(23.0, 29.0), HCSizeMake(10.0, 40.0));
    HCRectangle interactionRectangle = HCRectangleIntersection(left, right);
    ASSERT_TRUE(HCRectangleIsSimilar(interactionRectangle, HCRectangleIntersection(right, left), 0.00001));
    ASSERT_TRUE(HCRectangleContainsRectangle(left, interactionRectangle));
    ASSERT_TRUE(HCRectangleContainsRectangle(right, interactionRectangle));
    ASSERT_FALSE(HCRectangleContainsRectangle(interactionRectangle, left));
    ASSERT_FALSE(HCRectangleContainsRectangle(interactionRectangle, right));

    ASSERT_TRUE(HCRectangleIsEqual(left, HCRectangleUnion(left, left)));
    ASSERT_TRUE(HCRectangleIsEqual(right, HCRectangleUnion(right, right)));
    ASSERT_TRUE(HCRectangleIsEqual(left, HCRectangleIntersection(left, left)));
    ASSERT_TRUE(HCRectangleIsEqual(right, HCRectangleIntersection(right, right)));
}

CTEST(HCRectangle, IncludingPoint) {
    HCRectangle rectangle = HCRectangleMake(HCPointMake(10.0, 5.0), HCSizeMake(20.0, 30.0));
    HCPoint point = HCPointMake(50.0, 60.0);
    HCRectangle inclusionRectangle = HCRectangleIncludingPoint(rectangle, point);
    ASSERT_TRUE(HCRectangleContainsRectangle(inclusionRectangle, rectangle));
    ASSERT_TRUE(HCRectangleContainsPoint(inclusionRectangle, point));
    ASSERT_TRUE(HCRectangleIsSimilar(inclusionRectangle, HCRectangleMakeWithComponents(10.0, 5.0, 40.0, 55.0), 0.00001));
    
    HCPoint negativePoint = HCPointMake(-10.0, -30.0);
    HCRectangle negativeInclusionRectangle = HCRectangleIncludingPoint(rectangle, negativePoint);
    ASSERT_TRUE(HCRectangleContainsRectangle(negativeInclusionRectangle, rectangle));
    ASSERT_TRUE(!HCRectangleContainsPoint(negativeInclusionRectangle, point));
    ASSERT_TRUE(HCRectangleContainsPoint(negativeInclusionRectangle, negativePoint));
    ASSERT_TRUE(HCRectangleIsSimilar(negativeInclusionRectangle, HCRectangleMakeWithComponents(-10.0, -30.0, 40.0, 65.0), 0.00001));
}

CTEST(HCRectangle, Divide) {
    HCRectangle square = HCRectangleMake(HCPointMake(-0.5, -0.5), HCSizeMake(1.0, 1.0));
    HCRectangle a = HCRectangleInvalid;
    HCRectangle b = HCRectangleInvalid;
    HCReal amount = 0.4;
    HCRectangleDivide(square, &a, &b, amount, HCRectangleEdgeMinX);
    ASSERT_TRUE(HCRectangleIsEqual(square, HCRectangleUnion(a, b)));
    ASSERT_TRUE(HCRectangleIsEmpty(HCRectangleIntersection(a, b)));
    ASSERT_DBL_NEAR(HCRectangleWidth(a), HCRectangleWidth(square) * amount);
    ASSERT_DBL_NEAR(HCRectangleWidth(b), HCRectangleWidth(square) * (1.0 - amount));
    ASSERT_TRUE(HCRectangleContainsPoint(a, HCPointMake(HCRectangleMinX(square), HCRectangleMinY(square))));
    ASSERT_TRUE(HCRectangleContainsPoint(b, HCPointMake(HCRectangleMaxX(square), HCRectangleMaxY(square))));
    HCRectangleDivide(square, &a, &b, amount, HCRectangleEdgeMaxX);
    ASSERT_TRUE(HCRectangleIsEqual(square, HCRectangleUnion(a, b)));
    ASSERT_TRUE(HCRectangleIsEmpty(HCRectangleIntersection(a, b)));
    ASSERT_DBL_NEAR(HCRectangleWidth(a), HCRectangleWidth(square) * amount);
    ASSERT_DBL_NEAR(HCRectangleWidth(b), HCRectangleWidth(square) * (1.0 - amount));
    ASSERT_TRUE(HCRectangleContainsPoint(b, HCPointMake(HCRectangleMinX(square), HCRectangleMinY(square))));
    ASSERT_TRUE(HCRectangleContainsPoint(a, HCPointMake(HCRectangleMaxX(square), HCRectangleMaxY(square))));
    HCRectangleDivide(square, &a, &b, amount, HCRectangleEdgeMinY);
    ASSERT_TRUE(HCRectangleIsEqual(square, HCRectangleUnion(a, b)));
    ASSERT_TRUE(HCRectangleIsEmpty(HCRectangleIntersection(a, b)));
    ASSERT_DBL_NEAR(HCRectangleHeight(a), HCRectangleHeight(square) * amount);
    ASSERT_DBL_NEAR(HCRectangleHeight(b), HCRectangleHeight(square) * (1.0 - amount));
    ASSERT_TRUE(HCRectangleContainsPoint(a, HCPointMake(HCRectangleMinX(square), HCRectangleMinY(square))));
    ASSERT_TRUE(HCRectangleContainsPoint(b, HCPointMake(HCRectangleMaxX(square), HCRectangleMaxY(square))));
    HCRectangleDivide(square, &a, &b, amount, HCRectangleEdgeMaxY);
    ASSERT_TRUE(HCRectangleIsEqual(square, HCRectangleUnion(a, b)));
    ASSERT_TRUE(HCRectangleIsEmpty(HCRectangleIntersection(a, b)));
    ASSERT_DBL_NEAR(HCRectangleHeight(a), HCRectangleHeight(square) * amount);
    ASSERT_DBL_NEAR(HCRectangleHeight(b), HCRectangleHeight(square) * (1.0 - amount));
    ASSERT_TRUE(HCRectangleContainsPoint(b, HCPointMake(HCRectangleMinX(square), HCRectangleMinY(square))));
    ASSERT_TRUE(HCRectangleContainsPoint(a, HCPointMake(HCRectangleMaxX(square), HCRectangleMaxY(square))));
    
    HCRectangleDivide(square, &a, NULL, amount, HCRectangleEdgeMinX);
    ASSERT_DBL_NEAR(HCRectangleWidth(a), HCRectangleWidth(square) * amount);
    HCRectangleDivide(square, NULL, &b, amount, HCRectangleEdgeMinX);
    ASSERT_DBL_NEAR(HCRectangleWidth(b), HCRectangleWidth(square) * (1.0 - amount));
    HCRectangleDivide(square, NULL, NULL, amount, HCRectangleEdgeMinX);
    
    HCRectangleDivide(square, &a, NULL, 0.0, HCRectangleEdgeMinX);
    ASSERT_TRUE(HCRectangleIsEmpty(a));
    HCRectangleDivide(square, &a, NULL, HCRectangleWidth(square), HCRectangleEdgeMinX);
    ASSERT_TRUE(HCRectangleIsEqual(a, square));
    HCRectangleDivide(square, &a, NULL, 0.0, HCRectangleEdgeMaxX);
    ASSERT_TRUE(HCRectangleIsEmpty(a));
    HCRectangleDivide(square, &a, NULL, HCRectangleWidth(square), HCRectangleEdgeMaxX);
    ASSERT_TRUE(HCRectangleIsEqual(a, square));
    HCRectangleDivide(square, &a, NULL, 0.0, HCRectangleEdgeMinY);
    ASSERT_TRUE(HCRectangleIsEmpty(a));
    HCRectangleDivide(square, &a, NULL, HCRectangleHeight(square), HCRectangleEdgeMinY);
    ASSERT_TRUE(HCRectangleIsEqual(a, square));
    HCRectangleDivide(square, &a, NULL, 0.0, HCRectangleEdgeMaxY);
    ASSERT_TRUE(HCRectangleIsEmpty(a));
    HCRectangleDivide(square, &a, NULL, HCRectangleHeight(square), HCRectangleEdgeMaxY);
    ASSERT_TRUE(HCRectangleIsEqual(a, square));
    
    HCRectangleDivide(square, &a, &b, 2.0, HCRectangleEdgeMinX);
    ASSERT_TRUE(HCRectangleIsEqual(a, square));
    ASSERT_TRUE(HCRectangleIsEmpty(b));
    HCRectangleDivide(square, &a, &b, -1.0, HCRectangleEdgeMinX);
    ASSERT_TRUE(HCRectangleIsEmpty(a));
    ASSERT_TRUE(HCRectangleIsEqual(b, square));
}
