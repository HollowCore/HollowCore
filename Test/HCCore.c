///
/// @file HCCore.c
/// @package HollowCore
///
/// @author Matt Stoker
/// @date 1/14/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "ctest.h"
#include "../Source/HollowCore.h"

CTEST(HCCore, Boolean) {
    ASSERT_TRUE(HCBooleanIsEqual(true, !(false)));
    ASSERT_EQUAL(HCBooleanHashValue(false), 0);
    ASSERT_EQUAL(HCBooleanHashValue(true), 1);
}

CTEST(HCData, BooleanPrint) {
    HCBooleanPrint(true, stdout); // TODO: Not to stdout
}

CTEST(HCCore, Integer) {
    ASSERT_TRUE(HCIntegerIsEqual(4, 2 + 2));
    ASSERT_NOT_EQUAL(HCBooleanHashValue(42), 0);
}

CTEST(HCData, IntegerPrint) {
    HCIntegerPrint(42, stdout); // TODO: Not to stdout
}

CTEST(HCCore, Real) {
    ASSERT_TRUE(HCRealIsEqual(1.0, 0.5 + 0.5));
    ASSERT_NOT_EQUAL(HCRealHashValue(3.14159), 0);
}

CTEST(HCData, RealPrint) {
    HCRealPrint(3.14159, stdout); // TODO: Not to stdout
}

/*
CTEST(suite1, test1) {
}

CTEST(suite1, test2) {
    ASSERT_EQUAL(1,2);
}

CTEST(suite2, test1) {
    ASSERT_STR("foo", "bar");
}

CTEST(suite3, test3) {
}
 
CTEST(ctest, test_assert_str) {
    ASSERT_STR("foo", "foo");
    ASSERT_STR("foo", "bar");
}

CTEST(ctest, test_assert_equal) {
    ASSERT_EQUAL(123, 123);
    ASSERT_EQUAL(123, 456);
}

CTEST(ctest, test_assert_not_equal) {
    ASSERT_NOT_EQUAL(123, 456);
    ASSERT_NOT_EQUAL(123, 123);
}

CTEST(ctest, test_assert_interval) {
    ASSERT_INTERVAL(10, 20, 15);
    ASSERT_INTERVAL(1000, 2000, 3000);
}

CTEST(ctest, test_assert_null) {
    ASSERT_NULL(NULL);
    ASSERT_NULL((void*)0xdeadbeef);
}

CTEST(ctest, test_assert_not_null_const) {
    ASSERT_NOT_NULL((const char*)"hallo");
}

CTEST(ctest, test_assert_not_null) {
    ASSERT_NOT_NULL((void*)0xdeadbeef);
    ASSERT_NOT_NULL(NULL);
}

CTEST(ctest, test_assert_true) {
    ASSERT_TRUE(1);
    ASSERT_TRUE(0);
}

CTEST(ctest, test_assert_false) {
    ASSERT_FALSE(0);
    ASSERT_FALSE(1);
}

CTEST_SKIP(ctest, test_skip) {
    ASSERT_FAIL();
}

CTEST(ctest, test_assert_fail) {
    ASSERT_FAIL();
}

CTEST(ctest, test_null_null) {
    ASSERT_STR(NULL, NULL);
}

CTEST(ctest, test_null_string) {
    ASSERT_STR(NULL, "shouldfail");
}

CTEST(ctest, test_string_null) {
    ASSERT_STR("shouldfail", NULL);
}

CTEST(ctest, test_string_diff_ptrs) {
    const char *str = "abc\0abc";
    ASSERT_STR(str, str+4);
}

CTEST(ctest, test_large_numbers) {
    unsigned long exp = 4200000000u;
    ASSERT_EQUAL_U(exp, 4200000000u);
    ASSERT_NOT_EQUAL_U(exp, 1200000000u);
}

CTEST(ctest, test_ctest_err) {
    CTEST_ERR("error log");
}

CTEST(ctest, test_dbl_near) {
    double a = 0.000111;
    ASSERT_DBL_NEAR(0.0001, a);
}

CTEST(ctest, test_dbl_near_tol) {
    double a = 0.000111;
    ASSERT_DBL_NEAR_TOL(0.0001, a, 1e-5);
}

CTEST(ctest, test_dbl_far) {
    double a = 1.1;
    ASSERT_DBL_FAR(1., a);
    ASSERT_DBL_FAR_TOL(1., a, 0.01);
}
*/
