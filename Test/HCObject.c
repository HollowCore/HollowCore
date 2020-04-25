///
/// @file HCObject.c
/// @package HollowCore
///
/// @author Matt Stoker
/// @date 1/17/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <string.h>

CTEST(HCObject, Creation) {
    HCNumberRef number = HCNumberCreate();
    ASSERT_FALSE(HCNumberAsBoolean(number));
    HCRelease(number);
}

CTEST(HCObject, Type) {
    HCNumberRef number = HCNumberCreate();
    ASSERT_TRUE(strcmp(HCObjectName(number), HCNumberType->name) == 0);
    ASSERT_TRUE(HCObjectAncestor(number) == HCNumberType->ancestor);
    ASSERT_FALSE(HCObjectHasAncestor(number, HCNumberType));
    ASSERT_TRUE(HCObjectHasAncestor(number, HCObjectType));
    ASSERT_FALSE(HCObjectHasAncestor(NULL, HCObjectType));
    ASSERT_FALSE(HCObjectHasAncestor(number, NULL));
    ASSERT_FALSE(HCObjectHasAncestor(NULL, NULL));
    ASSERT_TRUE(HCObjectIsOfType(number, HCNumberType));
    ASSERT_FALSE(HCObjectIsOfType(number, HCObjectType));
    ASSERT_FALSE(HCObjectIsOfType(NULL, HCObjectType));
    ASSERT_FALSE(HCObjectIsOfType(number, NULL));
    ASSERT_FALSE(HCObjectIsOfType(NULL, NULL));
    ASSERT_TRUE(HCObjectIsOfKind(number, HCNumberType));
    ASSERT_TRUE(HCObjectIsOfKind(number, HCObjectType));
    ASSERT_FALSE(HCObjectIsOfKind(NULL, HCObjectType));
    ASSERT_FALSE(HCObjectIsOfKind(number, NULL));
    ASSERT_FALSE(HCObjectIsOfKind(NULL, NULL));
    HCRelease(number);
}

CTEST(HCObject, RetainRelease) {
    HCNumberRef number = HCNumberCreate();
    HCRetain(number);
    HCRelease(number);
    HCRelease(number);
}

CTEST(HCObject, EqualHash) {
    HCNumberRef a = HCNumberCreate();
    HCNumberRef b = HCNumberCreate();
    ASSERT_TRUE(HCObjectIsEqual((HCObjectRef)a, (HCObjectRef)a));
    ASSERT_FALSE(HCObjectIsEqual((HCObjectRef)a, (HCObjectRef)b));
    ASSERT_EQUAL(HCObjectHashValue((HCObjectRef)a), HCObjectHashValue((HCObjectRef)a));
    ASSERT_TRUE(HCIsEqual(a, a));
    ASSERT_TRUE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCObject, Print) {
    HCNumberRef a = HCNumberCreate();
    HCObjectPrint((HCObjectRef)a, stdout); // TODO: Not to stdout
    HCPrint(a, stdout); // TODO: Not to stdout
    HCRelease(a);
}
