//
//  HCList.c
//  Test
//
//  Created by Matt Stoker on 1/21/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCList_Internal.h"
#include <string.h>
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCListTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCList",
    },
    .isEqual = (void*)HCListIsEqual,
    .hashValue = (void*)HCListHashValue,
    .print = (void*)HCListPrint,
    .destroy = (void*)HCListDestroy,
};
HCType HCListType = (HCType)&HCListTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------
#define HCListNotFoundStatic (-1)
const HCInteger HCListNotFound = HCListNotFoundStatic;
const HCListIterator HCListIteratorInvalid = { .list = NULL, .index = HCListNotFoundStatic, .object = NULL, .state = (void*)HCListNotFoundStatic };

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCListRef HCListCreate() {
    return HCListCreateWithCapacity(8);
}

HCListRef HCListCreateWithCapacity(HCInteger capacity) {
    HCListRef self = calloc(sizeof(HCList), 1);
    HCListInit(self, capacity);
    return self;
}

void HCListInit(void* memory, HCInteger capacity) {
    HCRef* objects = malloc(capacity * sizeof(HCRef));
    // TODO: Check that the allocation proceeded successfully, determine how to pass the error otherwise
    
    HCObjectInit(memory);
    HCListRef self = memory;
    self->capacity = capacity;
    self->count = 0;
    self->objects = objects;
    self->base.type = HCListType;
}

void HCListDestroy(HCListRef self) {
    HCListClear(self);
    free(self->objects);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCListIsEqual(HCListRef self, HCListRef other) {
    if (self->count != other->count) {
        return false;
    }
    for (HCInteger index = 0; index < self->count; index++) {
        if (!HCIsEqual(HCListObjectAtIndex(self, index), HCListObjectAtIndex(other, index))) {
            return false;
        }
    }
    return true;
}

HCInteger HCListHashValue(HCListRef self) {
    HCInteger hash = 5381;
    for (HCInteger index = 0; index < self->count; index++) {
        HCRef object = HCListObjectAtIndex(self, index);
        HCInteger objectHash = HCHashValue(object);
        hash = ((hash << 5) + hash) + objectHash;
    }
    return hash;
}

void HCListPrint(HCListRef self, FILE* stream) {
    fprintf(stream, "[");
    for (HCInteger index = 0; index < self->count; index++) {
        HCRef object = HCListObjectAtIndex(self, index);
        HCPrint(object, stream);
        if (index != self->count - 1) {
            fprintf(stream, ",");
        }
    }
    fprintf(stream, "]");
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Content
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCListIsEmpty(HCListRef self) {
    return HCListCount(self) == 0;
}

HCInteger HCListCount(HCListRef self) {
    return self->count;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Searching
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCListContainsIndex(HCListRef self, HCInteger index) {
    return index >= 0 && index < self->count;
}

HCBoolean HCListContainsObject(HCListRef self, HCRef object) {
    return HCListContainsIndex(self, HCListFirstIndexOfObject(self, object));
}

HCInteger HCListFirstIndexOfObject(HCListRef self, HCRef object) {
    return HCListIndexOfObject(self, 0, false, object);
}

HCInteger HCListLastIndexOfObject(HCListRef self, HCRef object) {
    return HCListIndexOfObject(self, self->count, true, object);
}

HCInteger HCListIndexOfObject(HCListRef self, HCInteger searchIndex, HCBoolean reverseSearch, HCRef object) {
    // Find the first equal object from the search index in the requested direction
    if (!reverseSearch) {
        for (HCInteger index = searchIndex; index < self->count; index++) {
            if (HCIsEqual(object, HCListObjectAtIndex(self, index))) {
                return index;
            }
        }
    }
    else {
        for (HCInteger index = searchIndex - 1; index >= 0; index--) {
            if (HCIsEqual(object, HCListObjectAtIndex(self, index))) {
                return index;
            }
        }
    }
    return HCListNotFound;
}

HCRef HCListFirstObject(HCListRef self) {
    return HCListObjectAtIndex(self, 0);
}

HCRef HCListLastObject(HCListRef self) {
    return HCListObjectAtIndex(self, self->count - 1);
}

HCRef HCListObjectAtIndex(HCListRef self, HCInteger index) {
    return !HCListContainsIndex(self, index) ? NULL : self->objects[index];
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCListClear(HCListRef self) {
    // Release all object references and set count to zero
    for (HCInteger index = 0; index < self->count; index++) {
        HCRelease(self->objects[index]);
    }
    self->count = 0;
}

void HCListAddObject(HCListRef self, HCRef object) {
    HCListAddObjectAtIndex(self, self->count, object);
}

void HCListRemoveObject(HCListRef self) {
    HCListRemoveObjectAtIndex(self, self->count - 1);
}

void HCListAddObjectAtIndex(HCListRef self, HCInteger index, HCRef object) {
    // Check that the add index is acceptable
    if (!HCListContainsIndex(self, index) && index != self->count) {
        // TODO: Error, or just rely on list not increasing in count to indicate?
        return;
    }
    
    // Check that there is sufficient space for the object
    if (self->capacity == self->count) {
        HCInteger increasedCapacity = self->capacity * 2;
        self->objects = realloc(self->objects, increasedCapacity * sizeof(HCRef));
        // TODO: Check for realloc failure
        self->capacity = increasedCapacity;
    }
    
    // Shift objects to make a slot available for the object
    self->count++;
    for (HCInteger moveIndex = self->count - 1; moveIndex > index; moveIndex--) {
        self->objects[moveIndex] = self->objects[moveIndex - 1];
    }
    
    // Add the object
    self->objects[index] = HCRetain(object);
}

void HCListRemoveObjectAtIndex(HCListRef self, HCInteger index) {
    // Check that the remove index is acceptable
    if (!HCListContainsIndex(self, index)) {
        // TODO: Error, or just rely on list not decreasing in count to indicate?
        return;
    }
    
    // Remove the object
    // TODO: Return object?
    HCRelease(self->objects[index]);
    
    // Shift objects to close the slot the object occupied
    for (HCInteger moveIndex = index; moveIndex < self->count - 1; moveIndex++) {
        self->objects[moveIndex] = self->objects[moveIndex + 1];
    }
    self->count--;

    // Check if removing the object allows for a reduction in capacity
    if (self->capacity == self->count * 2) {
        HCInteger decreasedCapacity = self->capacity / 2;
        self->objects = realloc(self->objects, decreasedCapacity * sizeof(HCRef));
        // TODO: Check for realloc failure
        self->capacity = decreasedCapacity;
    }
}

void HCListRemoveFirstObjectEqualToObject(HCListRef self, HCRef object) {
    HCListRemoveObjectEqualToObject(self, 0, false, object);
}

void HCListRemoveLastObjectEqualToObject(HCListRef self, HCRef object) {
    HCListRemoveObjectEqualToObject(self, self->count, true, object);
}

void HCListRemoveAllObjectsEqualToObject(HCListRef self, HCRef object) {
    // Remove all objects equal to the requested object, back to front
    // TODO: Deque better for speed?
    for (HCInteger index = self->count - 1; index >= 0; index--) {
        if (HCIsEqual(object, HCListObjectAtIndex(self, index))) {
            HCListRemoveObjectAtIndex(self, index);
        }
    }
}

void HCListRemoveObjectEqualToObject(HCListRef self, HCInteger searchIndex, HCBoolean reverseSearch, HCRef object) {
    HCInteger index = HCListIndexOfObject(self, searchIndex, reverseSearch, object);
    if (HCListContainsIndex(self, index)) {
        HCListRemoveObjectAtIndex(self, index);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCListAddObjectReleased(HCListRef self, HCRef object) {
    HCListAddObject(self, object);
    HCRelease(object);
}

HCRef HCListRemoveObjectRetained(HCListRef self) {
    HCRef object = HCRetain(HCListLastObject(self));
    HCListRemoveObject(self);
    return object;
}

void HCListAddObjectReleasedAtIndex(HCListRef self, HCInteger index, HCRef object) {
    HCListAddObjectAtIndex(self, index, object);
    HCRelease(object);
}

HCRef HCListRemoveObjectRetainedAtIndex(HCListRef self, HCInteger index) {
    HCRef object = HCRetain(HCListObjectAtIndex(self, index));
    HCListRemoveObjectAtIndex(self, index);
    return object;
}

HCRef HCListRemoveFirstObjectRetainedEqualToObject(HCListRef self, HCRef object) {
    HCInteger index = HCListFirstIndexOfObject(self, object);
    return HCListRemoveObjectRetainedAtIndex(self, index);
}

HCRef HCListRemoveLastObjectRetainedEqualToObject(HCListRef self, HCRef object) {
    HCInteger index = HCListLastIndexOfObject(self, object);
    return HCListRemoveObjectRetainedAtIndex(self, index);
}

HCRef HCListRemoveObjectRetainedEqualToObject(HCListRef self, HCInteger searchIndex, HCBoolean reverseSearch, HCRef object) {
    HCInteger index = HCListIndexOfObject(self, searchIndex, reverseSearch, object);
    return HCListRemoveObjectRetainedAtIndex(self, index);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Iteration
//----------------------------------------------------------------------------------------------------------------------------------
HCListIterator HCListIterationBegin(HCListRef self) {
    return HCListIterationBeginAtIndex(self, 0);
}

HCListIterator HCListIterationBeginAtLast(HCListRef self) {
    return HCListIterationBeginAtIndex(self, self->count - 1);
}

HCListIterator HCListIterationBeginAtIndex(HCListRef self, HCInteger index) {
    HCListIterator iterator = {
        .list = self,
        .index = HCListContainsIndex(self, index) ? index : HCListNotFound,
        .object = HCListObjectAtIndex(self, index),
        .state = (void*)0x1
    };
    if (HCListIterationHasEnded(&iterator)) {
        HCListIterationEnd(&iterator);
    }
    return iterator;
}

void HCListIterationPrevious(HCListIterator* iterator) {
    iterator->index--;
    if (iterator->index < -1) {
        iterator->index = -1;
    }
    iterator->object = HCListObjectAtIndex(iterator->list, iterator->index);
}

void HCListIterationNext(HCListIterator* iterator) {
    iterator->index++;
    if (iterator->index > iterator->list->count) {
        iterator->index = iterator->list->count;
    }
    iterator->object = HCListObjectAtIndex(iterator->list, iterator->index);
}

void HCListIterationEnd(HCListIterator* iterator) {
    iterator->index = HCListNotFound;
    iterator->object = NULL;
}

HCBoolean HCListIterationHasBegun(HCListIterator* iterator) {
    return iterator->list == NULL || iterator->state == (void*)0x1;
}

HCBoolean HCListIterationHasPrevious(HCListIterator* iterator) {
    return iterator->list != NULL && iterator->index > 0;
}

HCBoolean HCListIterationHasNext(HCListIterator* iterator) {
    return iterator->list != NULL && iterator->index < HCListCount(iterator->list) - 1;
}

HCBoolean HCListIterationHasEnded(HCListIterator* iterator) {
    return HCListIterationHasBegun(iterator) && (iterator->list == NULL || iterator->object == NULL || !HCListContainsIndex(iterator->list, iterator->index));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Iteration Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCListForEach(HCListRef self, HCListForEachFunction forEachFunction, void* context) {
    if (forEachFunction == NULL) {
        return;
    }
    for (HCListIterator i = HCListIterationBegin(self); !HCListIterationHasEnded(&i); HCListIterationNext(&i)) {
        forEachFunction(context, i.object);
    }
}

HCListRef HCListFilterRetained(HCListRef self, HCListFilterFunction isIncluded, void* context) {
    HCListRef filtered = HCListCreate();
    if (isIncluded == NULL) {
        return filtered;
    }
    for (HCListIterator i = HCListIterationBegin(self); !HCListIterationHasEnded(&i); HCListIterationNext(&i)) {
        if (isIncluded(context, i.object)) {
            HCListAddObject(filtered, i.object);
        }
    }
    return filtered;
}

HCListRef HCListMapRetained(HCListRef self, HCListMapFunction transform, void* context) {
    HCListRef mapped = HCListCreateWithCapacity(HCListCount(self));
    for (HCListIterator i = HCListIterationBegin(self); !HCListIterationHasEnded(&i); HCListIterationNext(&i)) {
        HCRef mappedValue = transform == NULL ? HCRetain(i.object) : transform(context, i.object);
        HCListAddObjectReleased(mapped, mappedValue);
    }
    return mapped;
}

HCRef HCListReduceRetained(HCListRef self, HCRef initialValue, HCListReduceFunction nextPartialResult, void* context) {
    HCRef aggregateValue = HCRetain(initialValue);
    if (nextPartialResult == NULL) {
        return aggregateValue;
    }
    for (HCListIterator i = HCListIterationBegin(self); !HCListIterationHasEnded(&i); HCListIterationNext(&i)) {
        HCRef intermediateValue = nextPartialResult(context, aggregateValue, i.object);
        HCRelease(aggregateValue);
        aggregateValue = intermediateValue;
    }
    return aggregateValue;
}
