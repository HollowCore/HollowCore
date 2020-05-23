///
/// @file HCThread.c
/// @ingroup HollowCore
///
/// @author Braden Scothern
/// @date 3/20/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <unistd.h>

// Some of these tests will require valgrind to ensure that they work correctly.

void HCThreadTestFunctionEmpty(void* context) {
    (void)context; // Unused
}

void HCThreadTestFunctionIncrementCallCount(void* context) {
    HCInteger* callCount = context;
    (*callCount)++;
}

void HCThreadTestFunctionIsCurrentThread(void* context) {
    HCThreadRef* thread = context;
    ASSERT_TRUE(*thread == HCThreadGetCurrent());
}

#define HCThreadFunctionWhileNotCancelledSleep (100)
void HCThreadFunctionWhileNotCancelled(void* context) {
    HCInteger timeoutCount = (HCInteger)context;
    HCInteger count = 0;
    while (!HCThreadIsCancelled(HCThreadGetCurrent())) {
        usleep(HCThreadFunctionWhileNotCancelledSleep);
        count++;
        if (count > timeoutCount) {
            ASSERT_FAIL();
            return;
        }
    }
}

CTEST(HCThread, Create) {
    HCThreadRef thread = HCThreadCreate(HCThreadTestFunctionEmpty, NULL);
    HCRelease(thread);
}

CTEST(HCThread, CreateWithOptions) {
    
    HCThreadOption options[] = {
        HCThreadOptionReleaseContextOnDestroy,
        HCThreadOptionFreeContextOnDestroy,
        HCThreadOptionCancelOnDestroy,
        HCThreadOptionJoinOnDestroy,
    };
    
    for (HCInteger i = 0; i < (HCInteger)(sizeof(options) / sizeof(HCThreadOption)); i++) {
        HCThreadOption option = options[i];
        HCThreadRef thread = HCThreadCreateWithOptions(HCThreadTestFunctionEmpty, NULL, option);
        ASSERT_TRUE((HCThreadOptions(thread) & option) == option);
        ASSERT_TRUE((HCThreadOptions(thread) ^ option) == 0);
        HCRelease(thread);
    }
}

CTEST(HCThread, Start) {
    HCInteger callCount = 0;
    HCThreadRef thread = HCThreadCreate(HCThreadTestFunctionIncrementCallCount, &callCount);
    
    HCThreadExecute(thread);
    ASSERT_TRUE(!HCThreadIsJoined(thread));
    HCThreadJoin(thread);
    ASSERT_TRUE(HCThreadIsJoined(thread));

    ASSERT_TRUE(callCount == 1);
    
    HCRelease(thread);
}

CTEST(HCThread, Current) {
    HCThreadRef thread = NULL;
    thread = HCThreadCreate(HCThreadTestFunctionIsCurrentThread, &thread);
    
    HCThreadExecute(thread);
    HCThreadJoin(thread);
    
    HCRelease(thread);
}

CTEST(HCThread, IsExecutingThenCancel) {
    HCThreadRef thread = HCThreadCreate(HCThreadFunctionWhileNotCancelled, (void*)100);
    
    HCThreadExecute(thread);
    usleep(HCThreadFunctionWhileNotCancelledSleep * 2);
    ASSERT_TRUE(HCThreadIsExecuting(thread));
    HCThreadCancel(thread);
    HCThreadJoin(thread);
    ASSERT_TRUE(!HCThreadIsExecuting(thread));
    
    HCRelease(thread);
}

CTEST(HCThread, GetContext) {
    HCInteger context = 1;
    HCThreadRef thread = HCThreadCreate(HCThreadTestFunctionIncrementCallCount, &context);
    ASSERT_TRUE(HCThreadContext(thread) == &context);
    HCRelease(thread);
}

CTEST(HCThread, OptionReleaseContextOnDestroy) {
    HCStringRef string = HCStringCreateWithCString("");
    HCThreadRef thread = HCThreadCreateWithOptions(HCThreadTestFunctionEmpty, string, HCThreadOptionReleaseContextOnDestroy);
    HCRelease(thread);
}

CTEST(HCThread, OptionFreeContextOnDestroy) {
    HCInteger* i = malloc(sizeof(HCInteger));
    HCThreadRef thread = HCThreadCreateWithOptions(HCThreadTestFunctionEmpty, i, HCThreadOptionFreeContextOnDestroy);
    HCRelease(thread);
}

CTEST(HCThread, HCThreadOptionJoinOnDestroy) {
    HCThreadRef thread = HCThreadCreateWithOptions(HCThreadFunctionWhileNotCancelled, (void*)100, HCThreadOptionJoinOnDestroy);
    HCThreadExecute(thread);
    usleep(HCThreadFunctionWhileNotCancelledSleep * 2);
    ASSERT_TRUE(HCThreadIsExecuting(thread));
    HCThreadCancel(thread);
    HCRelease(thread);
}

CTEST(HCThread, OptionCancelOnDestroy) {
    HCThreadRef thread = HCThreadCreateWithOptions(HCThreadFunctionWhileNotCancelled, (void*)100, HCThreadOptionCancelOnDestroy);
    HCThreadExecute(thread);
    usleep(HCThreadFunctionWhileNotCancelledSleep * 2);
    ASSERT_TRUE(HCThreadIsExecuting(thread));
    HCRelease(thread);
}
