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

CTEST(HCRaster, DrawLine) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawLine(raster, 10, 90, 90, 10, HCRasterColorYellow);
    HCRasterSaveBMP(raster, "line.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadCurve) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawQuadCurve(raster, 10, 90, 50, 10, 90, 90, HCRasterColorCyan);
    HCRasterSaveBMP(raster, "quad_curve.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCurve) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawCurve(raster, 10, 90, 30, 10, 70, 10, 90, 90, HCRasterColorMagenta);
    HCRasterSaveBMP(raster, "curve.bmp");
    HCRelease(raster);
}
