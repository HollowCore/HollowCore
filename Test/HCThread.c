//
//  HCThread.c
//  Test
//
//  Created by Braden Scothern on 3/20/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <unistd.h>

// Some of these tests will require valgrind to ensure that they work correctly.

void HCThreadTestFunctionEmpty(void* context) {
}

void HCThreadTestFunctionIncrementCallCount(void* context) {
    int* callCount = context;
    (*callCount)++;
}

void HCThreadTestFunctionIsCurrentThread(void* context) {
    HCThreadRef* thread = context;
    ASSERT_TRUE(*thread == HCThreadGetCurrent());
}

#define HCThreadFunctionWhileNotCanceledSleep (100)
void HCThreadFunctionWhileNotCanceled(void* context) {
    HCInteger timeoutCount = (HCInteger)context;
    int count = 0;
    while (!HCThreadIsCanceled(HCThreadGetCurrent())) {
        usleep(HCThreadFunctionWhileNotCanceledSleep);
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
    
    for (int i = 0; i < sizeof(options) / sizeof(HCThreadOption); i++) {
        HCThreadOption option = options[i];
        HCThreadRef thread = HCThreadCreateWithOptions(HCThreadTestFunctionEmpty, NULL, option);
        ASSERT_TRUE((HCThreadGetOptions(thread) & option) == option);
        ASSERT_TRUE((HCThreadGetOptions(thread) ^ option) == 0);
        HCRelease(thread);
    }
}

CTEST(HCThread, Start) {
    int callCount = 0;
    HCThreadRef thread = HCThreadCreate(HCThreadTestFunctionIncrementCallCount, &callCount);
    
    HCThreadStart(thread);
    ASSERT_TRUE(!HCThreadIsJoined(thread));
    HCThreadJoin(thread);
    ASSERT_TRUE(HCThreadIsJoined(thread));

    ASSERT_TRUE(callCount == 1);
    
    HCRelease(thread);
}

CTEST(HCThread, Current) {
    HCThreadRef thread = NULL;
    thread = HCThreadCreate(HCThreadTestFunctionIsCurrentThread, &thread);
    
    HCThreadStart(thread);
    HCThreadJoin(thread);
    
    HCRelease(thread);
}

CTEST(HCThread, IsExecutingThenCancel) {
    HCThreadRef thread = HCThreadCreate(HCThreadFunctionWhileNotCanceled, (void*)100);
    
    HCThreadStart(thread);
    usleep(HCThreadFunctionWhileNotCanceledSleep * 2);
    ASSERT_TRUE(HCThreadIsExecuting(thread));
    HCThreadCancel(thread);
    HCThreadJoin(thread);
    ASSERT_TRUE(!HCThreadIsExecuting(thread));
    
    HCRelease(thread);
}

CTEST(HCThread, GetContext) {
    int context = 1;
    HCThreadRef thread = HCThreadCreate(HCThreadTestFunctionIncrementCallCount, &context);
    ASSERT_TRUE(HCThreadGetContext(thread) == &context);
    HCRelease(thread);
}

CTEST(HCThread, OptionReleaseContextOnDestroy) {
    HCStringRef string = HCStringCreateWithCString("");
    HCThreadRef thread = HCThreadCreateWithOptions(HCThreadTestFunctionEmpty, string, HCThreadOptionReleaseContextOnDestroy);
    HCRelease(thread);
}

CTEST(HCThread, OptionFreeContextOnDestroy) {
    int* i = malloc(sizeof(int));
    HCThreadRef thread = HCThreadCreateWithOptions(HCThreadTestFunctionEmpty, i, HCThreadOptionFreeContextOnDestroy);
    HCRelease(thread);
}

CTEST(HCThread, HCThreadOptionJoinOnDestroy) {
    HCThreadRef thread = HCThreadCreateWithOptions(HCThreadFunctionWhileNotCanceled, (void*)100, HCThreadOptionJoinOnDestroy);
    HCThreadStart(thread);
    usleep(HCThreadFunctionWhileNotCanceledSleep * 2);
    ASSERT_TRUE(HCThreadIsExecuting(thread));
    HCThreadCancel(thread);
    HCRelease(thread);
}

CTEST(HCThread, OptionCancelOnDestroy) {
    HCThreadRef thread = HCThreadCreateWithOptions(HCThreadFunctionWhileNotCanceled, (void*)100, HCThreadOptionCancelOnDestroy);
    HCThreadStart(thread);
    usleep(HCThreadFunctionWhileNotCanceledSleep * 2);
    ASSERT_TRUE(HCThreadIsExecuting(thread));
    HCRelease(thread);
}
