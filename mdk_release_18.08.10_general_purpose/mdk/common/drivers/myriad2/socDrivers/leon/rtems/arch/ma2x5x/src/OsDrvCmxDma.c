///
/// @file OsDrvCmxDma.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvCmxDma
/// @{
///

#include <OsDrvCmxDma.h>

#ifndef USE_CMX_DMA_NEW_DRIVER

#include <stdio.h>
#include <stdarg.h>
#include "OsCross.h"

static struct {
    rtems_id controlMutex;      //access control
    OS_MYRIAD_DRIVER_STATUS status;
} osCmxDmaControl;

extern int usedAgentsNr;//Number of used Link Agents(LA) by current processor
extern int startFrom;//The first LA to use
extern int intOffset;//Bit position from INT_STATUS register used for task status

static osCommonIrqShareInfo_t osCommonIrqSharedInfo;
static u32 eventIdCount = 0;
extern drvStat_t cmxDmaStatus;
extern u8 currentlyExecuting[];
// Lock utility by default wait forever to access the resource
#define OS_DRV_CMX_DMA_MUTEX_LOCK()           osDrvCmxDmaLockCriticalSection(osCmxDmaControl.controlMutex, true, RTEMS_NO_TIMEOUT);
// Unlock utility
#define OS_DRV_CMX_DMA_MUTEX_UNLOCK()         osDrvCmxDmaUnlockCriticalSection(osCmxDmaControl.controlMutex)

#ifdef OS_DRV_CMX_DMA_L1
    #define OS_DPRINTF1(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF1(...)
#endif

#ifdef OS_DRV_CMX_DMA_L2
    #define OS_DPRINTF2(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF2(...)
#endif

#ifdef OS_DRV_CMX_DMA_L3
    #define OS_DPRINTF3(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF3(...)
#endif

static int isDmaBusy(void)
{
    int i = 0;

    do{
        if(currentlyExecuting[i] != 0) return 1;
    }while(++i < NR_OF_LINK_AGENTS);

    return 0;
}

static void transferDoneDefaultCallback(dmaTransactionList* ListPtr, void* threadId)
{
    //notify the task owner that it's task has finished
    OsDrvCommonInterruptSendEvent((rtems_id)threadId, 1 << ListPtr->cfgLink.cfgBits.reserved2, &osCommonIrqSharedInfo.irqShared, OSCOMMON_EVENT_CMXDMA, TRUE);

}

static void cmxDmaIrqHandler(void *arg)
{
    UNUSED(arg); // hush the compiler warning.

    DrvCmxDmaIrqHandlerFunc(0);
}

// Initialize the CMXDMA driver
int OsDrvCmxDmaInitDefault(void)
{
    rtems_status_code status;

    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;

    if(osCmxDmaControl.status == OS_DRV_STAT_UNINITIALIZED)
    {
        status = rtems_semaphore_create(rtems_build_name('C', 'D', 'M', 'A'), 1, RTEMS_SIMPLE_BINARY_SEMAPHORE,0, (rtems_id *)&osCmxDmaControl.controlMutex);

        if(status == RTEMS_SUCCESSFUL)
        {
            cmxDmaStatus.status = DRV_CDMA_INITIALIZED;
            osCmxDmaControl.status = OS_DRV_STAT_INITIALIZED;
            //Configure the bare-metal driver on what hardware mutex to use,
            //the offset for interrupt status bits,
            //number of used link agents, and which is the first one to start using
            if(swcWhoAmI() == PROCESS_LEON_OS)
                DrvCmxDmaSetup(intOffset, usedAgentsNr, startFrom);
            else
            	DrvCmxDmaSetup(15, 1, 3);//LRT will use in default configuration only link agent 3

            BSP_Clear_interrupt(IRQ_CMXDMA);
            BSP_Set_interrupt_type_priority(IRQ_CMXDMA, POS_EDGE_INT, DEFAULT_CDMA_INT_LEVEL);
            BSP_interrupt_register(IRQ_CMXDMA, NULL, (rtems_interrupt_handler) cmxDmaIrqHandler, NULL);

            res = OS_MYR_DRV_SUCCESS;
        }
        else res = status;
    }
    else if(osCmxDmaControl.status == OS_DRV_STAT_INITIALIZED)
    {
        res = OS_MYR_DRV_ALREADY_INITIALIZED;
    }

    OS_DPRINTF1("OsDrvCmxDmaInit() res %d\n", res);

    return res;
}
// Initialize the CMXDMA driver
int OsDrvCmxDmaInit(u32 interruptLevel, u32 interruptOffset, u32 nrOfAgents, u32 startingFrom)
{
    rtems_status_code status;

    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;

    if(osCmxDmaControl.status == OS_DRV_STAT_UNINITIALIZED)
    {
        status = rtems_semaphore_create(rtems_build_name('C', 'D', 'M', 'A'), 1, RTEMS_SIMPLE_BINARY_SEMAPHORE,0, (rtems_id *)&osCmxDmaControl.controlMutex);

        if(status == RTEMS_SUCCESSFUL)
        {
            cmxDmaStatus.status = DRV_CDMA_INITIALIZED;
            osCmxDmaControl.status = OS_DRV_STAT_INITIALIZED;
            //Configure the bare-metal driver on what hardware mutex to use,
            //the offset for interrupt status bits,
            //number of used link agents, and which is the first one to start using
            DrvCmxDmaSetup(interruptOffset, nrOfAgents, startingFrom);

            BSP_Clear_interrupt(IRQ_CMXDMA);
            BSP_Set_interrupt_type_priority(IRQ_CMXDMA, POS_EDGE_INT, interruptLevel);
            BSP_interrupt_register(IRQ_CMXDMA, NULL, (rtems_interrupt_handler) cmxDmaIrqHandler, NULL);

            res = OS_MYR_DRV_SUCCESS;
        }
        else res = status;
    }
    else if(osCmxDmaControl.status == OS_DRV_STAT_INITIALIZED)
    {
        res = OS_MYR_DRV_ALREADY_INITIALIZED;
    }

    OS_DPRINTF1("OsDrvCmxDmaInit() res %d\n", res);

    return res;
}

/* Enter Critical Section */
static int osDrvCmxDmaLockCriticalSection(rtems_id sem, u32 wait, u32 wait_ticks)
{
    rtems_status_code status;
    int res;

    if (wait)
        status = rtems_semaphore_obtain(sem, RTEMS_WAIT, wait_ticks);
    else
        status = rtems_semaphore_obtain(sem, RTEMS_NO_WAIT, 0);

    switch (status)
    {
    case RTEMS_SUCCESSFUL:
        res = OS_MYR_DRV_SUCCESS;
        break;
    case RTEMS_TIMEOUT:
        res = OS_MYR_DRV_TIMEOUT;
        break;
    default:
        res = (int)status;  //mixing rtems return codes (which occupy the small values range <1000) into OS_MYRIAD_DRIVER_STATUS_CODEs (values >1000); TODO: create a proper translator here !
        break;
    }
    OS_DPRINTF2("osDrvCmxDmaLockCriticalSection res %d wait %d\n", res, wait);
    return res;
}

/* Abandon Critical Section */
static rtems_status_code osDrvCmxDmaUnlockCriticalSection(rtems_id sem)
{
    rtems_status_code status = rtems_semaphore_release(sem);
    int res;

    switch (status)
    {
    case RTEMS_SUCCESSFUL:
        res = OS_MYR_DRV_SUCCESS;
        break;
    case RTEMS_TIMEOUT:
        res = OS_MYR_DRV_TIMEOUT;
        break;
    default:
        res = (int)status;  //mixing rtems return codes (which occupy the small values range <1000) into OS_MYRIAD_DRIVER_STATUS_CODEs (values >1000); TODO: create a proper translator here !
        break;
    }
    OS_DPRINTF2("osDrvCmxDmaUnlockCriticalSection res %d\n", res);
    return res;
}


int OsDrvCmxDmaInitRequester(int priority, dmaRequesterId *reqId)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res;

    res = OS_DRV_CMX_DMA_MUTEX_LOCK();

    if(res == OS_MYR_DRV_SUCCESS)
    {
        if(osCmxDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            *reqId = DrvCmxDmaInitRequester(priority);
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_CMX_DMA_MUTEX_UNLOCK();
    }

    OS_DPRINTF1("OsDrvCmxDmaInitRequester() init requester %d \n", reqId);
    return res;
}

int OsDrvCmxDmaCreateTransactionFullOptions(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength, u32 SrcLineWidth, u32 DstLineWidth, s32 SrcStride, s32 DstStride)
{
    DrvCmxDmaCreateTransactionFullOptions(ReqId, NewTransaction, Src, Dst, ByteLength, SrcLineWidth, DstLineWidth, SrcStride, DstStride);

    return OS_MYR_DRV_SUCCESS;
}

int OsDrvCmxDmaCreateTransaction(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength)
{
    DrvCmxDmaCreateTransaction(ReqId, NewTransaction, Src, Dst, ByteLength);

    return OS_MYR_DRV_SUCCESS;
}

int OsDrvCmxDmaCreateTransactionSrcStride(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength, u32 LineWidth, s32 SrcStride)
{
    DrvCmxDmaCreateTransactionSrcStride(ReqId, NewTransaction, Src, Dst, ByteLength, LineWidth, SrcStride);

    return OS_MYR_DRV_SUCCESS;
}

int OsDrvCmxDmaCreateTransactionDstStride(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength, u32 LineWidth, s32 DstStride)
{
    DrvCmxDmaCreateTransactionDstStride(ReqId, NewTransaction, Src, Dst, ByteLength, LineWidth, DstStride);

    return OS_MYR_DRV_SUCCESS;
}

int OsDrvCmxDmaCreate3DTransaction(dmaRequesterId ReqId,
                                   dmaTransactionList *NewTransaction,
                                   u8 *Src,
                                   u8 *Dst,
                                   u32 ByteLength,
                                   u32 SrcWidth,
                                   u32 DstWidth,
                                   u32 SrcStride,
                                   u32 DstStride,
                                   u32 NumPlanes,
                                   u32 SrcPlaneStride,
                                   u32 DstPlaneStride) {
    int status = OS_MYR_DRV_SUCCESS;

    if (NULL == DrvCmxDmaCreate3DTransaction(ReqId, NewTransaction,
                                             Src, Dst, ByteLength,
                                             SrcWidth, DstWidth,
                                             SrcStride, DstStride,
                                             NumPlanes,
                                             SrcPlaneStride, DstPlaneStride)) {
        status = OS_MYR_DRV_ERROR;
    }

    return status;
}

int OsDrvCmxDmaLinkTasks(dmaTransactionList* listHead, u32 nmbTasks, ...)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;

        if(osCmxDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            //Link logic brought from bare-metal function,
            //It's a bit tricky to pass variable number of arguments to other function which expects the same.
            dmaTransactionList *listEnd, *val;
            va_list a_list;
            u32 i;

            va_start(a_list, nmbTasks);

            listEnd = listHead;
            for(i = 0; i < nmbTasks; i++)
            {
                val = va_arg(a_list, dmaTransactionList*);
                listEnd->linkAddress = val;
                listEnd = listEnd->linkAddress;
            }

            va_end(a_list);

            res = OS_MYR_DRV_SUCCESS;
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }

    return res;
}

int OsDrvCmxDmaStartListTask(dmaTransactionList* ListPtr, int *status)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res;
    rtems_id threadId;
    res = OS_DRV_CMX_DMA_MUTEX_LOCK();

    if(res == OS_MYR_DRV_SUCCESS)
    {
        if(osCmxDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            threadId = rtems_task_self();

            //Assign to each started task a different event available from osCommonIrqSharedInfo
            //The start of more tasks than are finishing can lead to assigning the same bit to more than one task
            //This can cause ambiguity and threads may go into suspended state, waiting indefinitely for tasks to finish.
            ListPtr->cfgLink.cfgBits.reserved2 = eventIdCount++ % 32;

            *status = DrvCmxDmaStartTaskAsync(ListPtr, transferDoneDefaultCallback, (void*)threadId);
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_CMX_DMA_MUTEX_UNLOCK();
    }

    OS_DPRINTF1("OsDrvCmxDmaStartListTask()  task %X \n", &ListPtr);
    return res;
}

int OsDrvCmxDmaWaitTask(dmaTransactionList* ListPtr)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res;

    res = OS_DRV_CMX_DMA_MUTEX_LOCK();

    if(osCmxDmaControl.status == OS_DRV_STAT_INITIALIZED)
    {
        u32 timeout = RTEMS_NO_TIMEOUT;
        //if the task is finished already, there is no reason to wait for it
        //field was set in bare-metal IRQ handler
        if(IS_DMA_FINISHED(ListPtr->cfgLink.fullCfgRegister))
            timeout = 0;

        OsCommonEventReceive(1 << ListPtr->cfgLink.cfgBits.reserved2, TRUE, TRUE, timeout, NULL, &osCommonIrqSharedInfo.irqShared, OSCOMMON_EVENT_CMXDMA);
        res = OS_MYR_DRV_SUCCESS;
    }
    else
    {
        res = OS_MYR_DRV_NOTOPENED;
    }

    OS_DRV_CMX_DMA_MUTEX_UNLOCK();

    OS_DPRINTF1("OsDrvCmxDmaWaitTask()  task %X \n", &ListPtr);
    return res;
}

int OsDrvCmxDmaIsTaskFinished(dmaTransactionList* ListPtr)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;

    if(osCmxDmaControl.status == OS_DRV_STAT_INITIALIZED)
    {
        //field was set in bare-metal IRQ handler
        if(IS_DMA_FINISHED(ListPtr->cfgLink.fullCfgRegister))
            res = OS_MYR_DRV_SUCCESS;
    }
    else
    {
        res = OS_MYR_DRV_NOTOPENED;
    }
    return res;
}

int OsDrvCmxDmaClose(void)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res;

    res = OS_DRV_CMX_DMA_MUTEX_LOCK();

    if(res == OS_MYR_DRV_SUCCESS)
    {
        if(!isDmaBusy())
        {
            osCmxDmaControl.status = OS_DRV_STAT_UNINITIALIZED;

            OS_DRV_CMX_DMA_MUTEX_UNLOCK();
            rtems_semaphore_delete(osCmxDmaControl.controlMutex);
        }
        else
        {
            OS_DRV_CMX_DMA_MUTEX_UNLOCK();
            //Not all tasks finished
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
    }
    OS_DPRINTF1("OsDrvCmxDmaClose() res %d\n", res);
    return res;
}

int OsDrvCmxDmaCreateTransactionExt(u32 Type,
                                    dmaRequesterId ReqId,
                                    dmaTransactionList *NewTransaction,
                                    u8* Src,
                                    u8* Dst,
                                    u32 ByteLength,
                                    u32 SrcLineWidth,
                                    u32 DstLineWidth,
                                    s32 SrcStride,
                                    s32 DstStride,
                                    u8 BurstLength)
{
    dmaTransactionList_t *tskEntry;

    tskEntry = DrvCmxDmaCreateTransactionExt(Type, ReqId, NewTransaction, Src, Dst, ByteLength, SrcLineWidth, DstLineWidth, SrcStride, DstStride, BurstLength);

    if (!tskEntry)
        return OS_MYR_DRV_ERROR;
    else
        return OS_MYR_DRV_SUCCESS;
}

#endif // USE_CMX_DMA_NEW_DRIVER

///@}

