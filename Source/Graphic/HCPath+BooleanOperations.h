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
HCPathRef HCPathUnion(HCPathRef self, HCPathRef other);
HCPathRef HCPathIntersection(HCPathRef self, HCPathRef other);
HCPathRef HCPathSubtraction(HCPathRef self, HCPathRef other);
HCPathRef HCPathExclusion(HCPathRef self, HCPathRef other);

#endif /* HCPath_BooleanOperations_h */
