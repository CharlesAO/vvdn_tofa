///
/// @file OsDrvAhbDma.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvAhbDma
/// @{
///


/// 
/// 
/// 
///
/// @brief     AHB DMA API RTEMS Functions.
///

/// 1: Includes
/// ----------------------------------------------------------------------------
#include <stdio.h>
#include <assert.h>
#include <bsp.h>
#include <bsp/irq-generic.h>
#include "OsDrvAhbDma.h"
#include "DrvRegUtils.h"
#include "OsCross.h"
#include "assert.h"
#include <rtems.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// Lock utility by default wait forever to access the resource
#define OS_DRV_AHB_DMA_MUTEX_LOCK()           osDrvAhbDmaLockCriticalSection(osAhbDmaControl.controlMutex, true, RTEMS_NO_TIMEOUT)
// Unlock utility
#define OS_DRV_AHB_DMA_MUTEX_UNLOCK()         osDrvAhbDmaUnlockCriticalSection(osAhbDmaControl.controlMutex)

#define OS_DRV_AHB_DMA_LOCK(x)      ahbDmaHandler[x]?protectionLock(x, osAhbDmaChannelControl[x].protection):OS_MYR_DRV_NOTOPENED
#define OS_DRV_AHB_DMA_UNLOCK(x)    ahbDmaHandler[x]?protectionUnlock(x, osAhbDmaChannelControl[x].protection):OS_MYR_DRV_NOTOPENED

#ifdef OS_DRV_AHB_DMA_L1
    #define OS_DPRINTF1(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF1(...)
#endif

#ifdef OS_DRV_AHB_DMA_L2
    #define OS_DPRINTF2(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF2(...)
#endif

#ifdef OS_DRV_AHB_DMA_L3
    #define OS_DPRINTF3(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF3(...)
#endif

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
static struct {
    u8 openedChannelsNo;
    rtems_id controlMutex;      //access control
    OS_MYRIAD_DRIVER_STATUS status;
} osAhbDmaControl;

static struct {
    rtems_id taskId;
    rtems_id channelMutex;
    osAhbDmaTransferStatus_t status;
    OS_MYRIAD_DRIVER_PROTECTION protection;
} osAhbDmaChannelControl[DRV_AHB_DMA_CHANNEL_NO];

static osCommonIrqShareInfo_t osCommonIrqSharedInfo;
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

// this function will be called in the ISR handler instead of bare metal provided callback
static void osAhbDmaTransferDoneCallback(u32 channel, ahbDmaChannelStatus_t status)
{
    //inform the calling task that the transmission is ready
    if (status == AHB_DMA_CHANNEL_ERROR)
    {
		OsDrvCommonInterruptSendEvent(osAhbDmaChannelControl[channel].taskId,
                                      AHB_DMA_ERROR(channel) | AHB_DMA_EVENT(channel),
                                      &osCommonIrqSharedInfo.irqShared,
                                      OSCOMMON_EVENT_AHBDMA,
                                      FALSE);
        //rtems_event_send(osAhbDmaChannelControl[channel].taskId, AHB_DMA_ERROR(channel) | AHB_DMA_EVENT(channel));
    }
    else if (status == AHB_DMA_CHANNEL_SUCCESS)
    {
		OsDrvCommonInterruptSendEvent(osAhbDmaChannelControl[channel].taskId,
                                      AHB_DMA_EVENT(channel),
                                      &osCommonIrqSharedInfo.irqShared,
                                      OSCOMMON_EVENT_AHBDMA,
                                      FALSE);
        //rtems_event_send(osAhbDmaChannelControl[channel].taskId, AHB_DMA_EVENT(channel));
    }
}

static void ahbDmaIrqHandler(void *arg)
{
    UNUSED(arg); // hush the compiler warning.

    DrvAhbDmaProcessIrq();
    BSP_Clear_interrupt(IRQ_AHB_DMA);
}

/* Enter Critical Section */
static int osDrvAhbDmaLockCriticalSection(rtems_id sem, u32 wait, u32 wait_ticks)
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
    OS_DPRINTF2("osDrvAhbDmaLockCriticalSection res %d wait %d\n", res, wait);
    return res;
}

/* Abandon Critical Section */
static rtems_status_code osDrvAhbDmaUnlockCriticalSection(rtems_id sem)
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
    OS_DPRINTF2("osDrvAhbDmaUnlockCriticalSection res %d\n", res);
    return res;
}

static int protectionLock(u32 channel, OS_MYRIAD_DRIVER_PROTECTION protectionType)
{
    int res = OS_MYR_DRV_ERROR;

    switch (protectionType)
    {
    case OS_MYR_PROTECTION_SEM:
        res = osDrvAhbDmaLockCriticalSection(osAhbDmaControl.controlMutex, true, RTEMS_NO_TIMEOUT);
        break;
    case OS_MYR_PROTECTION_SELF:
        if (osAhbDmaChannelControl[channel].status == OS_DRV_AHB_DMA_FREE)
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        else if (osAhbDmaChannelControl[channel].taskId == rtems_task_self())
        {
            res = OS_MYR_DRV_SUCCESS;
        }
        else
        {
            res = OS_MYR_DRV_RESOURCE_BUSY;
        }
        break;
    case OS_MYR_PROTECTION_CUSTOM_SEM:
        res = osDrvAhbDmaLockCriticalSection(osAhbDmaChannelControl[channel].channelMutex, true, RTEMS_NO_TIMEOUT);
        break;
    default:
        if (osAhbDmaChannelControl[channel].status == OS_DRV_AHB_DMA_FREE)
        {
            // trying to lock a channel that was not opened
            res = OS_MYR_DRV_NOTOPENED;
        }
        else
        {
            // the channel was initialized with an unknown protection parameter
            res = OS_MYR_DRV_ERROR;
        }
        break;
    }
    return res;
}

static int protectionUnlock(u32 channel, OS_MYRIAD_DRIVER_PROTECTION protectionType)
{
    int res = OS_MYR_DRV_SUCCESS;

    switch (protectionType)
    {
    case OS_MYR_PROTECTION_SEM:
        res = osDrvAhbDmaUnlockCriticalSection(osAhbDmaControl.controlMutex);
        break;
    case OS_MYR_PROTECTION_SELF:
        res = OS_MYR_DRV_SUCCESS;
        break;
    case OS_MYR_PROTECTION_CUSTOM_SEM:
        res = osDrvAhbDmaUnlockCriticalSection(osAhbDmaChannelControl[channel].channelMutex);
        break;
    default:
        res = OS_MYR_DRV_ERROR;
        break;
    }
    return res;
}

// transferDoneCallback parameter is not used for now; it will be used
// in the future to provide callbacks in thread context
int OsDrvAhbDmaOpenAnyChannel(osDrvAhbDmaHandler_t* handler,
                              ahbDmaTransferDoneCallback_t* transferDoneCallback,
                              OS_MYRIAD_DRIVER_PROTECTION protection)
{
    u32 i, res;
    s32 channel;

    res = OS_DRV_AHB_DMA_MUTEX_LOCK();
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (osAhbDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            channel = -1;
            for (i = 0; i < DRV_AHB_DMA_CHANNEL_NO; i++)
            {
                if (osAhbDmaChannelControl[i].status == OS_DRV_AHB_DMA_FREE)
                {
                    channel = i;
                    break;
                }
            }
            if (channel >= 0)
            {
                res = OsDrvAhbDmaOpenChannel(handler, channel, transferDoneCallback, protection);
            }
            else
            {
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_AHB_DMA_MUTEX_UNLOCK();
    }

    return res;
}

int OsDrvAhbDmaIsFreeChannel(u32 channel, s32* free)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;

    *free = 0;
    res = OS_DRV_AHB_DMA_MUTEX_LOCK();
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (osAhbDmaChannelControl[channel].status == OS_DRV_AHB_DMA_FREE)
        {
            *free = 1;
        }

        OS_DRV_AHB_DMA_MUTEX_UNLOCK();
    }
    return res;
}

static int osDrvAhbDmaWaitTransferFinish(u32 channels, u32 timeout, u32* channelsActive)
{
    rtems_status_code sc;
    u32 channel;
    u32 res;
    rtems_event_set eventsOut;
    rtems_event_set error = 0;

    assert(channelsActive != NULL);

    OS_DPRINTF3("osDrvAhbDmaWaitTransferFinish: waiting for events 0x%x\n", channels);

    // Wait for events
    switch(timeout) {
    case OS_DRV_AHB_WAIT_FOREVER: // standard wait
		sc = OsCommonEventReceive(channels, FALSE, TRUE, RTEMS_NO_TIMEOUT, &eventsOut, &osCommonIrqSharedInfo.irqShared, OSCOMMON_EVENT_AHBDMA);
        break;
    case 0: // We do not wait not a single tick
		sc = OsCommonEventReceive(channels, FALSE, FALSE, 0, &eventsOut, &osCommonIrqSharedInfo.irqShared, OSCOMMON_EVENT_AHBDMA);
        break;
    default: // We wait exactly for the time specified by the user
		sc = OsCommonEventReceive(channels, FALSE, TRUE, timeout, &eventsOut, &osCommonIrqSharedInfo.irqShared, OSCOMMON_EVENT_AHBDMA);
        break;
    }

    *channelsActive = channels & ~(eventsOut);

    switch (sc)
    {
    case RTEMS_SUCCESSFUL:
        channel = 0;
        while (eventsOut > 0)
        {
            // execute transfer done callback for each channel that registered one
            if ((eventsOut & 0x1) && (ahbDmaHandler[channel] != NULL))
            {
                //check for a pending error event on the current channel
				OsCommonEventReceive(AHB_DMA_ERROR(channel), FALSE, FALSE, 0, &error, &osCommonIrqSharedInfo.irqShared, OSCOMMON_EVENT_AHBDMA);
                if (error)
                {
                    res = OS_DRV_AHB_DMA_LOCK(channel);
                    if (res == OS_MYR_DRV_SUCCESS)
                    {
                        osAhbDmaChannelControl[channel].status = OS_DRV_AHB_DMA_ERROR;
                    }
                }
                else
                {
                    res = OS_DRV_AHB_DMA_LOCK(channel);
                    if (res == OS_MYR_DRV_SUCCESS)
                    {
                        osAhbDmaChannelControl[channel].status = OS_DRV_AHB_DMA_TAKEN;
                    }
                }
                OS_DRV_AHB_DMA_UNLOCK(channel);
            }
            channel++;
            eventsOut >>= 1;
        }
        res = OS_MYR_DRV_SUCCESS;
        break;
    case RTEMS_TIMEOUT:
        res = OS_MYR_DRV_TIMEOUT;
        break;
    default:
        res = OS_MYR_DRV_ERROR;
    }

    return res;
}

int OsDrvAhbDmaWaitTransferFinish(u32 noOfChannels, osDrvAhbDmaHandler_t* channelList,
                                  u32 timeout, u32* channelsActive )
{
    u32 i;
    u32 channels = 0;

    if (noOfChannels > DRV_AHB_DMA_CHANNEL_NO || channelsActive == NULL)
        return OS_DRV_AHB_DMA_ERROR;

    for (i = 0; i < noOfChannels; i++)
    {
        if (osAhbDmaChannelControl[channelList[i].channel].status == OS_DRV_AHB_DMA_RUNNING)
        {
            if (osAhbDmaChannelControl[channelList[i].channel].taskId == rtems_task_self())
            {
                channels |= (1 << channelList[i].channel);
            }
            else
            {
                // return the first channel for which the wait fails
                *channelsActive = (1 << channelList[i].channel);
                return OS_MYR_RESOURCE_NOT_OWNED;

            }
        }
    }
	OS_DPRINTF3("OsDrvAhbDmaWaitTransferFinish: waiting for events 0x%x\n", channels);
    return osDrvAhbDmaWaitTransferFinish(channels, timeout, channelsActive);
}

int OsDrvAhbDmaInit(u32 interruptLevel)
{
    rtems_status_code status;
    int dmaStatus;

    OS_MYRIAD_DRIVER_STATUS_CODE res;
    if (osAhbDmaControl.status == OS_DRV_STAT_UNINITIALIZED)
    {
        status = rtems_semaphore_create( rtems_build_name('A', 'H', 'B', 'S'), 1, RTEMS_BINARY_SEMAPHORE, 1,&osAhbDmaControl.controlMutex);

        if (status == RTEMS_SUCCESSFUL)
        {
            osAhbDmaControl.status = OS_DRV_STAT_INITIALIZED;
            dmaStatus = DrvAhbDmaOpen();

            switch (dmaStatus)
            {
            case AHB_DMA_DRV_SUCCESS:
                // interrupt configuration
                BSP_Clear_interrupt(IRQ_AHB_DMA);
                BSP_Set_interrupt_type_priority(IRQ_AHB_DMA, POS_LEVEL_INT, interruptLevel);
                BSP_interrupt_register(IRQ_AHB_DMA, NULL, (rtems_interrupt_handler) ahbDmaIrqHandler, NULL);
                res = OS_MYR_DRV_SUCCESS;
                break;
            case AHB_DMA_DRV_ALREADY_INITIALIZED:
                res = OS_MYR_DRV_ALREADY_INITIALIZED;
                break;
            default:
                res = OS_MYR_DRV_ERROR;
                break;
            }
        }
        else
            res = OS_MYR_DRV_ERROR;
    }
    else
    {
        res = OS_MYR_DRV_ALREADY_INITIALIZED;
    }

    OS_DPRINTF1("OsDrvAhbDmaInit() res %d\n", res);
    return res;  //finally returning as int ? (TODO check)
}

// transferDoneCallback parameter is not used for now; it will be used
// in the future to provide callbacks in thread context
int OsDrvAhbDmaOpenChannel(osDrvAhbDmaHandler_t* handler,
                           u32 channel,
                           ahbDmaTransferDoneCallback_t* transferDoneCallback,
                           OS_MYRIAD_DRIVER_PROTECTION protection)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    ahbDmaTransferDoneCallback_t* cb;
    int status;
    u32 i;

    res = OS_DRV_AHB_DMA_MUTEX_LOCK();
    if (res == OS_MYR_DRV_SUCCESS)
    {
        //check if the handler is already used
        for (i = 0; i < DRV_AHB_DMA_CHANNEL_NO; i++)
        {
            if (ahbDmaHandler[i] == handler)
            {
                OS_DRV_AHB_DMA_MUTEX_UNLOCK();
                return OS_MYR_DRV_RESOURCE_BUSY;
            }
        }

        if (osAhbDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            if (osAhbDmaChannelControl[channel].status == OS_DRV_AHB_DMA_FREE)
            {
                osAhbDmaChannelControl[channel].protection = protection;
                if (protection == OS_MYR_PROTECTION_CUSTOM_SEM)
                {
                    status = rtems_semaphore_create( rtems_build_name('A', 'H', 'B', '0' + channel), 1,
                            RTEMS_SIMPLE_BINARY_SEMAPHORE, 1, &osAhbDmaChannelControl[channel].channelMutex);
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
                }
                if (res == OS_MYR_DRV_SUCCESS)
                {
                    osAhbDmaChannelControl[channel].taskId = rtems_task_self();
                    osAhbDmaControl.openedChannelsNo++;
                    cb = transferDoneCallback?transferDoneCallback:osAhbDmaTransferDoneCallback;
                    status = DrvAhbDmaOpenChannel(handler, channel, cb);
                    if (status == AHB_DMA_CHANNEL_SUCCESS)
                    {
                        osAhbDmaChannelControl[channel].status = OS_DRV_AHB_DMA_TAKEN;
                    }
                    res = OSDRV_HW_DRV_CHECK_SUCCESS(status);
                }
            }
            else
            {
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_AHB_DMA_MUTEX_UNLOCK();
    }
    OS_DPRINTF1("OsDrvAhbDmaOpenChannel() channel %d res %d\n", channel, res);

    return res;
}

int OsDrvAhbDmaChannelConfig(osDrvAhbDmaHandler_t* handler,
                drvAhbDmaTrWidth_t dstTrWidth, drvAhbDmaTrWidth_t srcTrWidth,
                drvAhbDmaInc_t dstInc, drvAhbDmaInc_t srcInc,
                drvAhbDmaBurstSize_t dstBurstSize, drvAhbDmaBurstSize_t srcBurstSize,
                drvAhbDmaCtlTransferType_t transferType, u32 priority)
{
    int status;
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;

    res = OS_DRV_AHB_DMA_LOCK(handler->channel);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (osAhbDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_TAKEN)
            {
                status = DrvAhbDmaChannelConfig(handler, dstTrWidth, srcTrWidth,
                        dstInc, srcInc, dstBurstSize, srcBurstSize,
                        transferType, priority);
                res = OSDRV_HW_DRV_CHECK_SUCCESS(status);
            }
            else
            {
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_AHB_DMA_UNLOCK(handler->channel);
    }
    OS_DPRINTF1("OsDrvAhbDmaChannelConfig() res %d\n", res);

    return res;
}

int OsDrvAhbDmaSingleBlockTransfer(osDrvAhbDmaHandler_t* handler, void* src, void* dst, u32 transferSize, u32 enAndWait)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    u32 unused;
    int status;

    res = OS_DRV_AHB_DMA_LOCK(handler->channel);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (osAhbDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_TAKEN)
            {
                osAhbDmaChannelControl[handler->channel].status = OS_DRV_AHB_DMA_RUNNING;
                if ((osAhbDmaChannelControl[handler->channel].protection == OS_MYR_PROTECTION_SEM) ||
                    (osAhbDmaChannelControl[handler->channel].protection == OS_MYR_PROTECTION_CUSTOM_SEM))
                {
                    // only needed when mutex protection is used; in this case functions can
                    // be called from different threads, so taskId needs to be updated
                    osAhbDmaChannelControl[handler->channel].taskId = rtems_task_self();
                }
                status = DrvAhbDmaSingleBlockTransfer(handler, src, dst, transferSize);
                OS_DRV_AHB_DMA_UNLOCK(handler->channel);
                if ((status == AHB_DMA_DRV_SUCCESS) && (enAndWait == DRV_AHB_DMA_START_WAIT))
                {
                    res = osDrvAhbDmaWaitTransferFinish(AHB_DMA_EVENT(handler->channel), OS_DRV_AHB_WAIT_FOREVER, &unused);
                }
                else
                {
                    res = OSDRV_MAP_HW_ERROR(status);
                }
            }
            else
            {
                OS_DRV_AHB_DMA_UNLOCK(handler->channel);
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
        }
        else
        {
            OS_DRV_AHB_DMA_UNLOCK(handler->channel);
            res = OS_MYR_DRV_NOTOPENED;
        }
    }
    OS_DPRINTF1("OsDrvAhbDmaSingleBlockTransfer() transfer size %d res %d\n", transferSize, res);
    return res;
}

int OsDrvAhbDmaAutoreloadTransfer(osDrvAhbDmaHandler_t* handler, void* src, void* dst,
            u32 transferSize, u32 blockSize, ahbDmaAutoreload_t reload,  u32 enAndWait)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    u32 unused;
    int status;

    res = OS_DRV_AHB_DMA_LOCK(handler->channel);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (osAhbDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_TAKEN)
            {
                osAhbDmaChannelControl[handler->channel].status = OS_DRV_AHB_DMA_RUNNING;
                if (osAhbDmaChannelControl[handler->channel].protection == OS_MYR_PROTECTION_SEM)
                {
                    // only needed when mutex protection is used; in this case functions can
                    // be called from different threads, so taskId needs to be updated
                    osAhbDmaChannelControl[handler->channel].taskId = rtems_task_self();
                }
                status = DrvAhbDmaAutoreloadTransfer(handler, src, dst, transferSize, blockSize, reload);
                OS_DRV_AHB_DMA_UNLOCK(handler->channel);
                if ((status == AHB_DMA_DRV_SUCCESS) && (enAndWait == DRV_AHB_DMA_START_WAIT))
                {
                    osDrvAhbDmaWaitTransferFinish(AHB_DMA_EVENT(handler->channel), OS_DRV_AHB_WAIT_FOREVER, &unused);
                }
                else
                {
                    res = OSDRV_MAP_HW_ERROR(status);
                }
            }
            else
            {
                OS_DRV_AHB_DMA_UNLOCK(handler->channel);
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
        }
        else
        {
            OS_DRV_AHB_DMA_UNLOCK(handler->channel);
            res = OS_MYR_DRV_NOTOPENED;
        }
    }
    OS_DPRINTF1("OsDrvAhbDmaAutoreloadTransfer() transfer size %d block size %d res %d\n", transferSize, res, blockSize);
    return res;
}


int OsDrvAhbDmaSetHs(osDrvAhbDmaHandler_t* handler, ahbDmaHsSel_t dstHsSel,
        ahbDmaHsSel_t srcHsSel, u32 dstHwHs, u32 srcHwHs)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    int status;

    res = OS_DRV_AHB_DMA_LOCK(handler->channel);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (osAhbDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_TAKEN)
            {
                status = DrvAhbDmaSetHs(handler, dstHsSel, srcHsSel, dstHwHs, srcHwHs);
                res = OSDRV_HW_DRV_CHECK_SUCCESS(status);
            }
            else if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_FREE)
            {
                res = OS_MYR_DRV_NOTOPENED;
            }
            else if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_RUNNING)
            {
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
            else
            {
                res = OS_MYR_DRV_ERROR;
            }
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_AHB_DMA_UNLOCK(handler->channel);
    }
    OS_DPRINTF1("OsDrvAhbDmaSetHs() res %d\n", res);

    return res;
}

int OsDrvAhbDmaDstScatterConfig(osDrvAhbDmaHandler_t* handler, u32 dstScatterCount, u32 dstScatterInterval)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    int status;

    res = OS_DRV_AHB_DMA_LOCK(handler->channel);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (osAhbDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_TAKEN)
            {
                status = DrvAhbDmaDstScatterConfig(handler, dstScatterCount, dstScatterInterval);
                res = OSDRV_HW_DRV_CHECK_SUCCESS(status);
            }
            else if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_FREE)
            {
                res = OS_MYR_DRV_NOTOPENED;
            }
            else if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_RUNNING)
            {
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
            else
            {
                res = OS_MYR_DRV_ERROR;
            }
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_AHB_DMA_UNLOCK(handler->channel);
    }
    OS_DPRINTF1("OsDrvAhbDmaSetHs() count %d interval %d res %d\n", dstScatterCount, dstScatterInterval, res);

    return res;
}

int OsDrvAhbDmaDstScatterDisable(osDrvAhbDmaHandler_t* handler)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    int status;

    res = OS_DRV_AHB_DMA_LOCK(handler->channel);
    if(res == OS_MYR_DRV_SUCCESS)
    {
        if (osAhbDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_TAKEN)
            {
                status = DrvAhbDmaDstScatterDisable(handler);
                res = OSDRV_HW_DRV_CHECK_SUCCESS(status);
            }
            else if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_FREE)
            {
                res = OS_MYR_DRV_NOTOPENED;
            }
            else if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_RUNNING)
            {
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
            else
            {
                res = OS_MYR_DRV_ERROR;
            }
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_AHB_DMA_UNLOCK(handler->channel);
    }
    OS_DPRINTF1("OsDrvAhbDmaDstScatterDisable() res %d\n", res);

    return res;
}


int OsDrvAhbDmaSrcGatherConfig(osDrvAhbDmaHandler_t* handler, u32 srcGatherCount, u32 srcGatherInterval)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    int status;

    res = OS_DRV_AHB_DMA_LOCK(handler->channel);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (osAhbDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_TAKEN)
            {
                status = DrvAhbDmaSrcGatherConfig(handler, srcGatherCount, srcGatherInterval);
                res = OSDRV_HW_DRV_CHECK_SUCCESS(status);
            }
            else if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_FREE)
            {
                res = OS_MYR_DRV_NOTOPENED;
            }
            else if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_RUNNING)
            {
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
            else
            {
                res = OS_MYR_DRV_ERROR;
            }
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_AHB_DMA_UNLOCK(handler->channel);
    }
    OS_DPRINTF1("OsDrvAhbDmaSrcGatherConfig() count %d interval %d res %d\n", srcGatherCount, srcGatherInterval, res);
    return res;
}

int OsDrvAhbDmaSrcGatherDisable(osDrvAhbDmaHandler_t* handler)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    int status;

    res = OS_DRV_AHB_DMA_LOCK(handler->channel);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (osAhbDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_TAKEN)
            {
                status = DrvAhbDmaSrcGatherDisable(handler);
                res = OSDRV_HW_DRV_CHECK_SUCCESS(status);
            }
            else if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_FREE)
            {
                res = OS_MYR_DRV_NOTOPENED;
            }
            else if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_RUNNING)
            {
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
            else
            {
                res = OS_MYR_DRV_ERROR;
            }
        }
        else
        {
            res = OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_AHB_DMA_UNLOCK(handler->channel);
    }
    OS_DPRINTF1("OsDrvAhbDmaSrcGatherDisable() res %d\n", res);

    return res;
}

int OsDrvAhbDmaMemCopy(void* src, void* dst, s32 size)
{
    osDrvAhbDmaHandler_t handler;
    s32 transferedSize = 0;
    OS_MYRIAD_DRIVER_STATUS_CODE res;

    if (osAhbDmaControl.status == OS_DRV_STAT_INITIALIZED)
    {
        // get a free channel
        res = OsDrvAhbDmaOpenAnyChannel(&handler, NULL, OS_MYR_PROTECTION_SELF);
        if (res != OS_MYR_DRV_SUCCESS)
        {
            return res;
        }
        res = OsDrvAhbDmaChannelConfig(&handler,
                AHB_DMA_CTL_TR_WIDTH_8,
                AHB_DMA_CTL_TR_WIDTH_8, AHB_DMA_CTL_INC, AHB_DMA_CTL_INC,
                AHB_DMA_CTL_MSIZE_8, AHB_DMA_CTL_MSIZE_8, AHB_DMA_CTL_TT_FC_D_M2M,
                AHB_DMA_DEFAULT_CHANEL_PRIO);
        if (res == OS_MYR_DRV_SUCCESS)
        {
            while (size > 0)
            {
                if(size < DRV_AHB_MAX_BLOCK_SIZE)
                {
                    res = OsDrvAhbDmaSingleBlockTransfer(&handler, src + transferedSize, dst + transferedSize, size, DRV_AHB_DMA_START_WAIT);
                    if (res != OS_MYR_DRV_SUCCESS)
                    {
                        return res;
                    }
                    size = 0;
                    transferedSize += size;
                }
                else
                {
                    res = OsDrvAhbDmaSingleBlockTransfer(&handler, src + transferedSize, dst + transferedSize, DRV_AHB_MAX_BLOCK_SIZE, DRV_AHB_DMA_START_WAIT);
                    if (res != OS_MYR_DRV_SUCCESS)
                    {
                        return res;
                    }
                    size -= DRV_AHB_MAX_BLOCK_SIZE;
                    transferedSize += DRV_AHB_MAX_BLOCK_SIZE;
                }
            }
        }
        else
        {
            return res;
        }
        return OsDrvAhbDmaCloseChannel(&handler);
    }
    else
    {
        return OS_MYR_DRV_NOTOPENED;
    }
}

int OsDrvAhbDmaCloseChannel(osDrvAhbDmaHandler_t* handler)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;
    int status;

    res = OS_DRV_AHB_DMA_LOCK(handler->channel);
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (osAhbDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_RUNNING)
            {
                res =  OS_MYR_DRV_RESOURCE_BUSY;
            }
            else if (osAhbDmaChannelControl[handler->channel].status == OS_DRV_AHB_DMA_FREE)
            {
                res =  OS_MYR_DRV_NOTOPENED;
            }
            else
            {
                status = DrvAhbDmaCloseChannel(handler);
                res = OSDRV_HW_DRV_CHECK_SUCCESS(status);

                if (res == OS_MYR_DRV_SUCCESS)
                {
                    res = OS_DRV_AHB_DMA_MUTEX_LOCK();
                    if (res == OS_MYR_DRV_SUCCESS)
                    {
                        osAhbDmaChannelControl[handler->channel].status = OS_DRV_AHB_DMA_FREE;
                        osAhbDmaControl.openedChannelsNo--;
                        if (osAhbDmaChannelControl[handler->channel].protection == OS_MYR_PROTECTION_CUSTOM_SEM)
                        {
                            rtems_semaphore_delete(osAhbDmaChannelControl[handler->channel].channelMutex);
                        }
                        OS_DRV_AHB_DMA_MUTEX_UNLOCK();
                    }
                }
            }
        }
        else
        {
            res =  OS_MYR_DRV_NOTOPENED;
        }
        OS_DRV_AHB_DMA_UNLOCK(handler->channel);
    }
    OS_DPRINTF1("OsDrvAhbDmaCloseChannel() channel %d res %d\n", handler->channel, res);

    return res;
}

int OsDrvAhbDmaClose(void)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_ERROR;

    res = OS_DRV_AHB_DMA_MUTEX_LOCK();
    if (res == OS_MYR_DRV_SUCCESS)
    {
        if (osAhbDmaControl.status == OS_DRV_STAT_INITIALIZED)
        {
            // check the number of opened channels. If all the channels are closed
            // then it is safe to release the driver mutex
            if (osAhbDmaControl.openedChannelsNo == 0)
            {
                osAhbDmaControl.status = OS_DRV_STAT_UNINITIALIZED;
                // mutex can not be released while it is taken
                OS_DRV_AHB_DMA_MUTEX_UNLOCK();
                rtems_semaphore_delete(osAhbDmaControl.controlMutex);
            }
            else
            {
                OS_DRV_AHB_DMA_MUTEX_UNLOCK();
                // some channels are still opened
                res = OS_MYR_DRV_RESOURCE_BUSY;
            }
        }
        else
        {
            OS_DRV_AHB_DMA_MUTEX_UNLOCK();
            res = OS_MYR_DRV_NOTOPENED;
        }
    }
    OS_DPRINTF1("OsDrvAhbDmaClose() res %d\n", res);

    return res;
}

///@}
