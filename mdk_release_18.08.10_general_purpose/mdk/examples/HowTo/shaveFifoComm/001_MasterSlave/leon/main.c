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
#define TASK_FIFO 3
#define TASK_RESPONSE_FIFO 4

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u32 helloShave0_masterEntry[];
extern u32 helloShave0_fifoCommMasterRun[];
extern u32 helloShave1_fifoCommSlaveRun[];
extern u32 helloShave1_slaveInitEntry[];
extern u32 helloShave1_slaveEntry[];
extern u32 helloShave2_fifoCommSlaveRun[];
extern u32 helloShave2_slaveInitEntry[];
extern u32 helloShave2_slaveEntry[];
extern u32 helloShave3_fifoCommSlaveRun[];
extern u32 helloShave3_slaveInitEntry[];
extern u32 helloShave3_slaveEntry[];
extern u32 helloShave4_fifoCommSlaveRun[];
extern u32 helloShave4_slaveInitEntry[];
extern u32 helloShave4_slaveEntry[];
extern u32 helloShave5_fifoCommSlaveRun[];
extern u32 helloShave5_slaveInitEntry[];
extern u32 helloShave5_slaveEntry[];
extern u32 helloShave6_fifoCommSlaveRun[];
extern u32 helloShave6_slaveInitEntry[];
extern u32 helloShave6_slaveEntry[];
extern u32 helloShave7_fifoCommSlaveRun[];
extern u32 helloShave7_slaveInitEntry[];
extern u32 helloShave7_slaveEntry[];
extern u32 helloShave8_fifoCommSlaveRun[];
extern u32 helloShave8_slaveInitEntry[];
extern u32 helloShave8_slaveEntry[];
extern u32 helloShave9_fifoCommSlaveRun[];
extern u32 helloShave9_slaveInitEntry[];
extern u32 helloShave9_slaveEntry[];
extern u32 helloShave10_fifoCommSlaveRun[];
extern u32 helloShave10_slaveInitEntry[];
extern u32 helloShave10_slaveEntry[];
extern u32 helloShave11_fifoCommSlaveRun[];
extern u32 helloShave11_slaveInitEntry[];
extern u32 helloShave11_slaveEntry[];

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
  helloShave7_slaveEntry,
  helloShave8_slaveEntry,
  helloShave9_slaveEntry,
  helloShave10_slaveEntry,
  helloShave11_slaveEntry
};

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
    fifoCommTask_t taskTypes[NUMBER_OF_SLAVES_USED + 1];
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
    fifoCommMasterRegisterTaskType(&masterHandler, &taskTypes[0], TASK_FIFO, TASK_RESPONSE_FIFO);

    //init slave
    for (int i = 1; i <= NUMBER_OF_SLAVES_USED; i++)
    {
        fifoCommSlaveInit(&slaveHandler[i - 1]);
        fifoCommSlaveRegisterTaskType(&slaveHandler[i - 1], &taskTypes[i],
                                      &taskTypes[0], (fifoCommTaskCallback_t) slaveInitEntry[i - 1],
                                      (fifoCommTaskCallback_t) slaveEntry[i - 1]);
    }

    //open slaves
    for (int i = 1; i <= NUMBER_OF_SLAVES_USED; i++)
    {
        fifoCommStartSlave(&slaveHandler[i-1], &handler[i], slaveRun[i-1], i);

    }

    sc = OsDrvTimerInit();
    if (sc)
        exit(sc);
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

    //check for failure
    bool test = true;
    for (int i = 0; i < TASKS; i++)
        for (int j = 0; j < BUFFSIZE; j++)
            if (dataS.n != dataS.data[i][i])
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

    exit(0);
}
