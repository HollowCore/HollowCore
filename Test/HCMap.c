///
/// @file HCMap.c
/// @package Test
///
/// @author Matt Stoker
/// @date 2/2/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <math.h>

CTEST(HCMap, Creation) {
    HCMapRef empty = HCMapCreate();
    ASSERT_TRUE(HCMapIsEmpty(empty));
    HCRelease(empty);
}

CTEST(HCMap, Invalid) {
    ASSERT_TRUE(HCMapIterationHasBegun((HCMapIterator*)&HCMapIteratorInvalid));
    ASSERT_FALSE(HCMapIterationHasNext((HCMapIterator*)&HCMapIteratorInvalid));
    ASSERT_TRUE(HCMapIterationHasEnded((HCMapIterator*)&HCMapIteratorInvalid));
}

CTEST(HCMap, Access) {
    HCStringRef key = HCStringCreateWithInteger(42);
    HCNumberRef number = HCNumberCreateWithInteger(42);
    HCMapRef map = HCMapCreate();
    ASSERT_TRUE(HCMapIsEmpty(map));
    ASSERT_FALSE(HCMapContainsKey(map, key));
    ASSERT_FALSE(HCMapContainsObject(map, number));
    HCMapAddObjectForKey(map, key, number);
    ASSERT_FALSE(HCMapIsEmpty(map));
    ASSERT_TRUE(HCMapContainsKey(map, key));
    ASSERT_TRUE(HCIsEqual(HCMapFirstKey(map), key));
    ASSERT_TRUE(HCIsEqual(HCMapLastKey(map), key));
    ASSERT_TRUE(HCMapContainsObject(map, number));
    ASSERT_TRUE(HCIsEqual(HCMapFirstObject(map), number));
    ASSERT_TRUE(HCIsEqual(HCMapLastObject(map), number));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, key), number));
    HCRelease(map);
    HCRelease(key);
    HCRelease(number);
}

CTEST(HCMap, AddRemove) {
    HCMapRef map = HCMapCreate();
    HCStringRef zeroKey = HCStringCreateWithInteger(0);
    HCStringRef oneKey = HCStringCreateWithInteger(1);
    HCStringRef twoKey = HCStringCreateWithInteger(2);
    HCStringRef threeKey = HCStringCreateWithInteger(3);
    HCNumberRef zero = HCNumberCreateWithInteger(0);
    HCNumberRef one = HCNumberCreateWithInteger(1);
    HCNumberRef two = HCNumberCreateWithInteger(2);
    HCNumberRef three = HCNumberCreateWithInteger(3);
    HCMapAddObjectForKey(map, zeroKey, zero);
    ASSERT_FALSE(HCMapIsEmpty(map));
    ASSERT_EQUAL(HCMapCount(map), 1);
    ASSERT_TRUE(HCMapContainsKey(map, zeroKey));
    ASSERT_FALSE(HCMapContainsKey(map, oneKey));
    ASSERT_FALSE(HCMapContainsKey(map, twoKey));
    ASSERT_FALSE(HCMapContainsKey(map, threeKey));
    ASSERT_NOT_NULL(HCMapFirstKey(map));
    ASSERT_NOT_NULL(HCMapLastKey(map));
    ASSERT_TRUE(HCMapContainsObject(map, zero));
    ASSERT_FALSE(HCMapContainsObject(map, one));
    ASSERT_FALSE(HCMapContainsObject(map, two));
    ASSERT_FALSE(HCMapContainsObject(map, three));
    ASSERT_NOT_NULL(HCMapFirstObject(map));
    ASSERT_NOT_NULL(HCMapLastObject(map));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, zeroKey), zero));
    ASSERT_NULL(HCMapObjectForKey(map, oneKey));
    ASSERT_NULL(HCMapObjectForKey(map, twoKey));
    ASSERT_NULL(HCMapObjectForKey(map, threeKey));
    HCMapAddObjectForKey(map, oneKey, one);
    HCMapAddObjectForKey(map, twoKey, two);
    HCMapAddObjectForKey(map, threeKey, three);
    ASSERT_FALSE(HCMapIsEmpty(map));
    ASSERT_EQUAL(HCMapCount(map), 4);
    ASSERT_TRUE(HCMapContainsKey(map, zeroKey));
    ASSERT_TRUE(HCMapContainsKey(map, oneKey));
    ASSERT_TRUE(HCMapContainsKey(map, twoKey));
    ASSERT_TRUE(HCMapContainsKey(map, threeKey));
    ASSERT_NOT_NULL(HCMapFirstKey(map));
    ASSERT_NOT_NULL(HCMapLastKey(map));
    ASSERT_TRUE(HCMapContainsObject(map, zero));
    ASSERT_TRUE(HCMapContainsObject(map, one));
    ASSERT_TRUE(HCMapContainsObject(map, two));
    ASSERT_TRUE(HCMapContainsObject(map, three));
    ASSERT_NOT_NULL(HCMapFirstObject(map));
    ASSERT_NOT_NULL(HCMapLastObject(map));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, zeroKey), zero));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, oneKey), one));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, twoKey), two));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, threeKey), three));
    HCMapRemoveObjectForKey(map, twoKey);
    ASSERT_FALSE(HCMapIsEmpty(map));
    ASSERT_EQUAL(HCMapCount(map), 3);
    ASSERT_TRUE(HCMapContainsKey(map, zeroKey));
    ASSERT_TRUE(HCMapContainsKey(map, oneKey));
    ASSERT_FALSE(HCMapContainsKey(map, twoKey));
    ASSERT_TRUE(HCMapContainsKey(map, threeKey));
    ASSERT_NOT_NULL(HCMapFirstKey(map));
    ASSERT_NOT_NULL(HCMapLastKey(map));
    ASSERT_TRUE(HCMapContainsObject(map, zero));
    ASSERT_TRUE(HCMapContainsObject(map, one));
    ASSERT_FALSE(HCMapContainsObject(map, two));
    ASSERT_TRUE(HCMapContainsObject(map, three));
    ASSERT_NOT_NULL(HCMapFirstObject(map));
    ASSERT_NOT_NULL(HCMapLastObject(map));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, zeroKey), zero));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, oneKey), one));
    ASSERT_NULL(HCMapObjectForKey(map, twoKey));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, threeKey), three));
    HCMapRemoveObjectForKey(map, oneKey);
    ASSERT_FALSE(HCMapIsEmpty(map));
    ASSERT_EQUAL(HCMapCount(map), 2);
    ASSERT_TRUE(HCMapContainsKey(map, zeroKey));
    ASSERT_FALSE(HCMapContainsKey(map, oneKey));
    ASSERT_FALSE(HCMapContainsKey(map, twoKey));
    ASSERT_TRUE(HCMapContainsKey(map, threeKey));
    ASSERT_NOT_NULL(HCMapFirstKey(map));
    ASSERT_NOT_NULL(HCMapLastKey(map));
    ASSERT_TRUE(HCMapContainsObject(map, zero));
    ASSERT_FALSE(HCMapContainsObject(map, one));
    ASSERT_FALSE(HCMapContainsObject(map, two));
    ASSERT_TRUE(HCMapContainsObject(map, three));
    ASSERT_NOT_NULL(HCMapFirstObject(map));
    ASSERT_NOT_NULL(HCMapLastObject(map));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, zeroKey), zero));
    ASSERT_NULL(HCMapObjectForKey(map, oneKey));
    ASSERT_NULL(HCMapObjectForKey(map, twoKey));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, threeKey), three));
    HCMapRemoveObjectForKey(map, zeroKey);
    ASSERT_FALSE(HCMapIsEmpty(map));
    ASSERT_EQUAL(HCMapCount(map), 1);
    ASSERT_FALSE(HCMapContainsKey(map, zeroKey));
    ASSERT_FALSE(HCMapContainsKey(map, oneKey));
    ASSERT_FALSE(HCMapContainsKey(map, twoKey));
    ASSERT_TRUE(HCMapContainsKey(map, threeKey));
    ASSERT_NOT_NULL(HCMapFirstKey(map));
    ASSERT_NOT_NULL(HCMapLastKey(map));
    ASSERT_FALSE(HCMapContainsObject(map, zero));
    ASSERT_FALSE(HCMapContainsObject(map, one));
    ASSERT_FALSE(HCMapContainsObject(map, two));
    ASSERT_TRUE(HCMapContainsObject(map, three));
    ASSERT_NOT_NULL(HCMapFirstObject(map));
    ASSERT_NOT_NULL(HCMapLastObject(map));
    ASSERT_NULL(HCMapObjectForKey(map, zeroKey));
    ASSERT_NULL(HCMapObjectForKey(map, oneKey));
    ASSERT_NULL(HCMapObjectForKey(map, twoKey));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, threeKey), three));
    HCMapRemoveObjectForKey(map, threeKey);
    ASSERT_TRUE(HCMapIsEmpty(map));
    ASSERT_EQUAL(HCMapCount(map), 0);
    ASSERT_FALSE(HCMapContainsKey(map, zeroKey));
    ASSERT_FALSE(HCMapContainsKey(map, oneKey));
    ASSERT_FALSE(HCMapContainsKey(map, twoKey));
    ASSERT_FALSE(HCMapContainsKey(map, threeKey));
    ASSERT_NULL(HCMapFirstObject(map));
    ASSERT_NULL(HCMapLastObject(map));
    ASSERT_FALSE(HCMapContainsObject(map, zero));
    ASSERT_FALSE(HCMapContainsObject(map, one));
    ASSERT_FALSE(HCMapContainsObject(map, two));
    ASSERT_FALSE(HCMapContainsObject(map, three));
    ASSERT_NULL(HCMapFirstObject(map));
    ASSERT_NULL(HCMapLastObject(map));
    ASSERT_NULL(HCMapObjectForKey(map, zeroKey));
    ASSERT_NULL(HCMapObjectForKey(map, oneKey));
    ASSERT_NULL(HCMapObjectForKey(map, twoKey));
    ASSERT_NULL(HCMapObjectForKey(map, threeKey));
    HCRelease(map);
    HCRelease(zeroKey);
    HCRelease(oneKey);
    HCRelease(twoKey);
    HCRelease(threeKey);
    HCRelease(zero);
    HCRelease(one);
    HCRelease(two);
    HCRelease(three);
}

CTEST(HCMap, AddRemoveMany) {
    HCInteger count = 100;
    HCMapRef map = HCMapCreate();
    for (HCInteger index = 0; index < count; index++) {
        HCMapAddObjectReleasedForKeyReleased(map, HCStringCreateWithInteger(index), HCNumberCreateWithInteger(index));
        ASSERT_EQUAL(HCMapCount(map), index + 1);
        for (HCInteger checkIndex = 0; checkIndex <= index; checkIndex++) {
            HCStringRef key = HCStringCreateWithInteger(checkIndex);
            HCNumberRef object = HCNumberCreateWithInteger(checkIndex);
            ASSERT_TRUE(HCMapContainsKey(map, key));
            ASSERT_TRUE(HCMapContainsObject(map, object));
            HCRelease(key);
            HCRelease(object);
        }
    }
    for (HCInteger index = 0; index < count; index++) {
        HCStringRef key = HCStringCreateWithInteger(index);
        HCNumberRef object = HCNumberCreateWithInteger(index);
        HCMapRemoveObjectForKey(map, key);
        ASSERT_FALSE(HCMapContainsKey(map, key));
        ASSERT_FALSE(HCMapContainsObject(map, object));
        HCRelease(key);
        HCRelease(object);
    }
    HCRelease(map);
}

CTEST(HCMap, EqualHash) {
    HCMapRef a = HCMapCreate();
    HCMapAddObjectReleasedForCStringKey(a, "0", HCNumberCreateWithInteger(0));
    HCMapAddObjectReleasedForCStringKey(a, "1", HCNumberCreateWithInteger(1));
    HCMapAddObjectReleasedForCStringKey(a, "2", HCNumberCreateWithInteger(2));
    HCMapRef b = HCMapCreate();
    HCMapAddObjectReleasedForCStringKey(b, "0", HCNumberCreateWithInteger(0));
    HCMapAddObjectReleasedForCStringKey(b, "1", HCNumberCreateWithInteger(1));
    HCMapAddObjectReleasedForCStringKey(b, "2", HCNumberCreateWithInteger(2));
    HCMapRef c = HCMapCreate();
    HCMapAddObjectReleasedForCStringKey(c, "2", HCNumberCreateWithInteger(2));
    HCMapAddObjectReleasedForCStringKey(c, "1", HCNumberCreateWithInteger(1));
    ASSERT_TRUE(HCMapIsEqual(a, a));
    ASSERT_TRUE(HCMapIsEqual(b, b));
    ASSERT_TRUE(HCMapIsEqual(c, c));
    ASSERT_TRUE(HCMapIsEqual(a, b));
    ASSERT_TRUE(HCMapIsEqual(b, a));
    ASSERT_FALSE(HCMapIsEqual(a, c));
    ASSERT_FALSE(HCMapIsEqual(c, a));
    ASSERT_EQUAL(HCMapHashValue(a), HCMapHashValue(a));
    ASSERT_EQUAL(HCMapHashValue(a), HCMapHashValue(b));
    HCRelease(a);
    HCRelease(b);
    HCRelease(c);
}

CTEST(HCMap, Print) {
    HCMapRef a = HCMapCreate();
    HCMapPrint(a, stdout); // TODO: Not to stdout
    HCPrint(a, stdout); // TODO: Not to stdout
    HCRelease(a);
}

CTEST(HCMap, Clear) {
    HCMapRef map = HCMapCreate();
    ASSERT_TRUE(HCMapIsEmpty(map));
    HCMapAddObjectReleasedForCStringKey(map, "3", HCNumberCreateWithInteger(3));
    HCMapAddObjectReleasedForCStringKey(map, "4", HCNumberCreateWithInteger(4));
    HCMapAddObjectReleasedForCStringKey(map, "5", HCNumberCreateWithInteger(5));
    ASSERT_FALSE(HCMapIsEmpty(map));
    HCMapClear(map);
    ASSERT_TRUE(HCMapIsEmpty(map));
    HCRelease(map);
}

CTEST(HCMap, Search) {
    HCStringRef zeroKey = HCStringCreateWithInteger(0);
    HCStringRef oneKey = HCStringCreateWithInteger(1);
    HCStringRef twoKey = HCStringCreateWithInteger(2);
    HCStringRef threeKey = HCStringCreateWithInteger(3);
    HCStringRef fourKey = HCStringCreateWithInteger(4);
    HCNumberRef zero = HCNumberCreateWithInteger(0);
    HCNumberRef one = HCNumberCreateWithInteger(1);
    HCNumberRef two = HCNumberCreateWithInteger(2);
    HCNumberRef three = HCNumberCreateWithInteger(3);
    HCNumberRef four = HCNumberCreateWithInteger(4);
    HCMapRef map = HCMapCreate();
    ASSERT_FALSE(HCMapContainsKey(map, zeroKey));
    ASSERT_FALSE(HCMapContainsKey(map, oneKey));
    ASSERT_FALSE(HCMapContainsKey(map, twoKey));
    ASSERT_FALSE(HCMapContainsKey(map, threeKey));
    ASSERT_FALSE(HCMapContainsKey(map, fourKey));
    ASSERT_FALSE(HCMapContainsObject(map, zero));
    ASSERT_FALSE(HCMapContainsObject(map, one));
    ASSERT_FALSE(HCMapContainsObject(map, two));
    ASSERT_FALSE(HCMapContainsObject(map, three));
    ASSERT_FALSE(HCMapContainsObject(map, four));
    HCMapAddObjectForKey(map, zeroKey, zero);
    HCMapAddObjectForKey(map, oneKey, one);
    HCMapAddObjectForKey(map, twoKey, two);
    HCMapAddObjectForKey(map, threeKey, three);
    HCMapAddObjectForKey(map, fourKey, four);
    ASSERT_EQUAL(HCMapCount(map), 5);
    ASSERT_TRUE(HCMapContainsKey(map, zeroKey));
    ASSERT_TRUE(HCMapContainsKey(map, oneKey));
    ASSERT_TRUE(HCMapContainsKey(map, twoKey));
    ASSERT_TRUE(HCMapContainsKey(map, threeKey));
    ASSERT_TRUE(HCMapContainsKey(map, fourKey));
    ASSERT_TRUE(HCMapContainsObject(map, zero));
    ASSERT_TRUE(HCMapContainsObject(map, one));
    ASSERT_TRUE(HCMapContainsObject(map, two));
    ASSERT_TRUE(HCMapContainsObject(map, three));
    ASSERT_TRUE(HCMapContainsObject(map, four));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, zeroKey), zero));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, oneKey), one));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, twoKey), two));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, threeKey), three));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, fourKey), four));
    
    HCMapAddObjectForKey(map, fourKey, zero);
    HCMapAddObjectForKey(map, threeKey, one);
    HCMapAddObjectForKey(map, twoKey, two);
    HCMapAddObjectForKey(map, oneKey, three);
    HCMapAddObjectForKey(map, zeroKey, four);
    ASSERT_EQUAL(HCMapCount(map), 5);
    ASSERT_TRUE(HCMapContainsKey(map, zeroKey));
    ASSERT_TRUE(HCMapContainsKey(map, oneKey));
    ASSERT_TRUE(HCMapContainsKey(map, twoKey));
    ASSERT_TRUE(HCMapContainsKey(map, threeKey));
    ASSERT_TRUE(HCMapContainsKey(map, fourKey));
    ASSERT_TRUE(HCMapContainsObject(map, zero));
    ASSERT_TRUE(HCMapContainsObject(map, one));
    ASSERT_TRUE(HCMapContainsObject(map, two));
    ASSERT_TRUE(HCMapContainsObject(map, three));
    ASSERT_TRUE(HCMapContainsObject(map, four));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, fourKey), zero));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, threeKey), one));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, twoKey), two));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, oneKey), three));
    ASSERT_TRUE(HCIsEqual(HCMapObjectForKey(map, zeroKey), four));
    
    HCRelease(map);
    HCRelease(zeroKey);
    HCRelease(oneKey);
    HCRelease(twoKey);
    HCRelease(threeKey);
    HCRelease(fourKey);
    HCRelease(zero);
    HCRelease(one);
    HCRelease(two);
    HCRelease(three);
    HCRelease(four);
}

CTEST(HCMap, MemoryConvenience) {
    HCNumberRef zero = HCNumberCreateWithInteger(0);
    HCNumberRef one = HCNumberCreateWithInteger(1);
    HCNumberRef two = HCNumberCreateWithInteger(2);
    HCNumberRef three = HCNumberCreateWithInteger(3);
    HCNumberRef four = HCNumberCreateWithInteger(4);
    HCMapRef map = HCMapCreate();
    HCMapAddObjectForCStringKey(map, "0", zero);
    HCMapAddObjectForCStringKey(map, "1", one);
    HCMapAddObjectReleasedForCStringKey(map, "2", HCRetain(two));
    HCMapAddObjectReleasedForCStringKey(map, "3", HCRetain(three));
    HCMapAddObjectReleasedForCStringKey(map, "4", HCRetain(four));
    
    ASSERT_TRUE(HCMapContainsCStringKey(map, "2"));
    ASSERT_FALSE(HCMapContainsCStringKey(map, "two"));
    
    ASSERT_TRUE(HCIsEqual(HCMapObjectForCStringKey(map, "2"), two));
    
    ASSERT_TRUE(HCMapContainsCStringKey(map, "1"));
    HCMapRemoveObjectForCStringKey(map, "1");
    ASSERT_FALSE(HCMapContainsCStringKey(map, "1"));
    
    HCNumberRef a = HCMapRemoveObjectRetainedForCStringKey(map, "2");
    ASSERT_TRUE(HCIsEqual(a, two));
    HCRelease(a);
    
    HCNumberRef b = HCMapRemoveObjectRetainedForKeyReleased(map, HCStringCreateWithInteger(4));
    ASSERT_TRUE(HCIsEqual(b, four));
    HCRelease(b);
    
    HCRelease(map);
    HCRelease(zero);
    HCRelease(one);
    HCRelease(two);
    HCRelease(three);
    HCRelease(four);
}

CTEST(HCMap, Iteration) {
    HCStringRef zeroKey = HCStringCreateWithInteger(0);
    HCStringRef oneKey = HCStringCreateWithInteger(1);
    HCStringRef twoKey = HCStringCreateWithInteger(2);
    HCStringRef threeKey = HCStringCreateWithInteger(3);
    HCStringRef fourKey = HCStringCreateWithInteger(4);
    HCNumberRef zero = HCNumberCreateWithInteger(0);
    HCNumberRef one = HCNumberCreateWithInteger(1);
    HCNumberRef two = HCNumberCreateWithInteger(2);
    HCNumberRef three = HCNumberCreateWithInteger(3);
    HCNumberRef four = HCNumberCreateWithInteger(4);
    HCMapRef map = HCMapCreate();
    HCMapAddObjectForKey(map, zeroKey, zero);
    HCMapAddObjectForKey(map, oneKey, one);
    HCMapAddObjectForKey(map, twoKey, two);
    HCMapAddObjectForKey(map, threeKey, three);
    HCMapAddObjectForKey(map, fourKey, four);
    
    HCMapIterator i = HCMapIteratorInvalid;
    ASSERT_TRUE(HCMapIterationHasBegun(&i));
    ASSERT_FALSE(HCMapIterationHasNext(&i));
    ASSERT_TRUE(HCMapIterationHasEnded(&i));
    i = HCMapIterationBegin(map);
    ASSERT_TRUE(HCMapIterationHasBegun(&i));
    ASSERT_FALSE(HCMapIterationHasEnded(&i));
    ASSERT_TRUE(HCMapIterationHasNext(&i));
    ASSERT_TRUE(HCIsEqual(i.map, map));
    ASSERT_EQUAL(i.index, 0);
    ASSERT_TRUE(HCIsEqual(i.key, zeroKey) || HCIsEqual(i.key, oneKey) || HCIsEqual(i.key, twoKey) || HCIsEqual(i.key, threeKey) || HCIsEqual(i.key, fourKey));
    ASSERT_TRUE(HCIsEqual(i.object, zero) || HCIsEqual(i.object, one) || HCIsEqual(i.object, two) || HCIsEqual(i.object, three) || HCIsEqual(i.object, four));
    HCMapIterationNext(&i);
    ASSERT_TRUE(HCMapIterationHasBegun(&i));
    ASSERT_FALSE(HCMapIterationHasEnded(&i));
    ASSERT_TRUE(HCMapIterationHasNext(&i));
    ASSERT_TRUE(HCIsEqual(i.map, map));
    ASSERT_EQUAL(i.index, 1);
    ASSERT_TRUE(HCIsEqual(i.key, zeroKey) || HCIsEqual(i.key, oneKey) || HCIsEqual(i.key, twoKey) || HCIsEqual(i.key, threeKey) || HCIsEqual(i.key, fourKey));
    ASSERT_TRUE(HCIsEqual(i.object, zero) || HCIsEqual(i.object, one) || HCIsEqual(i.object, two) || HCIsEqual(i.object, three) || HCIsEqual(i.object, four));
    HCMapIterationNext(&i);
    ASSERT_TRUE(HCMapIterationHasBegun(&i));
    ASSERT_FALSE(HCMapIterationHasEnded(&i));
    ASSERT_TRUE(HCMapIterationHasNext(&i));
    ASSERT_TRUE(HCIsEqual(i.map, map));
    ASSERT_EQUAL(i.index, 2);
    ASSERT_TRUE(HCIsEqual(i.key, zeroKey) || HCIsEqual(i.key, oneKey) || HCIsEqual(i.key, twoKey) || HCIsEqual(i.key, threeKey) || HCIsEqual(i.key, fourKey));
    ASSERT_TRUE(HCIsEqual(i.object, zero) || HCIsEqual(i.object, one) || HCIsEqual(i.object, two) || HCIsEqual(i.object, three) || HCIsEqual(i.object, four));
    HCMapIterationNext(&i);
    ASSERT_TRUE(HCMapIterationHasBegun(&i));
    ASSERT_FALSE(HCMapIterationHasEnded(&i));
    ASSERT_TRUE(HCMapIterationHasNext(&i));
    ASSERT_TRUE(HCIsEqual(i.map, map));
    ASSERT_EQUAL(i.index, 3);
    ASSERT_TRUE(HCIsEqual(i.key, zeroKey) || HCIsEqual(i.key, oneKey) || HCIsEqual(i.key, twoKey) || HCIsEqual(i.key, threeKey) || HCIsEqual(i.key, fourKey));
    ASSERT_TRUE(HCIsEqual(i.object, zero) || HCIsEqual(i.object, one) || HCIsEqual(i.object, two) || HCIsEqual(i.object, three) || HCIsEqual(i.object, four));
    HCMapIterationNext(&i);
    ASSERT_TRUE(HCMapIterationHasBegun(&i));
    ASSERT_FALSE(HCMapIterationHasEnded(&i));
    ASSERT_FALSE(HCMapIterationHasNext(&i));
    ASSERT_TRUE(HCIsEqual(i.map, map));
    ASSERT_EQUAL(i.index, 4);
    ASSERT_TRUE(HCIsEqual(i.key, zeroKey) || HCIsEqual(i.key, oneKey) || HCIsEqual(i.key, twoKey) || HCIsEqual(i.key, threeKey) || HCIsEqual(i.key, fourKey));
    ASSERT_TRUE(HCIsEqual(i.object, zero) || HCIsEqual(i.object, one) || HCIsEqual(i.object, two) || HCIsEqual(i.object, three) || HCIsEqual(i.object, four));
    HCMapIterationNext(&i);
    ASSERT_TRUE(HCMapIterationHasBegun(&i));
    ASSERT_TRUE(HCMapIterationHasEnded(&i));
    ASSERT_FALSE(HCMapIterationHasNext(&i));
    ASSERT_TRUE(HCIsEqual(i.map, map));
    ASSERT_NULL(i.object);
    ASSERT_NULL(i.key);
    HCMapIterationEnd(&i);

    HCRelease(map);
    HCRelease(zeroKey);
    HCRelease(oneKey);
    HCRelease(twoKey);
    HCRelease(threeKey);
    HCRelease(fourKey);
    HCRelease(zero);
    HCRelease(one);
    HCRelease(two);
    HCRelease(three);
    HCRelease(four);
}

CTEST(HCMap, IterationLoop) {
    HCMapRef map = HCMapCreate();
    HCMapAddObjectReleasedForKeyReleased(map, HCStringCreateWithInteger(0), HCNumberCreateWithInteger(0));
    HCMapAddObjectReleasedForKeyReleased(map, HCStringCreateWithInteger(1), HCNumberCreateWithInteger(1));
    HCMapAddObjectReleasedForKeyReleased(map, HCStringCreateWithInteger(2), HCNumberCreateWithInteger(2));
    HCMapAddObjectReleasedForKeyReleased(map, HCStringCreateWithInteger(3), HCNumberCreateWithInteger(3));
    HCMapAddObjectReleasedForKeyReleased(map, HCStringCreateWithInteger(4), HCNumberCreateWithInteger(4));
    
    HCInteger count = 0;
    for (HCMapIterator i = HCMapIterationBegin(map); !HCMapIterationHasEnded(&i); HCMapIterationNext(&i)) {
        ASSERT_NOT_NULL(i.object);
        count++;
    }
    ASSERT_TRUE(count == HCMapCount(map));
    
    HCRelease(map);
}
