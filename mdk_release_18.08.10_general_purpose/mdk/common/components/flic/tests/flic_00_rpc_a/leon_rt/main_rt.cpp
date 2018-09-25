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
#include "../shared/testConfig.h"

// LRT
extern RmtBarr bLOS[N];
       RmtBarr bLRT[N] SECTION(".cmx_direct.bss");

//####################################################################
extern "C" void* POSIX_Init(void *args)
{
   UNUSED(args);
   FlicRmt::Init();

   for(int i=0; i<N; i++)
     bLRT[i].Create(&bLOS[i]);

   DrvLeonRTSignalBootCompleted();

   test_0(bLRT);
   test_1(bLRT);

   for(int i=0; i<N; i++)
    bLRT[i].Destroy();

   FlicRmt::Destroy();
   exit(0);
}
