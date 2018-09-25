///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Main leon file
///

// 1: Includes
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <DrvShaveL2Cache.h>
#include <bsp.h>
#include <DrvCpr.h>
#include <DrvLeon.h>
#include "OsDrvSvu.h"
#include "rtems_config.h"
#include <unistd.h>
#include "fifoCommInitApi.h"
#include "OsDrvTimer.h"
#include "common.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define SHAVE_USED 0

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u32 helloShave0_masterEntry[];
extern u32 helloShave0_fifoCommMasterRun[];

extern u32 helloShave1_slaveInitEntry[];
extern u32 helloShave1_slaveEntry[];
extern u32 helloShave2_slaveInitEntry[];
extern u32 helloShave2_slaveEntry[];
extern u32 helloShave3_slaveInitEntry[];
extern u32 helloShave3_slaveEntry[];
extern u32 helloShave4_slaveInitEntry[];
extern u32 helloShave4_slaveEntry[];
extern u32 helloShave5_slaveInitEntry[];
extern u32 helloShave5_slaveEntry[];
extern u32 helloShave6_slaveInitEntry[];
extern u32 helloShave6_slaveEntry2[];
extern u32 helloShave6_slaveEntry[];
extern u32 helloShave7_slaveInitEntry[];
extern u32 helloShave7_slaveEntry2[];
extern u32 helloShave8_slaveInitEntry[];
extern u32 helloShave8_slaveEntry2[];
extern u32 helloShave9_slaveInitEntry[];
extern u32 helloShave9_slaveEntry2[];
extern u32 helloShave10_slaveInitEntry[];
extern u32 helloShave10_slaveEntry2[];
extern u32 helloShave11_slaveInitEntry[];
extern u32 helloShave11_slaveEntry2[];
extern u32 helloShave1_fifoCommSlaveRun[];
extern u32 helloShave2_fifoCommSlaveRun[];
extern u32 helloShave3_fifoCommSlaveRun[];
extern u32 helloShave4_fifoCommSlaveRun[];
extern u32 helloShave5_fifoCommSlaveRun[];
extern u32 helloShave6_fifoCommSlaveRun[];
extern u32 helloShave7_fifoCommSlaveRun[];
extern u32 helloShave8_fifoCommSlaveRun[];
extern u32 helloShave9_fifoCommSlaveRun[];
extern u32 helloShave10_fifoCommSlaveRun[];
extern u32 helloShave11_fifoCommSlaveRun[];


    
u32* slaveInitEntry[] = {
  helloShave1_slaveInitEntry,
  helloShave2_slaveInitEntry,
  helloShave3_slaveInitEntry,
  helloShave4_slaveInitEntry,
  helloShave5_slaveInitEntry,
  helloShave6_slaveInitEntry,
  helloShave7_slaveInitEntry,
  helloShave8_slaveInitEntry,
  helloShave9_slaveInitEntry,
  helloShave10_slaveInitEntry,
  helloShave11_slaveInitEntry
};

u32* slaveEntry[] = {
  helloShave1_slaveEntry,
  helloShave2_slaveEntry,
  helloShave3_slaveEntry,
  helloShave4_slaveEntry,
  helloShave5_slaveEntry,
  helloShave6_slaveEntry,
  helloShave6_slaveEntry2,
  helloShave7_slaveEntry2,
  helloShave8_slaveEntry2,
  helloShave9_slaveEntry2,
  helloShave10_slaveEntry2,
  helloShave11_slaveEntry2
};

u32* slaveRun[] = {
  helloShave1_fifoCommSlaveRun,
  helloShave2_fifoCommSlaveRun,
  helloShave3_fifoCommSlaveRun,
  helloShave4_fifoCommSlaveRun,
  helloShave5_fifoCommSlaveRun,
  helloShave6_fifoCommSlaveRun,
  helloShave7_fifoCommSlaveRun,
  helloShave8_fifoCommSlaveRun,
  helloShave9_fifoCommSlaveRun,
  helloShave10_fifoCommSlaveRun,
  helloShave11_fifoCommSlaveRun
};
int TaskFifo[] = {0, 2, 4, 6, 8, 10};
int TaskResponseFifo[] = {1, 3, 5, 7, 9, 11};

MyDataType __attribute__((section(".cmx_direct.data"))) dataS;
// 4: Static Local Data
// ----------------------------------------------------------------------------
static osDrvSvuHandler_t handler[NUMBER_OF_SLAVES_USED + 1];

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

void *POSIX_Init(void *args)
{
    UNUSED(args);
    s32 sc;
    tyTimeStamp timeStamp;
    u64 elapsedTicks;
    fifoCommTask_t taskTypes[NUMBER_OF_SLAVES_USED + 3];
    fifoCommSlaveHandler_t slaveHandler[NUMBER_OF_SLAVES_USED];

    dataS.n = 10000;

    sc = initClocksAndMemory();
    if (sc)
        exit(sc);
    sc = OsDrvSvuInit();
    if (sc)
        exit(sc);

    // init master
    fifoCommMasterHandler_t masterHandler;
    fifoCommMasterInit(&masterHandler,
            (fifoCommMasterCallback_t) helloShave0_masterEntry);

     sc = OsDrvTimerInit();
     if (sc)
         exit(sc);

    //try to add 2 tasks using 3 different combinations of fifos
    int k = 0, t=1;
    while (k < 6)
    {
        bool test = true;
        //init all slaves
        for (int i = 1; i <= NUMBER_OF_SLAVES_USED; i++)
            fifoCommSlaveInit(&slaveHandler[i - 1]);

        fifoCommMasterRegisterTaskType(&masterHandler, &taskTypes[0],
                TaskFifo[k], TaskResponseFifo[k]);
        k++;
        fifoCommMasterRegisterTaskType(&masterHandler, &taskTypes[1],
                TaskFifo[k], TaskResponseFifo[k]);

        //register half of the slave to the first task
        for (int i = 1; i <= NUMBER_OF_SLAVES_USED / 2 + 1; i++)
            fifoCommSlaveRegisterTaskType(&slaveHandler[i - 1],
                    &taskTypes[i + 1], &taskTypes[0],
                    (fifoCommTaskCallback_t) slaveInitEntry[i - 1],
                    (fifoCommTaskCallback_t) slaveEntry[i - 1]);

        //the middle slave will be able to do both tasks
        //register second half of the slave to the second task
        for (int i = NUMBER_OF_SLAVES_USED / 2 + 1; i <= NUMBER_OF_SLAVES_USED;
                i++)
            fifoCommSlaveRegisterTaskType(&slaveHandler[i - 1],
                    &taskTypes[i + 2], &taskTypes[1],
                    (fifoCommTaskCallback_t) slaveInitEntry[i - 1],
                    (fifoCommTaskCallback_t) slaveEntry[i]);

        //start slaves
        for (int i = 1; i <= NUMBER_OF_SLAVES_USED; i++)
        {
            fifoCommStartSlave(&slaveHandler[i-1], &handler[i], slaveRun[i-1], i);
        }

        //start master and timer
        sc = OsDrvTimerStartTicksCount(&timeStamp);
        if (sc)
            exit(sc);
        sc = fifoCommStartMaster(&masterHandler, &handler[0], helloShave0_fifoCommMasterRun, &dataS, SHAVE_USED);
        if (sc)
            exit(sc);
        fifoCommWaitMaster(&handler[0]);
        sc = OsDrvTimerGetElapsedTicks(&timeStamp, &elapsedTicks);
        if (sc)
            exit(sc);
        printf("Elapsed time:%llu ticks\n", elapsedTicks);
        k++;

        //check for failure
        for (int i = 0; i < TASKS; i++)
            for (int j = 0; j < BUFFSIZE; j++)
                if (dataS.n*t + dataS.n*t*100!= dataS.data[i][i])
                    test = false;
        if (test)
            printf("Test passed\n");
        else
            printf("Test failed\n");

        //close slaves
        for (int i = 1; i <= NUMBER_OF_SLAVES_USED; i++)
        {
            fifoCommWaitSlave(&handler[i]);
        }

        t++;
    }

    exit(0);
}
