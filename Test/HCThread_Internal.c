///
/// @file HCThread_Internal.c
/// @ingroup HollowCore
///
/// @author Braden Scothern
/// @date 6/26/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "ctest.h"
#include <unistd.h>
#include "../Source/HollowCore.h"
#include "../Source/Thread/HCThread_Internal.h"

// This is work in progress research for having system threads that fully cleanup their memory at exit of the application.

HCThreadRef atExitRef = NULL;
void HCThreadFunctionAtExitClean() {
    HCRelease(atExitRef);
};

void HCThreadFunctionDetatch(void* context) {
    (void)context; // Unused
    HCThreadRef thread = HCThreadGetCurrent();
    pthread_detach(thread->pthread);
    atExitRef = thread;
    atexit(HCThreadFunctionAtExitClean);
}

CTEST(HCThread_Internal, Detatched) {
    HCThreadRef thread = HCThreadCreate(HCThreadFunctionDetatch, NULL);
    HCThreadExecute(thread);
}
