//
//  HCObject.c
//  HollowCore
//
//  Created by Matt Stoker on 1/17/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"

CTEST(HCObject, Creation) {
    HCNumberRef number = HCNumberCreate();
    ASSERT_FALSE(HCNumberAsBoolean(number));
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
}
