///
/// @file HCContourCurve.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 5/16/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "ctest.h"
#include "../Source/HollowCore.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Order / Conversion
//----------------------------------------------------------------------------------------------------------------------------------
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
    HCContourCurve xAxisAligned = HCContourCurveXAxisAligned(HCPointMake(1.0, 2.0), HCContourCurveMakeCubic(HCPointMake(3.0, 0.0), HCPointMake(5.0, 1.0), HCPointMake(7.0, 8.0)));
    ASSERT_FALSE(HCContourCurveIsInvalid(xAxisAligned));
    ASSERT_FALSE(HCContourCurveIsZero(xAxisAligned));
    ASSERT_FALSE(HCContourCurveIsInfinite(xAxisAligned));
    ASSERT_FALSE(HCContourCurveIsLinear(HCPointZero, xAxisAligned));
    ASSERT_FALSE(HCContourCurveIsQuadratic(HCPointZero, xAxisAligned));
    ASSERT_TRUE(HCContourCurveIsCubic(HCPointZero, xAxisAligned));
    ASSERT_DBL_NEAR(xAxisAligned.p.y, 0.0);
    
    HCContourCurve yAxisAligned = HCContourCurveYAxisAligned(HCPointMake(1.0, 2.0), HCContourCurveMakeCubic(HCPointMake(3.0, 0.0), HCPointMake(5.0, 1.0), HCPointMake(7.0, 8.0)));
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
    
    // TODO: Loop-at-start does not evaluate well
//    HCPoint p0LoopAtStart = HCPointMake(50.0, 0.0);
//    HCContourCurve curveLoopAtStart = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(40.0, 25.0));
//    ASSERT_TRUE(HCContourCurveCanonicalType(p0LoopAtStart, curveLoopAtStart) == HCContourCurveTypeCubicLoopAtStart);
//    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(p0LoopAtStart, curveLoopAtStart), HCPointMake(-0.75, 0.75), 0.0000001));

    // TODO: Loop-at-end does not evaluate well
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
    ASSERT_TRUE(HCContourCurveCanonicalType(p0Cusp, curveCusp) == HCContourCurveTypeCubicCusp);
    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(p0Cusp, curveCusp), HCPointMake(-1.0, 0.0), 0.0000001));
    
    HCPoint coanchorP0 = HCPointMake(1.0, 1.0);
    HCPoint coanchorP1 = HCPointMake(4.0, 1.0);
    HCContourCurve coanchor10 = HCContourCurveMakeCubic(coanchorP0, HCPointMake(3.0, 0.0), coanchorP1);
    ASSERT_TRUE(HCContourCurveCanonicalType(coanchorP0, coanchor10) == HCContourCurveTypeCubicSimple);
    HCContourCurve coanchor01 = HCContourCurveMakeCubic(HCPointMake(2.0, 0.0), coanchorP1, coanchorP1);
    ASSERT_TRUE(HCContourCurveCanonicalType(coanchorP0, coanchor01) == HCContourCurveTypeCubicSimple);
    
    HCPoint colinearP0 = HCPointMake(1.0, 1.0);
    HCContourCurve colinear = HCContourCurveMakeCubic(HCPointMake(2.0, 2.0), HCPointMake(3.0, 3.0), HCPointMake(4.0, 4.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(colinearP0, colinear) == HCContourCurveTypeLinear);
    
    HCPoint colinearQuadraticP0 = HCPointMake(1.0, 1.0);
    HCContourCurve colinearQuadratic = HCContourCurveMakeQuadratic(HCPointMake(2.0, 2.0), HCPointMake(3.0, 3.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(colinearQuadraticP0, colinearQuadratic) == HCContourCurveTypeLinear);
    
    // TODO: Co-quadratic does not evaluate well
//    HCPoint cqp0 = HCPointMake(20.0, 30.0);
//    HCPoint cqc = HCPointMake(25.0, 60.0);
//    HCPoint cqp1 = HCPointMake(30.0, 50.0);
//    HCPoint cqc0 = HCPointMake((2.0/3.0) * (cqc.x - cqp0.x), (2.0/3.0) * (cqc.y - cqp0.y));
//    HCPoint cqc1 = HCPointMake((2.0/3.0) * (cqc.x - cqp1.x), (2.0/3.0) * (cqc.y - cqp1.y));
//    HCContourCurve coquadratic = HCContourCurveMakeCubic(cqc0, cqc1, cqp1);
//    ASSERT_TRUE(HCContourCurveCanonicalType(cqp0, coquadratic) == HCContourCurveTypeQuadratic);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Evaluation
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCContourCurve, LinearEvaluation) {
    HCPoint s = HCPointInvalid;
    HCContourCurveEvaluateLinear(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), 0.5, &s.x, &s.y);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(2.0, 3.0), 0.000001));
}

CTEST(HCContourCurve, QuadraticEvaluation) {
    HCPoint s = HCPointInvalid;
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCContourCurveEvaluateQuadratic(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 2.0), 0.5, &s.x, &s.y, &dx, &dy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 3.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 0.0);
}

CTEST(HCContourCurve, CubicEvaluation) {
    HCPoint s = HCPointInvalid;
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCReal ddx = NAN;
    HCReal ddy = NAN;
    HCContourCurveEvaluateCubic(HCPointMake(1.0, 2.0), HCPointMake(2.0, 4.0), HCPointMake(4.0, 4.0), HCPointMake(5.0, 2.0), 0.5, &s.x, &s.y, &dx, &dy, &ddx, &ddy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 3.5), 0.000001));
    ASSERT_DBL_NEAR(dx, 1.5);
    ASSERT_DBL_NEAR(dy, 0.0);
    ASSERT_DBL_NEAR(ddx, 3.0);
    ASSERT_DBL_NEAR(ddy, 0.0);
}

CTEST(HCContourCurve, Evaluation) {
    HCPoint s = HCPointInvalid;
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCReal ddx = NAN;
    HCReal ddy = NAN;
    HCContourCurveEvaluate(HCPointMake(1.0, 2.0), HCContourCurveMakeCubic(HCPointMake(2.0, 4.0), HCPointMake(4.0, 4.0), HCPointMake(5.0, 2.0)), 0.5, &s.x, &s.y, &dx, &dy, &ddx, &ddy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 3.5), 0.000001));
    ASSERT_DBL_NEAR(dx, 1.5);
    ASSERT_DBL_NEAR(dy, 0.0);
    ASSERT_DBL_NEAR(ddx, 3.0);
    ASSERT_DBL_NEAR(ddy, 0.0);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Curvature
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Derivative
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Extrema
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearExtrema) {
    HCInteger count = 0;
    HCContourCurveExtremaLinear(HCPointMake(-1.0, 2.0), HCPointMake(3.0, -4.0), &count, NULL);
    ASSERT_TRUE(count == 0);
}

CTEST(HCContourCurve, QuadraticExtrema) {
    HCInteger count = 0;
    HCReal extrema[2];
    HCContourCurveExtremaQuadratic(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 2.0), &count, extrema);
    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(extrema[0] == 0.5);
}

CTEST(HCContourCurve, CubicExtrema) {
    HCInteger count = 0;
    HCReal extrema[6];
    HCContourCurveExtremaCubic(HCPointMake(1.0, 2.0), HCPointMake(2.0, 4.0), HCPointMake(4.0, 4.0), HCPointMake(5.0, 2.0), &count, extrema);
    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(extrema[0] == 0.5);
}

CTEST(HCContourCurve, Extrema) {
    HCInteger count = 0;
    HCReal extrema[6];
    HCContourCurveExtrema(HCPointMake(1.0, 2.0), HCContourCurveMakeCubic(HCPointMake(2.0, 4.0), HCPointMake(4.0, 4.0), HCPointMake(5.0, 2.0)), &count, extrema);
    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(extrema[0] == 0.5);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Inflection
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Bounds
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearBounds) {
    HCRectangle bounds = HCContourCurveBoundsLinear(HCPointMake(-1.0, 2.0), HCPointMake(3.0, -4.0));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(-1.0, -4.0, 3.0, 2.0)));
}

CTEST(HCContourCurve, QuadraticBounds) {
    HCRectangle bounds = HCContourCurveBoundsQuadratic(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 2.0));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(1.0, 2.0, 5.0, 3.0)));
}

CTEST(HCContourCurve, CubicBounds) {
    HCRectangle bounds = HCContourCurveBoundsCubic(HCPointMake(1.0, 2.0), HCPointMake(2.0, 4.0), HCPointMake(4.0, 4.0), HCPointMake(5.0, 2.0));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(1.0, 2.0, 5.0, 3.5)));
}

CTEST(HCContourCurve, Bounds) {
    HCRectangle bounds = HCContourCurveBounds(HCPointMake(1.0, 2.0), HCContourCurveMakeCubic(HCPointMake(2.0, 4.0), HCPointMake(4.0, 4.0), HCPointMake(5.0, 2.0)));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(1.0, 2.0, 5.0, 3.5)));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Length
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameterization by Arc Length
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameter Given Point
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameter Given Axis
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Intersection
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Projection
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Moulding
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Interpolation
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Fitting
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Split
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests
