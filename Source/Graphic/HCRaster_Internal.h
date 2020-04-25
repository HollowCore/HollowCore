///
/// @file HCRaster_Internal.h
/// @package HollowCore
///
/// @author Matt Stoker
/// @date 2/3/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCRaster_Internal_h
#define HCRaster_Internal_h

#include "../Core/HCObject_Internal.h"
#include "HCRaster.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCRaster {
    HCObject base;
    HCInteger width;
    HCInteger height;
    HCColor* pixels;
} HCRaster;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterInit(void* memory, HCInteger width, HCInteger height);
void HCRasterDestroy(HCRasterRef self);

#endif /* HCRaster_Internal_h */
