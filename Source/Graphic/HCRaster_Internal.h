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

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Color Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterColor HCRasterColorMake(HCReal a, HCReal r, HCReal g, HCReal b);
HCRasterColor HCRasterColorCombine(HCRasterColor c0, HCRasterColor c1, HCReal t);
HCRasterColor HCRasterColorCombine3(HCRasterColor ca, HCReal ta, HCRasterColor cb, HCReal tb, HCRasterColor cc, HCReal tc);

#endif /* HCRaster_Internal_h */
