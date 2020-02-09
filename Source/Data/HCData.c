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
HCDataRef HCDataCreate() {
    return HCDataCreateWithBytes(0, NULL);
}

HCDataRef HCDataCreateWithBytes(HCInteger size, const HCByte* bytes) {
    HCDataRef self = calloc(sizeof(HCData), 1);
    HCDataInit(self, size, bytes);
    return self;
}

HCDataRef HCDataCreateWithBoolean(HCBoolean value) {
    return HCDataCreateWithBytes(sizeof(value), (HCByte*)&value);
}

HCDataRef HCDataCreateWithInteger(HCInteger value) {
    return HCDataCreateWithBytes(sizeof(value), (HCByte*)&value);
}

HCDataRef HCDataCreateWithReal(HCReal value) {
    return HCDataCreateWithBytes(sizeof(value), (HCByte*)&value);
}

void HCDataInit(void* memory, HCInteger size, const HCByte* data) {
    HCByte* dataCopy = malloc(size);
    if (data != NULL) {
        memcpy(dataCopy, data, size);
    }
    HCDataInitWithoutCopying(memory, size, dataCopy);
}

void HCDataInitWithoutCopying(void* memory, HCInteger size, HCByte* data) {
    HCObjectInit(memory);
    HCDataRef self = memory;
    self->base.type = HCDataType;
    self->size = size;
    self->data = data;
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
    fprintf(stream, "<%s@%p,size:%li>", self->base.type->name, self, (long)self->size);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCDataIsEmpty(HCDataRef self) {
    return HCDataSize(self) == 0;
}

HCInteger HCDataSize(HCDataRef self) {
    return self->size;
}

const HCByte* HCDataBytes(HCDataRef self) {
    return self->data;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCDataIsBoolean(HCDataRef self) {
    return self->size == (HCInteger)sizeof(HCBoolean) && (self->data[0] == 0 || self->data[0] == 1);
}

HCBoolean HCDataAsBoolean(HCDataRef self) {
    return self->size > (HCInteger)sizeof(HCBoolean) ? false : *(HCBoolean*)self->data;
}

HCBoolean HCDataIsInteger(HCDataRef self) {
    return self->size == (HCInteger)sizeof(HCInteger);
}

HCInteger HCDataAsInteger(HCDataRef self) {
    return self->size > (HCInteger)sizeof(HCInteger) ? 0 : *(HCInteger*)self->data;
}

HCBoolean HCDataIsReal(HCDataRef self) {
    return self->size == (HCInteger)sizeof(HCReal) && !isnan(*(HCReal*)self->data);
}

HCReal HCDataAsReal(HCDataRef self) {
    return self->size > (HCInteger)sizeof(HCReal) ? NAN : *(HCReal*)self->data;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCDataClear(HCDataRef self) {
    HCDataRemoveBytes(self, self->size);
}

void HCDataAddBytes(HCDataRef self, HCInteger size, const HCByte* bytes) {
    self->data = realloc(self->data, self->size + size);
    if (bytes != NULL) {
        memcpy(self->data + self->size, bytes, size);
    }
    self->size += size;
    // TODO: Failable
}

void HCDataRemoveBytes(HCDataRef self, HCInteger size) {
    if (size <= 0) {
        return;
    }
    size = size > self->size ? 0 : self->size - size;
    self->data = realloc(self->data, size);
    self->size = size;
    // TODO: Failable
}

void HCDataAddBoolean(HCDataRef self, HCBoolean value) {
    HCDataAddBytes(self, sizeof(HCBoolean), (HCByte*)&value);
}

void HCDataRemoveBoolean(HCDataRef self) {
    HCDataRemoveBytes(self, sizeof(HCBoolean));
}

void HCDataAddInteger(HCDataRef self, HCInteger value) {
    HCDataAddBytes(self, sizeof(HCInteger), (HCByte*)&value);
}

void HCDataRemoveInteger(HCDataRef self) {
    HCDataRemoveBytes(self, sizeof(HCInteger));
}

void HCDataAddReal(HCDataRef self, HCReal value) {
    HCDataAddBytes(self, sizeof(HCReal), (HCByte*)&value);
}

void HCDataRemoveReal(HCDataRef self) {
    HCDataRemoveBytes(self, sizeof(HCReal));
}
