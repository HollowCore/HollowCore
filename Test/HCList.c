//
//  HCList.c
//  Test
//
//  Created by Matt Stoker on 1/21/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <math.h>

CTEST(HCList, Creation) {
    HCListRef empty = HCListCreate();
    ASSERT_TRUE(HCListIsEmpty(empty));
    HCRelease(empty);
}

CTEST(HCList, Invalid) {
    ASSERT_TRUE(HCListIterationHasBegun((HCListIterator*)&HCListIteratorInvalid));
    ASSERT_FALSE(HCListIterationHasPrevious((HCListIterator*)&HCListIteratorInvalid));
    ASSERT_FALSE(HCListIterationHasNext((HCListIterator*)&HCListIteratorInvalid));
    ASSERT_TRUE(HCListIterationHasEnded((HCListIterator*)&HCListIteratorInvalid));
}

CTEST(HCList, Access) {
    HCNumberRef number = HCNumberCreateWithInteger(42);
    HCListRef list = HCListCreate();
    ASSERT_TRUE(HCListIsEmpty(list));
    ASSERT_FALSE(HCListContainsObject(list, number));
    HCListAddObject(list, number);
    ASSERT_FALSE(HCListIsEmpty(list));
    ASSERT_TRUE(HCIsEqual(HCListFirstObject(list), number));
    ASSERT_TRUE(HCIsEqual(HCListLastObject(list), number));
    HCRelease(list);
    HCRelease(number);
}

CTEST(HCList, AddRemove) {
    HCListRef list = HCListCreate();
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(0));
    ASSERT_FALSE(HCListIsEmpty(list));
    ASSERT_EQUAL(HCListCount(list), 1);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(list, 0)), 0);
    ASSERT_EQUAL(HCNumberAsInteger(HCListFirstObject(list)), 0);
    ASSERT_EQUAL(HCNumberAsInteger(HCListLastObject(list)), 0);
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(1));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(2));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(3));
    ASSERT_FALSE(HCListIsEmpty(list));
    ASSERT_EQUAL(HCListCount(list), 4);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(list, 0)), 0);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(list, 1)), 1);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(list, 2)), 2);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(list, 3)), 3);
    ASSERT_EQUAL(HCNumberAsInteger(HCListFirstObject(list)), 0);
    ASSERT_EQUAL(HCNumberAsInteger(HCListLastObject(list)), 3);
    HCListRemoveObject(list);
    ASSERT_FALSE(HCListIsEmpty(list));
    ASSERT_EQUAL(HCListCount(list), 3);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(list, 0)), 0);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(list, 1)), 1);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(list, 2)), 2);
    ASSERT_EQUAL(HCNumberAsInteger(HCListFirstObject(list)), 0);
    ASSERT_EQUAL(HCNumberAsInteger(HCListLastObject(list)), 2);
    HCListRemoveObject(list);
    HCListRemoveObject(list);
    HCListRemoveObject(list);
    ASSERT_TRUE(HCListIsEmpty(list));
    ASSERT_NULL(HCListFirstObject(list));
    ASSERT_NULL(HCListLastObject(list));
    HCRelease(list);
}

CTEST(HCList, AddRemoveMany) {
    HCInteger count = 100;
    HCListRef list = HCListCreate();
    for (HCInteger index = 0; index < count; index++) {
        HCListAddObjectReleased(list, HCNumberCreateWithInteger(index));
        ASSERT_EQUAL(HCListCount(list), index + 1);
        for (HCInteger checkIndex = 0; checkIndex <= index; checkIndex++) {
            ASSERT_TRUE(HCListContainsIndex(list, checkIndex));
            ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(list, checkIndex)), checkIndex);
        }
    }
    for (HCInteger index = 0; index < count; index++) {
        HCListRemoveObjectAtIndex(list, 0);
        ASSERT_EQUAL(HCListCount(list), count - index - 1);
        ASSERT_FALSE(HCListContainsIndex(list, count - index));
        for (HCInteger checkIndex = 0; checkIndex < count - index - 1; checkIndex++) {
            ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(list, checkIndex)), checkIndex + index + 1);
        }
    }
    HCRelease(list);
}

CTEST(HCList, EqualHash) {
    HCListRef a = HCListCreate();
    HCListAddObjectReleased(a, HCNumberCreateWithInteger(0));
    HCListAddObjectReleased(a, HCNumberCreateWithInteger(1));
    HCListAddObjectReleased(a, HCNumberCreateWithInteger(2));
    HCListRef b = HCListCreate();
    HCListAddObjectReleased(b, HCNumberCreateWithInteger(0));
    HCListAddObjectReleased(b, HCNumberCreateWithInteger(1));
    HCListAddObjectReleased(b, HCNumberCreateWithInteger(2));
    HCListRef c = HCListCreate();
    HCListAddObjectReleased(c, HCNumberCreateWithInteger(2));
    HCListAddObjectReleased(c, HCNumberCreateWithInteger(1));
    HCListAddObjectReleased(c, HCNumberCreateWithInteger(0));
    ASSERT_TRUE(HCListIsEqual(a, a));
    ASSERT_TRUE(HCListIsEqual(b, b));
    ASSERT_TRUE(HCListIsEqual(c, c));
    ASSERT_TRUE(HCListIsEqual(a, b));
    ASSERT_TRUE(HCListIsEqual(b, a));
    ASSERT_FALSE(HCListIsEqual(a, c));
    ASSERT_FALSE(HCListIsEqual(c, a));
    ASSERT_EQUAL(HCListHashValue(a), HCListHashValue(a));
    ASSERT_EQUAL(HCListHashValue(a), HCListHashValue(b));
    HCRelease(a);
    HCRelease(b);
    HCRelease(c);
}

CTEST(HCList, Print) {
    HCListRef a = HCListCreate();
    HCListPrint(a, stdout); // TODO: Not to stdout
    HCPrint(a, stdout); // TODO: Not to stdout
    HCRelease(a);
}

CTEST(HCList, Clear) {
    HCListRef list = HCListCreate();
    ASSERT_TRUE(HCListIsEmpty(list));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(3));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(4));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(5));
    ASSERT_FALSE(HCListIsEmpty(list));
    HCListClear(list);
    ASSERT_TRUE(HCListIsEmpty(list));
    HCRelease(list);
}

CTEST(HCList, Search) {
    HCNumberRef zero = HCNumberCreateWithInteger(0);
    HCNumberRef one = HCNumberCreateWithInteger(1);
    HCNumberRef two = HCNumberCreateWithInteger(2);
    HCNumberRef three = HCNumberCreateWithInteger(3);
    HCNumberRef four = HCNumberCreateWithInteger(4);
    HCListRef list = HCListCreate();
    ASSERT_FALSE(HCListContainsObject(list, zero));
    HCListAddObject(list, zero);
    HCListAddObject(list, one);
    HCListAddObject(list, two);
    HCListAddObject(list, three);
    HCListAddObject(list, four);
    ASSERT_TRUE(HCListContainsObject(list, zero));
    ASSERT_EQUAL(HCListCount(list), 5);
    ASSERT_EQUAL(HCListFirstIndexOfObject(list, zero), 0);
    ASSERT_EQUAL(HCListLastIndexOfObject(list, zero), 0);
    ASSERT_EQUAL(HCListFirstIndexOfObject(list, one), 1);
    ASSERT_EQUAL(HCListLastIndexOfObject(list, one), 1);
    ASSERT_EQUAL(HCListFirstIndexOfObject(list, two), 2);
    ASSERT_EQUAL(HCListLastIndexOfObject(list, two), 2);
    ASSERT_EQUAL(HCListFirstIndexOfObject(list, three), 3);
    ASSERT_EQUAL(HCListLastIndexOfObject(list, three), 3);
    ASSERT_EQUAL(HCListFirstIndexOfObject(list, four), 4);
    ASSERT_EQUAL(HCListLastIndexOfObject(list, four), 4);
    HCListClear(list);
    
    HCListAddObject(list, zero);
    HCListAddObject(list, one);
    HCListAddObject(list, two);
    HCListAddObject(list, three);
    HCListAddObject(list, four);
    HCListAddObject(list, four);
    HCListAddObject(list, three);
    HCListAddObject(list, two);
    HCListAddObject(list, one);
    HCListAddObject(list, zero);
    HCListAddObject(list, zero);
    HCListAddObject(list, one);
    HCListAddObject(list, two);
    HCListAddObject(list, three);
    HCListAddObject(list, four);
    HCListAddObject(list, four);
    HCListAddObject(list, three);
    HCListAddObject(list, two);
    HCListAddObject(list, one);
    HCListAddObject(list, zero);
    ASSERT_EQUAL(HCListCount(list), 20);
    ASSERT_EQUAL(HCListFirstIndexOfObject(list, zero), 0);
    ASSERT_EQUAL(HCListLastIndexOfObject(list, zero), 19);
    ASSERT_EQUAL(HCListIndexOfObject(list, 5, true, zero), 0);
    ASSERT_EQUAL(HCListIndexOfObject(list, 5, false, zero), 9);
    ASSERT_EQUAL(HCListIndexOfObject(list, 15, true, zero), 10);
    ASSERT_EQUAL(HCListIndexOfObject(list, 15, false, zero), 19);
    ASSERT_EQUAL(HCListFirstIndexOfObject(list, one), 1);
    ASSERT_EQUAL(HCListLastIndexOfObject(list, one), 18);
    ASSERT_EQUAL(HCListIndexOfObject(list, 5, true, one), 1);
    ASSERT_EQUAL(HCListIndexOfObject(list, 5, false, one), 8);
    ASSERT_EQUAL(HCListIndexOfObject(list, 15, true, one), 11);
    ASSERT_EQUAL(HCListIndexOfObject(list, 15, false, one), 18);
    ASSERT_EQUAL(HCListFirstIndexOfObject(list, two), 2);
    ASSERT_EQUAL(HCListLastIndexOfObject(list, two), 17);
    ASSERT_EQUAL(HCListIndexOfObject(list, 5, true, two), 2);
    ASSERT_EQUAL(HCListIndexOfObject(list, 5, false, two), 7);
    ASSERT_EQUAL(HCListIndexOfObject(list, 15, true, two), 12);
    ASSERT_EQUAL(HCListIndexOfObject(list, 15, false, two), 17);
    ASSERT_EQUAL(HCListFirstIndexOfObject(list, three), 3);
    ASSERT_EQUAL(HCListLastIndexOfObject(list, three), 16);
    ASSERT_EQUAL(HCListIndexOfObject(list, 5, true, three), 3);
    ASSERT_EQUAL(HCListIndexOfObject(list, 5, false, three), 6);
    ASSERT_EQUAL(HCListIndexOfObject(list, 15, true, three), 13);
    ASSERT_EQUAL(HCListIndexOfObject(list, 15, false, three), 16);
    ASSERT_EQUAL(HCListFirstIndexOfObject(list, four), 4);
    ASSERT_EQUAL(HCListLastIndexOfObject(list, four), 15);
    ASSERT_EQUAL(HCListIndexOfObject(list, 5, true, four), 4);
    ASSERT_EQUAL(HCListIndexOfObject(list, 5, false, four), 5);
    ASSERT_EQUAL(HCListIndexOfObject(list, 15, true, four), 14);
    ASSERT_EQUAL(HCListIndexOfObject(list, 15, false, four), 15);
    
    HCRelease(list);
    HCRelease(zero);
    HCRelease(one);
    HCRelease(two);
    HCRelease(three);
    HCRelease(four);
}

CTEST(HCList, CompoundRemove) {
    HCNumberRef zero = HCNumberCreateWithInteger(0);
    HCNumberRef one = HCNumberCreateWithInteger(1);
    HCNumberRef two = HCNumberCreateWithInteger(2);
    HCNumberRef three = HCNumberCreateWithInteger(3);
    HCNumberRef four = HCNumberCreateWithInteger(4);
    HCListRef list = HCListCreate();
    HCListAddObject(list, zero);
    HCListAddObject(list, one);
    HCListAddObject(list, two);
    HCListAddObject(list, two);
    HCListAddObject(list, two);
    HCListAddObject(list, two);
    HCListAddObject(list, three);
    HCListAddObject(list, four);
    ASSERT_TRUE(HCListContainsObject(list, two));
    ASSERT_EQUAL(HCListFirstIndexOfObject(list, two), 2);
    ASSERT_EQUAL(HCListLastIndexOfObject(list, two), 5);
    HCListRemoveFirstObjectEqualToObject(list, two);
    ASSERT_TRUE(HCListContainsObject(list, two));
    ASSERT_EQUAL(HCListFirstIndexOfObject(list, two), 2);
    ASSERT_EQUAL(HCListLastIndexOfObject(list, two), 4);
    HCListRemoveLastObjectEqualToObject(list, two);
    ASSERT_TRUE(HCListContainsObject(list, two));
    ASSERT_EQUAL(HCListFirstIndexOfObject(list, two), 2);
    ASSERT_EQUAL(HCListLastIndexOfObject(list, two), 3);
    HCListRemoveAllObjectsEqualToObject(list, two);
    ASSERT_FALSE(HCListContainsObject(list, two));
    HCRelease(list);
    HCRelease(zero);
    HCRelease(one);
    HCRelease(two);
    HCRelease(three);
    HCRelease(four);
}

CTEST(HCList, MemoryConvenience) {
    HCNumberRef zero = HCNumberCreateWithInteger(0);
    HCNumberRef one = HCNumberCreateWithInteger(1);
    HCNumberRef two = HCNumberCreateWithInteger(2);
    HCNumberRef three = HCNumberCreateWithInteger(3);
    HCNumberRef four = HCNumberCreateWithInteger(4);
    HCListRef list = HCListCreate();
    HCListAddObjectReleased(list, HCRetain(zero));
    HCListAddObjectReleased(list, HCRetain(one));
    HCListAddObjectReleased(list, HCRetain(three));
    HCListAddObjectReleased(list, HCRetain(four));
    HCListAddObjectReleasedAtIndex(list, 2, HCRetain(two));
    ASSERT_TRUE(HCIsEqual(HCListObjectAtIndex(list, 0), zero));
    ASSERT_TRUE(HCIsEqual(HCListObjectAtIndex(list, 1), one));
    ASSERT_TRUE(HCIsEqual(HCListObjectAtIndex(list, 2), two));
    ASSERT_TRUE(HCIsEqual(HCListObjectAtIndex(list, 3), three));
    ASSERT_TRUE(HCIsEqual(HCListObjectAtIndex(list, 4), four));
    
    HCNumberRef a = HCListRemoveObjectRetainedAtIndex(list, 2);
    ASSERT_TRUE(HCIsEqual(a, two));
    HCRelease(a);
    
    HCNumberRef b = HCListRemoveObjectRetained(list);
    ASSERT_TRUE(HCIsEqual(b, four));
    HCRelease(b);
    
    HCNumberRef c = HCListRemoveFirstObjectRetainedEqualToObject(list, one);
    ASSERT_TRUE(HCIsEqual(c, one));
    HCRelease(c);
    
    HCNumberRef d = HCListRemoveLastObjectRetainedEqualToObject(list, three);
    ASSERT_TRUE(HCIsEqual(d, three));
    HCRelease(d);
    
    HCNumberRef e = HCListRemoveObjectRetainedEqualToObject(list, HCListCount(list), true, zero);
    ASSERT_TRUE(HCIsEqual(e, zero));
    HCRelease(e);
    
    HCRelease(list);
    HCRelease(zero);
    HCRelease(one);
    HCRelease(two);
    HCRelease(three);
    HCRelease(four);
}

CTEST(HCList, Iteration) {
    HCNumberRef zero = HCNumberCreateWithInteger(0);
    HCNumberRef one = HCNumberCreateWithInteger(1);
    HCNumberRef two = HCNumberCreateWithInteger(2);
    HCNumberRef three = HCNumberCreateWithInteger(3);
    HCNumberRef four = HCNumberCreateWithInteger(4);
    HCListRef list = HCListCreate();
    HCListAddObject(list, zero);
    HCListAddObject(list, one);
    HCListAddObject(list, two);
    HCListAddObject(list, three);
    HCListAddObject(list, four);
    
    HCListIterator i = HCListIteratorInvalid;
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_FALSE(HCListIterationHasPrevious(&i));
    ASSERT_FALSE(HCListIterationHasNext(&i));
    ASSERT_TRUE(HCListIterationHasEnded(&i));
    i = HCListIterationBegin(list);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_FALSE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 0);
    ASSERT_TRUE(HCIsEqual(i.object, zero));
    HCListIterationNext(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 1);
    ASSERT_TRUE(HCIsEqual(i.object, one));
    HCListIterationNext(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 2);
    ASSERT_TRUE(HCIsEqual(i.object, two));
    HCListIterationNext(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 3);
    ASSERT_TRUE(HCIsEqual(i.object, three));
    HCListIterationNext(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_FALSE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 4);
    ASSERT_TRUE(HCIsEqual(i.object, four));
    HCListIterationNext(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_FALSE(HCListIterationHasNext(&i));
    ASSERT_TRUE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_FALSE(HCListContainsIndex(list, i.index));
    ASSERT_NULL(i.object);
    
    i = HCListIterationBeginAtLast(list);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_FALSE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 4);
    ASSERT_TRUE(HCIsEqual(i.object, four));
    HCListIterationPrevious(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 3);
    ASSERT_TRUE(HCIsEqual(i.object, three));
    HCListIterationPrevious(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 2);
    ASSERT_TRUE(HCIsEqual(i.object, two));
    HCListIterationPrevious(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 1);
    ASSERT_TRUE(HCIsEqual(i.object, one));
    HCListIterationPrevious(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_FALSE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 0);
    ASSERT_TRUE(HCIsEqual(i.object, zero));
    HCListIterationPrevious(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_FALSE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_TRUE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, -1);
    ASSERT_NULL(i.object);
    HCListIterationPrevious(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_FALSE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_TRUE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, -1);
    ASSERT_NULL(i.object);
    HCListIterationNext(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_FALSE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 0);
    ASSERT_TRUE(HCIsEqual(i.object, zero));
    HCListIterationNext(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 1);
    ASSERT_TRUE(HCIsEqual(i.object, one));
    HCListIterationNext(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 2);
    ASSERT_TRUE(HCIsEqual(i.object, two));
    HCListIterationNext(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 3);
    ASSERT_TRUE(HCIsEqual(i.object, three));
    HCListIterationNext(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_FALSE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 4);
    ASSERT_TRUE(HCIsEqual(i.object, four));
    HCListIterationNext(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_FALSE(HCListIterationHasNext(&i));
    ASSERT_TRUE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 5);
    ASSERT_NULL(i.object);
    HCListIterationNext(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_FALSE(HCListIterationHasNext(&i));
    ASSERT_TRUE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 5);
    ASSERT_NULL(i.object);
    HCListIterationPrevious(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_FALSE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 4);
    ASSERT_TRUE(HCIsEqual(i.object, four));
    HCListIterationPrevious(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_TRUE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_FALSE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, 3);
    ASSERT_TRUE(HCIsEqual(i.object, three));
    HCListIterationEnd(&i);
    ASSERT_TRUE(HCListIterationHasBegun(&i));
    ASSERT_FALSE(HCListIterationHasPrevious(&i));
    ASSERT_TRUE(HCListIterationHasNext(&i));
    ASSERT_TRUE(HCListIterationHasEnded(&i));
    ASSERT_TRUE(HCIsEqual(i.list, list));
    ASSERT_EQUAL(i.index, HCListNotFound);
    ASSERT_NULL(i.object);
    
    HCRelease(list);
    HCRelease(zero);
    HCRelease(one);
    HCRelease(two);
    HCRelease(three);
    HCRelease(four);
}

CTEST(HCList, IterationLoop) {
    HCListRef list = HCListCreate();
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(0));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(1));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(2));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(3));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(4));
    
    HCInteger count = 0;
    for (HCListIterator i = HCListIterationBegin(list); !HCListIterationHasEnded(&i); HCListIterationNext(&i)) {
        ASSERT_NOT_NULL(i.object);
        count++;
    }
    ASSERT_TRUE(count == HCListCount(list));
    
    HCRelease(list);
}

CTEST(HCList, IterationLoopBackward) {
    HCListRef list = HCListCreate();
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(0));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(1));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(2));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(3));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(4));
    
    HCInteger count = 0;
    for (HCListIterator i = HCListIterationBeginAtLast(list); !HCListIterationHasEnded(&i); HCListIterationPrevious(&i)) {
        ASSERT_NOT_NULL(i.object);
        count++;
    }
    ASSERT_TRUE(count == HCListCount(list));
    
    HCRelease(list);
}

void HCListForEachTestFunction(void* context, HCRef value) {
    HCInteger* counter = (HCInteger*)context;
    ASSERT_TRUE(HCNumberAsInteger(value) == *counter);
    *counter += 1;
}

HCInteger HCListForEachTestFunctionCounter = 0;

CTEST(HCList, ForEach) {
    HCListRef list = HCListCreate();
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(0));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(1));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(2));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(3));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(4));
    
    HCListForEach(list, HCListForEachTestFunction, &HCListForEachTestFunctionCounter);
    ASSERT_EQUAL(HCListForEachTestFunctionCounter, 5);
    
    HCRelease(list);
}

HCBoolean HCListFilterTestFunction(void* context, HCRef value) {
    ASSERT_TRUE(context == (void*)0xDEADBEEF);
    return HCNumberAsInteger(value) >= 2;
}

CTEST(HCList, Filter) {
    HCListRef list = HCListCreate();
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(0));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(1));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(2));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(3));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(4));
    
    HCListRef filtered = HCListFilterRetained(list, HCListFilterTestFunction, (void*)0xDEADBEEF);
    ASSERT_EQUAL(HCListCount(filtered), 3);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(filtered, 0)), 2);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(filtered, 1)), 3);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(filtered, 2)), 4);
    
    HCRelease(list);
    HCRelease(filtered);
}

HCRef HCListMapTestFunction(void* context, HCRef value) {
    ASSERT_TRUE(context == (void*)0xDEADBEEF);
    return HCNumberCreateWithInteger(HCNumberAsInteger(value) * -1);
}

HCInteger HCListMapTestFunctionCounter = 0;

CTEST(HCList, Map) {
    HCListRef list = HCListCreate();
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(0));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(1));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(2));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(3));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(4));
    
    HCListRef transformed = HCListMapRetained(list, HCListMapTestFunction, (void*)0xDEADBEEF);
    ASSERT_EQUAL(HCListCount(transformed), 5);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(transformed, 0)), 0);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(transformed, 1)), -1);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(transformed, 2)), -2);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(transformed, 3)), -3);
    ASSERT_EQUAL(HCNumberAsInteger(HCListObjectAtIndex(transformed, 4)), -4);
    
    HCRelease(list);
    HCRelease(transformed);
}

HCRef HCListReduceTestFunction(void* context, HCRef aggregate, HCRef value) {
    ASSERT_TRUE(context == (void*)0xDEADBEEF);
    return HCNumberCreateWithInteger(HCNumberAsInteger(aggregate) + HCNumberAsInteger(value));
}

HCInteger HCListReduceTestFunctionCounter = 0;

CTEST(HCList, Reduce) {
    HCListRef list = HCListCreate();
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(0));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(1));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(2));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(3));
    HCListAddObjectReleased(list, HCNumberCreateWithInteger(4));
    
    HCNumberRef initialValue = HCNumberCreateWithInteger(10);
    HCNumberRef total = HCListReduceRetained(list, initialValue, HCListReduceTestFunction, (void*)0xDEADBEEF);
    ASSERT_EQUAL(HCNumberAsInteger(total), 10 + 0 + 1 + 2 + 3 + 4);
    
    HCRelease(initialValue);
    HCRelease(total);
    HCRelease(list);
}
