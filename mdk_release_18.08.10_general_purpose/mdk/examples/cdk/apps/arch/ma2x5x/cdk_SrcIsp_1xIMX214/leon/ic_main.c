///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test app main()
///

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
#include "hal/hal_camera_module/hat_cm_driver.h"
#include "sendOutApi.h"

#include <utils/mms_debug.h>
#include <utils/profile/profile.h>
#include <osal/osal_stdlib.h>
#include <osal/osal_mutex.h>
#include <osal/osal_time.h>
#include <osal/osal_string.h>
#include <camera_control.h>

mmsdbg_define_variable(
        vdl_ic,
        DL_DEFAULT,
        0,
        "vdl_ic",
        "Guzzi IC."
    );
#define MMSDEBUGLEVEL mmsdbg_use_variable(vdl_ic)

#define INSTANCES_COUNT_MAX     MAX_NR_OF_CAMS

#define IC_IMAGE_WIDTH           4208

extern int custom_cam_command;

void platform_cam_led1_toggle (void);

void los_ConfigureSource(
        int srcIdx,
        icSourceConfig *sconf,
        int pipeId
    );
void los_SetupSource(
        int srcIdx,
        icSourceSetup *sconf
    );
void los_SetupSourceCommit(void);
void inc_cam_stats_ready(
        void *p_prv,
        unsigned int seqNo,
        void *p_cfg_prv
    );
void inc_cam_capture_ready(
        void *p_prv,
        unsigned int seqNo,
        void *p_cfg_prv
    );
void inc_cam_ipipe_cfg_ready(
        void *p_prv,
        unsigned int seqNo,
        void *p_cfg_prv
    );
void inc_cam_ipipe_buff_locked(
        void *p_prv,
        void *userData,
        unsigned int sourceInstance,
        void *buffZsl,
        icTimestamp ts,
        unsigned int seqNo
    );
void inc_cam_frame_start(
        void *p_prv,
        uint32_t sourceInstance,
        uint32_t seqNo,
        icTimestamp ts,
        void *userData
    );
void inc_cam_frame_line_reached(
        void *p_prv,
        uint32_t sourceInstance,
        uint32_t seqNo,
        icTimestamp ts,
        void *userData
    );
void inc_cam_frame_end(
        void *p_prv,
        uint32_t sourceInstance,
        uint32_t seqNo,
        icTimestamp ts,
        void *userData
    );

void los_dataWasSent(FrameT *dataBufStruct, uint32_t outputId, uint32_t frmType);

void inc_cam_terminate_fr(
        void *p_prv,
        uint32_t sourceInstance,
        void *userData
    );

typedef struct {
    uint32_t    ref_cnt;
    osal_mutex *lock;
    icCtrl     *ctrl;
} g_ipipe_ctx_t;

static g_ipipe_ctx_t gctx;

sem_t semWaitForLrtReady;
sem_t semWaitForSourceCommit;
sem_t semWaitForSourceReady;
sem_t semWaitForSourceStoped;

static pthread_t eventThread;

icSourceSetup srcSet = {
        0, 0, 12, 0, 1, 1, 1, 1, QUARTER_MODE_FOR_VIDEO_PIPE
};

#define DDR_BUFFER_ALOCATED_MEM_SIZE ((128-30)*1024*1024)

static uint8_t ddrStaticAlocatedMemory[DDR_BUFFER_ALOCATED_MEM_SIZE] __attribute__((
        section(".ddr.bss")
    ));


/*
 * ****************************************************************************
 * ** Misc ********************************************************************
 * ****************************************************************************
 */

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

    /*
     * This thread runs until it is cancelled (pthread_cond_wait() is a
     * cancellation point).
     */
    for (;;) {
        if (!icGetEvent(ctrl, &ev)) {
            evno = ev.ctrl & IC_EVENT_CTRL_TYPE_MASK;

            switch (evno) {
            case IC_EVENT_TYPE_LEON_RT_READY:
                /* TODO: PROFILE_ADD */
                assert(sem_post(&semWaitForLrtReady) != -1);
                break;
            case IC_EVENT_TYPE_SETUP_SOURCES_RESULT:
                /* TODO: PROFILE_ADD */
                assert(sem_post(&semWaitForSourceCommit) != -1);
                break;
            case IC_EVENT_TYPE_SOURCE_READY:
                /* TODO: PROFILE_ADD */
                assert(sem_post(&semWaitForSourceReady) != -1);
                break;
            case IC_EVENT_TYPE_READOUT_START:
                if (ev.u.lineEvent.userData != NULL) {
                    PROFILE_ADD(
                            PROFILE_ID_LRT_START_FRAME,
                            ev.u.lineEvent.seqNo,
                            ev.u.lineEvent.sourceInstance
                        );
                    inc_cam_frame_start(
                            NULL,
                            ev.u.lineEvent.sourceInstance,
                            ev.u.lineEvent.seqNo,
                            ev.u.lineEvent.ts,
                            ev.u.lineEvent.userData
                        );
                    inc_cam_ipipe_cfg_ready(
                            NULL,
                            ev.u.lineEvent.seqNo,
                            ev.u.lineEvent.userData
                        );
                    inc_cam_frame_end(
                            NULL,
                            ev.u.lineEvent.sourceInstance,
                            ev.u.lineEvent.seqNo,
                            ev.u.lineEvent.ts,
                            ev.u.lineEvent.userData
                        );
                }
                break;
            case IC_EVENT_TYPE_LINE_REACHED:
                if (ev.u.lineEvent.userData != NULL) {
                    PROFILE_ADD(
                            PROFILE_ID_LRT_LINE_REACHED,
                            ev.u.lineEvent.userData,
                            ev.u.lineEvent.sourceInstance
                        );
                    #if 0 /* TODO: IPIPE2 not implelmented */
                    inc_cam_frame_line_reached(
                            NULL,
                            ev.u.lineEvent.sourceInstance,
                            ev.u.lineEvent.seqNo,
                            ev.u.lineEvent.ts,
                            ev.u.lineEvent.userData
                        );
                    #endif
                }
                break;
            case IC_EVENT_TYPE_READOUT_END:
                if (ev.u.lineEvent.userData != NULL) {
                    PROFILE_ADD(
                            PROFILE_ID_LRT_END_FRAME,
                            ev.u.lineEvent.userData,
                            ev.u.lineEvent.sourceInstance
                        );
                    inc_cam_stats_ready(
                            NULL,
                            ev.u.lineEvent.seqNo,
                            ev.u.lineEvent.userData
                        );
                }
                break;
            case IC_EVENT_TYPE_ISP_START:
                if (ev.u.ispEvent.userData != NULL) {
                    PROFILE_ADD(
                            PROFILE_ID_LRT_ISP_START,
                            ev.u.ispEvent.userData,
                            ev.u.ispEvent.ispInstance
                        );
                } else {
                    mmsdbg(
                            DL_ERROR,
                            "IC_EVENT_TYPE_ISP_START with userdata == NULL"
                        );
                }
                break;
            case IC_EVENT_TYPE_ISP_END:
                if (ev.u.ispEvent.userData != NULL) {
                    PROFILE_ADD(
                            PROFILE_ID_LRT_ISP_END,
                            ev.u.ispEvent.userData,
                            ev.u.ispEvent.ispInstance
                        );
                } else {
                    mmsdbg(
                            DL_ERROR,
                            "IC_EVENT_TYPE_ISP_END with userdata == NULL"
                        );
                }
                break;
            case IC_EVENT_TYPE_ZSL_LOCKED:
                PROFILE_ADD(
                        PROFILE_ID_LRT_ZSL_LOCKED,
                        ev.u.buffLockedZSL.userData,
                        ev.u.buffLockedZSL.sourceInstance
                    );
                inc_cam_ipipe_buff_locked(
                        NULL,
                        ev.u.buffLockedZSL.userData,
                        ev.u.buffLockedZSL.sourceInstance,
                        ev.u.buffLockedZSL.buffZsl,
                        ev.u.buffLockedZSL.buffZsl->timestamp[0], // time stamp when zsl was arrived in memory
                        //ev.u.buffLockedZSL.buffZsl->timestamp[ev.u.buffLockedZSL.buffZsl->timestampNr-1] // time stamp when frame was processed
                        ev.u.buffLockedZSL.buffZsl->seqNo
                    );
                break;
            case IC_EVENT_TYPE_SOURCE_STOPPED:
                /* TODO: PROFILE_ADD */
                assert(sem_post(&semWaitForSourceStoped) != -1);
                break;
            case IC_EVENT_TYPE_SEND_OUTPUT_DATA:
                {
                FrameT *frame;
                uint32_t outId;
                uint32_t frmType;
                uint32_t frmFmt;

                platform_cam_led1_toggle();

                frame   = ev.u.sentOutput.dataBufStruct;
                // guzzi create a dependency between source id and output id, so here will be the source id
                // depend on
                outId    = ctrl->icPipelineDescription.icQueryOutput[ev.u.sentOutput.outputId]->dependentSources;
                //outId   = ev.u.sentOutput.outputId;
                frmFmt  = ev.u.sentOutput.dataBufStruct->type; // format is yuv,rgb
                frmType = ctrl->icPipelineDescription.icQueryOutput[ev.u.sentOutput.outputId]->attrs; //type is still, video ...

                if (frame->stride[0] == IC_IMAGE_WIDTH){ //still frame
                    frmFmt  = FRAME_T_FORMAT_YUV420;
                    frmType = FRAME_DATA_TYPE_STILL;
                }

                PROFILE_ADD(
                        PROFILE_ID_LRT_SEND_DATA,
                        frame->appSpecificData,
                        outId
                    );

                assert(outId<INSTANCES_COUNT_MAX);
                assert(frmType<FRAME_DATA_TYPE_MAX);
                assert(frmFmt<FRAME_T_FORMAT_MAX);

                    sendOutSend(frame, outId, frmType, los_dataWasSent);
                }
                break;
            case IC_EVENT_TYPE_ERROR:
                PROFILE_ADD(
                        PROFILE_ID_LRT_ERROR,
                        ev.u.error.userData,
                        ev.u.error.sourceInstance
                    );
                if ((ev.u.error.userData) && (ev.u.error.errorNo == IC_ERROR_RT_OUT_BUFFERS_NOT_AVAILABLE)) {
                    inc_cam_terminate_fr(
                            NULL,
                            ev.u.error.sourceInstance,
                            ev.u.error.userData
                        );
                }
                break;
            case IC_EVENT_TYPE_TORN_DOWN:
                /* TODO: PROFILE_ADD */
                return NULL;
            default:
                mmsdbg(DL_ERROR, "Unknown evt: %d", evno);
            }
        } else {
            /*
             * no activity in last period, lrt crash
             * or lrt is started, but no camera connected
             * in this case cut down lrt
             */
            mmsdbg(DL_ERROR, "Error X");
        }
    }

    return NULL;
}

void los_start(void *arg)
{
    int policy;
    struct sched_param param;

    UNUSED(arg);

    PROFILE_ADD(PROFILE_ID_LOS_START, 0, 0);

    gctx.ref_cnt++;
    if (gctx.ref_cnt == 1) {
        gctx.lock = osal_mutex_create();

        assert(sem_init(&semWaitForLrtReady, 0, 0) != -1);
        assert(sem_init(&semWaitForSourceCommit, 0, 0) != -1);
        assert(sem_init(&semWaitForSourceReady, 0, 0) != -1);
        assert(sem_init(&semWaitForSourceStoped, 0, 0) != -1);
        gctx.ctrl = icSetup(
                1,
                (uint32_t)ddrStaticAlocatedMemory,
                sizeof (ddrStaticAlocatedMemory)
            );

        assert(pthread_create(&eventThread, NULL, eventLoop, &gctx) == 0);

        pthread_getschedparam(eventThread, &policy, &param);
        param.sched_priority = 210;
        pthread_setschedparam(eventThread, policy, &param);

        assert(sem_wait(&semWaitForLrtReady) != -1);

        sendOutInit();

        {
            int i;
            for (i = 0; i<INSTANCES_COUNT_MAX; i++)
            {
            hat_camera_limits_t limits;
                if ( 0 == hai_cm_driver_get_camera_limits(i, &limits)) {
                    srcSet.maxWidth  = limits.maxWidth;
                    srcSet.maxHeight = limits.maxHeight;
                    srcSet.maxPixels = limits.maxPixels;
                    srcSet.maxBpp    = 12; //limits.maxBpp; // Hack - force IpipeServer to allocate big enough buffers
                    los_SetupSource(i, &srcSet);
                }
            }
        }

        los_SetupSourceCommit();
    }
}

void los_stop(void)
{
    PROFILE_ADD(PROFILE_ID_LOS_STOP, 0, 0);

    assert(gctx.ref_cnt);

    gctx.ref_cnt--;
    if (!gctx.ref_cnt) {
        osal_usleep(33*1000);  // Wait for all in process frames to finish
        icTeardown(gctx.ctrl);
        pthread_join(eventThread, NULL);

        sendOutFini();
        osal_mutex_destroy(gctx.lock);
        sem_destroy(&semWaitForSourceStoped);
        sem_destroy(&semWaitForSourceReady);
        sem_destroy(&semWaitForSourceCommit);
        sem_destroy(&semWaitForLrtReady);
    }
}

void los_ConfigureGlobal(void *gconf)
{
    PROFILE_ADD(PROFILE_ID_LOS_GOB_CONFIG, gconf, 0);
}

void los_ConfigureSource(int srcIdx, icSourceConfig *sconf, int pipeId)
{
    UNUSED(pipeId);
//    osal_mutex_lock(gctx.lock);

    //Init source in RT
    PROFILE_ADD(PROFILE_ID_LOS_SRC_CONFIG, sconf, srcIdx);
    icConfigureSource(gctx.ctrl, (icSourceInstance)srcIdx, sconf);
//    osal_mutex_unlock(gctx.lock);
}
void los_SetupSource(int srcIdx, icSourceSetup *sconf)
{
//    osal_mutex_lock(gctx.lock);
    PROFILE_ADD(PROFILE_ID_LOS_SRC_CONFIG, sconf, srcIdx);
    icSetupSource (gctx.ctrl, (icSourceInstance)srcIdx, sconf);
//    osal_mutex_unlock(gctx.lock);
}
void los_SetupSourceCommit(void)
{
    osal_mutex_lock(gctx.lock);
    /* TODO: PROFILE_ADD */
    icSetupSourcesCommit(gctx.ctrl);
    assert(sem_wait(&semWaitForSourceCommit) != -1);
    osal_mutex_unlock(gctx.lock);
}

void los_ipipe_LockZSL(uint32_t srcIdx) {
    PROFILE_ADD(PROFILE_ID_LOS_LOCK_ZSL, 0, srcIdx);
    icLockZSL (gctx.ctrl, srcIdx, 0, IC_LOCKZSL_TS_RELATIVE);
}


void los_ipipe_TriggerCapture(void* buff, void *iconf, uint32_t srcIdx) {
    PROFILE_ADD(PROFILE_ID_LOS_TRIGGER_CAPTURE, buff, srcIdx);
    icTriggerCapture (gctx.ctrl, srcIdx, buff, iconf, 0);
}

void los_configIsp(void *iconf, int ispIdx)
{
//    osal_mutex_lock(gctx.lock);
    PROFILE_ADD(PROFILE_ID_LOS_CONFIG_ISP, ((icIspConfig *)iconf)->userData, ispIdx);
    //printf("Cfg: %p, %d \n", ((icIspConfig *)iconf)->userData, ispIdx);
    // TODO: HACK- until resize capability will be added on los side
    icIspConfig *ispCfg = (icIspConfig*)iconf;
    switch (custom_cam_command){
        case (CUSTOM_CAMERA_COMMAND__CAPTURE_MODE):
            ispCfg->updnCfg0.vN   = 1;
            ispCfg->updnCfg0.vD   = 1;
            ispCfg->updnCfg0.hN   = 1;
            ispCfg->updnCfg0.hD   = 1;
            custom_cam_command = CUSTOM_CAMERA_COMMAND__PREVIEW_MODE;
            break;
        default:
            mmsdbg(DL_ERROR, "Unknown custom control command: %d. Presuming preview mode.", custom_cam_command);
        case (CUSTOM_CAMERA_COMMAND__PREVIEW_MODE):
            ispCfg->updnCfg0.vN   = 1;
            ispCfg->updnCfg0.vD   = 2;
            ispCfg->updnCfg0.hN   = 1;
            ispCfg->updnCfg0.hD   = 2;
            break;
    }
    icConfigureIsp(gctx.ctrl, ispIdx, iconf);
//    osal_mutex_unlock(gctx.lock);
}

int los_startSource(int srcIdx)
{

    osal_mutex_lock(gctx.lock);
    PROFILE_ADD(PROFILE_ID_LOS_SRC_SART, 0, srcIdx);
    icStartSource(gctx.ctrl, (icSourceInstance)srcIdx);
    assert(sem_wait(&semWaitForSourceReady) != -1);
    osal_mutex_unlock(gctx.lock);

    return 0;
}

int los_stopSource(int srcIdx)
{
    osal_mutex_lock(gctx.lock);
    PROFILE_ADD(PROFILE_ID_LOS_SRC_STOP, 0, srcIdx);
    icStopSource(gctx.ctrl, (icSourceInstance)srcIdx);
    assert(sem_wait(&semWaitForSourceStoped) != -1);
    osal_mutex_unlock(gctx.lock);

    return 0;
}

void los_dataWasSent(FrameT *dataBufStruct, uint32_t outputId, uint32_t frmType)
{
    UNUSED(outputId);
    UNUSED(frmType);

    PROFILE_ADD(
            PROFILE_ID_LRT_DATA_SENT,
            dataBufStruct->appSpecificData,
            0
        );
    if (gctx.ref_cnt)
    {
        icDataReceived(gctx.ctrl, dataBufStruct);
    }
}

