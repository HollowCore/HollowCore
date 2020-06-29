///
/// @file HCPath+SetOperations.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 5/9/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCPath_SetOperations_h
#define HCPath_SetOperations_h

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

#endif /* HCPath_SetOperations_h */
