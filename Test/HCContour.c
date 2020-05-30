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
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 0), HCContourCurveMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 1), HCContourCurveMakeLinear(HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 2), HCContourCurveMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 3), HCContourCurveMakeLinear(HCPointMake(1.0, 3.0))));
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
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 0), HCContourCurveMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 1), HCContourCurveMakeQuadratic(HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 2), HCContourCurveMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 3), HCContourCurveMakeQuadratic(HCPointMake(3.0, 4.0), HCPointMake(1.0, 3.0))));
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
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 0), HCContourCurveMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 1), HCContourCurveMakeQuadratic(HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 2), HCContourCurveMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 3), HCContourCurveMakeCubic(HCPointMake(2.5, 4.0), HCPointMake(1.5, 4.0), HCPointMake(1.0, 3.0))));
    ASSERT_TRUE(HCContourCurveCount(contour) == 3);
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 0), HCCurveMakeQuadratic(HCPointMake(1.0, 1.0), HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 1), HCCurveMakeLinear(HCPointMake(3.0, 1.0), HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 2), HCCurveMakeCubic(HCPointMake(3.0, 3.0), HCPointMake(2.5, 4.0), HCPointMake(1.5, 4.0), HCPointMake(1.0, 3.0))));
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
    ASSERT_TRUE(HCContourComponentCount(contour) == 4);
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 0), HCContourCurveMakeLinear(HCPointMake(1.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 1), HCContourCurveMakeQuadratic(HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 2), HCContourCurveMakeLinear(HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCContourCurveIsEqual(HCContourComponentAt(contour, 3), HCContourCurveMakeCubic(HCPointMake(2.5, 4.0), HCPointMake(1.5, 4.0), HCPointMake(1.0, 3.0))));
    ASSERT_TRUE(HCContourCurveCount(contour) == 3);
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 0), HCCurveMakeQuadratic(HCPointMake(1.0, 1.0), HCPointMake(2.0, 0.0), HCPointMake(3.0, 1.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 1), HCCurveMakeLinear(HCPointMake(3.0, 1.0), HCPointMake(3.0, 3.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 2), HCCurveMakeCubic(HCPointMake(3.0, 3.0), HCPointMake(2.5, 4.0), HCPointMake(1.5, 4.0), HCPointMake(1.0, 3.0))));
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
