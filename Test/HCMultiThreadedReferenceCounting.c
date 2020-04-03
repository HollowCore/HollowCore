//
//  HCMultiThreadedReferenceCounting.c
//  Test
//
//  Created by Braden Scothern on 3/26/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include "../Source/Core/HCObject_Internal.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    HCNumberRef number;
    HCInteger retainCount;
} HCMultiThreadedReferenceCountingContext;

void HCMultiThreadedReferenceCountingThreadFunction(void* context);

CTEST(HCMultiThreadedReferenceCounting, SafeRandomRetainOrReleaseFavoringRetain) {
    static const int numberOfThreads = 8;
    static const int numberOfIterations = 1;
    
    for (HCInteger iteration = 0; iteration < numberOfIterations; iteration++) {
        HCListRef threads = HCListCreate();
        HCNumberRef number = HCNumberCreateWithInteger(42);
        HCMultiThreadedReferenceCountingContext contexts[numberOfThreads];

        for (HCInteger i = 0; i < numberOfThreads; i++) {
            contexts[i] = (HCMultiThreadedReferenceCountingContext){
                .number = number,
                .retainCount = 0,
            };
            HCThreadRef thread = HCThreadCreate(HCMultiThreadedReferenceCountingThreadFunction, &contexts[i]);
            HCListAddObjectReleased(threads, thread);
            HCThreadStart(thread);
        }

        // Give the threads time to hammer on stuff
        sleep(5);

        for (HCInteger i = 0; i < numberOfThreads; i++) {
            HCThreadCancel(HCListObjectAtIndex(threads, i));
        }

        for (HCInteger i = 0; i < numberOfThreads; i++) {
            HCThreadJoin(HCListObjectAtIndex(threads, i));
        }

        HCInteger retainCount = atomic_load_explicit(&((HCObjectRef)number)->referenceCount, memory_order_relaxed);
        for (HCInteger i = 0; i < numberOfThreads; i++) {
            retainCount -= contexts[i].retainCount;
        }

        ASSERT_EQUAL(1, retainCount);
        retainCount = atomic_load_explicit(&((HCObjectRef)number)->referenceCount, memory_order_relaxed);
        for (HCInteger i = 0; i < retainCount; i++) {
            HCRelease(number);
        }
        HCRelease(threads);
    }
}

void HCMultiThreadedReferenceCountingThreadFunction(void* _context) {
    HCMultiThreadedReferenceCountingContext* context = _context;
    HCThreadRef thread = HCThreadGetCurrent();
    
    while (!HCThreadIsCanceled(thread)) {
        time_t finishIteration = time(NULL) + 1;
        while (time(NULL) < finishIteration) {
            if (rand() % 5 >= 3 && context->retainCount > 0) {
                context->retainCount--;
                HCRelease(context->number);
            } else {
                context->retainCount++;
                HCRetain(context->number);
            }
        }
    }
}
