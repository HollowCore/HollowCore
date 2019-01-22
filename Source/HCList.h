//
//  HCList.h
//  Test
//
//  Created by Matt Stoker on 1/21/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCList_h
#define HCList_h

#include "HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCListType;
typedef struct HCList* HCListRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------
extern const HCInteger HCListNotFound;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCListRef HCListCreate(void);
HCListRef HCListCreateWithCapacity(HCInteger capacity);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCListIsEqual(HCListRef self, HCListRef other);
HCInteger HCListHashValue(HCListRef self);
void HCListPrint(HCListRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Content
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCListIsEmpty(HCListRef self);
HCInteger HCListCount(HCListRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Searching
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCListContainsIndex(HCListRef self, HCInteger index);
HCBoolean HCListContainsObject(HCListRef self, HCRef object);
HCInteger HCListFirstIndexOfObject(HCListRef self, HCRef object);
HCInteger HCListLastIndexOfObject(HCListRef self, HCRef object);
HCInteger HCListIndexOfObject(HCListRef self, HCInteger searchIndex, HCBoolean reverseSearch, HCRef object);
HCRef HCListFirstObject(HCListRef self);
HCRef HCListLastObject(HCListRef self);
HCRef HCListObjectAtIndex(HCListRef self, HCInteger index);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCListClear(HCListRef self);

void HCListAddObject(HCListRef self, HCRef object);
void HCListRemoveObject(HCListRef self);
void HCListAddObjectAtIndex(HCListRef self, HCInteger index, HCRef object);
void HCListRemoveObjectAtIndex(HCListRef self, HCInteger index);

void HCListRemoveFirstObjectEqualToObject(HCListRef self, HCRef object);
void HCListRemoveLastObjectEqualToObject(HCListRef self, HCRef object);
void HCListRemoveAllObjectsEqualToObject(HCListRef self, HCRef object);
void HCListRemoveObjectEqualToObject(HCListRef self, HCInteger searchIndex, HCBoolean reverseSearch, HCRef object);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCListAddObjectReleased(HCListRef self, HCRef object);
HCRef HCListRemoveObjectRetained(HCListRef self);
void HCListAddObjectReleasedAtIndex(HCListRef self, HCInteger index, HCRef object);
HCRef HCListRemoveObjectRetainedAtIndex(HCListRef self, HCInteger index);

HCRef HCListRemoveFirstObjectRetainedEqualToObject(HCListRef self, HCRef object);
HCRef HCListRemoveLastObjectRetainedEqualToObject(HCListRef self, HCRef object);
HCRef HCListRemoveObjectRetainedEqualToObject(HCListRef self, HCInteger searchIndex, HCBoolean reverseSearch, HCRef object);

#endif /* HCList_h */
