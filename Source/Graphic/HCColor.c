///
/// @file HCColor.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 3/6/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCColor.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCColor HCColorMake(HCReal a, HCReal r, HCReal g, HCReal b) {
    HCColor color = {.a = a, .r = r, .g = g, .b = b};
    return color;
}

HCColor HCColorMakeWithAHSB(HCReal alpha, HCReal hue, HCReal saturation, HCReal brightness) {
    HCReal scaledHue = hue * 360.0;
    HCReal c = brightness * saturation;
    HCReal x = c * (1.0 - fabs(fmod(scaledHue / 60.0, 2.0) - 1.0));
    HCReal m = brightness - c;
    
    if ((scaledHue >= 0.0) && (scaledHue < 60.0)) {
        return (HCColor){.a = alpha, .r = c + m, .g = x + m, .b = m};
    }
    else if ((scaledHue >= 60.0) && (scaledHue < 120.0)) {
        return (HCColor){.a = alpha, .r = x + m, .g = c + m, .b = m};
    }
    else if ((scaledHue >= 120.0) && (scaledHue < 180.0)) {
        return (HCColor){.a = alpha, .r = m, .g = c + m, .b = x + m};
    }
    else if ((scaledHue >= 180.0) && (scaledHue < 240.0)) {
        return (HCColor){.a = alpha, .r = m, .g = x + m, .b = c + m};
    }
    else if ((scaledHue >= 240.0) && (scaledHue < 300.0)) {
        return (HCColor){.a = alpha, .r = x + m, .g = m, .b = c + m};
    }
    else if ((scaledHue >= 300.0) && (scaledHue < 360.0)) {
        return (HCColor){.a = alpha, .r = c + m, .g = m, .b = x + m};
    }
    else {
        return (HCColor){.a = alpha, .r = m, .g = m, .b = m};
    }
}

HCColor HCColorMakeWithARGB(uint32_t color) {
    return (HCColor) {
        .a = (HCReal)((0xFF000000 & color) >> 24) / 255.0,
        .r = (HCReal)((0x00FF0000 & color) >> 16) / 255.0,
        .g = (HCReal)((0x0000FF00 & color) >>  8) / 255.0,
        .b = (HCReal)((0x000000FF & color) >>  0) / 255.0,
    };
}

HCColor HCColorWithAlpha(HCColor color, HCReal a) {
    return HCColorMake(a, color.r, color.g, color.b);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
uint32_t HCColorAsARGB8888(HCColor color) {
    return
        (uint32_t)fmax(0.0f, fmin(255.0f, color.a * 256.0f)) << 24 |
        (uint32_t)fmax(0.0f, fmin(255.0f, color.r * 256.0f)) << 16 |
        (uint32_t)fmax(0.0f, fmin(255.0f, color.g * 256.0f)) <<  8 |
        (uint32_t)fmax(0.0f, fmin(255.0f, color.b * 256.0f)) <<  0;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCColorIsInvalid(HCColor color) {
    return isnan(color.a) || isnan(color.r) || isnan(color.g) || isnan(color.b);
}

HCBoolean HCColorIsClamped(HCColor color) {
    return !HCColorIsInvalid(color) && (color.a >= 0.0) && (color.a <= 1.0) && (color.r >= 0.0) && (color.r <= 1.0) && (color.g >= 0.0) && (color.g <= 1.0) && (color.b >= 0.0) && (color.b <= 1.0);
}

HCBoolean HCColorIsSimilar(HCColor color, HCColor other, HCReal channelDissimilarity) {
    return
        HCRealIsSimilar(color.a, other.a, channelDissimilarity) &&
        HCRealIsSimilar(color.r, other.r, channelDissimilarity) &&
        HCRealIsSimilar(color.g, other.g, channelDissimilarity) &&
        HCRealIsSimilar(color.b, other.b, channelDissimilarity);
}

HCBoolean HCColorIsEqual(HCColor color, HCColor other) {
    return
        HCRealIsEqual(color.a, other.a) &&
        HCRealIsEqual(color.r, other.r) &&
        HCRealIsEqual(color.g, other.g) &&
        HCRealIsEqual(color.b, other.b);
}

HCInteger HCColorHashValue(HCColor color) {
    return
        HCRealHashValue(color.a) ^
        HCRealHashValue(color.r) ^
        HCRealHashValue(color.g) ^
        HCRealHashValue(color.b);
}

void HCColorPrint(HCColor color, FILE* stream) {
    fprintf(stream, "<r:%f,g:%f,b:%f,a:%f>", color.a, color.r, color.g, color.b);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCColor HCColorAdd(HCColor color, HCColor other) {
    return HCColorMake(color.a + other.a, color.r + other.r, color.g + other.g, color.b + other.b);
}

HCColor HCColorSubtract(HCColor color, HCColor other) {
    return HCColorMake(color.a - other.a, color.r - other.r, color.g - other.g, color.b - other.b);
}

HCColor HCColorMultiply(HCColor color, HCColor other) {
    return HCColorMake(color.a * other.a, color.r * other.r, color.g * other.g, color.b * other.b);
}

HCColor HCColorScale(HCColor color, HCReal scalar) {
    return HCColorMake(color.a * scalar, color.r * scalar, color.g * scalar, color.b * scalar);
}

HCColor HCColorClamp(HCColor color) {
    return HCColorMake(fmax(0.0, fmin(1.0, color.a)), fmax(0.0, fmin(1.0, color.r)), fmax(0.0, fmin(1.0, color.g)), fmax(0.0, fmin(1.0, color.b)));
}

HCColor HCColorInterpolate(HCColor color, HCColor other, HCReal t) {
    HCReal tc = 1.0 - t;
    return HCColorMake(tc * color.a + t * other.a, tc * color.r + t * other.r, tc * color.g + t * other.g, tc * color.b + t * other.b);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Combinations
//----------------------------------------------------------------------------------------------------------------------------------
HCColor HCColorCombine(HCColor c0, HCColor c1, HCReal t) {
    HCReal tc = 1.0 - t;
    HCReal a = tc * c0.a + t * c1.a;
    HCReal r = tc * c0.r + t * c1.r;
    HCReal g = tc * c0.g + t * c1.g;
    HCReal b = tc * c0.b + t * c1.b;
    return HCColorMake(a, r, g, b);
}

HCColor HCColorCombine3(HCColor ca, HCReal ta, HCColor cb, HCReal tb, HCColor cc, HCReal tc) {
    HCReal a = ta * ca.a + tb * cb.a + tc * cc.a;
    HCReal r = ta * ca.r + tb * cb.r + tc * cc.r;
    HCReal g = ta * ca.g + tb * cb.g + tc * cc.g;
    HCReal b = ta * ca.b + tb * cb.b + tc * cc.b;
    return HCColorMake(a, r, g, b);
}

HCColor HCColorCombine4(HCColor ca, HCReal ta, HCColor cb, HCReal tb, HCColor cc, HCReal tc, HCColor cd, HCReal td) {
    HCReal a = ta * ca.a + tb * cb.a + tc * cc.a + td * cd.a;
    HCReal r = ta * ca.r + tb * cb.r + tc * cc.r + td * cd.r;
    HCReal g = ta * ca.g + tb * cb.g + tc * cc.g + td * cd.g;
    HCReal b = ta * ca.b + tb * cb.b + tc * cc.b + td * cd.b;
    return HCColorMake(a, r, g, b);
}
