///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter-Leon : passing PoPtr<>
///

#include <Flic.h>
#include <FlicRmt.h>
#include <RmtBarr.h>
#include <DrvLeon.h>
#include "../shared/PlgEcho.h"

// LRT
       PlgB              plugB [N];
       RmtI<ImgFramePtr> skelA0[N];
extern RmtO<ImgFramePtr> stubA0[N];
       RmtI<ImgFramePtr> skelA1[N];
extern RmtO<ImgFramePtr> stubA1[N];
       RmtO<SyncPtrs>    stubC [N];
extern RmtI<SyncPtrs>    skelC [N];

       Pipeline    p;

extern RmtBarr     bLOS;
       RmtBarr     bLRT SECTION(".cmx_direct.bss");

//####################################################################
extern "C" void* POSIX_Init(void *args)
{
   unitTestInit();
   UNUSED(args);
   char name[16];
   FlicRmt::Init();

   for(int i=0; i<N; i++)
   {
     sprintf(name, "PlgB[%d]", i);

     skelA0[i].Create(&stubA0[i]);
     skelA1[i].Create(&stubA1[i]);
     stubC [i].Create(&skelC [i]);
     plugB [i].Create(N_MSG);

     p.Add(&plugB [i], name);
     p.Add(&skelA0[i]);
     p.Add(&skelA1[i]);
     p.Add(&stubC [i]);

     skelA0[i].out.Link(&plugB[i].in0);
     skelA1[i].out.Link(&plugB[i].in1);
     plugB [i].out.Link(&stubC[i].in );
   }
   bLRT.Create(&bLOS);

   DrvLeonRTSignalBootCompleted();

   p.Start();

   #if 0
    p.Wait(); //skel/stub never stop :(
   #else
    for(int i=0; i<N; i++)
      plugB[i].Join();
    unitTestFinalReport();
   //When plugB[] finished, test is ~ "over"
    bLRT.Wait();
   #endif

   FlicRmt::Destroy();
   exit(0);
}
