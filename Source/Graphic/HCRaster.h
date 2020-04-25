///
/// @file HCRaster.h
/// @package Test
///
/// @author Matt Stoker
/// @date 2/3/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCRaster_h
#define HCRaster_h

#include "../Core/HCObject.h"
#include "HCColor.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCRasterType;
typedef struct HCRaster* HCRasterRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
extern HCInteger HCRasterSizeMax;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterRef HCRasterCreate(HCInteger width, HCInteger height);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRasterIsEqual(HCRasterRef self, HCRasterRef other);
HCInteger HCRasterHashValue(HCRasterRef self);
void HCRasterPrint(HCRasterRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCRasterWidth(HCRasterRef self);
HCInteger HCRasterHeight(HCRasterRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Pixel Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCColor HCRasterPixelAt(HCRasterRef self, HCInteger xIndex, HCInteger yIndex);
void HCRasterSetPixelAt(HCRasterRef self, HCInteger xIndex, HCInteger yIndex, HCColor pixel);
void HCRasterPixelsAt(HCRasterRef self, HCInteger startXIndex, HCInteger startYIndex, HCInteger endXIndex, HCInteger endYIndex, HCColor* pixels);
void HCRasterSetPixelsAt(HCRasterRef self, HCInteger startXIndex, HCInteger startYIndex, HCInteger endXIndex, HCInteger endYIndex, const HCColor* pixels);
void HCRasterClear(HCRasterRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Lookup Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCColor HCRasterPixelNearest(HCRasterRef self, HCReal x, HCReal y);
HCColor HCRasterPixelFiltered(HCRasterRef self, HCReal x, HCReal y);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Data Extraction
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterExtractARGB8888(HCRasterRef self, uint32_t* pixels);

#endif /* HCRaster_h */
