//
//  HCPath+SVG.h
//  HollowCore
//
//  Created by Matt Stoker on 3/21/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#ifndef HCPath_SVG_h
#define HCPath_SVG_h

#include "HCPath.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCPathRef HCPathCreateWithSVGPathData(const char* path);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Parsing
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathParse(HCPathRef self, const char* path);
void HCPathAddCubicCurvesApproximatingArc(HCPathRef self, HCReal xr, HCReal yr, HCReal rotation, HCBoolean largeArc, HCBoolean sweep, HCReal x, HCReal y);

#endif /* HCPath_SVG_h */
