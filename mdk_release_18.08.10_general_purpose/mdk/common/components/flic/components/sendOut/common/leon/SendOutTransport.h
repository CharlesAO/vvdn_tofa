///
/// @file      SendOutTransport.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Containing Ipipe Los output interface.
///

#ifndef SEND_OUT_TRANSPORT_H
#define SEND_OUT_TRANSPORT_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 ~~~  Include standard types And General Plug-in used module
 **************************************************************************************************/
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
#ifndef MAX_NR_OF_OUTPUTS_PENDING
#define MAX_NR_OF_OUTPUTS_PENDING       16
#endif

#ifndef SEND_OUT_TX_THREAD_PRIORITY
#define SEND_OUT_TX_THREAD_PRIORITY     200
#endif

#ifndef TX_PACKET_SEND_TIMEOUT
#define TX_PACKET_SEND_TIMEOUT  (10000)
#endif

#ifndef SEND_OUT_MAX_WIDTH
#define SEND_OUT_MAX_WIDTH  (4208)
#endif

#ifndef SEND_OUT_MAX_HEIGHT
#define SEND_OUT_MAX_HEIGHT  (3120)
#endif

#define SEND_OUT_ENABLE_ALL_TYPES   (0xFFFFFFFF)
#define SEND_OUT_ENABLE_ALL_FORMATS (0xFFFFFFFF)

#define SO_ID_TO_ENABLE_BITMASK(x)    (1 << x)


/**************************************************************************************************
 ~~~  Basic project types definition
 **************************************************************************************************/
typedef void (*SendOutCbSent)(FrameT *frame, uint32_t outputId, uint32_t frmType);
typedef void (*InternalCbSent)(void *task);

typedef enum {
    TX_MODULE_STATE__IDLE,
    TX_MODULE_STATE__IDLE_TO_RUN,
    TX_MODULE_STATE__RUN,
} SoDevState;

typedef enum {
    TX_THREAD__ACTIVE,
    TX_THREAD__STOPPING,
} SoThrState;

typedef struct {
    FrameT*         buffer;
    uint32_t        outId;
    uint32_t        frmType;
    SendOutCbSent   sendOutCbSent;
    InternalCbSent  localCallback;
}SoElem;

typedef struct {
    SoElem      queue[MAX_NR_OF_OUTPUTS_PENDING];
    int32_t     queueIn;
    int32_t     queueOut;
    SoDevState  tx_state;
    sem_t       q_sync;
} SoQue;

typedef struct {
    pthread_t  thread;
    SoThrState state;
    sem_t      control_sem;
    sem_t      tx_sync;
    void       (*sendFrame)(SoElem *task);
} SoThr;


typedef struct {
    SoQue    que;
    SoThr    thrCtrl;
    uint32_t enableCam;// bit mask to enable sending frame from type FrameTFormatTypes see IcTypes.h
    uint32_t enableFmt;// bit mask to enable sending frame from type FrameTFormatTypes see IcTypes.h
    uint32_t enableType;// bit mask to enable sending frame from type FrameTFormatTypes see IcTypes.h
    uint8_t* blackUVBuf;
} SendOutDev;

//pthread_t thread;

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



/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
void sendOutTrCreate(SendOutDev *dev);
void sendOutTrInit(SendOutDev *dev);
void sendOutTrControl(SendOutDev *dev, uint32_t camEnBitMask, uint32_t frmTypeEnBitMask, uint32_t frmFmtEnBitMask);
void sendOutTrSend(SendOutDev *dev, FrameT *frame, uint32_t outputId, uint32_t frmType, SendOutCbSent sendOutCbSent);
void sendOutTrFini(SendOutDev *dev);
int  sendOutIsDisable(SendOutDev *dev, uint32_t outputId, uint32_t frmType, uint32_t frmFmt);
#ifdef __cplusplus
}
#endif
#endif //SEND_OUT_TRANSPORT_H
