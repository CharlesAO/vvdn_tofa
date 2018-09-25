#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <swcLeonUtils.h>
#include <DrvTimer.h>

#define IDLE_RUNS (1000*1000)

uint32_t idleCnt;
uint64_t idleStart;
uint64_t idleStop;
uint64_t idleExec;
uint64_t tAll, tIpipe, tIdle;

extern void appRxIspMain();
extern void setupIpipeServer();

//######################################################################
int idleRoutine()
{
   if(idleCnt == 0)
      DrvTimerGetSystemTicks64(&idleStart);

   uint32_t i;
   for(i=0; i<1000; i++){
     NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
     NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
     NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
     NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
     NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
   }
   idleCnt++;

   if(idleCnt == IDLE_RUNS){
      DrvTimerGetSystemTicks64(&idleStop);
      idleExec = idleStop - idleStart;
      return 1;
   }
   return 0;
}

//######################################################################
void getLeonLoad()
{
  //1nd run (with Ipipe)
    idleCnt = 0;
    setupIpipeServer();
    while (1) {
      appRxIspMain();
      if(idleRoutine())
        break;
    }
    tAll = idleExec;

  //2st run (just IDLE loop)
    swcLeonSetPIL(15); //Kill IRQs => no more Ipipe/Opipe events
    idleCnt = 0;
    while (1) {
      if(idleRoutine())
        break;
    }
    tIdle = idleExec;

    tIpipe = tAll - tIdle;

  //Reports
    printf("All  %lld cc\n", tAll);
    printf("Idle %lld cc\n", tIdle);
    printf("Ipipe Leon load = %.2f %%\n", tIpipe * 100.0f / tAll);
}
