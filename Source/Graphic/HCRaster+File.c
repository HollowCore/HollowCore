///
/// @file HCRaster+File.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 4/18/20
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCRaster+File.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - File Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterRef HCRasterCreateByLoadingPPM(const char* path) {
    // Open a file for reading
    FILE* f = fopen(path, "r");
    if (f == NULL)
        return NULL;
    
    // Verify file magic number
    int magic0 = fgetc(f);
    int magic1 = fgetc(f);
    if (magic0 != 'P' || (magic1 != '3' && magic1 != '6')) {
        return NULL;
    }
    
    // Read and validate the size and channel scale from the header and consume the newline sequence
    int width = 0;
    int height = 0;
    int channelMax = 0;
    if (fscanf(f, "%i %i %i", &width, &height, &channelMax) != 3 || width <= 0 || height <= 0 || channelMax <= 0 || channelMax > 255) {
        return NULL;
    }
    if (fgetc(f) == '\r') {
        char c = fgetc(f);
        if (c != '\n') {
            ungetc(c, f);
        }
    }
    HCBoolean binary = magic1 == '6';
    
    // Trash any comment lines
    if (!binary) {
        while (true) {
            // Read a comment start character or return it to the stream and continue
            int c = fgetc(f);
            if (c != '#') {
                ungetc(c, f);
                break;
            }
            
            // Read until a newline, accounting for possible \r\n sequences
            while (true) {
                c = fgetc(f);
                if (c == '\n' || c == EOF) {
                    break;
                }
                if (c == '\r') {
                    c = fgetc(f);
                    if (c != '\n') {
                        ungetc(c, f);
                    }
                    break;
                }
            }
        }
    }
    
    // Allocate raster
    HCRasterRef self = HCRasterCreate(width, height);
    if (self == NULL) {
        return NULL;
    }
    
    // Read pixel information
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(self); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(self); xIndex++) {
            int r = 0;
            int g = 0;
            int b = 0;
            if (!binary) {
                fscanf(f, "%i %i %i", &r, &g, &b);
            }
            else {
                fread(&r, 1, 1, f);
                fread(&g, 1, 1, f);
                fread(&b, 1, 1, f);
            }
            HCColor color = HCColorMake(1.0, (HCReal)r / (HCReal)channelMax, (HCReal)g / (HCReal)channelMax, (HCReal)b / (HCReal)channelMax);
            HCRasterSetPixelAt(self, xIndex, yIndex, color);
        }
    }
    fclose(f);
    
    return self;
}

void HCRasterSavePPM(HCRasterRef self, const char* path) {
    HCRasterSavePPMWithOptions(self, path, false);
}

void HCRasterSavePPMWithOptions(HCRasterRef self, const char* path, HCBoolean binary) {
    // Open file for writing
    FILE* file = fopen(path, "w");
    if (file == NULL)
        return;
    
    // Write PPM Header
    fprintf(file, "P%c %i %i 255\n", binary ? '6' : '3', (int)HCRasterWidth(self), (int)HCRasterHeight(self));
    
    // Write comment (where not saving binary)
    if (!binary) {
        fprintf(file, "# Created by HollowCore\n");
    }
    
    // Write PPM body, top row first, in red, green, blue byte order
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(self); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(self); xIndex++) {
            HCColor pixel = HCRasterPixelAt(self, xIndex, yIndex);
            HCByte r = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.r * 256.0f)));
            HCByte g = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.g * 256.0f)));
            HCByte b = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.b * 256.0f)));
            if (!binary) {
                fprintf(file, "%i %i %i ", (int)r, (int)g, (int)b);
            }
            else {
                fwrite(&r, sizeof(HCByte), 1, file);
                fwrite(&g, sizeof(HCByte), 1, file);
                fwrite(&b, sizeof(HCByte), 1, file);
            }
        }
        if (!binary) {
            fprintf(file, "\n");
        }
    }
    
    // Cleanup
    fclose(file);
}

HCRasterRef HCRasterCreateByLoadingBMP(const char* path) {
    // BMP (Windows V3)
    // Offset    Size    Description
    // 0         2       the magic number used to identify the BMP file: 0x42 0x4D (Hex code points for B and M in big-endian order)
    // 2         4       the size of the BMP file in bytes
    // 6         2       reserved; actual value depends on the application that creates the image
    // 8         2       reserved; actual value depends on the application that creates the image
    // 10        4       the offset, i.e. starting address, of the byte where the bitmap data can be found.
    // 14        4       the size of this header (40 bytes)
    // 18        4       the bitmap width in pixels (signed integer).
    // 22        4       the bitmap height in pixels (signed integer).
    // 26        2       the number of color planes being used. Must be set to 1.
    // 28        2       the number of bits per pixel, which is the color samplesPerPixel of the image. Typical values are 1, 4, 8, 16, 24 and 32.
    // 30        4       the compression method being used. See the next table for a list of possible values.
    // 34        4       the image size. This is the size of the raw bitmap data (see below), and should not be confused with the file size.
    // 38        4       the horizontal resolution of the image. (pixel per meter, signed integer)
    // 42        4       the vertical resolution of the image. (pixel per meter, signed integer)
    // 46        4       the number of colors in the color palette, or 0 to default to 2n.
    // 50        4       the number of important colors used, or 0 when every color is important; generally ignored.
    // 51        4*w*h   pixel data in XXRRGGBB uint32 format
    
    // Open file for reading
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        return NULL;
    }
    
    // Define header data
    uint16_t magicNumber = 0x4D42;
    uint32_t fileSize = 0;
    uint16_t reserved0 = 0;//0x4D41;
    uint16_t reserved1 = 0;//0x5454;
    uint32_t dataOffset = 54;
    uint32_t infoHeaderSize = 40;
    int32_t width = 0;
    int32_t height = 0;
    //uint16_t colorPlanes = 1;
    //uint16_t bitsPerPixel = 32;
    //uint32_t compression = 0;
    //uint32_t dataSize = width * height * bitsPerPixel / 8;
    //int32_t horizontalResolution = 2835;
    //int32_t verticalResolution = 2835;
    //uint32_t paletteColorCount = 0;
    //uint32_t importantPaletteColorCount = 0;
    
    // Read header data
    fread(&magicNumber, sizeof(uint16_t), 1, f);
    fread(&fileSize, sizeof(uint32_t), 1, f);
    fread(&reserved0, sizeof(uint16_t), 1, f);
    fread(&reserved1, sizeof(uint16_t), 1, f);
    fread(&dataOffset, sizeof(uint32_t), 1, f);
    fread(&infoHeaderSize, sizeof(uint32_t), 1, f);
    fread(&width, sizeof(int32_t), 1, f);
    fread(&height, sizeof(int32_t), 1, f);
    fseek(f, dataOffset, SEEK_SET);
    //fread(&colorPlanes, sizeof(uint16_t), 1, f);
    //fread(&bitsPerPixel, sizeof(uint16_t), 1, f);
    //fread(&compression, sizeof(uint32_t), 1, f);
    //fread(&dataSize, sizeof(uint32_t), 1, f);
    //fread(&horizontalResolution, sizeof(uint32_t), 1, f);
    //fread(&verticalResolution, sizeof(uint32_t), 1, f);
    //fread(&paletteColorCount, sizeof(uint32_t), 1, f);
    //fread(&importantPaletteColorCount, sizeof(uint32_t), 1, f);
    
    //TODO: Check header for sanity, support non-32bpp
    
    // Verify width and height
    HCBoolean reversed = height < 0;
    width = width < 0 ? -width : width;
    height = height < 0 ? -height : height;
    if (width <= 0 || height <= 0) {
        return NULL;
    }
    
    // Allocate raster
    HCRasterRef self = HCRasterCreate(width, height);
    if (self == NULL) {
        return NULL;
    }
    
    // Read BMP body (XXRRGGBB), respecting reversed boolean (bottom-to-top or top-to-bottom line ordering)
    for (HCInteger yIndex = reversed ? 0 : (HCRasterHeight(self) - 1); reversed ? (yIndex < HCRasterHeight(self)) : (yIndex >= 0); reversed ? yIndex++ : yIndex--) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(self); xIndex++) {
            uint32_t colorARGB = 0;
            fread(&colorARGB, sizeof(uint32_t), 1, f);
            HCColor color = HCColorMake(
                ((colorARGB & 0xFF000000) >> 24) / 255.0,
                ((colorARGB & 0x00FF0000) >> 16) / 255.0,
                ((colorARGB & 0x0000FF00) >>  8) / 255.0,
                ((colorARGB & 0x000000FF) >>  0) / 255.0);
            HCRasterSetPixelAt(self, xIndex, yIndex, color);
        }
    }
    
    // Cleanup
    fclose(f);
    
    return self;
}

void HCRasterSaveBMP(HCRasterRef self, const char* path) {
    HCRasterSaveBMPWithOptions(self, path, false);
}

void HCRasterSaveBMPWithOptions(HCRasterRef self, const char* path, HCBoolean reversed) {
    // BMP (Windows V3)
    // Offset    Size    Description
    // 0         2       the magic number used to identify the BMP file: 0x42 0x4D (Hex code points for B and M in big-endian order)
    // 2         4       the size of the BMP file in bytes
    // 6         2       reserved; actual value depends on the application that creates the image
    // 8         2       reserved; actual value depends on the application that creates the image
    // 10        4       the offset, i.e. starting address, of the byte where the bitmap data can be found.
    // 14        4       the size of this header (40 bytes)
    // 18        4       the bitmap width in pixels (signed integer).
    // 22        4       the bitmap height in pixels (signed integer).
    // 26        2       the number of color planes being used. Must be set to 1.
    // 28        2       the number of bits per pixel, which is the color samplesPerPixel of the image. Typical values are 1, 4, 8, 16, 24 and 32.
    // 30        4       the compression method being used. See the next table for a list of possible values.
    // 34        4       the image size. This is the size of the raw bitmap data (see below), and should not be confused with the file size.
    // 38        4       the horizontal resolution of the image. (pixel per meter, signed integer)
    // 42        4       the vertical resolution of the image. (pixel per meter, signed integer)
    // 46        4       the number of colors in the color palette, or 0 to default to 2n.
    // 50        4       the number of important colors used, or 0 when every color is important; generally ignored.
    // 51        4*w*h   pixel data in XXRRGGBB uint32 format
    
    // Open file for writing
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        return;
    }
    
    // Define header data, repecting reversed boolean in the height parameter
    uint16_t magicNumber = 0x4D42;
    uint16_t reserved0 = 0;//0x4D41;
    uint16_t reserved1 = 0;//0x5454;
    uint32_t dataOffset = 54;
    uint32_t infoHeaderSize = 40;
    int32_t width = (int32_t)HCRasterWidth(self);
    int32_t height = (int32_t)HCRasterHeight(self) * (reversed ? -1 : 1);
    uint16_t colorPlanes = 1;
    uint16_t bitsPerPixel = 32;
    uint32_t compression = 0;
    uint32_t dataSize = width * height * bitsPerPixel / 8;
    uint32_t horizontalResolution = 2835;
    uint32_t verticalResolution = 2835;
    uint32_t paletteColorCount = 0;
    uint32_t importantPaletteColorCount = 0;
    uint32_t fileSize = 54 + dataSize;
    
#ifdef __BIG_ENDIAN__
    // Change from big-endian to little-endian byte ordering
#define LE_32(data) (((data & 0x000000FF) << 24) | ((data & 0x0000FF00) << 8) | ((data & 0x00FF0000) >> 8) | ((data >> 24) && 0x000000FF))
#define LE_16(data) (((data & 0x00FF) << 8) | ((data >> 8) & 0x00FF))
#define PACK_PIXEL(alpha, red, green, blue) (0x00000000 | ((uint32_t)blue << 24) | ((uint32_t)green << 16) | ((uint32_t)red << 8) | ((uint32_t)alpha << 0))
    magicNumber = LE_16(magicNumber);
    reserved0 = LE_16(reserved0);
    reserved1 = LE_16(reserved1);
    dataOffset = LE_32(dataOffset);
    infoHeaderSize = LE_32(infoHeaderSize);
    width = LE_32(width);
    height = LE_32(height);
    colorPlanes = LE_16(colorPlanes);
    bitsPerPixel = LE_16(bitsPerPixel);
    compression = LE_32(compression);
    dataSize = LE_32(dataSize);
    horizontalResolution = LE_32(horizontalResolution);
    verticalResolution = LE_32(verticalResolution);
    paletteColorCount = LE_32(paletteColorCount);
    importantPaletteColorCount = LE_32(importantPaletteColorCount);
    fileSize = LE_32(fileSize);
#else
#define PACK_PIXEL(alpha, red, green, blue) (((uint32_t)alpha << 24)  | ((uint32_t)red << 16) | ((uint32_t)green << 8) | ((uint32_t)blue))
#endif //__BIG_ENDIAN__
    
    // Write BMP header (Windows V3, 32bbp)
    fwrite(&magicNumber, sizeof(magicNumber), 1, file);
    fwrite(&fileSize, sizeof(fileSize), 1, file);
    fwrite(&reserved0, sizeof(reserved0), 1, file);
    fwrite(&reserved1, sizeof(reserved1), 1, file);
    fwrite(&dataOffset, sizeof(dataOffset), 1, file);
    fwrite(&infoHeaderSize, sizeof(infoHeaderSize), 1, file);
    fwrite(&width, sizeof(width), 1, file);
    fwrite(&height, sizeof(height), 1, file);
    fwrite(&colorPlanes, sizeof(colorPlanes), 1, file);
    fwrite(&bitsPerPixel, sizeof(bitsPerPixel), 1, file);
    fwrite(&compression, sizeof(compression), 1, file);
    fwrite(&dataSize, sizeof(dataSize), 1, file);
    fwrite(&horizontalResolution, sizeof(horizontalResolution), 1, file);
    fwrite(&verticalResolution, sizeof(verticalResolution), 1, file);
    fwrite(&paletteColorCount, sizeof(paletteColorCount), 1, file);
    fwrite(&importantPaletteColorCount, sizeof(importantPaletteColorCount), 1, file);
    
    // Write BMP body (XXRRGGBB), respecting reversed boolean (bottom-to-top or top-to-bottom line ordering)
    for (HCInteger yIndex = reversed ? 0 : (HCRasterHeight(self) - 1); reversed ? (yIndex < HCRasterHeight(self)) : (yIndex >= 0); reversed ? yIndex++ : yIndex--) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(self); xIndex++) {
            HCColor pixel = HCRasterPixelAt(self, xIndex, yIndex);
            HCByte a = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.a * 256.0f)));
            HCByte r = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.r * 256.0f)));
            HCByte g = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.g * 256.0f)));
            HCByte b = (HCByte)fmax(0.0f, fmin(255.0f, floor(pixel.b * 256.0f)));
            uint32_t p = PACK_PIXEL(a, r, g, b);
            fwrite(&p, sizeof(p), 1, file);
        }
    }
    
    // Cleanup
    fclose(file);
}
