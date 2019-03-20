//
//  HCVector.h
//  HollowCore
//
//  Created by Matt Stoker on 3/6/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCVector_h
#define HCVector_h

#include "../Core/HCCore.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCVector {
    HCReal x;
    HCReal y;
    HCReal z;
} HCVector;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCVector HCVectorMake(HCReal x, HCReal y, HCReal z);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCVectorIsInvalid(HCVector vector);
HCBoolean HCVectorIsSimilar(HCVector vector, HCVector other, HCReal axisDissimilarity);
HCBoolean HCVectorIsEqual(HCVector vector, HCVector other);
HCInteger HCVectorHashValue(HCVector vector);
void HCVectorPrint(HCVector vector, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCVector HCVectorAdd(HCVector vector, HCVector other);
HCVector HCVectorSubtract(HCVector vector, HCVector other);
HCVector HCVectorScale(HCVector vector, HCReal scalar);
HCReal HCVectorDot(HCVector vector, HCVector other);
HCVector HCVectorCross(HCVector vector, HCVector other);
HCReal HCVectorMagnitude(HCVector vector);
HCVector HCVectorNormalize(HCVector vector);

#endif /* HCVector_h */
