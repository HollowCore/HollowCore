//
//  HCPath+BooleanOperations.c
//  HollowCore
//
//  Created by Matt Stoker on 5/9/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#include "HCPath+BooleanOperations.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Private Interface
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathCombineIntersectionFunction(void* context, HCBoolean* continueSearching, HCPathRef path, HCPathRef otherPath, HCPoint point);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Boolean Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCPathRef HCPathCombine(HCPathRef self, HCPathRef other, HCPathCombineOperation operation) {
    // Find intersections between paths
    HCListRef intersections = HCListCreate();
    HCPathIntersections(self, other, HCPathCombineIntersectionFunction, (void*)intersections);
    
    // TODO: Segment paths at intersection points
    // TODO: Discard path segments according to containment in the overlaps according to the operation
    // TODO: Build resultant path from remaining path segments
    
    // Visualize intersections
    HCPathRef path = HCPathCreate();
    for (HCListIterator i = HCListIterationBegin(intersections); !HCListIterationHasEnded(&i); HCListIterationNext(&i)) {
        HCPoint p = *(HCPoint*)HCDataBytes(i.object);
        HCPathIsEmpty(path) ? HCPathMove(path, p.x, p.y) : HCPathAddLine(path, p.x, p.y);
    }
    HCPathClose(path);
    
    HCRelease(intersections);
    return path;
}

void HCPathCombineIntersectionFunction(void* context, HCBoolean* continueSearching, HCPathRef path, HCPathRef otherPath, HCPoint point) {
    HCListRef intersections = (HCListRef)context;
    HCListAddObjectReleased(intersections, HCDataCreateWithBytes(sizeof(point), (HCByte*)&point));
}
