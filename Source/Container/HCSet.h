///
/// @file HCSet.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 1/23/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCSet_h
#define HCSet_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCSetType;
typedef struct HCSet* HCSetRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------
extern const HCInteger HCSetNotFound;

typedef struct HCSetIterator {
    HCSetRef set;
    HCInteger index;
    HCRef object;
    void* state;
} HCSetIterator;
extern const HCSetIterator HCSetIteratorInvalid;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCSetRef HCSetCreate(void);
HCSetRef HCSetCreateWithCapacity(HCInteger capacity);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSetIsEqual(HCSetRef self, HCSetRef other);
HCInteger HCSetHashValue(HCSetRef self);
void HCSetPrint(HCSetRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Content
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSetIsEmpty(HCSetRef self);
HCInteger HCSetCount(HCSetRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Searching
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSetContainsObject(HCSetRef self, HCRef object);
HCRef HCSetFirstObject(HCSetRef self);
HCRef HCSetLastObject(HCSetRef self);
HCRef HCSetObjectAtIterationIndex(HCSetRef self, HCInteger index);
HCRef HCSetObjectEqualToObject(HCSetRef self, HCRef object);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCSetClear(HCSetRef self);

void HCSetAddObject(HCSetRef self, HCRef object);
void HCSetRemoveObject(HCSetRef self, HCRef object);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCSetAddObjectReleased(HCSetRef self, HCRef object);
HCRef HCSetRemoveObjectRetained(HCSetRef self, HCRef object);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Iteration
//----------------------------------------------------------------------------------------------------------------------------------
HCSetIterator HCSetIterationBegin(HCSetRef self);
void HCSetIterationNext(HCSetIterator* iterator);
void HCSetIterationEnd(HCSetIterator* iterator);
HCBoolean HCSetIterationHasBegun(HCSetIterator* iterator);
HCBoolean HCSetIterationHasNext(HCSetIterator* iterator);
HCBoolean HCSetIterationHasEnded(HCSetIterator* iterator);

#endif /* HCSet_h */
