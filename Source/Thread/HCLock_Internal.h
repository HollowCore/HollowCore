//
//  HCLock_Internal.h
//  HollowCore
//
//  Created by Braden Scothern on 4/12/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCLock_Internal_h
#define HCLock_Internal_h

#include "../Core/HCObject_Internal.h"
#include "HCLock.h"
#include <pthread.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCLock {
    HCObject base;
    pthread_mutex_t mutex;
} HCLock;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCLockInit(void* memory);
void HCLockDestroy(HCLockRef self);

#endif /* HCLock_Internal_h */
