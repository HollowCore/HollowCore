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
    ASSERT_EQUAL(HCDataSize(data), sizeof(bytes));
    for (HCInteger byteIndex = 0; byteIndex < (HCInteger)(sizeof(bytes) * sizeof(HCByte)); byteIndex++) {
        ASSERT_EQUAL(HCDataBytes(data)[byteIndex], bytes[byteIndex]);
    }
    HCRelease(data);
}

CTEST(HCData, Boolean) {
    HCDataRef falseData = HCDataCreateWithBoolean(false);
    ASSERT_TRUE(HCDataIsBoolean(falseData));
    ASSERT_FALSE(HCDataAsBoolean(falseData));
    HCRelease(falseData);
    
    HCDataRef trueData = HCDataCreateWithBoolean(true);
    ASSERT_TRUE(HCDataIsBoolean(trueData));
    ASSERT_TRUE(HCDataAsBoolean(trueData));
    HCRelease(trueData);
}

CTEST(HCData, Integer) {
    HCDataRef zeroData = HCDataCreateWithInteger(0);
    ASSERT_TRUE(HCDataIsInteger(zeroData));
    ASSERT_EQUAL(HCDataAsInteger(zeroData), 0);
    HCRelease(zeroData);
    
    HCDataRef oneData = HCDataCreateWithInteger(1);
    ASSERT_TRUE(HCDataIsInteger(oneData));
    ASSERT_EQUAL(HCDataAsInteger(oneData), 1);
    HCRelease(oneData);
    
    HCDataRef negativeData = HCDataCreateWithInteger(-1);
    ASSERT_TRUE(HCDataIsInteger(negativeData));
    ASSERT_EQUAL(HCDataAsInteger(negativeData), -1);
    HCRelease(negativeData);
    
    HCDataRef smallData = HCDataCreateWithInteger(0xFF);
    ASSERT_TRUE(HCDataIsInteger(smallData));
    ASSERT_EQUAL(HCDataAsInteger(smallData), 0xFF);
    HCRelease(smallData);
    
    HCDataRef bigData = HCDataCreateWithInteger(0x6FFFFFFFFFFFFFFF);
    ASSERT_TRUE(HCDataIsInteger(bigData));
    ASSERT_EQUAL(HCDataAsInteger(bigData), 0x6FFFFFFFFFFFFFFF);
    HCRelease(bigData);
}

CTEST(HCData, Real) {
    HCDataRef zeroData = HCDataCreateWithReal(0.0);
    ASSERT_TRUE(HCDataIsReal(zeroData));
    ASSERT_DBL_NEAR(HCDataAsReal(zeroData), 0.0);
    HCRelease(zeroData);
    
    HCDataRef oneData = HCDataCreateWithReal(1.0);
    ASSERT_TRUE(HCDataIsReal(oneData));
    ASSERT_DBL_NEAR(HCDataAsReal(oneData), 1.0);
    HCRelease(oneData);
    
    HCDataRef negativeData = HCDataCreateWithReal(-1.0);
    ASSERT_TRUE(HCDataIsReal(negativeData));
    ASSERT_DBL_NEAR(HCDataAsReal(negativeData), -1.0);
    HCRelease(negativeData);
    
    HCDataRef smallData = HCDataCreateWithReal(100.0);
    ASSERT_TRUE(HCDataIsReal(smallData));
    ASSERT_DBL_NEAR(HCDataAsReal(smallData), 100.0);
    HCRelease(smallData);
    
    HCDataRef bigData = HCDataCreateWithReal(3.14159e18);
    ASSERT_TRUE(HCDataIsReal(bigData));
    ASSERT_DBL_NEAR(HCDataAsReal(bigData), (HCReal)3.14159e18);
    HCRelease(bigData);
    
    HCDataRef piData = HCDataCreateWithReal(M_PI);
    ASSERT_TRUE(HCDataIsReal(piData));
    ASSERT_DBL_NEAR(HCDataAsReal(piData), M_PI);
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

CTEST(HCData, Print) {
    HCDataRef a = HCDataCreateWithInteger(0xBADF00D);
    HCDataPrint(a, stdout); // TODO: Not to stdout
    HCPrint(a, stdout); // TODO: Not to stdout
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
    ASSERT_EQUAL(HCDataSize(data), sizeof(bytes));
    ASSERT_EQUAL(HCDataBytes(data)[0], bytes[0]);
    ASSERT_EQUAL(HCDataBytes(data)[1], bytes[1]);
    ASSERT_EQUAL(HCDataBytes(data)[2], bytes[2]);
    HCDataAddBytes(data, sizeof(bytes), bytes);
    ASSERT_EQUAL(HCDataSize(data), sizeof(bytes) * 2);
    ASSERT_EQUAL(HCDataBytes(data)[0], bytes[0]);
    ASSERT_EQUAL(HCDataBytes(data)[1], bytes[1]);
    ASSERT_EQUAL(HCDataBytes(data)[2], bytes[2]);
    ASSERT_EQUAL(HCDataBytes(data)[3], bytes[0]);
    ASSERT_EQUAL(HCDataBytes(data)[4], bytes[1]);
    ASSERT_EQUAL(HCDataBytes(data)[5], bytes[2]);
    HCRelease(data);
}

CTEST(HCData, RemoveBytes) {
    HCByte bytes[] = { 0x05, 0x04, 0x03 };
    HCDataRef data = HCDataCreateWithBytes(sizeof(bytes), bytes);
    ASSERT_EQUAL(HCDataSize(data), sizeof(bytes));
    ASSERT_EQUAL(HCDataBytes(data)[0], bytes[0]);
    ASSERT_EQUAL(HCDataBytes(data)[1], bytes[1]);
    ASSERT_EQUAL(HCDataBytes(data)[2], bytes[2]);
    HCDataRemoveBytes(data, sizeof(HCByte) * 2);
    ASSERT_EQUAL(HCDataSize(data), sizeof(bytes) - sizeof(HCByte) * 2);
    ASSERT_EQUAL(HCDataBytes(data)[0], bytes[0]);
    HCRelease(data);
}

CTEST(HCData, AddBoolean) {
    HCDataRef data = HCDataCreateWithBoolean(true);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCBoolean) * 1);
    ASSERT_TRUE(((HCBoolean*)HCDataBytes(data))[0]);
    HCDataAddBoolean(data, false);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCBoolean) * 2);
    ASSERT_TRUE(((HCBoolean*)HCDataBytes(data))[0]);
    ASSERT_FALSE(((HCBoolean*)HCDataBytes(data))[1]);
    HCDataAddBoolean(data, true);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCBoolean) * 3);
    ASSERT_TRUE(((HCBoolean*)HCDataBytes(data))[0]);
    ASSERT_FALSE(((HCBoolean*)HCDataBytes(data))[1]);
    ASSERT_TRUE(((HCBoolean*)HCDataBytes(data))[2]);
    HCRelease(data);
}

CTEST(HCData, RemoveBoolean) {
    HCDataRef data = HCDataCreateWithBoolean(true);
    HCDataAddBoolean(data, false);
    HCDataAddBoolean(data, true);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCBoolean) * 3);
    ASSERT_TRUE(((HCBoolean*)HCDataBytes(data))[0]);
    ASSERT_FALSE(((HCBoolean*)HCDataBytes(data))[1]);
    ASSERT_TRUE(((HCBoolean*)HCDataBytes(data))[2]);
    HCDataRemoveBoolean(data);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCBoolean) * 2);
    ASSERT_TRUE(((HCBoolean*)HCDataBytes(data))[0]);
    ASSERT_FALSE(((HCBoolean*)HCDataBytes(data))[1]);
    HCDataRemoveBoolean(data);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCBoolean) * 1);
    ASSERT_TRUE(((HCBoolean*)HCDataBytes(data))[0]);
    HCDataRemoveBoolean(data);
    ASSERT_EQUAL(HCDataSize(data), 0);
    HCRelease(data);
}

CTEST(HCData, AddInteger) {
    HCDataRef data = HCDataCreateWithInteger(5);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCInteger) * 1);
    ASSERT_EQUAL(((HCInteger*)HCDataBytes(data))[0], 5);
    HCDataAddInteger(data, 4);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCInteger) * 2);
    ASSERT_EQUAL(((HCInteger*)HCDataBytes(data))[0], 5);
    ASSERT_EQUAL(((HCInteger*)HCDataBytes(data))[1], 4);
    HCDataAddInteger(data, 3);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCInteger) * 3);
    ASSERT_EQUAL(((HCInteger*)HCDataBytes(data))[0], 5);
    ASSERT_EQUAL(((HCInteger*)HCDataBytes(data))[1], 4);
    ASSERT_EQUAL(((HCInteger*)HCDataBytes(data))[2], 3);
    HCRelease(data);
}

CTEST(HCData, RemoveInteger) {
    HCDataRef data = HCDataCreateWithInteger(5);
    HCDataAddInteger(data, 4);
    HCDataAddInteger(data, 3);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCInteger) * 3);
    ASSERT_EQUAL(((HCInteger*)HCDataBytes(data))[0], 5);
    ASSERT_EQUAL(((HCInteger*)HCDataBytes(data))[1], 4);
    ASSERT_EQUAL(((HCInteger*)HCDataBytes(data))[2], 3);
    HCDataRemoveInteger(data);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCInteger) * 2);
    ASSERT_EQUAL(((HCInteger*)HCDataBytes(data))[0], 5);
    ASSERT_EQUAL(((HCInteger*)HCDataBytes(data))[1], 4);
    HCDataRemoveInteger(data);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCInteger) * 1);
    ASSERT_EQUAL(((HCInteger*)HCDataBytes(data))[0], 5);
    HCDataRemoveInteger(data);
    ASSERT_EQUAL(HCDataSize(data), 0);
    HCRelease(data);
}

CTEST(HCData, AddReal) {
    HCDataRef data = HCDataCreateWithReal(5.0);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCReal) * 1);
    ASSERT_EQUAL(((HCReal*)HCDataBytes(data))[0], 5.0);
    HCDataAddReal(data, 4.0);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCReal) * 2);
    ASSERT_EQUAL(((HCReal*)HCDataBytes(data))[0], 5.0);
    ASSERT_EQUAL(((HCReal*)HCDataBytes(data))[1], 4.0);
    HCDataAddReal(data, 3.0);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCReal) * 3);
    ASSERT_EQUAL(((HCReal*)HCDataBytes(data))[0], 5.0);
    ASSERT_EQUAL(((HCReal*)HCDataBytes(data))[1], 4.0);
    ASSERT_EQUAL(((HCReal*)HCDataBytes(data))[2], 3.0);
    HCRelease(data);
}

CTEST(HCData, RemoveReal) {
    HCDataRef data = HCDataCreateWithReal(5.0);
    HCDataAddReal(data, 4.0);
    HCDataAddReal(data, 3.0);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCReal) * 3);
    ASSERT_EQUAL(((HCReal*)HCDataBytes(data))[0], 5.0);
    ASSERT_EQUAL(((HCReal*)HCDataBytes(data))[1], 4.0);
    ASSERT_EQUAL(((HCReal*)HCDataBytes(data))[2], 3.0);
    HCDataRemoveReal(data);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCReal) * 2);
    ASSERT_EQUAL(((HCReal*)HCDataBytes(data))[0], 5.0);
    ASSERT_EQUAL(((HCReal*)HCDataBytes(data))[1], 4.0);
    HCDataRemoveReal(data);
    ASSERT_EQUAL(HCDataSize(data), sizeof(HCReal) * 1);
    ASSERT_EQUAL(((HCReal*)HCDataBytes(data))[0], 5.0);
    HCDataRemoveReal(data);
    ASSERT_EQUAL(HCDataSize(data), 0);
    HCRelease(data);
}
