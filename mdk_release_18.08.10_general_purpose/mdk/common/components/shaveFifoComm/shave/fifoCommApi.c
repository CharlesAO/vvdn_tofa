///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     shave Fifo communication function implementations
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <svuCommonShave.h>
#include <ShDrvCmxFifo.h>

#include <swcWhoAmI.h>

#include "fifoCommApi.h"
#include "fifoCommApiDefines.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

void fifoCommMasterAddTask(fifoCommTask_t* taskType, void* taskParameters)
{
    scFifoWrite(taskType->taskFifoNr, (u32)taskParameters);
}

void* fifoCommMasterWaitTask(fifoCommTask_t* taskType)
{
    scFifoMonitorSelect(scGetShaveNumber(),taskType->responseFifoNr);
    scFifoWaitElement();
    // When playing with monitor select we may pass the waitElement function without actually having an element.
    // This protection could be done with Atomic read, too, however that's not taking less cycles
    // in future this could be optimized if the master could take different responses on the same fifo.
    while( scFifoGetFillLevel(taskType->responseFifoNr) == 0);
    return (void*) scFifoReadShave(taskType->responseFifoNr);
}

u32 fifoCommSlaveReadTask(fifoCommTask_t* taskType, u32* result)
{
    u32 retVal = 0;
#if defined(MA2150) || defined(MA2450)
    u64 res = scFifoReadShaveDwordAtomic(taskType->taskFifoNr);
    if ((res >> (sizeof(u64)*8-8)) == 0x0)
#else
    u32 res = 0;
    retVal = ShDrvCmxFifoNReadAtomicWord(taskType->taskFifoNr, &res);
    if (retVal == MYR_DRV_SUCCESS)
#endif
    {
      *result = (u32)res;
      retVal = 1;
    }
    return retVal;
}

void fifoCommSlaveNotifyTaskCompletion(fifoCommTask_t* taskType, void* taskParameters)
{
    scFifoWrite(taskType->responseFifoNr, (u32)taskParameters);
}

void fifoCommMasterRun(fifoCommMasterHandler_t* handler, void* params)
{
    handler->masterHandler(handler, params);
    SHAVE_HALT;
}

void fifoCommSlaveRun(fifoCommSlaveHandler_t* handler)
{
    fifoCommTask_t* task = handler->taskTypeList;
    handler->stopRequest = 0;
    while (task != NULL)
    {
        if(task->initHandler != NULL)
        {
            task->initHandler(NULL);
        }
        task = task->next;
    }

    if (handler->taskTypeList->next == NULL)
    {
        scFifoMonitorSelect(scGetShaveNumber(),
                handler->taskTypeList->taskFifoNr);
        while (1)
        {
            scFifoWaitElement();
            task = handler->taskTypeList;
            u32 taskParams;
            if (fifoCommSlaveReadTask(task, &taskParams))
            {
                task->taskHandler((void*) taskParams);
                fifoCommSlaveNotifyTaskCompletion(task, (void*) taskParams);
            }
            task = task->next;
            if (handler->stopRequest)
            {
                break;
            }
        }
    }
    else
    {
        while (1)
        {
            task = handler->taskTypeList;
            while (task != NULL)
            {
                u32 taskParams;
                if (fifoCommSlaveReadTask(task, &taskParams))
                {
                    task->taskHandler((void*) taskParams);
                    fifoCommSlaveNotifyTaskCompletion(task, (void*) taskParams);
                }
                task = task->next;
            }
            if (handler->stopRequest)
            {
                break;
            }
        }
    }

    SHAVE_HALT;
}

