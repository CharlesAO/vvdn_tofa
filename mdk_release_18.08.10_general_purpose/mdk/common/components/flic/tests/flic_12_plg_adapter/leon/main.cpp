///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     PlgAdapter test suite
///

#include <Flic.h>
#include <UnitTestApi.h>

extern void test1(int n);
extern void test2(int n);

//#########################################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

    test1(10);
    test2(10);

    unitTestFinalReport();
    exit(0);
}
