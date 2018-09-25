///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     new/delete with file info (due to Flic.h inclusion)
///

#include <Flic.h>

void test3()
{
    int *ii = new int;
    *ii = 60;
    delete ii;
}