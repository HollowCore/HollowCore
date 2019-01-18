//
//  HCNumber.c
//  HollowCore
//
//  Created by Matt Stoker on 1/14/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <math.h>

CTEST(HCNumber, Creation) {
    HCNumberRef number = HCNumberCreate();
    ASSERT_FALSE(HCNumberGetBoolean(number));
    HCRelease(number);
}

CTEST(HCNumber, Boolean) {
    HCNumberRef falseNumber = HCNumberCreateWithBoolean(false);
    ASSERT_FALSE(HCNumberGetBoolean(falseNumber));
    ASSERT_EQUAL(HCNumberGetInteger(falseNumber), 0);
    ASSERT_EQUAL(HCNumberGetReal(falseNumber), 0.0);
    HCRelease(falseNumber);
    
    HCNumberRef trueNumber = HCNumberCreateWithBoolean(true);
    ASSERT_TRUE(HCNumberGetBoolean(trueNumber));
    ASSERT_EQUAL(HCNumberGetInteger(trueNumber), 1);
    ASSERT_EQUAL(HCNumberGetReal(trueNumber), 1.0);
    HCRelease(trueNumber);
}

CTEST(HCNumber, Integer) {
    HCNumberRef zeroNumber = HCNumberCreateWithInteger(0);
    ASSERT_FALSE(HCNumberGetBoolean(zeroNumber));
    ASSERT_EQUAL(HCNumberGetInteger(zeroNumber), 0);
    ASSERT_EQUAL(HCNumberGetReal(zeroNumber), 0.0);
    HCRelease(zeroNumber);
    
    HCNumberRef oneNumber = HCNumberCreateWithInteger(1);
    ASSERT_TRUE(HCNumberGetBoolean(oneNumber));
    ASSERT_EQUAL(HCNumberGetInteger(oneNumber), 1);
    ASSERT_EQUAL(HCNumberGetReal(oneNumber), 1.0);
    HCRelease(oneNumber);
    
    HCNumberRef negativeNumber = HCNumberCreateWithInteger(-1);
    ASSERT_TRUE(HCNumberGetBoolean(negativeNumber));
    ASSERT_EQUAL(HCNumberGetInteger(negativeNumber), -1);
    ASSERT_EQUAL(HCNumberGetReal(negativeNumber), -1.0);
    HCRelease(negativeNumber);
    
    HCNumberRef smallNumber = HCNumberCreateWithInteger(0xFF);
    ASSERT_TRUE(HCNumberGetBoolean(smallNumber));
    ASSERT_EQUAL(HCNumberGetInteger(smallNumber), 0xFF);
    ASSERT_EQUAL(HCNumberGetReal(smallNumber), (HCReal)0xFF);
    HCRelease(smallNumber);
    
    HCNumberRef bigNumber = HCNumberCreateWithInteger(0x6FFFFFFFFFFFFFFF);
    ASSERT_TRUE(HCNumberGetBoolean(bigNumber));
    ASSERT_EQUAL(HCNumberGetInteger(bigNumber), 0x6FFFFFFFFFFFFFFF);
    ASSERT_EQUAL(HCNumberGetReal(bigNumber), (HCReal)0x6FFFFFFFFFFFFFFF);
    HCRelease(bigNumber);
}

CTEST(HCNumber, Real) {
    HCNumberRef zeroNumber = HCNumberCreateWithReal(0.0);
    ASSERT_FALSE(HCNumberGetBoolean(zeroNumber));
    ASSERT_EQUAL(HCNumberGetInteger(zeroNumber), 0);
    ASSERT_EQUAL(HCNumberGetReal(zeroNumber), 0.0);
    HCRelease(zeroNumber);
    
    HCNumberRef oneNumber = HCNumberCreateWithReal(1.0);
    ASSERT_TRUE(HCNumberGetBoolean(oneNumber));
    ASSERT_EQUAL(HCNumberGetInteger(oneNumber), 1);
    ASSERT_EQUAL(HCNumberGetReal(oneNumber), 1.0);
    HCRelease(oneNumber);
    
    HCNumberRef negativeNumber = HCNumberCreateWithReal(-1.0);
    ASSERT_TRUE(HCNumberGetBoolean(negativeNumber));
    ASSERT_EQUAL(HCNumberGetInteger(negativeNumber), -1);
    ASSERT_EQUAL(HCNumberGetReal(negativeNumber), -1.0);
    HCRelease(negativeNumber);
    
    HCNumberRef smallNumber = HCNumberCreateWithReal(100.0);
    ASSERT_TRUE(HCNumberGetBoolean(smallNumber));
    ASSERT_EQUAL(HCNumberGetInteger(smallNumber), 100);
    ASSERT_EQUAL(HCNumberGetReal(smallNumber), 100.0);
    HCRelease(smallNumber);
    
    HCNumberRef bigNumber = HCNumberCreateWithReal(3.14159e18);
    ASSERT_TRUE(HCNumberGetBoolean(bigNumber));
    ASSERT_EQUAL(HCNumberGetInteger(bigNumber), 3141590000000000000);
    ASSERT_EQUAL(HCNumberGetReal(bigNumber), (HCReal)3.14159e18);
    HCRelease(bigNumber);
    
    HCNumberRef piNumber = HCNumberCreateWithReal(M_PI);
    ASSERT_TRUE(HCNumberGetBoolean(piNumber));
    ASSERT_EQUAL(HCNumberGetInteger(piNumber), 3);
    ASSERT_EQUAL(HCNumberGetReal(piNumber), M_PI);
    HCRelease(piNumber);
}

CTEST(HCNumber, EqualHash) {
    HCNumberRef a = HCNumberCreateWithInteger(0xBADF00D);
    HCNumberRef b = HCNumberCreateWithInteger(0xBADF00D);
    HCNumberRef c = HCNumberCreateWithInteger(0xDEADBEEF);
    ASSERT_TRUE(HCNumberIsEqual(a, a));
    ASSERT_TRUE(HCNumberIsEqual(b, b));
    ASSERT_TRUE(HCNumberIsEqual(c, c));
    ASSERT_TRUE(HCNumberIsEqual(a, b));
    ASSERT_TRUE(HCNumberIsEqual(b, a));
    ASSERT_FALSE(HCNumberIsEqual(a, c));
    ASSERT_FALSE(HCNumberIsEqual(c, a));
    ASSERT_EQUAL(HCNumberHashValue(a), HCNumberHashValue(a));
    ASSERT_EQUAL(HCNumberHashValue(a), HCNumberHashValue(b));
    HCRelease(a);
    HCRelease(b);
    HCRelease(c);
    
    HCNumberRef falseNumber = HCNumberCreateWithBoolean(false);
    HCNumberRef zeroNumber = HCNumberCreateWithInteger(0);
    HCNumberRef zeroRealNumber = HCNumberCreateWithReal(0.0);
    ASSERT_TRUE(HCNumberIsEqual(falseNumber, falseNumber));
    ASSERT_TRUE(HCNumberIsEqual(zeroNumber, zeroNumber));
    ASSERT_TRUE(HCNumberIsEqual(zeroRealNumber, zeroRealNumber));
    ASSERT_TRUE(HCNumberIsEqual(falseNumber, zeroNumber));
    ASSERT_TRUE(HCNumberIsEqual(zeroNumber, falseNumber));
    ASSERT_TRUE(HCNumberIsEqual(falseNumber, zeroRealNumber));
    ASSERT_TRUE(HCNumberIsEqual(zeroRealNumber, falseNumber));
    ASSERT_TRUE(HCNumberIsEqual(zeroNumber, zeroRealNumber));
    ASSERT_TRUE(HCNumberIsEqual(zeroRealNumber, zeroNumber));
    ASSERT_EQUAL(HCNumberHashValue(falseNumber), HCNumberHashValue(zeroNumber));
    ASSERT_EQUAL(HCNumberHashValue(falseNumber), HCNumberHashValue(zeroRealNumber));
    ASSERT_EQUAL(HCNumberHashValue(zeroNumber), HCNumberHashValue(zeroRealNumber));
    HCRelease(falseNumber);
    HCRelease(zeroNumber);
    HCRelease(zeroRealNumber);
    
    HCNumberRef trueNumber = HCNumberCreateWithBoolean(true);
    HCNumberRef oneNumber = HCNumberCreateWithInteger(1);
    HCNumberRef oneRealNumber = HCNumberCreateWithReal(1.0);
    ASSERT_TRUE(HCNumberIsEqual(trueNumber, trueNumber));
    ASSERT_TRUE(HCNumberIsEqual(oneNumber, oneNumber));
    ASSERT_TRUE(HCNumberIsEqual(oneRealNumber, oneRealNumber));
    ASSERT_TRUE(HCNumberIsEqual(trueNumber, oneNumber));
    ASSERT_TRUE(HCNumberIsEqual(oneNumber, trueNumber));
    ASSERT_TRUE(HCNumberIsEqual(trueNumber, oneRealNumber));
    ASSERT_TRUE(HCNumberIsEqual(oneRealNumber, trueNumber));
    ASSERT_TRUE(HCNumberIsEqual(oneNumber, oneRealNumber));
    ASSERT_TRUE(HCNumberIsEqual(oneRealNumber, oneNumber));
    ASSERT_EQUAL(HCNumberHashValue(trueNumber), HCNumberHashValue(oneNumber));
    ASSERT_EQUAL(HCNumberHashValue(trueNumber), HCNumberHashValue(oneRealNumber));
    ASSERT_EQUAL(HCNumberHashValue(oneNumber), HCNumberHashValue(oneRealNumber));
    HCRelease(trueNumber);
    HCRelease(oneNumber);
    HCRelease(oneRealNumber);
}
