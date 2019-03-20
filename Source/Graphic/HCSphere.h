//
//  HCSphere.h
//  HollowCore
//
//  Created by Matt Stoker on 3/5/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCSphere_h
#define HCSphere_h

#include "HCPrimitive.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCSphereType;
typedef struct HCSphere* HCSphereRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCSphereRef HCSphereCreate(HCVector center, HCReal radius);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCSphereIsEqual(HCSphereRef self, HCSphereRef other);
HCInteger HCSphereHashValue(HCSphereRef self);
void HCSpherePrint(HCSphereRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Primitive Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCReal HCSphereIntersect(HCSphereRef self, HCRay ray);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCVector HCSphereCenter(HCSphereRef self);
HCReal HCSphereRadius(HCSphereRef self);

#endif /* HCSphere_h */
