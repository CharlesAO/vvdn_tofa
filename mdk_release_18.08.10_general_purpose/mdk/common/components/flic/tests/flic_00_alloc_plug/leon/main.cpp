///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Plugin with custom resources Alloc/Destroy cycle
///

#include <Flic.h>
#include <UnitTestApi.h>
#include "plug.h"
#include <MemCheck.h>

#define N_RUNS  (100*1000)

//globals for debug
uint32_t i;

//############################################
extern "C" void *POSIX_Init(void *)
{
    unitTestInit();

    printf("Please wait ...\n");
    for(i=0; i<N_RUNS; i++)
    {
        MyPlugin plg;
        plg.Create();
        if(i==0)      //dbg 1st iter
          ShowAllocs();
        plg.Stop  (); //destroy
        plg.Delete();
    }
    assert(0 == CheckMemLeaks(0));
    unitTestFinalReport();
    exit(0);
}
