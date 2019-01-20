//
//  HCData.h
//  Test
//
//  Created by Matt Stoker on 1/19/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCData_h
#define HCData_h

#include "HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCDataType;
typedef struct HCData* HCDataRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCDataRef HCDataCreate(void);
HCDataRef HCDataCreateWithSize(HCInteger size);
HCDataRef HCDataCreateWithBytes(HCInteger size, HCByte* bytes);
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
HCInteger HCDataGetSize(HCDataRef self);
const HCByte* HCDataGetBytes(HCDataRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCDataAsBoolean(HCDataRef self);
HCInteger HCDataAsInteger(HCDataRef self);
HCReal HCDataAsReal(HCDataRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCDataClear(HCDataRef self);
void HCDataResize(HCDataRef self, HCInteger size);
void HCDataAppendBytes(HCDataRef self, HCInteger size, HCByte* bytes);
void HCDataAppendBoolean(HCDataRef self, HCBoolean value);
void HCDataAppendInteger(HCDataRef self, HCInteger value);
void HCDataAppendReal(HCDataRef self, HCReal value);

#endif /* HCData_h */
