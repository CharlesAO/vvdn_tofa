
/**************************************************************************************************

 @File         : sendOut.c
 @Brief        : Send Output Interface
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015

 Description :


 **************************************************************************************************/

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include <stdio.h>
#include <string.h>                          //memset
#include <assert.h>
#include "sendOutApi.h"
#include "swcLeonUtils.h"
#include <rtems.h>

#include <semaphore.h>
#include <pthread.h>

#ifdef OUTPUT_UNIT_IS_HDMI
#include "LcdHdmiSend.h"
#endif
#ifdef OUTPUT_UNIT_IS_MIPI
#include "MipiSendApi.h"
#endif
#ifdef OUTPUT_UNIT_IS_USB
#include "UsbSend.h"
#endif

/**************************************************************************************************
 ~~~  Specific #defines
**************************************************************************************************/

#if (!defined(OUTPUT_UNIT_IS_HDMI) && !defined(OUTPUT_UNIT_IS_USB) && !defined(OUTPUT_UNIT_IS_MIPI))
#define OUTPUT_UNIT_IS_MIPI
#endif

#if (defined(OUTPUT_UNIT_IS_HDMI) && defined(OUTPUT_UNIT_IS_MIPI))
#error "HDMI and MIPI outputs can NOT be configured together"
#endif

#define SEND_OUT_CRITICAL_SECTION_ENTER()  sem_wait(sendOutQueue.q_sync);
#define SEND_OUT_CRITICAL_SECTION_EXIT()   sem_post(sendOutQueue.q_sync);

#define SEND_OUTPUT_DBG_DUMP(X) dumpOutputFrame(X)

/**************************************************************************************************
 ~~~  Global Data (Only if absolutely necessary)
**************************************************************************************************/
uint32_t dbgDumpFrame = 0; // get the commands for dump a frame from debugger

uint32_t dbgEnableOutput = 255; // lcd demo related

static uint32_t sndOutEnableCamera = (uint32_t)(-1);  // bit mask to enable sending frame from type FrameTFormatTypes see ImgTypes.h
static uint32_t sndOutEnableFormat = (uint32_t)(-1);  // bit mask to enable sending frame from type FrameTFormatTypes see ImgTypes.h
static uint32_t sndOutEnableType   = (uint32_t)(-1);  // bit mask to enable sending frame from type FrameTFormatTypes see ImgTypes.h
#define IS_DISABLED(CAM, TYPE, FMT) (  \
                                            ((sndOutEnableCamera & (1<<CAM))  == 0) \
                                         || ((sndOutEnableType   & (1<<TYPE)) == 0) \
                                         || ((sndOutEnableFormat & (1<<FMT))  == 0) \
                                    )

#ifdef USE_STATIC_HEADER
static  uint8_t                     sendOutHeaderBuf[SEND_OUT_MAX_WIDTH * SEND_OUT_HEADER_HEIGHT];
static  send_out_tx_buffer_header_t *sendOutHeader        = (send_out_tx_buffer_header_t *)&sendOutHeaderBuf[0u];
#endif

/**************************************************************************************************
 ~~~  Local variables
**************************************************************************************************/

#define MAX_NR_OF_OUTPUTS_PENDING       16


typedef enum {
    TX_MODULE_STATE__IDLE,
    TX_MODULE_STATE__IDLE_TO_RUN,
    TX_MODULE_STATE__RUN,
} tx_module_state_t;

typedef struct {
    SendOutElement_t  queue[MAX_NR_OF_OUTPUTS_PENDING];
    int32_t queueIn;
    int32_t queueOut;
    tx_module_state_t tx_state;
    sem_t *q_sync;
} SendOutSendQue_t;

static SendOutSendQue_t  sendOutQueue;

#ifndef SEND_OUT_TX_THREAD_PRIORITY
#define SEND_OUT_TX_THREAD_PRIORITY     200
#endif

#ifndef SEND_OUT_TX_THREAD_STACK_SIZE
#define SEND_OUT_TX_THREAD_STACK_SIZE   (4*1024)
#endif

#ifndef TX_PACKET_SEND_TIMEOUT
#define TX_PACKET_SEND_TIMEOUT  (10000)
#endif

typedef enum {
    TX_THREAD__IDLE,
    TX_THREAD__ACTIVE,
    TX_THREAD__STOPPING,
} tx_thread_state_t;

typedef struct {
	pthread_t *thread;
    tx_thread_state_t state;
    sem_t *control_sem;
    sem_t *tx_sync;
} send_out_tx_thread_t;

send_out_tx_thread_t send_out_module_storage;

pthread_t send_out_q_thread;
static sem_t send_out_q_sync_sem;
static sem_t send_out_control_sem;
static sem_t send_out_tx_sync_sem;


static void sendOut_thread_init(void);
static void sendOut_thread_fini(void);
static void  send_out_tx_callback(void *arg);
void sendOut_queueFlush(void);


static void * send_out_tx_thread_func(void *arg);
int send_out_sem_wait_timeout(sem_t *sem, uint32_t ms);

//Bypass Leon's  L2 caches
static inline void* sendGetNoCacheAddr(void* a) {
    uint32_t addr = (uint32_t)a;
    if((addr >> 28) == 0x7) return ((void*)(addr | 0x08000000)); //CMX addr
    if((addr >> 28) == 0x8) return ((void*)(addr | 0x40000000)); //DDR addr
    return ((void*)(addr)); //else ?
}
//Enable Leon's  L2 caches
static inline void* sendGetCacheAddr(void* a) {
    uint32_t addr = (uint32_t)a;
    if((addr >> 28) == 0x7) return ((void*)(addr & (~0x08000000))); //CMX addr
    if((addr >> 28) == 0x8) return ((void*)(addr & (~0x40000000))); //DDR addr
    return ((void*)(addr)); //else ?
}

static int  sendOut_queuePut(FrameT *buffer, uint32_t outId, uint32_t type, SendOutCbSent sendOutCbSent)
{
    SendOutElement_t * task;

    SEND_OUT_CRITICAL_SECTION_ENTER();
    if(sendOutQueue.queueIn == (( sendOutQueue.queueOut - 1 +
                                  MAX_NR_OF_OUTPUTS_PENDING) % MAX_NR_OF_OUTPUTS_PENDING))
    {
        SEND_OUT_CRITICAL_SECTION_EXIT();
        return -1; /* Queue Full*/
    }
    task = &sendOutQueue.queue[sendOutQueue.queueIn];
    sendOutQueue.queueIn = (sendOutQueue.queueIn + 1) % MAX_NR_OF_OUTPUTS_PENDING;

    task->buffer           = buffer;
    task->outId            = outId;
    task->frmType          = type;
    task->sendOutCbSent    = sendOutCbSent;
    task->localCallback    = send_out_tx_callback;

    if (sendOutQueue.tx_state == TX_MODULE_STATE__IDLE)
        sendOutQueue.tx_state = TX_MODULE_STATE__IDLE_TO_RUN;
    SEND_OUT_CRITICAL_SECTION_EXIT();

    return 0; // No errors
}

static int  sendOut_unlocked_queueGet(SendOutElement_t **old)
{
    if(sendOutQueue.queueIn == sendOutQueue.queueOut)
    {
        *old = NULL;
        sendOutQueue.tx_state = TX_MODULE_STATE__IDLE;
        SEND_OUT_CRITICAL_SECTION_EXIT();
        return -1; /* Queue Empty - nothing to get*/
    }
    *old = &sendOutQueue.queue[sendOutQueue.queueOut];
    sendOutQueue.queueOut = (sendOutQueue.queueOut + 1) % MAX_NR_OF_OUTPUTS_PENDING;
    sendOutQueue.tx_state = TX_MODULE_STATE__RUN;
    return 0; // No errors
}

static int  sendOut_queueGet(SendOutElement_t **old)
{
    int ret;
    SEND_OUT_CRITICAL_SECTION_ENTER();
    ret = sendOut_unlocked_queueGet(old);
    SEND_OUT_CRITICAL_SECTION_EXIT();
    return ret;
}

static void SendOut_queueInit(void)
{
    sendOutQueue.queueIn = sendOutQueue.queueOut = 0;
    sendOutQueue.tx_state = TX_MODULE_STATE__IDLE;
    if (sem_init(&send_out_q_sync_sem, 0, 1) != RTEMS_SUCCESSFUL) {
        printf("Failed to create queue sync semaphore\n");
        assert(0);
    }
    sendOutQueue.q_sync = &send_out_q_sync_sem;
}

static void SendOut_queueFini(void)
{
	sendOut_queueFlush();

	sem_destroy(sendOutQueue.q_sync);
	sendOutQueue.q_sync = NULL;
}

#ifdef USE_STATIC_HEADER

send_out_tx_buffer_header_t* sendOutGetBufferHeader(void)
{
    return sendOutHeader;
}

static inline client_tx_frame_header_t* sendOut_get_client_header(void)
{
    return (client_tx_frame_header_t*)(sendOutGetBufferHeader()->client_data);
}

static inline uint32_t send_out_tx_header_calc_sum(client_tx_frame_header_t *h) {
    UNUSED(h);
    return 0;
}

static void SendOut_prepareFrame(SendOutElement_t * tx_task)
{
    FrameT *frame = tx_task->buffer;
    client_tx_frame_header_t *h = sendOut_get_client_header();

    h = sendGetNoCacheAddr(h);
    //printf("-H %5d, %5d, %5d, %5d, %5d, %5d, %5d\n",tx_task->outId, tx_task->frmType, frame->stride[0], frame->stride[1], frame->height[0], frame->height[1], frame->type);
    sendOutGetBufferHeader()->chunk = 1;
    h->frame_type               = tx_task->frmType;          // FRAME_DATA_TYPE_PREVIEW;
    h->frame_format             = frame->fmt;      // FRAME_DATA_FORMAT_YUV420;
    h->frame_width              = frame->stride[0];  // PREVIEW_WIDTH;
    h->frame_height             = frame->height[0]; // PREVIEW_HEIGHT;
    h->frame_time_stamp_hi      = (uint32_t)(frame->ts[0]>>32);
    h->frame_time_stamp_lo      = (uint32_t)frame->ts[0];
    h->frame_proc_time_stamp_hi = (uint32_t)(frame->ts[1]>>32);
    h->frame_proc_time_stamp_lo = (uint32_t)frame->ts[1];
    h->frame_idx_req_hal        = 0;
    h->frame_idx_req_app        = 0;
    h->frame_idx_mipi_rx        = frame->seqNo;
    h->frame_idx_process        = 0;
    h->header_height            = SEND_OUT_HEADER_HEIGHT; // SEND_OUT_HEADER_HEIGHT; // PREVIEW_HEADER_HEIGHT;
    h->slice_data_type          = 0;
    h->slice_y_offset           = 0;
    h->slice_y_size             = frame->tSize[0];
    h->slice_uv_offset          = 0;
    h->slice_uv_size            = 0;
    h->slice_total_number       = 1; // PREVIEW_CHUNKS_NUM;
    h->slice_last_flag          = 0;
    h->debug_data_enable        = 0;
    h->camera_id                = tx_task->outId;
    h->buff_width               = h->frame_width;
    h->buff_height              = h->frame_height;
    h->buff_stride              = h->buff_width;
    if (h->frame_format == FRAME_FMT_RAW_10_PACK)
    {
        h->buff_pxl_size_nom        = 5;
        h->buff_pxl_size_denom      = 4;
        h->frame_width = h->frame_width * 4 / 5;
    } else
    if (h->frame_format == FRAME_FMT_RAW_12_PACK)
    {
        h->buff_pxl_size_nom        = 3;
        h->buff_pxl_size_denom      = 2;
        h->frame_width = h->frame_width * 2 / 3;
    } else
    {
        h->buff_pxl_size_nom        = 1;
        h->buff_pxl_size_denom      = 1;
    }

    h->check_sum                = send_out_tx_header_calc_sum(h);
}
#endif
/**************************************************************************************************
 ~~~ Local File function declarations
 **************************************************************************************************/
#ifdef SEND_OUT_DBG_DUMP
static void dumpOutputFrame(FrameT *frame);
#endif
/**************************************************************************************************
 ~~~ Functions Implementation
 **************************************************************************************************/
void sendOutCreate(SendOutInitCfg_t *cfg)
{
    TRANSPORT_create(cfg->TRANSPORT_PARAM);
}

// Function calling initialization function for configured output
void sendOutInit(void)
{
    TRANSPORT_init();

    SendOut_queueInit();

    sendOut_thread_init();
}

void sendOutControl(uint32_t camera_en_bit_mask, uint32_t frame_type_en_bit_mask, uint32_t  frame_format_en_bit_mask)
{
    printf("sendOut Ctrl: CAM 0x%x TYPE 0x%x FMT 0x%x\n",
            (unsigned int)camera_en_bit_mask,
            (unsigned int)frame_type_en_bit_mask,
            (unsigned int)frame_format_en_bit_mask);

    sndOutEnableCamera = camera_en_bit_mask;
    sndOutEnableType   = frame_type_en_bit_mask;
    sndOutEnableFormat = frame_format_en_bit_mask;
}

void sendOutSend(FrameT *frame, uint32_t outputId, uint32_t frmType, SendOutCbSent sendOutCbSent)
{
    if (IS_DISABLED(outputId, frmType, frame->type))
    {
        if (sendOutCbSent) {
            sendOutCbSent(frame, outputId, frmType);
        }
        return;
    }

#ifdef SEND_OUT_DBG_DUMP
    dumpOutputFrame(frame);
#endif

    if (sendOut_queuePut(frame, outputId, frmType, sendOutCbSent))
    {
        if (sendOutCbSent)
        {
            sendOutCbSent(frame, outputId, frmType);
        }
        return;
    }

    // Trigger Tx thread
    assert(RTEMS_SUCCESSFUL == sem_post(send_out_module_storage.control_sem));
}

// Function calling de-init function for configured output
void sendOutFini()
{
    sendOut_thread_fini();

    TRANSPORT_Fini();
    SendOut_queueFini();
}

void sendOut_queueFlush (void)
{
SendOutElement_t *taskDescriptor;

    SEND_OUT_CRITICAL_SECTION_ENTER();

    for(;;)
    {
        sendOut_unlocked_queueGet(&taskDescriptor);
        if (NULL == taskDescriptor)
        { // there is no element in queue
            return;
        }
        printf ("flushing %p\n", taskDescriptor);

        if (taskDescriptor->sendOutCbSent)  // call client's callback
        {
            taskDescriptor->sendOutCbSent(taskDescriptor->buffer, taskDescriptor->outId, taskDescriptor->frmType);
        }
    }
    SEND_OUT_CRITICAL_SECTION_EXIT();
}

static void send_out_tx_callback(void *arg)
{
    send_out_tx_thread_t *send_out_tx_thread;
    int err;

    UNUSED(arg);
    send_out_tx_thread = &send_out_module_storage;
    if (send_out_tx_thread->tx_sync)
    {
        err = sem_post(send_out_tx_thread->tx_sync);
        if (err != 0)
            printf("tx_sync post critical %d\n", err);
        assert(err == 0);
    } else
    {
        printf("send_out_tx_callback out of sync\n");
    }
}

int send_out_sem_wait_timeout(sem_t *sem, uint32_t ms)
{
    struct timespec abs_to;
    struct timeval now;
    uint64_t us;

    gettimeofday(&now, NULL);

    us = (uint64_t)now.tv_sec * 1000000 + (uint64_t)now.tv_usec + (uint64_t)ms * 1000;

    abs_to.tv_sec = us / 1000000;
    abs_to.tv_nsec = (us % 1000000) * 1000;

    return sem_timedwait(sem, &abs_to);
}

static void transport_try_send(send_out_tx_thread_t *send_out_tx_thread)
{
SendOutElement_t *taskDescriptor;
int err;

    sendOut_queueGet(&taskDescriptor);
    if (NULL == taskDescriptor)
    { // there is a new element in queue
        return;
    }
#ifdef USE_STATIC_HEADER
    SendOut_prepareFrame(taskDescriptor);
#endif
    TRANSPORT_SendFrame(taskDescriptor);

    err = send_out_sem_wait_timeout(send_out_tx_thread->tx_sync, TX_PACKET_SEND_TIMEOUT); // wait transfer to complete
    if (err != 0)
        printf("Timeout SendOut %d\n", err);
//    assert(err == 0);

    if (taskDescriptor->sendOutCbSent)  // call client's callback
    {
        taskDescriptor->sendOutCbSent(taskDescriptor->buffer, taskDescriptor->outId, taskDescriptor->frmType);
    }
}

static void * send_out_tx_thread_func(void *arg)
{
    send_out_tx_thread_t *send_out_tx_thread;
    int err;

    send_out_tx_thread = arg;

    for (;;) {
        err = sem_wait(send_out_tx_thread->control_sem);
        if (err) {
            printf("SendOut: Failed to obtain control_sem: err=%d", err);
        }
        if (send_out_tx_thread->state == TX_THREAD__STOPPING)
            break;

        transport_try_send(send_out_tx_thread);
    }
    return NULL;
}

static void sendOut_thread_init(void)
{
    send_out_tx_thread_t *send_out_tx_thread;

    send_out_tx_thread = &send_out_module_storage;

    if(sem_init(&send_out_control_sem, 0, 0) != RTEMS_SUCCESSFUL) {
        printf("Failed to create message control semaphore\n");
        goto exit1;
    }
    send_out_tx_thread->control_sem = &send_out_control_sem;

    if (sem_init(&send_out_tx_sync_sem, 0, 0) != RTEMS_SUCCESSFUL) {
        printf("Failed to create message tx_sync semaphore\n");
        goto exit2;
    }
    send_out_tx_thread->tx_sync = &send_out_tx_sync_sem;


    // create tx thread

    pthread_attr_t attr;
    struct sched_param param;
    assert(RTEMS_SUCCESSFUL == pthread_attr_init           (&attr));
    assert(RTEMS_SUCCESSFUL == pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
    assert(RTEMS_SUCCESSFUL == pthread_attr_setschedpolicy (&attr, SCHED_RR));
    assert(RTEMS_SUCCESSFUL == pthread_attr_getschedparam (&attr, &param));
    param.sched_priority = SEND_OUT_TX_THREAD_PRIORITY;
    assert(RTEMS_SUCCESSFUL == pthread_attr_setschedparam (&attr, &param));
    assert(RTEMS_SUCCESSFUL == pthread_create(&send_out_q_thread, &attr, send_out_tx_thread_func, send_out_tx_thread));
    assert(RTEMS_SUCCESSFUL == pthread_attr_destroy   (&attr));
    send_out_tx_thread->thread = &send_out_q_thread;
    send_out_tx_thread->state = TX_THREAD__ACTIVE;


    return;


exit2:
    sem_destroy(&send_out_tx_sync_sem);
    send_out_tx_thread->tx_sync = NULL;
exit1:
    sem_destroy(&send_out_control_sem);
    send_out_tx_thread->control_sem = NULL;
    return;
}

static void sendOut_thread_fini(void)
{
	send_out_tx_thread_t *send_out_tx_thread = &send_out_module_storage;

	if(send_out_tx_thread->state != TX_THREAD__IDLE)
	{
		send_out_tx_thread->state = TX_THREAD__STOPPING;
		sem_post(send_out_tx_thread->control_sem);    // Make sure thread will see this request
		pthread_join(send_out_q_thread, NULL);
		send_out_tx_thread->thread = NULL;
		send_out_tx_thread->state = TX_THREAD__IDLE;

		sem_destroy(send_out_tx_thread->control_sem);
		sem_destroy(send_out_tx_thread->tx_sync);
		send_out_tx_thread->control_sem = NULL;
		send_out_tx_thread->tx_sync = NULL;
	}
}


#ifdef SEND_OUT_DBG_DUMP
static void dumpOutputFrame(FrameT *frame)
{
    dbgDumpFrame = swcLeonReadNoCacheU32((uint32_t)&dbgDumpFrame);
    if(dbgDumpFrame) {
        if (FRAME_FMT_422I == frame->type) {
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

