///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter-Leon : passing 2xPoPtr<> then receiving a sync-msg with
///                         the two pointers
///

#include <Flic.h>
#include <FlicRmt.h>
#include <RmtBarr.h>
#include <MemAllocator.h>
#include <DrvLeon.h>
#include <DrvCpr.h>
#include "../shared/PlgEcho.h"

// LOS
       PlgA              plugA0[N]; //sends
       Pool<ImgFrame>    poolA0[N] SECTION(".cmx_direct.bss");
       RmtO<ImgFramePtr> stubA0[N]; //local stub
extern RmtI<ImgFramePtr> skelA0[N]; //remote skeleton on LRT

       PlgA              plugA1[N]; //sends
       Pool<ImgFrame>    poolA1[N] SECTION(".cmx_direct.bss");
       RmtO<ImgFramePtr> stubA1[N]; //local stub
extern RmtI<ImgFramePtr> skelA1[N]; //remote skeleton on LRT

       PlgC              plugC [N]; //sink
       RmtI<SyncPtrs>    skelC [N];
extern RmtO<SyncPtrs>    stubC [N];

       Pipeline       p;

extern RmtBarr        bLRT;
       RmtBarr        bLOS SECTION(".cmx_direct.bss");

//########################################################
extern "C" void* POSIX_Init(void *args)
{
   UNUSED(args);
   DrvCprInit();
   printf("Sys freq = %ld KHz\n", DrvCprGetSysClockKhz());
   printf("Estimated duration : %s\n", SZ_ETC);

   RgnAlloc.Create(RgnBuff, DEF_POOL_SZ);
   FlicRmt::Init();

   for(int i=0; i<N; i++)
   {
     poolA0[i].Create(&RgnAlloc, N_POOL_FRMS, FRM_SZ, true);
     stubA0[i].Create(&skelA0[i]);
     plugA0[i].Create(N_MSG, SEED(i));
     p.Add(&poolA0[i]);
     p.Add(&plugA0[i]);
     p.Add(&stubA0[i]);
     poolA0[i].out.Link(&plugA0[i].in);
     plugA0[i].out.Link(&stubA0[i].in);

     poolA1[i].Create(&RgnAlloc, N_POOL_FRMS, FRM_SZ, true);
     stubA1[i].Create(&skelA1[i]);
     plugA1[i].Create(N_MSG, SEED(i));
     p.Add(&poolA1[i]);
     p.Add(&plugA1[i]);
     p.Add(&stubA1[i]);
     poolA1[i].out.Link(&plugA1[i].in);
     plugA1[i].out.Link(&stubA1[i].in);

     plugC[i].Create(N_MSG, SEED(i));
     skelC[i].Create(&stubC [i]);
     p.Add(&plugC[i]);
     p.Add(&skelC[i]);
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
   for(int i=0; i<N; i++){
     plugC[i].Join();
   }
   bLOS.Wait(); //wait for LRT to hit the barrier

   FlicRmt::Destroy();
   exit(0);
}
