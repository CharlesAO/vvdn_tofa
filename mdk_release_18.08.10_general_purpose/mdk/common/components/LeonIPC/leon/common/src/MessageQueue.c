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
#include <rtems.h>
#include "OsCommon.h"
#include <bsp.h>
#include "OsCross.h"

#include <DrvIcb.h>

#include "MessageQueue.h"

// Drivers includes

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#define MSG_QUEUE_LOCK(id)   msgQueueLockCriticalSection(true, RTEMS_NO_TIMEOUT, (rtems_id)id)
#define MSG_QUEUE_UNLOCK(id) msgQueueUnlockCriticalSection((rtems_id)id)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

/* Enter Critical Section */
static int msgQueueLockCriticalSection(uint32_t wait, uint32_t wait_ticks, rtems_id sem)
{
    UNUSED(wait_ticks); //only for BareMetal
    UNUSED(sem);

    rtems_status_code status;
    int res;
    if (wait)
    {
        status = rtems_semaphore_obtain(sem, RTEMS_WAIT, wait_ticks);
    }
    else
    {
        status = rtems_semaphore_obtain(sem, RTEMS_NO_WAIT, 0);
    }    
    
    if (status == RTEMS_SUCCESSFUL)
    {
        res = MSG_QUEUE_SUCCESS;
    }
    else 
    {
        res = status;
    }
    return res;
}

/* Abandon Critical Section */
static int msgQueueUnlockCriticalSection(rtems_id sem)
{
    UNUSED(sem); //only for BareMetal

    rtems_status_code status;
    int res;
    
    status = rtems_semaphore_release(sem);
    if (status == RTEMS_SUCCESSFUL)
    {
        res = MSG_QUEUE_SUCCESS;
    }
    else 
    {
        res = status;
    }
    return res;
}

static inline uint32_t getNumberOfElements(circularMsgQueue_t* handler)
{
    // the number of elements in the queue is the difference between
    // the total number of writes and the total number of reads
    return handler->writeCount - handler->readCount;
}

int MessageQueueInit(circularMsgQueue_t* handler, uint32_t* messagePool, uint32_t messagePoolSize, uint32_t messageSize)
{
    int status;
    int res = MSG_QUEUE_SUCCESS;
        
    if (handler->queueInitialized)
    {
        //create the mutex if it wasn't created yet
        if (handler->mutexIdConsumer == 0)
        {
            status = rtems_semaphore_create( rtems_build_name('Q', 'S', 'E', 'M'), 1,
                RTEMS_SIMPLE_BINARY_SEMAPHORE, 1, (rtems_id*)(&handler->mutexIdConsumer));
            if (status != RTEMS_SUCCESSFUL)
            {
                return status;
            }       
        }
        return MSG_QUEUE_ALREADY_INITIALIZED;
    }
    else
    {
        assert(messagePoolSize > 0);
        assert(messageSize > 0);
        assert(messagePool != NULL);
        
        handler->messagePool = messagePool;
        handler->poolSize = messagePoolSize;
        handler->messageSize = messageSize;
        handler->writeIdx = messagePool;
        handler->readIdx = messagePool;
        handler->readCount = 0;
        handler->writeCount = 0;
        handler->mutexIdProducer = 0;
        handler->queueInitialized = 1;
        
        // create the queue mutex
        status = rtems_semaphore_create( rtems_build_name('Q', 'S', 'E', 'M'), 1,
                RTEMS_SIMPLE_BINARY_SEMAPHORE, 1, (rtems_id*)(&handler->mutexIdProducer));
        if (status != RTEMS_SUCCESSFUL)
        {
            res = status;
        }
    }
    return res;
}

static inline void writeMessage(circularMsgQueue_t* handler, const uint32_t* const message)
{   
    uint32_t i;
    
    for (i = 0; i < handler->messageSize; i++)
    {
         handler->writeIdx[i] = message[i];
    }
}

int MessageQueuePush(circularMsgQueue_t* handler, const uint32_t* const message)
{
    int status;
    
    status = MSG_QUEUE_LOCK(handler->mutexIdProducer);
    if (status == MSG_QUEUE_SUCCESS)
    {
        if (getNumberOfElements(handler) >= handler->poolSize)
        {
            MSG_QUEUE_UNLOCK(handler->mutexIdProducer);
            //the queue is full; generate an overflow error
            return MSG_QUEUE_OVERFLOW;
        }
        
        writeMessage(handler, message);
        
        //calculate the next index
        if (handler->writeIdx + handler->messageSize >= 
            handler->messagePool + (handler->poolSize * handler->messageSize))
        {
            handler->writeIdx = handler->messagePool;
        }
        else
        {
            handler->writeIdx += handler->messageSize;
        }
        //update number of writes
        handler->writeCount++;
        MSG_QUEUE_UNLOCK(handler->mutexIdProducer);
    }
    return status;
}

static inline void readMessage(circularMsgQueue_t* handler, uint32_t* const message)
{
    uint32_t i;

    for (i = 0; i < handler->messageSize; i++)
    {
        message[i] = handler->readIdx[i];
    }
}

int MessageQueuePop(circularMsgQueue_t* handler, uint32_t* const message)
{
    int status;    
    
    status = MSG_QUEUE_LOCK(handler->mutexIdConsumer);
    if (status == MSG_QUEUE_SUCCESS)
    {
        if (getNumberOfElements(handler) == 0)
        {
            MSG_QUEUE_UNLOCK(handler->mutexIdConsumer);
            // no message available in the queue
            return MSG_QUEUE_EMPTY;
        }
        
        readMessage(handler, message);
        
        //calculate the next index
        if (handler->readIdx + handler->messageSize >= 
            handler->messagePool + (handler->poolSize * handler->messageSize))
        {
            handler->readIdx = handler->messagePool;
        }
        else
        {
            handler->readIdx += handler->messageSize;
        }
        // update the number of reads
        handler->readCount++;
        MSG_QUEUE_UNLOCK(handler->mutexIdConsumer);
    }
    return status;
}


int MessageQueueIsEmpty(circularMsgQueue_t* handler, uint32_t* empty)
{
    int status;

    if (handler->queueInitialized)
    {
        *empty = (getNumberOfElements(handler) == 0);
        status = MSG_QUEUE_SUCCESS;
    }
    else
    {
        *empty = 0;
        status = MSG_QUEUE_NOTINITIALIZED;
    }
    return status;
}

int MessageQueueIsFull(circularMsgQueue_t* handler, uint32_t* full)
{
    int status;

    if (handler->queueInitialized)
    {
        *full = (getNumberOfElements(handler) == handler->poolSize);
        status = MSG_QUEUE_SUCCESS;
    }
    else
    {
        *full = 0;
        status = MSG_QUEUE_NOTINITIALIZED;
    }
    return status;
}

int MessageQueueGetNoOfElements(circularMsgQueue_t* handler, uint32_t* noOfMessages)
{
    int status;
    
    if (handler->writeCount < handler->readCount)
    {
        // a writeCount overflow happened: return with an error
        return MSG_QUEUE_SENT_TOO_MANY;
    }

    if (handler->queueInitialized)
    {
        *noOfMessages = getNumberOfElements(handler);
        status = MSG_QUEUE_SUCCESS;
    }
    else
    {
        *noOfMessages = 0;
        status = MSG_QUEUE_NOTINITIALIZED;
    }
    return status;
}
