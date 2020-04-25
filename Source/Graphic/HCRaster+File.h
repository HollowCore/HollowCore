///
/// @file HCRaster+File.h
/// @package HollowCore
///
/// @author Matt Stoker
/// @date 4/18/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCRaster_File_h
#define HCRaster_File_h

#include "HCRaster.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - File Operations
//----------------------------------------------------------------------------------------------------------------------------------
// TODO: Make these work with streams instead of files directly
HCRasterRef HCRasterCreateByLoadingPPM(const char* path);
void HCRasterSavePPM(HCRasterRef self, const char* path);
void HCRasterSavePPMWithOptions(HCRasterRef self, const char* path, HCBoolean binary);
HCRasterRef HCRasterCreateByLoadingBMP(const char* path);
void HCRasterSaveBMP(HCRasterRef self, const char* path);
void HCRasterSaveBMPWithOptions(HCRasterRef self, const char* path, HCBoolean reverse);

#endif /* HCRaster_File_h */
