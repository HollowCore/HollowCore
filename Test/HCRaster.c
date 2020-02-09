//
//  HCRaster.c
//  Test
//
//  Created by Matt Stoker on 2/3/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"

CTEST(HCRaster, Creation) {
    HCRasterRef raster = HCRasterCreate(1, 1);
    ASSERT_EQUAL(HCRasterWidth(raster), 1);
    ASSERT_EQUAL(HCRasterHeight(raster), 1);
    HCRelease(raster);
}

CTEST(HCRaster, EqualHash) {
    HCRasterRef a = HCRasterCreate(1, 1);
    HCRasterRef b = HCRasterCreate(1, 1);
    ASSERT_TRUE(HCObjectIsEqual((HCObjectRef)a, (HCObjectRef)a));
    ASSERT_FALSE(HCObjectIsEqual((HCObjectRef)a, (HCObjectRef)b));
    ASSERT_EQUAL(HCObjectHashValue((HCObjectRef)a), HCObjectHashValue((HCObjectRef)a));
    ASSERT_TRUE(HCIsEqual(a, a));
    ASSERT_TRUE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCRaster, Print) {
    HCRasterRef raster = HCRasterCreate(1, 1);
    HCObjectPrint((HCObjectRef)raster, stdout); // TODO: Not to stdout
    HCPrint(raster, stdout); // TODO: Not to stdout
    HCRelease(raster);
}

CTEST(HCRaster, PixelOperations) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 50).g == HCColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 49, 50).g == HCColorGreen.g);
    HCRasterSetPixelAt(raster, 50, 50, HCColorGreen);
    ASSERT_TRUE(HCRasterPixelAt(raster, 50, 50).g == HCColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 49, 50).g == HCColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 51, 50).g == HCColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 49).g == HCColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 51).g == HCColorGreen.g);
    HCColor pixels[25*25];
    for (HCInteger i = 0; i < (HCInteger)(sizeof(pixels) / sizeof(HCColor)); i++) {
        pixels[i] = HCColorRed;
    }
    HCRasterSetPixelsAt(raster, 30, 40, 55, 65, pixels);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 50).g == HCColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 49, 50).g == HCColorGreen.g);
    ASSERT_TRUE(HCRasterPixelAt(raster, 50, 50).r == HCColorRed.r);
    ASSERT_TRUE(HCRasterPixelAt(raster, 49, 50).r == HCColorRed.r);
    ASSERT_TRUE(HCRasterPixelAt(raster, 30, 50).r == HCColorRed.r);
    ASSERT_TRUE(HCRasterPixelAt(raster, 54, 50).r == HCColorRed.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 29, 50).r == HCColorRed.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 55, 50).r == HCColorRed.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 39).r == HCColorRed.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 65).r == HCColorRed.r);
    HCRasterSetPixelAt(raster, 30, 40, HCColorBlue);
    ASSERT_TRUE(HCRasterPixelAt(raster, 30, 40).b == HCColorBlue.b);
    HCRasterPixelsAt(raster, 30, 40, 55, 65, pixels);
    ASSERT_TRUE(pixels[0].b == HCColorBlue.b);
    ASSERT_FALSE(pixels[1].b == HCColorBlue.b);
    ASSERT_FALSE(pixels[1*(55-30)+0].b == HCColorBlue.b);
    HCRasterClear(raster);
    ASSERT_FALSE(HCRasterPixelAt(raster, 30, 40).b == HCColorBlue.b);
    HCRelease(raster);
}

CTEST(HCRaster, Lookup) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(raster); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(raster); xIndex++) {
            HCRasterSetPixelAt(raster, xIndex, yIndex, yIndex < 50 ? (xIndex < 50 ? HCColorRed : HCColorGreen) : (xIndex < 50 ? HCColorWhite : HCColorBlue));
        }
    }
    ASSERT_TRUE(HCRasterPixelNearest(raster, 50.5, 50.5).a == HCRasterPixelFiltered(raster, 50.5, 50.5).a);
    ASSERT_TRUE(HCRasterPixelNearest(raster, 50.5, 50.5).r == HCRasterPixelFiltered(raster, 50.5, 50.5).r);
    ASSERT_TRUE(HCRasterPixelNearest(raster, 50.5, 50.5).g == HCRasterPixelFiltered(raster, 50.5, 50.5).g);
    ASSERT_TRUE(HCRasterPixelNearest(raster, 50.5, 50.5).b == HCRasterPixelFiltered(raster, 50.5, 50.5).b);
    HCRelease(raster);
}

CTEST(HCRaster, DrawPoint) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 50).r == HCColorWhite.r);
    HCRasterDrawPoint(raster, 50, 50, HCColorWhite);
    ASSERT_TRUE(HCRasterPixelAt(raster, 50, 50).r == HCColorWhite.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 49, 50).r == HCColorWhite.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 51, 50).r == HCColorWhite.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 49).r == HCColorWhite.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 51).r == HCColorWhite.r);
    HCRasterSaveBMP(raster, "point.bmp");
    HCRasterSavePPM(raster, "point.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLine) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawLine(raster, 10, 80, 90, 20, HCColorYellow, HCColorMagenta);
    HCRasterSaveBMP(raster, "line.bmp");
    HCRasterSavePPM(raster, "line.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurve) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawQuadraticCurve(raster, 10, 90, 50, 10, 90, 90, HCColorMagenta, HCColorCyan);
    HCRasterSaveBMP(raster, "quadratic_curve.bmp");
    HCRasterSavePPM(raster, "quadratic_curve.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurve) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawCubicCurve(raster, 10, 90, 30, 10, 70, 10, 90, 90, HCColorCyan, HCColorYellow);
    HCRasterSaveBMP(raster, "cubic_curve.bmp");
    HCRasterSavePPM(raster, "cubic_curve.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawHorizontalPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 H 50", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_horizontal.bmp");
    HCRasterSavePPM(raster, "path_horizontal.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawHorizontalRelativePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 h 40", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_horizontal_relative.bmp");
    HCRasterSavePPM(raster, "path_horizontal_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawHorizontalPolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 H 25 50 75 90", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_horizontal_poly.bmp");
    HCRasterSavePPM(raster, "path_horizontal_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawHorizontalRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 h 15 25 25 15", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_horizontal_relative_poly.bmp");
    HCRasterSavePPM(raster, "path_horizontal_relative_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawVerticalPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 V 50", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_vertical.bmp");
    HCRasterSavePPM(raster, "path_vertical.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawVerticalRelativePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 v 30", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_vertical_relative.bmp");
    HCRasterSavePPM(raster, "path_vertical_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawVerticalPolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 V 25 50 75 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_vertical_poly.bmp");
    HCRasterSavePPM(raster, "path_vertical_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawVerticalRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 v 5 25 25 5", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_vertical_relative_poly.bmp");
    HCRasterSavePPM(raster, "path_vertical_relative_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLinePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 L 60 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_line.bmp");
    HCRasterSavePPM(raster, "path_line.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLineRelativePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 l 50 60", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_line_relative.bmp");
    HCRasterSavePPM(raster, "path_line_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLinePolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 L 60 80 50 40 90 20", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_line_poly.bmp");
    HCRasterSavePPM(raster, "path_line_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLineRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 l 50 60 -10 -40 40 -20", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_line_relative_poly.bmp");
    HCRasterSavePPM(raster, "path_line_relative_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurvePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 Q 95 10 180 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_curve.bmp");
    HCRasterSavePPM(raster, "path_quadratic_curve.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurveRelativePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 q 85 -70 170 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_curve_relative.bmp");
    HCRasterSavePPM(raster, "path_quadratic_curve_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurvePolyPath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 Q 95 10 180 80 95 150 10 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_curve_poly.bmp");
    HCRasterSavePPM(raster, "path_quadratic_curve_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurveRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 q 85 -70 170 0 -85 70 -170 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_curve_relative_poly.bmp");
    HCRasterSavePPM(raster, "path_quadratic_curve_relative_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticSmoothCurvePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 Q 52.5 10 95 80 T 180 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_smooth_curve.bmp");
    HCRasterSavePPM(raster, "path_quadratic_smooth_curve.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticSmoothCurveRelativePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 Q 52.5 10 95 80 t 85 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_smooth_curve_relative.bmp");
    HCRasterSavePPM(raster, "path_quadratic_smooth_curve_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticSmoothCurvePolyPath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 Q 52.5 10 95 80 T 180 80 95 80 10 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_smooth_curve_poly.bmp");
    HCRasterSavePPM(raster, "path_quadratic_smooth_curve_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticSmoothCurveRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 Q 52.5 10 95 80 t 85 0 -85 0 -85 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_smooth_curve_relative_poly.bmp");
    HCRasterSavePPM(raster, "path_quadratic_smooth_curve_relative_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurvePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 90 C 30 10 70 10 90 90", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_curve.bmp");
    HCRasterSavePPM(raster, "path_cubic_curve.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurveRelativePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 90 c 20 -80 60 -80 80 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_curve_relative.bmp");
    HCRasterSavePPM(raster, "path_cubic_curve_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurvePolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 90 C 30 10 70 10 90 90 70 40 30 40 10 90", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_curve_poly.bmp");
    HCRasterSavePPM(raster, "path_cubic_curve_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurveRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 90 c 20 -80 60 -80 80 0 -20 -50 -60 -50 -80 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_curve_relative_poly.bmp");
    HCRasterSavePPM(raster, "path_cubic_curve_relative_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicSmoothCurvePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 C 40 10 65 10 95 80 S 150 150 180 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_smooth_curve.bmp");
    HCRasterSavePPM(raster, "path_cubic_smooth_curve.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicSmoothCurveRelativePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 C 40 10 65 10 95 80 s 55 70 85 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_smooth_curve_relative.bmp");
    HCRasterSavePPM(raster, "path_cubic_smooth_curve_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicSmoothCurvePolyPath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 C 40 10 65 10 95 80 S 150 150 180 80 80 10 95 80 10 150 10 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_smooth_curve_poly.bmp");
    HCRasterSavePPM(raster, "path_cubic_smooth_curve_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicSmoothCurveRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 C 40 10 65 10 95 80 s 55 70 85 0 -100 -70 -85 0 -85 70 -85 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_smooth_curve_relative_poly.bmp");
    HCRasterSavePPM(raster, "path_cubic_smooth_curve_relative_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcPath) {
    HCRasterRef raster = HCRasterCreate(320, 320);
    HCRasterDrawPathData(raster, "M 10 315 L 110 215 A 30 50 0 0 1 162.55 162.45 L 315 10", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc.bmp");
    HCRasterSavePPM(raster, "path_arc.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcRelativePath) {
    HCRasterRef raster = HCRasterCreate(320, 320);
    HCRasterDrawPathData(raster, "M 10 315 L 110 215 a 30 50 0 0 1 52.55 -52.55 L 315 10", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_relative.bmp");
    HCRasterSavePPM(raster, "path_arc_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcPolyPath) {
    HCRasterRef raster = HCRasterCreate(320, 320);
    HCRasterDrawPathData(raster, "M 10 315 L 110 215 A 30 50 0 0 1 162.55 162.45 30 50 0 0 1 162.55 162.45 L 315 10", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_poly.bmp");
    HCRasterSavePPM(raster, "path_arc_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(320, 320);
    HCRasterDrawPathData(raster, "M 10 315 L 110 215 a 30 50 0 0 1 52.55 -52.55 a 30 50 0 0 1 52.55 -52.55 L 315 10", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_relative_poly.bmp");
    HCRasterSavePPM(raster, "path_arc_relative_poly.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 90 C 30 10 70 10 90 90 Z L 30 10 H 70 L 90 90 Q 50 10 10 90 Z", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path.bmp");
    HCRasterSavePPM(raster, "path.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawManyQuadratics) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCRasterDrawPathData(raster,
        "M 300 500 Q 500  1500 700 500 "
        "M 280 500 Q 500  1400 720 500 "
        "M 260 500 Q 500  1300 740 500 "
        "M 240 500 Q 500  1200 760 500 "
        "M 220 500 Q 500  1100 780 500 "
        "M 200 500 Q 500  1000 800 500 "
        "M 180 500 Q 500   900 820 500 "
        "M 160 500 Q 500   800 840 500 "
        "M 140 500 Q 500   700 860 500 "
        "M 120 500 Q 500   600 880 500 "
        "M 100 500 Q 500   500 900 500 "
        "M 120 500 Q 500   400 880 500 "
        "M 140 500 Q 500   300 860 500 "
        "M 160 500 Q 500   200 840 500 "
        "M 180 500 Q 500   100 820 500 "
        "M 200 500 Q 500     0 800 500 "
        "M 220 500 Q 500  -100 780 500 "
        "M 240 500 Q 500  -200 760 500 "
        "M 260 500 Q 500  -300 740 500 "
        "M 280 500 Q 500  -400 720 500 "
        "M 300 500 Q 500  -500 700 500 "
        , HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_many_quadratics.bmp");
    HCRasterSavePPM(raster, "path_many_quadratics.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawManyCubics) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCRasterDrawPathData(raster,
        "M 300 500 C 500  1500 500  1500 700 500 "
        "M 280 500 C 500  1400 500  1400 720 500 "
        "M 260 500 C 500  1300 500  1300 740 500 "
        "M 240 500 C 500  1200 500  1200 760 500 "
        "M 220 500 C 500  1100 500  1100 780 500 "
        "M 200 500 C 500  1000 500  1000 800 500 "
        "M 180 500 C 500   900 500   900 820 500 "
        "M 160 500 C 500   800 500   800 840 500 "
        "M 140 500 C 500   700 500   700 860 500 "
        "M 120 500 C 500   600 500   600 880 500 "
        "M 100 500 C 500   500 500   500 900 500 "
        "M 120 500 C 500   400 500   400 880 500 "
        "M 140 500 C 500   300 500   300 860 500 "
        "M 160 500 C 500   200 500   200 840 500 "
        "M 180 500 C 500   100 500   100 820 500 "
        "M 200 500 C 500     0 500     0 800 500 "
        "M 220 500 C 500  -100 500  -100 780 500 "
        "M 240 500 C 500  -200 500  -200 760 500 "
        "M 260 500 C 500  -300 500  -300 740 500 "
        "M 280 500 C 500  -400 500  -400 720 500 "
        "M 300 500 C 500  -500 500  -500 700 500 "
        , HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_many_cubics.bmp");
    HCRasterSavePPM(raster, "path_many_cubics.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawDinosaur) {
    const char* dinosaur = "m 134.51827,42.745763 c -16.01255,0 -29.04048,13.025256 -29.04048,29.035206 0,3.45164 2.79845,6.25017 6.24999,6.25017 3.45153,0 6.24999,-2.79853 6.24999,-6.25017 0,-9.115845 7.41881,-16.534865 16.5405,-16.534865 9.1217,0 16.54052,7.41902 16.54052,16.534865 0,3.45164 2.79845,6.25017 6.24999,6.25017 3.45153,0 6.24999,-2.79853 6.24999,-6.25017 0,-16.00995 -13.02794,-29.035206 -29.0405,-29.035206 z M 495.72086,350.60935 c -3.53088,-1.17497 -9.09728,-1.26652 -19.25962,-1.26652 -0.49745,-0.003 -1.01013,-0.003 -1.53808,0 -39.42867,0 -81.56424,-2.84431 -84.32913,-35.79504 -11.33422,-135.05921 -116.9829,-147.15671 -161.75824,-146.02447 -10.50414,0.14955 -20.07138,-4.68153 -26.00094,-13.18701 -5.93567,-8.51463 -7.15636,-18.9031 -3.44543,-28.30887 0.4028,-0.84231 9.96703,-20.87154 12.90587,-44.819331 C 216.36328,48.06512 205.71876,23.735847 181.51835,10.847922 152.49311,-4.6035481 121.14854,-3.5323515 90.881248,13.95775 86.559958,16.451099 81.701568,17.50704 76.922519,17.073678 61.660699,15.624052 33.441124,15.633208 15.637188,31.844586 5.2612254,41.28698 0,54.650994 0,71.564289 c 0,17.10862 5.7861249,31.311891 17.199686,42.210011 20.364358,19.45243 53.131048,22.86744 68.170092,23.36184 -7.434074,38.31281 -7.208249,70.08552 -7.192984,71.44359 0,32.52881 7.423389,82.77127 22.937366,123.33099 L 89.694108,437.47999 c -0.43945,6.22271 1.751715,12.41184 6.00586,16.97739 4.254145,4.56556 10.275252,7.18403 16.516092,7.18403 h 11.01111 l -0.9739,14.20022 c -0.4364,6.22576 1.75476,12.41489 6.00891,16.97739 4.25415,4.5625 10.2722,7.18098 16.51304,7.18098 h 46.24018 c 6.24084,0 12.2589,-2.61848 16.51305,-7.18403 4.25414,-4.56251 6.4453,-10.75164 6.01805,-16.72104 l -2.31323,-78.71002 c 7.28569,1.85857 15.16913,2.94808 23.24903,3.3513 l 0.8812,37.49759 c 0.9491,12.90623 11.83165,23.40761 24.26145,23.40761 h 9.24528 l 0.44705,13.77601 C 270.7852,491.26868 280.47146,500 296.58778,500 h 33.12985 c 8.62731,0 15.61887,-2.71004 20.21786,-7.84018 4.29382,-4.78529 6.12488,-11.31317 5.3009,-18.84511 l -5.1239,-49.2262 c 68.27996,23.81962 128.78099,-23.17567 149.24605,-64.79058 0.79655,-1.62052 0.8545,-3.50656 0.15565,-5.17286 -0.69579,-1.66326 -2.07824,-2.94808 -3.79333,-3.51572 z m -383.5048,98.53172 c -2.82593,0 -5.44433,-1.13833 -7.37304,-3.20748 -1.92566,-2.0661 -2.87781,-4.75782 -2.70081,-7.34273 l 9.08813,-83.98627 c 5.21583,9.92077 11.06757,18.67612 17.5781,25.66254 l -4.72373,68.87394 z m 147.40889,0 c -5.8197,0 -11.33422,-5.52383 -11.78283,-11.51154 l -0.85907,-36.83534 c 6.66847,-0.28345 13.34876,-1.02503 19.83793,-2.28926 l 1.64337,50.63614 z m 85.74819,-40.3728 c -2.03552,-0.90639 -4.40979,-0.66225 -6.21338,0.647 -1.80969,1.31229 -2.78014,3.4852 -2.54821,5.70694 l 6.19506,59.52004 c 0.4364,3.98876 -0.296,7.07417 -2.17895,9.17383 -2.72827,3.04574 -7.44018,3.68358 -10.91002,3.68358 h -33.12985 c -9.65881,0 -13.95262,-3.83617 -14.80101,-12.86046 l -2.71911,-84.11178 c -0.0613,-1.9013 -0.98572,-3.67136 -2.5116,-4.8097 -1.08947,-0.81179 -2.40173,-1.23905 -3.73534,-1.23905 -0.531,0 -1.06507,0.0671 -1.58692,0.2045 -21.36532,5.60929 -46.02655,4.95619 -64.3737,-1.69377 -1.94092,-0.70497 -4.10766,-0.40285 -5.79223,0.80874 -1.67846,1.21463 -2.64587,3.18001 -2.58483,5.24916 l 2.58178,87.67023 c 0.19835,2.8199 -0.75378,5.50857 -2.67944,7.57772 -1.92566,2.06607 -4.54406,3.2044 -7.36999,3.2044 h -46.24018 c -2.82592,0 -5.44433,-1.13833 -7.36999,-3.20443 -1.92871,-2.0661 -2.88085,-4.75782 -2.68249,-7.58993 l 6.74743,-98.35472 c 0.1251,-1.80669 -0.54019,-3.57675 -1.8219,-4.85548 -10.29509,-10.258 -19.01624,-26.00664 -26.14056,-44.22536 -0.04,-0.1453 -0.10415,-0.2724 -0.15415,-0.4135 -14.885312,-38.27199 -22.676822,-87.41654 -22.676822,-120.33751 -0.003,-0.35705 -0.2441,-36.12159 8.46252,-76.29297 0.40895,-1.8952 -0.08545,-3.87279 -1.33362,-5.35598 -1.251225,-1.48014 -3.091425,-2.24921 -5.056755,-2.21259 -0.4303,0.0245 -44.314526,1.70599 -66.943288,-19.94683 C 16.851762,96.143829 12.49996,85.303689 12.49996,71.564299 c 0,-13.24804 3.884883,-23.502225 11.550894,-30.475677 14.251689,-12.973377 38.510085,-12.811629 51.693665,-11.569527 7.406604,0.717182 14.813214,-0.933869 21.386694,-4.73646 26.342737,-15.2195321 53.497247,-16.2235841 78.512477,-2.899247 47.17707,25.116651 12.83568,97.982392 12.48473,98.714832 -0.0641,0.13425 -0.21055,0.48835 -0.2655,0.62563 -5.29784,13.37011 -3.53698,28.4218 4.71801,40.26294 8.33434,11.95711 21.85362,18.93972 36.55697,18.53077 51.29694,-1.15054 138.93111,14.59086 148.99885,134.57397 3.96728,47.30048 64.54155,47.19061 97.21363,47.25165 2.74048,-0.01 5.67626,-0.003 8.28857,0.0395 -22.85764,36.89981 -77.94791,73.78435 -138.26583,46.88543 z";
//    const char* dinosaur = "M 199.38942,126.02744 C 199.79222,125.18513 209.35645,105.15590 212.29529,81.20811";
    HCRasterRef raster = HCRasterCreate(500, 500);
    HCRasterDrawPathData(raster, dinosaur, HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_dinosaur.bmp");
    HCRasterSavePPM(raster, "path_dinosaur.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawIntersection) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCPathRef path = HCPathCreate(
        "M 200 500 C 500  1000 500  1000 800 500 Z "
        "M 200 500 C 500     0 500     0 800 500 Z "
    );
    HCPathRef rectangle = HCPathCreate("M 100 300 L 900 300 900 700 100 700 Z");
    for (HCInteger y = 0; y < HCRasterHeight(raster); y++) {
        for (HCInteger x = 0; x < HCRasterWidth(raster); x++) {
            HCPoint point = HCPointMake(x + 0.5, y + 0.5);
            HCBoolean pathContainsPoint = HCPathContainsPoint(path, point);
            HCBoolean rectangleContainsPoint = HCPathContainsPoint(rectangle, point);
            HCRasterSetPixelAt(raster, x, y, pathContainsPoint ? (rectangleContainsPoint ? HCColorGreen : HCColorBlue) : (rectangleContainsPoint ? HCColorYellow : HCColorBlack));
        }
    }
    HCRasterSaveBMP(raster, "path_intersection.bmp");
    HCRasterSavePPM(raster, "path_intersection.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawIntersectionNonZero) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCPathRef path = HCPathCreate(
        "M 200 500 C 500  1000 500  1000 800 500 Z "
        "M 200 500 C 500     0 500     0 800 500 Z "
    );
    HCPathRef rectangle = HCPathCreate("M 100 300 L 900 300 900 700 100 700 Z");
    for (HCInteger y = 0; y < HCRasterHeight(raster); y++) {
        for (HCInteger x = 0; x < HCRasterWidth(raster); x++) {
            HCPoint point = HCPointMake(x + 0.5, y + 0.5);
            HCBoolean pathContainsPoint = HCPathContainsPointNonZero(path, point);
            HCBoolean rectangleContainsPoint = HCPathContainsPointNonZero(rectangle, point);
            HCRasterSetPixelAt(raster, x, y, pathContainsPoint ? (rectangleContainsPoint ? HCColorGreen : HCColorBlue) : (rectangleContainsPoint ? HCColorYellow : HCColorBlack));
        }
    }
    HCRasterSaveBMP(raster, "path_intersection.bmp");
    HCRasterSavePPM(raster, "path_intersection.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawTriangle) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawTriangle(raster, 10, 80, 50, 20, 90, 70, HCColorRed, HCColorGreen, HCColorBlue);
    HCRasterSaveBMP(raster, "triangle.bmp");
    HCRasterSavePPM(raster, "triangle.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, FillTriangle) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillTriangle(raster, 10, 80, 50, 20, 90, 70, HCColorRed, HCColorGreen, HCColorBlue);
    HCRasterSaveBMP(raster, "triangle_filled.bmp");
    HCRasterSavePPM(raster, "triangle_filled.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, FillTexturedTriangle) {
    HCRasterRef texture = HCRasterCreate(100, 100);
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(texture); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(texture); xIndex++) {
            HCRasterSetPixelAt(texture, xIndex, yIndex, yIndex < 50 ? (xIndex < 50 ? HCColorRed : HCColorGreen) : (xIndex < 50 ? HCColorWhite : HCColorBlue));
        }
    }
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillTexturedTriangle(raster, 10, 80, 50, 20, 90, 70, texture, 0, 0, 100, 0, 0, 100);
    HCRasterSaveBMP(texture, "triangle_texture.bmp");
    HCRasterSavePPM(texture, "triangle_texture.ppm");
    HCRasterSaveBMP(raster, "triangle_textured.bmp");
    HCRasterSavePPM(raster, "triangle_textured.ppm");
    HCRelease(raster);
    HCRelease(texture);
}

CTEST(HCRaster, DrawQuad) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawQuad(raster, 30, 20, 80, 10, 90, 90, 10, 80, HCColorRed, HCColorGreen, HCColorBlue, HCColorWhite);
    HCRasterSaveBMP(raster, "quad.bmp");
    HCRasterSavePPM(raster, "quad.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, FillQuad) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillQuad(raster, 30, 20, 80, 10, 90, 90, 10, 80, HCColorRed, HCColorGreen, HCColorBlue, HCColorWhite);
    HCRasterSaveBMP(raster, "quad_filled.bmp");
    HCRasterSavePPM(raster, "quad_filled.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, FillTexturedQuad) {
    HCRasterRef texture = HCRasterCreate(100, 100);
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(texture); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(texture); xIndex++) {
            HCRasterSetPixelAt(texture, xIndex, yIndex, yIndex < 50 ? (xIndex < 50 ? HCColorRed : HCColorGreen) : (xIndex < 50 ? HCColorWhite : HCColorBlue));
        }
    }
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillTexturedQuad(raster, 30, 20, 80, 10, 90, 90, 10, 80, texture, 0, 0, 100, 0, 100, 100, 0, 100);
    HCRasterSaveBMP(texture, "quad_texture.bmp");
    HCRasterSavePPM(texture, "quad_texture.ppm");
    HCRasterSaveBMP(raster, "quad_textured.bmp");
    HCRasterSavePPM(raster, "quad_textured.ppm");
    HCRelease(raster);
    HCRelease(texture);
}

CTEST(HCRaster, Gradient) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillQuad(raster, -50, 50, 50, -50, 150, 50, 50, 150, HCColorRed, HCColorRed, HCColorGreen, HCColorGreen);
    HCRasterSaveBMP(raster, "gradient.bmp");
    HCRasterSavePPM(raster, "gradient.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, ExtractARGB8888) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterSetPixelAt(raster, 10, 20, HCColorWhite);
    uint32_t pixels[100*100];
    HCRasterExtractARGB8888(raster, pixels);
    ASSERT_TRUE(pixels[0] == 0);
    ASSERT_TRUE(pixels[20*100+10] == 0xFFFFFFFF);
    HCRelease(raster);
}

CTEST(HCRaster, SaveLoad) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(raster); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(raster); xIndex++) {
            HCRasterSetPixelAt(raster, xIndex, yIndex, yIndex < 50 ? (xIndex < 50 ? HCColorRed : HCColorGreen) : (xIndex < 50 ? HCColorWhite : HCColorBlue));
        }
    }

    HCRasterSavePPM(raster, "test_ppm.ppm");
    HCRasterRef ppm = HCRasterCreateByLoadingPPM("test_ppm.ppm");
    ASSERT_TRUE(HCRasterIsEqual(raster, ppm));
    HCRelease(ppm);
    
    HCRasterSavePPMWithOptions(raster, "test_ppm_binary.ppm", true);
    HCRasterRef ppmBinary = HCRasterCreateByLoadingPPM("test_ppm_binary.ppm");
    ASSERT_TRUE(HCRasterIsEqual(raster, ppmBinary));
    HCRelease(ppmBinary);
    
    HCRasterSaveBMP(raster, "test_bmp.bmp");
    HCRasterRef bmp = HCRasterCreateByLoadingBMP("test_bmp.bmp");
    ASSERT_TRUE(HCRasterIsEqual(raster, bmp));
    HCRelease(bmp);
    
    HCRasterSaveBMPWithOptions(raster, "test_bmp_reversed.bmp", true);
    HCRasterRef bmpReversed = HCRasterCreateByLoadingBMP("test_bmp_reversed.bmp");
    ASSERT_TRUE(HCRasterIsEqual(raster, bmpReversed));
    HCRelease(bmpReversed);
    
    HCRelease(raster);
}
