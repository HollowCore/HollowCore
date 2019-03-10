//
//  HCSphere_Internal.h
//  HollowCore
//
//  Created by Matt Stoker on 3/5/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCSphere_Internal_h
#define HCSphere_Internal_h

#include "HCPrimitive_Internal.h"
#include "HCSphere.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCSphere {
    HCPrimitive base;
    HCVector center;
    HCReal radius;
} HCSphere;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCSphereInit(void* memory, HCVector center, HCReal radius);
void HCSphereDestroy(HCSphereRef self);

#endif /* HCSphere_Internal_h */
