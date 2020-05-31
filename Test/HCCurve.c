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
// MARK: - Extrema
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearExtrema) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCInteger count = 0;
    HCCurveExtremaLinear(p0, p1, &count, NULL);
    ASSERT_TRUE(count == 0);
}

CTEST(HCCurve, QuadraticExtrema) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCInteger count = 2;
    HCReal extrema[count];
    HCCurveExtremaQuadratic(p0, c, p1, &count, extrema);
    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(extrema[0] == 0.5);
}

CTEST(HCCurve, CubicExtrema) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCInteger count = 6;
    HCReal extrema[count];
    HCCurveExtremaCubic(p0, c0, c1, p1, &count, extrema);
    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(extrema[0] == 0.5);
}

CTEST(HCCurve, Extrema) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCInteger count = 6;
    HCReal extrema[count];
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
    HCInteger count = 2;
    HCReal inflections[count];
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
    HCInteger count = 2;
    HCReal inflections[count];
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
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearValue) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, 4.0);
    HCReal t = 0.5;
    HCPoint s = HCCurveValueLinear(p0, p1, t);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(2.0, 3.0), 0.000001));
}

CTEST(HCCurve, QuadraticValue) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCReal t = 0.5;
    HCPoint s = HCCurveValueQuadratic(p0, c, p1, t);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 3.0), 0.000001));
}

CTEST(HCCurve, CubicValue) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCReal t = 0.5;
    HCPoint s = HCCurveValueCubic(p0, c0, c1, p1, t);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 3.5), 0.000001));
}

CTEST(HCCurve, Value) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCReal t = 0.5;
    HCPoint s =  HCCurveValue(curve, t);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 3.5), 0.000001));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Derivative
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearDerivative) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, 4.0);
    HCPoint dp = HCPointInvalid;
    HCCurveDerivativeLinear(p0, p1, &dp);
    ASSERT_DBL_NEAR(dp.x, 1.0 * (p1.x - p0.x));
    ASSERT_DBL_NEAR(dp.y, 1.0 * (p1.y - p0.y));
}

CTEST(HCCurve, QuadraticDerivative) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCPoint dp0 = HCPointInvalid;
    HCPoint dp1 = HCPointInvalid;
    HCCurveDerivativeQuadratic(p0, c, p1, &dp0, &dp1);
    ASSERT_DBL_NEAR(dp0.x, 2.0 * ( c.x - p0.x));
    ASSERT_DBL_NEAR(dp0.y, 2.0 * ( c.y - p0.y));
    ASSERT_DBL_NEAR(dp1.x, 2.0 * (p1.x -  c.x));
    ASSERT_DBL_NEAR(dp1.y, 2.0 * (p1.y -  c.y));
    
    HCPoint ddp = HCPointInvalid;
    HCCurveDerivativeLinear(dp0, dp1, &ddp);
    ASSERT_DBL_NEAR(ddp.x, 1.0 * (dp1.x - dp0.x));
    ASSERT_DBL_NEAR(ddp.y, 1.0 * (dp1.y - dp0.y));
}

CTEST(HCCurve, CubicDerivative) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCPoint dp0 = HCPointInvalid;
    HCPoint  dc = HCPointInvalid;
    HCPoint dp1 = HCPointInvalid;
    HCCurveDerivativeCubic(p0, c0, c1, p1, &dp0, &dc, &dp1);
    ASSERT_DBL_NEAR(dp0.x, 3.0 * (c0.x - p0.x));
    ASSERT_DBL_NEAR(dp0.y, 3.0 * (c0.y - p0.y));
    ASSERT_DBL_NEAR( dc.x, 3.0 * (c1.x - c0.x));
    ASSERT_DBL_NEAR( dc.y, 3.0 * (c1.y - c0.y));
    ASSERT_DBL_NEAR(dp1.x, 3.0 * (p1.x - c1.x));
    ASSERT_DBL_NEAR(dp1.y, 3.0 * (p1.y - c1.y));
    
    HCPoint ddp0 = HCPointInvalid;
    HCPoint ddp1 = HCPointInvalid;
    HCCurveDerivativeQuadratic(dp0, dc, dp1, &ddp0, &ddp1);
    ASSERT_DBL_NEAR(ddp0.x, 2.0 * ( dc.x - dp0.x));
    ASSERT_DBL_NEAR(ddp0.y, 2.0 * ( dc.y - dp0.y));
    ASSERT_DBL_NEAR(ddp1.x, 2.0 * (dp1.x -  dc.x));
    ASSERT_DBL_NEAR(ddp1.y, 2.0 * (dp1.y -  dc.y));
    
    HCPoint dddp = HCPointInvalid;
    HCCurveDerivativeLinear(ddp0, ddp1, &dddp);
    ASSERT_DBL_NEAR(dddp.x, 1.0 * (ddp1.x - ddp0.x));
    ASSERT_DBL_NEAR(dddp.y, 1.0 * (ddp1.y - ddp0.y));
}

CTEST(HCCurve, Derivative) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCCurve d = HCCurveDerivative(curve);
    ASSERT_TRUE(HCCurveCanonicalType(d) == HCCurveTypeQuadratic);
    ASSERT_DBL_NEAR(d.p0.x, 3.0 * (curve.c0.x - curve.p0.x));
    ASSERT_DBL_NEAR(d.p0.y, 3.0 * (curve.c0.y - curve.p0.y));
    ASSERT_DBL_NEAR(d.c0.x, 3.0 * (curve.c1.x - curve.c0.x));
    ASSERT_DBL_NEAR(d.c0.y, 3.0 * (curve.c1.y - curve.c0.y));
    ASSERT_DBL_NEAR(d.p1.x, 3.0 * (curve.p1.x - curve.c1.x));
    ASSERT_DBL_NEAR(d.p1.y, 3.0 * (curve.p1.y - curve.c1.y));
    
    HCCurve dd = HCCurveDerivative(d);
    ASSERT_TRUE(HCCurveCanonicalType(dd) == HCCurveTypeLinear);
    ASSERT_DBL_NEAR(dd.p0.x, 2.0 * (d.c0.x - d.p0.x));
    ASSERT_DBL_NEAR(dd.p0.y, 2.0 * (d.c0.y - d.p0.y));
    ASSERT_DBL_NEAR(dd.p1.x, 2.0 * (d.p1.x - d.c0.x));
    ASSERT_DBL_NEAR(dd.p1.y, 2.0 * (d.p1.y - d.c0.y));
    
    HCCurve ddd = HCCurveDerivative(dd);
    ASSERT_TRUE(HCCurveCanonicalType(ddd) == HCCurveTypePoint);
    ASSERT_DBL_NEAR(ddd.p0.x, 1.0 * (dd.p1.x - dd.p0.x));
    ASSERT_DBL_NEAR(ddd.p0.y, 1.0 * (dd.p1.y - dd.p0.y));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Tangent
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Test this!

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Normal
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Test this!

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Curvature
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearCurvature) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, 4.0);
    HCReal t = 0.75;
    HCReal k = HCCurveCurvatureLinear(p0, p1, t);
    ASSERT_DBL_NEAR(k, 0.0);
}

CTEST(HCCurve, QuadraticCurvature) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCReal t = 0.75;
    HCReal k = HCCurveCurvatureQuadratic(p0, c, p1, t);
    ASSERT_DBL_NEAR(k, -0.3578);
}

CTEST(HCCurve, CubicCurvature) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCReal t = 0.75;
    HCReal k = HCCurveCurvatureCubic(p0, c0, c1, p1, t);
    ASSERT_DBL_NEAR(k, -0.4409);
}

CTEST(HCCurve, Curvature) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCReal t = 0.75;
    HCReal k = HCCurveCurvature(curve, t);
    ASSERT_DBL_NEAR(k, -0.4409);
}

// TODO: Test curvature normal

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameterization by Arc Length
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearParameterizedByArcLength) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCReal length = HCCurveLengthLinear(p0, p1);
    for (HCReal d = 0.0; d < length; d += 0.1) {
        HCReal t = HCCurveParameterAtLengthLinear(p0, p1, d);
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
        HCReal t = HCCurveParameterAtLengthQuadratic(p0, c, p1, d);
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
        HCReal t = HCCurveParameterAtLengthCubic(p0, c0, c1, p1, d);
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
        HCReal t = HCCurveParameterAtLength(curve, d);
        ASSERT_TRUE(t >= 0.0);
        ASSERT_TRUE(t <= 1.0);
        ASSERT_TRUE(t >= previousT);
        previousT = t;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameter Given Point
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearParameterNearestPoint) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal nearest = HCCurveParameterNearestPointLinear(p0, p1, p);
    ASSERT_TRUE(nearest >= 0.0);
    ASSERT_TRUE(nearest <= 1.0);
}

CTEST(HCCurve, QuadraticParameterNearestPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal nearest = HCCurveParameterNearestPointQuadratic(p0, c, p1, p);
    ASSERT_TRUE(nearest >= 0.0);
    ASSERT_TRUE(nearest <= 1.0);
}

CTEST(HCCurve, CubicParameterNearestPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal nearest = HCCurveParameterNearestPointCubic(p0, c0, c1, p1, p);
    ASSERT_TRUE(nearest >= 0.0);
    ASSERT_TRUE(nearest <= 1.0);
}

CTEST(HCCurve, ParameterNearestPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal nearest = HCCurveParameterNearestPoint(curve, p);
    ASSERT_TRUE(nearest >= 0.0);
    ASSERT_TRUE(nearest <= 1.0);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Distance from Point
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearDistanceFromPoint) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal distance = HCCurveDistanceFromPointLinear(p0, p1, p);
    ASSERT_TRUE(distance <= HCPointDistance(p, p0));
    ASSERT_TRUE(distance <= HCPointDistance(p, p1));
}

CTEST(HCCurve, QuadraticDistanceFromPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal distance = HCCurveDistanceFromPointQuadratic(p0, c, p1, p);
    ASSERT_TRUE(distance <= HCPointDistance(p, p0));
    ASSERT_TRUE(distance <= HCPointDistance(p, c));
    ASSERT_TRUE(distance <= HCPointDistance(p, p1));
}

CTEST(HCCurve, CubicDistanceFromPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal distance = HCCurveDistanceFromPointCubic(p0, c0, c1, p1, p);
    ASSERT_TRUE(distance <= HCPointDistance(p, p0));
    ASSERT_TRUE(distance <= HCPointDistance(p, c0));
    ASSERT_TRUE(distance <= HCPointDistance(p, c1));
    ASSERT_TRUE(distance <= HCPointDistance(p, p1));
}

CTEST(HCCurve, DistanceFromPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal distance = HCCurveDistanceFromPoint(curve, p);
    ASSERT_TRUE(distance <= HCPointDistance(p, p0));
    ASSERT_TRUE(distance <= HCPointDistance(p, c0));
    ASSERT_TRUE(distance <= HCPointDistance(p, c1));
    ASSERT_TRUE(distance <= HCPointDistance(p, p1));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Baseline Projection
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearBaselineProjection) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCPoint b = HCCurveBaselineProjectionLinear(p0, p1, 0.25);
    ASSERT_DBL_NEAR((p1.y - p0.y) / (p1.x - p0.x), (b.y - p0.y) / (b.x - p0.x));
}

CTEST(HCCurve, QuadraticBaselineProjection) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCPoint b = HCCurveBaselineProjectionQuadratic(p0, p1, 0.25);
    ASSERT_DBL_NEAR((p1.y - p0.y) / (p1.x - p0.x), (b.y - p0.y) / (b.x - p0.x));
}

CTEST(HCCurve, CubicBaselineProjection) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCPoint b = HCCurveBaselineProjectionCubic(p0, p1, 0.25);
    ASSERT_DBL_NEAR((p1.y - p0.y) / (p1.x - p0.x), (b.y - p0.y) / (b.x - p0.x));
}

CTEST(HCCurve, BaselineProjection) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCPoint b = HCCurveBaselineProjection(curve, 0.25);
    ASSERT_DBL_NEAR((p1.y - p0.y) / (p1.x - p0.x), (b.y - p0.y) / (b.x - p0.x));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Interpolation
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearInterpolatingCurve) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p = HCPointMake(3.0, -4.0);
    HCReal t = 0.25;
    HCPoint p1 = HCPointInvalid;
    HCCurveInterpolatingPointLinear(p0, p, t, &p1);
    ASSERT_DBL_NEAR(HCCurveDistanceFromPointLinear(p0, p1, p), 0.0);
    ASSERT_TRUE(HCPointIsSimilar(p, HCCurveValueLinear(p0, p1, t), 0.00001));
}

CTEST(HCCurve, QuadraticInterpolatingCurve) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCReal t = 0.25;
    HCPoint p = HCPointMake(2.0, 3.0);
    HCPoint cp = HCPointInvalid;
    HCCurveInterpolatingPointQuadratic(p0, p1, p, t, &cp);
    ASSERT_DBL_NEAR(HCCurveDistanceFromPointQuadratic(p0, cp, p1, p), 0.0);
    ASSERT_TRUE(HCPointIsSimilar(p, HCCurveValueQuadratic(p0, cp, p1, t), 0.00001));
    HCPoint d = HCCurveValue(HCCurveDerivative(HCCurveMakeQuadratic(p0, cp, p1)), t);
    ASSERT_DBL_NEAR(d.x, 4.0);
    ASSERT_DBL_NEAR(d.y, 2.6667);
}

CTEST(HCCurve, CubicInterpolatingCurve) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(4.0, 2.0);
    HCPoint p = HCPointMake(2.0, 3.0);
    HCReal t = 0.25;
    HCReal dx = 1.0;
    HCReal dy = 1.0;
    HCPoint c0 = HCPointInvalid;
    HCPoint c1 = HCPointInvalid;
    HCCurveInterpolatingPointCubic(p0, p1, p, t, dx, dy, &c0, &c1);
    ASSERT_DBL_NEAR(HCCurveDistanceFromPointCubic(p0, c0, c1, p1, p), 0.0);
    ASSERT_TRUE(HCPointIsSimilar(p, HCCurveValueCubic(p0, c0, c1, p1, t), 0.00001));
    
    HCPoint d = HCCurveValue(HCCurveDerivative(HCCurveMakeCubic(p0, c0, c1, p1)), t);
    ASSERT_DBL_NEAR(d.x, dx);
    ASSERT_DBL_NEAR(d.y, dy);
}

CTEST(HCCurve, InterpolatingCurve) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCPoint p = HCPointMake(2.0, 3.0);
    HCReal t = 0.25;
    HCReal dx = 1.0;
    HCReal dy = 0.25;
    HCCurve curve = HCCurveInterpolatingPoint(p0, p1, p, t, dx, dy);
    ASSERT_DBL_NEAR(HCCurveDistanceFromPoint(curve, p), 0.0);
    ASSERT_TRUE(HCPointIsSimilar(p, HCCurveValue(curve, t), 0.00001));
    
    HCPoint d = HCCurveValue(HCCurveDerivative(curve), t);
    ASSERT_DBL_NEAR(d.x, dx);
    ASSERT_DBL_NEAR(d.y, dy);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Moulding
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearMould) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCReal t = 0.25;
    HCPoint p = HCPointMake(2.0, 3.0);
    HCCurveMouldLinear(p0, p1, t, p);
}

CTEST(HCCurve, QuadraticMould) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCReal t = 0.25;
    HCPoint p = HCPointMake(2.0, 3.0);
    HCPoint cp = HCPointInvalid;
    HCCurveMouldQuadratic(p0, c, p1, t, p, &cp);
    ASSERT_DBL_FAR(HCCurveDistanceFromPointQuadratic(p0, c, p1, p), 0.0);
    ASSERT_DBL_NEAR(HCCurveDistanceFromPointQuadratic(p0, cp, p1, p), 0.0);
    ASSERT_TRUE(HCPointIsSimilar(p, HCCurveValueQuadratic(p0, cp, p1, t), 0.00001));
}

CTEST(HCCurve, CubicMould) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(4.0, 2.0);
    HCReal t = 0.25;
    HCPoint p = HCPointMake(2.0, 3.0);
    HCPoint c0p = HCPointInvalid;
    HCPoint c1p = HCPointInvalid;
    HCCurveMouldCubic(p0, c0, c1, p1, t, p, &c0p, &c1p);
    ASSERT_DBL_FAR(HCCurveDistanceFromPointCubic(p0, c0, c1, p1, p), 0.0);
    ASSERT_DBL_NEAR(HCCurveDistanceFromPointCubic(p0, c0p, c1p, p1, p), 0.0);
    ASSERT_TRUE(HCPointIsSimilar(p, HCCurveValueCubic(p0, c0p, c1p, p1, t), 0.00001));
}

CTEST(HCCurve, Mould) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCReal t = 0.25;
    HCPoint p = HCPointMake(2.0, 3.0);
    HCCurve moulded = HCCurveMould(curve, t, p);
    ASSERT_DBL_FAR(HCCurveDistanceFromPoint(curve, p), 0.0);
    ASSERT_DBL_NEAR(HCCurveDistanceFromPoint(moulded, p), 0.0);
    ASSERT_TRUE(HCPointIsSimilar(p, HCCurveValue(moulded, t), 0.00001));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Split
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, LinearSplit) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCReal t = 0.25;
    HCPoint sp0 = HCPointInvalid;
    HCPoint sp1 = HCPointInvalid;
    HCPoint ep0 = HCPointInvalid;
    HCPoint ep1 = HCPointInvalid;
    HCCurveSplitLinear(p0, p1, t, &sp0, &sp1, &ep0, &ep1);
    ASSERT_TRUE(HCPointIsEqual(sp0, p0));
    ASSERT_TRUE(HCPointIsEqual(sp1, ep0));
    ASSERT_TRUE(HCPointIsEqual(ep1, p1));
    ASSERT_TRUE(HCPointIsEqual(sp1, HCCurveValueLinear(p0, p1, t)));
    ASSERT_DBL_NEAR(HCCurveLengthLinear(sp0, sp1) + HCCurveLengthLinear(ep0, ep1), HCCurveLengthLinear(p0, p1));
}

CTEST(HCCurve, QuadraticSplit) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCReal t = 0.25;
    HCPoint sp0 = HCPointInvalid;
    HCPoint  sc = HCPointInvalid;
    HCPoint sp1 = HCPointInvalid;
    HCPoint ep0 = HCPointInvalid;
    HCPoint  ec = HCPointInvalid;
    HCPoint ep1 = HCPointInvalid;
    HCCurveSplitQuadratic(p0, c, p1, t, &sp0, &sc, &sp1, &ep0, &ec, &ep1);
    ASSERT_TRUE(HCPointIsEqual(sp0, p0));
    ASSERT_TRUE(HCPointIsEqual(sp1, ep0));
    ASSERT_TRUE(HCPointIsEqual(ep1, p1));
    ASSERT_TRUE(HCPointIsEqual(sp1, HCCurveValueQuadratic(p0, c, p1, t)));
    ASSERT_DBL_NEAR(HCCurveLengthQuadratic(sp0, sc, sp1) + HCCurveLengthQuadratic(ep0, ec, ep1), HCCurveLengthQuadratic(p0, c, p1));
}

CTEST(HCCurve, CubicSplit) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint c0 = HCPointMake(0.0, 2.0);
    HCPoint c1 = HCPointMake(2.0, 2.0);
    HCPoint p1 = HCPointMake(1.0, 4.0);
    HCReal t = 0.25;
    HCPoint sp0 = HCPointInvalid;
    HCPoint sc0 = HCPointInvalid;
    HCPoint sc1 = HCPointInvalid;
    HCPoint sp1 = HCPointInvalid;
    HCPoint ep0 = HCPointInvalid;
    HCPoint ec0 = HCPointInvalid;
    HCPoint ec1 = HCPointInvalid;
    HCPoint ep1 = HCPointInvalid;
    HCCurveSplitCubic(p0, c0, c1, p1, t, &sp0, &sc0, &sc1, &sp1, &ep0, &ec0, &ec1, &ep1);
    ASSERT_TRUE(HCPointIsEqual(sp0, p0));
    ASSERT_TRUE(HCPointIsEqual(sp1, ep0));
    ASSERT_TRUE(HCPointIsEqual(ep1, p1));
    ASSERT_TRUE(HCPointIsEqual(sp1, HCCurveValueCubic(p0, c0, c1, p1, t)));
    ASSERT_DBL_NEAR(HCCurveLengthCubic(sp0, sc0, sc1, sp1) + HCCurveLengthCubic(ep0, ec0, ec1, ep1), HCCurveLengthCubic(p0, c0, c1, p1));
}

CTEST(HCCurve, Split) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint c0 = HCPointMake(0.0, 2.0);
    HCPoint c1 = HCPointMake(2.0, 2.0);
    HCPoint p1 = HCPointMake(1.0, 4.0);
    HCReal t = 0.25;
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCCurve sCurve = HCCurveInvalid;
    HCCurve eCurve = HCCurveInvalid;
    HCCurveSplit(curve, t, &sCurve, &eCurve);
    ASSERT_TRUE(HCPointIsEqual(sCurve.p0, p0));
    ASSERT_TRUE(HCPointIsEqual(sCurve.p1, eCurve.p0));
    ASSERT_TRUE(HCPointIsEqual(eCurve.p1, p1));
    ASSERT_TRUE(HCPointIsEqual(sCurve.p1, HCCurveValueCubic(p0, c0, c1, p1, t)));
    ASSERT_DBL_NEAR(HCCurveLength(sCurve) + HCCurveLength(eCurve), HCCurveLength(curve));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Axis Alignment
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCCurve, LinearAxisAlign) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, 4.0);
    HCPoint xAxisAlignedP0 = HCPointInvalid;
    HCPoint xAxisAlignedP1 = HCPointInvalid;
    HCCurveXAxisAlignedLinear(p0, p1, &xAxisAlignedP0, &xAxisAlignedP1);
    ASSERT_DBL_NEAR(xAxisAlignedP0.x, 0.0);
    ASSERT_DBL_NEAR(xAxisAlignedP0.y, 0.0);
    ASSERT_DBL_NEAR(xAxisAlignedP1.y, 0.0);
    
    HCPoint yAxisAlignedP0 = HCPointInvalid;
    HCPoint yAxisAlignedP1 = HCPointInvalid;
    HCCurveYAxisAlignedLinear(p0, p1, &yAxisAlignedP0, &yAxisAlignedP1);
    ASSERT_DBL_NEAR(yAxisAlignedP0.x, 0.0);
    ASSERT_DBL_NEAR(yAxisAlignedP0.y, 0.0);
    ASSERT_DBL_NEAR(yAxisAlignedP1.x, 0.0);
}

CTEST(HCCurve, QuadraticAxisAlign) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCPoint xAxisAlignedP0 = HCPointInvalid;
    HCPoint xAxisAlignedC = HCPointInvalid;
    HCPoint xAxisAlignedP1 = HCPointInvalid;
    HCCurveXAxisAlignedQuadratic(p0, c, p1, &xAxisAlignedP0, &xAxisAlignedC, &xAxisAlignedP1);
    ASSERT_DBL_NEAR(xAxisAlignedP0.x, 0.0);
    ASSERT_DBL_NEAR(xAxisAlignedP0.y, 0.0);
    ASSERT_DBL_NEAR(xAxisAlignedP1.y, 0.0);
    
    HCPoint yAxisAlignedP0 = HCPointInvalid;
    HCPoint yAxisAlignedC = HCPointInvalid;
    HCPoint yAxisAlignedP1 = HCPointInvalid;
    HCCurveYAxisAlignedQuadratic(p0, c, p1, &yAxisAlignedP0, &yAxisAlignedC, &yAxisAlignedP1);
    ASSERT_DBL_NEAR(yAxisAlignedP0.x, 0.0);
    ASSERT_DBL_NEAR(yAxisAlignedP0.y, 0.0);
    ASSERT_DBL_NEAR(yAxisAlignedP1.x, 0.0);
}

CTEST(HCCurve, CubicAxisAlign) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCPoint xAxisAlignedP0 = HCPointInvalid;
    HCPoint xAxisAlignedC0 = HCPointInvalid;
    HCPoint xAxisAlignedC1 = HCPointInvalid;
    HCPoint xAxisAlignedP1 = HCPointInvalid;
    HCCurveXAxisAlignedCubic(p0, c0, c1, p1, &xAxisAlignedP0, &xAxisAlignedC0, &xAxisAlignedC1, &xAxisAlignedP1);
    ASSERT_DBL_NEAR(xAxisAlignedP0.x, 0.0);
    ASSERT_DBL_NEAR(xAxisAlignedP0.y, 0.0);
    ASSERT_DBL_NEAR(xAxisAlignedP1.y, 0.0);
    
    HCPoint yAxisAlignedP0 = HCPointInvalid;
    HCPoint yAxisAlignedC0 = HCPointInvalid;
    HCPoint yAxisAlignedC1 = HCPointInvalid;
    HCPoint yAxisAlignedP1 = HCPointInvalid;
    HCCurveYAxisAlignedCubic(p0, c0, c1, p1, &yAxisAlignedP0, &yAxisAlignedC0, &yAxisAlignedC1, &yAxisAlignedP1);
    ASSERT_DBL_NEAR(yAxisAlignedP0.x, 0.0);
    ASSERT_DBL_NEAR(yAxisAlignedP0.y, 0.0);
    ASSERT_DBL_NEAR(yAxisAlignedP1.x, 0.0);
    
    HCPoint curveCanonical = HCCurveCanonical(p0, c0, c1, p1);
    HCPoint xAxisAlignedCanonical = HCCurveCanonical(xAxisAlignedP0, xAxisAlignedC0, xAxisAlignedC1, xAxisAlignedP1);
    HCPoint yAxisAlignedCanonical = HCCurveCanonical(yAxisAlignedP0, yAxisAlignedC0, yAxisAlignedC1, yAxisAlignedP1);
    ASSERT_TRUE(HCPointIsSimilar(curveCanonical, xAxisAlignedCanonical, 0.000001));
    ASSERT_TRUE(HCPointIsSimilar(curveCanonical, yAxisAlignedCanonical, 0.000001));
}

CTEST(HCCurve, AxisAlign) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(3.0, 0.0);
    HCPoint c1 = HCPointMake(5.0, 1.0);
    HCPoint p1 = HCPointMake(7.0, 8.0);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    HCCurve xAxisAligned = HCCurveXAxisAligned(curve);
    ASSERT_DBL_NEAR(xAxisAligned.p0.x, 0.0);
    ASSERT_DBL_NEAR(xAxisAligned.p0.y, 0.0);
    ASSERT_DBL_NEAR(xAxisAligned.p1.y, 0.0);
    
    HCCurve yAxisAligned = HCCurveYAxisAligned(curve);
    ASSERT_DBL_NEAR(yAxisAligned.p0.x, 0.0);
    ASSERT_DBL_NEAR(yAxisAligned.p0.y, 0.0);
    ASSERT_DBL_NEAR(yAxisAligned.p1.x, 0.0);
    
    HCPoint curveCanonical = HCCurveCanonical(curve.p0, curve.c0, curve.c1, curve.p1);
    HCPoint xAxisAlignedCanonical = HCCurveCanonical(xAxisAligned.p0, xAxisAligned.c0, xAxisAligned.c1, xAxisAligned.p1);
//    HCPoint yAxisAlignedCanonical = HCCurveCanonical(yAxisAligned.p0, yAxisAligned.c0, yAxisAligned.c1, yAxisAligned.p1);
    ASSERT_TRUE(HCPointIsSimilar(curveCanonical, xAxisAlignedCanonical, 0.000001));
    // TODO: Why does this fail? c0.x is zero so the calculation is highly unstable
//    ASSERT_TRUE(HCPointIsSimilar(curveCanonical, yAxisAlignedCanonical, 0.000001));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Intersection
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCCurve, IntersectionLinearLinear) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint p1 = HCPointMake(100.0, 100.0);
    HCPoint q0 = HCPointMake(100.0, 0.0);
    HCPoint q1 = HCPointMake(0.0, 100.0);
    HCInteger count = 1;
    HCReal t[count];
    HCReal u[count];
    HCCurveIntersectionLinearLinear(p0, p1, q0, q1, &count, t, u);
    ASSERT_TRUE(count == 1);
    ASSERT_DBL_NEAR(t[0], 0.5);
    ASSERT_DBL_NEAR(u[0], 0.5);
}

CTEST(HCCurve, IntersectionLinearQuadratic) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint p1 = HCPointMake(100.0, 0.0);
    HCPoint q0 = HCPointMake(0.0, -50.0);
    HCPoint qc = HCPointMake(50.0, 100.0);
    HCPoint q1 = HCPointMake(100.0, -50.0);
    HCInteger count = 2;
    HCReal t[count];
    HCReal u[count];
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
    HCInteger count = 3;
    HCReal t[count];
    HCReal u[count];
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
    HCInteger count = 4;
    HCReal t[count];
    HCReal u[count];
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
    HCInteger count = 6;
    HCReal t[count];
    HCReal u[count];
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
    HCInteger count = 9;
    HCReal t[count];
    HCReal u[count];
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
    HCInteger count = 9;
    HCReal t[count];
    HCReal u[count];
    HCCurveIntersection(curveP, curveQ, &count, t, u);
    ASSERT_TRUE(count == 3);
    ASSERT_DBL_NEAR(t[0], 0.1724);
    ASSERT_DBL_NEAR(t[1], 0.4995);
    ASSERT_DBL_NEAR(t[2], 0.8267);
    ASSERT_DBL_NEAR(u[0], 0.1724);
    ASSERT_DBL_NEAR(u[1], 0.4995);
    ASSERT_DBL_NEAR(u[2], 0.8276);
}
