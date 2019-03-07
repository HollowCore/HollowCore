//
//  HCColor.h
//  HollowCore
//
//  Created by Matt Stoker on 3/6/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCColor_h
#define HCColor_h

#include "../Core/HCCore.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCColor {
    HCReal a;
    HCReal r;
    HCReal g;
    HCReal b;
} HCColor;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Special Values
//----------------------------------------------------------------------------------------------------------------------------------
#define HCColorInvalidStatic    {.a = NAN, .r = NAN, .g = NAN, .b = NAN}
#define HCColorClearStatic      {.a = 0.0, .r = 0.0, .g = 0.0, .b = 0.0}
#define HCColorBlackStatic      {.a = 1.0, .r = 0.0, .g = 0.0, .b = 0.0}
#define HCColorWhiteStatic      {.a = 1.0, .r = 1.0, .g = 1.0, .b = 1.0}
#define HCColorGrayStatic       {.a = 1.0, .r = 0.5, .g = 0.5, .b = 0.5}
#define HCColorLightGrayStatic  {.a = 1.0, .r = 0.8, .g = 0.8, .b = 0.8}
#define HCColorDarkGrayStatic   {.a = 1.0, .r = 0.2, .g = 0.2, .b = 0.2}
#define HCColorRedStatic        {.a = 1.0, .r = 1.0, .g = 0.0, .b = 0.0}
#define HCColorGreenStatic      {.a = 1.0, .r = 0.0, .g = 1.0, .b = 0.0}
#define HCColorBlueStatic       {.a = 1.0, .r = 0.0, .g = 0.0, .b = 1.0}
#define HCColorCyanStatic       {.a = 1.0, .r = 0.0, .g = 1.0, .b = 1.0}
#define HCColorMagentaStatic    {.a = 1.0, .r = 1.0, .g = 0.0, .b = 1.0}
#define HCColorYellowStatic     {.a = 1.0, .r = 1.0, .g = 1.0, .b = 0.0}

static const HCColor HCColorInvalid = HCColorInvalidStatic;
static const HCColor HCColorClear = HCColorClearStatic;
static const HCColor HCColorBlack = HCColorBlackStatic;
static const HCColor HCColorWhite = HCColorWhiteStatic;
static const HCColor HCColorGray = HCColorGrayStatic;
static const HCColor HCColorLightGray = HCColorLightGrayStatic;
static const HCColor HCColorDarkGray = HCColorDarkGrayStatic;
static const HCColor HCColorRed = HCColorRedStatic;
static const HCColor HCColorGreen = HCColorGreenStatic;
static const HCColor HCColorBlue = HCColorBlueStatic;
static const HCColor HCColorCyan = HCColorCyanStatic;
static const HCColor HCColorMagenta = HCColorMagentaStatic;
static const HCColor HCColorYellow = HCColorYellowStatic;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCColor HCColorMake(HCReal a, HCReal r, HCReal g, HCReal b);
HCColor HCColorMakeWithAHSB(HCReal alpha, HCReal hue, HCReal saturation, HCReal brightness);
HCColor HCColorMakeWithARGB(uint32_t color);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
uint32_t HCColorAsARGB(HCColor color);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCColorIsInvalid(HCColor color);
HCBoolean HCColorIsClamped(HCColor color);
HCBoolean HCColorIsSimilar(HCColor color, HCColor other, HCReal channelDissimilarity);
HCBoolean HCColorIsEqual(HCColor color, HCColor other);
HCInteger HCColorHashValue(HCColor color);
void HCColorPrint(HCColor color, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCColor HCColorAdd(HCColor color, HCColor other);
HCColor HCColorSubtract(HCColor color, HCColor other);
HCColor HCColorMultiply(HCColor color, HCColor other);
HCColor HCColorScale(HCColor color, HCReal scalar);
HCColor HCColorClamp(HCColor color);
HCColor HCColorInterpolate(HCColor color, HCColor other, HCReal t);

#endif /* HCColor_h */
