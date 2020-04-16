//
//  HCLock.c
//  Test
//
//  Created by Braden Scothern on 4/12/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <unistd.h>

void* HCLockTestAcquiredExecute(void* context) {
    HCInteger* i = context;
    (*i)++;
    return NULL;
}

typedef struct MultiThreadedAcquiredAndRelinquishContext {
    HCLockRef lock;
    int i;
} MultiThreadedAcquiredAndRelinquishContext;

const HCInteger HCLockMultiThreadedAcquiredAndRelinquishIterationCount = 100;

void HCLockMultiThreadedAcquiredAndRelinquish(void* context) {
    MultiThreadedAcquiredAndRelinquishContext* multiThreadedAcquiredAndRelinquishContext = context;
    
    for (HCInteger i = 0; i < HCLockMultiThreadedAcquiredAndRelinquishIterationCount; i++) {
        // Make it so there is some stagger across the tests
        if (rand() % 4 == 0) {
            useconds_t seconds = rand() % 50 + 1;
            usleep(seconds);
        }
        HCLockAquire(multiThreadedAcquiredAndRelinquishContext->lock);
        multiThreadedAcquiredAndRelinquishContext->i++;
        HCLockRelinquish(multiThreadedAcquiredAndRelinquishContext->lock);
    }
}

CTEST(HCLock, Create) {
    HCLockRef lock = HCLockCreate();
    HCRelease(lock);
}

CTEST(HCLock, AquireAndRelinquish) {
    HCLockRef lock = HCLockCreate();
    HCLockAquire(lock);
    HCLockRelinquish(lock);
    HCRelease(lock);
}

CTEST(HCLock, MultiAquireAndRelinquish) {
    HCLockRef lock = HCLockCreate();
    for (int i = 0; i < 10000; i++) {
        HCLockAquire(lock);
        HCLockRelinquish(lock);
    }
    HCRelease(lock);
}

CTEST(HCLock, AcquiredExecute) {
    HCLockRef lock = HCLockCreate();
    HCInteger counter = 0;
    HCInteger iterationCount = 10000;
    for (HCInteger i = 0; i < iterationCount; i++) {
        HCLockExecuteAcquired(lock, HCLockTestAcquiredExecute, &counter);
    }
    
    ASSERT_TRUE(counter == iterationCount);
    
    HCRelease(lock);
}

CTEST(HCLock, MultiThreadedAcquiredAndRelinquish) {
    MultiThreadedAcquiredAndRelinquishContext context = {
        .lock = HCLockCreate(),
        .i = 0,
    };
    const HCInteger numberOfThreads = 10;
    HCListRef threads = HCListCreateWithCapacity(numberOfThreads);
    for (HCInteger i = 0; i < numberOfThreads; i++) {
        HCListAddObjectReleased(threads, HCThreadCreateWithOptions(HCLockMultiThreadedAcquiredAndRelinquish, &context, HCThreadOptionJoinOnDestroy));
    }

    for (HCListIterator i = HCListIterationBegin(threads); !HCListIterationHasEnded(&i); HCListIterationNext(&i)) {
        HCThreadStart(i.object);
    }
    HCRelease(threads);
    
    ASSERT_TRUE(context.i == HCLockMultiThreadedAcquiredAndRelinquishIterationCount * numberOfThreads);
    
    HCRelease(context.lock);
}
