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
    HCRasterDrawPath(raster, "M 10 20 H 50", HCColorGreen);
    HCRasterSaveBMP(raster, "path_horizontal.bmp");
    HCRasterSavePPM(raster, "path_horizontal.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawHorizontalRelativePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPath(raster, "M 10 20 h 40", HCColorGreen);
    HCRasterSaveBMP(raster, "path_horizontal_relative.bmp");
    HCRasterSavePPM(raster, "path_horizontal_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawVerticalPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPath(raster, "M 10 20 V 50", HCColorGreen);
    HCRasterSaveBMP(raster, "path_vertical.bmp");
    HCRasterSavePPM(raster, "path_vertical.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawVerticalRelativePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPath(raster, "M 10 20 v 30", HCColorGreen);
    HCRasterSaveBMP(raster, "path_vertical_relative.bmp");
    HCRasterSavePPM(raster, "path_vertical_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLinePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPath(raster, "M 10 20 L 60 80", HCColorGreen);
    HCRasterSaveBMP(raster, "path_line.bmp");
    HCRasterSavePPM(raster, "path_line.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLineRelativePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPath(raster, "M 10 20 l 50 60", HCColorGreen);
    HCRasterSaveBMP(raster, "path_line_relative.bmp");
    HCRasterSavePPM(raster, "path_line_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurvePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPath(raster, "M 10 80 Q 95 10 180 80", HCColorGreen);
    HCRasterSaveBMP(raster, "path_quadratic_curve.bmp");
    HCRasterSavePPM(raster, "path_quadratic_curve.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurveRelativePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPath(raster, "M 10 80 q 85 -70 170 0", HCColorGreen);
    HCRasterSaveBMP(raster, "path_quadratic_curve_relative.bmp");
    HCRasterSavePPM(raster, "path_quadratic_curve_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticSmoothCurvePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPath(raster, "M 10 80 Q 52.5 10 95 80 T 180 80", HCColorGreen);
    HCRasterSaveBMP(raster, "path_quadratic_smooth_curve.bmp");
    HCRasterSavePPM(raster, "path_quadratic_smooth_curve.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticSmoothCurveRelativePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPath(raster, "M 10 80 Q 52.5 10 95 80 t 85 0", HCColorGreen);
    HCRasterSaveBMP(raster, "path_quadratic_smooth_curve_relative.bmp");
    HCRasterSavePPM(raster, "path_quadratic_smooth_curve_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurvePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPath(raster, "M 10 90 C 30 10 70 10 90 90", HCColorGreen);
    HCRasterSaveBMP(raster, "path_cubic_curve.bmp");
    HCRasterSavePPM(raster, "path_cubic_curve.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurveRelativePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPath(raster, "M 10 90 c 20 -80 60 -80 80 0", HCColorGreen);
    HCRasterSaveBMP(raster, "path_cubic_curve_relative.bmp");
    HCRasterSavePPM(raster, "path_cubic_curve_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicSmoothCurvePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPath(raster, "M 10 80 C 40 10 65 10 95 80 S 150 150 180 80", HCColorGreen);
    HCRasterSaveBMP(raster, "path_cubic_smooth_curve.bmp");
    HCRasterSavePPM(raster, "path_cubic_smooth_curve.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicSmoothCurveRelativePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPath(raster, "M 10 80 C 40 10 65 10 95 80 s 55 70 85 0", HCColorGreen);
    HCRasterSaveBMP(raster, "path_cubic_smooth_curve_relative.bmp");
    HCRasterSavePPM(raster, "path_cubic_smooth_curve_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcPath) {
    HCRasterRef raster = HCRasterCreate(320, 320);
    HCRasterDrawPath(raster, "M 10 315 L 110 215 A 30 50 0 0 1 162.55 162.45 L 315 10", HCColorGreen);
    HCRasterSaveBMP(raster, "path_arc.bmp");
    HCRasterSavePPM(raster, "path_arc.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcRelativePath) {
    HCRasterRef raster = HCRasterCreate(320, 320);
    HCRasterDrawPath(raster, "M 10 315 L 110 215 a 30 50 0 0 1 52.55 -52.55 L 315 10", HCColorGreen);
    HCRasterSaveBMP(raster, "path_arc_relative.bmp");
    HCRasterSavePPM(raster, "path_arc_relative.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPath(raster, "M 10 90 C 30 10 70 10 90 90 Z L 30 10 H 70 L 90 90 Q 50 10 10 90", HCColorGreen);
    HCRasterSaveBMP(raster, "path.bmp");
    HCRasterSavePPM(raster, "path.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawManyQuadratics) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCRasterDrawPath(raster,
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
        , HCColorGreen);
    HCRasterSaveBMP(raster, "path_many_quadratics.bmp");
    HCRasterSavePPM(raster, "path_many_quadratics.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawManyCubics) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCRasterDrawPath(raster,
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
        , HCColorGreen);
    HCRasterSaveBMP(raster, "path_many_cubics.bmp");
    HCRasterSavePPM(raster, "path_many_cubics.ppm");
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
