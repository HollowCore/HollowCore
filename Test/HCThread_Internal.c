//
//  HCThread_Internal.c
//  Test
//
//  Created by Braden Scothern on 6/26/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include <unistd.h>
#include "../Source/HollowCore.h"
#include "../Source/Thread/HCThread_Internal.h"

HCThreadRef atExitRef = NULL;
void HCThreadFunctionAtExitClean() {
//    HCRelease(atExitRef);
};

void HCThreadFunctionDetatch(void* context) {
    HCThreadRef thread = HCThreadGetCurrent();
    pthread_detach(thread->pthread);
    atExitRef = thread;
    atexit(HCThreadFunctionAtExitClean);
}

CTEST(HCThread_Internal, Detatched) {
    HCThreadRef thread = HCThreadCreate(HCThreadFunctionDetatch, NULL);
    HCThreadStart(thread);
}
