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

void* HCLockTestAquiredExecute(void* context) {
    HCInteger* i = context;
    (*i)++;
    return NULL;
}

typedef struct MultiThreadedAquiredAndRelinquishContext {
    HCLockRef lock;
    int i;
} MultiThreadedAquiredAndRelinquishContext;

const HCInteger HCLockMultiThreadedAquiredAndRelinquishIterationCount = 100;

void HCLockMultiThreadedAquiredAndRelinquish(void* context) {
    MultiThreadedAquiredAndRelinquishContext* multiThreadedAquiredAndRelinquishContext = context;
    
    for (HCInteger i = 0; i < HCLockMultiThreadedAquiredAndRelinquishIterationCount; i++) {
        // Make it so there is some stagger across the tests
        if (rand() % 4 == 0) {
            useconds_t seconds = rand() % 50 + 1;
            usleep(seconds);
        }
        HCLockAquire(multiThreadedAquiredAndRelinquishContext->lock);
        multiThreadedAquiredAndRelinquishContext->i++;
        HCLockRelinquish(multiThreadedAquiredAndRelinquishContext->lock);
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

CTEST(HCLock, AquiredExecute) {
    HCLockRef lock = HCLockCreate();
    HCInteger counter = 0;
    HCInteger iterationCount = 10000;
    for (HCInteger i = 0; i < iterationCount; i++) {
        HCLockAquiredExecute(lock, HCLockTestAquiredExecute, &counter);
    }
    
    ASSERT_TRUE(counter == iterationCount);
    
    HCRelease(lock);
}

CTEST(HCLock, MultiThreadedAquiredAndRelinquish) {
    MultiThreadedAquiredAndRelinquishContext context = {
        .lock = HCLockCreate(),
        .i = 0,
    };
    const HCInteger numberOfThreads = 10;
    HCListRef threads = HCListCreateWithCapacity(numberOfThreads);
    for (HCInteger i = 0; i < numberOfThreads; i++) {
        HCListAddObjectReleased(threads, HCThreadCreateWithOptions(HCLockMultiThreadedAquiredAndRelinquish, &context, HCThreadOptionJoinOnDestroy));
    }

    for (HCListIterator i = HCListIterationBegin(threads); !HCListIterationHasEnded(&i); HCListIterationNext(&i)) {
        HCThreadStart(i.object);
    }
    HCRelease(threads);
    
    ASSERT_TRUE(context.i == HCLockMultiThreadedAquiredAndRelinquishIterationCount * numberOfThreads);
    
    HCRelease(context.lock);
}
