///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     FLIC stress test: an 128 element histogram is send by 128 senders
///            to an unique receiver
///

#include <Flic.h>
#include <rtems/score/heap.h>
#include <rtems/libcsupport.h>
#include <UnitTestApi.h>
#include <DrvCpr.h>

#if defined(MA2450)
#include <OsDrvTimer.h>
#endif
#include "PlgEcho.h"

#define N_HIST      128 //num elems in histogram = num Sender threads
#define MUL_FACT    100 //hist values multiplication factor

uint32_t histIn [N_HIST] SECTION(".cmx_direct.bss");
uint32_t histOut[N_HIST] SECTION(".cmx_direct.bss");

PlgEchoSend  plgSend[N_HIST];
PlgEchoRecv  plgRecv;
Pipeline     p;

#if defined(MA2450)
tyTimeStamp  ticks;
u64          cycles;
#endif
//############################################
Heap_Information_block  wi;
extern Heap_Control _Workspace_Area; //cool stuff

void rtemsInfo()
{
   rtems_workspace_get_information(&wi);
   printf("FREE total = %u (num = %u)\n", wi.Free.total, wi.Free.number);
   printf("USED total = %u (num = %u)\n", wi.Used.total, wi.Used.number);

   printf("[WorkSpace]\n");
   printf("  .area_begin  @ 0x%x\n", (unsigned int)_Workspace_Area.area_begin);
   printf("  .area_end    @ 0x%x\n", (unsigned int)_Workspace_Area.area_end  );
}

//############################################
int rand(int m) {
  static int seed = 123456789;
  static int a    = 3;
  static int c    = 77;
  seed = (a * seed + c) % m;
  return seed;
}
//############################################
int genHist()
{
   int i;
   int acc = 0;
   for(i=0; i<N_HIST; i++){
     histIn[i] = rand(128) * MUL_FACT + 1;
     acc      += histIn[i];
   //printf("histIn[%d] = %ld\n", i, histIn[i]);
   }
   printf("Hist size = %d, acc = %d\n", N_HIST, acc);
   return acc;
}
//############################################
extern "C" void *POSIX_Init (void *)
{
    int i, hSz;
    unitTestInit();

    DrvCprInit();
    #if defined(MA2450)
    OsDrvTimerInit();
    printf("Sys freq = %ld KHz\n", DrvCprGetSysClockKhz());
    #endif

    rtemsInfo();
    hSz = genHist();
    memset(histOut, 0, sizeof(histOut));

    plgRecv.Create(hSz, histOut);
    for(i=0; i<N_HIST; i++)
      plgSend[i].Create(i, histIn[i]);

    p.Add(&plgRecv);
    for(i=0; i<N_HIST; i++)
       p.Add(&plgSend[i]);

    for(i=0; i<N_HIST; i++)
       plgSend[i].out.Link(&plgRecv.in);

    #if defined(MA2450)
    printf("Please wait ...\n");
    OsDrvTimerStartTicksCount(&ticks);
    #endif

    p.Start();
    p.Wait();

    #if defined(MA2450)
    OsDrvTimerGetElapsedTicks(&ticks, &cycles);
    printf("Cycles = 0x%08lx%08lx\n", (uint32_t)(cycles>>32), (uint32_t)cycles);
    #endif

    unitTestMemCompare(histOut, histIn, sizeof(histIn));
    unitTestFinalReport();
    exit(0);
}
