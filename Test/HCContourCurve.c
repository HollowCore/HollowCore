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
    ASSERT_TRUE(HCContourCurveCanonicalType(invalidP0, invalid) == HCCurveTypeInvalid);
    
    HCPoint pointP0 = HCPointMake(1.0, 1.0);
    HCContourCurve point = HCContourCurveMakeCubic(pointP0, pointP0, pointP0);
    ASSERT_TRUE(HCContourCurveCanonicalType(pointP0, point) == HCCurveTypePoint);
    
    HCPoint linearP0 = HCPointMake(1.0, 1.0);
    HCContourCurve linear = HCContourCurveMakeLinear(HCPointMake(2.0, 2.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(linearP0, linear) == HCCurveTypeLinear);
    
    HCPoint quadraticP0 = HCPointMake(1.0, 1.0);
    HCContourCurve quadratic = HCContourCurveMakeQuadratic(HCPointMake(2.0, 2.0), HCPointMake(3.0, 1.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(quadraticP0, quadratic) == HCCurveTypeQuadratic);
    
    HCPoint simpleP0 = HCPointMake(0.0, 0.0);
    HCContourCurve simple = HCContourCurveMakeCubic(HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(simpleP0, simple) == HCCurveTypeCubicSimple);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(simpleP0, simple.c0, simple.c1, simple.p), HCPointMake(1.0, 0.0), 0.0000001));
    
    HCPoint singleInflectionP0 = HCPointMake(0.0, 0.0);
    HCContourCurve singleInflection = HCContourCurveMakeCubic(HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(50.0, 200.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(singleInflectionP0, singleInflection) == HCCurveTypeCubicSingleInflection);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(singleInflectionP0, singleInflection.c0, singleInflection.c1, singleInflection.p), HCPointMake(0.5, 2.0), 0.0000001));
    
    HCPoint doubleInflectionP0 = HCPointMake(0.0, 0.0);
    HCContourCurve doubleInflection = HCContourCurveMakeCubic(HCPointMake(125.0, 75.0), HCPointMake(100.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(doubleInflectionP0, doubleInflection) == HCCurveTypeCubicDoubleInflection);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(doubleInflectionP0, doubleInflection.c0, doubleInflection.c1, doubleInflection.p), HCPointMake(-1.5, 0.5), 0.0000001));
    
    HCPoint loopP0 = HCPointMake(25.0, 0.0);
    HCContourCurve loop = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(loopP0, loop) == HCCurveTypeCubicLoop);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(loopP0, loop.c0, loop.c1, loop.p), HCPointMake(-0.75, 0.0), 0.0000001));
    
    HCPoint loopAtStartP0 = HCPointMake(0.0, 0.0);
    HCContourCurve loopAtStart = HCContourCurveMakeCubic(HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(-300.0, -600.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(loopAtStartP0, loopAtStart) == HCCurveTypeCubicLoopAtStart);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(loopAtStartP0, loopAtStart.c0, loopAtStart.c1, loopAtStart.p), HCPointMake(-3.0, -6.0), 0.0000001));

    HCPoint loopAtEndP0 = HCPointMake(0.0, 0.0);
    HCContourCurve loopAtEnd = HCContourCurveMakeCubic(HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(50.0, ((sqrt(5.25) - 0.5) * 0.5) * 100.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(loopAtEndP0, loopAtEnd) == HCCurveTypeCubicLoopAtEnd);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(loopAtEndP0, loopAtEnd.c0, loopAtEnd.c1, loopAtEnd.p), HCPointMake(0.5, ((sqrt(5.25) - 0.5) * 0.5)), 0.0000001));
    
    HCPoint loopClosedP0 = HCPointMake(50.0, 0.0);
    HCContourCurve loopClosed = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(50.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(loopClosedP0, loopClosed) == HCCurveTypeCubicLoopClosed);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(loopClosedP0, loopClosed.c0, loopClosed.c1, loopClosed.p), HCPointMake(0.0, 0.0), 0.0000001));
    
    HCPoint cuspP0 = HCPointMake(0.0, 0.0);
    HCContourCurve cusp = HCContourCurveMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(cuspP0, cusp) == HCCurveTypeCubicCusp);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(cuspP0, cusp.c0, cusp.c1, cusp.p), HCPointMake(-1.0, 0.0), 0.0000001));
    
    HCPoint coanchorP0 = HCPointMake(1.0, 1.0);
    HCPoint coanchorP1 = HCPointMake(4.0, 1.0);
    HCContourCurve coanchor10 = HCContourCurveMakeCubic(coanchorP0, HCPointMake(3.0, 0.0), coanchorP1);
    ASSERT_TRUE(HCContourCurveCanonicalType(coanchorP0, coanchor10) == HCCurveTypeCubicSimple);
    HCContourCurve coanchor01 = HCContourCurveMakeCubic(HCPointMake(2.0, 0.0), coanchorP1, coanchorP1);
    ASSERT_TRUE(HCContourCurveCanonicalType(coanchorP0, coanchor01) == HCCurveTypeCubicSimple);
    
    HCPoint colinearP0 = HCPointMake(1.0, 1.0);
    HCContourCurve colinear = HCContourCurveMakeCubic(HCPointMake(2.0, 2.0), HCPointMake(3.0, 3.0), HCPointMake(4.0, 4.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(colinearP0, colinear) == HCCurveTypeLinear);
    
    HCPoint colinearQuadraticP0 = HCPointMake(1.0, 1.0);
    HCContourCurve colinearQuadratic = HCContourCurveMakeQuadratic(HCPointMake(2.0, 2.0), HCPointMake(3.0, 3.0));
    ASSERT_TRUE(HCContourCurveCanonicalType(colinearQuadraticP0, colinearQuadratic) == HCCurveTypeLinear);
    
    HCPoint cqp0 = HCPointMake(20.0, 30.0);
    HCPoint cqc = HCPointMake(25.0, 60.0);
    HCPoint cqp1 = HCPointMake(30.0, 50.0);
    HCPoint cqc0 = HCPointMake(cqp0.x + (2.0/3.0) * (cqc.x - cqp0.x), cqp0.y + (2.0/3.0) * (cqc.y - cqp0.y));
    HCPoint cqc1 = HCPointMake(cqp1.x + (2.0/3.0) * (cqc.x - cqp1.x), cqp1.y + (2.0/3.0) * (cqc.y - cqp1.y));
    HCContourCurve coquadratic = HCContourCurveMakeCubic(cqc0, cqc1, cqp1);
    ASSERT_TRUE(HCContourCurveCanonicalType(cqp0, coquadratic) == HCCurveTypeQuadratic);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Evaluation
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCContourCurve, LinearEvaluation) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, 4.0);
    HCContourCurve curve = HCContourCurveMakeLinear(p1);
    HCPoint s = HCPointInvalid;
    HCContourCurveEvaluate(p0, curve, 0.5, &s.x, &s.y, NULL, NULL, NULL, NULL);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(2.0, 3.0), 0.000001));
}

CTEST(HCContourCurve, QuadraticEvaluation) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint pc = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeQuadratic(pc, p1);
    HCPoint s = HCPointInvalid;
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCContourCurveEvaluate(p0, curve, 0.5, &s.x, &s.y, &dx, &dy, NULL, NULL);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 3.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 0.0);
}

CTEST(HCContourCurve, CubicEvaluation) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeCubic(c0, c1, p1);
    HCPoint s = HCPointInvalid;
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCReal ddx = NAN;
    HCReal ddy = NAN;
    HCContourCurveEvaluate(p0, curve, 0.5, &s.x, &s.y, &dx, &dy, &ddx, &ddy);
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
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(3.0, 0.0);
    HCPoint c1 = HCPointMake(5.0, 1.0);
    HCPoint p1 = HCPointMake(7.0, 8.0);
    HCContourCurve curve = HCContourCurveMakeCubic(c0, c1, p1);
    HCContourCurve xAxisAligned = HCContourCurveXAxisAligned(p0, curve);
    ASSERT_FALSE(HCContourCurveIsInvalid(xAxisAligned));
    ASSERT_FALSE(HCContourCurveIsZero(xAxisAligned));
    ASSERT_FALSE(HCContourCurveIsInfinite(xAxisAligned));
    ASSERT_FALSE(HCContourCurveIsLinear(HCPointZero, xAxisAligned));
    ASSERT_FALSE(HCContourCurveIsQuadratic(HCPointZero, xAxisAligned));
    ASSERT_TRUE(HCContourCurveIsCubic(HCPointZero, xAxisAligned));
    ASSERT_DBL_NEAR(xAxisAligned.p.y, 0.0);
    
    HCContourCurve yAxisAligned = HCContourCurveYAxisAligned(p0, curve);
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
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourCurve curve = HCContourCurveMakeLinear(p1);
    HCInteger count = 0;
    HCContourCurveExtrema(p0, curve, &count, NULL);
    ASSERT_TRUE(count == 0);
}

CTEST(HCContourCurve, QuadraticExtrema) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint pc = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeQuadratic(pc, p1);
    HCInteger count = 0;
    HCReal extrema[2];
    HCContourCurveExtrema(p0, curve, &count, extrema);
    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(extrema[0] == 0.5);
}

CTEST(HCContourCurve, CubicExtrema) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeCubic(c0, c1, p1);
    HCInteger count = 0;
    HCReal extrema[6];
    HCContourCurveExtrema(p0, curve, &count, extrema);
    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(extrema[0] == 0.5);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Inflection
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearInflections) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourCurve curve = HCContourCurveMakeLinear(p1);
    HCInteger count = 0;
    HCContourCurveInflections(p0, curve, &count, NULL);
    ASSERT_TRUE(count == 0);
}

CTEST(HCContourCurve, QuadraticInflection) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint pc = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeQuadratic(pc, p1);
    HCInteger count = 0;
    HCContourCurveInflections(p0, curve, &count, NULL);
    ASSERT_TRUE(count == 0);
}

CTEST(HCContourCurve, CubicInflection) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint c0 = HCPointMake(0.0, 2.0);
    HCPoint c1 = HCPointMake(2.0, 2.0);
    HCPoint p1 = HCPointMake(1.0, 4.0);
    HCContourCurve curve = HCContourCurveMakeCubic(c0, c1, p1);
    HCInteger count = 0;
    HCReal inflections[2];
    HCContourCurveInflections(p0, curve, &count, inflections);
    ASSERT_TRUE(HCCurveCanonicalTypeCubic(p0, c0, c1, p1) == HCCurveTypeCubicSingleInflection);
    ASSERT_TRUE(count == 1);
    ASSERT_DBL_NEAR(inflections[0], 0.4384);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Bounds
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearApproximateBounds) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourCurve curve = HCContourCurveMakeLinear(p1);
    HCRectangle bounds = HCContourCurveApproximateBounds(p0, curve);
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(
        fmin(p0.x, p1.x),
        fmin(p0.y, p1.y),
        fmax(p0.x, p1.x),
        fmax(p0.y, p1.y))));
}

CTEST(HCContourCurve, QuadraticApproximateBounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint pc = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeQuadratic(pc, p1);
    HCRectangle bounds = HCContourCurveApproximateBounds(p0, curve);
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(
        fmin(p0.x, fmin(pc.x, p1.x)),
        fmin(p0.y, fmin(pc.y, p1.y)),
        fmax(p0.x, fmax(pc.x, p1.x)),
        fmax(p0.y, fmax(pc.y, p1.y)))));
}

CTEST(HCContourCurve, CubicApproximateBounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeCubic(c0, c1, p1);
    HCRectangle bounds = HCContourCurveApproximateBounds(p0, curve);
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(
        fmin(p0.x, fmin(c0.x, fmin(c1.x, p1.x))),
        fmin(p0.y, fmin(c0.y, fmin(c1.y, p1.y))),
        fmax(p0.x, fmax(c0.x, fmax(c1.x, p1.x))),
        fmax(p0.y, fmax(c0.y, fmax(c1.y, p1.y))))));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Bounds
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearBounds) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourCurve curve = HCContourCurveMakeLinear(p1);
    HCRectangle bounds = HCContourCurveBounds(p0, HCContourCurveMakeLinear(p1));
    ASSERT_TRUE(HCRectangleContainsRectangle(HCContourCurveApproximateBounds(p0, curve), bounds));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(fmin(p0.x, p1.x), fmin(p0.y, p1.y), fmax(p0.x, p1.x), fmax(p0.y, p1.y))));
}

CTEST(HCContourCurve, QuadraticBounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint pc = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeQuadratic(pc, p1);
    HCRectangle bounds = HCContourCurveBounds(p0, curve);
    ASSERT_TRUE(HCRectangleContainsRectangle(HCContourCurveApproximateBounds(p0, curve), bounds));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(1.0, 2.0, 5.0, 3.0)));
}

CTEST(HCContourCurve, CubicBounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeCubic(c0, c1, p1);
    HCRectangle bounds = HCContourCurveBounds(p0, curve);
    ASSERT_TRUE(HCRectangleContainsRectangle(HCContourCurveApproximateBounds(p0, curve), bounds));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(1.0, 2.0, 5.0, 3.5)));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Length
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearLength) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourCurve curve = HCContourCurveMakeLinear(p1);
    HCReal length = HCContourCurveLength(p0, curve);
    ASSERT_DBL_NEAR(length, HCPointDistance(p0, p1));
}

CTEST(HCContourCurve, QuadraticLength) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeQuadratic(c, p1);
    HCReal length = HCContourCurveLength(p0, curve);
    ASSERT_TRUE(length > HCPointDistance(p0, p1) && length < HCPointDistance(p0, c) + HCPointDistance(c, p1));
}

CTEST(HCContourCurve, CubicLength) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeCubic(c0, c1, p1);
    HCReal length = HCContourCurveLength(p0, curve);
    ASSERT_TRUE(length > HCPointDistance(p0, p1) && length < HCPointDistance(p0, c0) + HCPointDistance(c0, c1) + HCPointDistance(c1, p1));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameterization by Arc Length
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearParameterizedByArcLength) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourCurve curve = HCContourCurveMakeLinear(p1);
    HCReal length = HCContourCurveLength(p0, curve);
    for (HCReal d = 0.0; d < length; d += 0.1) {
        HCReal t = HCContourCurveParameter(p0, curve, d);
        ASSERT_DBL_NEAR(t, length / d);
    }
}

CTEST(HCContourCurve, QuadraticParameterizedByArcLength) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeQuadratic(c, p1);
    HCReal length = HCContourCurveLength(p0, curve);
    HCReal previousT = 0.0;
    for (HCReal d = 0.0; d < length; d += 0.1) {
        HCReal t = HCContourCurveParameter(p0, curve, d);
        ASSERT_TRUE(t >= 0.0);
        ASSERT_TRUE(t <= 1.0);
        ASSERT_TRUE(t >= previousT);
        previousT = t;
    }
}

CTEST(HCContourCurve, CubicParameterizedByArcLength) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeCubic(c0, c1, p1);
    HCReal length = HCContourCurveLength(p0, curve);
    HCReal previousT = 0.0;
    for (HCReal d = 0.0; d < length; d += 0.1) {
        HCReal t = HCContourCurveParameter(p0, curve, d);
        ASSERT_TRUE(t >= 0.0);
        ASSERT_TRUE(t <= 1.0);
        ASSERT_TRUE(t >= previousT);
        previousT = t;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameter Given Point
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearParameterNearestPoint) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourCurve curve = HCContourCurveMakeLinear(p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal nearest = HCContourCurveParameterNearestPoint(p0, curve, p);
    ASSERT_TRUE(nearest >= 0.0);
    ASSERT_TRUE(nearest <= 1.0);
}

CTEST(HCContourCurve, QuadraticParameterNearestPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeQuadratic(c, p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal nearest = HCContourCurveParameterNearestPoint(p0, curve, p);
    ASSERT_TRUE(nearest >= 0.0);
    ASSERT_TRUE(nearest <= 1.0);
}

CTEST(HCContourCurve, CubicParameterNearestPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeCubic(c0, c1, p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal nearest = HCContourCurveParameterNearestPoint(p0, curve, p);
    ASSERT_TRUE(nearest >= 0.0);
    ASSERT_TRUE(nearest <= 1.0);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Distance from Point
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearDistanceFromPoint) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourCurve curve = HCContourCurveMakeLinear(p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal distance = HCContourCurveDistanceFromPoint(p0, curve, p);
    ASSERT_TRUE(distance <= HCPointDistance(p, p0));
    ASSERT_TRUE(distance <= HCPointDistance(p, p1));
}

CTEST(HCContourCurve, QuadraticDistanceFromPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeQuadratic(c, p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal distance = HCContourCurveDistanceFromPoint(p0, curve, p);
    ASSERT_TRUE(distance <= HCPointDistance(p, p0));
    ASSERT_TRUE(distance <= HCPointDistance(p, c));
    ASSERT_TRUE(distance <= HCPointDistance(p, p1));
}

CTEST(HCContourCurve, CubicDistanceFromPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeCubic(c0, c1, p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal distance = HCContourCurveDistanceFromPoint(p0, curve, p);
    ASSERT_TRUE(distance <= HCPointDistance(p, p0));
    ASSERT_TRUE(distance <= HCPointDistance(p, c0));
    ASSERT_TRUE(distance <= HCPointDistance(p, c1));
    ASSERT_TRUE(distance <= HCPointDistance(p, p1));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Baseline Projection
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearBaselineProjection) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourCurve curve = HCContourCurveMakeLinear(p1);
    HCPoint b = HCContourCurveBaselineProjection(p0, curve, 0.25);
    ASSERT_DBL_NEAR((p1.y - p0.y) / (p1.x - p0.x), (b.y - p0.y) / (b.x - p0.x));
}

CTEST(HCContourCurve, QuadraticBaselineProjection) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeQuadratic(c, p1);
    HCPoint b = HCContourCurveBaselineProjection(p0, curve, 0.25);
    ASSERT_DBL_NEAR((p1.y - p0.y) / (p1.x - p0.x), (b.y - p0.y) / (b.x - p0.x));
}

CTEST(HCContourCurve, CubicBaselineProjection) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeCubic(c0, c1, p1);
    HCPoint b = HCContourCurveBaselineProjection(p0, curve, 0.25);
    ASSERT_DBL_NEAR((p1.y - p0.y) / (p1.x - p0.x), (b.y - p0.y) / (b.x - p0.x));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Interpolation
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearInterpolatingCurve) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCPoint p = HCPointMake(2.0, 3.0);
    HCReal t = 1.0;
    HCReal dx = 0.0;
    HCReal dy = 0.0;
    HCContourCurve curve = HCContourCurveInterpolatingPoint(p0, p1, p, t, dx, dy);
    ASSERT_DBL_NEAR(HCContourCurveDistanceFromPoint(p0, curve, p), 0.0);
    HCReal cdx;
    HCReal cdy;
    HCContourCurveEvaluate(p0, curve, t, NULL, NULL, &cdx, &cdy, NULL, NULL);
    ASSERT_DBL_NEAR(dx, cdx);
    ASSERT_DBL_NEAR(dy, cdy);
}

CTEST(HCContourCurve, QuadraticInterpolatingCurve) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCPoint p = HCPointMake(2.0, 3.0);
    HCReal t = 0.25;
    HCReal dx = 0.0;
    HCReal dy = 0.0;
    HCContourCurve curve = HCContourCurveInterpolatingPoint(p0, p1, p, t, dx, dy);
    ASSERT_DBL_NEAR(HCContourCurveDistanceFromPoint(p0, curve, p), 0.0);
    HCReal cdx;
    HCReal cdy;
    HCContourCurveEvaluate(p0, curve, t, NULL, NULL, &cdx, &cdy, NULL, NULL);
    ASSERT_DBL_NEAR(dx, cdx);
    ASSERT_DBL_NEAR(dy, cdy);
}

CTEST(HCContourCurve, CubicInterpolatingCurve) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(4.0, 2.0);
    HCPoint p = HCPointMake(2.0, 3.0);
    HCReal t = 0.25;
    HCReal dx = -2.0;
    HCReal dy = 2.0;
    HCContourCurve curve = HCContourCurveInterpolatingPoint(p0, p1, p, t, dx, dy);
    ASSERT_DBL_NEAR(HCContourCurveDistanceFromPoint(p0, curve, p), 0.0);
    HCReal cdx;
    HCReal cdy;
    HCContourCurveEvaluate(p0, curve, t, NULL, NULL, &cdx, &cdy, NULL, NULL);
    ASSERT_DBL_NEAR(dx, cdx);
    ASSERT_DBL_NEAR(dy, cdy);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Moulding
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearMould) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourCurve curve = HCContourCurveMakeLinear(p1);
    HCReal t = 0.25;
    HCPoint p = HCPointMake(2.0, 3.0);
    HCContourCurve moulded = HCContourCurveMould(p0, curve, t, p);
    ASSERT_TRUE(HCContourCurveIsEqual(curve, moulded));
}

CTEST(HCContourCurve, QuadraticMould) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeQuadratic(c, p1);
    HCReal t = 0.25;
    HCPoint p = HCPointMake(2.0, 3.0);
    HCContourCurve moulded = HCContourCurveMould(p0, curve, t, p);
    ASSERT_DBL_FAR(HCContourCurveDistanceFromPoint(p0, curve, p), 0.0);
    ASSERT_DBL_NEAR(HCContourCurveDistanceFromPoint(p0, moulded, p), 0.0);
    ASSERT_TRUE(HCPointIsSimilar(p, HCContourCurveValue(p0, moulded, t), 0.00001));
}

CTEST(HCContourCurve, CubicMould) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourCurve curve = HCContourCurveMakeCubic(c0, c1, p1);
    HCReal t = 0.25;
    HCPoint p = HCPointMake(2.0, 3.0);
    HCContourCurve moulded = HCContourCurveMould(p0, curve, t, p);
    ASSERT_DBL_FAR(HCContourCurveDistanceFromPoint(p0, curve, p), 0.0);
    ASSERT_DBL_NEAR(HCContourCurveDistanceFromPoint(p0, moulded, p), 0.0);
    ASSERT_TRUE(HCPointIsSimilar(p, HCContourCurveValue(p0, moulded, t), 0.00001));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Fitting
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Intersection
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, IntersectionLinearLinear) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint p1 = HCPointMake(100.0, 100.0);
    HCPoint q0 = HCPointMake(100.0, 0.0);
    HCPoint q1 = HCPointMake(0.0, 100.0);
    HCContourCurve cp = HCContourCurveMakeLinear(p1);
    HCContourCurve cq = HCContourCurveMakeLinear(q1);
    HCInteger count = 0;
    HCReal t = NAN;
    HCReal u = NAN;
    HCContourCurveIntersection(p0, cp, q0, cq, &count, &t, &u);
    ASSERT_TRUE(count == 1);
    ASSERT_DBL_NEAR(t, 0.5);
    ASSERT_DBL_NEAR(u, 0.5);
}

CTEST(HCContourCurve, IntersectionLinearQuadratic) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint p1 = HCPointMake(100.0, 0.0);
    HCPoint q0 = HCPointMake(0.0, -50.0);
    HCPoint qc = HCPointMake(50.0, 100.0);
    HCPoint q1 = HCPointMake(100.0, -50.0);
    HCContourCurve cp = HCContourCurveMakeLinear(p1);
    HCContourCurve cq = HCContourCurveMakeQuadratic(qc, q1);
    HCInteger count = 0;
    HCReal t[2] = {NAN, NAN};
    HCReal u[2] = {NAN, NAN};
    HCContourCurveIntersection(p0, cp, q0, cq, &count, t, u);
    ASSERT_TRUE(count == 2);
    ASSERT_DBL_NEAR(t[0], 0.2113);
    ASSERT_DBL_NEAR(t[1], 0.7887);
    ASSERT_DBL_NEAR(u[0], 0.2113);
    ASSERT_DBL_NEAR(u[1], 0.7887);
}

CTEST(HCContourCurve, IntersectionLinearCubic) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint p1 = HCPointMake(150.0, 0.0);
    HCPoint q0 = HCPointMake(0.0, 50.0);
    HCPoint qc0 = HCPointMake(50.0, -100.0);
    HCPoint qc1 = HCPointMake(100.0, 100.0);
    HCPoint q1 = HCPointMake(150.0, -50.0);
    HCContourCurve cp = HCContourCurveMakeLinear(p1);
    HCContourCurve cq = HCContourCurveMakeCubic(qc0, qc1, q1);
    HCInteger count = 0;
    HCReal t[3] = {NAN, NAN};
    HCReal u[3] = {NAN, NAN};
    HCContourCurveIntersection(p0, cp, q0, cq, &count, t, u);
    ASSERT_TRUE(count == 3);
    ASSERT_DBL_NEAR(t[0], 0.8273);
    ASSERT_DBL_NEAR(t[1], 0.1726);
    ASSERT_DBL_NEAR(t[2], 0.5000);
    ASSERT_DBL_NEAR(u[0], 0.8273);
    ASSERT_DBL_NEAR(u[1], 0.1726);
    ASSERT_DBL_NEAR(u[2], 0.5000);
}

CTEST(HCContourCurve, IntersectionQuadraticQuadratic) {
    HCPoint p0 = HCPointMake(0.0, 50.0);
    HCPoint pc = HCPointMake(50.0, -100.0);
    HCPoint p1 = HCPointMake(100.0, 50.0);
    HCPoint q0 = HCPointMake(0.0, -50.0);
    HCPoint qc = HCPointMake(50.0, 100.0);
    HCPoint q1 = HCPointMake(100.0, -50.0);
    HCContourCurve cp = HCContourCurveMakeQuadratic(pc, p1);
    HCContourCurve cq = HCContourCurveMakeQuadratic(qc, q1);
    HCInteger count = 0;
    HCReal t[2] = {NAN, NAN};
    HCReal u[2] = {NAN, NAN};
    HCContourCurveIntersection(p0, cp, q0, cq, &count, t, u);
    ASSERT_TRUE(count == 2);
    ASSERT_DBL_NEAR(t[0], 0.2104);
    ASSERT_DBL_NEAR(t[1], 0.7886);
    ASSERT_DBL_NEAR(u[0], 0.2114);
    ASSERT_DBL_NEAR(u[1], 0.7886);
}

CTEST(HCContourCurve, IntersectionQuadraticCubic) {
    HCPoint p0 = HCPointMake(0.0, 50.0);
    HCPoint pc = HCPointMake(100.0, -100.0);
    HCPoint p1 = HCPointMake(150.0, 50.0);
    HCPoint q0 = HCPointMake(0.0, -50.0);
    HCPoint qc0 = HCPointMake(50.0, 100.0);
    HCPoint qc1 = HCPointMake(100.0, -100.0);
    HCPoint q1 = HCPointMake(150.0, -50.0);
    HCContourCurve cp = HCContourCurveMakeQuadratic(pc, p1);
    HCContourCurve cq = HCContourCurveMakeCubic(qc0, qc1, q1);
    HCInteger count = 0;
    HCReal t[6] = {NAN, NAN};
    HCReal u[6] = {NAN, NAN};
    HCContourCurveIntersection(p0, cp, q0, cq, &count, t, u);
    ASSERT_TRUE(count == 2);
    ASSERT_DBL_NEAR(t[0], 0.1821);
    ASSERT_DBL_NEAR(t[1], 0.4946);
    ASSERT_DBL_NEAR(u[0], 0.2310);
    ASSERT_DBL_NEAR(u[1], 0.5786);
}

CTEST(HCContourCurve, IntersectionCubicCubic) {
    HCPoint pp0 = HCPointMake(0.0, -50.0);
    HCPoint pc0 = HCPointMake(50.0, 100.0);
    HCPoint pc1 = HCPointMake(100.0, -100.0);
    HCPoint pp1 = HCPointMake(150.0, 50.0);
    HCPoint qp0 = HCPointMake(0.0, 50.0);
    HCPoint qc0 = HCPointMake(50.0, -100.0);
    HCPoint qc1 = HCPointMake(100.0, 100.0);
    HCPoint qp1 = HCPointMake(150.0, -50.0);
    HCContourCurve cp = HCContourCurveMakeCubic(pc0, pc1, pp1);
    HCContourCurve cq = HCContourCurveMakeCubic(qc0, qc1, qp1);
    HCInteger count = 0;
    HCReal t[9] = {NAN, NAN};
    HCReal u[9] = {NAN, NAN};
    HCContourCurveIntersection(pp0, cp, qp0, cq, &count, t, u);
    ASSERT_TRUE(count == 3);
    ASSERT_DBL_NEAR(t[0], 0.1724);
    ASSERT_DBL_NEAR(t[1], 0.4995);
    ASSERT_DBL_NEAR(t[2], 0.8267);
    ASSERT_DBL_NEAR(u[0], 0.1724);
    ASSERT_DBL_NEAR(u[1], 0.4995);
    ASSERT_DBL_NEAR(u[2], 0.8276);
}
