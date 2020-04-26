///
/// @file HCMap_Internal.c
/// @ingroup HollowCore
///
/// @author Braden Scothern
/// @date 6/25/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "ctest.h"
#include "../Source/HollowCore.h"
#include "../Source/Container/HCMap_Internal.h"

CTEST(HCMap_Internal, IteratorStateSize) {
    // Ensure that the state is large enough to contain the data intended to be stored in it.
    if ((HCInteger)HCMapIteratorStateSizeStatic < HCMapIteratorMinimumSizeRequiredForState()) {
        printf("\nCRITICAL ERROR: HCMapIteratorStateSizeStatic is smaller than is required by the implimentation of HCMapIterator logic!\n");
        ASSERT_FAIL();
    }
}
