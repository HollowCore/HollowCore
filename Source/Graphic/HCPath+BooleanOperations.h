///
/// @file HCPath+BooleanOperations.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 5/9/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCPath_BooleanOperations_h
#define HCPath_BooleanOperations_h

#include "HCPath.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Boolean Operations
//----------------------------------------------------------------------------------------------------------------------------------
typedef enum HCPathCombineOperation {
    HCPathCombineOperationUnion,
    HCPathCombineOperationIntersection,
    HCPathCombineOperationSubtraction,
    HCPathCombineOperationExclusion
} HCPathCombineOperation;

HCPathRef HCPathCombine(HCPathRef self, HCPathRef other, HCPathCombineOperation operation);

#endif /* HCPath_BooleanOperations_h */
