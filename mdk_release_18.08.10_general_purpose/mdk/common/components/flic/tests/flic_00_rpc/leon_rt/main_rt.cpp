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

sem_t waitCall;

//####################################################################
extern "C" void* POSIX_Init(void *args)
{
   UNUSED(args);

   FlicRmt::Init();
   sem_init(&waitCall, 0, 0);
   DrvLeonRTSignalBootCompleted();

   for(int i=0; i<INTERRUPTS_TO_GENERATE; i++)
   {
      sem_wait(&waitCall);
      printf("[LRT] %d\n", i);
      FlicRmt::RmiAck();
   }

   DrvLeonRTSignalStop();
   printf("LRT finish __\n");
   exit(0);
}
