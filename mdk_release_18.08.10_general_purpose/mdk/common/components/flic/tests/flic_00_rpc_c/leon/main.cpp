///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter-Leon : stub-skel crosses (chained)
///

#include <Flic.h>
#include <FlicRmt.h>
#include <DrvLeon.h>
#include <DrvCpr.h>
#include <UnitTestApi.h>
#include "../shared/PlgEcho.h"
#if !defined(MA2X8X)
#include <OsDrvTimer.h>
#endif

// LOS
PlgA<MyMsg> plugA;     //source
PlgB<MyMsg> plugB[N];  //buffer just N-1 are needed, but avoiding compile error when N = 1
PlgC<MyMsg> plugC;     //sink

RmtO<MyMsg> osStub[N]; //local stub
RmtI<MyMsg> osSkel[N]; //local stub

extern RmtI<MyMsg> rtSkel[N];
extern RmtO<MyMsg> rtStub[N];

       Pipeline    p;

#if !defined(MA2X8X)
tyTimeStamp  ticks;
u64          cycles;
#endif

//########################################################
extern "C" void* POSIX_Init(void *args)
{
   UNUSED(args);
   unitTestInit();

   DrvCprInit();
  #if !defined(MA2X8X)
   OsDrvTimerInit();
  #endif
   printf("Sys freq = %ld KHz\n", DrvCprGetSysClockKhz());
   printf("Estimated duration : %s\n", SZ_ETC);
   printf("sizeof(MyMsg)      = %d\n", sizeof(MyMsg));

   FlicRmt::Init();

  //Create
   plugA.Create(N_MSG);
   plugC.Create(N_MSG);
   for(int i=0; i<N-1; i++)
     plugB [i].Create(N_MSG);
   for(int i=0; i<N; i++){
     osStub[i].Create(&rtSkel[i]);
     osSkel[i].Create(&rtStub[i]);
   }

  //Add plugs to pipe
   p.Add(&plugA);
   p.Add(&plugC);
   for(int i=0; i<N-1; i++) p.Add(&plugB [i]);
   for(int i=0; i<N  ; i++) p.Add(&osStub[i]);
   for(int i=0; i<N  ; i++) p.Add(&osSkel[i]);

  //Links
   plugA      .out.Link(&osStub[0].in);
   osSkel[N-1].out.Link(&plugC    .in);
   for(int i=0; i<N-1; i++)
   {
      osSkel[i].out.Link(&plugB [i  ].in);
      plugB [i].out.Link(&osStub[i+1].in);
   }

  //Start LRT
   DrvLeonRTStartupStart();
   DrvLeonRTWaitForBoot ();

  #if !defined(MA2X8X)
   OsDrvTimerStartTicksCount(&ticks);
  #endif

   p.Start();
   plugC.Join();
   unitTestAssert(plugC.err == 0);

  #if !defined(MA2X8X)
   OsDrvTimerGetElapsedTicks(&ticks, &cycles);
   if(PRINT == 0){ //if debug print was disabled, profiling is relevant
     printf("Cycles = %ld\n", (uint32_t)(cycles/N_MSG));
   }
  #endif

   DrvLeonRTWaitExecution();
   unitTestFinalReport();
   FlicRmt::Destroy();
   exit(0);
}
