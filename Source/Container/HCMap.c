//
//  HCMap.c
//  HollowCore
//
//  Created by Matt Stoker on 2/2/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCMap_Internal.h"
#include <string.h>
#include <math.h>
#include "../Data/HCString.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCMapTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCMap",
    },
    .isEqual = (void*)HCMapIsEqual,
    .hashValue = (void*)HCMapHashValue,
    .print = (void*)HCMapPrint,
    .destroy = (void*)HCMapDestroy,
};
HCType HCMapType = (HCType)&HCMapTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------
#define HCMapNotFoundStatic (-1)
const HCInteger HCMapNotFound = HCMapNotFoundStatic;
const HCMapIterator HCMapIteratorInvalid = { .map = NULL, .index = HCMapNotFoundStatic, .object = NULL, .state = NULL };

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCMapRef HCMapCreate() {
    return HCMapCreateWithCapacity(8);
}

HCMapRef HCMapCreateWithCapacity(HCInteger capacity) {
    HCMapRef self = calloc(sizeof(HCMap), 1);
    HCMapInit(self, capacity);
    return self;
}

void HCMapInit(void* memory, HCInteger capacity) {
    HCSetRef keys = HCSetCreateWithCapacity(capacity);
    HCSetRef pairs = HCSetCreateWithCapacity(capacity);
    // TODO: Check that the allocation proceeded successfully, determine how to pass the error otherwise
    
    HCObjectInit(memory);
    HCMapRef self = memory;
    self->keys = keys;
    self->pairs = pairs;
    self->base.type = HCMapType;
}

void HCMapDestroy(HCMapRef self) {
    HCMapClear(self);
    HCRelease(self->keys);
    HCRelease(self->pairs);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCMapIsEqual(HCMapRef self, HCMapRef other) {
    if (HCMapCount(self) != HCMapCount(other)) {
        return false;
    }
    for (HCMapIterator i = HCMapIterationBegin(self); !HCMapIterationHasEnded(&i); HCMapIterationNext(&i)) {
        HCRef object = HCMapObjectForKey(other, i.key);
        if (!HCIsEqual(i.object, object)) {
            HCMapIterationEnd(&i);
            return false;
        }
    }
    return true;
}

HCInteger HCMapHashValue(HCMapRef self) {
    return HCHashValue(self->pairs);
}

void HCMapPrint(HCMapRef self, FILE* stream) {
    fprintf(stream, "{");
    for (HCMapIterator i = HCMapIterationBegin(self); !HCMapIterationHasEnded(&i); HCMapIterationNext(&i)) {
        HCPrint(i.key, stream);
        fprintf(stream, ",");
        HCPrint(i.object, stream);
        if (HCMapIterationHasNext(&i)) {
            fprintf(stream, ",");
        }
    }
    fprintf(stream, "}");
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Content
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCMapIsEmpty(HCMapRef self) {
    return HCMapCount(self) == 0;
}

HCInteger HCMapCount(HCMapRef self) {
    return HCSetCount(self->keys);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Searching
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCMapContainsKey(HCMapRef self, HCRef key) {
    return HCSetContainsObject(self->keys, key);
}

HCRef HCMapFirstKey(HCMapRef self) {
    return HCMapKeyAtIterationIndex(self, 0);
}

HCRef HCMapLastKey(HCMapRef self) {
    return HCMapKeyAtIterationIndex(self, HCMapCount(self) - 1);
}

HCRef HCMapKeyAtIterationIndex(HCMapRef self, HCInteger index) {
    return HCSetObjectAtIterationIndex(self->keys, index);
}

HCBoolean HCMapContainsObject(HCMapRef self, HCRef object) {
    for (HCMapIterator i = HCMapIterationBegin(self); !HCMapIterationHasEnded(&i); HCMapIterationNext(&i)) {
        if (HCIsEqual(i.object, object)) {
            HCMapIterationEnd(&i);
            return true;
        }
    }
    return false;
}

HCRef HCMapFirstObject(HCMapRef self) {
    return HCMapObjectAtIterationIndex(self, 0);
}

HCRef HCMapLastObject(HCMapRef self) {
    return HCMapObjectAtIterationIndex(self, HCMapCount(self) - 1);
}

HCRef HCMapObjectAtIterationIndex(HCMapRef self, HCInteger index) {
    return HCMapObjectForKey(self, HCMapKeyAtIterationIndex(self, index));
}

HCRef HCMapObjectForKey(HCMapRef self, HCRef key) {
    // TODO: How to avoid creating a pair just to do the comparison operation?
    HCMapPairRef keyEmptyPair = HCMapPairCreate(key, NULL);
    HCMapPairRef pair = HCSetObjectEqualToObject(self->pairs, keyEmptyPair);
    HCRelease(keyEmptyPair);
    return pair == NULL ? NULL : pair->object;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCMapClear(HCMapRef self) {
    HCSetClear(self->keys);
    HCSetClear(self->pairs);
}

void HCMapAddObjectForKey(HCMapRef self, HCRef key, HCRef object) {
    HCSetAddObjectReleased(self->pairs, HCMapPairCreate(key, object));
    HCSetAddObject(self->keys, key);
}

void HCMapRemoveObjectForKey(HCMapRef self, HCRef key) {
    // TODO: How to avoid creating a pair just to do the comparison operation?
    HCMapPairRef keyEmptyPair = HCMapPairCreate(key, NULL);
    HCMapPairRef pair = HCSetRemoveObjectRetained(self->pairs, keyEmptyPair);
    HCRelease(keyEmptyPair);
    if (pair != NULL) {
        HCSetRemoveObject(self->keys, pair->key);
        HCRelease(pair);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCMapAddObjectReleasedForKey(HCMapRef self, HCRef key, HCRef object) {
    HCMapAddObjectForKey(self, key, object);
    HCRelease(object);
}

void HCMapAddObjectReleasedForKeyReleased(HCMapRef self, HCRef key, HCRef object) {
    HCMapAddObjectForKey(self, key, object);
    HCRelease(key);
    HCRelease(object);
}

HCRef HCMapRemoveObjectRetainedForKey(HCMapRef self, HCRef key) {
    HCRef object = HCMapObjectForKey(self, key);
    HCRetain(object);
    HCMapRemoveObjectForKey(self, key);
    return object;
}

HCRef HCMapRemoveObjectRetainedForKeyReleased(HCMapRef self, HCRef key) {
    HCRef object = HCMapObjectForKey(self, key);
    HCRetain(object);
    HCMapRemoveObjectForKey(self, key);
    HCRelease(key);
    return object;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Null-Terminated String Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCMapContainsCStringKey(HCMapRef self, const char* key) {
    HCStringRef keyString = HCStringCreateWithCString(key);
    HCBoolean contained = HCMapContainsKey(self, keyString);
    HCRelease(keyString);
    return contained;
}

HCRef HCMapObjectForCStringKey(HCMapRef self, const char* key) {
    HCStringRef keyString = HCStringCreateWithCString(key);
    HCRef object = HCMapObjectForKey(self, keyString);
    HCRelease(keyString);
    return object;
}

void HCMapAddObjectForCStringKey(HCMapRef self, const char* key, HCRef object) {
    HCStringRef keyString = HCStringCreateWithCString(key);
    HCMapAddObjectForKey(self, keyString, object);
    HCRelease(keyString);
}

void HCMapRemoveObjectForCStringKey(HCMapRef self, const char* key) {
    HCStringRef keyString = HCStringCreateWithCString(key);
    HCMapRemoveObjectForKey(self, keyString);
    HCRelease(keyString);
}

void HCMapAddObjectReleasedForCStringKey(HCMapRef self, const char* key, HCRef object) {
    HCStringRef keyString = HCStringCreateWithCString(key);
    HCMapAddObjectReleasedForKey(self, keyString, object);
    HCRelease(keyString);
}

HCRef HCMapRemoveObjectRetainedForCStringKey(HCMapRef self, const char* key) {
    HCStringRef keyString = HCStringCreateWithCString(key);
    HCRef object = HCMapRemoveObjectRetainedForKey(self, keyString);
    HCRelease(keyString);
    return object;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Iteration
//----------------------------------------------------------------------------------------------------------------------------------
HCMapIterator HCMapIterationBegin(HCMapRef self) {
    HCSetIterator i = HCSetIterationBegin(self->pairs);
    HCSetIterator* pairIterator = malloc(sizeof(HCSetIterator));
    memcpy(pairIterator, &i, sizeof(i));
    HCMapPairRef pair = pairIterator->object;
    HCMapIterator iterator = {
        .map = self,
        .index = pairIterator->index,
        .object = pair == NULL ? NULL : pair->object,
        .key = pair == NULL ? NULL : pair->key,
        .state = pairIterator
    };
    return iterator;
}

void HCMapIterationNext(HCMapIterator* iterator) {
    HCSetIterator* pairIterator = iterator->state;
    HCSetIterationNext(pairIterator);
    HCMapPairRef pair = pairIterator->object;
    iterator->index = pairIterator->index;
    iterator->object = pair == NULL ? NULL : pair->object;
    iterator->key = pair == NULL ? NULL : pair->key;
}

void HCMapIterationEnd(HCMapIterator* iterator) {
    free(iterator->state);
    iterator->index = HCMapCount(iterator->map);
    iterator->object = NULL;
    iterator->key = NULL;
    iterator->state = NULL;
}

HCBoolean HCMapIterationHasBegun(HCMapIterator* iterator) {
    return iterator->index != HCMapNotFound;
}

HCBoolean HCMapIterationHasNext(HCMapIterator* iterator) {
    HCSetIterator* pairIterator = iterator->state;
    return HCSetIterationHasNext(pairIterator);
}

HCBoolean HCMapIterationHasEnded(HCMapIterator* iterator) {
    return HCMapIterationHasBegun(iterator) && iterator->map != NULL && (iterator->key == NULL || iterator->object == NULL);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Pair Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCMapPairTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCMapPair",
    },
    .isEqual = (void*)HCMapPairIsEqual,
    .hashValue = (void*)HCMapPairHashValue,
    .print = (void*)HCObjectPrint,
    .destroy = (void*)HCMapPairDestroy,
};
HCType HCMapPairType = (HCType)&HCMapPairTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Pair Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCMapPairRef HCMapPairCreate(HCRef key, HCRef object) {
    HCMapPairRef self = calloc(sizeof(HCMapPair), 1);
    HCMapPairInit(self, key, object);
    return self;
}

void HCMapPairInit(void* memory, HCRef key, HCRef object) {
    HCObjectInit(memory);
    HCMapPairRef self = memory;
    self->key = HCRetain(key);
    self->object = HCRetain(object);
    self->base.type = HCMapPairType;
}

void HCMapPairDestroy(HCMapPairRef self) {
    HCRelease(self->key);
    HCRelease(self->object);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Pair Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCMapPairIsEqual(HCMapPairRef self, HCMapPairRef other) {
    return HCIsEqual(self->key, other->key);
}

HCInteger HCMapPairHashValue(HCMapPairRef self) {
    return HCHashValue(self->key);
}
