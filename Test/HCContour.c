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

HCContourComponent knightComponents[] = {
    {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 233.154600, .y = 499.852390}},
    {.c0 = {.x = 186.489170, .y = 501.367520}, .c1 = {.x = 133.293860, .y = 490.730200}, .p = {.x = 105.279900, .y = 449.572540}},
    {.c0 = {.x = 83.6968540, .y = 420.308090}, .c1 = {.x = 72.9051260, .y = 367.924030}, .p = {.x = 74.3015010, .y = 332.517340}},
    {.c0 = {.x = 76.6691650, .y = 301.304880}, .c1 = {.x = 80.6249430, .y = 290.258840}, .p = {.x = 67.2347780, .y = 262.220580}},
    {.c0 = {.x = 53.8446150, .y = 234.182350}, .c1 = {.x = 23.1085100, .y = 189.151850}, .p = {.x = 22.7831810, .y = 154.980470}},
    {.c0 = {.x = 17.9141250, .y = 106.842320}, .c1 = {.x = 35.8156740, .y = 56.2028360}, .p = {.x = 74.6059450, .y = 26.0201730}},
    {.c0 = {.x = 93.8922300, .y = 10.7554500}, .c1 = {.x = 118.685370, .y = -1.5968120}, .p = {.x = 143.583580, .y = 0.16838000}},
    {.c0 = {.x = 162.484450, .y = 17.7334020}, .c1 = {.x = 111.534780, .y = 37.5494450}, .p = {.x = 140.507960, .y = 55.3881820}},
    {.c0 = {.x = 167.929270, .y = 75.6283860}, .c1 = {.x = 119.280470, .y = 83.3483890}, .p = {.x = 111.148800, .y = 102.004320}},
    {.c0 = {.x = 97.8493080, .y = 135.719660}, .c1 = {.x = 103.128640, .y = 176.962640}, .p = {.x = 127.610230, .y = 203.287770}},
    {.c0 = {.x = 148.579160, .y = 204.778680}, .c1 = {.x = 170.410970, .y = 198.778690}, .p = {.x = 191.843170, .y = 199.116140}},
    {.c0 = {.x = 245.221480, .y = 197.320660}, .c1 = {.x = 298.853700, .y = 200.737710}, .p = {.x = 351.215720, .y = 211.576010}},
    {.c0 = {.x = 375.277380, .y = 208.701460}, .c1 = {.x = 391.074880, .y = 183.856510}, .p = {.x = 398.801620, .y = 162.525660}},
    {.c0 = {.x = 409.364100, .y = 127.539670}, .c1 = {.x = 392.664200, .y = 87.6439140}, .p = {.x = 361.152360, .y = 69.5992160}},
    {.c0 = {.x = 344.987860, .y = 55.6752770}, .c1 = {.x = 387.746510, .y = 47.0575200}, .p = {.x = 360.248120, .y = 37.2588500}},
    {.c0 = {.x = 349.114390, .y = 30.2493240}, .c1 = {.x = 314.230410, .y = 4.10334300}, .p = {.x = 344.266730, .y = 0.21714800}},
    {.c0 = {.x = 397.339550, .y = -1.4695750}, .c1 = {.x = 450.817850, .y = 30.5818080}, .p = {.x = 469.250340, .y = 81.3076420}},
    {.c0 = {.x = 487.654610, .y = 128.981080}, .c1 = {.x = 487.199280, .y = 189.073470}, .p = {.x = 451.990250, .y = 229.155830}},
    {.c0 = {.x = 435.906060, .y = 250.001960}, .c1 = {.x = 413.686260, .y = 259.468610}, .p = {.x = 411.293310, .y = 289.643950}},
    {.c0 = {.x = 411.871870, .y = 335.016970}, .c1 = {.x = 414.729530, .y = 382.326870}, .p = {.x = 398.285710, .y = 425.518010}},
    {.c0 = {.x = 383.534280, .y = 466.355220}, .c1 = {.x = 343.074130, .y = 492.433840}, .p = {.x = 300.962290, .y = 496.869800}},
    {.c0 = {.x = 278.519200, .y = 500.323760}, .c1 = {.x = 255.792960, .y = 500.155080}, .p = {.x = 233.154600, .y = 499.852390}},
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
    HCInteger contourComponentCount = sizeof(knightComponents) / sizeof(HCContourComponent);
    HCContourComponent contourComponents[contourComponentCount];
    memcpy(contourComponents, knightComponents, contourComponentCount * sizeof(HCContourComponent));
    HCContour* contour = HCContourInitInComponents(contourComponents, contourComponentCount, true);
    
    ASSERT_TRUE(HCContourIsClosed(contour));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(contour), contourComponents[0].p));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(contour), contourComponents[contourComponentCount - 1].p));
    ASSERT_TRUE(HCRectangleIsSimilar(HCContourApproximateBounds(contour), HCRectangleMakeWithComponents(17.914124999999999, -1.5968119999999999, 469.74048499999998, 502.96433200000001), 1.0));
    ASSERT_TRUE(HCRectangleIsSimilar(HCContourBounds(contour), HCRectangleMakeWithComponents(22.000000204429124, 1.6939553126871232E-7, 459.28337246819666, 500.00000040638099), 1.0));
    ASSERT_TRUE(HCRealIsSimilar(HCContourLength(contour), 2142.6, 1.0));
    
    HCInteger extremaCount = contourComponentCount * 6;
    HCReal extrema[extremaCount];
    HCContourExtrema(contour, &extremaCount, extrema);
    ASSERT_TRUE(extremaCount == 48);
    
    HCInteger inflectionCount = contourComponentCount * 2;
    HCReal inflections[inflectionCount];
    HCContourInflections(contour, &inflectionCount, inflections);
    ASSERT_TRUE(inflectionCount == 8);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Components
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContour, Components) {
    HCInteger contourComponentCount = sizeof(knightComponents) / sizeof(HCContourComponent);
    HCContourComponent contourComponents[contourComponentCount];
    memcpy(contourComponents, knightComponents, contourComponentCount * sizeof(HCContourComponent));
    HCContour* contour = HCContourInitInComponents(contourComponents, contourComponentCount, true);
    
    ASSERT_TRUE(HCContourComponentCount(contour) == contourComponentCount);
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(contour, 10), contourComponents[10]));
    ASSERT_TRUE(HCContourComponentContainingParameter(contour, 0.25) == contourComponentCount / 4 + 1);
    ASSERT_TRUE(HCContourComponentParameterForParameter(contour, 0.25) == 0.25);
    ASSERT_TRUE(HCContourParameterForComponentParameter(contour, contourComponentCount / 4 + 1, 0.25) == 0.25);
    ASSERT_TRUE(HCContourComponents(contour) == contourComponents);
    
    for (HCInteger componentIndex = 0; componentIndex < HCContourComponentCount(contour); componentIndex++) {
        for (HCReal t = 0.0; t != 1.0; t = t + 0.001 < 1.0 ? t + 0.001 : 1.0) {
            if (HCContourComponentContainingParameter(contour, t) == componentIndex) {
                HCReal ct = HCContourParameterForComponentParameter(contour, componentIndex, HCContourComponentParameterForParameter(contour, t));
                ASSERT_TRUE(HCRealIsSimilar(ct, t, 0.000001));
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Components as Curves
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContour, ComponentsAsCurves) {
    HCContourComponent singleCurveContourComponents[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 10.0, .y = 20.0}},
        {.c0 = {.x = 20.0, .y = 10.0}, .c1 = {.x = 40.0, .y = 5.0}, .p = {.x = 50.0, .y = 25.0}}
    };
    HCContour* singleCurveContour = HCContourInitInComponents(singleCurveContourComponents, sizeof(singleCurveContourComponents) / sizeof(HCContourComponent), false);
    ASSERT_TRUE(HCContourCurveCount(singleCurveContour) == 1);
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(singleCurveContour, 0), HCCurveMakeCubic(singleCurveContourComponents[0].p, singleCurveContourComponents[1].c0, singleCurveContourComponents[1].c1, singleCurveContourComponents[1].p)));
    ASSERT_TRUE(HCContourCurveContainingParameter(singleCurveContour, 0.25) == 0);
    ASSERT_TRUE(HCContourCurveParameterForParameter(singleCurveContour, 0.25) == 0.25);
    ASSERT_TRUE(HCContourParameterForCurveParameter(singleCurveContour, 0, 0.25) == 0.25);
    
    HCInteger contourComponentCount = sizeof(knightComponents) / sizeof(HCContourComponent);
    HCContourComponent contourComponents[contourComponentCount];
    memcpy(contourComponents, knightComponents, contourComponentCount * sizeof(HCContourComponent));
    HCContour* contour = HCContourInitInComponents(contourComponents, contourComponentCount, true);
    
    ASSERT_TRUE(HCContourCurveCount(contour) == contourComponentCount - 1);
    ASSERT_TRUE(HCCurveIsEqual(HCContourCurveAt(contour, 10), HCCurveMakeCubic(contourComponents[10].p, contourComponents[11].c0, contourComponents[11].c1, contourComponents[11].p)));
    ASSERT_TRUE(HCContourCurveContainingParameter(contour, 0.25) == contourComponentCount / 4);
    ASSERT_TRUE(HCContourCurveParameterForParameter(contour, 0.25) == 0.25);
    ASSERT_TRUE(HCContourParameterForCurveParameter(contour, contourComponentCount / 4, 0.25) == 0.25);
    ASSERT_TRUE(HCContourCurveIndexForComponentIndex(contour, 10) == 9);
    ASSERT_TRUE(HCContourComponentIndexForCurveIndex(contour, 9) == 10);
    
    for (HCInteger curveIndex = 0; curveIndex < HCContourCurveCount(contour); curveIndex++) {
        for (HCReal t = 0.0; t != 1.0; t = t + 0.001 < 1.0 ? t + 0.001 : 1.0) {
            if (HCContourCurveContainingParameter(contour, t) == curveIndex) {
                HCReal ct = HCContourParameterForCurveParameter(contour, curveIndex, HCContourCurveParameterForParameter(contour, t));
                ASSERT_TRUE(HCRealIsSimilar(ct, t, 0.000001));
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCContour, Operations) {
    // NOTE: These tests provide only detection of a change in functionality, as the values were obtained by running the test itself
    
    HCInteger contourComponentCount = sizeof(knightComponents) / sizeof(HCContourComponent);
    HCContourComponent contourComponents[contourComponentCount];
    memcpy(contourComponents, knightComponents, contourComponentCount * sizeof(HCContourComponent));
    HCContour* contour = HCContourInitInComponents(contourComponents, contourComponentCount, true);
    
    ASSERT_TRUE(HCPointIsSimilar(HCContourValue(contour, 0.25), HCPointMake(90.0, 15.3), 0.1));
    ASSERT_TRUE(HCCurveIsSimilar(HCContourTangent(contour, 0.25), HCCurveMakeLinear(HCContourValue(contour, 0.25), HCPointMake(155.1, -24.0)), 0.1));
    ASSERT_TRUE(HCCurveIsSimilar(HCContourTangentUnit(contour, 0.25), HCCurveMakeLinear(HCContourValue(contour, 0.25), HCPointMake(90.9, 14.8)), 0.1));
    ASSERT_TRUE(HCCurveIsSimilar(HCContourNormal(contour, 0.25), HCCurveMakeLinear(HCContourValue(contour, 0.25), HCPointMake(129.3, 80.4)), 0.1));
    ASSERT_TRUE(HCCurveIsSimilar(HCContourNormalUnit(contour, 0.25), HCCurveMakeLinear(HCContourValue(contour, 0.25), HCPointMake(90.5, 16.1)), 0.1));
    ASSERT_TRUE(HCRealIsSimilar(HCContourCurvature(contour, 0.25), 0.00730, 0.00001));
    ASSERT_TRUE(HCCurveIsSimilar(HCContourCurvatureNormal(contour, 0.25), HCCurveMakeLinear(HCContourValue(contour, 0.25), HCPointMake(90.0, 15.3)), 0.1));
    ASSERT_TRUE(HCRealIsSimilar(HCContourParameterAtLength(contour, 1000.0), 0.48253, 0.00001));
    ASSERT_TRUE(HCRealIsSimilar(HCContourParameterNearestPoint(contour, HCPointMake(100.0, 100.0)), 0.38190, 0.00001));
    ASSERT_TRUE(HCRealIsSimilar(HCContourDistanceFromPoint(contour, HCPointMake(100.0, 100.0)), 11.13067, 0.00001));
    
    HCContourComponent qComponents[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = -10.0, .y = -10.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 510.0, .y = 510.0}},
    };
    HCContour* qContour = HCContourInitInComponents(qComponents, sizeof(qComponents) / sizeof(HCContourComponent), false);
    
    HCInteger intersectionCount = HCContourComponentCount(contour) * 9;
    HCReal t[intersectionCount];
    HCReal u[intersectionCount];
    HCContourIntersections(contour, qContour, &intersectionCount, t, u);
    ASSERT_TRUE(intersectionCount == 4);
    ASSERT_TRUE(HCRealIsSimilar(t[0], 0.22682, 0.00001));
    ASSERT_TRUE(HCRealIsSimilar(t[1], 0.38409, 0.00001));
    ASSERT_TRUE(HCRealIsSimilar(t[2], 0.47829, 0.00001));
    ASSERT_TRUE(HCRealIsSimilar(t[3], 0.89722, 0.00001));
    ASSERT_TRUE(HCRealIsSimilar(u[0], 0.11652, 0.00001));
    ASSERT_TRUE(HCRealIsSimilar(u[1], 0.22838, 0.00001));
    ASSERT_TRUE(HCRealIsSimilar(u[2], 0.40174, 0.00001));
    ASSERT_TRUE(HCRealIsSimilar(u[3], 0.79755, 0.00001));
}
