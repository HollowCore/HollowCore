//
//  HCString.c
//  Test
//
//  Created by Matt Stoker on 1/17/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCString.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCStringRef HCStringCreate(void);
HCStringRef HCStringCreateWithBoolean(HCBoolean value);
HCStringRef HCStringCreateWithInteger(HCInteger value);
HCStringRef HCStringCreateWithReal(HCReal value);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCStringIsEqual(HCStringRef self, HCStringRef other);
HCInteger HCStringHashValue(HCStringRef self);
void HCStringPrint(HCStringRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCStringGetCodeUnitCount(HCStringRef self);
HCStringCodeUnit HCStringGetCodeUnit(HCStringRef self, HCInteger index);
HCInteger HCStringGetCodePointCount(HCStringRef self);
HCStringCodePoint HCStringGetCodePoint(HCStringRef self, HCInteger index);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCStringCodePoint HCStringConvertCodeUnits(HCStringRef self, HCInteger codeUnitIndex, HCInteger* nextCodeUnitIndex);
