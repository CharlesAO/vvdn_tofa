/**************************************************************************************************

 @File         : app3CamIsp.c
 @Author       : MT
 @Brief        : Contains 3 Isp project main control functionality
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015

 Description :


 **************************************************************************************************/


/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "DrvLeonL2C.h"
#include "ipipe.h"
#include "ipipeDbg.h"
#include "FrameMgrUtils.h"
#include "ipipeUtils.h"
#include "ipipeOpipeUtils.h"
#include "FrameMgrApi.h"
#include "IpipeServerApi.h"
#include "ipipeMsgQueue.h"
#include "PlgSourceApi.h"
#include "PlgFifoApi.h"
#include "PlgIspFullApi.h"
#include "app3CamIsp.h"


/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/

#ifndef APP_CONFIGURATION
#warning 3CAM_ISP: USE DEAULT DEFINITIONS
#define MAX_NR_OF_CAMS              3
#define NR_OF_BUFFERS_PER_SOURCE    3
#define NR_OF_BUFFERS_PER_ISP_OUT   3
#define NR_OF_BUFFERS_PER_STILL_OUT 1

#else
//#warning 3CAM ISP: USE APPLICATION DEFINITIONS
#endif

/**************************************************************************************************
 ~~~  Local variables
 **************************************************************************************************/
static PlgSource      plgSource[3] SECTION(".cmx.cdmaDescriptors") ALIGNED(8);
static PlgIspFull     plgVdo1  [3] SECTION(".cmx.cdmaDescriptors") ALIGNED(8);
static PlgIspFull     plgStill [3] SECTION(".cmx.cdmaDescriptors") ALIGNED(8);
static PlgFifo        plgFifo;

static FramePool frameMgrPoolC    [MAX_NR_OF_CAMS];
static FramePool frameMgrPoolFifo [MAX_NR_OF_CAMS];
static FramePool frameMgrPoolVideo[MAX_NR_OF_CAMS];
static FramePool frameMgrPoolStill[MAX_NR_OF_CAMS];

static FrameT *frameMgrFrameC    [MAX_NR_OF_CAMS];
static FrameT *frameMgrFrameVideo[MAX_NR_OF_CAMS];
static FrameT *frameMgrFrameStill[MAX_NR_OF_CAMS];

static uint32_t        startSrcState[MAX_NR_OF_CAMS];
static uint32_t        stopSrcState [MAX_NR_OF_CAMS];
static icSourceConfig  *startSrcLocConfig[MAX_NR_OF_CAMS];
static uint32_t        tearDownEnable;
static void            *nextCfg [MAX_NR_OF_CAMS];
static uint32_t        previewAbleMode;
static FrameProducedCB cbOutputList[3*2];

/**************************************************************************************************
 ~~~ Local File function declarations
 **************************************************************************************************/
static void     turnOfapp3Cam(void);
static uint32_t checkTurnOfFinalStop(uint32_t *updateVal);
static void     startSourcesLocal(icSourceConfig  *sourceConfig, uint32_t sourceId);
static uint32_t getSourcePluginId(void *plg);
static uint32_t getIspPluginId(void *plg);
static void     app3CamIspStartSrc(uint32_t sourceInstance, icSourceConfig  *sourceConfig);
static void     app3CamIspStopSrc (uint32_t sourceInstance) ;
static void     cbEofSourceEvent(void *plg, FrameT *frame);
static void     cbSofSourceEvent(void *plg, FrameT *frame);
static void     cbConfigIsp    (uint32_t ispInstance, void *iconf);
static void     cbStartIspEvent(void *plg, uint32_t seqNr, void *userData);
static void     cbEndIspEvent  (void *plg, uint32_t seqNr, void *userData);
static void     cbOutput(FrameT *frame, void *pluginObj);

static void     cbErrorIspEvent(void* plg, icSeverity severity, icError errorNo, void *userData);
/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/

void app3CamIspCbIcSetup(icCtrl *ctrl) {
    uint32_t i;
    OpipeReset(); //general inits
    memset((void*)startSrcState, 0, sizeof(startSrcState));
    memset((void*)stopSrcState, 0, sizeof(stopSrcState));
    memset((void*)startSrcLocConfig, 0, sizeof(startSrcLocConfig));
    memset((void*)nextCfg, 0, sizeof(nextCfg));
    tearDownEnable = 0;
    previewAbleMode = 0;
    gServerInfo.cbDataWasSent   = NULL;
    // create plug-ins
    PlgFifoCreate   ((void*)&plgFifo);
    PlgFifoConfig   ((void*)&plgFifo, (uint32_t)MAX_NR_OF_CAMS);
    plgFifo.plg.init(frameMgrPoolFifo, 3, (void*)&plgFifo);
    for(i = 0; i < MAX_NR_OF_CAMS; i++) {
        gServerInfo.sourceServerCtrl[i].cbStartSource  = NULL;
        gServerInfo.sourceServerCtrl[i].cbStopSource   = NULL;
        gServerInfo.pluginServerCtrl[i].cbConfigPlugin = NULL;
        PlgSourceCreate ((void*)&plgSource[i], i);
        PlgIspFullCreate((void*)&plgVdo1[i]);
        PlgIspFullCreate((void*)&plgStill[i]);
        plgSource[0].plg.init(&frameMgrPoolC[i],     1, (void*)&plgSource[i]);
        plgVdo1[0].plg.init  (&frameMgrPoolVideo[i], 1, (void*)&plgVdo1[i]);
        plgStill[0].plg.init (&frameMgrPoolStill[i], 1, (void*)&plgStill[i]);
        // source that have associated trigger capture capability
        gServerInfo.sourceServerCtrl[i].pool = plgSource[i].outputPools;
        frameMgrFrameC[i]       = ipServerFrameMgrCreateList(NR_OF_BUFFERS_PER_SOURCE);
        frameMgrFrameVideo[i]   = ipServerFrameMgrCreateList(NR_OF_BUFFERS_PER_ISP_OUT);
        frameMgrFrameStill[i]   = ipServerFrameMgrCreateList(NR_OF_BUFFERS_PER_STILL_OUT);
        // init callback output, this pluginObj not refer this time to output plugin, refere to source
        // plugin, in order to identify the source
        cbOutputList[i  ].callback  = cbOutput;
        cbOutputList[i  ].pluginObj = &plgVdo1[i];
        cbOutputList[3+i].callback  = cbOutput;
        cbOutputList[3+i].pluginObj = &plgStill[i];
        // Source output pool
        FrameMgrCreatePool(&frameMgrPoolC[i], frameMgrFrameC[i], &plgFifo.plg.callbacks[i], 1);
        // Mux  output pool, special case for serialization plug-in, no frame inside, as it pass the input data out
        FrameMgrCreatePool(&frameMgrPoolFifo[i],  NULL,                  plgVdo1[i].plg.callbacks, 1);
        FrameMgrCreatePool(&frameMgrPoolVideo[i], frameMgrFrameVideo[i], &cbOutputList[i],         1);
        FrameMgrCreatePool(&frameMgrPoolStill[i], frameMgrFrameStill[i], &cbOutputList[3+i],       1);
        // create descriptions for available functionality regarding isp. Los isp side, base on this
        // informations, will properly update parameters and config sensors.
        ipServerRegSourceQuery(i,
                "Source",
                IC_SOURCE_ATTR_HAS_VIDEO_ISP | IC_SOURCE_ATTR_HAS_STILL_ISP |
                IC_SOURCE_ATTR_HAS_VIDEO_OUT |
                IC_SOURCE_ATTR_HAS_STILL_OUT |
                IC_SOURCE_ATTR_HAS_RAW_OUT   ,
                NR_OF_BUFFERS_PER_SOURCE,
                i, i + MAX_NR_OF_CAMS*1, i + MAX_NR_OF_CAMS*2, 0);

        ipServerRegIspQuery(i                       , "IspVdo",     IC_ISP_ATTR_VIDEO_LINK, i);
        ipServerRegIspQuery(i+MAX_NR_OF_CAMS        , "IspStill",   IC_ISP_ATTR_SILL_LINK , i);
        ipServerRegOutputQuery(i                    , "Out",        IC_OUTPUT_FRAME_DATA_TYPE_PREVIEW  , i); // preview cam output
        ipServerRegOutputQuery(i + MAX_NR_OF_CAMS*1 , "Out",        IC_OUTPUT_FRAME_DATA_TYPE_STILL    , i); // still cam output
        ipServerRegOutputQuery(i + MAX_NR_OF_CAMS*2 , "Out",        IC_OUTPUT_FRAME_DATA_TYPE_STILL_RAW, i); // raw cam output

        ipServerQueryAddChild(ctrl->icPipelineDescription.icQuerySource[i], ctrl->icPipelineDescription.icQueryIsp[i]);
        ipServerQueryAddChild(ctrl->icPipelineDescription.icQuerySource[i], ctrl->icPipelineDescription.icQueryIsp[i+MAX_NR_OF_CAMS]);
        ipServerQueryAddChild(ctrl->icPipelineDescription.icQuerySource[i], ctrl->icPipelineDescription.icQueryOutput[i+2*MAX_NR_OF_CAMS]);
        ipServerQueryAddChild(ctrl->icPipelineDescription.icQueryIsp[i], ctrl->icPipelineDescription.icQueryOutput[i]);
        ipServerQueryAddChild(ctrl->icPipelineDescription.icQueryIsp[i + MAX_NR_OF_CAMS*1], ctrl->icPipelineDescription.icQueryOutput[i + MAX_NR_OF_CAMS*1]);
    };
}

//
void app3CamIspCbIcTearDown(void) {
    turnOfapp3Cam();
    tearDownEnable = 1;
}

int sippOpipeResurcesFree(void) {
    uint32_t ret = 0;
    uint32_t x;
    for (x = 0; x < MAX_NR_OF_CAMS; x++) {
        ret = ret | plgVdo1[x].plg.status;
        ret = ret | plgStill[x].plg.status;
    }
    return(0 == ret);
}
//
void app3CamIspMain(void) {
    uint32_t x;
    if(tearDownEnable) {
        if(checkTurnOfFinalStop(&tearDownEnable)) {
            ipServerWasTornDown();
            exit(0);
        }
    }

    // trigger just if opipe is idle
    if (sippOpipeResurcesFree()) {
        for (x = 0; x < MAX_NR_OF_CAMS; x++) {
            // Start Source Command in order to avoid big interrupt time
            if (1 == startSrcState[x]) {
                startSourcesLocal(startSrcLocConfig[x], x);
                startSrcState[x] = 0;
            }
            // Stop Source Command in order to avoid big interrupt time
            if (1 == stopSrcState[x]) {
                if(plgSource[x].plg.fini)
                    plgSource[x].plg.fini(&plgSource[x]);
                if(plgVdo1[x].plg.fini)
                    plgVdo1[x].plg.fini(&plgVdo1[x]);
                while ( (PLG_STATS_RUNNING == plgSource[x].plg.status) ||
                        (PLG_STATS_RUNNING == plgVdo1[x].plg.status)) {
                    NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
                    NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
                }
                stopSrcState[x] = 0;
                ipServerSourceStopped(x);
            }
        }
        // check capture command queue
        TriggerCaptElement *captureDescriptor;
        if(0 == ipServerQueueGet(&captureDescriptor)) {
            // call still isp associated with this source
            plgStill[captureDescriptor->source].plg.trigger(captureDescriptor->buffer,
                    captureDescriptor->config, NULL, &plgStill[captureDescriptor->source]);
        }
        else {
            plgFifo.triger((void*)&plgFifo);
        }
        NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
    }
}

//
icStatusCode app3CamIspSrcComit(icCtrl *ctrl) {
    int32_t x;

    /*ALLOC*/ AllocOpipeReset(); //clear prev alloc
    /*ALLOC*/ AllocOpipeRxCmxBuffs (ctrl);
    /*ALLOC*/
    /*ALLOC*/ PlgIspBase *ispBase[MAX_NR_OF_CAMS]; //Isp buffs
    /*ALLOC*/ for(x=0; x<MAX_NR_OF_CAMS; x++)
    /*ALLOC*/   ispBase[x] = &plgVdo1[x].base;
    /*ALLOC*/ AllocOpipeIspCmxBuffs(ctrl, MAX_NR_OF_CAMS, ispBase);

    // Allocate frames buffers memory
    for (x = 0; x < MAX_NR_OF_CAMS; x++) {
        if(IPIPE_SRC_SETUP == ctrl->source[x].sourceStatus) {
            //size.w = (inSz.w * hN - 1)/hD + 1;
            //size.h = (inSz.h * vN - 1)/vD + 1;
            uint32_t maxIspW = ((ctrl->source[x].sourceSetup.maxWidth *
                    ctrl->source[x].sourceSetup.maxHorizN - 1) /
                    ctrl->source[x].sourceSetup.maxHorizD + 1);
            uint32_t maxIspH = ((ctrl->source[x].sourceSetup.maxHeight *
                    ctrl->source[x].sourceSetup.maxVertN - 1) /
                    ctrl->source[x].sourceSetup.maxVertD + 1);
            ipServerFrameMgrAddBuffs(frameMgrFrameStill[x],
                    maxIspW * maxIspH,
                    maxIspW * maxIspH >>1,
                    0);
            // Allocate smaller ddr as video output for cam0 is quarter from the input resolution
            uint32_t videoFrameSize = (maxIspW * maxIspH);
            if(QUARTER_MODE_FOR_VIDEO_PIPE == ctrl->source[x].sourceSetup.appSpecificInfo) {
                videoFrameSize = videoFrameSize>>2;
                previewAbleMode = 1;
            }
            ipServerFrameMgrAddBuffs(frameMgrFrameVideo[x],videoFrameSize,videoFrameSize>>1, 0);
            // this functionality is available just now !!! Important to do that
            gServerInfo.sourceServerCtrl[x].cbStartSource = app3CamIspStartSrc;
            gServerInfo.sourceServerCtrl[x].cbStopSource = app3CamIspStopSrc;
            gServerInfo.pluginServerCtrl[x].cbConfigPlugin = cbConfigIsp;
        }
    }

    gServerInfo.cbDataWasSent   = FrameMgrReleaseFrame; // !!! different approach here


    return IC_STATS_SUCCESS;
}




/**************************************************************************************************
 ~~~ Local Functions Implementation
 **************************************************************************************************/

// Output callback linked to plug-ins
static void cbOutput(FrameT *frame, void *pluginObj) {
    uint32_t ispInstance = getIspPluginId(pluginObj);

    if (IC_PIPECTL_MIPI_ENABLE & (((icIspConfig *)(frame->appSpecificData))->pipeControl)) {
        ipServerSendData(frame, ispInstance);
    } else {
        FrameMgrReleaseFrame(frame);
    }
}

static void cbSofSourceEvent(void *plg, FrameT *frame) {
    uint32_t idx = getSourcePluginId(plg);
    if(frame) {
        if(nextCfg[idx]) {
            frame->appSpecificData = nextCfg[idx];
            nextCfg[idx] = NULL;
            icTimestamp TimeSyncMsgGetTimeUs(void);
            frame->timestamp[0] = TimeSyncMsgGetTimeUs();
            ipServerReadoutStart((icSourceInstance)idx,
                    ((icIspConfig*)(frame->appSpecificData))->userData,
                    frame->seqNo, frame->timestamp[0]);
        }
        else {
            frame->appSpecificData = NULL;
            //ipServerReadoutStart((icSourceInstance)idx,  NULL, frame->seqNo, frame->timestamp[0]);
        }
    }
    else {
        //ipServerReadoutStart((icSourceInstance)idx, NULL, 0, 0);
    }
}
static void cbEofSourceEvent(void *plg, FrameT *frame) {
    uint32_t idx = getSourcePluginId(plg);
    if(frame) {
        if(frame->appSpecificData) {
            if (IC_PIPECTL_ZSL_LOCK & (((icIspConfig *)(frame->appSpecificData))->pipeControl)) {
                //            if (IC_PIPECTL_ZSL_LOCK &
                //                    (swcLeonReadNoCacheU32((uint32_t)&(((icIspConfig *)nextCfg[idx])->pipeControl)))) {
                FrameMgrIncreaseNrOfConsumer(frame, 1);
                ipServerFrameLocked((icSourceInstance)idx, frame);
            }
            ipServerReadoutEnd((icSourceInstance)idx,
                    ((icIspConfig*)(frame->appSpecificData))->userData,
                    frame->seqNo, frame->timestamp[0]);
        }
        else {
            //ipServerReadoutEnd((icSourceInstance)idx, 0, frame->seqNo, frame->timestamp[0]);
        }
    }
    else {
        //ipServerReadoutEnd((icSourceInstance)idx, NULL, 0, 0);
    }
}

static void cbStartIspEvent(void *plg, uint32_t seqNr, void *userData) {
    uint32_t ispInstance = getIspPluginId(plg);
    ipServerIspStart(ispInstance, seqNr, userData);
}
static void cbEndIspEvent(void *plg, uint32_t seqNr, void *userData) {
    uint32_t ispInstance = getIspPluginId(plg);
    ipServerIspEnd(ispInstance, seqNr, userData);
}

// Error reporting callbacks
void cbErrorIspEvent(void *plg, icSeverity severity, icError errorNo, void *userData) {
    uint32_t ispInstance = getIspPluginId(plg);
    ipServerIspReportError(ispInstance, severity, errorNo, userData);
}

static void cbConfigIsp(uint32_t ispInstance, void *iconf) {
    nextCfg[ispInstance] = iconf;
}

static void startSourcesLocal(icSourceConfig  *sourceConfig, uint32_t sourceId) {
    icSize       iSize;
    icSize       oVdoSize;
    iSize.w  = sourceConfig->cropWindow.x2 - sourceConfig->cropWindow.x1;
    iSize.h  = sourceConfig->cropWindow.y2 - sourceConfig->cropWindow.y1;
    oVdoSize = iSize;
    if(QUARTER_MODE_FOR_VIDEO_PIPE == pSrvIcCtrl->source[sourceId].sourceSetup.appSpecificInfo) {
        oVdoSize.w = iSize.w>>1;
        oVdoSize.h = iSize.h>>1;
    }
    FrmMgrUtilsInitList(frameMgrFrameC[sourceId], iSize,
            FrmMgrUtilsGetRawFrm(sourceConfig->bitsPerPixel, (sourceConfig->mipiRxData.recNrl <= IC_SIPP_DEVICE3 ? 1 : 0)));

    FrmMgrUtilsInitList(frameMgrFrameStill[sourceId], iSize, FRAME_T_FORMAT_YUV420);
    FrmMgrUtilsInitList(frameMgrFrameVideo[sourceId], oVdoSize, FRAME_T_FORMAT_YUV420);

    uint32_t sourceFmt = SIPP_FMT_16BIT; //CIF default
    if(sourceConfig->mipiRxData.recNrl <= IC_SIPP_DEVICE3)
        sourceFmt = GetFrameBppPackFormat(sourceConfig->bitsPerPixel);
    PlgIspFullConfig(&plgVdo1 [sourceId], iSize, sourceFmt, previewAbleMode);
    PlgIspFullConfig(&plgStill[sourceId], iSize, sourceFmt, previewAbleMode);

    //Pipes to clear used resources at EOF
    plgVdo1 [sourceId].op.p.flags |= CLEAN_EXIT;
    plgStill[sourceId].op.p.flags |= CLEAN_EXIT;

    if(QUARTER_MODE_FOR_VIDEO_PIPE == pSrvIcCtrl->source[sourceId].sourceSetup.appSpecificInfo)
        plgVdo1[sourceId].op.p.flags |=  GEN_PREVIEW;
    plgSource[sourceId].eofEvent        = cbEofSourceEvent;
    plgSource[sourceId].sofEvent        = cbSofSourceEvent;
    plgVdo1[sourceId].procesStart       = cbStartIspEvent;
    plgVdo1[sourceId].procesEnd         = cbEndIspEvent;
    plgVdo1[sourceId].procesIspError    = cbErrorIspEvent;
    plgStill[sourceId].procesStart      = cbStartIspEvent;
    plgStill[sourceId].procesEnd        = cbEndIspEvent;
    plgStill[sourceId].procesIspError   = cbErrorIspEvent;
    PlgSourceStart(&plgSource[sourceId], sourceConfig, sourceFmt);
    ipServerSourceReady(sourceId);
}

static void app3CamIspStartSrc(uint32_t sourceInstance, icSourceConfig  *sourceConfig) {
    startSrcState[sourceInstance] = 1;
    startSrcLocConfig[sourceInstance] = sourceConfig;
}
static void app3CamIspStopSrc(uint32_t sourceInstance) {
    stopSrcState[sourceInstance] = 1;
}

static void turnOfapp3Cam(void) {
    uint32_t i;
    for(i = 0; i < MAX_NR_OF_CAMS; i++) {
        if(plgVdo1[i].plg.fini)   plgVdo1[i].plg.fini(&plgVdo1[i]);
        if(plgSource[i].plg.fini) plgSource[i].plg.fini(&plgSource[i]);
        gServerInfo.sourceServerCtrl[i].cbStartSource  = NULL;
        gServerInfo.sourceServerCtrl[i].cbStopSource   = NULL;
        gServerInfo.pluginServerCtrl[i].cbConfigPlugin = NULL;
    }
    if(plgFifo.plg.fini) plgFifo.plg.fini(&plgFifo);
    gServerInfo.cbDataWasSent   = NULL;
}
static uint32_t checkTurnOfFinalStop(uint32_t *updateVal) {
    if(     (0 == plgSource[0].plg.status) &&
            (0 == plgSource[1].plg.status) &&
            (0 == plgSource[2].plg.status) &&
            (0 == plgVdo1[0].plg.status)   &&
            (0 == plgVdo1[1].plg.status)   &&
            (0 == plgVdo1[2].plg.status)   &&
            (0 == plgFifo.plg.status)         ) {
        MemMgrReset();
        *updateVal = 0;
        return 1;
    }
    return 0;
}

static uint32_t getSourcePluginId(void *plg) {
    uint32_t i;
    for(i = 0; i < MAX_NR_OF_CAMS; i++)
        if(plg == &plgSource[i]) return i;
    assert(0); return 0;
}

static uint32_t getIspPluginId(void *plg) {
    uint32_t i;
    for(i = 0; i < MAX_NR_OF_CAMS; i++)
        if(plg == &plgVdo1[i]) return i;
    for(i = MAX_NR_OF_CAMS; i < MAX_NR_OF_CAMS*2; i++)
        if(plg == &plgStill[i-MAX_NR_OF_CAMS]) return i;
    assert(0); return 0;
}


