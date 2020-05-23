///
/// @file HCCore.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 1/14/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "ctest.h"
#include "../Source/HollowCore.h"

CTEST(HCCore, BooleanEquality) {
    ASSERT_TRUE(HCBooleanIsEqual(false, false));
    ASSERT_TRUE(HCBooleanIsEqual(true, true));
    ASSERT_TRUE(HCBooleanIsEqual(true, !(false)));
    ASSERT_TRUE(HCBooleanIsEqual(false, !(true)));
    ASSERT_TRUE(HCBooleanIsEqual(false, (HCBoolean)0));
    ASSERT_TRUE(HCBooleanIsEqual(true, (HCBoolean)1));
    ASSERT_TRUE(HCBooleanIsEqual(true, (HCBoolean)2));
    ASSERT_TRUE(HCBooleanIsEqual(true, (HCBoolean)-1));
    ASSERT_TRUE(!HCBooleanIsEqual(true, false));
    ASSERT_TRUE(!HCBooleanIsEqual(false, true));
    ASSERT_EQUAL(HCBooleanHashValue(false), 0);
    ASSERT_EQUAL(HCBooleanHashValue(true), 1);
    ASSERT_EQUAL(HCBooleanHashValue((HCBoolean)0), 0);
    ASSERT_EQUAL(HCBooleanHashValue((HCBoolean)1), 1);
    ASSERT_EQUAL(HCBooleanHashValue((HCBoolean)2), 1);
    ASSERT_EQUAL(HCBooleanHashValue((HCBoolean)-1), 1);
}

CTEST(HCData, BooleanPrint) {
    HCBooleanPrint(true, stdout); // TODO: Not to stdout
}

CTEST(HCCore, IntegerEquality) {
    ASSERT_TRUE(HCIntegerIsEqual(4, 2 + 2));
    ASSERT_NOT_EQUAL(HCBooleanHashValue(42), 0);
}

CTEST(HCData, IntegerPrint) {
    HCIntegerPrint(42, stdout); // TODO: Not to stdout
}

CTEST(HCCore, RealEquality) {
    ASSERT_TRUE(HCRealIsInvalid(HCRealInvalid));
    ASSERT_TRUE(!HCRealIsInvalid(HCRealZero));
    ASSERT_TRUE(!HCRealIsInvalid(HCRealInfinity));
    ASSERT_TRUE(!HCRealIsInvalid(HCRealNegativeInfinity));
    
    ASSERT_TRUE(!HCRealIsInfinite(HCRealInvalid));
    ASSERT_TRUE(!HCRealIsInfinite(HCRealZero));
    ASSERT_TRUE(HCRealIsInfinite(HCRealInfinity));
    ASSERT_TRUE(HCRealIsInfinite(HCRealNegativeInfinity));
    
    ASSERT_TRUE(!HCRealIsFinite(HCRealInvalid));
    ASSERT_TRUE(HCRealIsFinite(HCRealZero));
    ASSERT_TRUE(!HCRealIsFinite(HCRealInfinity));
    ASSERT_TRUE(!HCRealIsFinite(HCRealNegativeInfinity));
    
    ASSERT_TRUE(HCRealIsSimilar(1.0, 1.1, 0.5));
    ASSERT_TRUE(!HCRealIsSimilar(1.0, 1.8, 0.5));
    ASSERT_TRUE(!HCRealIsSimilar(1.0, 1.5, 0.5));
    ASSERT_TRUE(HCRealIsSimilar(1.0, nextafter(1.5, 0.0), 0.5));
    
    ASSERT_TRUE(HCRealIsEqual(1.0, 0.5 + 0.5));
    ASSERT_TRUE(!HCRealIsEqual(2.0, 0.5 + 0.5));
    ASSERT_TRUE(!HCRealIsEqual(1.0, nextafter(1.0, 0.0)));
    ASSERT_TRUE(HCRealIsEqual(-1.0, -0.5 - 0.5));
    ASSERT_TRUE(!HCRealIsEqual(-2.0, -0.5 - 0.5));
    ASSERT_TRUE(!HCRealIsEqual(-1.0, nextafter(-1.0, 0.0)));
    
    for (HCReal real = -100.0; real < 100.0; real += 0.5) {
        if (HCRealIsEqual(real, (HCReal)((HCInteger)real))) {
            ASSERT_TRUE(HCRealHashValue(real) == HCIntegerHashValue((HCInteger)real));
        }
    }
}

CTEST(HCData, RealPrint) {
    HCRealPrint(3.14159, stdout); // TODO: Not to stdout
}
