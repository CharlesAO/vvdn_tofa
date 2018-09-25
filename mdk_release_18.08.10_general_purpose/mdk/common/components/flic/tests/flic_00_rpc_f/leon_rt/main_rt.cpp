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
#include <DrvLeon.h>
#include "../shared/PlgEcho.h"

// LRT
       PlgB<MyMsg> plugB[N];
       RmtI<MyMsg> skelB[N]; //skeleton for plgB
       RmtO<MyMsg> stubC[N]; //
extern RmtO<MyMsg> stubB[N]; //remote on LOS
extern RmtI<MyMsg> skelC[N]; //remote on LOS
       Pipeline    p;

extern RmtBarr     bLOS;
       RmtBarr     bLRT SECTION(".cmx_direct.bss");

//####################################################################
extern "C" void* POSIX_Init(void *args)
{
   UNUSED(args);
   FlicRmt::Init();

   for(int i=0; i<N; i++)
   {
     skelB[i].Create(&stubB[i]);
     stubC[i].Create(&skelC[i]);
     plugB[i].Create(N_MSG);
     p.Add(&plugB[i]);
     p.Add(&skelB[i]);
     p.Add(&stubC[i]);
     skelB[i].out.Link(&plugB[i].in);
     plugB[i].out.Link(&stubC[i].in);
   }
   bLRT.Create(&bLOS);

   DrvLeonRTSignalBootCompleted();

   p.Start();

   #if 0
    p.Wait(); //skel/stub never stop :(
   #else
    for(int i=0; i<N; i++)
      plugB[i].Join();
   //LRT reaches barrier:
    bLRT.Wait();
   #endif

   FlicRmt::Destroy();
   exit(0);
}
