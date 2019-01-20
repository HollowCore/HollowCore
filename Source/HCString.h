//
//  HCString.h
//  Test
//
//  Created by Matt Stoker on 1/17/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCString_h
#define HCString_h

#include "HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCStringType;
typedef struct HCString* HCStringRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef uint8_t HCStringCodeUnit;
typedef uint32_t HCStringCodePoint;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCStringRef HCStringCreate(void);
HCStringRef HCStringCreateWithBoolean(HCBoolean value);
HCStringRef HCStringCreateWithInteger(HCInteger value);
HCStringRef HCStringCreateWithReal(HCReal value);
HCStringRef HCStringCreateWithCString(const char* value);
HCStringRef HCStringCreateWithBytesUTF8(const char* value);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCStringIsEqual(HCStringRef self, HCStringRef other);
HCInteger HCStringHashValue(HCStringRef self);
void HCStringPrint(HCStringRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCStringIsEmpty(HCStringRef self);
HCInteger HCStringGetCodeUnitCount(HCStringRef self);
HCStringCodeUnit HCStringGetCodeUnit(HCStringRef self, HCInteger codeUnitIndex);
HCInteger HCStringGetCodePointCount(HCStringRef self);
HCStringCodePoint HCStringGetCodePoint(HCStringRef self, HCInteger codePointIndex);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCStringAsBoolean(HCStringRef self);
HCInteger HCStringAsInteger(HCStringRef self);
HCReal HCStringAsReal(HCStringRef self);
const char* HCStringAsCString(HCStringRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Comparison
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCStringIsEqualToCString(HCStringRef self, const char* string);

#endif /* HCString_h */
