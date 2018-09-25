///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#include "aeThread.h"

#include <errno.h>
#include <pthread.h>
#include "aeStats.h"
#include "aeAlgo.h"
#include <LeonIPCApi.h>
#include <swcShaveLoader.h>
#include <mvMacros.h>
#include <rtems.h>
#include <OsDrvCmxDma.h>

#include "aeCommStructs.h"

#include <mvLog.h>

#define PROCESS_MSG_SIZE       (sizeof(convertMessage_t) / sizeof(u32))
#define PROCESS_MSG_QUEUE_SIZE 2

#define AE_MSG_SIZE       (sizeof(aeMessage_t) / sizeof(u32))
#define AE_MSG_QUEUE_SIZE 2

#define CMX_NOCACHE __attribute__((section(".cmx_direct.data")))
#undef CMX_DATA // TODO: nasty thing here
#define CMX_DATA __attribute__((section(".cmx.data")))
#define STATS_AREA  __attribute__((section(".cmx_direct.data")))
#define DDR_BSS __attribute__((section(".ddr.bss")))

#define CONVERSION_SHAVE_NR 4

pthread_t aeThread;

leonIPCChannel_t CMX_NOCACHE ipcLRTtoLOS_AEvalues;
uint32_t CMX_NOCACHE messagePoolLRTtoLOS_AEvalues[AE_MSG_QUEUE_SIZE * AE_MSG_SIZE];

extern leonIPCChannel_t ipcLOStoLRT_ae[MAX_NR_CAMS];

// Number of cameras available on this board. Initialized before LRT boots up.
extern volatile int ae_camera_count;

static multiCamExp_t multiCamExp;

/// TODO: Use MAX/12 number of patches to allocate statically the AE statistics buffer for now (32kB to fit in CMX memory map).
/// This would be sufficient for VGA image resolution with patch size 32 pixels (stats elem 48B x 300 patches x 2 cameras).
/// The proper way would be to allocate the buffer dynamically - since the mounted camera is detected runtime.
/// TODO: increased stats buffer size *4 to have enough space for 720p
#define AE_STATS_BUFFER_SZ  (AE_STATS_MAX_PATCHES_HORZ * AE_STATS_MAX_PATCHES_VERT / 12 * 4)

ae_patch_stats statsPatch[MAX_NR_CAMS][AE_STATS_BUFFER_SZ] STATS_AREA __attribute__((aligned(16))); /// AE Statistics Buffer

static aeCamCfg_t camCfg[MAX_NR_CAMS];              /// Camera exposure and gain config info (supported ranges)

static aeExpGainTable_t aeExpGainTbl[MAX_NR_CAMS];  /// Camera {Exposure,Gain} regions table

static u16 dummyOutput[1280*720*2] DDR_BSS;

#define EVENT_CONVERSION_DONE RTEMS_EVENT_7
void convertDoneCb(u32 source)
{
    (void)source;
    rtems_event_send(aeThread, EVENT_CONVERSION_DONE);
}

pthread_mutex_t sippMutex;

struct processThreadCtx {
    int cnt;
    pthread_mutex_t *sippMutex;
};

static void initCamAEConfig(aeCamCfg_t *camAECfg, convertMessage_t *cfgMsg)
{
    if (cfgMsg->type == AE_MSG_CAM_PARAM_CFG)
    {
        camAECfg->min.integrationTime = cfgMsg->minExpTime;
        camAECfg->min.gainCode        = cfgMsg->minGain;
        camAECfg->max.integrationTime = cfgMsg->maxExpTime;
        camAECfg->max.gainCode        = cfgMsg->maxGain;
        camAECfg->gainFractBits       = cfgMsg->gainFractBits;

        //Limit exposure time to 20000 us
        camAECfg->max.integrationTime = MIN(camAECfg->max.integrationTime, 20000u);
    }
}

static int initExpGainTable(aeExpGainTable_t *tblExpGain, convertMessage_t *aeTblMsg)
{
    if (aeTblMsg->type == AE_MSG_EXP_GAIN_TBL)
    {
        int rgnCntr;

        for (rgnCntr = 0; rgnCntr < AE_EXPGAIN_TBL_MAX_SZ; rgnCntr++)
        {
            if (aeTblMsg->region[rgnCntr].exposureTimeUs == 0 || aeTblMsg->region[rgnCntr].gainMultiplier == 0.f)
                break;                          /// Table termination entry reached

            if (rgnCntr > 0)
            {
                if (aeTblMsg->region[rgnCntr].exposureTimeUs == aeTblMsg->region[rgnCntr - 1].exposureTimeUs) {
                    tblExpGain->region[rgnCntr].type = AE_REGION_TYPE_CONST_EXP;
                    if (aeTblMsg->region[rgnCntr].gainMultiplier <= aeTblMsg->region[rgnCntr - 1].gainMultiplier) {
                        mvLog(MVLOG_WARN, "Region %d: Gain isn't increasing while Exposure = const !!!\n", rgnCntr);
                        tblExpGain->regionNmbr = 0;
                        return (-1);            /// Gain value isn't correct -> discard the table
                    }
                } else if (aeTblMsg->region[rgnCntr].gainMultiplier == aeTblMsg->region[rgnCntr - 1].gainMultiplier) {
                    tblExpGain->region[rgnCntr].type = AE_REGION_TYPE_CONST_GAIN;
                    if (aeTblMsg->region[rgnCntr].exposureTimeUs <= aeTblMsg->region[rgnCntr - 1].exposureTimeUs) {
                        mvLog(MVLOG_WARN, "Region %d: Exposure isn't increasing while Gain = const !!!\n", rgnCntr);
                        tblExpGain->regionNmbr = 0;
                        return (-1);            /// Exposure value isn't correct -> discard the table
                    }
                } else {
                    mvLog(MVLOG_WARN, "Region %d: either Exposure or Gain should be kept const !!!\n", rgnCntr);
                    tblExpGain->regionNmbr = 0;
                    return (-1);                /// Neither Exposure nor Gain is kept const in the region -> discard the table
                }
            } else
                tblExpGain->region[rgnCntr].type = AE_REGION_TYPE_NA;

            tblExpGain->region[rgnCntr].range.integrationTime = aeTblMsg->region[rgnCntr].exposureTimeUs;
            tblExpGain->region[rgnCntr].range.gainCode        = aeTblMsg->region[rgnCntr].gainMultiplier;
            tblExpGain->region[rgnCntr].totalExp              = aeTblMsg->region[rgnCntr].exposureTimeUs *
                                                                aeTblMsg->region[rgnCntr].gainMultiplier;
        }

        if (rgnCntr <= 1)
        {
            tblExpGain->regionNmbr = 0;
            return (-1);                        /// {Exposure,Gain} regions table is empty/corrupt
        }

        tblExpGain->regionNmbr = rgnCntr - 1;
    }

    return 0;
}

void * aeLoop(void * ctx)
{
    struct processThreadCtx *tc = (struct processThreadCtx*)ctx;
    int sc;
    convertMessage_t message;
    FramePumpBuffer* fpbIn;
    uint64_t stamp, ticks;

    rtems_object_set_name(rtems_task_self(), __func__);

    sc = LeonIPCRxInit(&ipcLOStoLRT_ae[tc->cnt], NULL, IRQ_DYNAMIC_7 + tc->cnt, 5);
    if(sc != IPC_SUCCESS)
        mvLog(MVLOG_INFO, "LRT Unable to initialize RX leon OS to RT IPC !!!!");

    while(1)
    {

        sc = LeonIPCWaitMessage(&ipcLOStoLRT_ae[tc->cnt], 100);
        if(sc)
        {
            // If timeout reached, reset AE
            multiCamExp.camBitMask = 0;
            multiCamExp.frameNo = 0;

            continue;
            //TODO: watchdog timer here to restart LRT/shaves
        }

        sc = LeonIPCReadMessage(&ipcLOStoLRT_ae[tc->cnt], (u32*)&message);
        if (sc != IPC_SUCCESS)
        {
            mvLog(MVLOG_INFO, "ipcreadfail %d\n", sc);
            continue;
        }

        if (message.type == AE_MSG_PROCESS_FRAME)
        {
            aeStatsConfig_t *aeStatsCfg;

            fpbIn = &message.fpbIn;

            mvLog(MVLOG_DEBUG, "start camid nrcams   %d %d",
                  (int)fpbIn->cam_id, (int)fpbIn->frameCount);

            pthread_mutex_lock(tc->sippMutex);
            stamp = rtems_clock_get_uptime_nanoseconds ();
            // Get Camera resolution info on first frame message and create AE Stats pipe
            aeStatsCfg = aeStatsCheckIfConfig(fpbIn->fb.spec.width, fpbIn->fb.spec.height, fpbIn->fb.spec.bytesPP);
            if (aeStatsCfg == NULL) {};         // TODO: Add error handling / message
            if (aeStatsCfg->patchNmbrHorz * aeStatsCfg->patchNmbrVert > AE_STATS_BUFFER_SZ)
                mvLog(MVLOG_ERROR, "AE statsPatch buffer will overflow!!! Please increase AE_STATS_BUFFER_SZ!\n");
            aeStatsRun(&fpbIn->fb, dummyOutput, &statsPatch[fpbIn->cam_id][0]);
            ticks = rtems_clock_get_uptime_nanoseconds () - stamp;
            pthread_mutex_unlock(tc->sippMutex);

            mvLog(MVLOG_DEBUG, "stop camid nrcams id %d frameCount %d time %d ns",
                  (int)fpbIn->cam_id, (int)fpbIn->frameCount, (int) ticks);

            multiCamExp.camBitMask |= (1 << fpbIn->cam_id);
            multiCamExp.exp[fpbIn->cam_id] =
                aeGetNewExposure(&statsPatch[fpbIn->cam_id][0],
                                 aeStatsCfg->patchNmbrHorz,
                                 aeStatsCfg->patchNmbrVert,
                                 aeStatsCfg->patchWidth,
                                 aeStatsCfg->patchHeight,
                                 fpbIn->cam_id,
                                 message.curTotalExp,
                                 message.statsTotalExp,
                                 &camCfg[fpbIn->cam_id],
                                 &aeExpGainTbl[fpbIn->cam_id]);

            const int all_cam_exp_mask = (1 << ae_camera_count) - 1;
            if(multiCamExp.camBitMask == all_cam_exp_mask)  // Check if stat-s for all cameras are available
            {
                aeMessage_t aeMessage;
                int camId, sts;
                mvLog(MVLOG_DEBUG, "Sending back exposure for frame %d !!!\n",(int)fpbIn->frameCount);

                // Method to select common exposure for all cameras
                sts = aeMultiCamExpSelect(&multiCamExp, camCfg,
                                          ae_camera_count);
                if (sts == STS_MULTICAM_WRONG_EXP) {
                    mvLog(MVLOG_INFO, "Error when selecting common exposure for all cameras!!!\n");
                } else {
                    aeMessage.integrationTimeUs = multiCamExp.exp[0].integrationTime;   // Fill in common exposure time for all cameras
                    for (camId = 0; camId < ae_camera_count; camId++)
                    {
                        if (multiCamExp.camBitMask & (1 << camId))          // Ensure that current camera has valid stats (exp,gain)
                            aeMessage.gainCode[camId] = multiCamExp.exp[camId].gainCode;    // Fill in individual camera gains
                    }
                    sc = LeonIPCSendMessage(&ipcLRTtoLOS_AEvalues, (u32*)&aeMessage);
                    if(sc != IPC_SUCCESS)
                        mvLog(MVLOG_INFO, "LRT ae IPC unable to send message ! %d \n", sc);
                    if (sts == STS_MULTICAM_GAIN_OVF)
                        mvLog(MVLOG_DEBUG, "Common Exposure: Gain reached MAX for some camera!");
                }

                multiCamExp.frameNo++;
                multiCamExp.camBitMask = 0;
            }
        }
        else if (message.type == AE_MSG_CAM_PARAM_CFG)
        {
            initCamAEConfig(&camCfg[tc->cnt], &message);
        }
        else if (message.type == AE_MSG_EXP_GAIN_TBL)
        {
            if (initExpGainTable(&aeExpGainTbl[tc->cnt], &message))
                mvLog(MVLOG_WARN, "Camera %d: {Exposure,Gain} regions table is empty/corrupt !!!\n", tc->cnt);
        }
        else
        {
            mvLog(MVLOG_INFO, "Unsupported IPC AE message type (%d) received !!!\n", message.type);
        }
    }
}

struct processThreadCtx contexts[MAX_NR_CAMS];

void aeThreadInit(void)
{
    int sc;
    pthread_attr_t attr;
    int i;

    aeStatsInit();

    if(pthread_attr_init(&attr) !=0) {
        mvLog(MVLOG_INFO, "pthread_attr_init error");
    }
    if(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0) {
        mvLog(MVLOG_INFO, "pthread_attr_setinheritsched error");
    }
    if(pthread_attr_setschedpolicy(&attr, SCHED_FIFO) != 0) {
        mvLog(MVLOG_INFO, "pthread_attr_setschedpolicy error");
    }

    pthread_mutex_init(&sippMutex, NULL);

#ifndef MA2X8X
    sc = OsDrvCmxDmaInit(7, 7, 1, 1);
    if (sc)
        mvLog(MVLOG_ERROR, "CMX DMA init error %d! ", sc);
#endif

    memset(&multiCamExp, 0, sizeof(multiCamExp));

    mvLog(MVLOG_INFO, "Initializing auto exposure. Camera count: %d.",
          ae_camera_count);

    for(i = 0 ; i < ae_camera_count; i++)
    {
        contexts[i].sippMutex = &sippMutex;
        contexts[i].cnt = i;
        sc=pthread_create( &aeThread, &attr, aeLoop, (void*)&contexts[i]);
        if (sc) {
            mvLog(MVLOG_FATAL, "Cannot create auto-exposure thread: %s\n", strerror(errno));
        }
    }

    sc = LeonIPCTxInit(&ipcLRTtoLOS_AEvalues, messagePoolLRTtoLOS_AEvalues,
                       AE_MSG_QUEUE_SIZE, AE_MSG_SIZE);
    if(sc != IPC_SUCCESS)
        mvLog(MVLOG_INFO, "LRT Unable to initialize AutoExposure TX leon RT to OS IPC !!!!\n");
}
