//
//  HCSize.h
//  HollowCore
//
//  Created by Matt Stoker on 10/19/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCSize_h
#define HCSize_h

#include "../Core/HCCore.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCSize {
    HCReal width;
    HCReal height;
} HCSize;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Special Values
//----------------------------------------------------------------------------------------------------------------------------------
#define HCSizeInvalidStatic   {.width = NAN, .height = NAN}
#define HCSizeZeroStatic      {.width = 0.0, .height = 0.0}

static const HCSize HCSizeInvalid = HCSizeInvalidStatic;
static const HCSize HCSizeZero = HCSizeZeroStatic;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Constructors
//----------------------------------------------------------------------------------------------------------------------------------
HCSize HCSizeMake(HCReal width, HCReal height);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Equality
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSizeIsInvalid(HCSize size);
HCBoolean HCSizeIsSimilar(HCSize size, HCSize other, HCReal dimensionDissimilarity);
HCBoolean HCSizeIsEqual(HCSize size, HCSize other);
HCInteger HCSizeHashValue(HCSize size);
void HCSizePrint(HCSize size, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Queries
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSizeIsZero(HCSize size);
HCBoolean HCSizeIsInfinite(HCSize size);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCSize HCSizeIncrease(HCSize size, HCReal dx, HCReal dy);
HCSize HCSizeDecrease(HCSize size, HCReal dx, HCReal dy);
HCSize HCSizeScale(HCSize size, HCReal scalar);

#endif /* HCSize_h */
