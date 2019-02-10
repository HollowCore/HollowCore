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
    HCRasterPixelsAt(raster, 30, 40, 55, 65, pixels);
    ASSERT_TRUE(pixels[0].b == HCRasterColorBlue.b);
    ASSERT_FALSE(pixels[1].b == HCRasterColorBlue.b);
    ASSERT_FALSE(pixels[1*(55-30)+0].b == HCRasterColorBlue.b);
    HCRasterSaveBMP(raster, "rectangle.bmp");
    HCRasterSavePPM(raster, "rectangle.ppm");
    HCRelease(raster);
}

CTEST(HCRaster, DrawPoint) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPoint(raster, 50, 50, HCRasterColorWhite);
    HCRasterSaveBMP(raster, "point.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLine) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawLine(raster, 10, 80, 90, 20, HCRasterColorYellow, HCRasterColorMagenta);
    HCRasterSaveBMP(raster, "line.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurve) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawQuadraticCurve(raster, 10, 90, 50, 10, 90, 90, HCRasterColorMagenta, HCRasterColorCyan);
    HCRasterSaveBMP(raster, "quadratic_curve.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurve) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawCubicCurve(raster, 10, 90, 30, 10, 70, 10, 90, 90, HCRasterColorCyan, HCRasterColorYellow);
    HCRasterSaveBMP(raster, "cubic_curve.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPath(raster, "M 10 90 C 30 10 70 10 90 90 Z L 30 10 L 70 10 L 90 90 Q 50 10 10 90", HCRasterColorGreen);
    HCRasterSaveBMP(raster, "path.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawTriangle) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawTriangle(raster, 10, 80, 50, 20, 90, 70, HCRasterColorRed, HCRasterColorGreen, HCRasterColorBlue);
    HCRasterSaveBMP(raster, "triangle.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, FillTriangle) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillTriangle(raster, 10, 80, 50, 20, 90, 70, HCRasterColorRed, HCRasterColorGreen, HCRasterColorBlue);
    HCRasterSaveBMP(raster, "triangle_filled.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuad) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawQuad(raster, 10, 80, 30, 20, 80, 10, 90, 90, HCRasterColorRed, HCRasterColorGreen, HCRasterColorBlue, HCRasterColorWhite);
    HCRasterSaveBMP(raster, "quad.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, FillQuad) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillQuad(raster, 10, 80, 30, 20, 80, 10, 90, 90, HCRasterColorRed, HCRasterColorGreen, HCRasterColorBlue, HCRasterColorWhite);
    HCRasterSaveBMP(raster, "quad_filled.bmp");
    HCRelease(raster);
}
