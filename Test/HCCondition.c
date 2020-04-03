//
//  HCCondition.c
//  Test
//
//  Created by Braden Scothern on 4/1/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <unistd.h>

typedef struct {
    HCConditionRef condition;
    HCBoolean ran;
} HCConditionTestRanData;

void HCConditionTestRanDataFunction(void* context) {
    HCConditionTestRanData* data = context;
    
    HCConditionWaitAquired(data->condition);
    data->ran = true;
}

void HCConditionTestRanDataFunction2(void* context) {
    HCConditionTestRanData* data = context;
    
    HCConditionAquire(data->condition);
    HCConditionWait(data->condition);
    HCConditionRelinquish(data->condition);
    data->ran = true;
}

void* HCConditionTestAquiredExecute(void* context) {
    HCInteger* i = context;
    (*i)++;
    return NULL;
}

typedef struct MultiThreadedAquiredAndRelinquishContext {
    HCConditionRef lock;
    int i;
} MultiThreadedAquiredAndRelinquishContext;

const HCInteger HCConditionMultiThreadedAquiredAndRelinquishIterationCount = 100;

void HCConditionMultiThreadedAquiredAndRelinquish(void* context) {
    MultiThreadedAquiredAndRelinquishContext* multiThreadedAquiredAndRelinquishContext = context;
    
    for (HCInteger i = 0; i < HCConditionMultiThreadedAquiredAndRelinquishIterationCount; i++) {
        // Make it so there is some stagger across the tests
        if (rand() % 4 == 0) {
            useconds_t seconds = rand() % 50 + 1;
            usleep(seconds);
        }
        HCConditionAquire(multiThreadedAquiredAndRelinquishContext->lock);
        multiThreadedAquiredAndRelinquishContext->i++;
        HCConditionRelinquish(multiThreadedAquiredAndRelinquishContext->lock);
    }
}

CTEST(HCCondition, Create) {
    HCConditionRef lock = HCConditionCreate();
    HCRelease(lock);
}

CTEST(HCCondition, AquireAndRelinquish) {
    HCConditionRef lock = HCConditionCreate();
    HCConditionAquire(lock);
    HCConditionRelinquish(lock);
    HCRelease(lock);
}

CTEST(HCCondition, MultiAquireAndRelinquish) {
    HCConditionRef lock = HCConditionCreate();
    for (int i = 0; i < 10000; i++) {
        HCConditionAquire(lock);
        HCConditionRelinquish(lock);
    }
    HCRelease(lock);
}

CTEST(HCCondition, AquiredExecute) {
    HCConditionRef lock = HCConditionCreate();
    HCInteger counter = 0;
    HCInteger iterationCount = 10000;
    for (HCInteger i = 0; i < iterationCount; i++) {
        HCConditionExecuteAquired(lock, HCConditionTestAquiredExecute, &counter);
    }
    
    ASSERT_TRUE(counter == iterationCount);
    
    HCRelease(lock);
}

CTEST(HCCondition, MultiThreadedAquiredAndRelinquish) {
    MultiThreadedAquiredAndRelinquishContext context = {
        .lock = HCConditionCreate(),
        .i = 0,
    };
    const HCInteger numberOfThreads = 10;
    HCListRef threads = HCListCreateWithCapacity(numberOfThreads);
    for (HCInteger i = 0; i < numberOfThreads; i++) {
        HCListAddObjectReleased(threads, HCThreadCreateWithOptions(HCConditionMultiThreadedAquiredAndRelinquish, &context, HCThreadOptionJoinOnDestroy));
    }

    for (HCListIterator i = HCListIterationBegin(threads); !HCListIterationHasEnded(&i); HCListIterationNext(&i)) {
        HCThreadStart(i.object);
    }
    HCRelease(threads);
    
    ASSERT_TRUE(context.i == HCConditionMultiThreadedAquiredAndRelinquishIterationCount * numberOfThreads);
    
    HCRelease(context.lock);
}

CTEST(HCCondition, RaisingEventSignal) {
    HCConditionRef condition = HCConditionCreate();
    HCConditionTestRanData data[2] = {
        {
            .condition = condition,
            .ran = false
        },
        {
            .condition = condition,
            .ran = false
        }
    };
    
    HCThreadRef thread1 = HCThreadCreate(HCConditionTestRanDataFunction, &data[0]);
    HCThreadRef thread2 = HCThreadCreate(HCConditionTestRanDataFunction, &data[1]);
    HCThreadStart(thread1);
    HCThreadStart(thread2);
    
    usleep(1000);
    HCConditionAquire(condition);
    HCConditionRelinquishRaisingEvent(condition, HCConditionEventSignal);
    usleep(1000);
    ASSERT_TRUE(data[0].ran != data[1].ran);
    HCConditionRaiseEventAquired(condition, HCConditionEventSignal);
    usleep(1000);
    ASSERT_TRUE(data[0].ran && data[1].ran);

    HCThreadJoin(thread1);
    HCThreadJoin(thread2);
    HCRelease(thread1);
    HCRelease(thread2);
    HCRelease(condition);
}

CTEST(HCCondition, RaisingEventBroadcast) {
    HCConditionRef condition = HCConditionCreate();
    HCConditionTestRanData data[2] = {
        {
            .condition = condition,
            .ran = false
        },
        {
            .condition = condition,
            .ran = false
        }
    };
    
    HCThreadRef thread1 = HCThreadCreate(HCConditionTestRanDataFunction, &data[0]);
    HCThreadRef thread2 = HCThreadCreate(HCConditionTestRanDataFunction, &data[1]);
    HCThreadStart(thread1);
    HCThreadStart(thread2);
    
    usleep(1000);
    HCConditionRaiseEventAquired(condition, HCConditionEventBroadcast);
    HCThreadJoin(thread1);
    HCThreadJoin(thread2);
    ASSERT_TRUE(data[0].ran == data[1].ran);
    
    HCRelease(thread1);
    HCRelease(thread2);
    HCRelease(condition);
}

void* HCConditionExecuteRaisingEventAquiredTest(void* context) {
    HCBoolean* ran = context;
    *ran = true;
    return NULL;
}

CTEST(HCCondition, ExecuteRaisingEventAquired) {
    HCConditionRef condition = HCConditionCreate();
    HCConditionTestRanData data = {
        .condition = condition,
        .ran = false
    };
    HCBoolean ran = false;
    HCThreadRef thread = HCThreadCreate(HCConditionTestRanDataFunction, &data);
    HCThreadStart(thread);
    
    usleep(1000);
    HCConditionExecuteRaisingEventAquired(condition, HCConditionExecuteRaisingEventAquiredTest, &ran, HCConditionEventSignal);
    
    ASSERT_TRUE(ran);
    HCThreadJoin(thread);
    ASSERT_TRUE(data.ran);
    
    HCRelease(thread);
    HCRelease(condition);
}

CTEST(HCCondition, Signal) {
    HCConditionRef condition = HCConditionCreate();
    HCConditionTestRanData data = {
        .condition = condition,
        .ran = false
    };
    HCThreadRef thread = HCThreadCreate(HCConditionTestRanDataFunction, &data);
    HCThreadStart(thread);
    
    usleep(1000);
    HCConditionAquire(condition);
    HCConditionSignal(condition);
    HCConditionRelinquish(condition);
    
    HCThreadJoin(thread);
    ASSERT_TRUE(data.ran);
    HCRelease(thread);
    HCRelease(condition);
}

CTEST(HCCondition, Broadcast) {
    HCConditionRef condition = HCConditionCreate();
        HCConditionTestRanData data[2] = {
        {
            .condition = condition,
            .ran = false
        },
        {
            .condition = condition,
            .ran = false
        }
    };
    HCThreadRef thread1 = HCThreadCreate(HCConditionTestRanDataFunction, &data[0]);
    HCThreadRef thread2 = HCThreadCreate(HCConditionTestRanDataFunction, &data[1]);
    HCThreadStart(thread1);
    HCThreadStart(thread2);
    
    usleep(1000);
    HCConditionAquire(condition);
    HCConditionBroadcast(condition);
    HCConditionRelinquish(condition);
    
    HCThreadJoin(thread1);
    HCThreadJoin(thread2);
    ASSERT_TRUE(data[0].ran && data[1].ran);
    HCRelease(thread1);
    HCRelease(thread2);
    HCRelease(condition);
}

CTEST(HCCondition, RaiseEventSignal) {
    HCConditionRef condition = HCConditionCreate();
    HCConditionTestRanData data = {
        .condition = condition,
        .ran = false
    };
    HCThreadRef thread = HCThreadCreate(HCConditionTestRanDataFunction, &data);
    HCThreadStart(thread);
    
    usleep(1000);
    HCConditionAquire(condition);
    HCConditionRaiseEvent(condition, HCConditionEventSignal);
    HCConditionRelinquish(condition);
    
    HCThreadJoin(thread);
    ASSERT_TRUE(data.ran);
    HCRelease(thread);
    HCRelease(condition);
}

CTEST(HCCondition, RaiseEventBroadcast) {
    HCConditionRef condition = HCConditionCreate();
        HCConditionTestRanData data[2] = {
        {
            .condition = condition,
            .ran = false
        },
        {
            .condition = condition,
            .ran = false
        }
    };
    HCThreadRef thread1 = HCThreadCreate(HCConditionTestRanDataFunction, &data[0]);
    HCThreadRef thread2 = HCThreadCreate(HCConditionTestRanDataFunction, &data[1]);
    HCThreadStart(thread1);
    HCThreadStart(thread2);
    
    usleep(1000);
    HCConditionAquire(condition);
    HCConditionRaiseEvent(condition, HCConditionEventBroadcast);
    HCConditionRelinquish(condition);
    
    HCThreadJoin(thread1);
    HCThreadJoin(thread2);
    ASSERT_TRUE(data[0].ran && data[1].ran);
    HCRelease(thread1);
    HCRelease(thread2);
    HCRelease(condition);
}

CTEST(HCCondition, RaiseEventSignalAquired) {
    HCConditionRef condition = HCConditionCreate();
    HCConditionTestRanData data = {
        .condition = condition,
        .ran = false
    };
    HCThreadRef thread = HCThreadCreate(HCConditionTestRanDataFunction, &data);
    HCThreadStart(thread);
    
    usleep(1000);
    HCConditionRaiseEventAquired(condition, HCConditionEventSignal);
    
    HCThreadJoin(thread);
    ASSERT_TRUE(data.ran);
    HCRelease(thread);
    HCRelease(condition);
}

CTEST(HCCondition, RaiseEventBroadcastAquired) {
    HCConditionRef condition = HCConditionCreate();
        HCConditionTestRanData data[2] = {
        {
            .condition = condition,
            .ran = false
        },
        {
            .condition = condition,
            .ran = false
        }
    };
    HCThreadRef thread1 = HCThreadCreate(HCConditionTestRanDataFunction, &data[0]);
    HCThreadRef thread2 = HCThreadCreate(HCConditionTestRanDataFunction, &data[1]);
    HCThreadStart(thread1);
    HCThreadStart(thread2);
    
    usleep(1000);
    HCConditionRaiseEventAquired(condition, HCConditionEventBroadcast);
    
    HCThreadJoin(thread1);
    HCThreadJoin(thread2);
    ASSERT_TRUE(data[0].ran && data[1].ran);
    HCRelease(thread1);
    HCRelease(thread2);
    HCRelease(condition);
}

CTEST(HCCondition, Wait) {
    HCConditionRef condition = HCConditionCreate();
    HCConditionTestRanData data = {
        .condition = condition,
        .ran = false
    };
    HCThreadRef thread = HCThreadCreate(HCConditionTestRanDataFunction2, &data);
    HCThreadStart(thread);
    
    usleep(1000);
    HCConditionRaiseEventAquired(condition, HCConditionEventSignal);
    
    HCThreadJoin(thread);
    ASSERT_TRUE(data.ran);
    HCRelease(thread);
    HCRelease(condition);
}

typedef struct {
    HCConditionRef condition;
    HCBoolean ran;
    HCBoolean didTimeout;
} HCConditionTestWaitThenExecuteData;

void* HCConditionTestWaitThenExecuteDataFunction(void* context) {
    HCConditionTestWaitThenExecuteData* data = context;
    data->ran = true;
    return data;
}

void HCConditionTestWaitThenExecuteDataFunctionRunner(void* context) {
    HCConditionTestWaitThenExecuteData* data = context;
    ASSERT_TRUE(HCConditionWaitThenExecuteAquired(data->condition, HCConditionTestWaitThenExecuteDataFunction, context) == data);
}

CTEST(HCCondition, WaitThenExecute) {
    HCConditionRef condition = HCConditionCreate();
    HCConditionTestWaitThenExecuteData data = {
        .condition = condition,
        .ran = false,
        .didTimeout = false,
    };
    HCThreadRef thread = HCThreadCreate(HCConditionTestWaitThenExecuteDataFunctionRunner, &data);
    HCThreadStart(thread);
    
    usleep(1000);
    ASSERT_FALSE(data.ran);
    HCConditionRaiseEventAquired(condition, HCConditionEventSignal);

    HCThreadJoin(thread);
    ASSERT_TRUE(data.ran);
    HCRelease(thread);
    HCRelease(condition);
}

void HCConditionTestWaitTimeoutThenExecuteDataFunctionRunner(void* context) {
    HCConditionTestWaitThenExecuteData* data = context;
    void* result = NULL;
    HCConditionWaitTimeoutThenExecuteAquired(data->condition, 0.1, HCConditionTestWaitThenExecuteDataFunction, data, &result, &data->didTimeout);
    if (!data->didTimeout) {
        ASSERT_TRUE(result == data);
    } else {
        ASSERT_TRUE(result == NULL);
    }
}

CTEST(HCCondition, WaitTimeoutThenExecute1) {
    HCConditionRef condition = HCConditionCreate();
    HCConditionTestWaitThenExecuteData data = {
        .condition = condition,
        .ran = false,
        .didTimeout = false,
    };
    HCThreadRef thread = HCThreadCreate(HCConditionTestWaitTimeoutThenExecuteDataFunctionRunner, &data);
    HCThreadStart(thread);
    
    usleep(1000);
    ASSERT_FALSE(data.ran);
    ASSERT_FALSE(data.didTimeout);
    HCConditionRaiseEventAquired(condition, HCConditionEventSignal);

    HCThreadJoin(thread);
    ASSERT_TRUE(data.ran);
    ASSERT_FALSE(data.didTimeout);
    HCRelease(thread);
    HCRelease(condition);
}

CTEST(HCCondition, WaitTimeoutThenExecute2) {
    HCConditionRef condition = HCConditionCreate();
    HCConditionTestWaitThenExecuteData data = {
        .condition = condition,
        .ran = false,
        .didTimeout = false,
    };
    HCThreadRef thread = HCThreadCreate(HCConditionTestWaitTimeoutThenExecuteDataFunctionRunner, &data);
    HCThreadStart(thread);
    
    sleep(1);
    HCConditionRaiseEventAquired(condition, HCConditionEventSignal);

    HCThreadJoin(thread);
    ASSERT_FALSE(data.ran);
    ASSERT_TRUE(data.didTimeout);
    HCRelease(thread);
    HCRelease(condition);
}

typedef struct {
    HCConditionRef condition;
    HCBoolean ran;
    HCBoolean wait;
} HCConditionTestWaitWhileThenExecuteData;

HCBoolean HCConditionTestWaitWhileThenExecuteDataWaitWhileFunction(void* context) {
    HCConditionTestWaitWhileThenExecuteData* data = context;
    return data->wait;
}

void* HCConditionTestWaitWhileThenExecuteDataFunction(void* context) {
    HCConditionTestWaitWhileThenExecuteData* data = context;
    data->ran = true;
    return context;
}

void HCConditionWaitWhileThenExecuteDataFunctionRunner(void* context) {
    HCConditionTestWaitWhileThenExecuteData* data = context;

    HCConditionWaitWhileThenExecuteAquired(data->condition, HCConditionTestWaitWhileThenExecuteDataWaitWhileFunction, context, 5.0, HCConditionTestWaitWhileThenExecuteDataFunction, context);
}

CTEST(HCCondition, WaitWhileThenExecute) {
    HCConditionRef condition = HCConditionCreate();
    HCConditionTestWaitWhileThenExecuteData data = {
        .condition = condition,
        .ran = false,
        .wait = true,
    };
    HCThreadRef thread = HCThreadCreate(HCConditionWaitWhileThenExecuteDataFunctionRunner, &data);
    HCThreadStart(thread);
    
    usleep(1000);
    ASSERT_FALSE(data.ran);
    ASSERT_TRUE(data.wait);
    HCConditionRaiseEventAquired(condition, HCConditionEventSignal);
    usleep(1000);
    ASSERT_FALSE(data.ran);
    ASSERT_TRUE(data.wait);
    data.wait = false;
    HCConditionRaiseEventAquired(condition, HCConditionEventSignal);
    usleep(1000);
    ASSERT_TRUE(data.ran);
    ASSERT_FALSE(data.wait);
    
    HCThreadJoin(thread);
    HCRelease(thread);
    HCRelease(condition);
}
