//
//  HCJSON.c
//  Test
//
//  Created by Matt Stoker on 2/18/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include <string.h>

CTEST(HCJSON, Null) {
    const char* json = "null";
    HCDataRef data = HCDataCreateWithBytes(strlen(json), (HCByte*)json);
    HCRef value = HCJSONValueCreateWithData(data);
    ASSERT_TRUE(HCJSONValueTypeForObject(value) == HCJSONValueTypeNull);
    HCRelease(data);
    HCRelease(value);
}

CTEST(HCJSON, False) {
    const char* json = "false";
    HCDataRef data = HCDataCreateWithBytes(strlen(json), (HCByte*)json);
    HCRef value = HCJSONValueCreateWithData(data);
    ASSERT_TRUE(HCJSONValueTypeForObject(value) == HCJSONValueTypeFalse);
    HCRelease(data);
    HCRelease(value);
}

CTEST(HCJSON, True) {
    const char* json = "true";
    HCDataRef data = HCDataCreateWithBytes(strlen(json), (HCByte*)json);
    HCRef value = HCJSONValueCreateWithData(data);
    ASSERT_TRUE(HCJSONValueTypeForObject(value) == HCJSONValueTypeTrue);
    HCRelease(data);
    HCRelease(value);
}

CTEST(HCJSON, Number) {
    const char* json = "42";
    HCDataRef data = HCDataCreateWithBytes(strlen(json), (HCByte*)json);
    HCRef value = HCJSONValueCreateWithData(data);
    ASSERT_TRUE(HCJSONValueTypeForObject(value) == HCJSONValueTypeNumber);
    HCRelease(data);
    HCRelease(value);
}

CTEST(HCJSON, String) {
    const char* json = "\"I am the very model of a Modern Major General.\"";
    HCDataRef data = HCDataCreateWithBytes(strlen(json), (HCByte*)json);
    HCRef value = HCJSONValueCreateWithData(data);
    ASSERT_TRUE(HCJSONValueTypeForObject(value) == HCJSONValueTypeString);
    HCRelease(data);
    HCRelease(value);
}

CTEST(HCJSON, Array) {
    const char* json = "[true,\"two\",3.5]";
    HCDataRef data = HCDataCreateWithBytes(strlen(json), (HCByte*)json);
    HCRef value = HCJSONValueCreateWithData(data);
    ASSERT_TRUE(HCJSONValueTypeForObject(value) == HCJSONValueTypeArray);
    HCListRef list = (HCListRef)value;
    ASSERT_EQUAL(HCListCount(list), 3);
    ASSERT_TRUE(HCJSONValueTypeForObject(HCListObjectAtIndex(list, 0)) == HCJSONValueTypeTrue);
    ASSERT_EQUAL(HCNumberAsBoolean(HCListObjectAtIndex(list, 0)), true);
    ASSERT_TRUE(HCJSONValueTypeForObject(HCListObjectAtIndex(list, 1)) == HCJSONValueTypeString);
    ASSERT_STR(HCStringAsCString(HCListObjectAtIndex(list, 1)), "two");
    ASSERT_TRUE(HCJSONValueTypeForObject(HCListObjectAtIndex(list, 2)) == HCJSONValueTypeNumber);
    ASSERT_DBL_NEAR(HCNumberAsReal(HCListObjectAtIndex(list, 2)), 3.5);
    HCRelease(data);
    HCRelease(value);
}

CTEST(HCJSON, Object) {
    const char* json = "{\"answer\":42,\"question\":\"unknown\"}";
    HCDataRef data = HCDataCreateWithBytes(strlen(json), (HCByte*)json);
    HCRef value = HCJSONValueCreateWithData(data);
    ASSERT_TRUE(HCJSONValueTypeForObject(value) == HCJSONValueTypeObject);
    HCMapRef map = (HCMapRef)value;
    ASSERT_EQUAL(HCMapCount(map), 2);
    ASSERT_TRUE(HCJSONValueTypeForObject(HCMapObjectForCStringKey(map, "answer")) == HCJSONValueTypeNumber);
    ASSERT_EQUAL(HCNumberAsInteger(HCMapObjectForCStringKey(map, "answer")), 42);
    ASSERT_TRUE(HCJSONValueTypeForObject(HCMapObjectForCStringKey(map, "question")) == HCJSONValueTypeString);
    ASSERT_STR(HCStringAsCString(HCMapObjectForCStringKey(map, "question")), "unknown");
    HCRelease(data);
    HCRelease(value);
}

CTEST(HCJSON, NullData) {
    HCJSONValueRef value = NULL;
    HCDataRef data = HCJSONValueAsDataRetained(value);
    HCStringRef jsonString = HCStringCreateWithBytes(HCStringEncodingUTF8, HCDataSize(data), HCDataBytes(data));
    ASSERT_STR(HCStringAsCString(jsonString), "null");
    HCRelease(data);
    HCRelease(value);
    HCRelease(jsonString);
}

CTEST(HCJSON, FalseData) {
    HCJSONValueRef value = HCNumberCreateWithBoolean(false);
    HCDataRef data = HCJSONValueAsDataRetained(value);
    HCStringRef jsonString = HCStringCreateWithBytes(HCStringEncodingUTF8, HCDataSize(data), HCDataBytes(data));
    ASSERT_STR(HCStringAsCString(jsonString), "false");
    HCRelease(data);
    HCRelease(value);
    HCRelease(jsonString);
}

CTEST(HCJSON, TrueData) {
    HCJSONValueRef value = HCNumberCreateWithBoolean(true);
    HCDataRef data = HCJSONValueAsDataRetained(value);
    HCStringRef jsonString = HCStringCreateWithBytes(HCStringEncodingUTF8, HCDataSize(data), HCDataBytes(data));
    ASSERT_STR(HCStringAsCString(jsonString), "true");
    HCRelease(data);
    HCRelease(value);
    HCRelease(jsonString);
}

CTEST(HCJSON, NumberData) {
    HCJSONValueRef value = HCNumberCreateWithInteger(42);
    HCDataRef data = HCJSONValueAsDataRetained(value);
    HCStringRef jsonString = HCStringCreateWithBytes(HCStringEncodingUTF8, HCDataSize(data), HCDataBytes(data));
    ASSERT_STR(HCStringAsCString(jsonString), "42");
    HCRelease(data);
    HCRelease(value);
    HCRelease(jsonString);
}

CTEST(HCJSON, StringData) {
    HCJSONValueRef value = HCStringCreateWithCString("I've information animal, vegetable, and mineral.");
    HCDataRef data = HCJSONValueAsDataRetained(value);
    HCStringRef jsonString = HCStringCreateWithBytes(HCStringEncodingUTF8, HCDataSize(data), HCDataBytes(data));
    ASSERT_STR(HCStringAsCString(jsonString), "\"I've information animal, vegetable, and mineral.\"");
    HCRelease(data);
    HCRelease(value);
    HCRelease(jsonString);
}

CTEST(HCJSON, ArrayData) {
    HCJSONValueRef value = HCListCreate();
    HCListAddObjectReleased(value, HCNumberCreateWithBoolean(true));
    HCListAddObjectReleased(value, HCStringCreateWithCString("two"));
    HCListAddObjectReleased(value, HCNumberCreateWithReal(3.5));
    HCDataRef data = HCJSONValueAsDataRetained(value);
    HCStringRef jsonString = HCStringCreateWithBytes(HCStringEncodingUTF8, HCDataSize(data), HCDataBytes(data));
    ASSERT_STR(HCStringAsCString(jsonString), "[true,\"two\",3.5]");
    HCRelease(data);
    HCRelease(value);
    HCRelease(jsonString);
}

CTEST(HCJSON, ObjectData) {
    HCJSONValueRef value = HCMapCreate();
    HCMapAddObjectReleasedForCStringKey(value, "answer", HCNumberCreateWithInteger(42));
    HCMapAddObjectReleasedForCStringKey(value, "question", HCStringCreateWithCString("unknown"));
    HCDataRef data = HCJSONValueAsDataRetained(value);
    HCStringRef jsonString = HCStringCreateWithBytes(HCStringEncodingUTF8, HCDataSize(data), HCDataBytes(data));
    ASSERT_STR(HCStringAsCString(jsonString), "{\"answer\":42,\"question\":\"unknown\"}");
    HCRelease(data);
    HCRelease(value);
    HCRelease(jsonString);
}
