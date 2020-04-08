//
//  HCPath.c
//  Test
//
//  Created by Matt Stoker on 12/28/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"

CTEST(HCPath, Creation) {
    HCPathRef path = HCPathCreateEmpty();
    ASSERT_EQUAL(HCPathElementCount(path), 0);
    HCRelease(path);
}

CTEST(HCPath, EqualHash) {
    HCPathRef a = HCPathCreateWithSVGPathData("M 1 2 L 1 2 Q 1 2 3 4 C 1 2 3 4 5 6 Z");
    HCPathRef b = HCPathCreateWithSVGPathData("M 1 2 L 1 2 Q 1 2 3 4 C 1 2 3 4 5 6 Z");
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

CTEST(HCPath, Print) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 1 2 L 1 2 Q 1 2 3 4 C 1 2 3 4 5 6 Z");
    HCObjectPrint((HCObjectRef)path, stdout); // TODO: Not to stdout
    HCPrint(path, stdout); // TODO: Not to stdout
    HCRelease(path);
}

CTEST(HCPath, CreateWithElements) {
    HCPoint points[] = {
        {.x = 1.0, .y = 2.0},
        {.x = 3.0, .y = 4.0},
        {.x = 5.0, .y = 6.0},
    };
    HCPathElement elements[] = {
        {.command = HCPathCommandMove, .points = points},
        {.command = HCPathCommandAddLine, .points = points},
        {.command = HCPathCommandAddQuadraticCurve, .points = points},
        {.command = HCPathCommandAddCubicCurve, .points = points},
        {.command = HCPathCommandCloseSubpath, .points = NULL},
    };
    HCPathRef path = HCPathCreateWithElements(elements, sizeof(elements) / sizeof(HCPathElement));
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(1.0, 2.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(1.0, 2.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(1.0, 2.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(3.0, 4.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(1.0, 2.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[1], HCPointMake(3.0, 4.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(5.0, 6.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandCloseSubpath);
    HCRelease(path);
}

CTEST(HCPath, Rectangle) {
    HCRectangle rectangle = HCRectangleMakeWithComponents(10.0, 20.0, 30.0, 40.0);
    HCPathRef path = HCPathCreateRectangle(rectangle);
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(HCRectangleMinX(rectangle), HCRectangleMinY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(HCRectangleMaxX(rectangle), HCRectangleMinY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(HCRectangleMaxX(rectangle), HCRectangleMaxY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(HCRectangleMinX(rectangle), HCRectangleMaxY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandCloseSubpath);
    HCRelease(path);
}

CTEST(HCPath, Ellipse) {
    HCRectangle rectangle = HCRectangleMakeWithComponents(10.0, 20.0, 30.0, 40.0);
    HCPathRef path = HCPathCreateEllipse(rectangle);
    ASSERT_EQUAL(HCPathElementCount(path), 6);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(HCRectangleMaxX(rectangle), HCRectangleMidY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(HCRectangleMidX(rectangle), HCRectangleMaxY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(HCRectangleMinX(rectangle), HCRectangleMidY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(HCRectangleMidX(rectangle), HCRectangleMinY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[2], HCPointMake(HCRectangleMaxX(rectangle), HCRectangleMidY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 5).command == HCPathCommandCloseSubpath);
    HCRelease(path);
}

CTEST(HCPath, HorizontalPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 H 50");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(50.0, 20.0)));
    HCRelease(path);
}

CTEST(HCPath, HorizontalRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 h 40");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(50.0, 20.0)));
    HCRelease(path);
}

CTEST(HCPath, HorizontalPolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 H 25 50 75 90");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(25.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(50.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(75.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(90.0, 20.0)));
    HCRelease(path);
}

CTEST(HCPath, HorizontalRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 h 15 25 25 15");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(25.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(50.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(75.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(90.0, 20.0)));
    HCRelease(path);
}

CTEST(HCPath, VerticalPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 V 50");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 50.0)));
    HCRelease(path);
}

CTEST(HCPath, VerticalRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 v 30");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 50.0)));
    HCRelease(path);
}

CTEST(HCPath, VerticalPolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 V 25 50 75 80");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 25.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(10.0, 50.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(10.0, 75.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, VerticalRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 v 5 25 25 5");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 25.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(10.0, 50.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(10.0, 75.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, LinePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 L 60 80");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(60.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, LineRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 l 50 60");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(60.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, LinePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 L 60 80 50 40 90 20");
    ASSERT_EQUAL(HCPathElementCount(path), 4);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(60.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(50.0, 40.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(90.0, 20.0)));
    HCRelease(path);
}

CTEST(HCPath, LineRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 l 50 60 -10 -40 40 -20");
    ASSERT_EQUAL(HCPathElementCount(path), 4);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(60.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(50.0, 40.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(90.0, 20.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticCurvePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 95 10 180 80");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(95.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(180.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticCurveRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 q 85 -70 170 0");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(95.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(180.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticCurvePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 95 10 180 80 95 150 10 80");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(95.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(180.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(95.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticCurveRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 q 85 -70 170 0 -85 70 -170 0");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(95.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(180.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(95.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticSmoothCurvePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 52.5 10 95 80 T 180 80");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(52.5, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(95, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(137.5, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(180.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticSmoothCurveRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 52.5 10 95 80 t 85 0");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(52.5, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(95, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(137.5, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(180.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticSmoothCurvePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 52.5 10 95 80 T 180 80 95 80 10 80");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(52.5, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(95, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(137.5, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(180.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(222.5, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[1], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(-32.5, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[1], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticSmoothCurveRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 52.5 10 95 80 t 85 0 -85 0 -85 0");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(52.5, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(95, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(137.5, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(180.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(222.5, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[1], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(-32.5, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[1], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicCurvePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 90 C 30 10 70 10 90 90");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(70.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(90.0, 90.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicCurveRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 90 c 20 -80 60 -80 80 0");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(70.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(90.0, 90.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicCurvePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 90 C 30 10 70 10 90 90 70 40 30 40 10 90");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(70.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(90.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(70.0, 40.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(30.0, 40.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(10.0, 90.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicCurveRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 90 c 20 -80 60 -80 80 0 -20 -50 -60 -50 -80 0");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(70.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(90.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(70.0, 40.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(30.0, 40.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(10.0, 90.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicSmoothCurvePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 C 40 10 65 10 95 80 S 150 150 180 80");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(40.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(65.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(125.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(150.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(180.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicSmoothCurveRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 C 40 10 65 10 95 80 s 55 70 85 0");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(40.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(65.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(125.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(150.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(180.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicSmoothCurvePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 C 40 10 65 10 95 80 S 150 150 180 80 80 10 95 80 10 150 10 80");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(40.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(65.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(125.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(150.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(180.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(210.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[1], HCPointMake(80.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(110.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[1], HCPointMake(10.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[2], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicSmoothCurveRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 C 40 10 65 10 95 80 s 55 70 85 0 -100 -70 -85 0 -85 70 -85 0");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(40.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(65.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(125.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(150.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(180.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(210.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[1], HCPointMake(80.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(110.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[1], HCPointMake(10.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[2], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, ArcPath) {
    HCPathRef path = HCPathCreateWithSVGPathData(
        "M25 350 L 75 325"
        "A25  25 -30 0 1 125 300 L 175 275"
        "A25  50 -30 0 1 225 250 L 275 225"
        "A25  75 -30 0 1 325 200 L 375 175"
        "A25 100 -30 0 1 425 150 L 475 125"
    );
    ASSERT_EQUAL(HCPathElementCount(path), 14);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(25.0, 350.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(75.0, 325.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(125.0, 300.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(175.0, 275.0)));
    ASSERT_TRUE(HCPathElementAt(path, 5).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 6).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 6).points[2], HCPointMake(225.0, 250.0)));
    ASSERT_TRUE(HCPathElementAt(path, 7).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 7).points[0], HCPointMake(275.0, 225.0)));
    ASSERT_TRUE(HCPathElementAt(path, 8).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 9).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 9).points[2], HCPointMake(325.0, 200.0)));
    ASSERT_TRUE(HCPathElementAt(path, 10).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 10).points[0], HCPointMake(375.0, 175.0)));
    ASSERT_TRUE(HCPathElementAt(path, 11).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 12).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 12).points[2], HCPointMake(425.0, 150.0)));
    ASSERT_TRUE(HCPathElementAt(path, 13).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 13).points[0], HCPointMake(475.0, 125.0)));
    HCRelease(path);
}

CTEST(HCPath, ArcRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData(
        "M25 350 l 50 -25"
        "a25  25 -30 0 1 50 -25 l 50 -25"
        "a25  50 -30 0 1 50 -25 l 50 -25"
        "a25  75 -30 0 1 50 -25 l 50 -25"
        "a25 100 -30 0 1 50 -25 l 50 -25"
    );
    ASSERT_EQUAL(HCPathElementCount(path), 14);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(25.0, 350.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(75.0, 325.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(125.0, 300.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(175.0, 275.0)));
    ASSERT_TRUE(HCPathElementAt(path, 5).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 6).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 6).points[2], HCPointMake(225.0, 250.0)));
    ASSERT_TRUE(HCPathElementAt(path, 7).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 7).points[0], HCPointMake(275.0, 225.0)));
    ASSERT_TRUE(HCPathElementAt(path, 8).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 9).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 9).points[2], HCPointMake(325.0, 200.0)));
    ASSERT_TRUE(HCPathElementAt(path, 10).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 10).points[0], HCPointMake(375.0, 175.0)));
    ASSERT_TRUE(HCPathElementAt(path, 11).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 12).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 12).points[2], HCPointMake(425.0, 150.0)));
    ASSERT_TRUE(HCPathElementAt(path, 13).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 13).points[0], HCPointMake(475.0, 125.0)));
    HCRelease(path);
}

CTEST(HCPath, ArcPolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 315 L 110 215 A 30 50 60 0 1 162.55 162.55 30 50 30 1 0 215.10 110.10 L 315 10");
    ASSERT_EQUAL(HCPathElementCount(path), 8);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 315.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(110.0, 215.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(162.55, 162.55)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 5).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 6).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 6).points[2], HCPointMake(215.10, 110.10)));
    ASSERT_TRUE(HCPathElementAt(path, 7).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 7).points[0], HCPointMake(315.0, 10.0)));
    HCRelease(path);
}

CTEST(HCPath, ArcRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 315 L 110 215 a 30 50 60 0 1 52.55 -52.45 30 50 30 1 0 52.55 -52.45 L 315 10");
    ASSERT_EQUAL(HCPathElementCount(path), 8);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 315.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(110.0, 215.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(162.55, 162.55)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 5).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 6).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsSimilar(HCPathElementAt(path, 6).points[2], HCPointMake(215.10, 110.10), 0.000001));
    ASSERT_TRUE(HCPathElementAt(path, 7).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 7).points[0], HCPointMake(315.0, 10.0)));
    HCRelease(path);
}

CTEST(HCPath, Path) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 90 C 30 10 70 10 90 90 Z L 30 10 H 70 L 90 90 Q 50 10 10 90 Z");
    ASSERT_EQUAL(HCPathElementCount(path), 8);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(70.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(90.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandCloseSubpath);
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(70.0, 10.0)));
    ASSERT_TRUE(HCPathElementAt(path, 5).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 5).points[0], HCPointMake(90.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 6).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 6).points[0], HCPointMake(50.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 6).points[1], HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 7).command == HCPathCommandCloseSubpath);
    HCRelease(path);
}

CTEST(HCPath, ManyQuadratics) {
    HCPathRef path = HCPathCreateWithSVGPathData(
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
    );
    ASSERT_EQUAL(HCPathElementCount(path), 42);
    HCRelease(path);
}

CTEST(HCPath, ManyCubics) {
    HCPathRef path = HCPathCreateWithSVGPathData(
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
    );
    ASSERT_EQUAL(HCPathElementCount(path), 42);
    HCRelease(path);
}

CTEST(HCPath, Dinosaur) {
    const char* dinosaur = "m 134.51827,42.745763 c -16.01255,0 -29.04048,13.025256 -29.04048,29.035206 0,3.45164 2.79845,6.25017 6.24999,6.25017 3.45153,0 6.24999,-2.79853 6.24999,-6.25017 0,-9.115845 7.41881,-16.534865 16.5405,-16.534865 9.1217,0 16.54052,7.41902 16.54052,16.534865 0,3.45164 2.79845,6.25017 6.24999,6.25017 3.45153,0 6.24999,-2.79853 6.24999,-6.25017 0,-16.00995 -13.02794,-29.035206 -29.0405,-29.035206 z M 495.72086,350.60935 c -3.53088,-1.17497 -9.09728,-1.26652 -19.25962,-1.26652 -0.49745,-0.003 -1.01013,-0.003 -1.53808,0 -39.42867,0 -81.56424,-2.84431 -84.32913,-35.79504 -11.33422,-135.05921 -116.9829,-147.15671 -161.75824,-146.02447 -10.50414,0.14955 -20.07138,-4.68153 -26.00094,-13.18701 -5.93567,-8.51463 -7.15636,-18.9031 -3.44543,-28.30887 0.4028,-0.84231 9.96703,-20.87154 12.90587,-44.819331 C 216.36328,48.06512 205.71876,23.735847 181.51835,10.847922 152.49311,-4.6035481 121.14854,-3.5323515 90.881248,13.95775 86.559958,16.451099 81.701568,17.50704 76.922519,17.073678 61.660699,15.624052 33.441124,15.633208 15.637188,31.844586 5.2612254,41.28698 0,54.650994 0,71.564289 c 0,17.10862 5.7861249,31.311891 17.199686,42.210011 20.364358,19.45243 53.131048,22.86744 68.170092,23.36184 -7.434074,38.31281 -7.208249,70.08552 -7.192984,71.44359 0,32.52881 7.423389,82.77127 22.937366,123.33099 L 89.694108,437.47999 c -0.43945,6.22271 1.751715,12.41184 6.00586,16.97739 4.254145,4.56556 10.275252,7.18403 16.516092,7.18403 h 11.01111 l -0.9739,14.20022 c -0.4364,6.22576 1.75476,12.41489 6.00891,16.97739 4.25415,4.5625 10.2722,7.18098 16.51304,7.18098 h 46.24018 c 6.24084,0 12.2589,-2.61848 16.51305,-7.18403 4.25414,-4.56251 6.4453,-10.75164 6.01805,-16.72104 l -2.31323,-78.71002 c 7.28569,1.85857 15.16913,2.94808 23.24903,3.3513 l 0.8812,37.49759 c 0.9491,12.90623 11.83165,23.40761 24.26145,23.40761 h 9.24528 l 0.44705,13.77601 C 270.7852,491.26868 280.47146,500 296.58778,500 h 33.12985 c 8.62731,0 15.61887,-2.71004 20.21786,-7.84018 4.29382,-4.78529 6.12488,-11.31317 5.3009,-18.84511 l -5.1239,-49.2262 c 68.27996,23.81962 128.78099,-23.17567 149.24605,-64.79058 0.79655,-1.62052 0.8545,-3.50656 0.15565,-5.17286 -0.69579,-1.66326 -2.07824,-2.94808 -3.79333,-3.51572 z m -383.5048,98.53172 c -2.82593,0 -5.44433,-1.13833 -7.37304,-3.20748 -1.92566,-2.0661 -2.87781,-4.75782 -2.70081,-7.34273 l 9.08813,-83.98627 c 5.21583,9.92077 11.06757,18.67612 17.5781,25.66254 l -4.72373,68.87394 z m 147.40889,0 c -5.8197,0 -11.33422,-5.52383 -11.78283,-11.51154 l -0.85907,-36.83534 c 6.66847,-0.28345 13.34876,-1.02503 19.83793,-2.28926 l 1.64337,50.63614 z m 85.74819,-40.3728 c -2.03552,-0.90639 -4.40979,-0.66225 -6.21338,0.647 -1.80969,1.31229 -2.78014,3.4852 -2.54821,5.70694 l 6.19506,59.52004 c 0.4364,3.98876 -0.296,7.07417 -2.17895,9.17383 -2.72827,3.04574 -7.44018,3.68358 -10.91002,3.68358 h -33.12985 c -9.65881,0 -13.95262,-3.83617 -14.80101,-12.86046 l -2.71911,-84.11178 c -0.0613,-1.9013 -0.98572,-3.67136 -2.5116,-4.8097 -1.08947,-0.81179 -2.40173,-1.23905 -3.73534,-1.23905 -0.531,0 -1.06507,0.0671 -1.58692,0.2045 -21.36532,5.60929 -46.02655,4.95619 -64.3737,-1.69377 -1.94092,-0.70497 -4.10766,-0.40285 -5.79223,0.80874 -1.67846,1.21463 -2.64587,3.18001 -2.58483,5.24916 l 2.58178,87.67023 c 0.19835,2.8199 -0.75378,5.50857 -2.67944,7.57772 -1.92566,2.06607 -4.54406,3.2044 -7.36999,3.2044 h -46.24018 c -2.82592,0 -5.44433,-1.13833 -7.36999,-3.20443 -1.92871,-2.0661 -2.88085,-4.75782 -2.68249,-7.58993 l 6.74743,-98.35472 c 0.1251,-1.80669 -0.54019,-3.57675 -1.8219,-4.85548 -10.29509,-10.258 -19.01624,-26.00664 -26.14056,-44.22536 -0.04,-0.1453 -0.10415,-0.2724 -0.15415,-0.4135 -14.885312,-38.27199 -22.676822,-87.41654 -22.676822,-120.33751 -0.003,-0.35705 -0.2441,-36.12159 8.46252,-76.29297 0.40895,-1.8952 -0.08545,-3.87279 -1.33362,-5.35598 -1.251225,-1.48014 -3.091425,-2.24921 -5.056755,-2.21259 -0.4303,0.0245 -44.314526,1.70599 -66.943288,-19.94683 C 16.851762,96.143829 12.49996,85.303689 12.49996,71.564299 c 0,-13.24804 3.884883,-23.502225 11.550894,-30.475677 14.251689,-12.973377 38.510085,-12.811629 51.693665,-11.569527 7.406604,0.717182 14.813214,-0.933869 21.386694,-4.73646 26.342737,-15.2195321 53.497247,-16.2235841 78.512477,-2.899247 47.17707,25.116651 12.83568,97.982392 12.48473,98.714832 -0.0641,0.13425 -0.21055,0.48835 -0.2655,0.62563 -5.29784,13.37011 -3.53698,28.4218 4.71801,40.26294 8.33434,11.95711 21.85362,18.93972 36.55697,18.53077 51.29694,-1.15054 138.93111,14.59086 148.99885,134.57397 3.96728,47.30048 64.54155,47.19061 97.21363,47.25165 2.74048,-0.01 5.67626,-0.003 8.28857,0.0395 -22.85764,36.89981 -77.94791,73.78435 -138.26583,46.88543 z";
    HCPathRef path = HCPathCreateWithSVGPathData(dinosaur);
    ASSERT_EQUAL(HCPathElementCount(path), 109);
    HCRelease(path);
}

CTEST(HCPath, MoveImplicitLine) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 30 40 50 60");
    ASSERT_TRUE(HCPathElementCount(path) == 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(30.0, 40.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(50.0, 60.0)));
    HCRelease(path);
}

CTEST(HCPath, MoveImplicitLineRelative) {
    HCPathRef path = HCPathCreateWithSVGPathData("m 10 20 30 40 50 60");
    ASSERT_TRUE(HCPathElementCount(path) == 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(40.0, 60.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(90.0, 120.0)));
    HCRelease(path);
}

CTEST(HCPath, DegenerateLine) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 L 10 10");
    ASSERT_TRUE(HCPathElementCount(path) == 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 10.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 10.0)));
    HCRelease(path);
}

CTEST(HCPath, DegenerateQuadratic) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 Q 10 10 10 10");
    ASSERT_TRUE(HCPathElementCount(path) == 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 10.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(10.0, 10.0)));
    HCRelease(path);
}

CTEST(HCPath, DegenerateCubic) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 C 10 10 10 10 10 10");
    ASSERT_TRUE(HCPathElementCount(path) == 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 10.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(10.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(10.0, 10.0)));
    HCRelease(path);
}

CTEST(HCPath, Size) {
    HCPathRef emptyPath = HCPathCreateWithSVGPathData("");
    ASSERT_TRUE(HCRectangleIsEqual(HCPathBounds(emptyPath), HCRectangleZero));
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 L 30 10 30 20 10 20 Z");
    ASSERT_TRUE(HCRectangleIsEqual(HCPathBounds(path), HCRectangleMake(HCPointMake(10.0, 10.0), HCSizeMake(20.0, 10.0))));
    HCRelease(emptyPath);
    HCRelease(path);
}

CTEST(HCPath, MutabilityAndCurrentPoint) {
    HCPathRef path = HCPathCreateWithSVGPathData("");
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointZero));
    HCPathMove(path, 5.0, 10.0);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(5.0, 10.0)));
    HCPathAddLine(path, 50.0, 100.0);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(50.0, 100.0)));
    HCPathAddQuadraticCurve(path, 8.0, 40.0, -20.0, 60.0);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(-20.0, 60.0)));
    HCPathAddCubicCurve(path, 80.0, 45.0, -10.0, -80.0, 35.0, 77.0);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(35.0, 77.0)));
    HCPathClose(path);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(5.0, 10.0)));
    HCPathRemoveLastElement(path);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(35.0, 77.0)));
    HCPathRemoveLastElement(path);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(-20.0, 60.0)));
    HCPathRemoveLastElement(path);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(50.0, 100.0)));
    HCPathRemoveLastElement(path);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(5.0, 10.0)));
    HCPathRemoveLastElement(path);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointZero));
    HCRelease(path);
}

CTEST(HCPath, PolylineOfBox) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 L 60 80 30 10 M 1 2 L 6 8 3 1");
    ASSERT_TRUE(HCIntegerIsEqual(HCPathElementCount(path), 6));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathElementPolylinePointCount(path, 0), 0));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathElementPolylinePointCount(path, 1), 2));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementPolylinePointAt(path, 1, 0), HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementPolylinePointAt(path, 1, 1), HCPointMake(60.0, 80.0)));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathElementPolylinePointCount(path, 2), 2));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementPolylinePointAt(path, 2, 0), HCPointMake(60.0, 80.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementPolylinePointAt(path, 2, 1), HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathElementPolylinePointCount(path, 3), 0));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathElementPolylinePointCount(path, 4), 2));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementPolylinePointAt(path, 4, 0), HCPointMake(1.0, 2.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementPolylinePointAt(path, 4, 1), HCPointMake(6.0, 8.0)));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathElementPolylinePointCount(path, 5), 2));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementPolylinePointAt(path, 5, 0), HCPointMake(6.0, 8.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementPolylinePointAt(path, 5, 1), HCPointMake(3.0, 1.0)));
    HCRelease(path);
}

CTEST(HCPath, LineSegmentsFromQuadratic) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 95 10 180 80");
    ASSERT_TRUE(HCIntegerIsEqual(HCPathElementCount(path), 2));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathElementPolylinePointCount(path, 0), 0));
    ASSERT_TRUE(HCPathElementPolylinePointCount(path, 1) > 10);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementPolylinePointAt(path, 1, 0), HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementPolylinePointAt(path, 1, HCPathElementPolylinePointCount(path, 1) - 1), HCPointMake(180.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, LineSegmentsFromCubic) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 90 C 30 10 70 10 90 90");
    ASSERT_TRUE(HCIntegerIsEqual(HCPathElementCount(path), 2));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathElementPolylinePointCount(path, 0), 0));
    ASSERT_TRUE(HCPathElementPolylinePointCount(path, 1) > 10);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementPolylinePointAt(path, 1, 0), HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementPolylinePointAt(path, 1, HCPathElementPolylinePointCount(path, 1) - 1), HCPointMake(90.0, 90.0)));
    HCRelease(path);
}

CTEST(HCPath, Subpaths) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 1 2 L 3 4 M 3 4 Q 5 6 7 8 L 9 0 Z M 5 6 C 7 8 9 0 1 2 Z");
    HCListRef subpaths = HCPathSubpathsRetained(path);
    ASSERT_EQUAL(HCListCount(subpaths), 3);
    HCPathRef subpath0 = HCListObjectAtIndex(subpaths, 0);
    ASSERT_EQUAL(HCPathElementCount(subpath0), 2);
    ASSERT_TRUE(HCPathElementAt(subpath0, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(subpath0, 1).command == HCPathCommandAddLine);
    HCPathRef subpath1 = HCListObjectAtIndex(subpaths, 1);
    ASSERT_EQUAL(HCPathElementCount(subpath1), 4);
    ASSERT_TRUE(HCPathElementAt(subpath1, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(subpath1, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPathElementAt(subpath1, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPathElementAt(subpath1, 3).command == HCPathCommandCloseSubpath);
    HCPathRef subpath2 = HCListObjectAtIndex(subpaths, 2);
    ASSERT_EQUAL(HCPathElementCount(subpath2), 3);
    ASSERT_TRUE(HCPathElementAt(subpath2, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(subpath2, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(subpath2, 2).command == HCPathCommandCloseSubpath);
    HCRelease(path);
    HCRelease(subpaths);
}

CTEST(HCPath, OpenSubpaths) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 1 2 L 3 4 M 3 4 Q 5 6 7 8 L 9 0 Z M 5 6 C 7 8 9 0 1 2 Z");
    HCListRef openSubpaths = HCPathOpenSubpathsRetained(path);
    ASSERT_EQUAL(HCListCount(openSubpaths), 1);
    HCPathRef subpath0 = HCListObjectAtIndex(openSubpaths, 0);
    ASSERT_EQUAL(HCPathElementCount(subpath0), 2);
    ASSERT_TRUE(HCPathElementAt(subpath0, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(subpath0, 1).command == HCPathCommandAddLine);
    HCRelease(path);
    HCRelease(openSubpaths);
}

CTEST(HCPath, ClosedSubpaths) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 1 2 L 3 4 M 3 4 Q 5 6 7 8 L 9 0 Z M 5 6 C 7 8 9 0 1 2 Z");
    HCListRef closedSubpaths = HCPathClosedSubpathsRetained(path);
    ASSERT_EQUAL(HCListCount(closedSubpaths), 2);
    HCPathRef subpath0 = HCListObjectAtIndex(closedSubpaths, 0);
    ASSERT_EQUAL(HCPathElementCount(subpath0), 4);
    ASSERT_TRUE(HCPathElementAt(subpath0, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(subpath0, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPathElementAt(subpath0, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPathElementAt(subpath0, 3).command == HCPathCommandCloseSubpath);
    HCPathRef subpath1 = HCListObjectAtIndex(closedSubpaths, 1);
    ASSERT_EQUAL(HCPathElementCount(subpath1), 3);
    ASSERT_TRUE(HCPathElementAt(subpath1, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(subpath1, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(subpath1, 2).command == HCPathCommandCloseSubpath);
    HCRelease(path);
    HCRelease(closedSubpaths);
}

CTEST(HCPath, OpenSubpathsAsPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 1 2 L 3 4 M 3 4 Q 5 6 7 8 L 9 0 Z M 5 6 C 7 8 9 0 1 2 Z");
    HCPathRef open = HCPathOpenSubpathsAsPathRetained(path);
    ASSERT_EQUAL(HCPathElementCount(open), 2);
    ASSERT_TRUE(HCPathElementAt(open, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(open, 1).command == HCPathCommandAddLine);
    HCRelease(path);
    HCRelease(open);
}

CTEST(HCPath, ClosedSubpathsAsPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 1 2 L 3 4 M 3 4 Q 5 6 7 8 L 9 0 Z M 5 6 C 7 8 9 0 1 2 Z");
    HCPathRef closed = HCPathClosedSubpathsAsPathRetained(path);
    ASSERT_EQUAL(HCPathElementCount(closed), 7);
    ASSERT_TRUE(HCPathElementAt(closed, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(closed, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPathElementAt(closed, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPathElementAt(closed, 3).command == HCPathCommandCloseSubpath);
    ASSERT_TRUE(HCPathElementAt(closed, 4).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(closed, 5).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(closed, 6).command == HCPathCommandCloseSubpath);
    HCRelease(path);
    HCRelease(closed);
}

CTEST(HCPath, LinePathLinePathIntersection) {
    HCPathRef a = HCPathCreateWithSVGPathData("M 10 20 L 90 100");
    HCPathRef b = HCPathCreateWithSVGPathData("M 70 10 L 20 80");
    ASSERT_TRUE(HCPathIntersectsPath(a, b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCPath, QuadraticPathQuadraticPathIntersection) {
    HCPathRef a = HCPathCreateWithSVGPathData("M 10 30 Q 50 90 90 30");
    HCPathRef b = HCPathCreateWithSVGPathData("M 10 70 Q 50 10 90 70");
    ASSERT_TRUE(HCPathIntersectsPath(a, b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCPath, CubicPathCubicPathIntersection) {
    HCPathRef a = HCPathCreateWithSVGPathData("M 10 30 C 30 90 70 90 90 30");
    HCPathRef b = HCPathCreateWithSVGPathData("M 10 70 C 30 10 70 10 90 70");
    ASSERT_TRUE(HCPathIntersectsPath(a, b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCPath, ContainsPoint) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 L 30 10 30 20 10 20 Z M 18 14 L 22 14 22 16 18 16 Z");
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 15.0)));
    ASSERT_TRUE(HCPathContainsPoint(path, HCPointMake(11.0, 15.0)));
    ASSERT_TRUE(HCPathContainsPoint(path, HCPointMake(29.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(9.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(31.0, 15.0)));
    ASSERT_TRUE(HCPathContainsPoint(path, HCPointMake(20.0, 11.0)));
    ASSERT_TRUE(HCPathContainsPoint(path, HCPointMake(20.0, 19.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 9.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 21.0)));
    HCRelease(path);
}

CTEST(HCPath, OpenPathContainsPoint) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 L 30 10 30 20 10 20");
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(11.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(29.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(9.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(31.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 11.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 19.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 9.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 21.0)));
    HCRelease(path);
}

CTEST(HCPath, ContainsPointNonZero) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 L 30 10 30 20 10 20 Z M 18 14 L 22 14 22 16 18 16 Z");
    // TODO: Enable when this test is available
//    ASSERT_TRUE(HCPathContainsPointNonZero(path, HCPointMake(20.0, 15.0)));
//    ASSERT_TRUE(HCPathContainsPointNonZero(path, HCPointMake(11.0, 15.0)));
//    ASSERT_TRUE(HCPathContainsPointNonZero(path, HCPointMake(29.0, 15.0)));
//    ASSERT_FALSE(HCPathContainsPointNonZero(path, HCPointMake(9.0, 15.0)));
//    ASSERT_FALSE(HCPathContainsPointNonZero(path, HCPointMake(31.0, 15.0)));
//    ASSERT_TRUE(HCPathContainsPointNonZero(path, HCPointMake(20.0, 11.0)));
//    ASSERT_TRUE(HCPathContainsPointNonZero(path, HCPointMake(20.0, 19.0)));
//    ASSERT_FALSE(HCPathContainsPointNonZero(path, HCPointMake(20.0, 9.0)));
//    ASSERT_FALSE(HCPathContainsPointNonZero(path, HCPointMake(20.0, 21.0)));
    HCRelease(path);
}
