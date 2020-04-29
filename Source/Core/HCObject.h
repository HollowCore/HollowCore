///
/// @file HCObject.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 12/28/18
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

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
// MARK: - Object Type Query
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCTypeHasAncestor(HCType type, HCType other);
HCBoolean HCTypeIsOfType(HCType type, HCType other);
HCBoolean HCTypeIsOfKind(HCType type, HCType other);
HCTypeName HCObjectName(HCRef object);
HCType HCObjectAncestor(HCRef object);
HCBoolean HCObjectHasAncestor(HCRef object, HCType type);
HCBoolean HCObjectIsOfType(HCRef object, HCType type);
HCBoolean HCObjectIsOfKind(HCRef object, HCType type);

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
