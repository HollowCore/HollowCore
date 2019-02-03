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
    // Retain on the null reference is a no-op
    if (self == NULL) {
        return NULL;
    }
    
    // TODO: Atomic
    ((HCObjectRef)self)->referenceCount++;
    return self;
}

void HCRelease(HCRef self) {
    // Release on the null reference is a no-op
    if (self == NULL) {
        return;
    }
    
    // TODO: Atomic
    ((HCObjectRef)self)->referenceCount--;
    if (((HCObjectRef)self)->referenceCount <= 0) {
        for (HCType type = ((HCObjectRef)self)->type; type != NULL; type = type->ancestor) {
            ((HCObjectTypeData*)type)->destroy(self);
        }
        free(self);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCIsEqual(HCRef self, HCRef other) {
//    TODO: Find the highest common ancestor to perform the equality check
    return self != NULL && other != NULL && ((HCObjectTypeData*)((HCObjectRef)self)->type)->isEqual(self, other);
}

HCInteger HCHashValue(HCRef self) {
    return self == NULL ? 0 : ((HCObjectTypeData*)((HCObjectRef)self)->type)->hashValue(self);
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
    self->referenceCount = 1;
}

void HCObjectDestroy(HCObjectRef self) {
    (void)self; // Unused
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCObjectIsEqual(HCObjectRef self, HCObjectRef other) {
    return self == other;
}

HCInteger HCObjectHashValue(HCObjectRef self) {
    return (HCInteger)self;
}

void HCObjectPrint(HCObjectRef self, FILE* stream) {
    fprintf(stream, "<%s@%p>", self->type->name, self);
}
