///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter-Leon : waiting barrier
///

#include <Flic.h>
#include <FlicRmt.h>
#include <RmtBarr.h>
#include <DrvLeon.h>
#include <DrvCpr.h>
#include <UnitTestApi.h>
#include "../shared/testConfig.h"

// LOS
extern RmtBarr bLRT[N];
       RmtBarr bLOS[N] SECTION(".cmx_direct.bss");

//########################################################

//########################################################
extern "C" void* POSIX_Init(void *args)
{
   UNUSED(args);
   DrvCprInit();
   unitTestInit();
   printf("Sys freq = %ld KHz\n", DrvCprGetSysClockKhz());

   FlicRmt::Init();

   for(int i=0; i<N; i++)
      bLOS[i].Create(&bLRT[i]);

  //Start LRT
   DrvLeonRTStartupStart();
   DrvLeonRTWaitForBoot ();

   test_0(bLOS);
   for(int i=0; i<N; i++)
    unitTestAssert(bLOS[i].hitCnt == RUNS);

   test_1(bLOS);
   for(int i=0; i<N; i++)
    unitTestAssert(bLOS[i].hitCnt == RUNS*2);


   for(int i=0; i<N; i++)
    bLOS[i].Destroy();

   unitTestFinalReport();
   FlicRmt::Destroy();
   exit(0);
}
