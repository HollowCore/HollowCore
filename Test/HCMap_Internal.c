//
//  HCMap_Internal.c
//  Test
//
//  Created by Braden Scothern on 6/25/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"
#include "../Source/Container/HCMap_Internal.h"

CTEST(HCMap, IteratorStateSize) {
    // Ensure that the state is large enough to contain the data intended to be stored in it.
    if (HCMapIteratorStateSizeStatic < _HCMapIteratorMinimumSizeRequiredForState()) {
        printf("\nCRITICAL ERROR: HCMapIteratorStateSizeStatic is smaller than is required by the implimentation of HCMapIterator logic!\n");
        ASSERT_FAIL();
    }
}
