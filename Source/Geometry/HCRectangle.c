//
//  HCRectangle.c
//  HollowCore
//
//  Created by Matt Stoker on 10/19/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCRectangle.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCRectangle HCRectangleMake(HCPoint origin, HCSize size) {
    return (HCRectangle){.origin = origin, .size = size};
}

HCRectangle HCRectangleMakeWithComponents(HCReal x, HCReal y, HCReal width, HCReal height) {
    return HCRectangleMake(HCPointMake(x, y), HCSizeMake(width, height));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRectangleIsInvalid(HCRectangle rectangle) {
    return HCPointIsInvalid(rectangle.origin) || HCSizeIsInvalid(rectangle.size);
}

HCBoolean HCRectangleIsSimilar(HCRectangle rectangle, HCRectangle other, HCReal elementDissimilarity) {
    return
        HCPointIsSimilar(rectangle.origin, other.origin, elementDissimilarity) &&
        HCSizeIsSimilar(rectangle.size, other.size, elementDissimilarity);
}

HCBoolean HCRectangleIsEqual(HCRectangle rectangle, HCRectangle other) {
    return
        HCPointIsEqual(rectangle.origin, other.origin) &&
        HCSizeIsEqual(rectangle.size, other.size);
}

HCInteger HCRectangleHashValue(HCRectangle rectangle) {
    return
        HCPointHashValue(rectangle.origin) ^
        HCSizeHashValue(rectangle.size);
}

void HCRectanglePrint(HCRectangle rectangle, FILE* stream) {
    fprintf(stream, "<");
    fprintf(stream, "o:");
    HCPointPrint(rectangle.origin, stream);
    fprintf(stream, ",");
    fprintf(stream, "s:");
    HCSizePrint(rectangle.size, stream);
    fprintf(stream, ">");
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Queries
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRectangleIsZero(HCRectangle rectangle) {
    return HCRectangleIsEqual(rectangle, HCRectangleZero);
}
HCBoolean HCRectangleIsInfinite(HCRectangle rectangle) {
    return HCPointIsInfinite(rectangle.origin) || HCSizeIsInfinite(rectangle.size);
}

HCBoolean HCRectangleIsEmpty(HCRectangle rectangle) {
    return rectangle.size.width * rectangle.size.height == 0.0;
}

HCBoolean HCRectangleContainsPoint(HCRectangle rectangle, HCPoint point) {
    return point.x >= HCRectangleMinX(rectangle) && point.x <= HCRectangleMaxX(rectangle) && point.y >= HCRectangleMinY(rectangle) && point.y <= HCRectangleMaxY(rectangle);
}

HCBoolean HCRectangleContainsRectangle(HCRectangle rectangle, HCRectangle other) {
    return HCRectangleContainsPoint(rectangle, other.origin) && HCRectangleContainsPoint(rectangle, HCPointOffset(other.origin, other.size.width, other.size.height));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Calculated Properties
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCRectangleWidth(HCRectangle rectangle) {
    return fabs(rectangle.size.width);
}

HCReal HCRectangleHeight(HCRectangle rectangle) {
    return fabs(rectangle.size.height);
}

HCReal HCRectangleMinX(HCRectangle rectangle) {
    return rectangle.size.width > 0.0 ? rectangle.origin.x : (rectangle.origin.x + rectangle.size.width);
}

HCReal HCRectangleMinY(HCRectangle rectangle) {
    return rectangle.size.height > 0.0 ? rectangle.origin.y : (rectangle.origin.y + rectangle.size.height);
}

HCReal HCRectangleMidX(HCRectangle rectangle) {
    return rectangle.origin.x + rectangle.size.width * 0.5;
}

HCReal HCRectangleMidY(HCRectangle rectangle) {
    return rectangle.origin.y + rectangle.size.height * 0.5;
}

HCReal HCRectangleMaxX(HCRectangle rectangle) {
    return rectangle.size.width > 0.0 ? (rectangle.origin.x + rectangle.size.width) : rectangle.origin.x;
}

HCReal HCRectangleMaxY(HCRectangle rectangle) {
    return rectangle.size.height > 0.0 ? (rectangle.origin.y + rectangle.size.height) : rectangle.origin.y;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCRectangle HCRectangleStandardize(HCRectangle rectangle) {
    if (rectangle.size.width < 0.0) {
        rectangle.origin.x += rectangle.size.width;
        rectangle.size.width *= -1.0;
    }
    if (rectangle.size.height < 0.0) {
        rectangle.origin.y += rectangle.size.height;
        rectangle.size.height *= -1.0;
    }
    return rectangle;
}

HCRectangle HCRectangleIntegral(HCRectangle rectangle) {
    rectangle.origin.x = round(rectangle.origin.x);
    rectangle.origin.y = round(rectangle.origin.y);
    rectangle.size.width = round(rectangle.size.width);
    rectangle.size.height = round(rectangle.size.height);
    return rectangle;
}

HCRectangle HCRectangleOutset(HCRectangle rectangle, HCReal dx, HCReal dy) {
    rectangle.origin.x -= dx;
    rectangle.origin.y -= dy;
    rectangle.size.width += dx * 2.0;
    rectangle.size.height += dy * 2.0;
    return rectangle;
}

HCRectangle HCRectangleInset(HCRectangle rectangle, HCReal dx, HCReal dy) {
    return HCRectangleOutset(rectangle, -dx, -dy);
}

HCRectangle HCRectangleOffset(HCRectangle rectangle, HCReal dx, HCReal dy) {
    rectangle.origin = HCPointOffset(rectangle.origin, dx, dy);
    return rectangle;
}

HCRectangle HCRectangleUnion(HCRectangle rectangle, HCRectangle other) {
    HCReal minX = fmin(HCRectangleMinX(rectangle), HCRectangleMinX(other));
    HCReal minY = fmin(HCRectangleMinY(rectangle), HCRectangleMinY(other));
    HCReal maxX = fmax(HCRectangleMaxX(rectangle), HCRectangleMaxX(other));
    HCReal maxY = fmax(HCRectangleMaxY(rectangle), HCRectangleMaxY(other));
    return HCRectangleMake(HCPointMake(minX, minY), HCSizeMake(maxX - minX, maxY - minY));
}

HCRectangle HCRectangleIntersection(HCRectangle rectangle, HCRectangle other) {
    HCReal maxMinX = fmax(HCRectangleMinX(rectangle), HCRectangleMinX(other));
    HCReal maxMinY = fmax(HCRectangleMinY(rectangle), HCRectangleMinY(other));
    HCReal minMaxX = fmin(HCRectangleMaxX(rectangle), HCRectangleMaxX(other));
    HCReal minMaxY = fmin(HCRectangleMaxY(rectangle), HCRectangleMaxY(other));
    return (minMaxX < maxMinX || minMaxY < maxMinY) ?
        HCRectangleMake(HCPointMake((maxMinX - minMaxX) * 0.5, (maxMinY - minMaxY) * 0.5), HCSizeZero) :
        HCRectangleMake(HCPointMake(maxMinX, maxMinY), HCSizeMake(minMaxX - maxMinX, minMaxY - maxMinY));
}

HCRectangle HCRectangleIncludingPoint(HCRectangle rectangle, HCPoint point) {
    HCReal minX = fmin(HCRectangleMinX(rectangle), point.x);
    HCReal minY = fmin(HCRectangleMinY(rectangle), point.y);
    HCReal maxX = fmax(HCRectangleMaxX(rectangle), point.x);
    HCReal maxY = fmax(HCRectangleMaxY(rectangle), point.y);
    return HCRectangleMake(HCPointMake(minX, minY), HCSizeMake(maxX - minX, maxY - minY));
}

void HCRectangleDivide(HCRectangle rectangle, HCRectangle* slice, HCRectangle* remainder, HCReal amount, HCRectangleEdge edge) {
    // Adjust the requested amount if it is out of bounds
    amount = fmax(0.0, amount);
    if (edge == HCRectangleEdgeMinX || edge == HCRectangleEdgeMaxX) {
        amount = fmin(HCRectangleWidth(rectangle), amount);
    }
    else {
        amount = fmin(HCRectangleHeight(rectangle), amount);
    }
    
    // Divide the rectangle by obtaining the extents of the requested divided parts
    HCReal sliceMinX = NAN;
    HCReal sliceMinY = NAN;
    HCReal sliceMaxX = NAN;
    HCReal sliceMaxY = NAN;
    HCReal remainderMinX = NAN;
    HCReal remainderMinY = NAN;
    HCReal remainderMaxX = NAN;
    HCReal remainderMaxY = NAN;
    switch (edge) {
        case HCRectangleEdgeMinX:
            sliceMinX = HCRectangleMinX(rectangle);
            sliceMinY = HCRectangleMinY(rectangle);
            sliceMaxX = HCRectangleMinX(rectangle) + HCRectangleWidth(rectangle) * amount;
            sliceMaxY = HCRectangleMaxY(rectangle);
            remainderMinX = sliceMaxX;
            remainderMinY = HCRectangleMinY(rectangle);
            remainderMaxX = HCRectangleMaxX(rectangle);
            remainderMaxY = HCRectangleMaxY(rectangle);
            break;
        case HCRectangleEdgeMinY:
            sliceMinX = HCRectangleMinX(rectangle);
            sliceMinY = HCRectangleMinY(rectangle);
            sliceMaxX = HCRectangleMaxX(rectangle);
            sliceMaxY = HCRectangleMinY(rectangle) + HCRectangleHeight(rectangle) * amount;
            remainderMinX = HCRectangleMinX(rectangle);
            remainderMinY = sliceMaxY;
            remainderMaxX = HCRectangleMaxX(rectangle);
            remainderMaxY = HCRectangleMaxY(rectangle);
            break;
        case HCRectangleEdgeMaxX:
            sliceMinX = HCRectangleMaxX(rectangle) + HCRectangleWidth(rectangle) * -amount;
            sliceMinY = HCRectangleMinY(rectangle);
            sliceMaxX = HCRectangleMaxX(rectangle);
            sliceMaxY = HCRectangleMaxY(rectangle);
            remainderMinX = HCRectangleMinX(rectangle);
            remainderMinY = HCRectangleMinY(rectangle);
            remainderMaxX = sliceMinX;
            remainderMaxY = HCRectangleMaxY(rectangle);
            break;
        case HCRectangleEdgeMaxY:
            sliceMinX = HCRectangleMinX(rectangle);
            sliceMinY = HCRectangleMaxY(rectangle) + HCRectangleHeight(rectangle) * -amount;
            sliceMaxX = HCRectangleMaxX(rectangle);
            sliceMaxY = HCRectangleMaxY(rectangle);
            remainderMinX = HCRectangleMinX(rectangle);
            remainderMinY = HCRectangleMinY(rectangle);
            remainderMaxX = HCRectangleMaxX(rectangle);
            remainderMaxY = sliceMinY;
            break;
    }
    
    // Construct rectangles from the extents
    if (slice != NULL) {
        *slice = HCRectangleMake(HCPointMake(sliceMinX, sliceMinY), HCSizeMake(sliceMaxX - sliceMinX, sliceMaxY - sliceMinY));
    }
    if (remainder != NULL) {
        *remainder = HCRectangleMake(HCPointMake(remainderMinX, remainderMinY), HCSizeMake(remainderMaxX - remainderMinX, remainderMaxY - remainderMinY));
    }
}
