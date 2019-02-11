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
}

CTEST(HCRaster, PixelOperations) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 50).g == HCRasterColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 49, 50).g == HCRasterColorGreen.g);
    HCRasterSetPixelAt(raster, 50, 50, HCRasterColorGreen);
    ASSERT_TRUE(HCRasterPixelAt(raster, 50, 50).g == HCRasterColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 49, 50).g == HCRasterColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 51, 50).g == HCRasterColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 49).g == HCRasterColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 51).g == HCRasterColorGreen.g);
    HCRasterColor pixels[25*25];
    for (HCInteger i = 0; i < sizeof(pixels) / sizeof(HCRasterColor); i++) {
        pixels[i] = HCRasterColorRed;
    }
    HCRasterSetPixelsAt(raster, 30, 40, 55, 65, pixels);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 50).g == HCRasterColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 49, 50).g == HCRasterColorGreen.g);
    ASSERT_TRUE(HCRasterPixelAt(raster, 50, 50).r == HCRasterColorRed.r);
    ASSERT_TRUE(HCRasterPixelAt(raster, 49, 50).r == HCRasterColorRed.r);
    ASSERT_TRUE(HCRasterPixelAt(raster, 30, 50).r == HCRasterColorRed.r);
    ASSERT_TRUE(HCRasterPixelAt(raster, 54, 50).r == HCRasterColorRed.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 29, 50).r == HCRasterColorRed.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 55, 50).r == HCRasterColorRed.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 39).r == HCRasterColorRed.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 65).r == HCRasterColorRed.r);
    HCRasterSetPixelAt(raster, 30, 40, HCRasterColorBlue);
    ASSERT_TRUE(HCRasterPixelAt(raster, 30, 40).b == HCRasterColorBlue.b);
    HCRasterPixelsAt(raster, 30, 40, 55, 65, pixels);
    ASSERT_TRUE(pixels[0].b == HCRasterColorBlue.b);
    ASSERT_FALSE(pixels[1].b == HCRasterColorBlue.b);
    ASSERT_FALSE(pixels[1*(55-30)+0].b == HCRasterColorBlue.b);
    HCRasterClear(raster);
    ASSERT_FALSE(HCRasterPixelAt(raster, 30, 40).b == HCRasterColorBlue.b);
    HCRelease(raster);
}

CTEST(HCRaster, DrawPoint) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 50).r == HCRasterColorWhite.r);
    HCRasterDrawPoint(raster, 50, 50, HCRasterColorWhite);
    ASSERT_TRUE(HCRasterPixelAt(raster, 50, 50).r == HCRasterColorWhite.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 49, 50).r == HCRasterColorWhite.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 51, 50).r == HCRasterColorWhite.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 49).r == HCRasterColorWhite.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 51).r == HCRasterColorWhite.r);
    HCRasterSaveBMP(raster, "point.bmp");
    HCRasterSavePPM(raster, "point.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLine) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawLine(raster, 10, 80, 90, 20, HCRasterColorYellow, HCRasterColorMagenta);
    HCRasterSaveBMP(raster, "line.bmp");
    HCRasterSavePPM(raster, "line.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurve) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawQuadraticCurve(raster, 10, 90, 50, 10, 90, 90, HCRasterColorMagenta, HCRasterColorCyan);
    HCRasterSaveBMP(raster, "quadratic_curve.bmp");
    HCRasterSavePPM(raster, "quadratic_curve.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurve) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawCubicCurve(raster, 10, 90, 30, 10, 70, 10, 90, 90, HCRasterColorCyan, HCRasterColorYellow);
    HCRasterSaveBMP(raster, "cubic_curve.bmp");
    HCRasterSavePPM(raster, "cubic_curve.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPath(raster, "M 10 90 C 30 10 70 10 90 90 Z L 30 10 L 70 10 L 90 90 Q 50 10 10 90", HCRasterColorGreen);
    HCRasterSaveBMP(raster, "path.bmp");
    HCRasterSavePPM(raster, "path.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawTriangle) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawTriangle(raster, 10, 80, 50, 20, 90, 70, HCRasterColorRed, HCRasterColorGreen, HCRasterColorBlue);
    HCRasterSaveBMP(raster, "triangle.bmp");
    HCRasterSavePPM(raster, "triangle.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, FillTriangle) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillTriangle(raster, 10, 80, 50, 20, 90, 70, HCRasterColorRed, HCRasterColorGreen, HCRasterColorBlue);
    HCRasterSaveBMP(raster, "triangle_filled.bmp");
    HCRasterSavePPM(raster, "triangle_filled.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, FillTexturedTriangle) {
    HCRasterRef texture = HCRasterCreate(100, 100);
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(texture); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(texture); xIndex++) {
            HCRasterSetPixelAt(texture, xIndex, yIndex, xIndex < 50 ? HCRasterColorRed : HCRasterColorGreen);
        }
    }
//    HCRasterSaveBMP(texture, "texture.bmp");
    HCRelease(texture);
    texture = HCRasterCreateByLoadingBMP("texture.bmp");
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillTexturedTriangle(raster, 10, 80, 50, 20, 90, 70, texture, 0, 0, 100, 0, 0, 100);
    HCRasterSaveBMP(texture, "texture.bmp");
    HCRasterSavePPM(texture, "texture.ppm");
    HCRasterSaveBMP(raster, "triangle_textured.bmp");
    HCRasterSavePPM(raster, "triangle_textured.ppm");
    HCRelease(raster);
    HCRelease(texture);
}

CTEST(HCRaster, DrawQuad) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawQuad(raster, 30, 20, 80, 10, 90, 90, 10, 80, HCRasterColorRed, HCRasterColorGreen, HCRasterColorBlue, HCRasterColorWhite);
    HCRasterSaveBMP(raster, "quad.bmp");
    HCRasterSavePPM(raster, "quad.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, FillQuad) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillQuad(raster, 30, 20, 80, 10, 90, 90, 10, 80, HCRasterColorRed, HCRasterColorGreen, HCRasterColorBlue, HCRasterColorWhite);
    HCRasterSaveBMP(raster, "quad_filled.bmp");
    HCRasterSavePPM(raster, "quad_filled.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, FillTexturedQuad) {
    HCRasterRef texture = HCRasterCreate(100, 100);
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(texture); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(texture); xIndex++) {
            HCRasterSetPixelAt(texture, xIndex, yIndex, xIndex < 50 ? HCRasterColorRed : HCRasterColorGreen);
        }
    }
//    HCRasterSaveBMP(texture, "texture.bmp");
    HCRelease(texture);
    texture = HCRasterCreateByLoadingBMP("texture.bmp");
    HCRasterRef raster = HCRasterCreate(100, 100);
    for (HCInteger i = 0; i < HCRasterHeight(texture); i++) {
        HCRasterFillTexturedQuad(raster, 30, i, 80, 10, 90, 90, 10, 80, texture, 0, 0, 100, 0, 100, 100, 0, 100);
        char file[0xFF];
        sprintf(file, "_test-%li.bmp", (long)i);
        HCRasterSaveBMP(raster, file);
        HCRasterClear(raster);
    }
    HCRasterFillTexturedQuad(raster, 30, 20, 80, 10, 90, 90, 10, 80, texture, 0, 0, 100, 0, 100, 100, 0, 100);
    HCRasterSaveBMP(texture, "texture.bmp");
    HCRasterSavePPM(texture, "texture.ppm");
    HCRasterSaveBMP(raster, "quad_textured.bmp");
    HCRasterSavePPM(raster, "quad_textured.ppm");
    HCRelease(raster);
    HCRelease(texture);
}

CTEST(HCRaster, Gradient) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillQuad(raster, -50, 50, 50, -50, 150, 50, 50, 150, HCRasterColorRed, HCRasterColorRed, HCRasterColorGreen, HCRasterColorGreen);
    HCRasterSaveBMP(raster, "gradient.bmp");
    HCRasterSavePPM(raster, "gradient.ppm");
    HCRelease(raster);
}
