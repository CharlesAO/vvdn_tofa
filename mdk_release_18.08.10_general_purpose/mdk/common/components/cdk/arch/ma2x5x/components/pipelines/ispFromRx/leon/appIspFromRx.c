#include <string.h>
#include <assert.h>
#include "ipipe.h"
#include "IpipeServerApi.h"
#include "ipipeOpipeUtils.h"
#include "FrameMgrUtils.h"
#include "FrameMgrApi.h"
#include "PlgSrcIspApi.h"
#include "appIspFromRx.h"

#define MAX_NR_OF_CAMS             1
#define NR_OF_BUFFERS_PER_SOURCE   2

//in cdmaDescriptor section that bypasses caches !!!
static PlgSrcIsp   plgSrcIsp SECTION(".cmx.cdmaDescriptors") ALIGNED(8);
static FramePool   frameMgrPoolC;
static FrameT     *frameMgrFrameC;

static uint32_t        startSrcState;
static uint32_t        stopSrcState;
static icSourceConfig  *startSrcLocConfig;
static uint32_t        tearDownEnable;
static void            *nextCfg;
static FrameProducedCB cbOutputList;

static uint32_t getSourcePluginId(void *plg) {UNUSED(plg); return 0;}
static void     startSourcesLocal(icSourceConfig  *sourceConfig, uint32_t sourceId);
static void     appRxIspStartSrc(uint32_t sourceInstance, icSourceConfig  *sourceConfig);
static void     appRxIspStopSrc (uint32_t sourceInstance) ;
static void     cbEofSourceEvent(void *plg, FrameT *frame);
static void     cbSofSourceEvent(void *plg, FrameT *frame);
static void     cbConfigIsp    (uint32_t ispInstance, void *iconf);
static void     cbOutput(FrameT *frame, void *pluginObj);

//#################################################################################################
void appRxIspCbIcSetup(icCtrl *ctrl)
{
    OpipeReset(); //general inits

    memset((void*)startSrcState,     0, sizeof(startSrcState));
    memset((void*)stopSrcState,      0, sizeof(stopSrcState));
    memset((void*)startSrcLocConfig, 0, sizeof(startSrcLocConfig));
    nextCfg                   = NULL;
    tearDownEnable            = 0;
    gServerInfo.cbDataWasSent = NULL;

    {// create plug-ins
        uint32_t i = 0;
        gServerInfo.sourceServerCtrl[i].cbStartSource  = NULL;
        gServerInfo.sourceServerCtrl[i].cbStopSource   = NULL;
        gServerInfo.pluginServerCtrl[i].cbConfigPlugin = NULL;

        PlgSrcIspCreate ((void*)&plgSrcIsp, i);
        plgSrcIsp.plg.init(&frameMgrPoolC, 1, (void*)&plgSrcIsp);

        // source that have associated trigger capture capability
        gServerInfo.sourceServerCtrl[i].pool = plgSrcIsp.outputPools;
        frameMgrFrameC = ipServerFrameMgrCreateList(NR_OF_BUFFERS_PER_SOURCE);

        // init callback output, this pluginObj not refer this time to output plugin, refere to source
        // plugin, in order to identify the source
        cbOutputList.callback  = cbOutput;
        cbOutputList.pluginObj = &plgSrcIsp;

       // SOURCE->OUTPUT link
        FrameMgrCreatePool(&frameMgrPoolC, frameMgrFrameC, &cbOutputList, 1);

       // create descriptions for available functionality regarding isp. Los isp side, base on this
       // informations, will properly update parameters and config sensors.
       ipServerRegSourceQuery(i,
               "Source",
               IC_SOURCE_ATTR_HAS_VIDEO_ISP |
               IC_SOURCE_ATTR_HAS_VIDEO_OUT,
               NR_OF_BUFFERS_PER_SOURCE,
               i, i + MAX_NR_OF_CAMS*1, i + MAX_NR_OF_CAMS*2, 0);

       ipServerRegIspQuery   (i , "IspVdo", IC_ISP_ATTR_VIDEO_LINK, i);
       ipServerRegOutputQuery(i , "Out",    IC_OUTPUT_FRAME_DATA_TYPE_PREVIEW  , i); // preview cam output

       ipServerQueryAddChild(ctrl->icPipelineDescription.icQuerySource[i], ctrl->icPipelineDescription.icQueryIsp[i]);
       ipServerQueryAddChild(ctrl->icPipelineDescription.icQueryIsp   [i], ctrl->icPipelineDescription.icQueryOutput[i]);
    };
}

void appRxIspCbIcTearDown(void) {
    //turnOfappRx();
    tearDownEnable = 1;
}

int sippOpipeResurcesFree(void) {
   return 1;
}

//#################################################################################################
void appRxIspMain()
{
    if(tearDownEnable) {
       ipServerWasTornDown();
       exit(0);
    }

    //trigger just if opipe is idle
    if (sippOpipeResurcesFree()) {
        //for (x = 0; x < MAX_NR_OF_CAMS; x++)
        uint32_t x = 0;
        {
            // Start Source Command in order to avoid big interrupt time
            if(1 == startSrcState)
            {
                startSourcesLocal(startSrcLocConfig, x);
                startSrcState = 0;
            }

            if(1 == stopSrcState)
            {
                plgSrcIsp.plg.fini(&plgSrcIsp);

                //Blocking wait for ISP to finish
                while (PLG_STATS_RUNNING == plgSrcIsp.plg.status){
                    NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
                    NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
                }
                stopSrcState = 0; //clear flag
                ipServerSourceStopped(0); //tell LOS we're stopped
            }
        }
    }
}

//#################################################################################################
icStatusCode appRxIspSrcComit(icCtrl *ctrl) {
    int32_t x;

  //Before this CB is called, full-DDR SOURCE buffers are allocated !!!

  //Ajust UV plane pointers within these buffs
    FrameT *cur = &frameMgrFrameC[0];
    do{
     /*DBG*/ //printf("fbPtr = 0x%lx\n", (uint32_t)cur->fbPtr[0]);
      cur->fbPtr[1] = cur->fbPtr[0] + 4192*3120; //TBD: do this properly
      cur = cur->next;
    }while(cur);

    /*ALLOC*/ AllocOpipeReset(); //clear prev alloc
    /*ALLOC*/ PlgSrcIspCmxAlloc(&plgSrcIsp, &ctrl->source[0].sourceSetup, CheckAllocOmemPool());
    /*DBG*/ //printf("Free = %ld\n", MemMgrGetFreeMem(CheckAllocOmemPool()));

    x = 0;
    if(IPIPE_SRC_SETUP == ctrl->source[x].sourceStatus) {
        gServerInfo.sourceServerCtrl[x].cbStartSource  = appRxIspStartSrc;
        gServerInfo.sourceServerCtrl[x].cbStopSource   = appRxIspStopSrc;
        gServerInfo.pluginServerCtrl[x].cbConfigPlugin = cbConfigIsp;
    }

    gServerInfo.cbDataWasSent = FrameMgrReleaseFrame;
    return IC_STATS_SUCCESS;
}

//#################################################################################################
// Local Functions Implementation

// Output callback linked to plug-ins
static void cbOutput(FrameT *frame, void *pluginObj) {
    UNUSED(pluginObj);
    //uint32_t ispInstance = getIspPluginId(pluginObj);
    ipServerSendData(frame, 0);
}

static void cbSofSourceEvent(void *plg, FrameT *frame) {
    uint32_t idx = getSourcePluginId(plg);
    if(frame) {
        if(nextCfg) {
            frame->appSpecificData = nextCfg;
            nextCfg = NULL;
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
}

static void cbEofSourceEvent(void *plg, FrameT *frame) {
    uint32_t idx = getSourcePluginId(plg);
    if(frame) {
        if(frame->appSpecificData) {
            ipServerReadoutEnd((icSourceInstance)idx,
                    ((icIspConfig*)(frame->appSpecificData))->userData,
                    frame->seqNo, frame->timestamp[0]);
        }
    }
    else{ //send READOUT_END to LOS even when frames get skipped,
          //as that triggers test code to send new configs
        ipServerReadoutEnd((icSourceInstance)idx, 0, 0, 0);
    }
}

static void cbConfigIsp(uint32_t ispInstance, void *iconf) {
    UNUSED(ispInstance);
    nextCfg             = iconf;
    plgSrcIsp.nxtIspCfg = nextCfg;

   //===============================================================
   //Patches that are NOT required for metal-fixed ma2150 silicon:
    extern uint16_t cmxGammaLut[512*4];
    plgSrcIsp.nxtIspCfg->gamma.table  = cmxGammaLut; //for bug 22777
    plgSrcIsp.nxtIspCfg->dog.strength = 0; //LTM-Only for better performance on ma2150
   //===============================================================
}

static void startSourcesLocal(icSourceConfig  *sourceConfig, uint32_t sourceId) {
    icSize   iSize;
    iSize.w  = sourceConfig->cropWindow.x2 - sourceConfig->cropWindow.x1;
    iSize.h  = sourceConfig->cropWindow.y2 - sourceConfig->cropWindow.y1;

  //new SrcIsp outputs YUV420 !
    FrmMgrUtilsInitList(frameMgrFrameC, iSize, FRAME_T_FORMAT_YUV420);

    plgSrcIsp.eofEvent = cbEofSourceEvent;
    plgSrcIsp.sofEvent = cbSofSourceEvent;
    PlgSrcIspStart(&plgSrcIsp, sourceConfig);
    ipServerSourceReady(sourceId);
}

static void appRxIspStartSrc(uint32_t sourceInstance, icSourceConfig  *sourceConfig) {
    UNUSED(sourceInstance);
    startSrcState     = 1;
    startSrcLocConfig = sourceConfig;
}
static void appRxIspStopSrc(uint32_t sourceInstance) {
    UNUSED(sourceInstance);
    stopSrcState = 1;
}
