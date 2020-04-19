//
//  HCRaster+File.h
//  HollowCore
//
//  Created by Matt Stoker on 4/18/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#ifndef HCRaster_File_h
#define HCRaster_File_h

#include "HCRaster.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - File Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterRef HCRasterCreateByLoadingPPM(const char* path);
void HCRasterSavePPM(HCRasterRef self, const char* path);
void HCRasterSavePPMWithOptions(HCRasterRef self, const char* path, HCBoolean binary);
HCRasterRef HCRasterCreateByLoadingBMP(const char* path);
void HCRasterSaveBMP(HCRasterRef self, const char* path);
void HCRasterSaveBMPWithOptions(HCRasterRef self, const char* path, HCBoolean reverse);

#endif /* HCRaster_File_h */
