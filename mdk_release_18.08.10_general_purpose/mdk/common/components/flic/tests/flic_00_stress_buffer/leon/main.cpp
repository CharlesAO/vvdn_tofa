///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     FLIC stress test: single Buffer written by N x Writes
///            and read by N x Readers
///

#include <Flic.h>
#include <UnitTestApi.h>
#include <DrvCpr.h>
#include <MsgBuf.h>
#include <BehQueue.h>
#if defined(MA2450)
#include <OsDrvTimer.h>
#endif


#define M_FACT    1000  //multiplication factor
#define N_THR       64  //num Writers = num Readers
#define B_SZ       128  //buffer size

//Custom message
struct MyMsg{
  uint32_t data;
};

//============================================
//The unique buffer under test
MsgBuf<MyMsg> gBuff;
//============================================

uint32_t  iHist [N_THR]; //input hist
uint32_t  oHist [N_THR]; //otutput hist
uint32_t  order [N_THR]; //0,1,...
pthread_t wrThrd[N_THR];
pthread_t rdThrd[N_THR];

#if defined(MA2450)
tyTimeStamp  ticks;
u64          cycles;
#endif
//#############################################################
int rand(int m) {
  static int seed = 123456789;
  static int a    = 3;
  static int c    = 77;
  seed = (a * seed + c) % m;
  return seed;
}

//=============================================================
int genHist()
{
   int i;
   int acc = 0;
   for(i=0; i<N_THR; i++){
     order[i] = i;
     iHist[i] = rand(N_THR) * M_FACT + 1;
     acc     += iHist[i];
   //printf("histIn[%d] = %ld\n", i, histIn[i]);
   }
   printf("Hist size = %d, acc = %d\n", N_THR, acc);
   return acc;
}

//#############################################################
//Each writer sends iHist[arg] elements to buffer
void *funWr(void *arg)
{
    MyMsg m;
    uint32_t hPos = *((uint32_t*)arg);
    m.data = hPos;
    for(uint32_t i=0; i<iHist[hPos]; i++)
      gBuff.Push(NULL, &m);

    return NULL;
}
//=============================================================
//Each reader gets iHist[N_THR-1-hPos] elements from buffer
void *funRd(void *arg)
{
    MyMsg m;
    Atomic a;
    uint32_t hPos = *((uint32_t*)arg);
    for(uint32_t i=0; i<iHist[N_THR-1-hPos]; i++)
    {
      gBuff.Pop(NULL, &m);

    //Update output histogram
      a.Enter();
        oHist[m.data]++;
      a.Leave();
    }
    return NULL;
}

//#############################################################
extern "C" void *POSIX_Init (void *)
{
    uint32_t i;
    int sc;
    pthread_attr_t attr;

    unitTestInit();
    DrvCprInit();
    #if defined(MA2450)
    OsDrvTimerInit();
    printf("Sys freq = %ld KHz\n", DrvCprGetSysClockKhz());
    #endif
    gBuff.Create(B_SZ);

   //Gen test input histogram
    genHist();
   //Clear output histogram
    memset(oHist, 0, sizeof(oHist));

   //Create WR/RD threads:
    sc = pthread_attr_init           (&attr);                         if (sc) exit(sc);
    sc = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED); if (sc) exit(sc);
    sc = pthread_attr_setschedpolicy (&attr, SCHED_RR);               if (sc) exit(sc);

    #if defined(MA2450)
    printf("Please wait ...\n");
    OsDrvTimerStartTicksCount(&ticks);
    #endif
   //Start readers (buffer empty so should just hang)
    for(i=0; i<N_THR; i++){
      sc = pthread_create(&rdThrd[i], &attr, &funRd, &order[i]);
      if(sc) exit(sc);
    }
   //Start writers (buffer empty so should just hang)
    for(i=0; i<N_THR; i++){
      sc = pthread_create(&wrThrd[i], &attr, &funWr, &order[i]);
      if(sc) exit(sc);
    }
    sc = pthread_attr_destroy(&attr);                                  if (sc) exit(sc);

   //Wait for things to end:
    for(i=0; i<N_THR; i++){
       sc = pthread_join(rdThrd[i], NULL);  if(sc)  exit(sc);
       sc = pthread_join(wrThrd[i], NULL);  if(sc)  exit(sc);
    }

    #if defined(MA2450)
    OsDrvTimerGetElapsedTicks(&ticks, &cycles);
    printf("Cycles = 0x%08lx%08lx\n", (uint32_t)(cycles>>32), (uint32_t)cycles);
    #endif
    unitTestMemCompare(oHist, iHist, sizeof(iHist));
    unitTestFinalReport();
    exit(0);
}
