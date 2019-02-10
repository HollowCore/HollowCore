//
//  HCNumber.h
//  HollowCore
//
//  Created by Matt Stoker on 12/28/18.
//  Copyright © 2018 HollowCore. All rights reserved.
//

#ifndef HCNumber_h
#define HCNumber_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCNumberType;
typedef struct HCNumber* HCNumberRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCNumberRef HCNumberCreate(void);
HCNumberRef HCNumberCreateWithBoolean(HCBoolean value);
HCNumberRef HCNumberCreateWithInteger(HCInteger value);
HCNumberRef HCNumberCreateWithReal(HCReal value);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCNumberIsEqual(HCNumberRef self, HCNumberRef other);
HCInteger HCNumberHashValue(HCNumberRef self);
void HCNumberPrint(HCNumberRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCNumberIsBoolean(HCNumberRef self);
HCBoolean HCNumberAsBoolean(HCNumberRef self);
HCBoolean HCNumberIsInteger(HCNumberRef self);
HCInteger HCNumberAsInteger(HCNumberRef self);
HCBoolean HCNumberIsReal(HCNumberRef self);
HCReal HCNumberAsReal(HCNumberRef self);

#endif /* HCNumber_h */
