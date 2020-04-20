//
//  HCContour.c
//  Test
//
//  Created by Matt Stoker on 2/11/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"

CTEST(HCContourCurve, Creation) {
    HCContourCurve linear = HCContourCurveMakeLinear(HCPointMake(-1.0, 2.0));
    ASSERT_DBL_NEAR(linear.p.x, -1.0);
    ASSERT_DBL_NEAR(linear.p.y, 2.0);
    
    HCContourCurve quadratic = HCContourCurveMakeQuadratic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0));
    ASSERT_DBL_NEAR(quadratic.c0.x, -1.0);
    ASSERT_DBL_NEAR(quadratic.c0.y, 2.0);
    ASSERT_DBL_NEAR(quadratic.p.x, -3.0);
    ASSERT_DBL_NEAR(quadratic.p.y, 4.0);
    
    HCContourCurve cubic = HCContourCurveMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0));
    ASSERT_DBL_NEAR(cubic.c0.x, -1.0);
    ASSERT_DBL_NEAR(cubic.c0.y, 2.0);
    ASSERT_DBL_NEAR(cubic.c1.x, -3.0);
    ASSERT_DBL_NEAR(cubic.c1.y, 4.0);
    ASSERT_DBL_NEAR(cubic.p.x, -5.0);
    ASSERT_DBL_NEAR(cubic.p.y, 6.0);
}

CTEST(HCContourCurve, Equality) {
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveMakeLinear(HCPointMake(1.0, 2.0)), HCContourCurveMakeLinear(HCPointMake(1.5, 2.5)), 0.6));
    ASSERT_FALSE(HCContourCurveIsSimilar(HCContourCurveMakeLinear(HCPointMake(1.0, 2.0)), HCContourCurveMakeLinear(HCPointMake(1.5, 2.5)), 0.4));
    ASSERT_FALSE(HCContourCurveIsEqual(HCContourCurveMakeLinear(HCPointMake(1.0, -2.5)), HCContourCurveMakeLinear(HCPointMake(1.0, 2.5))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveMakeLinear(HCPointMake(1.0, -2.5)), HCContourCurveMakeLinear(HCPointMake(1.0, -2.5))));
    ASSERT_EQUAL(HCContourCurveHashValue(HCContourCurveMakeLinear(HCPointMake(1.0, -2.5))), HCContourCurveHashValue(HCContourCurveMakeLinear(HCPointMake(1.0, -2.5))));
}

CTEST(HCContourCurve, Print) {
    HCContourCurve e = HCContourCurveMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0));
    HCContourCurvePrint(e, stdout); // TODO: Not to stdout
}

CTEST(HCContourCurve, Queries) {
    HCContourCurve invalid = HCContourCurveInvalid;
    ASSERT_TRUE(HCContourCurveIsInvalid(invalid));
    ASSERT_FALSE(HCContourCurveIsLinear(invalid));
    ASSERT_FALSE(HCContourCurveIsQuadratic(invalid));
    ASSERT_FALSE(HCContourCurveIsCubic(invalid));
    ASSERT_FALSE(HCContourCurveIsZero(invalid));
    ASSERT_FALSE(HCContourCurveIsInfinite(invalid));
    
    HCContourCurve zero = HCContourCurveZero;
    ASSERT_FALSE(HCContourCurveIsInvalid(zero));
    ASSERT_TRUE(HCContourCurveIsLinear(zero));
    ASSERT_FALSE(HCContourCurveIsQuadratic(zero));
    ASSERT_FALSE(HCContourCurveIsCubic(zero));
    ASSERT_TRUE(HCContourCurveIsZero(zero));
    ASSERT_FALSE(HCContourCurveIsInfinite(zero));
    
    HCContourCurve linear = HCContourCurveMakeLinear(HCPointMake(-1.0, 2.0));
    ASSERT_FALSE(HCContourCurveIsInvalid(linear));
    ASSERT_TRUE(HCContourCurveIsLinear(linear));
    ASSERT_FALSE(HCContourCurveIsQuadratic(linear));
    ASSERT_FALSE(HCContourCurveIsCubic(linear));
    ASSERT_FALSE(HCContourCurveIsZero(linear));
    ASSERT_FALSE(HCContourCurveIsInfinite(linear));
    
    HCContourCurve quadratic = HCContourCurveMakeQuadratic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0));
    ASSERT_FALSE(HCContourCurveIsInvalid(quadratic));
    ASSERT_FALSE(HCContourCurveIsLinear(quadratic));
    ASSERT_TRUE(HCContourCurveIsQuadratic(quadratic));
    ASSERT_FALSE(HCContourCurveIsCubic(quadratic));
    ASSERT_FALSE(HCContourCurveIsZero(quadratic));
    ASSERT_FALSE(HCContourCurveIsInfinite(quadratic));
    
    HCContourCurve cubic = HCContourCurveMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0));
    ASSERT_FALSE(HCContourCurveIsInvalid(cubic));
    ASSERT_FALSE(HCContourCurveIsLinear(cubic));
    ASSERT_FALSE(HCContourCurveIsQuadratic(cubic));
    ASSERT_TRUE(HCContourCurveIsCubic(cubic));
    ASSERT_FALSE(HCContourCurveIsZero(cubic));
    ASSERT_FALSE(HCContourCurveIsInfinite(cubic));
}

CTEST(HCContourCurve, Conversion) {
    HCContourCurve linear = HCContourCurveMakeLinear(HCPointMake(-1.0, 2.0));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsLinear(linear), linear, 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsQuadratic(linear), HCContourCurveMakeQuadratic(linear.p, linear.p), 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsCubic(linear), HCContourCurveMakeCubic(linear.p, linear.p, linear.p), 0.000001));
    HCContourCurve quadratic = HCContourCurveMakeQuadratic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsLinear(quadratic), HCContourCurveMakeLinear(quadratic.p), 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsQuadratic(quadratic), quadratic, 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsCubic(quadratic), HCContourCurveMakeCubic(quadratic.c0, quadratic.c0, quadratic.p), 0.000001));
    HCContourCurve cubic = HCContourCurveMakeCubic(HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0), HCPointMake(7.0, 8.0));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsLinear(cubic), HCContourCurveMakeLinear(cubic.p), 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsQuadratic(cubic), HCContourCurveMakeQuadratic(HCPointInterpolate(cubic.c0, cubic.c1, 0.5), cubic.p), 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsCubic(cubic), cubic, 0.000001));
}

CTEST(HCContourCurve, LineEvaluation) {
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCPoint s = HCContourEvaluateLinearCurve(0.5, HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), &dx, &dy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(2.0, 3.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
}

CTEST(HCContourCurve, QuadraticEvaluation) {
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCPoint s = HCContourEvaluateQuadraticCurve(0.5, HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0), &dx, &dy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 4.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
}

CTEST(HCContourCurve, CubicEvaluation) {
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCPoint s = HCContourEvaluateCubicCurve(0.5, HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0), HCPointMake(7.0, 8.0), &dx, &dy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(4.0, 5.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
}

CTEST(HCContourCurve, ElementEvaluation) {
    HCContourCurve cubic = HCContourCurveMakeCubic(HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0), HCPointMake(7.0, 8.0));
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCPoint s = HCContourEvaluateCurve(0.5, HCPointMake(1.0, 2.0), cubic, &dx, &dy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(4.0, 5.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
}

CTEST(HCContour, CreateWithCurves) {
    HCContourCurve curves[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    HCContourCurve contour[4];
    HCContourInitWithCurves(contour, curves, sizeof(curves) / sizeof(HCContourCurve), true);
    ASSERT_TRUE(HCContourIsClosed(contour));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCContourCurveCount(contour) == 4);
    ASSERT_TRUE(HCContourCurveIsEqual(contour[0], HCContourCurveMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(contour[1], HCContourCurveMakeQuadratic(HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(contour[2], HCContourCurveMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(contour[3], HCContourCurveMakeCubic(HCPointMake(2.5, 4.0), HCPointMake(1.5, 4.0), HCPointMake(1.0, 3.0))));
}

CTEST(HCContour, CreatePolyline) {
    HCPoint startPoint = {.x = 1.0, .y = 1.0};
    HCPoint polyline[] = {
        {.x = 3.0, .y = 1.0},
        {.x = 3.0, .y = 3.0},
        {.x = 1.0, .y = 3.0},
    };
    HCContourCurve contour[4];
    HCContourInitWithPolyline(contour, startPoint, polyline, sizeof(polyline) / sizeof(HCPoint), true);
    ASSERT_TRUE(HCContourIsClosed(contour));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCContourCurveCount(contour) == 4);
    ASSERT_TRUE(HCContourCurveIsEqual(contour[0], HCContourCurveMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(contour[1], HCContourCurveMakeLinear(HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(contour[2], HCContourCurveMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(contour[3], HCContourCurveMakeLinear(HCPointMake(1.0, 3.0))));
}

CTEST(HCContour, CreatePolyquadratic) {
    HCPoint startPoint = {.x = 1.0, .y = 1.0};
    HCPoint polyquadratic[] = {
        {.x = 2.0, .y = 0.0}, {.x = 3.0, .y = 1.0},
        HCPointInvalidStatic, {.x = 3.0, .y = 3.0},
        {.x = 3.0, .y = 4.0}, {.x = 1.0, .y = 3.0},
    };
    HCContourCurve contour[4];
    HCContourInitWithPolyquadratic(contour, startPoint, polyquadratic, sizeof(polyquadratic) / sizeof(HCPoint), true);
    ASSERT_TRUE(HCContourIsClosed(contour));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCContourCurveCount(contour) == 4);
    ASSERT_TRUE(HCContourCurveIsEqual(contour[0], HCContourCurveMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(contour[1], HCContourCurveMakeQuadratic(HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(contour[2], HCContourCurveMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(contour[3], HCContourCurveMakeQuadratic(HCPointMake(3.0, 4.0), HCPointMake(1.0, 3.0))));
}

CTEST(HCContour, CreatePolycubic) {
    HCPoint startPoint = {.x = 1.0, .y = 1.0};
    HCPoint polycubic[] = {
        {.x = 2.0, .y = 0.0}, HCPointInvalidStatic, {.x = 3.0, .y = 1.0},
        HCPointInvalidStatic, HCPointInvalidStatic, {.x = 3.0, .y = 3.0},
        {.x = 2.5, .y = 4.0}, {.x = 1.5, .y = 4.0}, {.x = 1.0, .y = 3.0},
    };
    HCContourCurve contour[4];
    HCContourInitWithPolycubic(contour, startPoint, polycubic, sizeof(polycubic) / sizeof(HCPoint), true);
    ASSERT_TRUE(HCContourIsClosed(contour));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCContourCurveCount(contour) == 4);
    ASSERT_TRUE(HCContourCurveIsEqual(contour[0], HCContourCurveMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(contour[1], HCContourCurveMakeQuadratic(HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(contour[2], HCContourCurveMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(contour[3], HCContourCurveMakeCubic(HCPointMake(2.5, 4.0), HCPointMake(1.5, 4.0), HCPointMake(1.0, 3.0))));
}

CTEST(HCContour, Equality) {
    HCContourCurve a[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    a[0] = HCContourAtlasMake(a[0].p, sizeof(a) / sizeof(HCContourCurve), true).curve;
    HCContourCurve b[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    b[0] = HCContourAtlasMake(a[0].p, sizeof(b) / sizeof(HCContourCurve), true).curve;
    HCContourCurve c[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    c[0] = HCContourAtlasMake(a[0].p, sizeof(c) / sizeof(HCContourCurve), false).curve;
    
    ASSERT_TRUE(HCContourIsEqual(a, a));
    ASSERT_TRUE(HCContourHashValue(a) == HCContourHashValue(a));
    ASSERT_TRUE(HCContourIsEqual(a, b));
    ASSERT_TRUE(HCContourHashValue(a) == HCContourHashValue(b));
    ASSERT_TRUE(HCContourIsEqual(b, a));
    ASSERT_TRUE(HCContourHashValue(b) == HCContourHashValue(a));
    ASSERT_TRUE(!HCContourIsEqual(a, c));
}

CTEST(HCContour, Print) {
    HCContourCurve contour[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    contour[0] = HCContourAtlasMake(contour[0].p, sizeof(contour) / sizeof(HCContourCurve), true).curve;
    HCContourPrint(contour, stdout); // TODO: Not to stdout
}
