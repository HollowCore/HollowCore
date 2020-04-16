//
//  HCPath+Shapes.h
//  HollowCore
//
//  Created by Matt Stoker on 4/15/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#ifndef HCPath_Shapes_h
#define HCPath_Shapes_h

#include "HCPath.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCPathRef HCPathCreateRectangle(HCRectangle rectangle);
HCPathRef HCPathCreateEllipse(HCRectangle ellipseBounds);

#endif /* HCPath_Shapes_h */
