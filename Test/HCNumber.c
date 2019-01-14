//
//  HCNumber.c
//  HollowCore
//
//  Created by Matt Stoker on 1/14/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HCNumber.h"

CTEST(HCNumber, Creation) {
    HCNumberRef number = HCNumberCreate();
    ASSERT_FALSE(HCNumberGetBoolean(number));
    HCRelease(number);
}
