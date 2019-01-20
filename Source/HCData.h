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
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCDataAsBoolean(HCDataRef self);
HCInteger HCDataAsInteger(HCDataRef self);
HCReal HCDataAsReal(HCDataRef self);

#endif /* HCData_h */
