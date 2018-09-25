///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter-Leon : multiple stub-skel chains
///

#include <Flic.h>
#include <FlicRmt.h>
#include <RmtBarr.h>
#include <DrvLeon.h>
#include "../shared/PlgEcho.h"

// LRT
       PlgB<MyMsg> plugB[N];
       RmtI<MyMsg> skelB[N]; //skeleton for plgB
extern RmtO<MyMsg> stubA[N]; //remote stub on LOS
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
     plugB[i].Create(N_MSG, SEED(i), i*DELAY);
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
   //When plugB[] finished, test is over:
    for(int i=0; i<N; i++)
      unitTestAssert(plugB[i].err == 0);
    unitTestFinalReport();
    bLRT.Wait();
   #endif

   FlicRmt::Destroy();
   exit(0);
}
