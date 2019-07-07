//
//  HCSet.c
//  Test
//
//  Created by Matt Stoker on 1/23/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <math.h>

CTEST(HCSet, Creation) {
    HCSetRef empty = HCSetCreate();
    ASSERT_TRUE(HCSetIsEmpty(empty));
    HCRelease(empty);
}

CTEST(HCSet, Invalid) {
    ASSERT_TRUE(HCSetIterationHasBegun((HCSetIterator*)&HCSetIteratorInvalid));
    ASSERT_FALSE(HCSetIterationHasNext((HCSetIterator*)&HCSetIteratorInvalid));
    ASSERT_TRUE(HCSetIterationHasEnded((HCSetIterator*)&HCSetIteratorInvalid));
}

CTEST(HCSet, Access) {
    HCNumberRef number = HCNumberCreateWithInteger(42);
    HCSetRef set = HCSetCreate();
    ASSERT_TRUE(HCSetIsEmpty(set));
    ASSERT_FALSE(HCSetContainsObject(set, number));
    HCSetAddObject(set, number);
    ASSERT_FALSE(HCSetIsEmpty(set));
    ASSERT_TRUE(HCSetContainsObject(set, number));
    ASSERT_TRUE(HCIsEqual(HCSetFirstObject(set), number));
    ASSERT_TRUE(HCIsEqual(HCSetLastObject(set), number));
    HCRelease(set);
    HCRelease(number);
}

CTEST(HCSet, AddRemove) {
    HCSetRef set = HCSetCreate();
    HCNumberRef zero = HCNumberCreateWithInteger(0);
    HCNumberRef one = HCNumberCreateWithInteger(1);
    HCNumberRef two = HCNumberCreateWithInteger(2);
    HCNumberRef three = HCNumberCreateWithInteger(3);
    HCSetAddObject(set, zero);
    ASSERT_FALSE(HCSetIsEmpty(set));
    ASSERT_EQUAL(HCSetCount(set), 1);
    ASSERT_TRUE(HCSetContainsObject(set, zero));
    ASSERT_FALSE(HCSetContainsObject(set, one));
    ASSERT_FALSE(HCSetContainsObject(set, two));
    ASSERT_FALSE(HCSetContainsObject(set, three));
    ASSERT_NOT_NULL(HCSetFirstObject(set));
    ASSERT_NOT_NULL(HCSetLastObject(set));
    HCSetAddObject(set, one);
    HCSetAddObject(set, two);
    HCSetAddObject(set, three);
    ASSERT_FALSE(HCSetIsEmpty(set));
    ASSERT_EQUAL(HCSetCount(set), 4);
    ASSERT_TRUE(HCSetContainsObject(set, zero));
    ASSERT_TRUE(HCSetContainsObject(set, one));
    ASSERT_TRUE(HCSetContainsObject(set, two));
    ASSERT_TRUE(HCSetContainsObject(set, three));
    ASSERT_NOT_NULL(HCSetFirstObject(set));
    ASSERT_NOT_NULL(HCSetLastObject(set));
    HCSetRemoveObject(set, two);
    ASSERT_FALSE(HCSetIsEmpty(set));
    ASSERT_EQUAL(HCSetCount(set), 3);
    ASSERT_TRUE(HCSetContainsObject(set, zero));
    ASSERT_TRUE(HCSetContainsObject(set, one));
    ASSERT_FALSE(HCSetContainsObject(set, two));
    ASSERT_TRUE(HCSetContainsObject(set, three));
    ASSERT_NOT_NULL(HCSetFirstObject(set));
    ASSERT_NOT_NULL(HCSetLastObject(set));
    HCSetRemoveObject(set, one);
    ASSERT_FALSE(HCSetIsEmpty(set));
    ASSERT_EQUAL(HCSetCount(set), 2);
    ASSERT_TRUE(HCSetContainsObject(set, zero));
    ASSERT_FALSE(HCSetContainsObject(set, one));
    ASSERT_FALSE(HCSetContainsObject(set, two));
    ASSERT_TRUE(HCSetContainsObject(set, three));
    ASSERT_NOT_NULL(HCSetFirstObject(set));
    ASSERT_NOT_NULL(HCSetLastObject(set));
    HCSetRemoveObject(set, zero);
    ASSERT_FALSE(HCSetIsEmpty(set));
    ASSERT_EQUAL(HCSetCount(set), 1);
    ASSERT_FALSE(HCSetContainsObject(set, zero));
    ASSERT_FALSE(HCSetContainsObject(set, one));
    ASSERT_FALSE(HCSetContainsObject(set, two));
    ASSERT_TRUE(HCSetContainsObject(set, three));
    ASSERT_NOT_NULL(HCSetFirstObject(set));
    ASSERT_NOT_NULL(HCSetLastObject(set));
    HCSetRemoveObject(set, three);
    ASSERT_TRUE(HCSetIsEmpty(set));
    ASSERT_EQUAL(HCSetCount(set), 0);
    ASSERT_FALSE(HCSetContainsObject(set, zero));
    ASSERT_FALSE(HCSetContainsObject(set, one));
    ASSERT_FALSE(HCSetContainsObject(set, two));
    ASSERT_FALSE(HCSetContainsObject(set, three));
    ASSERT_NULL(HCSetFirstObject(set));
    ASSERT_NULL(HCSetLastObject(set));
    HCRelease(set);
    HCRelease(zero);
    HCRelease(one);
    HCRelease(two);
    HCRelease(three);
}

CTEST(HCSet, AddRemoveMany) {
    HCInteger count = 100;
    HCSetRef set = HCSetCreate();
    for (HCInteger index = 0; index < count; index++) {
        HCSetAddObjectReleased(set, HCNumberCreateWithInteger(index));
        ASSERT_EQUAL(HCSetCount(set), index + 1);
        for (HCInteger checkIndex = 0; checkIndex <= index; checkIndex++) {
            HCNumberRef number = HCNumberCreateWithInteger(checkIndex);
            ASSERT_TRUE(HCSetContainsObject(set, number));
            HCRelease(number);
        }
    }
    for (HCInteger index = 0; index < count; index++) {
        HCNumberRef number = HCNumberCreateWithInteger(index);
        HCSetRemoveObject(set, number);
        ASSERT_FALSE(HCSetContainsObject(set, number));
        HCRelease(number);
    }
    HCRelease(set);
}

CTEST(HCSet, EqualHash) {
    HCSetRef a = HCSetCreate();
    HCSetAddObjectReleased(a, HCNumberCreateWithInteger(0));
    HCSetAddObjectReleased(a, HCNumberCreateWithInteger(1));
    HCSetAddObjectReleased(a, HCNumberCreateWithInteger(2));
    HCSetRef b = HCSetCreate();
    HCSetAddObjectReleased(b, HCNumberCreateWithInteger(0));
    HCSetAddObjectReleased(b, HCNumberCreateWithInteger(1));
    HCSetAddObjectReleased(b, HCNumberCreateWithInteger(2));
    HCSetRef c = HCSetCreate();
    HCSetAddObjectReleased(c, HCNumberCreateWithInteger(2));
    HCSetAddObjectReleased(c, HCNumberCreateWithInteger(1));
    ASSERT_TRUE(HCSetIsEqual(a, a));
    ASSERT_TRUE(HCSetIsEqual(b, b));
    ASSERT_TRUE(HCSetIsEqual(c, c));
    ASSERT_TRUE(HCSetIsEqual(a, b));
    ASSERT_TRUE(HCSetIsEqual(b, a));
    ASSERT_FALSE(HCSetIsEqual(a, c));
    ASSERT_FALSE(HCSetIsEqual(c, a));
    ASSERT_EQUAL(HCSetHashValue(a), HCSetHashValue(a));
    ASSERT_EQUAL(HCSetHashValue(a), HCSetHashValue(b));
    HCRelease(a);
    HCRelease(b);
    HCRelease(c);
}

CTEST(HCSet, Print) {
    HCSetRef a = HCSetCreate();
    HCSetPrint(a, stdout); // TODO: Not to stdout
    HCPrint(a, stdout); // TODO: Not to stdout
    HCRelease(a);
}

CTEST(HCSet, Clear) {
    HCSetRef set = HCSetCreate();
    ASSERT_TRUE(HCSetIsEmpty(set));
    HCSetAddObjectReleased(set, HCNumberCreateWithInteger(3));
    HCSetAddObjectReleased(set, HCNumberCreateWithInteger(4));
    HCSetAddObjectReleased(set, HCNumberCreateWithInteger(5));
    ASSERT_FALSE(HCSetIsEmpty(set));
    HCSetClear(set);
    ASSERT_TRUE(HCSetIsEmpty(set));
    HCRelease(set);
}

CTEST(HCSet, Search) {
    HCNumberRef zero = HCNumberCreateWithInteger(0);
    HCNumberRef one = HCNumberCreateWithInteger(1);
    HCNumberRef two = HCNumberCreateWithInteger(2);
    HCNumberRef three = HCNumberCreateWithInteger(3);
    HCNumberRef four = HCNumberCreateWithInteger(4);
    HCSetRef set = HCSetCreate();
    ASSERT_FALSE(HCSetContainsObject(set, zero));
    ASSERT_FALSE(HCSetContainsObject(set, one));
    ASSERT_FALSE(HCSetContainsObject(set, two));
    ASSERT_FALSE(HCSetContainsObject(set, three));
    ASSERT_FALSE(HCSetContainsObject(set, four));
    HCSetAddObject(set, zero);
    HCSetAddObject(set, one);
    HCSetAddObject(set, two);
    HCSetAddObject(set, three);
    HCSetAddObject(set, four);
    ASSERT_EQUAL(HCSetCount(set), 5);
    ASSERT_TRUE(HCSetContainsObject(set, zero));
    ASSERT_TRUE(HCSetContainsObject(set, one));
    ASSERT_TRUE(HCSetContainsObject(set, two));
    ASSERT_TRUE(HCSetContainsObject(set, three));
    ASSERT_TRUE(HCSetContainsObject(set, four));
    HCSetClear(set);
    
    HCSetAddObject(set, zero);
    HCSetAddObject(set, one);
    HCSetAddObject(set, two);
    HCSetAddObject(set, three);
    HCSetAddObject(set, four);
    HCSetAddObject(set, four);
    HCSetAddObject(set, three);
    HCSetAddObject(set, two);
    HCSetAddObject(set, one);
    HCSetAddObject(set, zero);
    HCSetAddObject(set, zero);
    HCSetAddObject(set, one);
    HCSetAddObject(set, two);
    HCSetAddObject(set, three);
    HCSetAddObject(set, four);
    HCSetAddObject(set, four);
    HCSetAddObject(set, three);
    HCSetAddObject(set, two);
    HCSetAddObject(set, one);
    HCSetAddObject(set, zero);
    ASSERT_EQUAL(HCSetCount(set), 5);
    ASSERT_TRUE(HCSetContainsObject(set, zero));
    ASSERT_TRUE(HCSetContainsObject(set, one));
    ASSERT_TRUE(HCSetContainsObject(set, two));
    ASSERT_TRUE(HCSetContainsObject(set, three));
    ASSERT_TRUE(HCSetContainsObject(set, four));
    
    HCRelease(set);
    HCRelease(zero);
    HCRelease(one);
    HCRelease(two);
    HCRelease(three);
    HCRelease(four);
}

CTEST(HCSet, MemoryConvenience) {
    HCNumberRef zero = HCNumberCreateWithInteger(0);
    HCNumberRef one = HCNumberCreateWithInteger(1);
    HCNumberRef two = HCNumberCreateWithInteger(2);
    HCNumberRef three = HCNumberCreateWithInteger(3);
    HCNumberRef four = HCNumberCreateWithInteger(4);
    HCSetRef set = HCSetCreate();
    HCSetAddObjectReleased(set, HCRetain(zero));
    HCSetAddObjectReleased(set, HCRetain(one));
    HCSetAddObjectReleased(set, HCRetain(two));
    HCSetAddObjectReleased(set, HCRetain(three));
    HCSetAddObjectReleased(set, HCRetain(four));
    
    HCNumberRef a = HCSetRemoveObjectRetained(set, two);
    ASSERT_TRUE(HCIsEqual(a, two));
    HCRelease(a);
    
    HCNumberRef b = HCSetRemoveObjectRetained(set, four);
    ASSERT_TRUE(HCIsEqual(b, four));
    HCRelease(b);
    
    HCRelease(set);
    HCRelease(zero);
    HCRelease(one);
    HCRelease(two);
    HCRelease(three);
    HCRelease(four);
}

CTEST(HCSet, Iteration) {
    HCNumberRef zero = HCNumberCreateWithInteger(0);
    HCNumberRef one = HCNumberCreateWithInteger(1);
    HCNumberRef two = HCNumberCreateWithInteger(2);
    HCNumberRef three = HCNumberCreateWithInteger(3);
    HCNumberRef four = HCNumberCreateWithInteger(4);
    HCSetRef set = HCSetCreate();
    HCSetAddObject(set, zero);
    HCSetAddObject(set, one);
    HCSetAddObject(set, two);
    HCSetAddObject(set, three);
    HCSetAddObject(set, four);
    
    HCSetIterator i = HCSetIteratorInvalid;
    ASSERT_TRUE(HCSetIterationHasBegun(&i));
    ASSERT_FALSE(HCSetIterationHasNext(&i));
    ASSERT_TRUE(HCSetIterationHasEnded(&i));
    i = HCSetIterationBegin(set);
    ASSERT_TRUE(HCSetIterationHasBegun(&i));
    ASSERT_FALSE(HCSetIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.set, set));
    ASSERT_EQUAL(i.index, 0);
    ASSERT_TRUE(HCIsEqual(i.object, zero));
    HCSetIterationNext(&i);
    ASSERT_TRUE(HCSetIterationHasBegun(&i));
    ASSERT_FALSE(HCSetIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.set, set));
    ASSERT_EQUAL(i.index, 1);
    ASSERT_TRUE(HCIsEqual(i.object, one));
    HCSetIterationNext(&i);
    ASSERT_TRUE(HCSetIterationHasBegun(&i));
    ASSERT_FALSE(HCSetIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.set, set));
    ASSERT_EQUAL(i.index, 2);
    ASSERT_TRUE(HCIsEqual(i.object, two));
    HCSetIterationNext(&i);
    ASSERT_TRUE(HCSetIterationHasBegun(&i));
    ASSERT_FALSE(HCSetIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.set, set));
    ASSERT_EQUAL(i.index, 3);
    ASSERT_TRUE(HCIsEqual(i.object, three));
    HCSetIterationNext(&i);
    ASSERT_TRUE(HCSetIterationHasBegun(&i));
    ASSERT_FALSE(HCSetIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.set, set));
    ASSERT_EQUAL(i.index, 4);
    ASSERT_TRUE(HCIsEqual(i.object, four));
    HCSetIterationNext(&i);
    ASSERT_TRUE(HCSetIterationHasBegun(&i));
    ASSERT_TRUE(HCSetIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.set, set));
    ASSERT_NULL(i.object);
    
    HCRelease(set);
    HCRelease(zero);
    HCRelease(one);
    HCRelease(two);
    HCRelease(three);
    HCRelease(four);
}

CTEST(HCSet, IterationLoop) {
    HCSetRef set = HCSetCreate();
    HCSetAddObjectReleased(set, HCNumberCreateWithInteger(0));
    HCSetAddObjectReleased(set, HCNumberCreateWithInteger(1));
    HCSetAddObjectReleased(set, HCNumberCreateWithInteger(2));
    HCSetAddObjectReleased(set, HCNumberCreateWithInteger(3));
    HCSetAddObjectReleased(set, HCNumberCreateWithInteger(4));
    
    HCInteger count = 0;
    for (HCSetIterator i = HCSetIterationBegin(set); !HCSetIterationHasEnded(&i); HCSetIterationNext(&i)) {
        ASSERT_NOT_NULL(i.object);
        count++;
    }
    ASSERT_TRUE(count == HCSetCount(set));

    HCRelease(set);
}
