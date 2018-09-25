///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     shave Fifo communication init functions
///

// 1: Includes
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include "fifoCommInitApi.h"
#include "OsDrvSvu.h"
#include "OsCommon.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)

// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

int fifoCommStartMaster(fifoCommMasterHandler_t* masterHandler,
        osDrvSvuHandler_t* handler, u32* fifoCommMasterRun, void* masterParams, u32 shaveNumber)
{
    int sc;
    sc = OsDrvSvuOpenShave(handler, shaveNumber, OS_MYR_PROTECTION_SEM);
    if (sc != OS_MYR_DRV_SUCCESS)
        return sc;
    sc = OsDrvSvuResetShave(handler);
    if (sc)
        exit(sc);
    sc = OsDrvSvuSetAbsoluteDefaultStack(handler);
    if (sc)
        exit(sc);
    sc = OsDrvSvuStartShaveCC(handler, (u32)fifoCommMasterRun,
            "ii", (u32) masterHandler, masterParams);
    if (sc)
        exit(sc);
    return 0;
}

void fifoCommStartSlave(fifoCommSlaveHandler_t* slaveHandler,
        osDrvSvuHandler_t* handler, u32* slaveRunP, int shaveNumber)
{
    int sc;
    sc = OsDrvSvuOpenShave(handler, shaveNumber, OS_MYR_PROTECTION_SEM);
    if (sc != OS_MYR_DRV_SUCCESS)
        printf("[T2] cannot open shave %d\n", shaveNumber);
    else
    {
        sc = OsDrvSvuResetShave(handler);
        if (sc)
            exit(sc);
        sc = OsDrvSvuSetAbsoluteDefaultStack(handler);
        if (sc)
            exit(sc);
        sc = OsDrvSvuStartShaveCC(handler, (u32) slaveRunP, "i",
                (u32) slaveHandler);
        if (sc)
            exit(sc);
    }
}

void fifoCommWaitSlave(osDrvSvuHandler_t* handler)
{
    int sc;
    u32 run;
    sc = OsDrvSvuStop(handler);
    if (sc)
        exit(sc);
    sc = OsDrvSvuResetShave(handler);
    if (sc)
        exit(sc);
    sc = OsDrvSvuWaitShaves(1, handler, OS_DRV_SVU_WAIT_FOREVER, &run);
    if (sc)
        exit(sc);
    sc = OsDrvSvuCloseShave(handler);
    if (sc)
        exit(sc);
}

void fifoCommWaitMaster(osDrvSvuHandler_t* handler)
{
    int sc;
    u32 run;
    sc = OsDrvSvuWaitShaves(1, handler, OS_DRV_SVU_WAIT_FOREVER, &run);
    if (sc)
        exit(sc);
    sc = OsDrvSvuCloseShave(handler);
    if (sc)
        exit(sc);
}

void fifoCommMasterInit(fifoCommMasterHandler_t* handler, fifoCommMasterCallback_t taskHandler)
{
    handler->masterHandler = taskHandler;
    handler->taskTypeList = NULL;
}

void fifoCommSlaveInit(fifoCommSlaveHandler_t* handler)
{
    handler->taskTypeList = NULL;
    handler->stopRequest = 0;
}

void fifoCommMasterRegisterTaskType(fifoCommMasterHandler_t* masterHandler, fifoCommTask_t* taskType,
                                    u32 taskFifoNr, u32 responseFifoNr)
{
    taskType->taskFifoNr = taskFifoNr;
    taskType->responseFifoNr = responseFifoNr;
    taskType->next = NULL;
    taskType->initHandler = NULL;
    taskType->taskHandler = NULL;
    if (masterHandler->taskTypeList == NULL)
    {
        masterHandler->taskTypeList = taskType;
    }
    else
    {
        taskType->next = masterHandler->taskTypeList;
        masterHandler->taskTypeList = taskType;
    }
}

void fifoCommSlaveRegisterTaskType(fifoCommSlaveHandler_t* slaveHandler, fifoCommTask_t* taskType,
                                   fifoCommTask_t* masterTask, fifoCommTaskCallback_t initHandler,
                                   fifoCommTaskCallback_t taskHandler)
{
    if (slaveHandler == NULL || taskType == NULL || masterTask ==NULL)
    {
        return;
    }
    taskType->taskFifoNr = masterTask->taskFifoNr;
    taskType->responseFifoNr = masterTask->responseFifoNr;
    taskType->next = NULL;
    taskType->initHandler = initHandler;
    taskType->taskHandler = taskHandler;
    if (slaveHandler->taskTypeList == NULL)
    {
        slaveHandler->taskTypeList = taskType;
    }
    else
    {
        taskType->next = slaveHandler->taskTypeList;
        slaveHandler->taskTypeList= taskType;
    }
}

