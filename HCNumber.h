//
//  HCNumber.h
//  HollowCore
//
//  Created by Matt Stoker on 12/28/18.
//  Copyright © 2018 HollowCore. All rights reserved.
//

#ifndef HCNumber_h
#define HCNumber_h

#include "HCObject.h"

#define HCNumberTypeName "HCNumber"
typedef struct HCNumber* HCNumberRef;
extern const HCType HCNumberType;

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
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCNumberGetBoolean(HCNumberRef self);
HCInteger HCNumberGetInteger(HCNumberRef self);
HCReal HCNumberGetReal(HCNumberRef self);

#endif /* HCNumber_h */
