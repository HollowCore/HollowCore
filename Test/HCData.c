//
//  HCData.c
//  Test
//
//  Created by Matt Stoker on 1/19/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <math.h>

CTEST(HCData, Creation) {
    HCDataRef number = HCDataCreate();
    ASSERT_FALSE(HCDataAsBoolean(number));
    HCRelease(number);
}

CTEST(HCData, Boolean) {
    HCDataRef falseData = HCDataCreateWithBoolean(false);
    ASSERT_FALSE(HCDataAsBoolean(falseData));
    HCRelease(falseData);
    
    HCDataRef trueData = HCDataCreateWithBoolean(true);
    ASSERT_TRUE(HCDataAsBoolean(trueData));
    HCRelease(trueData);
}

CTEST(HCData, Integer) {
    HCDataRef zeroData = HCDataCreateWithInteger(0);
    ASSERT_EQUAL(HCDataAsInteger(zeroData), 0);
    HCRelease(zeroData);
    
    HCDataRef oneData = HCDataCreateWithInteger(1);
    ASSERT_EQUAL(HCDataAsInteger(oneData), 1);
    HCRelease(oneData);
    
    HCDataRef negativeData = HCDataCreateWithInteger(-1);
    ASSERT_EQUAL(HCDataAsInteger(negativeData), -1);
    HCRelease(negativeData);
    
    HCDataRef smallData = HCDataCreateWithInteger(0xFF);
    ASSERT_EQUAL(HCDataAsInteger(smallData), 0xFF);
    HCRelease(smallData);
    
    HCDataRef bigData = HCDataCreateWithInteger(0x6FFFFFFFFFFFFFFF);
    ASSERT_EQUAL(HCDataAsInteger(bigData), 0x6FFFFFFFFFFFFFFF);
    HCRelease(bigData);
}

CTEST(HCData, Real) {
    HCDataRef zeroData = HCDataCreateWithReal(0.0);
    ASSERT_EQUAL(HCDataAsReal(zeroData), 0.0);
    HCRelease(zeroData);
    
    HCDataRef oneData = HCDataCreateWithReal(1.0);
    ASSERT_EQUAL(HCDataAsReal(oneData), 1.0);
    HCRelease(oneData);
    
    HCDataRef negativeData = HCDataCreateWithReal(-1.0);
    ASSERT_EQUAL(HCDataAsReal(negativeData), -1.0);
    HCRelease(negativeData);
    
    HCDataRef smallData = HCDataCreateWithReal(100.0);
    ASSERT_EQUAL(HCDataAsReal(smallData), 100.0);
    HCRelease(smallData);
    
    HCDataRef bigData = HCDataCreateWithReal(3.14159e18);
    ASSERT_EQUAL(HCDataAsReal(bigData), (HCReal)3.14159e18);
    HCRelease(bigData);
    
    HCDataRef piData = HCDataCreateWithReal(M_PI);
    ASSERT_EQUAL(HCDataAsReal(piData), M_PI);
    HCRelease(piData);
}

CTEST(HCData, EqualHash) {
    HCDataRef a = HCDataCreateWithInteger(0xBADF00D);
    HCDataRef b = HCDataCreateWithInteger(0xBADF00D);
    HCDataRef c = HCDataCreateWithInteger(0xDEADBEEF);
    ASSERT_TRUE(HCDataIsEqual(a, a));
    ASSERT_TRUE(HCDataIsEqual(b, b));
    ASSERT_TRUE(HCDataIsEqual(c, c));
    ASSERT_TRUE(HCDataIsEqual(a, b));
    ASSERT_TRUE(HCDataIsEqual(b, a));
    ASSERT_FALSE(HCDataIsEqual(a, c));
    ASSERT_FALSE(HCDataIsEqual(c, a));
    ASSERT_EQUAL(HCDataHashValue(a), HCDataHashValue(a));
    ASSERT_EQUAL(HCDataHashValue(a), HCDataHashValue(b));
    HCRelease(a);
    HCRelease(b);
    HCRelease(c);
}
