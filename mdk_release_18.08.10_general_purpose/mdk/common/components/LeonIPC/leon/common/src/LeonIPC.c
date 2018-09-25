///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Sensor Functions.
///
/// This is the implementation of a Sensor library.
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <swcLeonUtils.h>
#include <DrvIcb.h>
#include "LeonIPCApi.h"
#include "OsCommon.h"
#include <rtems.h>
#include "OsCross.h"

// Drivers includes

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
//Number of maximum size of a message the leon can handle
#define MAX_BODY_SIZE 20
#define IPC_EVENT(src) (1 << ((src) < 32 ? (src) : (src) - 32))

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
osCommonIrqShareInfo_t osCrossInformation;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static leonIPCChannel_t* __attribute__((section(".cmx_direct.data"))) ipcConsumer[ICB_INT_NO];

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
static void LeonIPCMessagingISR(uint32_t irqSrc)
{
    if (ipcConsumer[irqSrc]->protoCallBack == NULL)
    {
        //send an event if no callback was registered
        rtems_event_send(ipcConsumer[irqSrc]->ipcTaskId, IPC_EVENT(irqSrc));
    }
    else
    {
        //execute the callback provided by the user
        ipcConsumer[irqSrc]->protoCallBack((struct leonIPCChannel_t*)ipcConsumer[irqSrc]);
    }
}

int LeonIPCTxInit(leonIPCChannel_t* channel, uint32_t* messagePool, uint32_t messagePoolSize, uint32_t messageSize)
{
    uint32_t status;

    if (channel->txInitialized)
    {
        return IPC_TX_ALREADY_INITIALIZED;
    }
    status = MessageQueueInit(&channel->msgQueue, messagePool, messagePoolSize, messageSize);
    if (status == MSG_QUEUE_SUCCESS)
    {
        //No posting untill configured for one
        channel->msgType = IPC_POSTING_OFF;

        //No callback until configured for one
        channel->protoCallBack = NULL;

        // mark the Tx side as initalized
        channel->txInitialized = 1;
    }

    return status;
}

int LeonIPCRxReassignSinkThread(leonIPCChannel_t* channel)
{
    if (!channel->rxInitialized)
    {
        return IPC_RX_NOTINITIALIZED;
    }
    if (!channel->txInitialized)
    {
        return IPC_TX_NOTINITIALIZED;
    }

    if (channel->ipcTaskId == 0)
    {
        channel->ipcTaskId = rtems_task_self();
        return IPC_SUCCESS;
    }
    else
    {
        return RTEMS_NOT_OWNER_OF_RESOURCE;
    }

    return IPC_SUCCESS;
}

int LeonIPCRxReleaseSinkThread(leonIPCChannel_t* channel)
{
    if (!channel->rxInitialized)
    {
        return IPC_RX_NOTINITIALIZED;
    }
    if (!channel->txInitialized)
    {
        return IPC_TX_NOTINITIALIZED;
    }

    if (channel->ipcTaskId == rtems_task_self())
    {
        channel->ipcTaskId = 0;
        return IPC_SUCCESS;
    }
    else
    {
        return RTEMS_NOT_OWNER_OF_RESOURCE;
    }
}

int LeonIPCRxInit(leonIPCChannel_t* channel, leonIPCCallback_t msgCallback, uint32_t irqNo, uint32_t irqPrio)
{
    int status;

    if (channel->rxInitialized)
    {
        return IPC_RX_ALREADY_INITIALIZED;
    }
    if (channel->txInitialized)
    {
        //TODO find a way to remove the following the dummy queue init
        //dummy call to MessageQueueInit to create a semaphore if it is needed
        status = MessageQueueInit(&channel->msgQueue, NULL, 0, 0);
        if (status != MSG_QUEUE_ALREADY_INITIALIZED)
        {
            return status;
        }

        channel->protoCallBack = msgCallback;
        if (msgCallback == NULL)
        {
            channel->msgType = IPC_POSTING_OFF;
        }
        else
        {
            // interrupt setup
            channel->msgType = IPC_POSTING_IRQ;
        }
        channel->irqNo = irqNo;
        channel->irqPrio = irqPrio;
        channel->ipcTaskId = rtems_task_self();
        ipcConsumer[irqNo] = channel;
        //interrupt setup
        BSP_Clear_interrupt(irqNo);
        BSP_Set_interrupt_type_priority(irqNo, POS_EDGE_INT, irqPrio);
        BSP_interrupt_register(irqNo, NULL, (rtems_interrupt_handler)LeonIPCMessagingISR, (void*)irqNo);
        channel->rxInitialized = 1;
        status = IPC_SUCCESS;
    }
    else
    {
        status = IPC_TX_NOTINITIALIZED;
    }
    return status;
}

int LeonIPCSendMessage(leonIPCChannel_t* channel, uint32_t* message)
{
    int res, status;

    if (!(channel->rxInitialized))
    {
        return IPC_RX_NOTINITIALIZED;
    }
    status = MessageQueuePush(&channel->msgQueue, message);
    switch (status)
    {
        case MSG_QUEUE_SUCCESS:
            // If this is an async type of messaging, signal this to the consumer
            BSP_Force_remote_interrupt(channel->irqNo);
            res = IPC_SUCCESS;
            break;
        case MSG_QUEUE_OVERFLOW:
            res = IPC_QUEUE_OVERFLOW;
            break;
        default:
            res = status;
            break;
    }
    return res;
}

int LeonIPCWaitMessage(leonIPCChannel_t* channel, uint32_t timeout)
{
    int status;
    int res = IPC_SUCCESS;
    uint32_t eventsOut;

    if (!(channel->rxInitialized))
    {
        return IPC_RX_NOTINITIALIZED;
    }

    //the events are sent only when no callback are used
    if (channel->msgType == IPC_POSTING_OFF)
    {
        switch(timeout)
        {
        case IPC_WAIT_FOREVER: // standard wait
            status = rtems_event_receive(IPC_EVENT(channel->irqNo), RTEMS_WAIT | RTEMS_EVENT_ALL, RTEMS_NO_TIMEOUT, &eventsOut);
            break;
        case 0: // We do not wait not a single tick
            status = rtems_event_receive(IPC_EVENT(channel->irqNo), RTEMS_NO_WAIT | RTEMS_EVENT_ALL, 0, &eventsOut);
            break;
        default: // We wait exactly for the time specified by the user
            status = rtems_event_receive(IPC_EVENT(channel->irqNo), RTEMS_WAIT | RTEMS_EVENT_ALL, timeout, &eventsOut);
            break;
        }

        switch(status)
        {
        case RTEMS_SUCCESSFUL:
            res = IPC_SUCCESS;
            break;
        case RTEMS_TIMEOUT:
            res = IPC_TIMEOUT;
            break;
        default:
            res = status;
            break;
        }
    }
    return res;
}

int LeonIPCNumberOfPendingMessages(leonIPCChannel_t* channel, uint32_t* noOfmessages)
{
    if (!(channel->txInitialized))
    {
        return IPC_TX_NOTINITIALIZED;
    }
    return MessageQueueGetNoOfElements(&channel->msgQueue, noOfmessages);
}

int LeonIPCReadMessage(leonIPCChannel_t* channel, uint32_t* message)
{
    int res, status;
    assert(message != NULL);

    if (!(channel->txInitialized))
    {
        return IPC_TX_NOTINITIALIZED;
    }
    status = MessageQueuePop(&channel->msgQueue, message);
    switch (status)
    {
        case MSG_QUEUE_SUCCESS:
            res = IPC_SUCCESS;
            break;
        case MSG_QUEUE_EMPTY:
            res = IPC_QUEUE_EMPTY;
            break;
        default:
            res = status;
            break;
    }
    return res;
}
