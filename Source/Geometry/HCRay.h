//
//  HCRay.h
//  HollowCore
//
//  Created by Matt Stoker on 3/10/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCRay_h
#define HCRay_h

#include "HCVector.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCRay {
    HCVector origin;
    HCVector direction;
} HCRay;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Special Values
//----------------------------------------------------------------------------------------------------------------------------------
#define HCRayInvalidStatic   {.origin = HCVectorInvalidStatic, .direction = HCVectorInvalidStatic}
#define HCRayZeroStatic      {.origin = HCVectorZeroStatic, .direction = HCVectorZeroStatic}

static const HCRay HCRayInvalid = HCRayInvalidStatic;
static const HCRay HCRayZero = HCRayZeroStatic;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCRay HCRayMake(HCVector origin, HCVector direction);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRayIsInvalid(HCRay ray);
HCBoolean HCRayIsSimilar(HCRay ray, HCRay other, HCReal axisDissimilarity);
HCBoolean HCRayIsEqual(HCRay ray, HCRay other);
HCInteger HCRayHashValue(HCRay ray);
void HCRayPrint(HCRay ray, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Queries
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRayIsZero(HCRay ray);
HCBoolean HCRayIsInfinite(HCRay ray);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCVector HCRayPointAt(HCRay ray, HCReal scale);

#endif /* HCRay_h */
