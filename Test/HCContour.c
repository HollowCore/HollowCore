///
/// @file HCContour.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 2/11/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

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
    ASSERT_FALSE(HCContourCurveIsZero(invalid));
    ASSERT_FALSE(HCContourCurveIsInfinite(invalid));
    
    HCContourCurve zero = HCContourCurveZero;
    ASSERT_FALSE(HCContourCurveIsInvalid(zero));
    ASSERT_TRUE(HCContourCurveIsZero(zero));
    ASSERT_FALSE(HCContourCurveIsInfinite(zero));
}

CTEST(HCContourCurve, Order) {
    HCContourCurve linear = HCContourCurveMakeLinear(HCPointMake(-1.0, 2.0));
    ASSERT_FALSE(HCContourCurveIsInvalid(linear));
    ASSERT_FALSE(HCContourCurveIsZero(linear));
    ASSERT_FALSE(HCContourCurveIsInfinite(linear));
    ASSERT_TRUE(HCContourCurveIsLinear(HCPointZero, linear));
    ASSERT_FALSE(HCContourCurveIsQuadratic(HCPointZero, linear));
    ASSERT_FALSE(HCContourCurveIsCubic(HCPointZero, linear));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsLinear(HCPointZero, linear), linear, 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsQuadratic(HCPointZero, linear), HCContourCurveMakeQuadratic(linear.p, linear.p), 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsCubic(HCPointZero, linear), HCContourCurveMakeCubic(linear.p, linear.p, linear.p), 0.000001));
    
    HCContourCurve quadratic = HCContourCurveMakeQuadratic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0));
    ASSERT_FALSE(HCContourCurveIsInvalid(quadratic));
    ASSERT_FALSE(HCContourCurveIsZero(quadratic));
    ASSERT_FALSE(HCContourCurveIsInfinite(quadratic));
    ASSERT_FALSE(HCContourCurveIsLinear(HCPointZero, quadratic));
    ASSERT_TRUE(HCContourCurveIsQuadratic(HCPointZero, quadratic));
    ASSERT_FALSE(HCContourCurveIsCubic(HCPointZero, quadratic));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsLinear(HCPointZero, quadratic), HCContourCurveMakeLinear(quadratic.p), 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsQuadratic(HCPointZero, quadratic), quadratic, 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsCubic(HCPointZero, quadratic), HCContourCurveMakeCubic(quadratic.c0, quadratic.c0, quadratic.p), 0.000001));
    
    HCContourCurve cubic = HCContourCurveMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0));
    ASSERT_FALSE(HCContourCurveIsInvalid(cubic));
    ASSERT_FALSE(HCContourCurveIsZero(cubic));
    ASSERT_FALSE(HCContourCurveIsInfinite(cubic));
    ASSERT_FALSE(HCContourCurveIsLinear(HCPointZero, cubic));
    ASSERT_FALSE(HCContourCurveIsQuadratic(HCPointZero, cubic));
    ASSERT_TRUE(HCContourCurveIsCubic(HCPointZero, cubic));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsLinear(HCPointZero, cubic), HCContourCurveMakeLinear(cubic.p), 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsQuadratic(HCPointZero, cubic), HCContourCurveMakeQuadratic(HCPointInterpolate(cubic.c0, cubic.c1, 0.5), cubic.p), 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsCubic(HCPointZero, cubic), cubic, 0.000001));
}

CTEST(HCContourCurve, AxisAligned) {
    HCContourCurve xAxisAligned = HCContourCurveXAxisAligned(HCPointMake(-1.0, 2.0), HCContourCurveMakeCubic(HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0), HCPointMake(-7.0, 8.0)));
    ASSERT_FALSE(HCContourCurveIsInvalid(xAxisAligned));
    ASSERT_FALSE(HCContourCurveIsZero(xAxisAligned));
    ASSERT_FALSE(HCContourCurveIsInfinite(xAxisAligned));
    ASSERT_FALSE(HCContourCurveIsLinear(HCPointZero, xAxisAligned));
    ASSERT_FALSE(HCContourCurveIsQuadratic(HCPointZero, xAxisAligned));
    ASSERT_TRUE(HCContourCurveIsCubic(HCPointZero, xAxisAligned));
    ASSERT_DBL_NEAR(xAxisAligned.p.y, 0.0);
    
    HCContourCurve yAxisAligned = HCContourCurveYAxisAligned(HCPointMake(-1.0, 2.0), HCContourCurveMakeCubic(HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0), HCPointMake(-7.0, 8.0)));
    ASSERT_FALSE(HCContourCurveIsInvalid(yAxisAligned));
    ASSERT_FALSE(HCContourCurveIsZero(yAxisAligned));
    ASSERT_FALSE(HCContourCurveIsInfinite(yAxisAligned));
    ASSERT_FALSE(HCContourCurveIsLinear(HCPointZero, yAxisAligned));
    ASSERT_FALSE(HCContourCurveIsQuadratic(HCPointZero, yAxisAligned));
    ASSERT_TRUE(HCContourCurveIsCubic(HCPointZero, yAxisAligned));
    ASSERT_DBL_NEAR(yAxisAligned.p.x, 0.0);
}

CTEST(HCContourCurve, Canonical) {
    HCPoint p0Invalid = HCPointInvalid;
    HCContourCurve curveInvalid = HCContourCurveInvalid;
    ASSERT_TRUE(HCContourCurveCanonicalType(p0Invalid, curveInvalid) == HCContourCurveTypeInvalid);
    
    HCPoint p0Point = HCPointMake(1.0, 1.0);
    HCContourCurve curvePoint = HCContourCurveMakeCubic(p0Point, p0Point, p0Point);
    ASSERT_TRUE(HCContourCurveCanonicalType(p0Point, curvePoint) == HCContourCurveTypePoint);
    
    HCPoint p0Linear = HCPointMake(1.0, 1.0);
    HCContourCurve curveLinear = HCContourCurveMakeLinear(HCPointMake(2.0, 2.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(p0Linear, curveLinear) == HCContourCurveTypeLinear);
    
    HCPoint p0Quadratic = HCPointMake(1.0, 1.0);
    HCContourCurve curveQuadratic = HCContourCurveMakeQuadratic(HCPointMake(2.0, 2.0), HCPointMake(3.0, 1.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(p0Quadratic, curveQuadratic) == HCContourCurveTypeQuadratic);
    
    HCPoint p0Simple = HCPointMake(0.0, 0.0);
    HCContourCurve curveSimple = HCContourCurveMakeCubic(HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(p0Simple, curveSimple) == HCContourCurveTypeCubicSimple);
    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(p0Simple, curveSimple), HCPointMake(1.0, 0.0), 0.0000001));
    
    HCPoint p0SingleInflection = HCPointMake(0.0, 0.0);
    HCContourCurve curveSingleInflection = HCContourCurveMakeCubic(HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(50.0, 200.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(p0SingleInflection, curveSingleInflection) == HCContourCurveTypeCubicSingleInflection);
    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(p0SingleInflection, curveSingleInflection), HCPointMake(0.5, 2.0), 0.0000001));
    
    HCPoint p0DoubleInflection = HCPointMake(0.0, 0.0);
    HCContourCurve curveDoubleInflection = HCContourCurveMakeCubic(HCPointMake(125.0, 75.0), HCPointMake(100.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(p0DoubleInflection, curveDoubleInflection) == HCContourCurveTypeCubicDoubleInflection);
    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(p0DoubleInflection, curveDoubleInflection), HCPointMake(-1.5, 0.5), 0.0000001));
    
    HCPoint p0Loop = HCPointMake(25.0, 0.0);
    HCContourCurve curveLoop = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(p0Loop, curveLoop) == HCContourCurveTypeCubicLoop);
    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(p0Loop, curveLoop), HCPointMake(-0.75, 0.0), 0.0000001));
    
//    HCPoint p0LoopAtStart = HCPointMake(50.0, 0.0);
//    HCContourCurve curveLoopAtStart = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(40.0, 25.0));
//    ASSERT_TRUE(HCContourCurveCanonicalType(p0LoopAtStart, curveLoopAtStart) == HCContourCurveTypeCubicLoopAtStart);
//    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(p0LoopAtStart, curveLoopAtStart), HCPointMake(-0.75, 0.75), 0.0000001));
//
//    HCPoint p0LoopAtEnd = HCPointMake(60.0, 25.0);
//    HCContourCurve curveLoopAtEnd = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(50.0, 0.0));
//    ASSERT_TRUE(HCContourCurveCanonicalType(p0LoopAtEnd, curveLoopAtEnd) == HCContourCurveTypeCubicLoopAtEnd);
//    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(p0LoopAtEnd, curveLoopAtEnd), HCPointMake(0.1, 0.5), 0.0000001));
    
    HCPoint p0LoopClosed = HCPointMake(50.0, 0.0);
    HCContourCurve curveLoopClosed = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(50.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(p0LoopClosed, curveLoopClosed) == HCContourCurveTypeCubicLoopClosed);
    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(p0LoopClosed, curveLoopClosed), HCPointMake(0.0, 0.0), 0.0000001));
    
    HCPoint p0Cusp = HCPointMake(0.0, 0.0);
    HCContourCurve curveCusp = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(p0Cusp, curveCusp) == HCContourCurveTypeCubicLoopCusp);
    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(p0Cusp, curveCusp), HCPointMake(-1.0, 0.0), 0.0000001));
}

CTEST(HCContourCurve, LineEvaluation) {
    HCPoint s = HCPointInvalid;
    HCContourCurveEvaluateLinear(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), 0.5, &s.x, &s.y);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(2.0, 3.0), 0.000001));
}

CTEST(HCContourCurve, QuadraticEvaluation) {
    HCPoint s = HCPointInvalid;
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCContourCurveEvaluateQuadratic(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0), 0.5, &s.x, &s.y, &dx, &dy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 4.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
}

CTEST(HCContourCurve, CubicEvaluation) {
    HCPoint s = HCPointInvalid;
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCReal ddx = NAN;
    HCReal ddy = NAN;
    HCContourCurveEvaluateCubic(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0), HCPointMake(7.0, 8.0), 0.5, &s.x, &s.y, &dx, &dy, &ddx, &ddy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(4.0, 5.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
    ASSERT_DBL_NEAR(ddx, 10.0);
    ASSERT_DBL_NEAR(ddy, 4.0);
}

CTEST(HCContourCurve, ElementEvaluation) {
    HCPoint s = HCPointInvalid;
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCReal ddx = NAN;
    HCReal ddy = NAN;
    HCContourCurveEvaluate(HCPointMake(1.0, 2.0), HCContourCurveMakeCubic(HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0), HCPointMake(7.0, 8.0)), 0.5, &s.x, &s.y, &dx, &dy, &ddx, &ddy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(4.0, 5.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 2.0);
    ASSERT_DBL_NEAR(ddx, 10.0);
    ASSERT_DBL_NEAR(ddy, 4.0);
}

CTEST(HCContour, CreatePolyline) {
    HCPoint startPoint = {.x = 1.0, .y = 1.0};
    HCPoint polyline[] = {
        {.x = 3.0, .y = 1.0},
        {.x = 3.0, .y = 3.0},
        {.x = 1.0, .y = 3.0},
    };
    HCContour contour[4];
    HCContourInitWithPolyline(contour, startPoint, polyline, sizeof(polyline) / sizeof(HCPoint), true);
    ASSERT_TRUE(HCContourIsClosed(contour));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCContourCurveCount(contour) == 4);
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 0), HCContourCurveMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 1), HCContourCurveMakeLinear(HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 2), HCContourCurveMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 3), HCContourCurveMakeLinear(HCPointMake(1.0, 3.0))));
}

CTEST(HCContour, CreatePolyquadratic) {
    HCPoint startPoint = {.x = 1.0, .y = 1.0};
    HCPoint polyquadratic[] = {
        {.x = 2.0, .y = 0.0}, {.x = 3.0, .y = 1.0},
        HCPointInvalidStatic, {.x = 3.0, .y = 3.0},
        {.x = 3.0, .y = 4.0}, {.x = 1.0, .y = 3.0},
    };
    HCContour contour[4];
    HCContourInitWithPolyquadratic(contour, startPoint, polyquadratic, sizeof(polyquadratic) / (sizeof(HCPoint) * 2), true);
    ASSERT_TRUE(HCContourIsClosed(contour));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCContourCurveCount(contour) == 4);
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 0), HCContourCurveMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 1), HCContourCurveMakeQuadratic(HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 2), HCContourCurveMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 3), HCContourCurveMakeQuadratic(HCPointMake(3.0, 4.0), HCPointMake(1.0, 3.0))));
}

CTEST(HCContour, CreatePolycubic) {
    HCPoint startPoint = {.x = 1.0, .y = 1.0};
    HCPoint polycubic[] = {
        {.x = 2.0, .y = 0.0}, HCPointInvalidStatic, {.x = 3.0, .y = 1.0},
        HCPointInvalidStatic, HCPointInvalidStatic, {.x = 3.0, .y = 3.0},
        {.x = 2.5, .y = 4.0}, {.x = 1.5, .y = 4.0}, {.x = 1.0, .y = 3.0},
    };
    HCContour contour[4];
    HCContourInitWithPolycubic(contour, startPoint, polycubic, sizeof(polycubic) / (sizeof(HCPoint) * 3), true);
    ASSERT_TRUE(HCContourIsClosed(contour));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCContourCurveCount(contour) == 4);
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 0), HCContourCurveMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 1), HCContourCurveMakeQuadratic(HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 2), HCContourCurveMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 3), HCContourCurveMakeCubic(HCPointMake(2.5, 4.0), HCPointMake(1.5, 4.0), HCPointMake(1.0, 3.0))));
}

CTEST(HCContour, CreateWithCurves) {
    HCContourCurve curves[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    HCContour contour[4];
    HCContourInitWithCurves(contour, curves, sizeof(curves) / sizeof(HCContourCurve), true);
    ASSERT_TRUE(HCContourIsClosed(contour));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCContourCurveCount(contour) == 4);
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 0), HCContourCurveMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 1), HCContourCurveMakeQuadratic(HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 2), HCContourCurveMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourCurveAt(contour, 3), HCContourCurveMakeCubic(HCPointMake(2.5, 4.0), HCPointMake(1.5, 4.0), HCPointMake(1.0, 3.0))));
}

CTEST(HCContour, Equality) {
    HCContourCurve a[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    HCContour* contourA = HCContourInitInCurves(a, sizeof(a) / sizeof(HCContourCurve), true);
    HCContourCurve b[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    HCContour* contourB = HCContourInitInCurves(b, sizeof(b) / sizeof(HCContourCurve), true);
    HCContourCurve c[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    HCContour* contourC = HCContourInitInCurves(c, sizeof(c) / sizeof(HCContourCurve), false);
    
    ASSERT_TRUE(HCContourIsEqual(contourA, contourA));
    ASSERT_TRUE(HCContourHashValue(contourA) == HCContourHashValue(contourA));
    ASSERT_TRUE(HCContourIsEqual(contourA, contourB));
    ASSERT_TRUE(HCContourHashValue(contourA) == HCContourHashValue(contourB));
    ASSERT_TRUE(HCContourIsEqual(contourB, contourA));
    ASSERT_TRUE(HCContourHashValue(contourB) == HCContourHashValue(contourA));
    ASSERT_TRUE(!HCContourIsEqual(contourA, contourC));
}

CTEST(HCContour, Print) {
    HCContourCurve curves[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    HCContour* contour = HCContourInitInCurves(curves, sizeof(curves) / sizeof(HCContourCurve), true);
    HCContourPrint(contour, stdout); // TODO: Not to stdout
}
