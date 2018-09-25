///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter-Leon : passing PoPtr<> + return
///

#include <Flic.h>
#include <FlicRmt.h>
#include <RmtBarr.h>
#include <MemAllocator.h>
#include <DrvLeon.h>
#include <DrvCpr.h>
#include "../shared/PlgEcho.h"

// LOS
       Pool<ImgFrame> pool [N] SECTION(".cmx_direct.bss");
       PlgA<MyMsg>    plugA[N]; //source
       PlgC<MyMsg>    plugC[N]; //sink
       RmtO<MyMsg>    stubB[N]; //local stub
       RmtI<MyMsg>    skelC[N]; //local skel
extern RmtI<MyMsg>    skelB[N]; //remote skeleton on LRT
extern RmtO<MyMsg>    stubC[N]; //remote stub on LRT
       Pipeline       p;

extern RmtBarr        bLRT;
       RmtBarr        bLOS SECTION(".cmx_direct.bss");

//########################################################
extern "C" void* POSIX_Init(void *args)
{
   UNUSED(args);
   unitTestInit();
   DrvCprInit();
   printf("Sys freq = %ld KHz\n", DrvCprGetSysClockKhz());
   printf("Estimated duration : %s\n", SZ_ETC);

   RgnAlloc.Create(RgnBuff, DEF_POOL_SZ);
   FlicRmt::Init();

   for(int i=0; i<N; i++)
   {
     pool [i].Create(&RgnAlloc, N_POOL_FRMS, FRM_SZ, true);
     stubB[i].Create(&skelB[i]);
     skelC[i].Create(&stubC[i]);
     plugA[i].Create(N_MSG, SEED(i));
     plugC[i].Create(N_MSG, SEED(i));
     p.Add(&pool [i]);
     p.Add(&plugA[i]);
     p.Add(&plugC[i]);
     p.Add(&stubB[i]);
     p.Add(&skelC[i]);
     pool [i].out.Link(&plugA[i].in);
     plugA[i].out.Link(&stubB[i].in);
     skelC[i].out.Link(&plugC[i].in);
   }
   bLOS.Create(&bLRT);

  //Start LRT
   DrvLeonRTStartupStart();
   DrvLeonRTWaitForBoot ();

   p.Start();

  //Skel/stub never stop so can't just do "p.Wait()" in this test
  //waiting instead after relevant/active plugins.
  //In a practical app a p.Stop() would be invoked some user-interface block
   for(int i=0; i<N; i++)
    plugC[i].Join();
   bLOS.Wait(); //wait for LRT to hit the barrier

   for(int i=0; i<N; i++)
     unitTestAssert(plugC[i].err == 0);
   unitTestFinalReport();

   FlicRmt::Destroy();
   exit(0);
}
