///
/// @file HCColor.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 3/6/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCColor_h
#define HCColor_h

#include "../Core/HCCore.h"
#include <math.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Type
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
#define HCColorInvalidStatic    ((HCColor){.a = NAN, .r = NAN, .g = NAN, .b = NAN})
#define HCColorClearStatic      ((HCColor){.a = 0.0, .r = 0.0, .g = 0.0, .b = 0.0})
#define HCColorBlackStatic      ((HCColor){.a = 1.0, .r = 0.0, .g = 0.0, .b = 0.0})
#define HCColorWhiteStatic      ((HCColor){.a = 1.0, .r = 1.0, .g = 1.0, .b = 1.0})
#define HCColorGrayStatic       ((HCColor){.a = 1.0, .r = 0.5, .g = 0.5, .b = 0.5})
#define HCColorLightGrayStatic  ((HCColor){.a = 1.0, .r = 0.8, .g = 0.8, .b = 0.8})
#define HCColorDarkGrayStatic   ((HCColor){.a = 1.0, .r = 0.2, .g = 0.2, .b = 0.2})
#define HCColorRedStatic        ((HCColor){.a = 1.0, .r = 1.0, .g = 0.0, .b = 0.0})
#define HCColorGreenStatic      ((HCColor){.a = 1.0, .r = 0.0, .g = 1.0, .b = 0.0})
#define HCColorBlueStatic       ((HCColor){.a = 1.0, .r = 0.0, .g = 0.0, .b = 1.0})
#define HCColorCyanStatic       ((HCColor){.a = 1.0, .r = 0.0, .g = 1.0, .b = 1.0})
#define HCColorMagentaStatic    ((HCColor){.a = 1.0, .r = 1.0, .g = 0.0, .b = 1.0})
#define HCColorYellowStatic     ((HCColor){.a = 1.0, .r = 1.0, .g = 1.0, .b = 0.0})

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
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCColor HCColorMake(HCReal a, HCReal r, HCReal g, HCReal b);
HCColor HCColorMakeWithAHSB(HCReal alpha, HCReal hue, HCReal saturation, HCReal brightness);
HCColor HCColorMakeWithARGB(uint32_t color);
HCColor HCColorWithAlpha(HCColor color, HCReal a);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
uint32_t HCColorAsARGB8888(HCColor color);

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

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Combinations
//----------------------------------------------------------------------------------------------------------------------------------
HCColor HCColorCombine(HCColor c0, HCColor c1, HCReal t);
HCColor HCColorCombine3(HCColor ca, HCReal ta, HCColor cb, HCReal tb, HCColor cc, HCReal tc);
HCColor HCColorCombine4(HCColor ca, HCReal ta, HCColor cb, HCReal tb, HCColor cc, HCReal tc, HCColor cd, HCReal td);

#endif /* HCColor_h */
