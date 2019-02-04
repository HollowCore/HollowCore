//
//  HCRaster_Internal.h
//  HollowCore
//
//  Created by Matt Stoker on 2/3/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

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
    HCRasterColor* pixels;
} HCRaster;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterInit(void* memory, HCInteger width, HCInteger height);
void HCRasterDestroy(HCRasterRef self);

#endif /* HCRaster_Internal_h */
