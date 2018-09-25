///
/// @file      ic_main.c
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Interface between Isp base Pipeline and app 3A Cfg and sensor
///            Platform(s) supported : ma2x8x
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <assert.h>
#include <ipipe.h>
#include "IspCommon.h"
#include <rtems.h>
// app have to define out macro available, depend by isp and camera control interface are you using
// rhis file have to be at application level
#include "AppSpecInterface.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
APP_SPEC_DEFS01

//APP_SPEC_DEFS02




typedef struct {
    uint32_t        ref_cnt;
    pthread_mutex_t lock;
    icCtrl  *ctrl;
} g_ipipe_ctx_t;


// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
void platform_cam_led1_toggle (void);



// 4: Static Local Data
// ----------------------------------------------------------------------------
static g_ipipe_ctx_t gctx;

static sem_t semWaitForLrtReady;
static sem_t semWaitForSourceCommit;
static sem_t semWaitForSourceReady;
static sem_t semWaitForSourceStoped;

static pthread_t eventThread;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// Isp 3A and camera control interface callbacks
// default 3A and camera controls modules have implement this functions in order to
// have feedback from receiver image events and isp events
void inc_cam_stats_ready(void *p_prv, unsigned int seqNo, void *p_cfg_prv);
void inc_cam_capture_ready(void *p_prv, unsigned int seqNo, void *p_cfg_prv);
void inc_cam_ipipe_cfg_ready(void *p_prv, unsigned int seqNo, void *p_cfg_prv);
void inc_cam_ipipe_buff_locked(void *p_prv, void *userData, unsigned int sourceInstance,
                void *buffZsl, uint64_t ts, unsigned int seqNo);
void inc_cam_frame_start( void *p_prv, uint32_t sourceInstance, uint32_t seqNo,
                uint64_t ts, void *userData);
void inc_cam_frame_line_reached(void *p_prv, uint32_t sourceInstance, uint32_t seqNo,
                uint64_t ts, void *userData);
void inc_cam_frame_end(void *p_prv, uint32_t sourceInstance, uint32_t seqNo,
                uint64_t ts, void *userData);
void inc_cam_terminate_fr( void *p_prv, uint32_t sourceInstance, void *userData);

// Isp upper layer can control start, stop the cams receiver, and can control the isp
// base on this functions
void los_ConfigureSource(int srcIdx, icSourceConfig *sconf, int pipeId);
void los_SetupSource(int srcIdx, icSourceSetup *sconf);
void los_SetupSourceCommit(void);
// extra capability
void los_dataWasSent(FrameT *dataBufStruct, uint32_t outputId, uint32_t frmType);

// debug information about fps. Updating just when frame was produced "get fpsCams"
void updateFps(uint32_t outputId);



// 6: Functions Implementation
// ----------------------------------------------------------------------------

/*
 * ****************************************************************************
 * ** Should run at a relatively high priority so that the event queue doesn't
 * ** fill up (if it did, we would drop events).
 * ****************************************************************************
 */
static void * eventLoop(void *vCtrl)
{
    g_ipipe_ctx_t *ctx;
    icCtrl *ctrl;
    icEvent ev;
    unsigned int evno;

    ctx = vCtrl;
    ctrl = ctx->ctrl;
    rtems_object_set_name(RTEMS_SELF, "evLp");
    /*
     * This thread runs until it is cancelled (pthread_cond_wait() is a
     * cancellation point).
     */
    for (;;) {
        if (!icGetEvent(ctrl, &ev)) {
            evno = ev.ctrl & IC_EVENT_CTRL_TYPE_MASK;
            switch (evno) {
            case IC_EVENT_TYPE_LEON_RT_READY:
                /* TODO: _PROFILE_ADD */
                assert(sem_post(&semWaitForLrtReady) != -1);
                break;
            case IC_EVENT_TYPE_SETUP_SOURCES_RESULT:
                /* TODO: _PROFILE_ADD */
                assert(sem_post(&semWaitForSourceCommit) != -1);
                break;
            case IC_EVENT_TYPE_SOURCE_READY:
                /* TODO: _PROFILE_ADD */
                assert(sem_post(&semWaitForSourceReady) != -1);
                break;
            case IC_EVENT_TYPE_READOUT_START:
                //printf("IC_EVENT_TYPE_READOUT_START \n");
                if (ev.u.lineEvent.userData != NULL) {
                    _PROFILE_ADD(PROFILE_ID_LRT_START_FRAME, ev.u.lineEvent.userData,ev.u.lineEvent.sourceInstance);
                    inc_cam_frame_start(NULL, ev.u.lineEvent.sourceInstance,
                                    ev.u.lineEvent.seqNo, ev.u.lineEvent.ts, ev.u.lineEvent.userData);
                    inc_cam_ipipe_cfg_ready(NULL, ev.u.lineEvent.seqNo, ev.u.lineEvent.userData);
                }
                break;
            case IC_EVENT_TYPE_LINE_REACHED:
                if (ev.u.lineEvent.userData != NULL) {
                    _PROFILE_ADD(PROFILE_ID_LRT_LINE_REACHED, ev.u.lineEvent.userData,
                                    ev.u.lineEvent.sourceInstance);
                    if (0) { /* TODO: IPIPE2 not implelmented */
                        inc_cam_frame_line_reached(NULL, ev.u.lineEvent.sourceInstance,
                                        ev.u.lineEvent.seqNo, ev.u.lineEvent.ts, ev.u.lineEvent.userData);
                    }
                }
                break;
            case IC_EVENT_TYPE_READOUT_END:
                //printf("IC_EVENT_TYPE_READOUT_END \n");
                if (ev.u.lineEvent.userData != NULL) {
                    _PROFILE_ADD(PROFILE_ID_LRT_END_FRAME, ev.u.lineEvent.userData,
                                    ev.u.lineEvent.sourceInstance);
                    inc_cam_frame_end(NULL, ev.u.lineEvent.sourceInstance, ev.u.lineEvent.seqNo,
                                    ev.u.lineEvent.ts, ev.u.lineEvent.userData);
                }
                break;
            case IC_EVENT_TYPE_ISP_START:
                if (ev.u.ispEvent.userData != NULL) {
                    _PROFILE_ADD(PROFILE_ID_LRT_ISP_START, ev.u.ispEvent.userData,
                                    ev.u.ispEvent.ispInstance);
                    if (0) { /* TODO: IPIPE2 not implelmented */
                        inc_cam_ipipe_cfg_ready(NULL, ev.u.ispEvent.seqNo, ev.u.ispEvent.userData);
                    }
                } else {
                    APP_DBG_ERROR(DL_ERROR, "IC_EVENT_TYPE_ISP_START with userdata == NULL");
                }
                break;
            case IC_EVENT_TYPE_ISP_END:
                if (ev.u.ispEvent.userData != NULL) {
                    _PROFILE_ADD(PROFILE_ID_LRT_ISP_END, ev.u.ispEvent.userData,
                                    ev.u.ispEvent.ispInstance);
                    inc_cam_stats_ready(NULL, ev.u.ispEvent.seqNo, ev.u.ispEvent.userData);
                    if(0) { /* TODO: IPIPE2 not implelmented */
                        inc_cam_capture_ready(NULL, ev.u.ispEvent.seqNo, ev.u.ispEvent.userData);
                    }
                } else {
                    APP_DBG_ERROR(DL_ERROR, "IC_EVENT_TYPE_ISP_END with userdata == NULL");
                }
                break;
            case IC_EVENT_TYPE_ZSL_LOCKED:
                _PROFILE_ADD(PROFILE_ID_LRT_ZSL_LOCKED, ev.u.buffLockedZSL.userData,
                                ev.u.buffLockedZSL.sourceInstance);
                inc_cam_ipipe_buff_locked(NULL, ev.u.buffLockedZSL.userData,
                                ev.u.buffLockedZSL.sourceInstance, ev.u.buffLockedZSL.buffZsl,
                                ev.u.buffLockedZSL.ts,  ev.u.buffLockedZSL.seqNo);
                break;
            case IC_EVENT_TYPE_SOURCE_STOPPED:
                /* TODO: _PROFILE_ADD */
                assert(sem_post(&semWaitForSourceStoped) != -1);
                break;
            case IC_EVENT_TYPE_SEND_OUTPUT_DATA:
            {
                FrameT *frame;
                uint32_t outId;
                uint32_t frmType;
                uint32_t frmFmt;

                platform_cam_led1_toggle();
                // TODO: proper implement this !!!
                frame   = ev.u.sentOutput.dataBufStruct;
                outId   = frame->instId;
                if(outId >= INSTANCES_COUNT_MAX) {
                    frmType = IC_OUTPUT_FRAME_DATA_TYPE_STILL;
                }
                else {
                    frmType = IC_OUTPUT_FRAME_DATA_TYPE_PREVIEW;
                }
                frmFmt  = ev.u.sentOutput.dataBufStruct->fmt;

                _PROFILE_ADD(PROFILE_ID_LRT_SEND_DATA, frame->userData, outId);
                assert(outId<INSTANCES_COUNT_MAX*2);
                assert(frmType<FRAME_TYPE_MAX);
                assert(frmFmt<FRAME_FMT_MAX);
                //los_dataWasSent(frame, outId, frmFmt);
                //TODO: !!!
                if(frmType == IC_OUTPUT_FRAME_DATA_TYPE_PREVIEW) {
                    sendOutSend(frame, outId, frmType, los_dataWasSent);
                    updateFps(outId);
                }
                else {
#ifdef OUTPUT_UNIT_IS_USB
                    sendOutSend(frame, outId, frmType, los_dataWasSent);
#else
                    los_dataWasSent(frame, outId, frmFmt);
                    printf("Still Frame Produced. Run Above command in order to save the output. \n");
                    printf("savefile outStilImg_%lux%lu_P420.yuv %p %lu\n", frame->stride[0],frame->height[0],
                            frame->fbPtr[0], frame->stride[0]*frame->height[0]*3/2);
#endif
                }
            }
            break;
            case IC_EVENT_TYPE_ERROR:
                _PROFILE_ADD(PROFILE_ID_LRT_ERROR, ev.u.error.userData, ev.u.error.sourceInstance);
                printf("error: %d\n", ev.u.error.errorNo);
                if ((ev.u.error.userData) && (ev.u.error.errorNo == IC_ERROR_RT_OUT_BUFFERS_NOT_AVAILABLE)) {
                    inc_cam_terminate_fr(
                            NULL,
                            ev.u.error.sourceInstance,
                            ev.u.error.userData
                        );
                }
                break;
            case IC_EVENT_TYPE_TORN_DOWN:
                /* TODO: _PROFILE_ADD */
                return NULL;
            default:
                APP_DBG_ERROR(DL_ERROR, "Unknown evt: %d", evno);
            }
        } else {
            return NULL;
            /*
             * no activity in last period, lrt crash
             * or lrt is started, but no camera connected
             * in this case cut down lrt
             */
            APP_DBG_ERROR(DL_ERROR, "Error X");
        }
    }

    return NULL;
}


int getSrcLimitsLoc(uint32_t srcId, icSourceSetup* srcSet) {
    return (UPDATE_SRC_LIMITS(srcId, srcSet));
}

void los_start(void *arg)
{
    UNUSED(arg);
    int policy;
    struct sched_param param;
    _PROFILE_ADD(PROFILE_ID_LOS_START, 0, 0);
    gctx.ref_cnt++;
    if (gctx.ref_cnt == 1) {
        assert(0 == pthread_mutex_init(&gctx.lock, NULL));
        assert(sem_init(&semWaitForLrtReady, 0, 0) != -1);
        assert(sem_init(&semWaitForSourceCommit, 0, 0) != -1);
        assert(sem_init(&semWaitForSourceReady, 0, 0) != -1);
        assert(sem_init(&semWaitForSourceStoped, 0, 0) != -1);

        PIPE_CREATE(getSrcLimitsLoc);
        gctx.ctrl = icSetup(1, (uint32_t)NULL, 0);
        assert(pthread_create(&eventThread, NULL, eventLoop, &gctx) == 0);
        pthread_getschedparam(eventThread, &policy, &param);
        param.sched_priority = 210;
        pthread_setschedparam(eventThread, policy, &param);
        //assert(sem_wait(&semWaitForLrtReady) != -1);
        sendOutInit(); //TODO: !!!
        {
            int i;
            for (i = 0; i<INSTANCES_COUNT_MAX; i++) {
                static icSourceSetup srcSet = {
                                0, 0, 10, 0, 1, 1, 1, 1, 0
                };
                if(0 == UPDATE_SRC_LIMITS(i, &srcSet)) {
                    los_SetupSource(i, &srcSet);
                }
            }
        }
        los_SetupSourceCommit();
    }
}

void los_stop(void) {
    _PROFILE_ADD(PROFILE_ID_LOS_STOP, 0, 0);
    assert(gctx.ref_cnt);
    gctx.ref_cnt--;
    if (!gctx.ref_cnt) {
        //usleep(33*1000);
        icTeardown(gctx.ctrl);
        PIPE_DESTROY();
        pthread_join(eventThread, NULL);

        sendOutFini();
        pthread_mutex_destroy(&gctx.lock);
        sem_destroy(&semWaitForSourceStoped);
        sem_destroy(&semWaitForSourceReady);
        sem_destroy(&semWaitForSourceCommit);
        sem_destroy(&semWaitForLrtReady);
    }
}

void los_ConfigureGlobal(void *gconf) {
    _PROFILE_ADD(PROFILE_ID_LOS_GOB_CONFIG, gconf, 0);
}

void los_ConfigureSource(int srcIdx, icSourceConfig *sconf, int pipeId) {
    UNUSED(pipeId);
    //pthread_mutex_lock(&gctx.lock);
    _PROFILE_ADD(PROFILE_ID_LOS_SRC_CONFIG, sconf, srcIdx);
    icConfigureSource(gctx.ctrl, (icSourceInstance)srcIdx, sconf);
    //pthread_mutex_unlock(&gctx.lock);
}
void los_SetupSource(int srcIdx, icSourceSetup *sconf) {
    //    pthread_mutex_lock(&gctx.lock);
    _PROFILE_ADD(PROFILE_ID_LOS_SRC_CONFIG, sconf, srcIdx);
    icSetupSource (gctx.ctrl, (icSourceInstance)srcIdx, sconf);
    //    pthread_mutex_unlock(&gctx.lock);
}
void los_SetupSourceCommit(void) {
    pthread_mutex_lock(&gctx.lock);
    /* TODO: _PROFILE_ADD */
    icSetupSourcesCommit(gctx.ctrl);
    //assert(sem_wait(&semWaitForSourceCommit) != -1);
    pthread_mutex_unlock(&gctx.lock);
}

void los_ipipe_LockZSL(uint32_t srcIdx) {
    _PROFILE_ADD(PROFILE_ID_LOS_LOCK_ZSL, 0, srcIdx);
    icLockZSL (gctx.ctrl, srcIdx, 0, IC_LOCKZSL_TS_RELATIVE);
}


void los_ipipe_TriggerCapture(void* buff, void *iconf, uint32_t srcIdx) {
    _PROFILE_ADD(PROFILE_ID_LOS_TRIGGER_CAPTURE, buff, srcIdx);
    icTriggerCapture (gctx.ctrl, srcIdx, buff, iconf, 0);
}

void los_configIsp(void *iconf, int ispIdx) {
    //    pthread_mutex_lock(&gctx.lock);
    _PROFILE_ADD(PROFILE_ID_LOS_CONFIG_ISP, ((icIspConfig *)iconf)->userData, ispIdx);
    //printf("Cfg: %p, %d \n", ((icIspConfig *)iconf)->userData, ispIdx);
    // TODO: HACK- until resize capability will be added on los side
    icIspConfig *ispCfg = (icIspConfig*)iconf;
    ispCfg->updnCfg0.vN   = 1;
    ispCfg->updnCfg0.vD   = 1;
    ispCfg->updnCfg0.hN   = 1;
    ispCfg->updnCfg0.hD   = 1;
    icConfigureIsp(gctx.ctrl, ispIdx, iconf);
    //    pthread_mutex_unlock(&gctx.lock);
}

int los_startSource(int srcIdx) {
    pthread_mutex_lock(&gctx.lock);
    _PROFILE_ADD(PROFILE_ID_LOS_SRC_SART, 0, srcIdx);
    icStartSource(gctx.ctrl, (icSourceInstance)srcIdx);
    assert(sem_wait(&semWaitForSourceReady) != -1);
    pthread_mutex_unlock(&gctx.lock);
    return 0;
}

int los_stopSource(int srcIdx) {
    pthread_mutex_lock(&gctx.lock);
    _PROFILE_ADD(PROFILE_ID_LOS_SRC_STOP, 0, srcIdx);
    icStopSource(gctx.ctrl, (icSourceInstance)srcIdx);
    assert(sem_wait(&semWaitForSourceStoped) != -1);
    pthread_mutex_unlock(&gctx.lock);
    return 0;
}

void los_dataWasSent(FrameT *dataBufStruct, uint32_t outputId, uint32_t frmType) {
    UNUSED(outputId);
    UNUSED(frmType);
    _PROFILE_ADD(PROFILE_ID_LRT_DATA_SENT, dataBufStruct->userData, 0);
    icDataReceived(gctx.ctrl, dataBufStruct);
}


//=================================================================================================
//=================================================================================================
//         debug information. what is the fps for each output.
//          moviDebug command: "get fpsCams"
//=================================================================================================
float fpsCams[IPIPE_MAX_OUTPUTS_ALlOWED];
static uint64_t oldTime[IPIPE_MAX_OUTPUTS_ALlOWED];
static uint32_t frmContor[IPIPE_MAX_OUTPUTS_ALlOWED];
uint32_t frmCounterTotal[IPIPE_MAX_OUTPUTS_ALlOWED];

void updateFps(uint32_t outputId) {
    struct timespec tp;

    (frmContor[outputId])++;

    if(0 == oldTime[outputId]) {
        clock_gettime(CLOCK_REALTIME, &tp);
        oldTime[outputId] = (uint64_t)((uint64_t)tp.tv_nsec/1000 + (uint64_t)tp.tv_sec * 1000000);
        return;
    }

    if(frmContor[outputId] > 240) {
        clock_gettime(CLOCK_REALTIME, &tp);
        uint64_t crtTime = (uint64_t)((uint64_t)tp.tv_nsec/1000 + (uint64_t)tp.tv_sec * 1000000);
        uint64_t difTime = crtTime - oldTime[outputId];
        fpsCams[outputId] = (float)frmContor[outputId]/((float)difTime/1000000.0f);
        frmContor[outputId] = 0;
        oldTime[outputId] = crtTime;
    }
    frmCounterTotal[outputId]++;
}


