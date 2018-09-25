///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter-Leon : stub-skel crosses (chained)
///

#include <Flic.h>
#include <FlicRmt.h>
#include <RmtBarr.h>
#include <DrvLeon.h>
#include "../shared/PlgEcho.h"

// LRT
       PlgB<MyMsg> plugB [N];  //buffer

extern RmtO<MyMsg> osStub[N];
extern RmtI<MyMsg> osSkel[N];

       RmtI<MyMsg> rtSkel[N];
       RmtO<MyMsg> rtStub[N];

       Pipeline    p;

//####################################################################
extern "C" void* POSIX_Init(void *args)
{
   UNUSED(args);

   FlicRmt::Init();

   for(int i=0; i<N; i++)
   {
     plugB [i].Create(N_MSG);
     rtStub[i].Create(&osSkel[i]);
     rtSkel[i].Create(&osStub[i]);
   }

   for(int i=0; i<N; i++){
     p.Add(&plugB [i]);
     p.Add(&rtStub[i]);
     p.Add(&rtSkel[i]);
   }

   for(int i=0; i<N; i++){
     rtSkel[i].out.Link(&plugB[i].in);
     plugB [i].out.Link(&rtStub[i].in);
   }

   DrvLeonRTSignalBootCompleted();

   p.Start();
   plugB[N-1].Join();
   sleep(1); //???

   DrvLeonRTSignalStop();
   FlicRmt::Destroy();
   exit(0);
}
