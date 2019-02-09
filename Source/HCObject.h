//
//  HCObject.h
//  HollowCore
//
//  Created by Matt Stoker on 12/28/18.
//  Copyright © 2018 HollowCore. All rights reserved.
//

#ifndef HCObject_h
#define HCObject_h

#include "HCCore.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef HCBoolean (*HCObjectIsEqualFunction)(HCRef self, HCRef other);
typedef HCInteger (*HCObjectHashValueFunction)(HCRef self);
typedef void (*HCObjectPrintFunction)(HCRef self, FILE* stream);
typedef void (*HCObjectDestoryFunction)(HCRef self);

typedef const struct HCObjectTypeData {
    HCTypeData base;
    HCObjectIsEqualFunction isEqual;
    HCObjectHashValueFunction hashValue;
    HCObjectPrintFunction print;
    HCObjectDestoryFunction destroy;
} HCObjectTypeData;
extern HCType HCObjectType;

typedef struct HCObject* HCObjectRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Management
//----------------------------------------------------------------------------------------------------------------------------------
HCRef HCRetain(HCRef self);
void HCRelease(HCRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCIsEqual(HCRef self, HCRef other);
HCInteger HCHashValue(HCRef self);
void HCPrint(HCRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
// NOTE: Instances of HCObject should be created using sub-types

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCObjectIsEqual(HCObjectRef self, HCObjectRef other);
HCInteger HCObjectHashValue(HCObjectRef self);
void HCObjectPrint(HCObjectRef self, FILE* stream);

#endif /* HCObject_h */
