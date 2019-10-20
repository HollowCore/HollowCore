//
//  HCVector.c
//  HollowCore
//
//  Created by Matt Stoker on 3/6/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCVector.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCVector HCVectorMake(HCReal x, HCReal y, HCReal z) {
    return (HCVector){.x = x, .y = y, .z = z};
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCVectorIsInvalid(HCVector vector) {
    return isnan(vector.x) || isnan(vector.y) || isnan(vector.z);
}

HCBoolean HCVectorIsSimilar(HCVector vector, HCVector other, HCReal axisDissimilarity) {
    return
        HCRealIsSimilar(vector.x, other.x, axisDissimilarity) &&
        HCRealIsSimilar(vector.y, other.y, axisDissimilarity) &&
        HCRealIsSimilar(vector.z, other.z, axisDissimilarity);
}

HCBoolean HCVectorIsEqual(HCVector vector, HCVector other) {
    return
        HCRealIsEqual(vector.x, other.x) &&
        HCRealIsEqual(vector.y, other.y) &&
        HCRealIsEqual(vector.z, other.z);
}

HCInteger HCVectorHashValue(HCVector vector) {
    return
        HCRealHashValue(vector.x) ^
        HCRealHashValue(vector.y) ^
        HCRealHashValue(vector.z);
}

void HCVectorPrint(HCVector vector, FILE* stream) {
    fprintf(stream, "<x:%f,y:%f,z:%f>", vector.x, vector.y, vector.z);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Queries
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCVectorIsZero(HCVector vector) {
    return HCVectorIsEqual(vector, HCVectorZero);
}

HCBoolean HCVectorIsInfinite(HCVector vector) {
    return isinf(vector.x) || isinf(vector.y) || isinf(vector.z);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCVector HCVectorAdd(HCVector vector, HCVector other) {
    return HCVectorMake(vector.x + other.x, vector.y + other.y, vector.z + other.z);
}

HCVector HCVectorSubtract(HCVector vector, HCVector other) {
    return HCVectorMake(vector.x - other.x, vector.y - other.y, vector.z - other.z);
}

HCVector HCVectorScale(HCVector v, HCReal scalar) {
    return HCVectorMake(v.x * scalar, v.y * scalar, v.z * scalar);
}

HCReal HCVectorDot(HCVector vector, HCVector other) {
    return vector.x * other.x + vector.y * other.y + vector.z * other.z;
}

HCVector HCVectorCross(HCVector vector, HCVector other) {
    return HCVectorMake(vector.y * other.z - vector.z * other.y, vector.z * other.x - vector.x * other.z, vector.x * other.y - vector.y * other.x);
}

HCReal HCVectorMagnitude(HCVector vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

HCVector HCVectorNormalize(HCVector vector) {
    return HCVectorScale(vector, 1.0 / HCVectorMagnitude(vector));
}
