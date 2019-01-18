//
//  HCString.c
//  Test
//
//  Created by Matt Stoker on 1/18/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"

CTEST(HCString, Creation) {
    HCStringRef empty = HCStringCreate();
    ASSERT_TRUE(HCStringIsEmpty(empty));
    HCRelease(empty);
}

CTEST(HCString, Boolean) {
    HCStringRef falseString = HCStringCreateWithBoolean(false);
    ASSERT_FALSE(HCStringIsEmpty(falseString));
    ASSERT_STR(HCStringAsCString(falseString), "⊭");
    HCRelease(falseString);
}

CTEST(HCString, Integer) {
    HCStringRef everythingString = HCStringCreateWithInteger(42);
    ASSERT_FALSE(HCStringIsEmpty(everythingString));
    ASSERT_STR(HCStringAsCString(everythingString), "42");
    HCRelease(everythingString);
}

CTEST(HCString, Real) {
    HCStringRef halfString = HCStringCreateWithReal(0.5);
    ASSERT_FALSE(HCStringIsEmpty(halfString));
    ASSERT_STR(HCStringAsCString(halfString), "0.5");
    HCRelease(halfString);
}

CTEST(HCString, CString) {
    HCStringRef generalString = HCStringCreateWithCString("I am the very model of a modern Major General.");
    ASSERT_FALSE(HCStringIsEmpty(generalString));
    ASSERT_STR(HCStringAsCString(generalString), "I am the very model of a modern Major General.");
    HCRelease(generalString);
}
