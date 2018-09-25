///
/// @file      SendOutTransport.c
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Send Output Internal functions
///            Will be use by HDMI, MIPI, USB Send Output module in order to manage
///            the thread in queue streaming list
///            Platform(s) supported : ma2x8x
///

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "SendOutTransport.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <rtems.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <sched.h>
#include <assert.h>
#include "FrameTypes.h"


/**************************************************************************************************
 ~~~  Specific #defines
**************************************************************************************************/
//#define SEND_OUT_DBG_DUMP

/**************************************************************************************************
 ~~~  Global Data (Only if absolutely necessary)
**************************************************************************************************/
uint8_t ddrBlackUV[SEND_OUT_MAX_WIDTH*SEND_OUT_MAX_HEIGHT>>2]
                   __attribute__((section(".ddr.bss")))  __attribute__((aligned(8)));


/**************************************************************************************************
 ~~~  Local variables
**************************************************************************************************/


/**************************************************************************************************
 ~~~ Local File function declarations
 **************************************************************************************************/
static void* sendOutTxThreadFunc(void* arg);
static int   sendOutQueuePut(SendOutDev *dev, FrameT *buffer, uint32_t outId, uint32_t type, SendOutCbSent sendOutCbSent);
static int   sendOutQueueGet(SendOutDev *dev, SoElem **old);
static int   semWaitTimeout(sem_t *sem, uint32_t ms);

static void  sendOutTxLocCallback(void *arg);

/**************************************************************************************************
 ~~~ Exported Functions Implementation
 **************************************************************************************************/


void sendOutTrCreate(SendOutDev *dev) {
    memset(ddrBlackUV, 0x80, sizeof(ddrBlackUV));
    memset((void*)dev, 0, sizeof(SendOutDev));
    dev->enableCam  = (uint32_t)(-1);
    dev->enableFmt  = (uint32_t)(-1);
    dev->enableType = (uint32_t)(-1);
    dev->blackUVBuf = ddrBlackUV;
}

void sendOutTrInit(SendOutDev *dev) {
    dev->que.queueIn = dev->que.queueOut = 0;
    dev->que.tx_state = TX_MODULE_STATE__IDLE;
    if (sem_init(&dev->que.q_sync, 0, 1) != RTEMS_SUCCESSFUL) {
        printf("Failed to create message tx_sync semaphore\n");
        goto exit0;
    }

    SoThr *thr = &dev->thrCtrl;
    thr->state = TX_THREAD__ACTIVE;
    if (sem_init(&thr->control_sem, 0, 0) != RTEMS_SUCCESSFUL) {
        printf("Failed to create message control semaphore\n");
        goto exit1;
    }
    if (sem_init(&thr->tx_sync, 0, 0) != RTEMS_SUCCESSFUL) {
        printf("Failed to create message tx_sync semaphore\n");
        goto exit2;
    }

    pthread_attr_t attr;
    struct sched_param param;
    assert(RTEMS_SUCCESSFUL == pthread_attr_init           (&attr));
    assert(RTEMS_SUCCESSFUL == pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
    assert(RTEMS_SUCCESSFUL == pthread_attr_setschedpolicy (&attr, SCHED_RR));
    assert(RTEMS_SUCCESSFUL == pthread_attr_getschedparam (&attr, &param));
    param.sched_priority = SEND_OUT_TX_THREAD_PRIORITY;
    assert(RTEMS_SUCCESSFUL == pthread_attr_setschedparam (&attr, &param));
    assert(RTEMS_SUCCESSFUL == pthread_create(&thr->thread, &attr, sendOutTxThreadFunc, dev));
    assert(RTEMS_SUCCESSFUL == pthread_attr_destroy   (&attr));
    return;

//exit3:
//    sem_destroy(&thr->tx_sync);
exit2:
    sem_destroy(&thr->control_sem);
exit1:
    sem_destroy(&dev->que.q_sync);
exit0:
    assert(0); // Impossible to create Send output module
return;
}

void sendOutTrControl(SendOutDev *dev, uint32_t camEnBitMask, uint32_t frmTypeEnBitMask, uint32_t frmFmtEnBitMask) {
    printf("sendOut Ctrl: CAM 0x%x TYPE 0x%x FMT 0x%x\n", (unsigned int)camEnBitMask,
            (unsigned int)frmTypeEnBitMask, (unsigned int)frmFmtEnBitMask);
    dev->enableCam  = camEnBitMask;
    dev->enableType = frmTypeEnBitMask;
    dev->enableFmt  = frmFmtEnBitMask;
}


void sendOutTrSend(SendOutDev *dev, FrameT *frame, uint32_t outputId, uint32_t frmType, SendOutCbSent sendOutCbSent) {
    if (sendOutIsDisable(dev, outputId, frmType, frame->type)) {
        if (sendOutCbSent) {
            sendOutCbSent(frame, outputId, frmType);
        }
        return;
    }

#ifdef SEND_OUT_DBG_DUMP
    static void dumpOutputFrame(FrameT *frame);
    dumpOutputFrame(frame);
#endif

    if (sendOutQueuePut(dev, frame, outputId, frmType, sendOutCbSent)) {
        if (sendOutCbSent) {
            sendOutCbSent(frame, outputId, frmType);
        }
        return;
    }
    assert(RTEMS_SUCCESSFUL == sem_post(&dev->thrCtrl.control_sem));
}

// Function calling de-init function for configured output
void sendOutTrFini(SendOutDev *dev) {
    dev->thrCtrl.state = TX_THREAD__STOPPING;
    sem_post(&dev->thrCtrl.control_sem);    // Make sure thread will see this request
    assert(RTEMS_SUCCESSFUL == pthread_join(dev->thrCtrl.thread, NULL));
    assert(RTEMS_SUCCESSFUL == sem_destroy(&dev->thrCtrl.control_sem));
    assert(RTEMS_SUCCESSFUL == sem_destroy(&dev->thrCtrl.tx_sync));
    assert(RTEMS_SUCCESSFUL == sem_destroy(&dev->que.q_sync));
}

/**************************************************************************************************
 ~~~ Local File Functions Implementation
 **************************************************************************************************/
//
static void * sendOutTxThreadFunc(void* arg)
{
    SendOutDev* dev = (SendOutDev*)arg;
    int err;
    for (;;) {
        err = sem_wait(&dev->thrCtrl.control_sem);
        if (err) {
            printf("SendOut: Failed to obtain control_sem: err=%d", err);
        }
        if (dev->thrCtrl.state == TX_THREAD__STOPPING)
            break;

        {
            SoElem *taskDescriptor;
            if(sendOutQueueGet(dev, &taskDescriptor)) {
                // there is a new element in queue
            }
            else {
                dev->thrCtrl.sendFrame(taskDescriptor);

                err = semWaitTimeout(&dev->thrCtrl.tx_sync, TX_PACKET_SEND_TIMEOUT); // wait transfer to complete
                if (err != 0) printf("Timeout SendOut %d\n", err);
                //    assert(err == 0);
                if (taskDescriptor->sendOutCbSent) {  // call client's callback
                    taskDescriptor->sendOutCbSent(taskDescriptor->buffer, taskDescriptor->outId, taskDescriptor->frmType);
                }
            }

        }
    }
    // flush the que
    {
        SoElem *taskDescriptor;
        if(0 == sendOutQueueGet(dev, &taskDescriptor)) {
            if (taskDescriptor->sendOutCbSent)  {// call client's callback
                taskDescriptor->sendOutCbSent(taskDescriptor->buffer, taskDescriptor->outId, taskDescriptor->frmType);
            }
        }
    }
    return NULL;
}
//
static int  sendOutQueuePut(SendOutDev *dev, FrameT *buffer, uint32_t outId, uint32_t type, SendOutCbSent sendOutCbSent) {
    SoElem * task;
    sem_wait(&dev->que.q_sync);
    if(dev->que.queueIn == (( dev->que.queueOut - 1 +
                                  MAX_NR_OF_OUTPUTS_PENDING) % MAX_NR_OF_OUTPUTS_PENDING)) {
        sem_post(&dev->que.q_sync);
        return -1; /* Queue Full*/
    }
    task = &dev->que.queue[dev->que.queueIn];
    dev->que.queueIn = (dev->que.queueIn + 1) % MAX_NR_OF_OUTPUTS_PENDING;

    task->buffer           = buffer;
    task->outId            = outId;
    task->frmType          = type;
    task->sendOutCbSent    = sendOutCbSent;
    task->localCallback    = sendOutTxLocCallback;
    if (dev->que.tx_state == TX_MODULE_STATE__IDLE) {
        dev->que.tx_state = TX_MODULE_STATE__IDLE_TO_RUN;
    }
    sem_post(&dev->que.q_sync);

    return 0; // No errors
}
//
static int  sendOutQueueGet(SendOutDev *dev, SoElem **old) {
    sem_wait(&dev->que.q_sync);
    if(dev->que.queueIn == dev->que.queueOut) {
        *old = NULL;
        dev->que.tx_state = TX_MODULE_STATE__IDLE;
        sem_post(&dev->que.q_sync);
        return -1; /* Queue Empty - nothing to get*/
    }
    *old = &dev->que.queue[dev->que.queueOut];
    dev->que.queueOut = (dev->que.queueOut + 1) % MAX_NR_OF_OUTPUTS_PENDING;
    dev->que.tx_state = TX_MODULE_STATE__RUN;
    sem_post(&dev->que.q_sync);
    return 0; // No errors
}
//
static int semWaitTimeout(sem_t *sem, uint32_t ms) {
    struct timespec abs_to;
    struct timeval now;
    uint64_t us;

    gettimeofday(&now, NULL);
    us = (uint64_t)now.tv_sec * 1000000 + (uint64_t)now.tv_usec + (uint64_t)ms * 1000;
    abs_to.tv_sec = us / 1000000;
    abs_to.tv_nsec = (us % 1000000) * 1000;
    return sem_timedwait(sem, &abs_to);
}
//
static void sendOutTxLocCallback(void *arg) {
    SendOutDev* dev = (SendOutDev*)arg;
    assert(0 == sem_post(&dev->thrCtrl.tx_sync));
}
//
int sendOutIsDisable(SendOutDev *dev, uint32_t outputId, uint32_t frmType, uint32_t frmFmt) {
    return ((0 == (dev->enableCam & (1<<outputId))) || (0 == (dev->enableFmt & (1<<frmFmt))) ||
                    (0 == (dev->enableType & (1<<frmType))));
}

#ifdef SEND_OUT_DBG_DUMP
uint32_t dbgDumpFrame = 0; // get the commands for dump a frame from debugger
static void dumpOutputFrame(FrameT *frame)
{
    dbgDumpFrame = swcLeonReadNoCacheU32((uint32_t)&dbgDumpFrame);
    if(dbgDumpFrame) {
        if (FRAME_T_FORMAT_422I == frame->type) {
            printf("save %p %ld img_%ldx%ld_I422.yuv \n",
                    frame->fbPtr[0],
                    frame->height[0] * frame->stride[0],
                    frame->stride[0]>>1,
                    frame->height[0]
            );
            printf("savefile img_%ldx%ld_I422.yuv %p %ld \n",
                    frame->stride[0]>>1,
                    frame->height[0],
                    frame->fbPtr[0],
                    frame->height[0] * frame->stride[0]
            );
        }
        else {
            printf("save %p %ld vdLuma_%ldx%ld_P420.yuv \n",
                    frame->fbPtr[0],
                    frame->height[0] * frame->stride[0],
                    frame->stride[0],
                    frame->height[0]
            );
            printf("save %p %ld vdChroma_%ldx%ld_P420.yuv \n",
                    frame->fbPtr[1],
                    frame->height[1] * frame->stride[1],
                    frame->stride[1],
                    frame->height[1]
            );
        }
        dbgDumpFrame = 0;
    }
}
#endif

