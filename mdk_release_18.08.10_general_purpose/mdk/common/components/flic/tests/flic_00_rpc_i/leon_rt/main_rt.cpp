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
       PlgB              plugB[N];
       RmtI<ImgFramePtr> skelB[N]; //skeleton for plgB
extern RmtO<ImgFramePtr> stubA[N]; //remote stub on LOS
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
     skelB[i].Create(&stubA[i]);
     plugB[i].Create(N_MSG, DELAY+i*80);
     p.Add(&plugB[i], name);
     p.Add(&skelB[i]);
     skelB[i].out.Link(&plugB[i].in);
   }
   bLRT.Create(&bLOS);

   DrvLeonRTSignalBootCompleted();

   p.Start();

   #if 0
    p.Wait(); //skel/stub never stop :(
   #else
    for(int i=0; i<N; i++)
      plugB[i].Join();

    for(uint32_t i=0; i<N; i++){
      printf("plugB[%ld] : got %ld out of %d\n", i, plugB[i].nRec, N_MSG);
      unitTestAssert(plugB[i].err == 0);
    }
    unitTestFinalReport();

   //When plugB[] finished, test is ~over:
      bLRT.Wait();
   #endif

   FlicRmt::Destroy();
   exit(0);
}
