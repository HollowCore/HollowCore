//
//  HCObject_Internal.h
//  HollowCore
//
//  Created by Matt Stoker on 12/28/18.
//  Copyright © 2018 HollowCore. All rights reserved.
//

#ifndef HCObject_Internal_h
#define HCObject_Internal_h

#include "HCObject.h"


//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
struct HCObject {
    const HCType* type;
    HCAtomicInteger referenceCount;
};
typedef struct HCObject HCObject;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCObjectInit(void* memory);
void HCObjectDestroy(HCObjectRef self);

#endif /* HCObject_Internal_h */
