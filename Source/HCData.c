//
//  HCData.c
//  Test
//
//  Created by Matt Stoker on 1/19/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCData_Internal.h"
#include <string.h>
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCDataTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCData",
    },
    .isEqual = (void*)HCDataIsEqual,
    .hashValue = (void*)HCDataHashValue,
    .print = (void*)HCDataPrint,
    .destroy = (void*)HCDataDestroy,
};
HCType HCDataType = (HCType)&HCDataTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCDataRef HCDataCreate(void) {
    return HCDataCreateWithBoolean(false);
}

HCDataRef HCDataCreateWithBoolean(HCBoolean value) {
    HCDataRef self = calloc(sizeof(HCData), 1);
    HCDataInit(self, sizeof(value), (HCByte*)&value);
    return self;
}

HCDataRef HCDataCreateWithInteger(HCInteger value) {
    HCDataRef self = calloc(sizeof(HCData), 1);
    HCDataInit(self, sizeof(value), (HCByte*)&value);
    return self;
}

HCDataRef HCDataCreateWithReal(HCReal value) {
    HCDataRef self = calloc(sizeof(HCData), 1);
    HCDataInit(self, sizeof(value), (HCByte*)&value);
    return self;
}

void HCDataInit(void* memory, HCInteger size, HCByte* data) {
    HCByte* dataCopy = malloc(size);
    memcpy(dataCopy, data, size);
    // TODO: Check that the allocation and copy proceed successfully, determine how to pass the error otherwise
    
    HCObjectInit(memory);
    HCDataRef self = memory;
    self->base.type = HCDataType;
    self->size = size;
    self->data = dataCopy;
}

void HCDataDestroy(HCDataRef self) {
    free(self->data);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCDataIsEqual(HCDataRef self, HCDataRef other) {
    if (self->size != other->size) {
        return false;
    }
    return memcmp(self->data, other->data, self->size) == 0;
}

HCInteger HCDataHashValue(HCDataRef self) {
    HCInteger hash = 5381;
    for (HCInteger byteIndex = 0; byteIndex < self->size; byteIndex++) {
        HCByte b = self->data[byteIndex];
        hash = ((hash << 5) + hash) + b;
    }
    return hash;
}

void HCDataPrint(HCDataRef self, FILE* stream) {
    HCObjectPrint((HCObjectRef)self, stream);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCDataAsBoolean(HCDataRef self) {
    return self->size > sizeof(HCBoolean) ? false : *(HCBoolean*)self->data;
}

HCInteger HCDataAsInteger(HCDataRef self) {
    return self->size > sizeof(HCInteger) ? 0 : *(HCInteger*)self->data;
}

HCReal HCDataAsReal(HCDataRef self) {
    return self->size > sizeof(HCReal) ? NAN : *(HCReal*)self->data;
}
