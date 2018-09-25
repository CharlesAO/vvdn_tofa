///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter-Leon : stub-skel crosses (chained, no buffers)
///

#include <Flic.h>
#include <FlicRmt.h>
#include <RmtBarr.h>
#include <DrvLeon.h>
#include "../shared/PlgEcho.h"
#if defined(MA2X8X)
#include <OsDrvDynIrq.h>
#endif

// LRT
extern RmtO<MyMsg> osStub[N];
extern RmtI<MyMsg> osSkel[N];

       RmtI<MyMsg> rtSkel[N];
       RmtO<MyMsg> rtStub[N];

       Pipeline    p;

//####################################################################
extern "C" void* POSIX_Init(void *args)
{
   UNUSED(args);

   #if defined(MA2X8X)
   //Requesting some irq resources before FlicRmt::Init,
   //so that the resouces allocated on the 2 Leons differ.
    uint32_t testDynIrq1;
    uint32_t testDynIrq2;
    assert(OK == OsDrvDynIrqRequest(&testDynIrq1));
    assert(OK == OsDrvDynIrqRequest(&testDynIrq2));
   #endif

   FlicRmt::Init();

   for(int i=0; i<N; i++)
   {
     rtStub[i].Create(&osSkel[i]);
     rtSkel[i].Create(&osStub[i]);
   }

   for(int i=0; i<N; i++){
     p.Add(&rtStub[i]);
     p.Add(&rtSkel[i]);
   }

   for(int i=0; i<N; i++){
     rtSkel[i].out.Link(&rtStub[i].in);
   }

   DrvLeonRTSignalBootCompleted();

   p.Start();
   sleep(60*60); //let test finish...

   DrvLeonRTSignalStop();

   #if defined(MA2X8X)
    assert(OK == OsDrvDynIrqRelease(testDynIrq1));
    assert(OK == OsDrvDynIrqRelease(testDynIrq2));
   #endif
   FlicRmt::Destroy();
   exit(0);
}
