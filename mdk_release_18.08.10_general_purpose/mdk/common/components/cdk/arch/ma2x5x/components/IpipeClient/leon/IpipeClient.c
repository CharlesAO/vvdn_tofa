
/**************************************************************************************************

 @File         : main.c
 @Author       : MT
 @Brief        : Contains main debug control for application
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015

 Description :
    This file allow initialization and control of lrt pipes. Control is made
    base on debuger interaction. Application can run on Pc or on movidius
    MV182 board.

 **************************************************************************************************/

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
//stl
#include <string.h>               //memcpy
#include "assert.h"
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#if defined(__sparc)
#include "bsp.h"
#include "DrvLeon.h"
#include "DrvCpr.h"
#include "OsDrvCpr.h"
#endif
//
#include "ipipe.h"
#include "ipipeUtils.h"
#include "ipipeMsgQueue.h"

/**************************************************************************************************
 ~~~  Local variables
 **************************************************************************************************/
sem_t semEvent;
#if defined(__sparc)
extern icCtrl    lrt_gIcCtrl;                   //Shared with OS Leon
extern uint32_t      *lrt_start;                     //Entry point of the leon_rt processor
#define BYPASS_CACHE    0x08000000
#else
// lrt on Pc is a thread
pthread_t               LeonRTThread;
extern icCtrl    gIcCtrl;                   //Shared with OS Leon
#define BYPASS_CACHE    0x00000000
extern void  *lrt_start_sim(void *);
#endif
icCtrl          *pIcCtrl = (icCtrl*)0xDEADBEEF; //ptr that bypasses L2

// "*ev" needs to be obtained previously with "ipipeQueueGetNextOutputSlot"
static inline void ipipeQueueSendToServerEvent(icEvent *ev) {
    ev->ctrl |= IC_EVENT_CTRL_OWNED; //consummer owns this from now on
    // it is interrupt base starting with this revision
#if defined(__sparc)
    // TODO: check this mechanism, is not defined
    BSP_Force_remote_interrupt(INTER_CPU_INT);
#else
    extern void scanInputEvents(uint32_t unused);
    scanInputEvents(0);
#endif

}

void ipipeClientISR(uint32_t unused)
{
    UNUSED(unused);
    //back events from RT side, this should stay in APP space really.
    if(sem_post(&semEvent) == -1)  {    /// New event. Post to the semaphore
        printf("sem_post error\n");
    }
}

//#############################################################################
// - Initializes inter-cpu-interrupt
// - Start RT-Leon from its entry point
icCtrl *icSetup(int32_t appID,    uint32_t ddrStart,  uint32_t ddrSize) {
    int rc;

    //0)Get & use a pointer that bypasses L2 cache
#if defined(__sparc)
    pIcCtrl = (icCtrl *)(((uint32_t)&lrt_gIcCtrl) | BYPASS_CACHE);
#else
    pIcCtrl = (icCtrl *)(((uint32_t)&    gIcCtrl) | BYPASS_CACHE);
#endif
    /// Initialize the semaphore for event handler
    if((rc = sem_init(&semEvent, 0, 0)) == -1)
        printf("pthread_create: %s\n", strerror(rc));

    //1)Prepare to get interrupts form Leon-RT side, as following step
    //  finishes with an RT-to-OS interrupt
#if defined(__sparc)
    /// Setup the IRQ
    BSP_Mask_interrupt(INTER_CPU_INT);
    BSP_Clear_interrupt(INTER_CPU_INT);
    BSP_Set_interrupt_type_priority(INTER_CPU_INT, POS_EDGE_INT, INTER_CPU_INT_LEVEL);
    BSP_interrupt_register(INTER_CPU_INT, NULL, (rtems_interrupt_handler) &ipipeClientISR, (void *)(INTER_CPU_INT));
    BSP_Unmask_interrupt(INTER_CPU_INT);
#else
    // PC  will be directly calling of irq function from other thread.
#endif

    //2)Init incomming queue access (as Server is not yet started)
    //  Clear first incoming event (as we'll soon start polling and don't want false results)
    memset((uint32_t*)pIcCtrl,     0, sizeof(icCtrl    ));
    pIcCtrl->framepoolBase = (void*)ddrStart;
    pIcCtrl->framepoolSize = ddrSize;
    pIcCtrl->appId         = appID;

    OsDrvCprGetSysClockPerUs(&pIcCtrl->sysClkPerUs);

    //3)Start the LeonRT application
#if defined(__sparc)
    // if lrt remain in some unnoLught stage, this function will restart it anyway
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET, DEV_MSS_LRT  | DEV_MSS_LRT_ICB);
    DrvCprSysDeviceAction(MSS_DOMAIN, ENABLE_CLKS, DEV_MSS_LRT |  DEV_MSS_LRT_DSU |DEV_MSS_LRT_L2C);
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, DEV_MSS_LRT  | DEV_MSS_LRT_ICB);
    DrvLeonRTStartup((uint32_t)&lrt_start);
#else
    if ((rc = pthread_create(&LeonRTThread, NULL, lrt_start_sim, NULL)) != 0) {
        fprintf(stderr, "pthread_create: %s\n", strerror(rc));
        exit(1);
    }
#endif
    return (pIcCtrl);
}

void    *icQuery                (icCtrl *ctrl, icQueryType queryType, int32_t index) {
  //IcPipelineDescription pipeDescription = ctrl->icPipelineDescription;
    switch (queryType) {
    case IC_QUERY_TYPE_SOURCE:
        if(index < IPIPE_MAX_SOURCES_ALLOWED)
            return((void*)ctrl->icPipelineDescription.icQuerySource[index]);
        else
            return NULL;
    case IC_QUERY_TYPE_ISP:
        if(index < IPIPE_MAX_ISP_ALlOWED)
            return((void*)ctrl->icPipelineDescription.icQueryIsp[index]);
        else
            return NULL;
    case IC_QUERY_TYPE_OUTPUT:
        if(index < IPIPE_MAX_OUTPUTS_ALlOWED)
            return((void*)ctrl->icPipelineDescription.icQueryOutput[index]);
        else
            return NULL;
    case IC_QUERY_TYPE_USERPLG:
        if(index < IPIPE_MAX_USERPLUG_ALlOWED)
            return((void*)ctrl->icPipelineDescription.icQueryPlg[index]);
        else
            return NULL;
    default:
        assert(0xDEADDEAD != queryType);
        return (NULL);
    }
}

//#################################################################################################
void icSetupSource (icCtrl *ctrl, icSourceInstance srcIdx, icSourceSetup *setup) {
    assert(srcIdx <  IPIPE_MAX_SOURCES_ALLOWED);
    assert(IPIPE_INITIALIZED == ctrl->status);
    memcpy((void*)&ctrl->source[srcIdx].sourceSetup, setup, sizeof(icSourceSetup));
    icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
    ev->u.configureSource.sourceInstance = srcIdx;  //identify instance
    ev->ctrl                             = IC_EVENT_TYPE_SETUP_SOURCE;   //type of event
    ipipeQueueSendToServerEvent(ev);
}

//#################################################################################################
int32_t icConfigureSourceDynamic(icCtrl *ctrl, icSourceInstance srcIdx, icSourceConfigDynamic *config)
{
    assert(srcIdx <  IPIPE_MAX_SOURCES_ALLOWED);
    assert(IPIPE_UNINITIALIZED != ctrl->status); //can be INITIALIZED or IN_USE, doesn't matter
    icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
    ev->u.configureSourceDynamic.sourceInstance = srcIdx;
    ev->u.configureSourceDynamic.pDynConfig     = (icSourceConfigDynamic*)ipipeGetNoCacheAddr((uint32_t)config);
    ev->ctrl                                    = IC_EVENT_TYPE_CONFIG_SOURCE_DYNAMIC; //type of event
    ipipeQueueSendToServerEvent(ev);
    return 0;
}

//#################################################################################################
void icSetupSourcesCommit (icCtrl *ctrl) {
    assert(IPIPE_INITIALIZED == ctrl->status);
    struct timeval tv;
    icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
    ev->ctrl    = IC_EVENT_TYPE_SETUP_SOURCES_COMMIT;   //type of event

    {//Sync LOS/LRT time-stamping
        uint32_t high, low;

        //Read LOS time
        high    = GET_REG_WORD_VAL(TIM1_BASE_ADR + TIM_FREE_CNT1_OFFSET);//high
        low     = GET_REG_WORD_VAL(TIM1_BASE_ADR + TIM_FREE_CNT0_OFFSET);//low

        (void) gettimeofday(&tv, NULL);
        ctrl->curTime = (icTimestamp)tv.tv_sec * 1000000 + (icTimestamp)tv.tv_usec;

        ctrl->ticksDelay = ((icTimestamp)high << 32) | ((icTimestamp)low);
    }

    //Send event
    ipipeQueueSendToServerEvent(ev);
}

//#################################################################################################
void icConfigureSource(icCtrl *ctrl, icSourceInstance srcIdx, icSourceConfig *config) {
    assert(srcIdx <  IPIPE_MAX_SOURCES_ALLOWED);
    assert(IPIPE_CONFIGURED == ctrl->status);
    memcpy((void*)&ctrl->source[srcIdx].sourceConfig, config, sizeof(icSourceConfig));
    icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
    ev->u.configureSource.sourceInstance = srcIdx;  //identify instance
    ev->ctrl                             = IC_EVENT_TYPE_CONFIG_SOURCE;   //type of event
    ipipeQueueSendToServerEvent(ev);
    assert(IPIPE_CONFIGURED == ctrl->status);
    assert(IPIPE_SRC_SETUP == ctrl->source[ev->u.configureSource.sourceInstance].sourceStatus);
    ctrl->source[ev->u.configureSource.sourceInstance].sourceStatus = IPIPE_SRC_CONFIGURED;
}

//#######################################################################################
//Send START signal
int32_t icStartSource(icCtrl *ctrl, icSourceInstance srcIdx) {
    assert(IPIPE_CONFIGURED == ctrl->status);
    assert(IPIPE_SRC_CONFIGURED == ctrl->source[srcIdx].sourceStatus);
    icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
    ev->u.configureSource.sourceInstance = srcIdx;  //identify instance
    ev->ctrl                             = IC_EVENT_TYPE_START_SOURCE;   //type of event
    ipipeQueueSendToServerEvent(ev);
    return 0;
}

int32_t icStopSource            (icCtrl *ctrl, icSourceInstance srcIdx) {
    icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
    ev->u.configureSource.sourceInstance = srcIdx;  //identify instance
    ev->ctrl                             = IC_EVENT_TYPE_STOP_SOURCE;   //type of event
    ipipeQueueSendToServerEvent(ev);
    return 0;
}

//#######################################################################################
int32_t icConfigureIsp(icCtrl *ctrl, int32_t ispIdx, void *cfg)
{
    icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
    ev->u.configureIsp.ispInstance    = ispIdx;   //identify instance
    ev->u.configureIsp.pConfigStruct  = ipipeGetNoCacheAddr((uint32_t)cfg); //ptr to configuration data struct
    ev->ctrl                          = IC_EVENT_TYPE_CONFIG_ISP; //type of event
    ipipeQueueSendToServerEvent(ev);
    return 0;
}

void  icDataReceived(icCtrl *ctrl, FrameT *dataBufStruct) {
    icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
    ev->u.dataWasSent.dataBufStruct = dataBufStruct;   //identify instance
    ev->ctrl                        = IC_EVENT_TYPE_OUTPUT_DATA_RECEIVED;//type of event
    ipipeQueueSendToServerEvent(ev);
}


int icSemWaitTimeout(sem_t *sem, uint32_t ms)
{
    struct timespec abs_to;
    struct timeval now;
    uint64_t us;

    gettimeofday(&now, NULL);

    us = (uint64_t)now.tv_sec * 1000000 + (uint64_t)now.tv_usec + (uint64_t)ms * 1000;

    abs_to.tv_sec = us / 1000000;
    abs_to.tv_nsec = (us % 1000000) * 1000;

    if (sem_timedwait(sem, &abs_to))
        return errno;
    return 0;
}


int32_t icGetEvent(icCtrl *ctrl, icEvent *ev)
{
  //volatile icEvent *evPtr;
    UNUSED(ctrl);

    for (;;) {
        if (IC_EVENT_CTRL_OWNED & pIcCtrl->eventQIn.eventQ[pIcCtrl->eventQIn.readIdx].ctrl) {
            ipipeQueueGetEvent(&pIcCtrl->eventQIn, ev);
            break;
        }
        icSemWaitTimeout(&semEvent, 20);
    }

    return 0;
}


//#############################################################################
//Halt all processing and streaming, and release all processor resources associated with IPIPE.
//The specified control structure, �ctrl� is no longer valid after this call.
//icSetup() needs to be called again prior to making any other client API calls.
int32_t icTeardown              (icCtrl *ctrl) {
    if(ctrl) {

        icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
        ev->ctrl                          = IC_EVENT_TYPE_TEAR_DOWN; //type of event
        ipipeQueueSendToServerEvent(ev);

#if defined(__sparc)
#else
        /* Stop the LeonRT thread and wait for it to exit */
        //pthread_cancel(leonRTThread);

        pthread_join(LeonRTThread, NULL);
#endif
        return (0);
    }
    return (0);
}

int32_t icLockZSL (icCtrl *ctrl, icSourceInstance source, uint64_t frameSel, icLockZSLFlags flags) {
    icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
    ev->u.lockZSL.sourceInstance         = source;
    ev->u.lockZSL.frameSel               = frameSel;
    ev->u.lockZSL.flags                  = flags;
    ev->ctrl                          = IC_EVENT_TYPE_LOCK_ZSL; //type of event
    ipipeQueueSendToServerEvent(ev);
    return 0;
}

int32_t icUnlockZSL             (icCtrl *ctrl, icSourceInstance source, FrameT *buff) {
    icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
    ev->u.unlockZSL.sourceInstance  = source;  //identify instance
    ev->u.unlockZSL.buff            = buff; // identify frame locked refer to
    ev->ctrl                             = IC_EVENT_TYPE_UNLOCK_ZSL;   //type of event
    ipipeQueueSendToServerEvent(ev);
    return 0;
}

int32_t icTriggerCapture        (icCtrl *ctrl, icSourceInstance source, void * buff,
        void *cfg, icCaptureFlags flags) {
    icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
    ev->u.capture.sourceInstance         = source;
    ev->u.capture.pConfigStruct          = (void*)ipipeGetNoCacheAddr((uint32_t)cfg);
    ev->u.capture.buff                   = buff;
    ev->u.capture.flags                  = flags;  //identify instance
    ev->ctrl                             = IC_EVENT_TYPE_CAPTURE;   //type of event
    ipipeQueueSendToServerEvent(ev);
    return 0;
}

int32_t icZSLAdd                (icCtrl *ctrl, icSourceInstance source, uint32_t ZSLframesToAdd)
{
    icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
    ev->u.addZSL.sourceInstance          = source;  //identify instance
    ev->u.addZSL.addZSLframesNo          = ZSLframesToAdd;
    ev->ctrl                             = IC_EVENT_TYPE_ZSL_ADD;   //type of event
    ipipeQueueSendToServerEvent(ev);
    return 0;
}

int32_t icUserMsgSendToLrt      (icCtrl *ctrl, void *eventStruct, uint32_t id) {
    icEvent *ev = ipipeQueueGetNextOutputSlot(&ctrl->eventQOut);
    ev->u.generalEvent.eventStruct = eventStruct;
    ev->u.generalEvent.id          = id;
    ev->ctrl                             = IC_EVENT_TYPE_USER;   //type of event
    ipipeQueueSendToServerEvent(ev);
    return 0;
}

