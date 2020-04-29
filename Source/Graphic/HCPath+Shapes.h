///
/// @file HCPath+Shapes.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 4/15/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCPath_Shapes_h
#define HCPath_Shapes_h

#include "HCPath.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCPathRef HCPathCreateRectangle(HCRectangle rectangle);
HCPathRef HCPathCreateEllipse(HCRectangle ellipseBounds);

#endif /* HCPath_Shapes_h */
