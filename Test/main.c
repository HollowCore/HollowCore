//
//  main.c
//  Test
//
//  Created by Matt Stoker on 1/14/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include <stdio.h>

#define CTEST_MAIN
#define CTEST_SEGFAULT
//#define CTEST_NO_COLORS
//#define CTEST_COLOR_OK
#include "ctest.h"

int main(int argc, const char *argv[]) {
    return ctest_main(argc, argv);
}

