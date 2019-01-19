//
//  HCString.c
//  Test
//
//  Created by Matt Stoker on 1/18/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <string.h>

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

CTEST(HCString, CodeUnits) {
    const char* simple = "ABCD";
    HCStringRef simpleString = HCStringCreateWithCString(simple);
    ASSERT_EQUAL(HCStringGetCodeUnitCount(simpleString), 4);
    ASSERT_EQUAL(HCStringGetCodeUnit(simpleString, 0), (HCStringCodeUnit)simple[0]);
    ASSERT_EQUAL(HCStringGetCodeUnit(simpleString, 1), (HCStringCodeUnit)simple[1]);
    ASSERT_EQUAL(HCStringGetCodeUnit(simpleString, 2), (HCStringCodeUnit)simple[2]);
    ASSERT_EQUAL(HCStringGetCodeUnit(simpleString, 3), (HCStringCodeUnit)simple[3]);
    HCRelease(simpleString);
    
    const char* complex = "A⊭B⊭C⊭D";
    HCStringRef complexString = HCStringCreateWithCString(complex);
    ASSERT_EQUAL(HCStringGetCodeUnitCount(complexString), 13);
    for (HCInteger codeUnitIndex = 0; codeUnitIndex < strlen(complex); codeUnitIndex++) {
        ASSERT_EQUAL(HCStringGetCodeUnit(complexString, codeUnitIndex), (HCStringCodeUnit)complex[codeUnitIndex]);
    }
    HCRelease(complexString);
}

CTEST(HCString, CodePoints) {
    HCStringRef simpleString = HCStringCreateWithCString("ABCD");
    ASSERT_EQUAL(HCStringGetCodePointCount(simpleString), 4);
    ASSERT_EQUAL(HCStringGetCodePoint(simpleString, 0), 'A');
    ASSERT_EQUAL(HCStringGetCodePoint(simpleString, 1), 'B');
    ASSERT_EQUAL(HCStringGetCodePoint(simpleString, 2), 'C');
    ASSERT_EQUAL(HCStringGetCodePoint(simpleString, 3), 'D');
    HCRelease(simpleString);
    
    HCStringRef complexString = HCStringCreateWithCString("A⊭B⊭C⊭D");
    ASSERT_EQUAL(HCStringGetCodePointCount(complexString), 7);
    ASSERT_EQUAL(HCStringGetCodePoint(complexString, 0), 'A');
    ASSERT_EQUAL(HCStringGetCodePoint(complexString, 1), 0x22AD); // ⊭
    ASSERT_EQUAL(HCStringGetCodePoint(complexString, 2), 'B');
    ASSERT_EQUAL(HCStringGetCodePoint(complexString, 3), 0x22AD); // ⊭
    ASSERT_EQUAL(HCStringGetCodePoint(complexString, 4), 'C');
    ASSERT_EQUAL(HCStringGetCodePoint(complexString, 5), 0x22AD); // ⊭
    ASSERT_EQUAL(HCStringGetCodePoint(complexString, 6), 'D');
    HCRelease(complexString);
}
