///
/// @file HCContourComponent.c
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
    HCContourComponent linear = HCContourComponentMakeLinear(HCPointMake(-1.0, 2.0));
    ASSERT_DBL_NEAR(linear.p.x, -1.0);
    ASSERT_DBL_NEAR(linear.p.y, 2.0);
    
    HCContourComponent quadratic = HCContourComponentMakeQuadratic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0));
    ASSERT_DBL_NEAR(quadratic.c0.x, -1.0);
    ASSERT_DBL_NEAR(quadratic.c0.y, 2.0);
    ASSERT_DBL_NEAR(quadratic.p.x, -3.0);
    ASSERT_DBL_NEAR(quadratic.p.y, 4.0);
    
    HCContourComponent cubic = HCContourComponentMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0));
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
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentMakeLinear(HCPointMake(1.0, 2.0)), HCContourComponentMakeLinear(HCPointMake(1.5, 2.5)), 0.6));
    ASSERT_FALSE(HCContourComponentIsSimilar(HCContourComponentMakeLinear(HCPointMake(1.0, 2.0)), HCContourComponentMakeLinear(HCPointMake(1.5, 2.5)), 0.4));
    ASSERT_FALSE(HCContourComponentIsEqual(HCContourComponentMakeLinear(HCPointMake(1.0, -2.5)), HCContourComponentMakeLinear(HCPointMake(1.0, 2.5))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentMakeLinear(HCPointMake(1.0, -2.5)), HCContourComponentMakeLinear(HCPointMake(1.0, -2.5))));
    ASSERT_EQUAL(HCContourComponentHashValue(HCContourComponentMakeLinear(HCPointMake(1.0, -2.5))), HCContourComponentHashValue(HCContourComponentMakeLinear(HCPointMake(1.0, -2.5))));
}

CTEST(HCContourCurve, Print) {
    HCContourComponent e = HCContourComponentMakeCubic(HCPointMake(-1.0, 2.0), HCPointMake(-3.0, 4.0), HCPointMake(-5.0, 6.0));
    HCContourComponentPrint(e, stdout); // TODO: Not to stdout
}

CTEST(HCContourCurve, Queries) {
    HCContourComponent invalid = HCContourComponentInvalid;
    ASSERT_TRUE(HCContourComponentIsInvalid(invalid));
    ASSERT_FALSE(HCContourComponentIsZero(invalid));
    ASSERT_FALSE(HCContourComponentIsInfinite(invalid));
    
    HCContourComponent zero = HCContourComponentZero;
    ASSERT_FALSE(HCContourComponentIsInvalid(zero));
    ASSERT_TRUE(HCContourComponentIsZero(zero));
    ASSERT_FALSE(HCContourComponentIsInfinite(zero));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Order
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCContourCurve, Order) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint c0 = HCPointMake(3.0, -4.0);
    HCPoint c1 = HCPointMake(-5.0, 6.0);
    HCPoint p1 = HCPointMake(7.0, -8.0);
    
    HCContourComponent linear = HCContourComponentMakeLinear(p1);
    ASSERT_FALSE(HCContourComponentIsInvalid(linear));
    ASSERT_FALSE(HCContourComponentIsZero(linear));
    ASSERT_FALSE(HCContourComponentIsInfinite(linear));
    ASSERT_TRUE(HCContourComponentIsLinear(p0, linear));
    ASSERT_FALSE(HCContourComponentIsQuadratic(p0, linear));
    ASSERT_FALSE(HCContourComponentIsCubic(p0, linear));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsLinear(p0, linear), linear, 0.000001));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsQuadratic(p0, linear), HCContourComponentMakeQuadratic(p1, p1), 0.000001));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsCubic(p0, linear), HCContourComponentMakeCubic(p0, p1, p1), 0.000001));
    
    HCContourComponent quadratic = HCContourComponentMakeQuadratic(c0, p1);
    ASSERT_FALSE(HCContourComponentIsInvalid(quadratic));
    ASSERT_FALSE(HCContourComponentIsZero(quadratic));
    ASSERT_FALSE(HCContourComponentIsInfinite(quadratic));
    ASSERT_FALSE(HCContourComponentIsLinear(p0, quadratic));
    ASSERT_TRUE(HCContourComponentIsQuadratic(p0, quadratic));
    ASSERT_FALSE(HCContourComponentIsCubic(p0, quadratic));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsLinear(p0, quadratic), HCContourComponentMakeLinear(quadratic.p), 0.000001));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsQuadratic(p0, quadratic), quadratic, 0.000001));
    HCPoint qc0 = HCPointMake(p0.x + (2.0/3.0) * (c0.x - p0.x), p0.y + (2.0/3.0) * (c0.y - p0.y));
    HCPoint qc1 = HCPointMake(p1.x + (2.0/3.0) * (c0.x - p1.x), p1.y + (2.0/3.0) * (c0.y - p1.y));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsCubic(p0, quadratic), HCContourComponentMakeCubic(qc0, qc1, p1), 0.000001));
    
    HCContourComponent cubic = HCContourComponentMakeCubic(c0, c1, p1);
    ASSERT_FALSE(HCContourComponentIsInvalid(cubic));
    ASSERT_FALSE(HCContourComponentIsZero(cubic));
    ASSERT_FALSE(HCContourComponentIsInfinite(cubic));
    ASSERT_FALSE(HCContourComponentIsLinear(p0, cubic));
    ASSERT_FALSE(HCContourComponentIsQuadratic(p0, cubic));
    ASSERT_TRUE(HCContourComponentIsCubic(p0, cubic));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsLinear(p0, cubic), HCContourComponentMakeLinear(cubic.p), 0.000001));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsQuadratic(p0, cubic), HCContourComponentMakeQuadratic(HCPointInterpolate(cubic.c0, cubic.c1, 0.5), cubic.p), 0.000001));
    ASSERT_TRUE(HCContourComponentIsSimilar(HCContourComponentAsCubic(p0, cubic), cubic, 0.000001));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Canonical Type
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCContourCurve, Canonical) {
    HCPoint invalidP0 = HCPointInvalid;
    HCContourComponent invalid = HCContourComponentInvalid;
    ASSERT_TRUE(HCContourComponentCanonicalType(invalidP0, invalid) == HCCurveTypeInvalid);
    
    HCPoint pointP0 = HCPointMake(1.0, 1.0);
    HCContourComponent point = HCContourComponentMakeCubic(pointP0, pointP0, pointP0);
    ASSERT_TRUE(HCContourComponentCanonicalType(pointP0, point) == HCCurveTypePoint);
    
    HCPoint linearP0 = HCPointMake(1.0, 1.0);
    HCContourComponent linear = HCContourComponentMakeLinear(HCPointMake(2.0, 2.0));
    ASSERT_TRUE(HCContourComponentCanonicalType(linearP0, linear) == HCCurveTypeLinear);
    
    HCPoint quadraticP0 = HCPointMake(1.0, 1.0);
    HCContourComponent quadratic = HCContourComponentMakeQuadratic(HCPointMake(2.0, 2.0), HCPointMake(3.0, 1.0));
    ASSERT_TRUE(HCContourComponentCanonicalType(quadraticP0, quadratic) == HCCurveTypeQuadratic);
    
    HCPoint simpleP0 = HCPointMake(0.0, 0.0);
    HCContourComponent simple = HCContourComponentMakeCubic(HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourComponentCanonicalType(simpleP0, simple) == HCCurveTypeCubicSimple);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(simpleP0, simple.c0, simple.c1, simple.p), HCPointMake(1.0, 0.0), 0.0000001));
    
    HCPoint singleInflectionP0 = HCPointMake(0.0, 0.0);
    HCContourComponent singleInflection = HCContourComponentMakeCubic(HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(50.0, 200.0));
    ASSERT_TRUE(HCContourComponentCanonicalType(singleInflectionP0, singleInflection) == HCCurveTypeCubicSingleInflection);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(singleInflectionP0, singleInflection.c0, singleInflection.c1, singleInflection.p), HCPointMake(0.5, 2.0), 0.0000001));
    
    HCPoint doubleInflectionP0 = HCPointMake(0.0, 0.0);
    HCContourComponent doubleInflection = HCContourComponentMakeCubic(HCPointMake(125.0, 75.0), HCPointMake(100.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourComponentCanonicalType(doubleInflectionP0, doubleInflection) == HCCurveTypeCubicDoubleInflection);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(doubleInflectionP0, doubleInflection.c0, doubleInflection.c1, doubleInflection.p), HCPointMake(-1.5, 0.5), 0.0000001));
    
    HCPoint loopP0 = HCPointMake(25.0, 0.0);
    HCContourComponent loop = HCContourComponentMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourComponentCanonicalType(loopP0, loop) == HCCurveTypeCubicLoop);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(loopP0, loop.c0, loop.c1, loop.p), HCPointMake(-0.75, 0.0), 0.0000001));
    
    HCPoint loopAtStartP0 = HCPointMake(0.0, 0.0);
    HCContourComponent loopAtStart = HCContourComponentMakeCubic(HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(-300.0, -600.0));
    ASSERT_TRUE(HCContourComponentCanonicalType(loopAtStartP0, loopAtStart) == HCCurveTypeCubicLoopAtStart);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(loopAtStartP0, loopAtStart.c0, loopAtStart.c1, loopAtStart.p), HCPointMake(-3.0, -6.0), 0.0000001));

    HCPoint loopAtEndP0 = HCPointMake(0.0, 0.0);
    HCContourComponent loopAtEnd = HCContourComponentMakeCubic(HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(50.0, ((sqrt(5.25) - 0.5) * 0.5) * 100.0));
    ASSERT_TRUE(HCContourComponentCanonicalType(loopAtEndP0, loopAtEnd) == HCCurveTypeCubicLoopAtEnd);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(loopAtEndP0, loopAtEnd.c0, loopAtEnd.c1, loopAtEnd.p), HCPointMake(0.5, ((sqrt(5.25) - 0.5) * 0.5)), 0.0000001));
    
    HCPoint loopClosedP0 = HCPointMake(50.0, 0.0);
    HCContourComponent loopClosed = HCContourComponentMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(50.0, 0.0));
    ASSERT_TRUE(HCContourComponentCanonicalType(loopClosedP0, loopClosed) == HCCurveTypeCubicLoopClosed);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(loopClosedP0, loopClosed.c0, loopClosed.c1, loopClosed.p), HCPointMake(0.0, 0.0), 0.0000001));
    
    HCPoint cuspP0 = HCPointMake(0.0, 0.0);
    HCContourComponent cusp = HCContourComponentMakeCubic(HCPointMake(100.0, 100.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 0.0));
    ASSERT_TRUE(HCContourComponentCanonicalType(cuspP0, cusp) == HCCurveTypeCubicCusp);
    ASSERT_TRUE(HCPointIsSimilar(HCCurveCanonical(cuspP0, cusp.c0, cusp.c1, cusp.p), HCPointMake(-1.0, 0.0), 0.0000001));
    
    HCPoint coanchorP0 = HCPointMake(1.0, 1.0);
    HCPoint coanchorP1 = HCPointMake(4.0, 1.0);
    HCContourComponent coanchor10 = HCContourComponentMakeCubic(coanchorP0, HCPointMake(3.0, 0.0), coanchorP1);
    ASSERT_TRUE(HCContourComponentCanonicalType(coanchorP0, coanchor10) == HCCurveTypeCubicSimple);
    HCContourComponent coanchor01 = HCContourComponentMakeCubic(HCPointMake(2.0, 0.0), coanchorP1, coanchorP1);
    ASSERT_TRUE(HCContourComponentCanonicalType(coanchorP0, coanchor01) == HCCurveTypeCubicSimple);
    
    HCPoint colinearP0 = HCPointMake(1.0, 1.0);
    HCContourComponent colinear = HCContourComponentMakeCubic(HCPointMake(2.0, 2.0), HCPointMake(3.0, 3.0), HCPointMake(4.0, 4.0));
    ASSERT_TRUE(HCContourComponentCanonicalType(colinearP0, colinear) == HCCurveTypeLinear);
    
    HCPoint colinearQuadraticP0 = HCPointMake(1.0, 1.0);
    HCContourComponent colinearQuadratic = HCContourComponentMakeQuadratic(HCPointMake(2.0, 2.0), HCPointMake(3.0, 3.0));
    ASSERT_TRUE(HCContourComponentCanonicalType(colinearQuadraticP0, colinearQuadratic) == HCCurveTypeLinear);
    
    HCPoint cqp0 = HCPointMake(20.0, 30.0);
    HCPoint cqc = HCPointMake(25.0, 60.0);
    HCPoint cqp1 = HCPointMake(30.0, 50.0);
    HCPoint cqc0 = HCPointMake(cqp0.x + (2.0/3.0) * (cqc.x - cqp0.x), cqp0.y + (2.0/3.0) * (cqc.y - cqp0.y));
    HCPoint cqc1 = HCPointMake(cqp1.x + (2.0/3.0) * (cqc.x - cqp1.x), cqp1.y + (2.0/3.0) * (cqc.y - cqp1.y));
    HCContourComponent coquadratic = HCContourComponentMakeCubic(cqc0, cqc1, cqp1);
    ASSERT_TRUE(HCContourComponentCanonicalType(cqp0, coquadratic) == HCCurveTypeQuadratic);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Extrema
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearExtrema) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCInteger count = 0;
    HCContourComponentExtrema(p0, component, &count, NULL);
    ASSERT_TRUE(count == 0);
}

CTEST(HCContourCurve, QuadraticExtrema) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCInteger count = 0;
    HCReal extrema[2];
    HCContourComponentExtrema(p0, component, &count, extrema);
    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(extrema[0] == 0.5);
}

CTEST(HCContourCurve, CubicExtrema) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCInteger count = 0;
    HCReal extrema[6];
    HCContourComponentExtrema(p0, component, &count, extrema);
    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(extrema[0] == 0.5);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Inflection
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearInflections) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCInteger count = 0;
    HCContourComponentInflections(p0, component, &count, NULL);
    ASSERT_TRUE(count == 0);
}

CTEST(HCContourCurve, QuadraticInflection) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCInteger count = 0;
    HCContourComponentInflections(p0, component, &count, NULL);
    ASSERT_TRUE(count == 0);
}

CTEST(HCContourCurve, CubicInflection) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint c0 = HCPointMake(0.0, 2.0);
    HCPoint c1 = HCPointMake(2.0, 2.0);
    HCPoint p1 = HCPointMake(1.0, 4.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCInteger count = 0;
    HCReal inflections[2];
    HCContourComponentInflections(p0, component, &count, inflections);
    ASSERT_TRUE(HCCurveCanonicalTypeCubic(p0, c0, c1, p1) == HCCurveTypeCubicSingleInflection);
    ASSERT_TRUE(count == 1);
    ASSERT_DBL_NEAR(inflections[0], 0.4384);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Approximate Bounds
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearApproximateBounds) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCRectangle bounds = HCContourComponentApproximateBounds(p0, component);
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(
        fmin(p0.x, p1.x),
        fmin(p0.y, p1.y),
        fmax(p0.x, p1.x),
        fmax(p0.y, p1.y))));
}

CTEST(HCContourCurve, QuadraticApproximateBounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCRectangle bounds = HCContourComponentApproximateBounds(p0, component);
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(
        fmin(p0.x, fmin(c.x, p1.x)),
        fmin(p0.y, fmin(c.y, p1.y)),
        fmax(p0.x, fmax(c.x, p1.x)),
        fmax(p0.y, fmax(c.y, p1.y)))));
}

CTEST(HCContourCurve, CubicApproximateBounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCRectangle bounds = HCContourComponentApproximateBounds(p0, component);
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
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCRectangle bounds = HCContourComponentBounds(p0, HCContourComponentMakeLinear(p1));
    ASSERT_TRUE(HCRectangleContainsRectangle(HCContourComponentApproximateBounds(p0, component), bounds));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(fmin(p0.x, p1.x), fmin(p0.y, p1.y), fmax(p0.x, p1.x), fmax(p0.y, p1.y))));
}

CTEST(HCContourCurve, QuadraticBounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCRectangle bounds = HCContourComponentBounds(p0, component);
    ASSERT_TRUE(HCRectangleContainsRectangle(HCContourComponentApproximateBounds(p0, component), bounds));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(1.0, 2.0, 5.0, 3.0)));
}

CTEST(HCContourCurve, CubicBounds) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCRectangle bounds = HCContourComponentBounds(p0, component);
    ASSERT_TRUE(HCRectangleContainsRectangle(HCContourComponentApproximateBounds(p0, component), bounds));
    ASSERT_TRUE(HCRectangleIsEqual(bounds, HCRectangleMakeWithEdges(1.0, 2.0, 5.0, 3.5)));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Length
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearLength) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCReal length = HCContourComponentLength(p0, component);
    ASSERT_DBL_NEAR(length, HCPointDistance(p0, p1));
}

CTEST(HCContourCurve, QuadraticLength) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCReal length = HCContourComponentLength(p0, component);
    ASSERT_TRUE(length > HCPointDistance(p0, p1) && length < HCPointDistance(p0, c) + HCPointDistance(c, p1));
}

CTEST(HCContourCurve, CubicLength) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCReal length = HCContourComponentLength(p0, component);
    ASSERT_TRUE(length > HCPointDistance(p0, p1) && length < HCPointDistance(p0, c0) + HCPointDistance(c0, c1) + HCPointDistance(c1, p1));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearValue) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, 4.0);
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCReal t = 0.5;
    HCPoint s = HCContourComponentValue(p0, component, t);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(2.0, 3.0), 0.000001));
}

CTEST(HCContourCurve, QuadraticValue) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCReal t = 0.5;
    HCPoint s = HCContourComponentValue(p0, component, t);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 3.0), 0.000001));
}

CTEST(HCContourCurve, CubicValue) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCReal t = 0.5;
    HCPoint s = HCContourComponentValue(p0, component, t);
    ASSERT_TRUE(HCPointIsSimilar(s, HCPointMake(3.0, 3.5), 0.000001));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Derivative
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearDerivative) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, 4.0);
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCPoint dp0 = HCPointInvalid;
    HCContourComponent d = HCContourComponentInvalid;
    HCContourComponentDerivative(p0, component, &dp0, &d);
    ASSERT_DBL_NEAR(dp0.x, 1.0 * (p1.x - p0.x));
    ASSERT_DBL_NEAR(dp0.y, 1.0 * (p1.y - p0.y));
}

CTEST(HCContourCurve, QuadraticDerivative) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCPoint dp0 = HCPointInvalid;
    HCContourComponent d = HCContourComponentInvalid;
    HCContourComponentDerivative(p0, component, &dp0, &d);
    ASSERT_DBL_NEAR(dp0.x, 2.0 * ( c.x - p0.x));
    ASSERT_DBL_NEAR(dp0.y, 2.0 * ( c.y - p0.y));
    ASSERT_DBL_NEAR(d.p.x, 2.0 * (p1.x -  c.x));
    ASSERT_DBL_NEAR(d.p.y, 2.0 * (p1.y -  c.y));
    
    HCPoint ddp0 = HCPointInvalid;
    HCContourComponent dd = HCContourComponentInvalid;
    HCContourComponentDerivative(dp0, d, &ddp0, &dd);
    ASSERT_DBL_NEAR(ddp0.x, 1.0 * (d.p.x - dp0.x));
    ASSERT_DBL_NEAR(ddp0.y, 1.0 * (d.p.y - dp0.y));
}

CTEST(HCContourCurve, CubicDerivative) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCPoint dp0 = HCPointInvalid;
    HCContourComponent d = HCContourComponentInvalid;
    HCContourComponentDerivative(p0, component, &dp0, &d);
    ASSERT_DBL_NEAR( dp0.x, 3.0 * (c0.x - p0.x));
    ASSERT_DBL_NEAR( dp0.y, 3.0 * (c0.y - p0.y));
    ASSERT_DBL_NEAR(d.c0.x, 3.0 * (c1.x - c0.x));
    ASSERT_DBL_NEAR(d.c0.y, 3.0 * (c1.y - c0.y));
    ASSERT_DBL_NEAR( d.p.x, 3.0 * (p1.x - c1.x));
    ASSERT_DBL_NEAR( d.p.y, 3.0 * (p1.y - c1.y));
    
    HCPoint ddp0 = HCPointInvalid;
    HCContourComponent dd = HCContourComponentInvalid;
    HCContourComponentDerivative(dp0, d, &ddp0, &dd);
    ASSERT_DBL_NEAR(ddp0.x, 2.0 * (d.c0.x - dp0.x));
    ASSERT_DBL_NEAR(ddp0.y, 2.0 * (d.c0.y - dp0.y));
    ASSERT_DBL_NEAR(dd.p.x, 2.0 * (d.p.x - d.c0.x));
    ASSERT_DBL_NEAR(dd.p.y, 2.0 * (d.p.y - d.c0.y));
    
    HCPoint dddp = HCPointInvalid;
    HCContourComponentDerivative(ddp0, dd, &dddp, NULL);
    ASSERT_DBL_NEAR(dddp.x, 1.0 * (dd.p.x - ddp0.x));
    ASSERT_DBL_NEAR(dddp.y, 1.0 * (dd.p.y - ddp0.y));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Curvature
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearCurvature) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, 4.0);
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCReal t = 0.75;
    HCReal k = HCContourComponentCurvature(p0, component, t);
    ASSERT_DBL_NEAR(k, 0.0);
}

CTEST(HCContourCurve, QuadraticCurvature) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCReal t = 0.75;
    HCReal k = HCContourComponentCurvature(p0, component, t);
    ASSERT_DBL_NEAR(k, -0.3578);
}

CTEST(HCContourCurve, CubicCurvature) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCReal t = 0.75;
    HCReal k = HCContourComponentCurvature(p0, component, t);
    ASSERT_DBL_NEAR(k, -0.4409);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Parameterization by Arc Length
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearParameterizedByArcLength) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCReal length = HCContourComponentLength(p0, component);
    for (HCReal d = 0.0; d < length; d += 0.1) {
        HCReal t = HCContourComponentParameterAtLength(p0, component, d);
        ASSERT_DBL_NEAR(t, length / d);
    }
}

CTEST(HCContourCurve, QuadraticParameterizedByArcLength) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCReal length = HCContourComponentLength(p0, component);
    HCReal previousT = 0.0;
    for (HCReal d = 0.0; d < length; d += 0.1) {
        HCReal t = HCContourComponentParameterAtLength(p0, component, d);
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
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCReal length = HCContourComponentLength(p0, component);
    HCReal previousT = 0.0;
    for (HCReal d = 0.0; d < length; d += 0.1) {
        HCReal t = HCContourComponentParameterAtLength(p0, component, d);
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
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal nearest = HCContourComponentParameterNearestPoint(p0, component, p);
    ASSERT_TRUE(nearest >= 0.0);
    ASSERT_TRUE(nearest <= 1.0);
}

CTEST(HCContourCurve, QuadraticParameterNearestPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal nearest = HCContourComponentParameterNearestPoint(p0, component, p);
    ASSERT_TRUE(nearest >= 0.0);
    ASSERT_TRUE(nearest <= 1.0);
}

CTEST(HCContourCurve, CubicParameterNearestPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal nearest = HCContourComponentParameterNearestPoint(p0, component, p);
    ASSERT_TRUE(nearest >= 0.0);
    ASSERT_TRUE(nearest <= 1.0);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Distance from Point
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearDistanceFromPoint) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal distance = HCContourComponentDistanceFromPoint(p0, component, p);
    ASSERT_TRUE(distance <= HCPointDistance(p, p0));
    ASSERT_TRUE(distance <= HCPointDistance(p, p1));
}

CTEST(HCContourCurve, QuadraticDistanceFromPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal distance = HCContourComponentDistanceFromPoint(p0, component, p);
    ASSERT_TRUE(distance <= HCPointDistance(p, p0));
    ASSERT_TRUE(distance <= HCPointDistance(p, c));
    ASSERT_TRUE(distance <= HCPointDistance(p, p1));
}

CTEST(HCContourCurve, CubicDistanceFromPoint) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCPoint p = HCPointMake(1.0, -1.0);
    HCReal distance = HCContourComponentDistanceFromPoint(p0, component, p);
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
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCPoint b = HCContourComponentBaselineProjection(p0, component, 0.25);
    ASSERT_DBL_NEAR((p1.y - p0.y) / (p1.x - p0.x), (b.y - p0.y) / (b.x - p0.x));
}

CTEST(HCContourCurve, QuadraticBaselineProjection) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCPoint b = HCContourComponentBaselineProjection(p0, component, 0.25);
    ASSERT_DBL_NEAR((p1.y - p0.y) / (p1.x - p0.x), (b.y - p0.y) / (b.x - p0.x));
}

CTEST(HCContourCurve, CubicBaselineProjection) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCPoint b = HCContourComponentBaselineProjection(p0, component, 0.25);
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
    HCContourComponent component = HCContourComponentInterpolatingPoint(p0, p1, p, t, dx, dy);
    ASSERT_DBL_NEAR(HCContourComponentDistanceFromPoint(p0, component, p), 0.0);
}

CTEST(HCContourCurve, QuadraticInterpolatingCurve) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCPoint p = HCPointMake(2.0, 3.0);
    HCReal t = 0.25;
    HCReal dx = 0.0;
    HCReal dy = 0.0;
    HCContourComponent component = HCContourComponentInterpolatingPoint(p0, p1, p, t, dx, dy);
    ASSERT_DBL_NEAR(HCContourComponentDistanceFromPoint(p0, component, p), 0.0);
}

CTEST(HCContourCurve, CubicInterpolatingCurve) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint p1 = HCPointMake(4.0, 2.0);
    HCPoint p = HCPointMake(2.0, 3.0);
    HCReal t = 0.25;
    HCReal dx = -2.0;
    HCReal dy = 2.0;
    HCContourComponent component = HCContourComponentInterpolatingPoint(p0, p1, p, t, dx, dy);
    ASSERT_DBL_NEAR(HCContourComponentDistanceFromPoint(p0, component, p), 0.0);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Moulding
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearMould) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCReal t = 0.25;
    HCPoint p = HCPointMake(2.0, 3.0);
    HCContourComponent moulded = HCContourComponentMould(p0, component, t, p);
    ASSERT_TRUE(HCContourComponentIsEqual(component, moulded));
}

CTEST(HCContourCurve, QuadraticMould) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCReal t = 0.25;
    HCPoint p = HCPointMake(2.0, 3.0);
    HCContourComponent moulded = HCContourComponentMould(p0, component, t, p);
    ASSERT_DBL_FAR(HCContourComponentDistanceFromPoint(p0, component, p), 0.0);
    ASSERT_DBL_NEAR(HCContourComponentDistanceFromPoint(p0, moulded, p), 0.0);
    ASSERT_TRUE(HCPointIsSimilar(p, HCContourComponentValue(p0, moulded, t), 0.00001));
}

CTEST(HCContourCurve, CubicMould) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(2.0, 4.0);
    HCPoint c1 = HCPointMake(4.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCReal t = 0.25;
    HCPoint p = HCPointMake(2.0, 3.0);
    HCContourComponent moulded = HCContourComponentMould(p0, component, t, p);
    ASSERT_DBL_FAR(HCContourComponentDistanceFromPoint(p0, component, p), 0.0);
    ASSERT_DBL_NEAR(HCContourComponentDistanceFromPoint(p0, moulded, p), 0.0);
    ASSERT_TRUE(HCPointIsSimilar(p, HCContourComponentValue(p0, moulded, t), 0.00001));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Split
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, LinearSplit) {
    HCPoint p0 = HCPointMake(-1.0, 2.0);
    HCPoint p1 = HCPointMake(3.0, -4.0);
    HCContourComponent component = HCContourComponentMakeLinear(p1);
    HCReal t = 0.25;
    HCPoint sp0 = HCPointInvalid;
    HCContourComponent sComponent = HCContourComponentInvalid;
    HCPoint ep0 = HCPointInvalid;
    HCContourComponent eComponent = HCContourComponentInvalid;
    HCContourComponentSplit(p0, component, t, &sp0, &sComponent, &ep0, &eComponent);
    ASSERT_TRUE(HCPointIsEqual(sp0, p0));
    ASSERT_TRUE(HCPointIsEqual(sComponent.p, ep0));
    ASSERT_TRUE(HCPointIsEqual(eComponent.p, component.p));
    ASSERT_TRUE(HCPointIsEqual(sComponent.p, HCContourComponentValue(p0, component, t)));
    ASSERT_DBL_NEAR(HCContourComponentLength(sp0, sComponent) + HCContourComponentLength(ep0, eComponent), HCContourComponentLength(p0, component));
}

CTEST(HCContourCurve, QuadraticSplit) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint  c = HCPointMake(3.0, 4.0);
    HCPoint p1 = HCPointMake(5.0, 2.0);
    HCContourComponent component = HCContourComponentMakeQuadratic(c, p1);
    HCReal t = 0.25;
    HCPoint sp0 = HCPointInvalid;
    HCContourComponent sComponent = HCContourComponentInvalid;
    HCPoint ep0 = HCPointInvalid;
    HCContourComponent eComponent = HCContourComponentInvalid;
    HCContourComponentSplit(p0, component, t, &sp0, &sComponent, &ep0, &eComponent);
    ASSERT_TRUE(HCPointIsEqual(sp0, p0));
    ASSERT_TRUE(HCPointIsEqual(sComponent.p, ep0));
    ASSERT_TRUE(HCPointIsEqual(eComponent.p, component.p));
    ASSERT_TRUE(HCPointIsEqual(sComponent.p, HCContourComponentValue(p0, component, t)));
    ASSERT_DBL_NEAR(HCContourComponentLength(sp0, sComponent) + HCContourComponentLength(ep0, eComponent), HCContourComponentLength(p0, component));
}

CTEST(HCContourCurve, CubicSplit) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint c0 = HCPointMake(0.0, 2.0);
    HCPoint c1 = HCPointMake(2.0, 2.0);
    HCPoint p1 = HCPointMake(1.0, 4.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCReal t = 0.25;
    HCPoint sp0 = HCPointInvalid;
    HCContourComponent sComponent = HCContourComponentInvalid;
    HCPoint ep0 = HCPointInvalid;
    HCContourComponent eComponent = HCContourComponentInvalid;
    HCContourComponentSplit(p0, component, t, &sp0, &sComponent, &ep0, &eComponent);
    ASSERT_TRUE(HCPointIsEqual(sp0, p0));
    ASSERT_TRUE(HCPointIsEqual(sComponent.p, ep0));
    ASSERT_TRUE(HCPointIsEqual(eComponent.p, component.p));
    ASSERT_TRUE(HCPointIsEqual(sComponent.p, HCContourComponentValue(p0, component, t)));
    ASSERT_DBL_NEAR(HCContourComponentLength(sp0, sComponent) + HCContourComponentLength(ep0, eComponent), HCContourComponentLength(p0, component));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Axis Alignment
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCContourCurve, AxisAligned) {
    HCPoint p0 = HCPointMake(1.0, 2.0);
    HCPoint c0 = HCPointMake(3.0, 0.0);
    HCPoint c1 = HCPointMake(5.0, 1.0);
    HCPoint p1 = HCPointMake(7.0, 8.0);
    HCContourComponent component = HCContourComponentMakeCubic(c0, c1, p1);
    HCContourComponent xAxisAligned = HCContourComponentXAxisAligned(p0, component);
    ASSERT_FALSE(HCContourComponentIsInvalid(xAxisAligned));
    ASSERT_FALSE(HCContourComponentIsZero(xAxisAligned));
    ASSERT_FALSE(HCContourComponentIsInfinite(xAxisAligned));
    ASSERT_FALSE(HCContourComponentIsLinear(HCPointZero, xAxisAligned));
    ASSERT_FALSE(HCContourComponentIsQuadratic(HCPointZero, xAxisAligned));
    ASSERT_TRUE(HCContourComponentIsCubic(HCPointZero, xAxisAligned));
    ASSERT_DBL_NEAR(xAxisAligned.p.y, 0.0);
    
    HCContourComponent yAxisAligned = HCContourComponentYAxisAligned(p0, component);
    ASSERT_FALSE(HCContourComponentIsInvalid(yAxisAligned));
    ASSERT_FALSE(HCContourComponentIsZero(yAxisAligned));
    ASSERT_FALSE(HCContourComponentIsInfinite(yAxisAligned));
    ASSERT_FALSE(HCContourComponentIsLinear(HCPointZero, yAxisAligned));
    ASSERT_FALSE(HCContourComponentIsQuadratic(HCPointZero, yAxisAligned));
    ASSERT_TRUE(HCContourComponentIsCubic(HCPointZero, yAxisAligned));
    ASSERT_DBL_NEAR(yAxisAligned.p.x, 0.0);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Intersection
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContourCurve, IntersectionLinearLinear) {
    HCPoint p0 = HCPointMake(0.0, 0.0);
    HCPoint p1 = HCPointMake(100.0, 100.0);
    HCPoint q0 = HCPointMake(100.0, 0.0);
    HCPoint q1 = HCPointMake(0.0, 100.0);
    HCContourComponent cp = HCContourComponentMakeLinear(p1);
    HCContourComponent cq = HCContourComponentMakeLinear(q1);
    HCInteger count = 0;
    HCReal t = NAN;
    HCReal u = NAN;
    HCContourComponentIntersection(p0, cp, q0, cq, &count, &t, &u);
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
    HCContourComponent cp = HCContourComponentMakeLinear(p1);
    HCContourComponent cq = HCContourComponentMakeQuadratic(qc, q1);
    HCInteger count = 0;
    HCReal t[2] = {NAN, NAN};
    HCReal u[2] = {NAN, NAN};
    HCContourComponentIntersection(p0, cp, q0, cq, &count, t, u);
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
    HCContourComponent cp = HCContourComponentMakeLinear(p1);
    HCContourComponent cq = HCContourComponentMakeCubic(qc0, qc1, q1);
    HCInteger count = 0;
    HCReal t[3] = {NAN, NAN};
    HCReal u[3] = {NAN, NAN};
    HCContourComponentIntersection(p0, cp, q0, cq, &count, t, u);
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
    HCContourComponent cp = HCContourComponentMakeQuadratic(pc, p1);
    HCContourComponent cq = HCContourComponentMakeQuadratic(qc, q1);
    HCInteger count = 0;
    HCReal t[2] = {NAN, NAN};
    HCReal u[2] = {NAN, NAN};
    HCContourComponentIntersection(p0, cp, q0, cq, &count, t, u);
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
    HCContourComponent cp = HCContourComponentMakeQuadratic(pc, p1);
    HCContourComponent cq = HCContourComponentMakeCubic(qc0, qc1, q1);
    HCInteger count = 0;
    HCReal t[6] = {NAN, NAN};
    HCReal u[6] = {NAN, NAN};
    HCContourComponentIntersection(p0, cp, q0, cq, &count, t, u);
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
    HCContourComponent cp = HCContourComponentMakeCubic(pc0, pc1, pp1);
    HCContourComponent cq = HCContourComponentMakeCubic(qc0, qc1, qp1);
    HCInteger count = 0;
    HCReal t[9] = {NAN, NAN};
    HCReal u[9] = {NAN, NAN};
    HCContourComponentIntersection(pp0, cp, qp0, cq, &count, t, u);
    ASSERT_TRUE(count == 3);
    ASSERT_DBL_NEAR(t[0], 0.1724);
    ASSERT_DBL_NEAR(t[1], 0.4995);
    ASSERT_DBL_NEAR(t[2], 0.8267);
    ASSERT_DBL_NEAR(u[0], 0.1724);
    ASSERT_DBL_NEAR(u[1], 0.4995);
    ASSERT_DBL_NEAR(u[2], 0.8276);
}
