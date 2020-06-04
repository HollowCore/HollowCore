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
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Test Data
//----------------------------------------------------------------------------------------------------------------------------------

HCContourComponent dinosaurComponents[] = {
    {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 495.720860, .y = 350.609350}},
    {.c0 = {.x = 492.189980, .y = 349.434380}, .c1 = {.x = 486.623580, .y = 349.342830}, .p = {.x = 476.461240, .y = 349.342830}},
    {.c0 = {.x = 475.963790, .y = 349.339830}, .c1 = {.x = 475.451110, .y = 349.339830}, .p = {.x = 474.923160, .y = 349.342830}},
    {.c0 = {.x = 435.494490, .y = 349.342830}, .c1 = {.x = 393.358920, .y = 346.498520}, .p = {.x = 390.594030, .y = 313.547790}},
    {.c0 = {.x = 379.259810, .y = 178.488580}, .c1 = {.x = 273.611130, .y = 166.391080}, .p = {.x = 228.835790, .y = 167.523320}},
    {.c0 = {.x = 218.331650, .y = 167.672870}, .c1 = {.x = 208.764410, .y = 162.841790}, .p = {.x = 202.834850, .y = 154.336310}},
    {.c0 = {.x = 196.899180, .y = 145.821680}, .c1 = {.x = 195.678490, .y = 135.433210}, .p = {.x = 199.389420, .y = 126.027440}},
    {.c0 = {.x = 199.792220, .y = 125.185130}, .c1 = {.x = 209.356450, .y = 105.155900}, .p = {.x = 212.295290, .y = 81.2081090}},
    {.c0 = {.x = 216.363280, .y = 48.0651200}, .c1 = {.x = 205.718760, .y = 23.7358470}, .p = {.x = 181.518350, .y = 10.8479220}},
    {.c0 = {.x = 152.493110, .y = -4.6035480}, .c1 = {.x = 121.148540, .y = -3.5323510}, .p = {.x = 90.8812480, .y = 13.9577500}},
    {.c0 = {.x = 86.5599580, .y = 16.4510990}, .c1 = {.x = 81.7015680, .y = 17.5070400}, .p = {.x = 76.9225190, .y = 17.0736780}},
    {.c0 = {.x = 61.6606990, .y = 15.6240520}, .c1 = {.x = 33.4411240, .y = 15.6332080}, .p = {.x = 15.6371880, .y = 31.8445860}},
    {.c0 = {.x = 5.26122500, .y = 41.2869800}, .c1 = {.x = 0.00000000, .y = 54.6509940}, .p = {.x = 0.00000000, .y = 71.5642890}},
    {.c0 = {.x = 0.00000000, .y = 88.6729090}, .c1 = {.x = 5.78612500, .y = 102.876180}, .p = {.x = 17.1996860, .y = 113.774300}},
    {.c0 = {.x = 37.5640440, .y = 133.226730}, .c1 = {.x = 70.3307340, .y = 136.641740}, .p = {.x = 85.3697780, .y = 137.136140}},
    {.c0 = {.x = 77.9357040, .y = 175.448950}, .c1 = {.x = 78.1615290, .y = 207.221660}, .p = {.x = 78.1767940, .y = 208.579730}},
    {.c0 = {.x = 78.1767940, .y = 241.108540}, .c1 = {.x = 85.6001830, .y = 291.351000}, .p = {.x = 101.114160, .y = 331.910720}},
    {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 89.6941080, .y = 437.479990}},
    {.c0 = {.x = 89.2546580, .y = 443.702700}, .c1 = {.x = 91.4458230, .y = 449.891830}, .p = {.x = 95.6999680, .y = 454.457380}},
    {.c0 = {.x = 99.9541130, .y = 459.022940}, .c1 = {.x = 105.975220, .y = 461.641410}, .p = {.x = 112.216060, .y = 461.641410}},
    {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 123.227170, .y = 461.641410}},
    {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 122.253270, .y = 475.841630}},
    {.c0 = {.x = 121.816870, .y = 482.067390}, .c1 = {.x = 124.008030, .y = 488.256520}, .p = {.x = 128.262180, .y = 492.819020}},
    {.c0 = {.x = 132.516330, .y = 497.381520}, .c1 = {.x = 138.534380, .y = 500.000000}, .p = {.x = 144.775220, .y = 500.000000}},
    {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 191.015400, .y = 500.000000}},
    {.c0 = {.x = 197.256240, .y = 500.000000}, .c1 = {.x = 203.274300, .y = 497.381520}, .p = {.x = 207.528450, .y = 492.815970}},
    {.c0 = {.x = 211.782590, .y = 488.253460}, .c1 = {.x = 213.973750, .y = 482.064330}, .p = {.x = 213.546500, .y = 476.094930}},
    {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 211.233270, .y = 397.384910}},
    {.c0 = {.x = 218.518960, .y = 399.243480}, .c1 = {.x = 226.402400, .y = 400.332990}, .p = {.x = 234.482300, .y = 400.736210}},
    {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 235.363500, .y = 438.233800}},
    {.c0 = {.x = 236.312600, .y = 451.140030}, .c1 = {.x = 247.195150, .y = 461.641410}, .p = {.x = 259.624950, .y = 461.641410}},
    {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 268.870230, .y = 461.641410}},
    {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 269.317280, .y = 475.417420}},
    {.c0 = {.x = 270.785200, .y = 491.268680}, .c1 = {.x = 280.471460, .y = 500.000000}, .p = {.x = 296.587780, .y = 500.000000}},
    {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 329.717630, .y = 500.000000}},
    {.c0 = {.x = 338.344940, .y = 500.000000}, .c1 = {.x = 345.336500, .y = 497.289960}, .p = {.x = 349.935490, .y = 492.159820}},
    {.c0 = {.x = 354.229310, .y = 487.374530}, .c1 = {.x = 356.060370, .y = 480.846650}, .p = {.x = 355.236390, .y = 473.314710}},
    {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 350.112490, .y = 424.088510}},
    {.c0 = {.x = 418.392450, .y = 447.908130}, .c1 = {.x = 478.893480, .y = 400.912840}, .p = {.x = 499.358540, .y = 359.297930}},
    {.c0 = {.x = 500.155090, .y = 357.677410}, .c1 = {.x = 500.213040, .y = 355.791370}, .p = {.x = 499.514190, .y = 354.125070}},
    {.c0 = {.x = 498.818400, .y = 352.461810}, .c1 = {.x = 497.435950, .y = 351.176990}, .p = {.x = 495.720860, .y = 350.609350}},
};

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

CTEST(HCContour, Attributes) {
    HCInteger contourComponentCount = sizeof(dinosaurComponents) / sizeof(HCContourComponent);
    HCContourComponent contourComponents[contourComponentCount];
    memcpy(contourComponents, dinosaurComponents, contourComponentCount * sizeof(HCContourComponent));
    HCContour* contour = HCContourInitInComponents(contourComponents, contourComponentCount, true);
    
    ASSERT_TRUE(HCContourIsClosed(contour));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(contour), contourComponents[0].p));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(contour), contourComponents[contourComponentCount - 1].p));
    ASSERT_TRUE(HCRectangleIsSimilar(HCContourApproximateBounds(contour), HCRectangleMakeWithComponents(0.0, -5.0, 500.0, 505.0), 1.0));
    ASSERT_TRUE(HCRectangleIsSimilar(HCContourBounds(contour), HCRectangleMakeWithComponents(0.0, 0.0, 500.0, 500.0), 1.0));
    ASSERT_TRUE(HCRealIsSimilar(HCContourLength(contour), 1935.0, 1.0));
    
    HCInteger extremaCount = contourComponentCount * 6;
    HCReal extrema[extremaCount];
    HCContourExtrema(contour, &extremaCount, extrema);
    ASSERT_TRUE(extremaCount == 38);
    
    HCInteger inflectionCount = contourComponentCount * 2;
    HCReal inflections[inflectionCount];
    HCContourInflections(contour, &inflectionCount, inflections);
    ASSERT_TRUE(inflectionCount == 0);
}

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
