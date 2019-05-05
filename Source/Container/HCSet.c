//
//  HCSet.c
//  HollowCore
//
//  Created by Matt Stoker on 1/23/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCSet_Internal.h"
#include <string.h>
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCSetTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCSet",
    },
    .isEqual = (void*)HCSetIsEqual,
    .hashValue = (void*)HCSetHashValue,
    .print = (void*)HCSetPrint,
    .destroy = (void*)HCSetDestroy,
};
HCType HCSetType = (HCType)&HCSetTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------
#define HCSetNotFoundStatic (-1)
const HCInteger HCSetNotFound = HCSetNotFoundStatic;
const HCSetIterator HCSetIteratorInvalid = { .set = NULL, .index = HCSetNotFoundStatic, .object = NULL, .state = NULL };

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCSetRef HCSetCreate() {
    return HCSetCreateWithCapacity(8);
}

HCSetRef HCSetCreateWithCapacity(HCInteger capacity) {
    HCSetRef self = calloc(sizeof(HCSet), 1);
    HCSetInit(self, capacity);
    return self;
}

void HCSetInit(void* memory, HCInteger capacity) {
    HCSetSlot* slots = calloc(sizeof(HCSetSlot), capacity);
    // TODO: Check that the allocation proceeded successfully, determine how to pass the error otherwise
    
    HCObjectInit(memory);
    HCSetRef self = memory;
    self->capacity = capacity;
    self->count = 0;
    self->slots = slots;
    self->base.type = HCSetType;
}

void HCSetDestroy(HCSetRef self) {
    HCSetClear(self);
    for (HCInteger slotIndex = 0; slotIndex < self->capacity; slotIndex++) {
        HCSetSlot* slot = self->slots[slotIndex].next;
        while (slot != NULL) {
            HCSetSlot* next = slot->next;
            free(slot);
            slot = next;
        }
    }
    free(self->slots);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSetIsEqual(HCSetRef self, HCSetRef other) {
    if (self->count != other->count) {
        return false;
    }
    for (HCSetIterator i = HCSetIterationBegin(self); !HCSetIterationHasEnded(&i); HCSetIterationNext(&i)) {
        if (!HCSetContainsObject(other, i.object)) {
            return false;
        }
    }
    return true;
}

HCInteger HCSetHashValue(HCSetRef self) {
    HCInteger hash = 5381;
    for (HCSetIterator i = HCSetIterationBegin(self); !HCSetIterationHasEnded(&i); HCSetIterationNext(&i)) {
        HCInteger objectHash = HCHashValue(i.object);
        hash = ((hash << 5) + hash) + objectHash;
    }
    return hash;
}

void HCSetPrint(HCSetRef self, FILE* stream) {
    fprintf(stream, "[");
    for (HCSetIterator i = HCSetIterationBegin(self); !HCSetIterationHasEnded(&i); HCSetIterationNext(&i)) {
        HCPrint(i.object, stream);
        if (HCSetIterationHasNext(&i)) {
            fprintf(stream, ",");
        }
    }
    fprintf(stream, "]");
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Content
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSetIsEmpty(HCSetRef self) {
    return HCSetCount(self) == 0;
}

HCInteger HCSetCount(HCSetRef self) {
    return self->count;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Searching
//----------------------------------------------------------------------------------------------------------------------------------
void HCSetFindSlotContainingObject(HCSetRef self, HCRef object, HCInteger* objectSlotIndex, HCBoolean* found, HCSetSlot** resultSlot) {
    // Report failure on requests to find the null object
    if (object == NULL) {
        if (objectSlotIndex != NULL) {
            *objectSlotIndex = HCSetNotFound;
        }
        if (found != NULL) {
            *found = false;
        }
        if (resultSlot != NULL) {
            *resultSlot = NULL;
        }
        return;
    }
    
    // Find the slot index the object should occupy
    HCInteger objectHash = HCHashValue(object);
    HCInteger slotIndex = objectHash % self->capacity;
    HCSetSlot* slot = &self->slots[slotIndex];
    if (objectSlotIndex != NULL) {
        *objectSlotIndex = slotIndex;
    }
    
    // Find an empty slot in the linked list at the slot index, checking for equal objects along the way
    if (found != NULL || resultSlot != NULL) {
        while (true) {
            if (slot->object != NULL && HCIsEqual(slot->object, object)) {
                if (found != NULL) {
                    *found = true;
                }
                if (resultSlot != NULL) {
                    *resultSlot = slot;
                }
                return;
            }
            if (slot->next == NULL) {
                if (found != NULL) {
                    *found = false;
                }
                if (resultSlot != NULL) {
                    *resultSlot = slot;
                }
                return;
            }
            slot = slot->next;
        }
    }
}

HCInteger HCSetNextSlotFromSlot(HCSetRef self, HCInteger index) {
    // Iterate to find the next occupied slot
    // TODO: How to determine this more effectively?
    HCInteger startSlotIndex = index == HCSetNotFound ? 0 : (index + 1);
    for (HCInteger slotIndex = startSlotIndex; slotIndex < self->capacity; slotIndex++) {
        if (self->slots[slotIndex].object != NULL) {
            return slotIndex;
        }
    }
    return HCSetNotFound;
}

HCBoolean HCSetContainsObject(HCSetRef self, HCRef object) {
    HCBoolean found = false;
    HCSetFindSlotContainingObject(self, object, NULL, &found, NULL);
    return found;
}

HCRef HCSetFirstObject(HCSetRef self) {
    return HCSetObjectAtIterationIndex(self, 0);
}

HCRef HCSetLastObject(HCSetRef self) {
    return HCSetObjectAtIterationIndex(self, self->count - 1);
}

HCRef HCSetObjectAtIterationIndex(HCSetRef self, HCInteger index) {
    // Iterate to the index requested and return the object found there
    HCSetIterator i = HCSetIterationBegin(self);
    HCRef object = i.object;
    while (HCSetIterationHasNext(&i)) {
        HCSetIterationNext(&i);
        if (i.index == index) {
            object = i.object;
        }
    }
    HCSetIterationEnd(&i);
    return object;
}

HCRef HCSetObjectEqualToObject(HCSetRef self, HCRef object) {
    HCBoolean found = false;
    HCSetSlot* slot = NULL;
    HCSetFindSlotContainingObject(self, object, NULL, &found, &slot);
    if (!found) {
        return NULL;
    }
    return slot->object;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCSetClear(HCSetRef self) {
    // Release all object references in all slots and set count to zero
    // TODO: Track first / last occupied slot or a linked-list of slots to shorten operation
    for (HCInteger slotIndex = 0; slotIndex < self->capacity; slotIndex++) {
        HCSetSlot* slot = &self->slots[slotIndex];
        do {
            HCRelease(slot->object);
            slot->object = NULL;
            HCSetSlot* next = slot->next;
            slot = next;
        } while (slot != NULL);
    }
    self->count = 0;
}

void HCSetAddObject(HCSetRef self, HCRef object) {
    // TODO: Check capacity, if it is greater than expand threshold, expand and rehash
    
    // Find the slot index the object should occupy, as well as the end slot at that index
    HCSetSlot* slot = NULL;
    HCBoolean found = false;
    HCSetFindSlotContainingObject(self, object, NULL, &found, &slot);
    
    // Where an equal object was already found in the set, replace it, otherwise add it to the set
    // NOTE: Slots at the head of the list at the slot index are treated differently than linked slots
    if (found) {
        HCRef previousObject = slot->object;
        slot->object = HCRetain(object);
        HCRelease(previousObject);
    }
    else {
        if (slot->object != NULL) {
            HCSetSlot* next = slot->next;
            slot->next = malloc(sizeof(HCSetSlot));
            slot = slot->next;
            slot->next = next;
        }
        slot->object = HCRetain(object);
        self->count++;
    }
}

void HCSetRemoveObject(HCSetRef self, HCRef object) {
    // Find the slot index the object should occupy, as well as the slot to which it is linked
    HCSetSlot* slot = NULL;
    HCInteger slotIndex = HCSetNotFound;
    HCBoolean found = false;
    HCSetFindSlotContainingObject(self, object, &slotIndex, &found, &slot);
    
    // Given that the object was in the set, unlink it from its slot and clean the slot up where needed
    // NOTE: Slots at the head of the list at the slot index are treated differently than linked slots
    if (found) {
        HCRelease(slot->object);
        self->count--;
        HCSetSlot* nextSlot = slot->next;
        if (nextSlot == NULL) {
            slot->object = NULL;
        }
        else {
            slot->object = nextSlot->object;
            slot->next = nextSlot->next;
            free(nextSlot);
        }
    }
    
    // TODO: Check capacity, if it is smaller than contract threshold, contract and rehash
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCSetAddObjectReleased(HCSetRef self, HCRef object) {
    HCSetAddObject(self, object);
    HCRelease(object);
}

HCRef HCSetRemoveObjectRetained(HCSetRef self, HCRef object) {
    // Find the slot index the object should occupy, as well as the end slot at that index
    HCSetSlot* slot = NULL;
    HCBoolean found = false;
    HCSetFindSlotContainingObject(self, object, NULL, &found, &slot);
    
    // Retain the matching object if found, remove it from the set, and return it
    HCRef foundObject = slot->object;
    if (found) {
        HCRetain(foundObject);
    }
    HCSetRemoveObject(self, object);
    return foundObject;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Iteration
//----------------------------------------------------------------------------------------------------------------------------------
HCSetIterator HCSetIterationBegin(HCSetRef self) {
    // Determine if the set has any items to iterate over
    if (self->count == 0) {
        HCSetIterator iterator = {
            .set = self,
            .index = self->count,
            .object = NULL,
            .state = NULL
        };
        return iterator;
    }
    
    // Begin iteration with the object in the first slot
    HCInteger firstSlotIndex = HCSetNextSlotFromSlot(self, HCSetNotFound);
    HCSetSlot* slot = &self->slots[firstSlotIndex];
    HCSetIterator iterator = {
        .set = self,
        .index = 0,
        .object = slot->object,
        .state = slot
    };
    return iterator;
}

void HCSetIterationNext(HCSetIterator* iterator) {
    // Check the iteration state for the slot returned by the last call to next
    HCInteger startSlotIndex = HCSetNotFound;
    if (iterator->state != NULL) {
        HCSetSlot* previousSlot = iterator->state;
        
        // Where the last slot has another entry, it is the next iteration result
        if (previousSlot->next != NULL) {
            iterator->state = previousSlot->next;
            iterator->object = previousSlot->next->object;
            iterator->index++;
            return;
        }
        
        // Obtain the slot index for the last iteration result for use in determining the next result
        HCSetFindSlotContainingObject(iterator->set, previousSlot->object, &startSlotIndex, NULL, NULL);
    }
    
    // Start at the beginning of the slot array when previous iteration data cannot be determined
    if (startSlotIndex == HCSetNotFound) {
        startSlotIndex = 0;
    }
    
    // Find the next occupied slot in the slot array
    HCInteger slotIndex = HCSetNextSlotFromSlot(iterator->set, startSlotIndex);
    if (slotIndex == HCSetNotFound) {
        HCSetIterationEnd(iterator);
    }
    else {
        HCSetSlot* slot = &iterator->set->slots[slotIndex];
        if (slot->object != NULL) {
            iterator->state = slot;
            iterator->object = slot->object;
            iterator->index++;
        }
    }
}

void HCSetIterationEnd(HCSetIterator* iterator) {
    iterator->index = iterator->set->count;
    iterator->object = NULL;
    iterator->state = NULL;
}

HCBoolean HCSetIterationHasBegun(HCSetIterator* iterator) {
    return iterator->index != HCSetNotFound;
}

HCBoolean HCSetIterationHasNext(HCSetIterator* iterator) {
    return iterator->set != NULL && iterator->index < iterator->set->count - 1;
}

HCBoolean HCSetIterationHasEnded(HCSetIterator* iterator) {
    return HCSetIterationHasBegun(iterator) && iterator->set != NULL && (iterator->object == NULL || iterator->index == iterator->set->count);
}
