//
//  HCThread.h
//  HollowCore
//
//  Created by Braden Scothern on 2/14/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCThread_h
#define HCThread_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCThreadType;
typedef struct HCThread* HCThreadRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef void (*HCThreadFunction)(void* context);

typedef enum HCThreadOption {
    HCThreadOptionNone                      = 0b0,

    // Exclusive from HCThreadOptionFreeContextOnDestroy
    HCThreadOptionReleaseContextOnDestroy   = 0b1,

    // Exclusive from HCThreadOptionReleaseContextOnDestroy
    HCThreadOptionFreeContextOnDestroy      = 0b10,

    HCThreadOptionCancelOnDestroy           = 0b100,
    HCThreadOptionJoinOnDestroy             = 0b1000,

    // TODO: Should this option exist? It is dangerous both ways...
    HCThreadOptionNoRetainOfSelfOnStart     = 0b10000,

} HCThreadOption;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCThreadRef HCThreadCreate(HCThreadFunction function, void* context);
HCThreadRef HCThreadCreateWithOptions(HCThreadFunction function, void* context, HCThreadOption options);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCThreadIsEqual(HCThreadRef self, HCThreadRef other);
HCInteger HCThreadHashValue(HCThreadRef self);
void HCThreadPrint(HCThreadRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Execution State
//----------------------------------------------------------------------------------------------------------------------------------
void HCThreadStart(HCThreadRef self);
void HCThreadCancel(HCThreadRef self);
HCBoolean HCThreadIsExecuting(HCThreadRef self);
HCBoolean HCThreadIsFinished(HCThreadRef self);
HCBoolean HCThreadIsCanceled(HCThreadRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Management
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCThreadIsJoined(HCThreadRef self);
void HCThreadJoin(HCThreadRef self);
void* HCThreadGetContext(HCThreadRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Options
//----------------------------------------------------------------------------------------------------------------------------------
HCThreadOption HCThreadGetOptions(HCThreadRef self);
void HCThreadSetOptions(HCThreadRef self, HCThreadOption options);
void HCThreadUnsetOptions(HCThreadRef self, HCThreadOption options);
void HCThreadClearOptions(HCThreadRef self);

#endif /* HCThread_h */
