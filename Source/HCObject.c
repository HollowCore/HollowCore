//
//  HCObject.c
//  HollowCore
//
//  Created by Matt Stoker on 12/28/18.
//  Copyright © 2018 HollowCore. All rights reserved.
//

#include "HCObject_Internal.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Management
//----------------------------------------------------------------------------------------------------------------------------------
HCRef HCRetain(HCRef self) {
    // TODO: Atomic
    ((HCObjectRef)self)->referenceCount++;
    return self;
}

void HCRelease(HCRef self) {
    // TODO: Atomic
    ((HCObjectRef)self)->referenceCount--;
    if (((HCObjectRef)self)->referenceCount <= 0) {
        for (HCType type = ((HCObjectRef)self)->type; type != NULL; type = type->ancestor) {
            ((HCObjectTypeData*)((HCObjectRef)self)->type)->destroy(self);
        }
        free(self);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCIsEqual(HCRef self, HCRef other) {
    return ((HCObjectTypeData*)((HCObjectRef)self)->type)->isEqual(self, other);
}

HCInteger HCHashValue(HCRef self) {
    return ((HCObjectTypeData*)((HCObjectRef)self)->type)->hashValue(self);
}

void HCPrint(HCRef self, FILE* stream) {
    ((HCObjectTypeData*)((HCObjectRef)self)->type)->print(self, stream);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
HCObjectTypeData HCObjectTypeDataInstance = {
    .base = {
        .ancestor = NULL,
        .name = "HCObject",
    },
    .isEqual = (void*)HCObjectIsEqual,
    .hashValue = (void*)HCObjectHashValue,
    .print = (void*)HCObjectPrint,
    .destroy = (void*)HCObjectDestroy,
};
HCType HCObjectType = &HCObjectTypeDataInstance.base;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCObjectInit(void* memory) {
    HCObjectRef self = memory;
    self->type = HCObjectType;
}

void HCObjectDestroy(HCObjectRef self) {
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCObjectIsEqual(HCObjectRef self, HCObjectRef other) {
    return false;
}

HCInteger HCObjectHashValue(HCObjectRef self) {
    return 0;
}

void HCObjectPrint(HCObjectRef self, FILE* stream) {
    printf("<%s> %p", self->type->name, self);
}
