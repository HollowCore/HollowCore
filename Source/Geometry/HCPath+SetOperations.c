///
/// @file HCPath+SetOperations.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 5/9/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCPath+SetOperations.h"

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
    HCPathForEachIntersection(self, other, HCPathCombineIntersectionFunction, (void*)intersections);
    
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
