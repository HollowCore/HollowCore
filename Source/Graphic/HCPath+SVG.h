///
/// @file HCPath+SVG.h
/// @package HollowCore
///
/// @author Matt Stoker
/// @date 3/21/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCPath_SVG_h
#define HCPath_SVG_h

#include "HCPath.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCPathRef HCPathCreateWithSVGPathData(const char* path);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - SVG Path Data Conversion
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathPrintSVGPathData(HCPathRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - SVG Path Data Parsing
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathParseSVGPathData(HCPathRef self, const char* path);
void HCPathAddCubicCurvesApproximatingArc(HCPathRef self, HCReal xr, HCReal yr, HCReal rotation, HCBoolean largeArc, HCBoolean sweep, HCReal x, HCReal y);

#endif /* HCPath_SVG_h */
