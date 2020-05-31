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

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
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
    ASSERT_TRUE(HCContourComponentCount(contour) == 4);
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 0), HCContourComponentMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 1), HCContourComponentMakeLinear(HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 2), HCContourComponentMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 3), HCContourComponentMakeLinear(HCPointMake(1.0, 3.0))));
    ASSERT_TRUE(HCContourCurveCount(contour) == 3);
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 0), HCCurveMakeLinear(HCPointMake(1.0, 1.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 1), HCCurveMakeLinear(HCPointMake(3.0, 1.0), HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 2), HCCurveMakeLinear(HCPointMake(3.0, 3.0), HCPointMake(1.0, 3.0))));
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
    ASSERT_TRUE(HCContourComponentCount(contour) == 4);
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 0), HCContourComponentMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 1), HCContourComponentMakeQuadratic(HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 2), HCContourComponentMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 3), HCContourComponentMakeQuadratic(HCPointMake(3.0, 4.0), HCPointMake(1.0, 3.0))));
    ASSERT_TRUE(HCContourCurveCount(contour) == 3);
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 0), HCCurveMakeQuadratic(HCPointMake(1.0, 1.0), HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 1), HCCurveMakeLinear(HCPointMake(3.0, 1.0), HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 2), HCCurveMakeQuadratic(HCPointMake(3.0, 3.0), HCPointMake(3.0, 4.0), HCPointMake(1.0, 3.0))));
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
    ASSERT_TRUE(HCContourComponentCount(contour) == 4);
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 0), HCContourComponentMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 1), HCContourComponentMakeQuadratic(HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 2), HCContourComponentMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 3), HCContourComponentMakeCubic(HCPointMake(2.5, 4.0), HCPointMake(1.5, 4.0), HCPointMake(1.0, 3.0))));
    ASSERT_TRUE(HCContourCurveCount(contour) == 3);
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 0), HCCurveMakeQuadratic(HCPointMake(1.0, 1.0), HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 1), HCCurveMakeLinear(HCPointMake(3.0, 1.0), HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 2), HCCurveMakeCubic(HCPointMake(3.0, 3.0), HCPointMake(2.5, 4.0), HCPointMake(1.5, 4.0), HCPointMake(1.0, 3.0))));
}

CTEST(HCContour, CreateWithCurves) {
    HCContourComponent components[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    HCContour contour[4];
    HCContourInitWithComponents(contour, components, sizeof(components) / sizeof(HCContourComponent), true);
    ASSERT_TRUE(HCContourIsClosed(contour));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(contour), HCPointMake(1.0, 1.0)));
    ASSERT_TRUE(HCContourComponentCount(contour) == 4);
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 0), HCContourComponentMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 1), HCContourComponentMakeQuadratic(HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 2), HCContourComponentMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 3), HCContourComponentMakeCubic(HCPointMake(2.5, 4.0), HCPointMake(1.5, 4.0), HCPointMake(1.0, 3.0))));
    ASSERT_TRUE(HCContourCurveCount(contour) == 3);
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 0), HCCurveMakeQuadratic(HCPointMake(1.0, 1.0), HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 1), HCCurveMakeLinear(HCPointMake(3.0, 1.0), HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 2), HCCurveMakeCubic(HCPointMake(3.0, 3.0), HCPointMake(2.5, 4.0), HCPointMake(1.5, 4.0), HCPointMake(1.0, 3.0))));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCContour, Equality) {
    HCContourComponent a[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    HCContour* contourA = HCContourInitInComponents(a, sizeof(a) / sizeof(HCContourComponent), true);
    HCContourComponent b[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    HCContour* contourB = HCContourInitInComponents(b, sizeof(b) / sizeof(HCContourComponent), true);
    HCContourComponent c[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    HCContour* contourC = HCContourInitInComponents(c, sizeof(c) / sizeof(HCContourComponent), false);
    
    ASSERT_TRUE(HCContourIsEqual(contourA, contourA));
    ASSERT_TRUE(HCContourHashValue(contourA) == HCContourHashValue(contourA));
    ASSERT_TRUE(HCContourIsEqual(contourA, contourB));
    ASSERT_TRUE(HCContourHashValue(contourA) == HCContourHashValue(contourB));
    ASSERT_TRUE(HCContourIsEqual(contourB, contourA));
    ASSERT_TRUE(HCContourHashValue(contourB) == HCContourHashValue(contourA));
    ASSERT_TRUE(!HCContourIsEqual(contourA, contourC));
}

CTEST(HCContour, Print) {
    HCContourComponent components[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 1.0, .y = 1.0}},
        {.c0 = {.x = 2.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 1.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 3.0, .y = 3.0}},
        {.c0 = {.x = 2.5, .y = 4.0}, .c1 = {.x = 1.5, .y = 4.0}, .p = {.x = 1.0, .y = 3.0}},
    };
    HCContour* contour = HCContourInitInComponents(components, sizeof(components) / sizeof(HCContourComponent), true);
    HCContourPrint(contour, stdout); // TODO: Not to stdout
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
// TODO: Test these!
//HCBoolean HCContourIsClosed(const HCContour* contour);
//HCPoint HCContourStartPoint(const HCContour* contour);
//HCPoint HCContourEndPoint(const HCContour* contour);
//void HCContourExtrema(const HCContour* contour, HCInteger* count, HCReal* extrema);
//void HCContourInflections(const HCContour* contour, HCInteger* count, HCReal* inflections);
//HCRectangle HCContourApproximateBounds(const HCContour* contour);
//HCRectangle HCContourBounds(const HCContour* contour);
//HCReal HCContourLength(const HCContour* contour);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Components
//----------------------------------------------------------------------------------------------------------------------------------
// TODO: Test these!
//HCInteger HCContourComponentCount(const HCContour* contour);
//HCContourComponent HCContourComponentAt(const HCContour* contour, HCInteger componentIndex);
//HCContourComponent HCContourComponentContaining(const HCContour* contour, HCReal t);
//HCInteger HCContourComponentIndexContaining(const HCContour* contour, HCReal t);
//HCReal HCContourComponentParameterFor(const HCContour* contour, HCReal t);
//const HCContourComponent* HCContourComponents(const HCContour* contour);
//HCInteger HCContourCurveCount(const HCContour* contour);
//HCCurve HCContourCurveAt(const HCContour* contour, HCInteger curveIndex);
//HCCurve HCContourCurveContaining(const HCContour* contour, HCReal t);
//HCInteger HCContourCurveIndexContaining(const HCContour* contour, HCReal t);
//HCReal HCContourCurveParameterFor(const HCContour* contour, HCReal t);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
// TODO: Test these!
//HCPoint HCContourValue(const HCContour* contour, HCReal t);
//HCCurve HCContourTangent(const HCContour* contour, HCReal t);
//HCCurve HCContourTangentUnit(const HCContour* contour, HCReal t);
//HCCurve HCContourNormal(const HCContour* contour, HCReal t);
//HCCurve HCContourNormalUnit(const HCContour* contour, HCReal t);
//HCReal HCContourCurvature(const HCContour* contour, HCReal t);
//HCCurve HCContourCurvatureNormal(const HCContour* contour, HCReal t);
//HCReal HCContourParameterAtLength(const HCContour* contour, HCReal d);
//HCReal HCContourParameterNearestPoint(const HCContour* contour, HCPoint p);
//HCReal HCContourDistanceFromPoint(const HCContour* contour, HCPoint p);
//void HCContourIntersection(const HCContour* pContour, const HCContour* qContour, HCInteger* count, HCReal* t, HCReal* u);
