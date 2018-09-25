///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#include <sys/ioctl.h>
#include <DrvIcb.h>
#include "LeonIPCApi.h"
#include "mvMacros.h"

#include "aeCommStructs.h"
#include "aeApi.h"

#define MVLOG_UNIT_NAME simpleae
#include "mvLog.h"

#define CMX_NOCACHE  __attribute__((section(".cmx_direct.data")))

#define CAMCOUNT 2

#define MSG_SIZE_PROCESS       (sizeof(convertMessage_t) / sizeof(u32))
#define MSG_QUEUE_SIZE_PROCESS 1

leonIPCChannel_t CMX_NOCACHE lrt_ipcLOStoLRT_ae[CAMCOUNT];
static uint32_t CMX_NOCACHE messagePoolLOStoLRT_ae[CAMCOUNT][MSG_QUEUE_SIZE_PROCESS * MSG_SIZE_PROCESS];
extern leonIPCChannel_t lrt_ipcLRTtoLOS_AEvalues;

static volatile u32 ipcErrorCounter;

/// Keep Total Exposure history for each camera
static float TE_N[CAMCOUNT];
static float TE_N_Minus_1[CAMCOUNT];
// static float TE_N_Minus_2[COUNT_OF(cams)];

static int ipcSendAEConfig(int id, FramePumpCamExpGainInfo *aeConfig)
{
    int sc;
    convertMessage_t message;

    message.type          = AE_MSG_CAM_PARAM_CFG;
    message.minExpTime    = aeConfig->min.exposureTimeUs;
    message.minGain       = aeConfig->min.gainMultiplier;
    message.maxExpTime    = aeConfig->max.exposureTimeUs;
    message.maxGain       = aeConfig->max.gainMultiplier;
    message.gainFractBits = aeConfig->gainFractBits;

    /// Reuse lrt_ipcLOStoLRT_ae channel: the 1-st message sent over it holds the camera AE config
    sc = LeonIPCSendMessage(&lrt_ipcLOStoLRT_ae[id], (u32*)&message);
    if(sc != IPC_SUCCESS)
    {
        ipcErrorCounter++;
        mvLog(MVLOG_WARN, "%s LOS IPC notify unable to send message ! %d ", __func__, sc);
    }

    return sc;
}

static int ipcSendExpGainTable(int id, FramePumpCamExpGainTable *tblExpGain)
{
    int sc, i;
    convertMessage_t message;

    message.type          = AE_MSG_EXP_GAIN_TBL;
    for (i = 0; i < FP_EXPGAIN_TBL_MAX_SZ; i++)
    {
        message.region[i] = tblExpGain->region[i];
    }

    /// Reuse lrt_ipcLOStoLRT_ae channel: the 2-nd message sent over it holds the {Exposure,Gain} regions table
    sc = LeonIPCSendMessage(&lrt_ipcLOStoLRT_ae[id], (u32*)&message);
    if(sc != IPC_SUCCESS)
    {
        ipcErrorCounter++;
        mvLog(MVLOG_WARN, "%s LOS IPC notify unable to send message ! %d ", __func__, sc);
    }

    return sc;
}

static void initTotalExposureHistory(int camId, u32 exposureTimeUs, float gainMultiplier)
{
    float initialTE;

    initialTE = exposureTimeUs * gainMultiplier;
    if (initialTE <= 10)
        initialTE = 10;         /// Clip Total Exposure to min value

    // TE_N_Minus_2[camId] = initialTE;
    TE_N_Minus_1[camId] = initialTE;
    TE_N[camId]         = initialTE;
}

static void updateTotalExposureHistory(int camId, u32 exposureTimeUs, float gainMultiplier)
{
    /// Keep track of Total Exposure that was programmed for previous frames
    // TE_N_Minus_2[camId] = TE_N_Minus_1[camId];
    TE_N_Minus_1[camId] = TE_N[camId];

    TE_N[camId] = exposureTimeUs * gainMultiplier;
    if (TE_N[camId] <= 10)
        TE_N[camId] = 10;       /// Clip Total Exposure to min value
}


int simpleAEstart(const int fd, const int id, const char * name)
{
    FramePumpCamExpGainInfo camExpGainInfo;
    FramePumpCamExpGainTable camExpGainTbl;
    int sc;

    sc = ioctl(fd, FRAME_PUMP_GET_EXPGAIN_INFO, &camExpGainInfo);
    if(sc)
    {
        mvLog(MVLOG_ERROR, "Camera %s failed to get Min/Max/Start exposure time & gain values !!!", name);
        return sc;
    }
    ipcSendAEConfig(id, &camExpGainInfo);
    initTotalExposureHistory(id, camExpGainInfo.start.exposureTimeUs, camExpGainInfo.start.gainMultiplier);

    sc = ioctl(fd, FRAME_PUMP_GET_EXPGAIN_TABLE, &camExpGainTbl);
    if(sc)
    {
        mvLog(MVLOG_ERROR, "Camera %s failed to get {Exposure,Gain} regions table !!!", name);
        return sc;
    }
    ipcSendExpGainTable(id, &camExpGainTbl);

    return 0;
}

int simpleAEinit()
{
    int sc;

    for (int i = 0; i <  CAMCOUNT; i++) {
        sc = LeonIPCTxInit(&lrt_ipcLOStoLRT_ae[i], messagePoolLOStoLRT_ae[i],
                           MSG_QUEUE_SIZE_PROCESS, MSG_SIZE_PROCESS);
        if (sc != IPC_SUCCESS)
        {
            mvLog(MVLOG_FATAL, "LOS Unable to initialize TX leon OS to RT IPC %d !!!!", sc);
            return sc;
        }
    }

    return 0;
}

int simpleAEpostInit()
{
    int sc;

    sc = LeonIPCRxInit(&lrt_ipcLRTtoLOS_AEvalues, NULL, IRQ_DYNAMIC_7, 5);
    if(sc != IPC_SUCCESS)
    {
        mvLog(MVLOG_FATAL, "LOS Unable to initialize RX leon RT to OS IPC !!!!");
        return sc;
    }
    else
    {
        sc = LeonIPCRxReleaseSinkThread(&lrt_ipcLRTtoLOS_AEvalues);
        if(sc)
            mvLog(MVLOG_ERROR, "PostInit IPC release error %d", sc);
    }

    return 0;
}


aeMessage_t simpleAEget ()
{
    FramePumpCamExpGain camExpGainPair;
    aeMessage_t received = {0};
    uint32_t noMessages;
    int sc;

    sc = LeonIPCRxReassignSinkThread(&lrt_ipcLRTtoLOS_AEvalues);
    if(sc)
        mvLog(MVLOG_ERROR, "IPC reassign error %d", sc);

    sc = LeonIPCNumberOfPendingMessages(&lrt_ipcLRTtoLOS_AEvalues, &noMessages);
    if(sc)
        mvLog(MVLOG_WARN, "Error getting messages %d !", sc);

    if(noMessages == 0)
    {
        sc = LeonIPCWaitMessage(&lrt_ipcLRTtoLOS_AEvalues, 33);
        if(sc)
        {
            mvLog(MVLOG_WARN, "Timed out auto-exposure !");
        }
    }

    if (LeonIPCReadMessage(&lrt_ipcLRTtoLOS_AEvalues, (u32*)&received) == IPC_SUCCESS)
    {
        for (int camId = 0; camId < CAMCOUNT; camId++)   // Set {exposure & gain} for each camera separately
        {
            camExpGainPair.exposureTimeUs = received.integrationTimeUs;
            camExpGainPair.gainMultiplier = received.gainCode[camId];

            updateTotalExposureHistory(camId, camExpGainPair.exposureTimeUs, camExpGainPair.gainMultiplier);
        }
    }

    sc = LeonIPCRxReleaseSinkThread(&lrt_ipcLRTtoLOS_AEvalues);
    if(sc)
        mvLog(MVLOG_ERROR, "IPC release error %d", sc);

    return received;
}

int simpleAEprocessFrame(const int id, const FramePumpBuffer *frameIn)
{
    int sc;
    convertMessage_t message;

    message.type = AE_MSG_PROCESS_FRAME;
    message.fpbIn = *frameIn;
    message.curTotalExp   = TE_N[id];
    message.statsTotalExp = TE_N_Minus_1[id];

    sc = LeonIPCSendMessage(&lrt_ipcLOStoLRT_ae[id], (u32*)&message);
    if(sc != IPC_SUCCESS)
    {
        ipcErrorCounter++;
        mvLog(MVLOG_WARN, "%s LOS IPC notify unable to send message ! %d ", __func__, sc);
    }
    return sc;
}
