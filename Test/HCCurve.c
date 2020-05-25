///
/// @file HCCurve.c
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
CTEST(HCCurve, Creation) {
    HCCurve linear = HCCurveMakeLinear(HCPointMake(-1.0, 2.0), HCPointMake(3.0, -4.0));
    ASSERT_TRUE(linear.p0.x == -1.0);
    ASSERT_TRUE(linear.p0.y == 2.0);
    ASSERT_TRUE(linear.p1.x == 3.0);
    ASSERT_TRUE(linear.p1.y == -4.0);
    
    HCCurve quadratic = HCCurveMakeQuadratic(HCPointMake(-1.0, 2.0), HCPointMake(3.0, -4.0), HCPointMake(-5.0, 6.0));
    ASSERT_TRUE(quadratic.p0.x == -1.0);
    ASSERT_TRUE(quadratic.p0.y == 2.0);
    ASSERT_TRUE(quadratic.c0.x == 3.0);
    ASSERT_TRUE(quadratic.c0.y == -4.0);
    ASSERT_TRUE(quadratic.p1.x == -5.0);
    ASSERT_TRUE(quadratic.p1.y == 6.0);

    HCCurve cubic = HCCurveMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(3.0, -4.0), HCPointMake(-5.0, 6.0), HCPointMake(7.0, -8.0));
    ASSERT_TRUE(cubic.p0.x == -1.0);
    ASSERT_TRUE(cubic.p0.y == 2.0);
    ASSERT_TRUE(cubic.c0.x == 3.0);
    ASSERT_TRUE(cubic.c0.y == -4.0);
    ASSERT_TRUE(cubic.c1.x == -5.0);
    ASSERT_TRUE(cubic.c1.y == 6.0);
    ASSERT_TRUE(cubic.p1.x == 7.0);
    ASSERT_TRUE(cubic.p1.y == -8.0);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCCurve, Equality) {
    ASSERT_TRUE(HCCurveIsSimilar(HCCurveMakeLinear(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0)), HCCurveMakeLinear(HCPointMake(1.5, 2.5), HCPointMake(3.5, 4.5)), 0.6));
    ASSERT_FALSE(HCCurveIsSimilar(HCCurveMakeLinear(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0)), HCCurveMakeLinear(HCPointMake(1.5, 2.5), HCPointMake(3.5, 4.5)), 0.4));
    ASSERT_FALSE(HCCurveIsEqual(HCCurveMakeLinear(HCPointMake(1.0, -2.5), HCPointMake(3.0, 4.0)), HCCurveMakeLinear(HCPointMake(1.0, 2.5), HCPointMake(3.5, 4.5))));
    ASSERT_TRUE(HCCurveIsEqual(HCCurveMakeLinear(HCPointMake(1.0, -2.5), HCPointMake(3.0, 4.0)), HCCurveMakeLinear(HCPointMake(1.0, -2.5), HCPointMake(3.0, 4.0))));
    ASSERT_EQUAL(HCCurveHashValue(HCCurveMakeLinear(HCPointMake(1.0, -2.5), HCPointMake(3.0, 4.0))), HCCurveHashValue(HCCurveMakeLinear(HCPointMake(1.0, -2.5), HCPointMake(3.0, 4.0))));
}

CTEST(HCCurve, Print) {
    HCCurve e = HCCurveMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(3.0, -4.0), HCPointMake(-5.0, 6.0), HCPointMake(7.0, -8.0));
    HCCurvePrint(e, stdout); // TODO: Not to stdout
}

CTEST(HCCurve, Queries) {
    HCCurve invalid = HCCurveInvalid;
    ASSERT_TRUE(HCCurveIsInvalid(invalid));
    ASSERT_FALSE(HCCurveIsZero(invalid));
    ASSERT_FALSE(HCCurveIsInfinite(invalid));
    
    HCCurve zero = HCCurveZero;
    ASSERT_FALSE(HCCurveIsInvalid(zero));
    ASSERT_TRUE(HCCurveIsZero(zero));
    ASSERT_FALSE(HCCurveIsInfinite(zero));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Order
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCCurve, Order) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint c0 = HCPointMake(3.0, -4.0);
    HCPoint c1 = HCPointMake(-5.0, 6.0);
    HCPoint p1 = HCPointMake(7.0, -8.0);
    
    HCCurve linear = HCCurveMakeLinear(p0, p1);
    ASSERT_FALSE(HCCurveIsInvalid(linear));
    ASSERT_FALSE(HCCurveIsZero(linear));
    ASSERT_FALSE(HCCurveIsInfinite(linear));
    ASSERT_TRUE(HCCurveIsLinear(linear));
    ASSERT_FALSE(HCCurveIsQuadratic(linear));
    ASSERT_FALSE(HCCurveIsCubic(linear));
    ASSERT_TRUE(HCCurveIsSimilar(HCCurveAsLinear(linear), linear, 0.000001));
    ASSERT_TRUE(HCCurveIsSimilar(HCCurveAsQuadratic(linear), HCCurveMakeQuadratic(p0, HCPointInterpolate(p0, p1, 0.5), p1), 0.000001));
    ASSERT_TRUE(HCCurveIsSimilar(HCCurveAsCubic(linear), HCCurveMakeCubic(p0, p0, p1, p1), 0.000001));
    
    HCCurve quadratic = HCCurveMakeQuadratic(p0, c0, p1);
    ASSERT_FALSE(HCCurveIsInvalid(quadratic));
    ASSERT_FALSE(HCCurveIsZero(quadratic));
    ASSERT_FALSE(HCCurveIsInfinite(quadratic));
    ASSERT_FALSE(HCCurveIsLinear(quadratic));
    ASSERT_TRUE(HCCurveIsQuadratic(quadratic));
    ASSERT_FALSE(HCCurveIsCubic(quadratic));
    ASSERT_TRUE(HCCurveIsSimilar(HCCurveAsLinear(quadratic), HCCurveMakeLinear(p0, p1), 0.000001));
    ASSERT_TRUE(HCCurveIsSimilar(HCCurveAsQuadratic(quadratic), quadratic, 0.000001));
    HCPoint qc0 = HCPointMake(p0.x + (2.0/3.0) * (c0.x - p0.x), p0.y + (2.0/3.0) * (c0.y - p0.y));
    HCPoint qc1 = HCPointMake(p1.x + (2.0/3.0) * (c0.x - p1.x), p1.y + (2.0/3.0) * (c0.y - p1.y));
    ASSERT_TRUE(HCCurveIsSimilar(HCCurveAsCubic(quadratic), HCCurveMakeCubic(p0, qc0, qc1, p1), 0.000001));
    
    HCCurve cubic = HCCurveMakeCubic(p0, c0, c1, p1);
    ASSERT_FALSE(HCCurveIsInvalid(cubic));
    ASSERT_FALSE(HCCurveIsZero(cubic));
    ASSERT_FALSE(HCCurveIsInfinite(cubic));
    ASSERT_FALSE(HCCurveIsLinear(cubic));
    ASSERT_FALSE(HCCurveIsQuadratic(cubic));
    ASSERT_TRUE(HCCurveIsCubic(cubic));
    ASSERT_TRUE(HCCurveIsSimilar(HCCurveAsLinear(cubic), HCCurveMakeLinear(p0, p1), 0.000001));
    ASSERT_TRUE(HCCurveIsSimilar(HCCurveAsQuadratic(cubic), HCCurveMakeQuadratic(p0, HCPointInterpolate(c0, c1, 0.5), p1), 0.000001));
    ASSERT_TRUE(HCCurveIsSimilar(HCCurveAsCubic(cubic), cubic, 0.000001));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Canonical Type
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCCurve, Canonical) {
    HCCurve invalid = HCCurveInvalid;
    ASSERT_TRUE(HCCurveCanonicalType(invalid) == HCCurveTypeInvalid);
    
    HCCurve point = HCCurveMakeCubic(HCPointMake(1.0, 1.0), HCPointMake(1.0, 1.0), HCPointMake(1.0, 1.0), HCPointMake(1.0, 1.0));
    ASSERT_TRUE(HCCurveCanonicalType(point) == HCCurveTypePoint);
    
    HCCurve linear = HCCurveMakeLinear(HCPointMake(1.0, 1.0), HCPointMake(2.0, 2.0));
    ASSERT_TRUE(HCCurveCanonicalType(linear) == HCCurveTypeLinear);
    
    HCCurve quadratic = HCCurveMakeQuadratic(HCPointMake(1.0, 1.0), HCPointMake(2.0, 2.0), HCPointMake(3.0, 1.0));
    ASSERT_TRUE(HCCurveCanonicalType(quadratic) == HCCurveTypeQuadratic);
    
    HCCurve simple = HCCurveMakeCubic(HCPointMake(0.0, 0.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCCurveCanonicalType(simple) == HCCurveTypeCubicSimple);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(simple.p0, simple.c0, simple.c1, simple.p1), HCPointMake(1.0, 0.0), 0.0000001));
    
    HCCurve singleInflection = HCCurveMakeCubic(HCPointMake(0.0, 0.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(50.0, 200.0));
    ASSERT_TRUE(HCCurveCanonicalType(singleInflection) == HCCurveTypeCubicSingleInflection);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(singleInflection.p0, singleInflection.c0, singleInflection.c1, singleInflection.p1), HCPointMake(0.5, 2.0), 0.0000001));
    
    HCCurve doubleInflection = HCCurveMakeCubic(HCPointMake(0.0, 0.0), HCPointMake(125.0, 75.0), HCPointMake(100.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCCurveCanonicalType(doubleInflection) == HCCurveTypeCubicDoubleInflection);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(doubleInflection.p0, doubleInflection.c0, doubleInflection.c1, doubleInflection.p1), HCPointMake(-1.5, 0.5), 0.0000001));
    
    HCCurve loop = HCCurveMakeCubic(HCPointMake(25.0, 0.0), HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCCurveCanonicalType(loop) == HCCurveTypeCubicLoop);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(loop.p0, loop.c0, loop.c1, loop.p1), HCPointMake(-0.75, 0.0), 0.0000001));
    
    HCCurve loopAtStart = HCCurveMakeCubic(HCPointMake(0.0, 0.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(-300.0, -600.0));
    ASSERT_TRUE(HCCurveCanonicalType(loopAtStart) == HCCurveTypeCubicLoopAtStart);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(loopAtStart.p0, loopAtStart.c0, loopAtStart.c1, loopAtStart.p1), HCPointMake(-3.0, -6.0), 0.0000001));

    HCCurve loopAtEnd = HCCurveMakeCubic(HCPointMake(0.0, 0.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(50.0, ((sqrt(5.25) - 0.5) * 0.5) * 100.0));
    ASSERT_TRUE(HCCurveCanonicalType(loopAtEnd) == HCCurveTypeCubicLoopAtEnd);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(loopAtEnd.p0, loopAtEnd.c0, loopAtEnd.c1, loopAtEnd.p1), HCPointMake(0.5, ((sqrt(5.25) - 0.5) * 0.5)), 0.0000001));
    
    HCCurve loopClosed = HCCurveMakeCubic(HCPointMake(50.0, 0.0), HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(50.0, 0.0));
    ASSERT_TRUE(HCCurveCanonicalType(loopClosed) == HCCurveTypeCubicLoopClosed);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(loopClosed.p0, loopClosed.c0, loopClosed.c1, loopClosed.p1), HCPointMake(0.0, 0.0), 0.0000001));
    
    HCCurve cusp = HCCurveMakeCubic(HCPointMake(0.0, 0.0), HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCCurveCanonicalType(cusp) == HCCurveTypeCubicCusp);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(cusp.p0, cusp.c0, cusp.c1, cusp.p1), HCPointMake(-1.0, 0.0), 0.0000001));
    
    HCPoint coanchorP0 = HCPointMake(1.0, 1.0);
    HCPoint coanchorP1 = HCPointMake(4.0, 1.0);
    HCCurve coanchor10 = HCCurveMakeCubic(coanchorP0, coanchorP0, HCPointMake(3.0, 0.0), coanchorP1);
    ASSERT_TRUE(HCCurveCanonicalType(coanchor10) == HCCurveTypeCubicSimple);
    HCCurve coanchor01 = HCCurveMakeCubic(coanchorP0, HCPointMake(2.0, 0.0), coanchorP1, coanchorP1);
    ASSERT_TRUE(HCCurveCanonicalType(coanchor01) == HCCurveTypeCubicSimple);
    
    HCCurve colinear = HCCurveMakeCubic(HCPointMake(1.0, 1.0), HCPointMake(2.0, 2.0), HCPointMake(3.0, 3.0), HCPointMake(4.0, 4.0));
    ASSERT_TRUE(HCCurveCanonicalType(colinear) == HCCurveTypeLinear);
    
    HCCurve colinearQuadratic = HCCurveMakeQuadratic(HCPointMake(1.0, 1.0), HCPointMake(2.0, 2.0), HCPointMake(3.0, 3.0));
    ASSERT_TRUE(HCCurveCanonicalType(colinearQuadratic) == HCCurveTypeLinear);
    
    HCPoint cqp0 = HCPointMake(20.0, 30.0);
    HCPoint  cqc = HCPointMake(25.0, 60.0);
    HCPoint cqp1 = HCPointMake(30.0, 50.0);
    HCPoint cqc0 = HCPointMake(cqp0.x + (2.0/3.0) * (cqc.x - cqp0.x), cqp0.y + (2.0/3.0) * (cqc.y - cqp0.y));
    HCPoint cqc1 = HCPointMake(cqp1.x + (2.0/3.0) * (cqc.x - cqp1.x), cqp1.y + (2.0/3.0) * (cqc.y - cqp1.y));
    HCCurve coquadratic = HCCurveMakeCubic(cqp0, cqc0, cqc1, cqp1);
    ASSERT_TRUE(HCCurveCanonicalType(coquadratic) == HCCurveTypeQuadratic);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Tests

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Evaluation
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCCurve, LinearEvaluation) {
    HCPoint s = HCPointInvalid;
    HCCurveEvaluateLinear(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), 0.5, &s.x, &s.y);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(2.0, 3.0), 0.000001));
}

CTEST(HCCurve, QuadraticEvaluation) {
    HCPoint s = HCPointInvalid;
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCCurveEvaluateQuadratic(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 2.0), 0.5, &s.x, &s.y, &dx, &dy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 3.0), 0.000001));
    ASSERT_DBL_NEAR(dx, 2.0);
    ASSERT_DBL_NEAR(dy, 0.0);
}

CTEST(HCCurve, CubicEvaluation) {
    HCPoint s = HCPointInvalid;
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCReal ddx = NAN;
    HCReal ddy = NAN;
    HCCurveEvaluateCubic(HCPointMake(1.0, 2.0), HCPointMake(2.0, 4.0), HCPointMake(4.0, 4.0), HCPointMake(5.0, 2.0), 0.5, &s.x, &s.y, &dx, &dy, &ddx, &ddy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 3.5), 0.000001));
    ASSERT_DBL_NEAR(dx, 1.5);
    ASSERT_DBL_NEAR(dy, 0.0);
    ASSERT_DBL_NEAR(ddx, 3.0);
    ASSERT_DBL_NEAR(ddy, 0.0);
}

CTEST(HCCurve, Evaluation) {
    HCPoint s = HCPointInvalid;
    HCReal dx = NAN;
    HCReal dy = NAN;
    HCReal ddx = NAN;
    HCReal ddy = NAN;
    HCCurve curve = HCCurveMakeCubic(HCPointMake(1.0, 2.0), HCPointMake(2.0, 4.0), HCPointMake(4.0, 4.0), HCPointMake(5.0, 2.0));
    HCCurveEvaluate(curve, 0.5, &s.x, &s.y, &dx, &dy, &ddx, &ddy);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 3.5), 0.000001));
    ASSERT_DBL_NEAR(dx, 1.5);
    ASSERT_DBL_NEAR(dy, 0.0);
    ASSERT_DBL_NEAR(ddx, 3.0);
    ASSERT_DBL_NEAR(ddy, 0.0);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Axis Alignment
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCCurve, AxisAligned) {
    HCCurve curve = HCCurveMakeCubic(HCPointMake(1.0, 2.0), HCPointMake(3.0, 0.0), HCPointMake(5.0, 1.0), HCPointMake(7.0, 8.0));
    HCCurve xAxisAligned = HCCurveXAxisAligned(curve);
    ASSERT_FALSE(HCCurveIsInvalid(xAxisAligned));
    ASSERT_FALSE(HCCurveIsZero(xAxisAligned));
    ASSERT_FALSE(HCCurveIsInfinite(xAxisAligned));
    ASSERT_FALSE(HCCurveIsLinear(xAxisAligned));
    ASSERT_FALSE(HCCurveIsQuadratic(xAxisAligned));
    ASSERT_TRUE(HCCurveIsCubic(xAxisAligned));
    ASSERT_DBL_NEAR(xAxisAligned.p1.y, 0.0);
    
    HCCurve yAxisAligned = HCCurveYAxisAligned(curve);
    ASSERT_FALSE(HCCurveIsInvalid(yAxisAligned));
    ASSERT_FALSE(HCCurveIsZero(yAxisAligned));
    ASSERT_FALSE(HCCurveIsInfinite(yAxisAligned));
    ASSERT_FALSE(HCCurveIsLinear(yAxisAligned));
    ASSERT_FALSE(HCCurveIsQuadratic(yAxisAligned));
    ASSERT_TRUE(HCCurveIsCubic(yAxisAligned));
    ASSERT_DBL_NEAR(yAxisAligned.p1.x, 0.0);
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

CTEST(HCCurve, LinearExtrema) {
    HCInteger count = 0;
    HCCurveExtremaLinear(HCPointMake(-1.0, 2.0), HCPointMake(3.0, -4.0), &count, NULL);
    ASSERT_TRUE(count == 0);
}

CTEST(HCCurve, QuadraticExtrema) {
    HCInteger count = 0;
    HCReal extrema[2];
    HCCurveExtremaQuadratic(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 2.0), &count, extrema);
    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(extrema[0] == 0.5);
}

CTEST(HCCurve, CubicExtrema) {
    HCInteger count = 0;
    HCReal extrema[6];
    HCCurveExtremaCubic(HCPointMake(1.0, 2.0), HCPointMake(2.0, 4.0), HCPointMake(4.0, 4.0), HCPointMake(5.0, 2.0), &count, extrema);
    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(extrema[0] == 0.5);
}

CTEST(HCCurve, Extrema) {
    HCCurve curve = HCCurveMakeCubic(HCPointMake(1.0, 2.0), HCPointMake(2.0, 4.0), HCPointMake(4.0, 4.0), HCPointMake(5.0, 2.0));
    HCInteger count = 0;
    HCReal extrema[6];
    HCCurveExtrema(curve, &count, extrema);
    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(extrema[0] == 0.5);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Inflection
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearInflections) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCInteger count = 0;
    HCCurveInflectionsLinear(p0, p1, &count, NULL);
    ASSERT_TRUE(count == 0);
}

CTEST(HCCurve, QuadraticInflection) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCInteger count = 0;
    HCCurveInflectionsQuadratic(p0, c, p1, &count, NULL);
    ASSERT_TRUE(count == 0);
}

CTEST(HCCurve, CubicInflection) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint c0 = HCPointMake(0.0, 2.0);
    HCPoint c1 = HCPointMake(2.0, 2.0);
    HCPoint p1 = HCPointMake(1.0, 4.0);
    HCInteger count = 0;
    HCReal inflections[2];
    HCCurveInflectionsCubic(p0, c0, c1, p1, &count, inflections);
    ASSERT_TRUE(HCCurveCanonicalTypeCubic(p0, c0, c1, p1) == HCCurveTypeCubicSingleInflection);
    ASSERT_TRUE(count == 1);
    ASSERT_DBL_NEAR(inflections[0], 0.4384);
}

CTEST(HCCurve, Inflection) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint c0 = HCPointMake(0.0, 2.0);
    HCPoint c1 = HCPointMake(2.0, 2.0);
    HCPoint p1 = HCPointMake(1.0, 4.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCInteger count = 0;
    HCReal inflections[2];
    HCCurveInflections(curve, &count, inflections);
    ASSERT_TRUE(HCCurveCanonicalType(curve) == HCCurveTypeCubicSingleInflection);
    ASSERT_TRUE(count == 1);
    ASSERT_DBL_NEAR(inflections[0], 0.4384);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Bounds
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearApproximateBounds) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCRectangle bounds = HCCurveApproximateBoundsLinear(p0, p1);
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(
        fmin(p0.x, p1.x),
        fmin(p0.y, p1.y),
        fmax(p0.x, p1.x),
        fmax(p0.y, p1.y))));
}

CTEST(HCCurve, QuadraticApproximateBounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCRectangle bounds = HCCurveApproximateBoundsQuadratic(p0, c, p1);
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(
        fmin(p0.x, fmin(c.x, p1.x)),
        fmin(p0.y, fmin(c.y, p1.y)),
        fmax(p0.x, fmax(c.x, p1.x)),
        fmax(p0.y, fmax(c.y, p1.y)))));
}

CTEST(HCCurve, CubicApproximateBounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCRectangle bounds = HCCurveApproximateBoundsCubic(p0, c0, c1, p1);
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(
        fmin(p0.x, fmin(c0.x, fmin(c1.x, p1.x))),
        fmin(p0.y, fmin(c0.y, fmin(c1.y, p1.y))),
        fmax(p0.x, fmax(c0.x, fmax(c1.x, p1.x))),
        fmax(p0.y, fmax(c0.y, fmax(c1.y, p1.y))))));
}

CTEST(HCCurve, ApproximateBounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCRectangle bounds = HCCurveApproximateBounds(curve);
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(
        fmin(p0.x, fmin(c0.x, fmin(c1.x, p1.x))),
        fmin(p0.y, fmin(c0.y, fmin(c1.y, p1.y))),
        fmax(p0.x, fmax(c0.x, fmax(c1.x, p1.x))),
        fmax(p0.y, fmax(c0.y, fmax(c1.y, p1.y))))));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Bounds
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearBounds) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCRectangle bounds = HCCurveBoundsLinear(p0, p1);
    ASSERT_TRUE(HCRectangleContainsRectangle(HCCurveApproximateBoundsLinear(p0, p1), bounds));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(fmin(p0.x, p1.x), fmin(p0.y, p1.y), fmax(p0.x, p1.x), fmax(p0.y, p1.y))));
}

CTEST(HCCurve, QuadraticBounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCRectangle bounds = HCCurveBoundsQuadratic(p0, c, p1);
    ASSERT_TRUE(HCRectangleContainsRectangle(HCCurveApproximateBoundsQuadratic(p0, c, p1), bounds));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(1.0, 2.0, 5.0, 3.0)));
}

CTEST(HCCurve, CubicBounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCRectangle bounds = HCCurveBoundsCubic(p0, c0, c1, p1);
    ASSERT_TRUE(HCRectangleContainsRectangle(HCCurveApproximateBoundsCubic(p0, c0, c1, p1), bounds));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(1.0, 2.0, 5.0, 3.5)));
}

CTEST(HCCurve, Bounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCRectangle bounds = HCCurveBounds(curve);
    ASSERT_TRUE(HCRectangleContainsRectangle(HCCurveApproximateBounds(curve), bounds));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(1.0, 2.0, 5.0, 3.5)));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Length
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearLength) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCReal length = HCCurveLengthLinear(p0, p1);
    ASSERT_DBL_NEAR(length, HCPointDistance(p0, p1));
}

CTEST(HCCurve, QuadraticLength) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCReal length = HCCurveLengthQuadratic(p0, c, p1);
    ASSERT_TRUE(length > HCPointDistance(p0, p1) && length < HCPointDistance(p0, c) + HCPointDistance(c, p1));
}

CTEST(HCCurve, CubicLength) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCReal length = HCCurveLengthCubic(p0, c0, c1, p1);
    ASSERT_TRUE(length > HCPointDistance(p0, p1) && length < HCPointDistance(p0, c0) + HCPointDistance(c0, c1) + HCPointDistance(c1, p1));
}

CTEST(HCCurve, Length) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCReal length = HCCurveLength(curve);
    ASSERT_TRUE(length > HCPointDistance(p0, p1) && length < HCPointDistance(p0, c0) + HCPointDistance(c0, c1) + HCPointDistance(c1, p1));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameterization by Arc Length
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearParameterizedByArcLength) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCReal length = HCCurveLengthLinear(p0, p1);
    for (HCReal d = 0.0; d < length; d += 0.1) {
        HCReal t = HCCurveParameterLinear(p0, p1, d);
        ASSERT_DBL_NEAR(t, length / d);
    }
}

CTEST(HCCurve, QuadraticParameterizedByArcLength) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCReal length = HCCurveLengthQuadratic(p0, c, p1);
    HCReal previousT = 0.0;
    for (HCReal d = 0.0; d < length; d += 0.1) {
        HCReal t = HCCurveParameterQuadratic(p0, c, p1, d);
        ASSERT_TRUE(t >= 0.0);
        ASSERT_TRUE(t <= 1.0);
        ASSERT_TRUE(t >= previousT);
        previousT = t;
    }
}

CTEST(HCCurve, CubicParameterizedByArcLength) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCReal length = HCCurveLengthCubic(p0, c0, c1, p1);
    HCReal previousT = 0.0;
    for (HCReal d = 0.0; d < length; d += 0.1) {
        HCReal t = HCCurveParameterCubic(p0, c0, c1, p1, d);
        ASSERT_TRUE(t >= 0.0);
        ASSERT_TRUE(t <= 1.0);
        ASSERT_TRUE(t >= previousT);
        previousT = t;
    }
}

CTEST(HCCurve, ParameterizedByArcLength) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCReal length = HCCurveLength(curve);
    HCReal previousT = 0.0;
    for (HCReal d = 0.0; d < length; d += 0.1) {
        HCReal t = HCCurveParameter(curve, d);
        ASSERT_TRUE(t >= 0.0);
        ASSERT_TRUE(t <= 1.0);
        ASSERT_TRUE(t >= previousT);
        previousT = t;
    }
}

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

CTEST(HCCurve, IntersectionLinearLinear) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint p1 = HCPointMake(100.0, 100.0);
    HCPoint q0 = HCPointMake(100.0, 0.0);
    HCPoint q1 = HCPointMake(0.0, 100.0);
    HCInteger count = 0;
    HCReal t = NAN;
    HCReal u = NAN;
    HCCurveIntersectionLinearLinear(p0, p1, q0, q1, &count, &t, &u);
    ASSERT_TRUE(count == 1);
    ASSERT_DBL_NEAR(t, 0.5);
    ASSERT_DBL_NEAR(u, 0.5);
}

CTEST(HCCurve, IntersectionLinearQuadratic) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint p1 = HCPointMake(100.0, 0.0);
    HCPoint q0 = HCPointMake(0.0, -50.0);
    HCPoint qc = HCPointMake(50.0, 100.0);
    HCPoint q1 = HCPointMake(100.0, -50.0);
    HCInteger count = 0;
    HCReal t[2] = {NAN, NAN};
    HCReal u[2] = {NAN, NAN};
    HCCurveIntersectionLinearQuadratic(p0, p1, q0, qc, q1, &count, t, u);
    ASSERT_TRUE(count == 2);
    ASSERT_DBL_NEAR(t[0], 0.2113);
    ASSERT_DBL_NEAR(t[1], 0.7887);
    ASSERT_DBL_NEAR(u[0], 0.2113);
    ASSERT_DBL_NEAR(u[1], 0.7887);
}

CTEST(HCCurve, IntersectionLinearCubic) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint p1 = HCPointMake(150.0, 0.0);
    HCPoint q0 = HCPointMake(0.0, 50.0);
    HCPoint qc0 = HCPointMake(50.0, -100.0);
    HCPoint qc1 = HCPointMake(100.0, 100.0);
    HCPoint q1 = HCPointMake(150.0, -50.0);
    HCInteger count = 0;
    HCReal t[3] = {NAN, NAN};
    HCReal u[3] = {NAN, NAN};
    HCCurveIntersectionLinearCubic(p0, p1, q0, qc0, qc1, q1, &count, t, u);
    ASSERT_TRUE(count == 3);
    ASSERT_DBL_NEAR(t[0], 0.8273);
    ASSERT_DBL_NEAR(t[1], 0.1726);
    ASSERT_DBL_NEAR(t[2], 0.5000);
    ASSERT_DBL_NEAR(u[0], 0.8273);
    ASSERT_DBL_NEAR(u[1], 0.1726);
    ASSERT_DBL_NEAR(u[2], 0.5000);
}

CTEST(HCCurve, IntersectionQuadraticQuadratic) {
    HCPoint p0 = HCPointMake(0.0, 50.0);
    HCPoint pc = HCPointMake(50.0, -100.0);
    HCPoint p1 = HCPointMake(100.0, 50.0);
    HCPoint q0 = HCPointMake(0.0, -50.0);
    HCPoint qc = HCPointMake(50.0, 100.0);
    HCPoint q1 = HCPointMake(100.0, -50.0);
    HCInteger count = 0;
    HCReal t[2] = {NAN, NAN};
    HCReal u[2] = {NAN, NAN};
    HCCurveIntersectionQuadraticQuadratic(p0, pc, p1, q0, qc, q1, &count, t, u);
    ASSERT_TRUE(count == 2);
    ASSERT_DBL_NEAR(t[0], 0.2104);
    ASSERT_DBL_NEAR(t[1], 0.7886);
    ASSERT_DBL_NEAR(u[0], 0.2114);
    ASSERT_DBL_NEAR(u[1], 0.7886);
}

CTEST(HCCurve, IntersectionQuadraticCubic) {
    HCPoint p0 = HCPointMake(0.0, 50.0);
    HCPoint pc = HCPointMake(100.0, -100.0);
    HCPoint p1 = HCPointMake(150.0, 50.0);
    HCPoint q0 = HCPointMake(0.0, -50.0);
    HCPoint qc0 = HCPointMake(50.0, 100.0);
    HCPoint qc1 = HCPointMake(100.0, -100.0);
    HCPoint q1 = HCPointMake(150.0, -50.0);
    HCInteger count = 0;
    HCReal t[6] = {NAN, NAN};
    HCReal u[6] = {NAN, NAN};
    HCCurveIntersectionQuadraticCubic(p0, pc, p1, q0, qc0, qc1, q1, &count, t, u);
    ASSERT_TRUE(count == 2);
    ASSERT_DBL_NEAR(t[0], 0.1821);
    ASSERT_DBL_NEAR(t[1], 0.4946);
    ASSERT_DBL_NEAR(u[0], 0.2310);
    ASSERT_DBL_NEAR(u[1], 0.5786);
}

CTEST(HCCurve, IntersectionCubicCubic) {
    HCPoint p0 = HCPointMake(0.0, -50.0);
    HCPoint c0 = HCPointMake(50.0, 100.0);
    HCPoint c1 = HCPointMake(100.0, -100.0);
    HCPoint p1 = HCPointMake(150.0, 50.0);
    HCPoint q0 = HCPointMake(0.0, 50.0);
    HCPoint qc0 = HCPointMake(50.0, -100.0);
    HCPoint qc1 = HCPointMake(100.0, 100.0);
    HCPoint q1 = HCPointMake(150.0, -50.0);
    HCInteger count = 0;
    HCReal t[9] = {NAN, NAN};
    HCReal u[9] = {NAN, NAN};
    HCCurveIntersectionCubicCubic(p0, c0, c1, p1, q0, qc0, qc1, q1, &count, t, u);
    ASSERT_TRUE(count == 3);
    ASSERT_DBL_NEAR(t[0], 0.1724);
    ASSERT_DBL_NEAR(t[1], 0.4995);
    ASSERT_DBL_NEAR(t[2], 0.8267);
    ASSERT_DBL_NEAR(u[0], 0.1724);
    ASSERT_DBL_NEAR(u[1], 0.4995);
    ASSERT_DBL_NEAR(u[2], 0.8276);
}

CTEST(HCCurve, Intersection) {
    HCPoint pp0 = HCPointMake(0.0, -50.0);
    HCPoint pc0 = HCPointMake(50.0, 100.0);
    HCPoint pc1 = HCPointMake(100.0, -100.0);
    HCPoint pp1 = HCPointMake(150.0, 50.0);
    HCCurve curveP = HCCurveMakeCubic(pp0, pc0, pc1, pp1);
    HCPoint qp0 = HCPointMake(0.0, 50.0);
    HCPoint qc0 = HCPointMake(50.0, -100.0);
    HCPoint qc1 = HCPointMake(100.0, 100.0);
    HCPoint qp1 = HCPointMake(150.0, -50.0);
    HCCurve curveQ = HCCurveMakeCubic(qp0, qc0, qc1, qp1);
    HCInteger count = 0;
    HCReal t[9] = {NAN, NAN};
    HCReal u[9] = {NAN, NAN};
    HCCurveIntersection(curveP, curveQ, &count, t, u);
    ASSERT_TRUE(count == 3);
    ASSERT_DBL_NEAR(t[0], 0.1724);
    ASSERT_DBL_NEAR(t[1], 0.4995);
    ASSERT_DBL_NEAR(t[2], 0.8267);
    ASSERT_DBL_NEAR(u[0], 0.1724);
    ASSERT_DBL_NEAR(u[1], 0.4995);
    ASSERT_DBL_NEAR(u[2], 0.8276);
}

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
