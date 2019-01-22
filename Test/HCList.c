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

CTEST(HCList, Access) {
    HCNumberRef number = HCNumberCreateWithInteger(42);
    HCListRef list = HCListCreate();
    ASSERT_TRUE(HCListIsEmpty(list));
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
