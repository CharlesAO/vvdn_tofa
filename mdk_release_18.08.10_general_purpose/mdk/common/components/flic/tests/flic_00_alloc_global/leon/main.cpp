///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Print allocs
///

#include <Flic.h>
#include <UnitTestApi.h>
#include <MemCheck.h>

extern void test1();
extern void test2();
extern void test3();

//#########################################
extern "C" void *POSIX_Init (void *args)
{
    UNUSED(args);
    unitTestInit();

    int *ioio = new int;
    *ioio = 4;
    delete ioio;

    test1();
    test2();
    test3();

    unitTestAssert(OK == CheckMemLeaks(1));
    unitTestAssert(OK == CheckOverflow(1));

  //overflow
    int *tmp = new int[4];
    tmp[4]   = 3; //cause canary overwrite
    unitTestAssert(OK != CheckOverflow(1));
    delete tmp;

    unitTestFinalReport();
    exit(0);
}