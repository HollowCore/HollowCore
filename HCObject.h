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
#define HCObjectTypeName "HCObject"
typedef struct HCObject* HCObjectRef;
extern const struct HCType* HCObjectType;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Type System
//----------------------------------------------------------------------------------------------------------------------------------
typedef HCBoolean (*HCObjectIsEqualFunction)(HCObjectRef self, HCObjectRef other);
typedef HCInteger (*HCObjectHashValueFunction)(HCObjectRef self);
typedef void (*HCObjectPrintFunction)(HCObjectRef self, FILE* stream);
typedef void (*HCObjectDestoryFunction)(HCObjectRef self);

struct HCType {
    struct HCType* ancestor;
    HCTypeName name;
    HCObjectIsEqualFunction isEqual;
    HCObjectHashValueFunction hashValue;
    HCObjectPrintFunction print;
    HCObjectDestoryFunction destroy;
};
typedef struct HCType HCType;

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
