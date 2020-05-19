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
// MARK: - Order
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCContourCurve, Order) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint c0 = HCPointMake(3.0, -4.0);
    HCPoint c1 = HCPointMake(-5.0, 6.0);
    HCPoint p1 = HCPointMake(7.0, -8.0);
    
    HCContourCurve linear = HCContourCurveMakeLinear(p1);
    ASSERT_FALSE(HCContourCurveIsInvalid(linear));
    ASSERT_FALSE(HCContourCurveIsZero(linear));
    ASSERT_FALSE(HCContourCurveIsInfinite(linear));
    ASSERT_TRUE(HCContourCurveIsLinear(p0, linear));
    ASSERT_FALSE(HCContourCurveIsQuadratic(p0, linear));
    ASSERT_FALSE(HCContourCurveIsCubic(p0, linear));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsLinear(p0, linear), linear, 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsQuadratic(p0, linear), HCContourCurveMakeQuadratic(p1, p1), 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsCubic(p0, linear), HCContourCurveMakeCubic(p0, p1, p1), 0.000001));
    
    HCContourCurve quadratic = HCContourCurveMakeQuadratic(c0, p1);
    ASSERT_FALSE(HCContourCurveIsInvalid(quadratic));
    ASSERT_FALSE(HCContourCurveIsZero(quadratic));
    ASSERT_FALSE(HCContourCurveIsInfinite(quadratic));
    ASSERT_FALSE(HCContourCurveIsLinear(p0, quadratic));
    ASSERT_TRUE(HCContourCurveIsQuadratic(p0, quadratic));
    ASSERT_FALSE(HCContourCurveIsCubic(p0, quadratic));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsLinear(p0, quadratic), HCContourCurveMakeLinear(quadratic.p), 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsQuadratic(p0, quadratic), quadratic, 0.000001));
    HCPoint qc0 = HCPointMake(p0.x + (2.0/3.0) * (c0.x - p0.x), p0.y + (2.0/3.0) * (c0.y - p0.y));
    HCPoint qc1 = HCPointMake(p1.x + (2.0/3.0) * (c0.x - p1.x), p1.y + (2.0/3.0) * (c0.y - p1.y));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsCubic(p0, quadratic), HCContourCurveMakeCubic(qc0, qc1, p1), 0.000001));
    
    HCContourCurve cubic = HCContourCurveMakeCubic(c0, c1, p1);
    ASSERT_FALSE(HCContourCurveIsInvalid(cubic));
    ASSERT_FALSE(HCContourCurveIsZero(cubic));
    ASSERT_FALSE(HCContourCurveIsInfinite(cubic));
    ASSERT_FALSE(HCContourCurveIsLinear(p0, cubic));
    ASSERT_FALSE(HCContourCurveIsQuadratic(p0, cubic));
    ASSERT_TRUE(HCContourCurveIsCubic(p0, cubic));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsLinear(p0, cubic), HCContourCurveMakeLinear(cubic.p), 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsQuadratic(p0, cubic), HCContourCurveMakeQuadratic(HCPointInterpolate(cubic.c0, cubic.c1, 0.5), cubic.p), 0.000001));
    ASSERT_TRUE(HCContourCurveIsSimilar(HCContourCurveAsCubic(p0, cubic), cubic, 0.000001));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Canonical Type
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCContourCurve, Canonical) {
    HCPoint invalidP0 = HCPointInvalid;
    HCContourCurve invalid = HCContourCurveInvalid;
    ASSERT_TRUE(HCContourCurveCanonicalType(invalidP0, invalid) == HCContourCurveTypeInvalid);
    
    HCPoint pointP0 = HCPointMake(1.0, 1.0);
    HCContourCurve point = HCContourCurveMakeCubic(pointP0, pointP0, pointP0);
    ASSERT_TRUE(HCContourCurveCanonicalType(pointP0, point) == HCContourCurveTypePoint);
    
    HCPoint linearP0 = HCPointMake(1.0, 1.0);
    HCContourCurve linear = HCContourCurveMakeLinear(HCPointMake(2.0, 2.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(linearP0, linear) == HCContourCurveTypeLinear);
    
    HCPoint quadraticP0 = HCPointMake(1.0, 1.0);
    HCContourCurve quadratic = HCContourCurveMakeQuadratic(HCPointMake(2.0, 2.0), HCPointMake(3.0, 1.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(quadraticP0, quadratic) == HCContourCurveTypeQuadratic);
    
    HCPoint simpleP0 = HCPointMake(0.0, 0.0);
    HCContourCurve simple = HCContourCurveMakeCubic(HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(simpleP0, simple) == HCContourCurveTypeCubicSimple);
    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(simpleP0, simple.c0, simple.c1, simple.p), HCPointMake(1.0, 0.0), 0.0000001));
    
    HCPoint singleInflectionP0 = HCPointMake(0.0, 0.0);
    HCContourCurve singleInflection = HCContourCurveMakeCubic(HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(50.0, 200.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(singleInflectionP0, singleInflection) == HCContourCurveTypeCubicSingleInflection);
    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(singleInflectionP0, singleInflection.c0, singleInflection.c1, singleInflection.p), HCPointMake(0.5, 2.0), 0.0000001));
    
    HCPoint doubleInflectionP0 = HCPointMake(0.0, 0.0);
    HCContourCurve doubleInflection = HCContourCurveMakeCubic(HCPointMake(125.0, 75.0), HCPointMake(100.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(doubleInflectionP0, doubleInflection) == HCContourCurveTypeCubicDoubleInflection);
    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(doubleInflectionP0, doubleInflection.c0, doubleInflection.c1, doubleInflection.p), HCPointMake(-1.5, 0.5), 0.0000001));
    
    HCPoint loopP0 = HCPointMake(25.0, 0.0);
    HCContourCurve loop = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(loopP0, loop) == HCContourCurveTypeCubicLoop);
    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(loopP0, loop.c0, loop.c1, loop.p), HCPointMake(-0.75, 0.0), 0.0000001));
    
    // TODO: Loop-at-start does not evaluate well
//    HCPoint loopAtStartP0 = HCPointMake(50.0, 0.0);
//    HCContourCurve loopAtStart = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(40.0, 25.0));
//    ASSERT_TRUE(HCContourCurveCanonicalType(loopAtStartP0, loopAtStart) == HCContourCurveTypeCubicLoopAtStart);
//    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(loopAtStartP0, loopAtStart.c0, loopAtStart.c1, loopAtStart.p), HCPointMake(-0.75, 0.75), 0.0000001));

    // TODO: Loop-at-end does not evaluate well
//    HCPoint loopAtEndP0 = HCPointMake(60.0, 25.0);
//    HCContourCurve loopAtEnd = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(50.0, 0.0));
//    ASSERT_TRUE(HCContourCurveCanonicalType(loopAtEndP0, loopAtEnd) == HCContourCurveTypeCubicLoopAtEnd);
//    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(loopAtEndP0, loopAtEnd.c0, loopAtEnd.c1, loopAtEnd.p), HCPointMake(0.1, 0.5), 0.0000001));
    
    HCPoint loopClosedP0 = HCPointMake(50.0, 0.0);
    HCContourCurve loopClosed = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(50.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(loopClosedP0, loopClosed) == HCContourCurveTypeCubicLoopClosed);
    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(loopClosedP0, loopClosed.c0, loopClosed.c1, loopClosed.p), HCPointMake(0.0, 0.0), 0.0000001));
    
    HCPoint cuspP0 = HCPointMake(0.0, 0.0);
    HCContourCurve cusp = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(cuspP0, cusp) == HCContourCurveTypeCubicCusp);
    ASSERT_TRUE(HCPointIsSimilar(HCContourCurveCanonical(cuspP0, cusp.c0, cusp.c1, cusp.p), HCPointMake(-1.0, 0.0), 0.0000001));
    
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
    
    HCPoint cqp0 = HCPointMake(20.0, 30.0);
    HCPoint cqc = HCPointMake(25.0, 60.0);
    HCPoint cqp1 = HCPointMake(30.0, 50.0);
    HCPoint cqc0 = HCPointMake(cqp0.x + (2.0/3.0) * (cqc.x - cqp0.x), cqp0.y + (2.0/3.0) * (cqc.y - cqp0.y));
    HCPoint cqc1 = HCPointMake(cqp1.x + (2.0/3.0) * (cqc.x - cqp1.x), cqp1.y + (2.0/3.0) * (cqc.y - cqp1.y));
    HCContourCurve coquadratic = HCContourCurveMakeCubic(cqc0, cqc1, cqp1);
    ASSERT_TRUE(HCContourCurveCanonicalType(cqp0, coquadratic) == HCContourCurveTypeQuadratic);
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
// MARK: - Axis Alignment
//----------------------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Split
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

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
