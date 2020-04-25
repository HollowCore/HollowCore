///
/// @file HCSize.c
/// @package HollowCore
///
/// @author Matt Stoker
/// @date 10/19/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCSize.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCSize HCSizeMake(HCReal width, HCReal height) {
    return (HCSize){.width = width, .height = height};
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSizeIsInvalid(HCSize size) {
    return isnan(size.width) || isnan(size.height);
}

HCBoolean HCSizeIsSimilar(HCSize size, HCSize other, HCReal dimensionDissimilarity) {
    return
        HCRealIsSimilar(size.width, other.width, dimensionDissimilarity) &&
        HCRealIsSimilar(size.height, other.height, dimensionDissimilarity);
}

HCBoolean HCSizeIsEqual(HCSize size, HCSize other) {
    return
        HCRealIsEqual(size.width, other.width) &&
        HCRealIsEqual(size.height, other.height);
}

HCInteger HCSizeHashValue(HCSize size) {
    return
        HCRealHashValue(size.width) ^
        HCRealHashValue(size.height);
}

void HCSizePrint(HCSize size, FILE* stream) {
    fprintf(stream, "<w:%f,h:%f>", size.width, size.height);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Queries
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSizeIsZero(HCSize size) {
    return HCSizeIsEqual(size, HCSizeZero);
}

HCBoolean HCSizeIsInfinite(HCSize size) {
    return isinf(size.width) || isinf(size.height);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCSize HCSizeIncrease(HCSize size, HCReal dx, HCReal dy) {
    return HCSizeMake(size.width + dx, size.height + dy);
}

HCSize HCSizeDecrease(HCSize size, HCReal dx, HCReal dy) {
    return HCSizeIncrease(size, -dx, -dy);
}

HCSize HCSizeScale(HCSize size, HCReal scalar) {
    return HCSizeMake(size.width * scalar, size.height * scalar);
}
