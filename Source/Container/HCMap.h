//
//  HCMap.h
//  HollowCore
//
//  Created by Matt Stoker on 2/2/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCMap_h
#define HCMap_h

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCMapType;
typedef struct HCMap* HCMapRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------
extern const HCInteger HCMapNotFound;
#define HCMapIteratorStateSizeStatic (4 * sizeof(HCInteger))

typedef struct HCMapIterator {
    HCMapRef map;
    HCInteger index;
    HCRef object;
    HCRef key;
    HCByte state[HCMapIteratorStateSizeStatic];
} HCMapIterator;
extern const HCMapIterator HCMapIteratorInvalid;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCMapRef HCMapCreate(void);
HCMapRef HCMapCreateWithCapacity(HCInteger capacity);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCMapIsEqual(HCMapRef self, HCMapRef other);
HCInteger HCMapHashValue(HCMapRef self);
void HCMapPrint(HCMapRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Content
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCMapIsEmpty(HCMapRef self);
HCInteger HCMapCount(HCMapRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Searching
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCMapContainsKey(HCMapRef self, HCRef key);
HCRef HCMapFirstKey(HCMapRef self);
HCRef HCMapLastKey(HCMapRef self);
HCRef HCMapKeyAtIterationIndex(HCMapRef self, HCInteger index);
HCBoolean HCMapContainsObject(HCMapRef self, HCRef object);
HCRef HCMapFirstObject(HCMapRef self);
HCRef HCMapLastObject(HCMapRef self);
HCRef HCMapObjectAtIterationIndex(HCMapRef self, HCInteger index);
HCRef HCMapObjectForKey(HCMapRef self, HCRef key);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCMapClear(HCMapRef self);

void HCMapAddObjectForKey(HCMapRef self, HCRef key, HCRef object);
void HCMapRemoveObjectForKey(HCMapRef self, HCRef key);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCMapAddObjectReleasedForKey(HCMapRef self, HCRef key, HCRef object);
void HCMapAddObjectReleasedForKeyReleased(HCMapRef self, HCRef key, HCRef object);
HCRef HCMapRemoveObjectRetainedForKey(HCMapRef self, HCRef key);
HCRef HCMapRemoveObjectRetainedForKeyReleased(HCMapRef self, HCRef key);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Null-Terminated String Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCMapContainsCStringKey(HCMapRef self, const char* key);
HCRef HCMapObjectForCStringKey(HCMapRef self, const char* key);
void HCMapAddObjectForCStringKey(HCMapRef self, const char* key, HCRef object);
void HCMapRemoveObjectForCStringKey(HCMapRef self, const char* key);
void HCMapAddObjectReleasedForCStringKey(HCMapRef self, const char* key, HCRef object);
HCRef HCMapRemoveObjectRetainedForCStringKey(HCMapRef self, const char* key);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Iteration
//----------------------------------------------------------------------------------------------------------------------------------
HCMapIterator HCMapIterationBegin(HCMapRef self);
void HCMapIterationNext(HCMapIterator* iterator);
void HCMapIterationEnd(HCMapIterator* iterator);
HCBoolean HCMapIterationHasBegun(HCMapIterator* iterator);
HCBoolean HCMapIterationHasNext(HCMapIterator* iterator);
HCBoolean HCMapIterationHasEnded(HCMapIterator* iterator);

#endif /* HCMap_h */
