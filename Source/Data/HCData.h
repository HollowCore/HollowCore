//
//  HCData.h
//  Test
//
//  Created by Matt Stoker on 1/19/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCData_h
#define HCData_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCDataType;
typedef struct HCData* HCDataRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCDataRef HCDataCreate(void);
HCDataRef HCDataCreateWithBytes(HCInteger size, const HCByte* bytes);
HCDataRef HCDataCreateWithBoolean(HCBoolean value);
HCDataRef HCDataCreateWithInteger(HCInteger value);
HCDataRef HCDataCreateWithReal(HCReal value);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCDataIsEqual(HCDataRef self, HCDataRef other);
HCInteger HCDataHashValue(HCDataRef self);
void HCDataPrint(HCDataRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCDataIsEmpty(HCDataRef self);
HCInteger HCDataSize(HCDataRef self);
const HCByte* HCDataBytes(HCDataRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCDataIsBoolean(HCDataRef self);
HCBoolean HCDataAsBoolean(HCDataRef self);
HCBoolean HCDataIsInteger(HCDataRef self);
HCInteger HCDataAsInteger(HCDataRef self);
HCBoolean HCDataIsReal(HCDataRef self);
HCReal HCDataAsReal(HCDataRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCDataClear(HCDataRef self);
void HCDataAddBytes(HCDataRef self, HCInteger size, const HCByte* bytes);
void HCDataRemoveBytes(HCDataRef self, HCInteger size);
void HCDataAddBoolean(HCDataRef self, HCBoolean value);
void HCDataRemoveBoolean(HCDataRef self);
void HCDataAddInteger(HCDataRef self, HCInteger value);
void HCDataRemoveInteger(HCDataRef self);
void HCDataAddReal(HCDataRef self, HCReal value);
void HCDataRemoveReal(HCDataRef self);

#endif /* HCData_h */
