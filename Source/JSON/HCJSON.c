///
/// @file HCJSON.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 2/17/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "HCJSON_Internal.h"
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCRef HCJSONValueCreateWithData(HCDataRef data) {
    json_value* jsonParserValue = json_parse((const json_char*)HCDataBytes(data), HCDataSize(data));
    HCJSONValueRef value = HCJSONValueCreateWithJSONParserValue(jsonParserValue);
    json_value_free(jsonParserValue);
    return value;
}

HCJSONValueRef HCJSONValueCreateWithJSONParserValue(json_value* jsonParserValue) {
    // Null values are HCJSONValueTypeNull
    if (jsonParserValue == NULL) {
        return NULL;
    }
    
    // Based on the value type, create and return a corresponding JSON object containing the data found in the value
    switch (jsonParserValue->type) {
        case json_none: return NULL;
        case json_null: return NULL;
        case json_boolean: return HCNumberCreateWithBoolean(jsonParserValue->u.boolean);
        case json_integer: return HCNumberCreateWithInteger(jsonParserValue->u.integer);
        case json_double: return HCNumberCreateWithReal(jsonParserValue->u.dbl);
        case json_string: return HCStringCreateWithBytes(HCStringEncodingUTF8, jsonParserValue->u.string.length, (HCByte*)jsonParserValue->u.string.ptr);
        case json_array: {
            // Convert and aggregate the JSON array elements in a list
            HCListRef list = HCListCreateWithCapacity(jsonParserValue->u.array.length);
            for (unsigned int elementIndex = 0; elementIndex < jsonParserValue->u.array.length; elementIndex++) {
                HCJSONValueRef element = HCJSONValueCreateWithJSONParserValue(jsonParserValue->u.array.values[elementIndex]);
                HCListAddObjectReleased(list, element);
            }
            return list;
        } break;
        case json_object: {
            // Convert and aggregate the JSON object names and elements in a map
            HCMapRef map = HCMapCreateWithCapacity(jsonParserValue->u.object.length);
            for (unsigned int elementIndex = 0; elementIndex < jsonParserValue->u.object.length; elementIndex++) {
                json_object_entry* entry = &(jsonParserValue->u.object.values[elementIndex]);
                HCStringRef key = HCStringCreateWithBytes(HCStringEncodingUTF8, entry->name_length, (HCByte*)entry->name);
                HCJSONValueRef element = HCJSONValueCreateWithJSONParserValue(entry->value);
                HCMapAddObjectReleasedForKeyReleased(map, key, element);
            }
            return map;
        } break;
    }
    return NULL;
}

HCDataRef HCJSONValueAsDataRetained(HCJSONValueRef value) {
    HCDataRef data = HCDataCreate();
    HCJSONValueAppendToData(value, data);
    return data;
}

void HCJSONValueAppendToData(HCJSONValueRef value, HCDataRef data) {
    HCJSONValueType type = HCJSONValueTypeForObject(value);
    switch (type) {
        case HCJSONValueTypeFalse: HCDataAddBytes(data, strlen("false"), (HCByte*)"false"); break;
        case HCJSONValueTypeTrue: HCDataAddBytes(data, strlen("true"), (HCByte*)"true"); break;
        case HCJSONValueTypeNumber: {
            HCNumberRef number = (HCNumberRef)value;
            if (HCNumberIsInteger(number)) {
                HCInteger integer = HCNumberAsInteger(number);
                char integerString[0xFF];
                sprintf(integerString, "%" PRIi64, integer);
                HCDataAddBytes(data, strlen(integerString), (HCByte*)integerString);
            }
            else {
                HCReal real = HCNumberAsReal(number);
                char realString[0xFF];
                sprintf(realString, "%.17g", real);
                HCDataAddBytes(data, strlen(realString), (HCByte*)realString);
            }
        } break;
        case HCJSONValueTypeString: {
            HCStringRef string = (HCStringRef)value;
            HCDataAddBytes(data, 1, (HCByte*)"\"");
            HCDataAddBytes(data, HCStringCodeUnitCount(string), (HCByte*)HCStringAsCString(string));
            HCDataAddBytes(data, 1, (HCByte*)"\"");
        } break;
        case HCJSONValueTypeArray: {
            HCListRef list = (HCListRef)value;
            HCDataAddBytes(data, 1, (HCByte*)"[");
            for (HCListIterator i = HCListIterationBegin(list); !HCListIterationHasEnded(&i); HCListIterationNext(&i)) {
                HCJSONValueAppendToData(i.object, data);
                if (HCListIterationHasNext(&i)) {
                    HCDataAddBytes(data, 1, (HCByte*)",");
                }
            }
            HCDataAddBytes(data, 1, (HCByte*)"]");
        } break;
        case HCJSONValueTypeObject: {
            HCMapRef map = (HCMapRef)value;
            HCDataAddBytes(data, 1, (HCByte*)"{");
            for (HCMapIterator i = HCMapIterationBegin(map); !HCMapIterationHasEnded(&i); HCMapIterationNext(&i)) {
                if (!HCObjectIsOfKind(i.key, HCStringType)) {
                    continue;
                }
                HCJSONValueAppendToData(i.key, data);
                HCDataAddBytes(data, 1, (HCByte*)":");
                HCJSONValueAppendToData(i.object, data);
                if (HCMapIterationHasNext(&i)) {
                    HCDataAddBytes(data, 1, (HCByte*)",");
                }
            }
            HCDataAddBytes(data, 1, (HCByte*)"}");
        } break;
        default: HCDataAddBytes(data, strlen("null"), (HCByte*)"null"); break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Query
//----------------------------------------------------------------------------------------------------------------------------------
HCJSONValueType HCJSONValueTypeForObject(HCJSONValueRef object) {
    // Query the object type to determine if it's kind is of one of the corresponding JSON object types
    if (object == NULL) {
        return HCJSONValueTypeNull;
    }
    if (HCObjectIsOfKind(object, HCNumberType)) {
        HCNumberRef number = (HCNumberRef)object;
        if (HCNumberIsBoolean(number)) {
            return HCNumberAsBoolean(number) ? HCJSONValueTypeTrue : HCJSONValueTypeFalse;
        }
        return HCJSONValueTypeNumber;
    }
    if (HCObjectIsOfKind(object, HCStringType)) {
        return HCJSONValueTypeString;
    }
    if (HCObjectIsOfKind(object, HCListType)) {
        return HCJSONValueTypeArray;
    }
    if (HCObjectIsOfKind(object, HCMapType)) {
        return HCJSONValueTypeObject;
    }
    return HCJSONValueTypeUnknown;
}
