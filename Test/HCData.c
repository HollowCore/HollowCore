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
    HCDataRef empty = HCDataCreate();
    ASSERT_TRUE(HCDataIsEmpty(empty));
    HCRelease(empty);
}

CTEST(HCData, Bytes) {
    HCByte bytes[] = { 5, 4, 3, 2, 1 };
    HCDataRef data = HCDataCreateWithBytes(sizeof(bytes), bytes);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(bytes));
    for (HCInteger byteIndex = 0; byteIndex < sizeof(bytes) * sizeof(HCByte); byteIndex++) {
        ASSERT_EQUAL(HCDataGetBytes(data)[byteIndex], bytes[byteIndex]);
    }
    HCRelease(data);
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

CTEST(HCData, Clear) {
    HCDataRef data = HCDataCreateWithBoolean(true);
    ASSERT_FALSE(HCDataIsEmpty(data));
    HCDataClear(data);
    ASSERT_TRUE(HCDataIsEmpty(data));
    HCRelease(data);
}

CTEST(HCData, AddBytes) {
    HCByte bytes[] = { 0x05, 0x04, 0x03 };
    HCDataRef data = HCDataCreateWithBytes(sizeof(bytes), bytes);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(bytes));
    ASSERT_EQUAL(HCDataGetBytes(data)[0], bytes[0]);
    ASSERT_EQUAL(HCDataGetBytes(data)[1], bytes[1]);
    ASSERT_EQUAL(HCDataGetBytes(data)[2], bytes[2]);
    HCDataAddBytes(data, sizeof(bytes), bytes);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(bytes) * 2);
    ASSERT_EQUAL(HCDataGetBytes(data)[0], bytes[0]);
    ASSERT_EQUAL(HCDataGetBytes(data)[1], bytes[1]);
    ASSERT_EQUAL(HCDataGetBytes(data)[2], bytes[2]);
    ASSERT_EQUAL(HCDataGetBytes(data)[3], bytes[0]);
    ASSERT_EQUAL(HCDataGetBytes(data)[4], bytes[1]);
    ASSERT_EQUAL(HCDataGetBytes(data)[5], bytes[2]);
    HCRelease(data);
}

CTEST(HCData, RemoveBytes) {
    HCByte bytes[] = { 0x05, 0x04, 0x03 };
    HCDataRef data = HCDataCreateWithBytes(sizeof(bytes), bytes);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(bytes));
    ASSERT_EQUAL(HCDataGetBytes(data)[0], bytes[0]);
    ASSERT_EQUAL(HCDataGetBytes(data)[1], bytes[1]);
    ASSERT_EQUAL(HCDataGetBytes(data)[2], bytes[2]);
    HCDataRemoveBytes(data, sizeof(HCByte) * 2);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(bytes) - sizeof(HCByte) * 2);
    ASSERT_EQUAL(HCDataGetBytes(data)[0], bytes[0]);
    HCRelease(data);
}

CTEST(HCData, AddBoolean) {
    HCDataRef data = HCDataCreateWithBoolean(true);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCBoolean) * 1);
    ASSERT_TRUE(((HCBoolean*)HCDataGetBytes(data))[0]);
    HCDataAddBoolean(data, false);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCBoolean) * 2);
    ASSERT_TRUE(((HCBoolean*)HCDataGetBytes(data))[0]);
    ASSERT_FALSE(((HCBoolean*)HCDataGetBytes(data))[1]);
    HCDataAddBoolean(data, true);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCBoolean) * 3);
    ASSERT_TRUE(((HCBoolean*)HCDataGetBytes(data))[0]);
    ASSERT_FALSE(((HCBoolean*)HCDataGetBytes(data))[1]);
    ASSERT_TRUE(((HCBoolean*)HCDataGetBytes(data))[2]);
    HCRelease(data);
}

CTEST(HCData, RemoveBoolean) {
    HCDataRef data = HCDataCreateWithBoolean(true);
    HCDataAddBoolean(data, false);
    HCDataAddBoolean(data, true);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCBoolean) * 3);
    ASSERT_TRUE(((HCBoolean*)HCDataGetBytes(data))[0]);
    ASSERT_FALSE(((HCBoolean*)HCDataGetBytes(data))[1]);
    ASSERT_TRUE(((HCBoolean*)HCDataGetBytes(data))[2]);
    HCDataRemoveBoolean(data);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCBoolean) * 2);
    ASSERT_TRUE(((HCBoolean*)HCDataGetBytes(data))[0]);
    ASSERT_FALSE(((HCBoolean*)HCDataGetBytes(data))[1]);
    HCDataRemoveBoolean(data);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCBoolean) * 1);
    ASSERT_TRUE(((HCBoolean*)HCDataGetBytes(data))[0]);
    HCDataRemoveBoolean(data);
    ASSERT_EQUAL(HCDataGetSize(data), 0);
    HCRelease(data);
}

CTEST(HCData, AddInteger) {
    HCDataRef data = HCDataCreateWithInteger(5);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCInteger) * 1);
    ASSERT_EQUAL(((HCInteger*)HCDataGetBytes(data))[0], 5);
    HCDataAddInteger(data, 4);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCInteger) * 2);
    ASSERT_EQUAL(((HCInteger*)HCDataGetBytes(data))[0], 5);
    ASSERT_EQUAL(((HCInteger*)HCDataGetBytes(data))[1], 4);
    HCDataAddInteger(data, 3);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCInteger) * 3);
    ASSERT_EQUAL(((HCInteger*)HCDataGetBytes(data))[0], 5);
    ASSERT_EQUAL(((HCInteger*)HCDataGetBytes(data))[1], 4);
    ASSERT_EQUAL(((HCInteger*)HCDataGetBytes(data))[2], 3);
    HCRelease(data);
}

CTEST(HCData, RemoveInteger) {
    HCDataRef data = HCDataCreateWithInteger(5);
    HCDataAddInteger(data, 4);
    HCDataAddInteger(data, 3);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCInteger) * 3);
    ASSERT_EQUAL(((HCInteger*)HCDataGetBytes(data))[0], 5);
    ASSERT_EQUAL(((HCInteger*)HCDataGetBytes(data))[1], 4);
    ASSERT_EQUAL(((HCInteger*)HCDataGetBytes(data))[2], 3);
    HCDataRemoveInteger(data);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCInteger) * 2);
    ASSERT_EQUAL(((HCInteger*)HCDataGetBytes(data))[0], 5);
    ASSERT_EQUAL(((HCInteger*)HCDataGetBytes(data))[1], 4);
    HCDataRemoveInteger(data);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCInteger) * 1);
    ASSERT_EQUAL(((HCInteger*)HCDataGetBytes(data))[0], 5);
    HCDataRemoveInteger(data);
    ASSERT_EQUAL(HCDataGetSize(data), 0);
    HCRelease(data);
}

CTEST(HCData, AddReal) {
    HCDataRef data = HCDataCreateWithReal(5.0);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCReal) * 1);
    ASSERT_EQUAL(((HCReal*)HCDataGetBytes(data))[0], 5.0);
    HCDataAddReal(data, 4.0);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCReal) * 2);
    ASSERT_EQUAL(((HCReal*)HCDataGetBytes(data))[0], 5.0);
    ASSERT_EQUAL(((HCReal*)HCDataGetBytes(data))[1], 4.0);
    HCDataAddReal(data, 3.0);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCReal) * 3);
    ASSERT_EQUAL(((HCReal*)HCDataGetBytes(data))[0], 5.0);
    ASSERT_EQUAL(((HCReal*)HCDataGetBytes(data))[1], 4.0);
    ASSERT_EQUAL(((HCReal*)HCDataGetBytes(data))[2], 3.0);
    HCRelease(data);
}

CTEST(HCData, RemoveReal) {
    HCDataRef data = HCDataCreateWithReal(5.0);
    HCDataAddReal(data, 4.0);
    HCDataAddReal(data, 3.0);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCReal) * 3);
    ASSERT_EQUAL(((HCReal*)HCDataGetBytes(data))[0], 5.0);
    ASSERT_EQUAL(((HCReal*)HCDataGetBytes(data))[1], 4.0);
    ASSERT_EQUAL(((HCReal*)HCDataGetBytes(data))[2], 3.0);
    HCDataRemoveReal(data);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCReal) * 2);
    ASSERT_EQUAL(((HCReal*)HCDataGetBytes(data))[0], 5.0);
    ASSERT_EQUAL(((HCReal*)HCDataGetBytes(data))[1], 4.0);
    HCDataRemoveReal(data);
    ASSERT_EQUAL(HCDataGetSize(data), sizeof(HCReal) * 1);
    ASSERT_EQUAL(((HCReal*)HCDataGetBytes(data))[0], 5.0);
    HCDataRemoveReal(data);
    ASSERT_EQUAL(HCDataGetSize(data), 0);
    HCRelease(data);
}
