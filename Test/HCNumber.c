///
/// @file HCNumber.c
/// @package HollowCore
///
/// @author Matt Stoker
/// @date 1/14/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <math.h>

CTEST(HCNumber, Creation) {
    HCNumberRef number = HCNumberCreate();
    ASSERT_FALSE(HCNumberAsBoolean(number));
    HCRelease(number);
}

CTEST(HCNumber, Boolean) {
    HCNumberRef falseNumber = HCNumberCreateWithBoolean(false);
    ASSERT_TRUE(HCNumberIsBoolean(falseNumber));
    ASSERT_FALSE(HCNumberIsInteger(falseNumber));
    ASSERT_FALSE(HCNumberIsReal(falseNumber));
    ASSERT_FALSE(HCNumberAsBoolean(falseNumber));
    ASSERT_EQUAL(HCNumberAsInteger(falseNumber), 0);
    ASSERT_EQUAL(HCNumberAsReal(falseNumber), 0.0);
    HCRelease(falseNumber);
    
    HCNumberRef trueNumber = HCNumberCreateWithBoolean(true);
    ASSERT_TRUE(HCNumberAsBoolean(trueNumber));
    ASSERT_EQUAL(HCNumberAsInteger(trueNumber), 1);
    ASSERT_EQUAL(HCNumberAsReal(trueNumber), 1.0);
    HCRelease(trueNumber);
}

CTEST(HCNumber, Integer) {
    HCNumberRef zeroNumber = HCNumberCreateWithInteger(0);
    ASSERT_FALSE(HCNumberIsBoolean(zeroNumber));
    ASSERT_TRUE(HCNumberIsInteger(zeroNumber));
    ASSERT_FALSE(HCNumberIsReal(zeroNumber));
    ASSERT_FALSE(HCNumberAsBoolean(zeroNumber));
    ASSERT_EQUAL(HCNumberAsInteger(zeroNumber), 0);
    ASSERT_EQUAL(HCNumberAsReal(zeroNumber), 0.0);
    HCRelease(zeroNumber);
    
    HCNumberRef oneNumber = HCNumberCreateWithInteger(1);
    ASSERT_TRUE(HCNumberAsBoolean(oneNumber));
    ASSERT_EQUAL(HCNumberAsInteger(oneNumber), 1);
    ASSERT_EQUAL(HCNumberAsReal(oneNumber), 1.0);
    HCRelease(oneNumber);
    
    HCNumberRef negativeNumber = HCNumberCreateWithInteger(-1);
    ASSERT_TRUE(HCNumberAsBoolean(negativeNumber));
    ASSERT_EQUAL(HCNumberAsInteger(negativeNumber), -1);
    ASSERT_EQUAL(HCNumberAsReal(negativeNumber), -1.0);
    HCRelease(negativeNumber);
    
    HCNumberRef smallNumber = HCNumberCreateWithInteger(0xFF);
    ASSERT_TRUE(HCNumberAsBoolean(smallNumber));
    ASSERT_EQUAL(HCNumberAsInteger(smallNumber), 0xFF);
    ASSERT_EQUAL(HCNumberAsReal(smallNumber), (HCReal)0xFF);
    HCRelease(smallNumber);
    
    HCNumberRef bigNumber = HCNumberCreateWithInteger(0x6FFFFFFFFFFFFFFF);
    ASSERT_TRUE(HCNumberAsBoolean(bigNumber));
    ASSERT_EQUAL(HCNumberAsInteger(bigNumber), 0x6FFFFFFFFFFFFFFF);
    ASSERT_EQUAL(HCNumberAsReal(bigNumber), (HCReal)0x6FFFFFFFFFFFFFFF);
    HCRelease(bigNumber);
}

CTEST(HCNumber, Real) {
    HCNumberRef zeroNumber = HCNumberCreateWithReal(0.0);
    ASSERT_FALSE(HCNumberIsBoolean(zeroNumber));
    ASSERT_FALSE(HCNumberIsInteger(zeroNumber));
    ASSERT_TRUE(HCNumberIsReal(zeroNumber));
    ASSERT_FALSE(HCNumberAsBoolean(zeroNumber));
    ASSERT_EQUAL(HCNumberAsInteger(zeroNumber), 0);
    ASSERT_EQUAL(HCNumberAsReal(zeroNumber), 0.0);
    HCRelease(zeroNumber);
    
    HCNumberRef oneNumber = HCNumberCreateWithReal(1.0);
    ASSERT_TRUE(HCNumberAsBoolean(oneNumber));
    ASSERT_EQUAL(HCNumberAsInteger(oneNumber), 1);
    ASSERT_EQUAL(HCNumberAsReal(oneNumber), 1.0);
    HCRelease(oneNumber);
    
    HCNumberRef negativeNumber = HCNumberCreateWithReal(-1.0);
    ASSERT_TRUE(HCNumberAsBoolean(negativeNumber));
    ASSERT_EQUAL(HCNumberAsInteger(negativeNumber), -1);
    ASSERT_EQUAL(HCNumberAsReal(negativeNumber), -1.0);
    HCRelease(negativeNumber);
    
    HCNumberRef smallNumber = HCNumberCreateWithReal(100.0);
    ASSERT_TRUE(HCNumberAsBoolean(smallNumber));
    ASSERT_EQUAL(HCNumberAsInteger(smallNumber), 100);
    ASSERT_EQUAL(HCNumberAsReal(smallNumber), 100.0);
    HCRelease(smallNumber);
    
    HCNumberRef bigNumber = HCNumberCreateWithReal(3.14159e18);
    ASSERT_TRUE(HCNumberAsBoolean(bigNumber));
    ASSERT_EQUAL(HCNumberAsInteger(bigNumber), 3141590000000000000);
    ASSERT_EQUAL(HCNumberAsReal(bigNumber), (HCReal)3.14159e18);
    HCRelease(bigNumber);
    
    HCNumberRef piNumber = HCNumberCreateWithReal(M_PI);
    ASSERT_TRUE(HCNumberAsBoolean(piNumber));
    ASSERT_EQUAL(HCNumberAsInteger(piNumber), 3);
    ASSERT_DBL_NEAR(HCNumberAsReal(piNumber), M_PI);
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

CTEST(HCNumber, Print) {
    HCNumberRef a = HCNumberCreateWithReal(M_PI);
    HCNumberPrint(a, stdout); // TODO: Not to stdout
    HCPrint(a, stdout); // TODO: Not to stdout
    HCRelease(a);
}
