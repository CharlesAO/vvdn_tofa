///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter-Leon : basic IRQ test
///

#include <DrvLeon.h>
#include "../shared/testConfig.h"
#include <FlicRmt.h>

extern sem_t lrt_waitCall;

//########################################################
extern "C" void* POSIX_Init(void *args)
{
   UNUSED(args);
   FlicRmt::Init();

  //Start LRT
   DrvLeonRTStartupStart();
   DrvLeonRTWaitForBoot ();

   for(int i = 0; i < INTERRUPTS_TO_GENERATE; i++)
   {
      RmiCtx ctx;
      ctx.semToWake = &lrt_waitCall;
      FlicRmt::RmiCall(&ctx); //waits for ACK
   }

    DrvLeonRTWaitExecution();
    printf("LOS finish __\n");
    exit(0);
}
