///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <OsDrvCpr.h>
#if defined(MA2150) || defined(MA2450)
#include <OsDrvShaveL2Cache.h>
#else
#include <OsDrvInit.h>
#endif

#include "OsDrvSvu.h"

#include <brdRtems.h>

#include "flicPlgVtrack.h"
#include "plgVisionStereo.h"
#include "DataCollector.h"
#include "swcFrameTypes.h"
#include "XLink.h"
#include <DrvLeon.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// -----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// -----------------------------------------------------------------------------
// 4: Static Local Data
// -----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

XLinkGlobalHandler_t handler;

typedef enum{
    VTRACK_ALGO= 0xAA,
    STEREO_ALGO= 0x55,
}algoType;

typedef enum{
    RES_720P= 0x0,
    RES_VGA
}algoResolution;
typedef enum moviEvalCommand_t{
    START,
    STOP,
    ADD_ALGO,
    BPP,
    RES,
}moviEvalCommand_t;
typedef struct moviEvalConfig{
    moviEvalCommand_t cmd;
    algoType type;
    int value;
}moviEvalConfig;

extern dataCollectorContext lrt_dcc;
extern float lrt_calib_array[9];



visionBasePlg* createVtrackPlg()
{
    assert(0 && "Vtrack not supported on LOS for now\n");
    return NULL;
}


visionBasePlg* createStereoPlg(float* ca){
    UNUSED(ca);
    assert(0 && "Stereo not supported on LOS for now\n");
    return NULL;
}

//This plugin is just adding the plugin info, not the actual plugin. The actual pluggin will be created later either on LOS either on LRT.
void addPluginToDcc(swcProcessorType where, dataCollectorAlgoType type){
    lrt_dcc.processingPlugin[lrt_dcc.processingCount][0] = NULL;
    lrt_dcc.type[lrt_dcc.processingCount] = type;
    lrt_dcc.processingthreadCnt[lrt_dcc.processingCount]++;
    lrt_dcc.where[lrt_dcc.processingCount] = where;
    lrt_dcc.processingCount++;
}

int startLLRT(void)
{
    DrvLeonRTStartupStart();
    DrvLeonRTWaitForBoot();
    return 0;
}
extern "C" void *POSIX_Init(void *args)
{
    UNUSED(args);
    rtems_status_code sc;
    int bpp = 1;
    algoResolution resolution = RES_VGA;
    sc = brdInitializeBoard();
    if (sc != RTEMS_SUCCESSFUL) {
        printf("Failure to initialize board driver !\n");
        return NULL;
    }

    #if defined(MA2150) || defined(MA2450)
    #else
    // ***Configure HW resources***
        OsDrvCprDeviceConfig devConfig = {
            OS_DRV_CPR_DEV_MSS_CV_HARRIS,
            OS_DRV_CPR_DEV_ENABLE
        };
        assert(OsDrvCprSysDeviceAction(&devConfig) == 0);
        devConfig.device = OS_DRV_CPR_DEV_MSS_CV_MINMAX;
        assert(OsDrvCprSysDeviceAction(&devConfig) == 0);
        devConfig.device = OS_DRV_CPR_DEV_MSS_CV_STEREO;
        assert(OsDrvCprSysDeviceAction(&devConfig) == 0);
        devConfig.device = OS_DRV_CPR_DEV_MSS_CV_WARP0;
        assert(OsDrvCprSysDeviceAction(&devConfig) == 0);
        devConfig.device = OS_DRV_CPR_DEV_MSS_CV_WARP1;
        assert(OsDrvCprSysDeviceAction(&devConfig) == 0);
    #endif

    memset(&lrt_dcc, 0, sizeof(lrt_dcc));
    lrt_dcc.camName[0] = (char*)"/dev/video_tracking_left";
    lrt_dcc.camName[1] =  (char*)"/dev/video_tracking_right";
    lrt_dcc.camCnt = 2;
    lrt_dcc.camFrameRate = 30;
    lrt_dcc.latencyControl = 1;

    XLinkInitialize(&handler);
    streamId_t configChannel;
    while((configChannel = XLinkOpenStream(0, "config", 1024)) == INVALID_STREAM_ID);
    streamPacketDesc_t *packet;
    int config = 1;
    while(config){
        XLinkReadData(configChannel, &packet);
        if(packet->length != sizeof(moviEvalConfig)){
            assert(0 && "Broken protocol\n");
        }
        moviEvalConfig* cfg = (moviEvalConfig*)packet->data;
        switch (cfg->cmd){
        case START:
            config = 0;
            break;
        case STOP:
            break;
        case ADD_ALGO:{
            XLinkReadData(configChannel, &packet);
            memcpy(lrt_calib_array, packet->data, 36);
            printf("----------------------- %x\n", cfg->type);
            dataCollectorAlgoType t = cfg->type == VTRACK_ALGO ? ALGO_VTRACK : ALGO_STEREO;
            addPluginToDcc(PROCESS_LEON_RT, t);
            XLinkReleaseData(configChannel);
        }
            break;
        case BPP:
            bpp = cfg->value;
            break;
        case RES:
            resolution = (algoResolution)cfg->value;
            break;
        default:
            break;
        }
        XLinkReleaseData(configChannel);

    }
    int binningEnable = 0;

    lrt_dcc.frSpec.bytesPP = bpp;
    if (resolution == RES_720P){
        lrt_dcc.frSpec.width = 1280;
        lrt_dcc.frSpec.height = 720;
    }else{
        lrt_dcc.frSpec.width = 640;
        lrt_dcc.frSpec.height = 400;
        binningEnable = 1;
    }
    lrt_dcc.frSpec.stride = lrt_dcc.frSpec.width * bpp;
    brdInitializeSensors(bpp, binningEnable);
    DataCollectorInit(&lrt_dcc);
    //host added all desired plugins. Create the ones from LOS
    int id;
    for(id = 0; id < lrt_dcc.processingCount; id++){
        if(lrt_dcc.where[id] == PROCESS_LEON_OS){
            if (lrt_dcc.type[id] == ALGO_VTRACK){
                lrt_dcc.processingPlugin[id][0] = createVtrackPlg();
            }else if(lrt_dcc.type[id] == ALGO_STEREO){
                lrt_dcc.processingPlugin[id][0] = createStereoPlg(lrt_calib_array);
            }
        }else{
        }
    }
    // When starting LRT, the LRT will read the list of plugins and will create the LRT ones
    // When LRT signals boot complete, the plugins are all created.
    // Don't touch the shared dataCollectorContext while this function is running. LRT fills the plugin pointers.
    startLLRT();

    OsDrvSvuInit();
    DataCollectorStart(&lrt_dcc);
    return NULL;
}
