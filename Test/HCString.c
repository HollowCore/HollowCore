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

CTEST(HCString, CString) {
    HCStringRef generalString = HCStringCreateWithCString("I am the very model of a modern Major General.");
    ASSERT_FALSE(HCStringIsEmpty(generalString));
    ASSERT_STR(HCStringAsCString(generalString), "I am the very model of a modern Major General.");
    HCRelease(generalString);
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

CTEST(HCString, EqualHash) {
    HCStringRef a = HCStringCreateWithInteger(0xBADF00D);
    HCStringRef b = HCStringCreateWithInteger(0xBADF00D);
    HCStringRef c = HCStringCreateWithCString("Good food");
    ASSERT_TRUE(HCStringIsEqual(a, a));
    ASSERT_TRUE(HCStringIsEqual(b, b));
    ASSERT_TRUE(HCStringIsEqual(c, c));
    ASSERT_TRUE(HCStringIsEqual(a, b));
    ASSERT_TRUE(HCStringIsEqual(b, a));
    ASSERT_FALSE(HCStringIsEqual(a, c));
    ASSERT_FALSE(HCStringIsEqual(c, a));
    ASSERT_EQUAL(HCStringHashValue(a), HCStringHashValue(a));
    ASSERT_EQUAL(HCStringHashValue(a), HCStringHashValue(b));
    HCRelease(a);
    HCRelease(b);
    HCRelease(c);
}

CTEST(HCString, Print) {
    HCStringRef string = HCStringCreateWithCString("Four score and seven years ago");
    HCStringPrint(string, stdout); // TODO: Not to stdout
    HCPrint(string, stdout); // TODO: Not to stdout
}

CTEST(HCString, CodeUnits) {
    const char* simple = "ABCD";
    HCStringRef simpleString = HCStringCreateWithCString(simple);
    ASSERT_EQUAL(HCStringCodeUnitCount(simpleString), 4);
    ASSERT_EQUAL(HCStringCodeUnitAtIndex(simpleString, 0), (HCStringCodeUnit)simple[0]);
    ASSERT_EQUAL(HCStringCodeUnitAtIndex(simpleString, 1), (HCStringCodeUnit)simple[1]);
    ASSERT_EQUAL(HCStringCodeUnitAtIndex(simpleString, 2), (HCStringCodeUnit)simple[2]);
    ASSERT_EQUAL(HCStringCodeUnitAtIndex(simpleString, 3), (HCStringCodeUnit)simple[3]);
    HCRelease(simpleString);
    
    const char* complex = "A⊭B⊨C⊭D";
    HCStringRef complexString = HCStringCreateWithCString(complex);
    ASSERT_EQUAL(HCStringCodeUnitCount(complexString), 13);
    for (HCInteger codeUnitIndex = 0; codeUnitIndex < (HCInteger)strlen(complex); codeUnitIndex++) {
        ASSERT_EQUAL(HCStringCodeUnitAtIndex(complexString, codeUnitIndex), (HCStringCodeUnit)complex[codeUnitIndex]);
    }
    HCRelease(complexString);
}

CTEST(HCString, CodePoints) {
    HCStringRef simpleString = HCStringCreateWithCString("ABCD");
    ASSERT_EQUAL(HCStringCodePointCount(simpleString), 4);
    ASSERT_EQUAL(HCStringCodePointAtIndex(simpleString, 0), 'A');
    ASSERT_EQUAL(HCStringCodePointAtIndex(simpleString, 1), 'B');
    ASSERT_EQUAL(HCStringCodePointAtIndex(simpleString, 2), 'C');
    ASSERT_EQUAL(HCStringCodePointAtIndex(simpleString, 3), 'D');
    HCRelease(simpleString);
    
    HCStringRef complexString = HCStringCreateWithCString("A⊭B⊨C⊭D");
    ASSERT_EQUAL(HCStringCodePointCount(complexString), 7);
    ASSERT_EQUAL(HCStringCodePointAtIndex(complexString, 0), 'A');
    ASSERT_EQUAL(HCStringCodePointAtIndex(complexString, 1), 0x22AD); // ⊭
    ASSERT_EQUAL(HCStringCodePointAtIndex(complexString, 2), 'B');
    ASSERT_EQUAL(HCStringCodePointAtIndex(complexString, 3), 0x22A8); // ⊨
    ASSERT_EQUAL(HCStringCodePointAtIndex(complexString, 4), 'C');
    ASSERT_EQUAL(HCStringCodePointAtIndex(complexString, 5), 0x22AD); // ⊭
    ASSERT_EQUAL(HCStringCodePointAtIndex(complexString, 6), 'D');
    HCRelease(complexString);
}

CTEST(HCString, ExtractCodeUnits) {
    HCStringRef simpleString = HCStringCreateWithCString("ABCD");
    HCStringCodeUnit extracted[2];
    HCStringExtractCodeUnits(simpleString, 1, 2, extracted);
    ASSERT_EQUAL(extracted[0], 'B');
    ASSERT_EQUAL(extracted[1], 'C');
    HCRelease(simpleString);
}

CTEST(HCString, ExtractCodePoints) {
    HCStringRef complexString = HCStringCreateWithCString("A⊭B⊨C⊭D");
    HCStringCodePoint extracted[2];
    HCStringExtractCodePoints(complexString, 2, 2, extracted);
    ASSERT_EQUAL(extracted[0], 'B');
    ASSERT_EQUAL(extracted[1], 0x22A8); // ⊨
    HCRelease(complexString);
}

CTEST(HCString, CStringConversion) {
    const char* string = "I am the very model of a modern Major General.";
    HCStringRef generalString = HCStringCreateWithCString(string);
    ASSERT_TRUE(HCStringIsCString(generalString));
    ASSERT_STR(HCStringAsCString(generalString), string);
    HCRelease(generalString);
    
    const char* brokenString = "I am the very model of a\0 modern Major General.";
    HCStringRef okString = HCStringCreateWithBytes(HCStringEncodingUTF8, strlen(string) + 1, (HCByte*)brokenString);
    ASSERT_FALSE(HCStringIsCString(okString));
    ASSERT_STR(HCStringAsCString(okString), brokenString);
    HCRelease(okString);
}

CTEST(HCString, BooleanConversion) {
    HCStringRef false0 = HCStringCreateWithCString("⊭");
    ASSERT_TRUE(HCStringIsBoolean(false0));
    ASSERT_FALSE(HCStringAsBoolean(false0));
    HCRelease(false0);
    
    HCStringRef false1 = HCStringCreateWithCString("0");
    ASSERT_TRUE(HCStringIsBoolean(false1));
    ASSERT_FALSE(HCStringAsBoolean(false1));
    HCRelease(false1);
    
    HCStringRef false2 = HCStringCreateWithCString("f");
    ASSERT_TRUE(HCStringIsBoolean(false2));
    ASSERT_FALSE(HCStringAsBoolean(false2));
    HCRelease(false2);
    
    HCStringRef false3 = HCStringCreateWithCString("false");
    ASSERT_TRUE(HCStringIsBoolean(false3));
    ASSERT_FALSE(HCStringAsBoolean(false3));
    HCRelease(false3);
    
    HCStringRef falseSentence = HCStringCreateWithCString("false but not exactly");
    ASSERT_FALSE(HCStringIsBoolean(falseSentence));
    ASSERT_FALSE(HCStringAsBoolean(falseSentence));
    HCRelease(falseSentence);
    
    HCStringRef true0 = HCStringCreateWithCString("⊨");
    ASSERT_TRUE(HCStringIsBoolean(true0));
    ASSERT_TRUE(HCStringAsBoolean(true0));
    HCRelease(true0);
    
    HCStringRef true1 = HCStringCreateWithCString("1");
    ASSERT_TRUE(HCStringIsBoolean(true1));
    ASSERT_TRUE(HCStringAsBoolean(true1));
    HCRelease(true1);
    
    HCStringRef true2 = HCStringCreateWithCString("t");
    ASSERT_TRUE(HCStringIsBoolean(true2));
    ASSERT_TRUE(HCStringAsBoolean(true2));
    HCRelease(true2);
    
    HCStringRef true3 = HCStringCreateWithCString("true");
    ASSERT_TRUE(HCStringIsBoolean(true3));
    ASSERT_TRUE(HCStringAsBoolean(true3));
    HCRelease(true3);
    
    HCStringRef trueSentence = HCStringCreateWithCString("true but not exactly");
    ASSERT_FALSE(HCStringIsBoolean(trueSentence));
    ASSERT_TRUE(HCStringAsBoolean(trueSentence));
    HCRelease(trueSentence);
}

CTEST(HCString, IntegerConversion) {
    for (HCInteger integer = -1000; integer < 1000; integer++) {
        HCStringRef integerString = HCStringCreateWithInteger(integer);
        ASSERT_TRUE(HCStringIsInteger(integerString));
        ASSERT_EQUAL(HCStringAsInteger(integerString), integer);
        HCRelease(integerString);
    }
    
    HCStringRef everything = HCStringCreateWithCString("42 is the answer to the ultimate question of life, the universe, and everyting");
    ASSERT_FALSE(HCStringIsInteger(everything));
    ASSERT_EQUAL(HCStringAsInteger(everything), 42);
    HCRelease(everything);
}

CTEST(HCString, RealConversion) {
    for (HCReal real = -10.0; real < 10.0; real += 0.01) {
        HCStringRef realString = HCStringCreateWithReal(real);
        ASSERT_TRUE(HCStringIsReal(realString));
        ASSERT_EQUAL(HCStringAsReal(realString), real);
        HCRelease(realString);
    }
    
    HCStringRef pie = HCStringCreateWithCString("3.14159 is just about as good as pi");
    ASSERT_FALSE(HCStringIsReal(pie));
    ASSERT_DBL_NEAR(HCStringAsReal(pie), 3.14159);
    HCRelease(pie);
}
