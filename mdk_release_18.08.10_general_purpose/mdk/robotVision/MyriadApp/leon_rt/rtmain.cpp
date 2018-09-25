///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///


// 1: Includes
// ----------------------------------------------------------------------------

#include <DataCollector.h>
#include <stdio.h>
#include <DataCollector.h>
#include <DrvLeon.h>
#include "swcFrameTypes.h"
#include "flicPlgVtrack.h"
#if defined(MA2150) || defined(MA2450)
#include <OsDrvShaveL2Cache.h>
#include <OsDrvSvu.h>
#else
#include <OsDrvInit.h>
#include <OsDrvStereoDefines.h>

#endif
#include "plgVisionStereo.h"
#include "aeThread.h"
#include <swcShaveLoader.h>

#ifdef MONOISP
#include <brdRtems.h>
#endif

#define CMX_NOCACHE __attribute__((section(".cmx_direct.data")))
#define CMX_DATA_S3 __attribute__((section(".shv3.S.data")))
#define CMX_DATA_S4 __attribute__((section(".shv4.S.data")))
#define CMX_DATA_S5 __attribute__((section(".shv5.S.data")))
#define CMX_DATA_S6 __attribute__((section(".shv6.S.data")))
#define CMX_DATA_S7 __attribute__((section(".shv7.S.data")))
#define CMX_DATA_S8 __attribute__((section(".shv8.S.data")))
#define CMX_DATA_S9 __attribute__((section(".shv9.S.data")))

#define CMX_DATA __attribute__((section(".cmx.data")))
#define DDR_AREA __attribute__((section(".ddr.bss")))

#define SHAVENUM1(tasknum) CMX_DATA_S ## tasknum
#define SHAVENUM(tasknum) SHAVENUM1(tasknum)

#define MIN_DISTANCE_SQUARED 50
//number of shaves
#define NUM_SHAVES_GAUSS 1
#if defined(MA2150) || defined(MA2450)
#define NUM_SHAVES_CORNER 1
#else
#define NUM_SHAVES_CORNER 0
#endif
#define NUM_SHAVES_OF 2
//shave numbers
#define SHAVE_GAUSS     6
#define SHAVE_CORNER    7
#define SHAVE_PP        9
#define SHAVE_OF1       6
#define SHAVE_OF2       7
#define SHAVE_FM        8

int partitions[CFG_NUM_SHAVES];

dataCollectorContext dcc SECTION(".cmx_direct.data");
swcShaveUnit_t gaussList[1] =   {SHAVE_GAUSS};
swcShaveUnit_t cornerList[1] =  {SHAVE_CORNER};
swcShaveUnit_t ofList[2] =      {SHAVE_OF1, SHAVE_OF2};

// VTRACK
#define MAX_FEATURES 480
SHAVENUM(SHAVE_GAUSS) t_vpPpCmxBuffs vpPpCmxBuffs __attribute__((aligned(16)));
SHAVENUM(SHAVE_GAUSS) t_vpVgaCmxHeap vpCmxHeap; // VGA CMX heap is bigger then 720P



DDR_AREA t_vp720DdrHeap vpDdrHeap __attribute__((aligned(4096)));
SHAVENUM(SHAVE_CORNER) t_vpCmxInternals<MAX_FEATURES> vpBuffs;
SHAVENUM(SHAVE_FM) t_vp720PpBuffs vpPpBuffs __attribute__((aligned(16)));
SHAVENUM(SHAVE_GAUSS) t_vPipeShaveConfig vPipeShaveCfg;
SHAVENUM(SHAVE_GAUSS) t_vPipeResourceCfg vpResCfg;
static SHAVENUM(SHAVE_GAUSS) t_vPipeAlgoCfg algoCfg;
SHAVENUM(SHAVE_GAUSS) t_vPipeParams params;
CMX_NOCACHE uint8_t bufVtrackPlg[sizeof(plgFlicVTrack)] __attribute__((aligned(64)));
// STEREO
CMX_NOCACHE uint8_t bufStereoPlg[sizeof(plgVisionStereo)] __attribute__((aligned(64)));
CMX_NOCACHE float calib_array[9];

extern pthread_mutex_t sippMutex;


visionBasePlg* createVtrackPlg()
{
    plgFlicVTrack* vTrack;
    vpResCfg.vp_shaveCfg = &vPipeShaveCfg;

    // MEMORY
    vpResCfg.vp_memCfg.vpCmxBuffs = vpBuffs.buf;
    vpResCfg.vp_memCfg.vpCmxHeap  = vpCmxHeap;
    vpResCfg.vp_memCfg.vpDdrHeap  = vpDdrHeap;
    vpResCfg.vp_memCfg.vpPpBuffs = vpPpBuffs;
    vpResCfg.vp_memCfg.vpPpCmxBuffs = vpPpCmxBuffs;

    // CACHE
    vpResCfg.vp_cacheCfg.pp_part_data = partitions[3];
    vpResCfg.vp_cacheCfg.of_part_data = partitions[3];
    vpResCfg.vp_cacheCfg.fm_part_data = partitions[3];
    vpResCfg.vp_cacheCfg.pp_part_instr = partitions[2];
    vpResCfg.vp_cacheCfg.of_part_instr = partitions[2];
    vpResCfg.vp_cacheCfg.fm_part_instr = partitions[2];

    // HW FIFO
    vpResCfg.vp_fifoCfg.fifo1 = 6;
    vpResCfg.vp_fifoCfg.fifo2 = 7;
    vpResCfg.vp_fifoCfg.fifo3 = 8;
    vpResCfg.vp_fifoCfg.fifo4 = 9;
    // SHAVES
    vPipeShaveCfg.gaussNoShaves   = NUM_SHAVES_GAUSS;
    vPipeShaveCfg.gaussShaveList  = gaussList;
    vPipeShaveCfg.cornerNoShaves  = NUM_SHAVES_CORNER;
    vPipeShaveCfg.cornerShaveList = cornerList;
    vPipeShaveCfg.ppShaveNum      = SHAVE_PP;
    vPipeShaveCfg.ofNoShaves      = NUM_SHAVES_OF;
    vPipeShaveCfg.ofShaveList     = ofList;
    vPipeShaveCfg.fmShaveNum      = SHAVE_FM;

    //vTrack runtime parameters init
    frameSpec frSpec;
    frSpec.bytesPP = 1;
    frSpec.height = dcc.frSpec.height;
    frSpec.width = dcc.frSpec.width;
    frSpec.type = RAW8;
    frSpec.stride = frSpec.width * frSpec.bytesPP;
    vPipeInitDefaultHarrisRuntimeParameters(&params);
    params.lostFeatureError = 50000;
    params.thresholdMin = 6000000.0;
    params.minDistanceSquared = MIN_DISTANCE_SQUARED;
    params.minHarrisResponseOldFeatures = 200000.0;

    //vTrack static algorithm configuration
    vPipeInitDefaultAlgoCfg(&algoCfg, &frSpec);

    customProcCtx ctx;
    ctx.camCnt = 1;
    ctx.frameRateDivider = 1;
    ctx.dropCondition = 0;

    vTrack = new (bufVtrackPlg) plgFlicVTrack(&vpResCfg, VPIPE_MODE_PP_FM_OF, &algoCfg, &params, ctx, true);
    vTrack->mutex = &sippMutex;
    vTrack->Create();
    return vTrack;
}

#if !defined(MA2150) && !defined(MA2450)
void InitCfgResStereo(stereo_cfg &cfg, stereo_resources &res)
{

    rtems_status_code sc;
    cfg.wsgbmEn = true;
    cfg.disp2depthEn = false;
    cfg.flipByDisp2depth = false;
    cfg.flipByWarp = true;

    // set stereo calibration parameters
    cfg.calib.baseline = 0.035;
    cfg.calib.fov = 69.0;
    cfg.calib.width = dcc.frSpec.width;
    cfg.calib.height = dcc.frSpec.height;
    cfg.calib.RightWarp = {true,{1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0}};
    cfg.calib.LeftWarp = {false,{1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0}};

    // stereo core configuration is tuned inside the Stereo component
    // user can set custom settings here using cfg.stereocore

    // set stereo resources
    res.wsgbm_cmx_start = 0;
    res.wsgbm_cmx_end = 1;

    sc = rtems_semaphore_create(rtems_build_name('S', 'S', 'P', 'P'), 1,
                                RTEMS_SIMPLE_BINARY_SEMAPHORE, 0, (rtems_id *)&res.sipp_prot_sem);
    assert(sc == RTEMS_SUCCESSFUL);


}
#endif

visionBasePlg* createStereoPlg(){
#if defined(MA2150) || defined(MA2450)
#else
    sippInitialize();
    stereo_cfg cfg;
    stereo_resources res;

    InitCfgResStereo(cfg, res);
    cfg.calib.RightWarp = {true,
        {calib_array[0],
        calib_array[1],
        calib_array[2],
        calib_array[3],
        calib_array[4],
        calib_array[5],
        calib_array[6],
        calib_array[7],
        calib_array[8]}
    };
#endif
    customProcCtx ctx;
    ctx.camCnt = 2;
    ctx.frameRateDivider = 1;
    ctx.dropCondition = 0;
#if defined(MA2150) || defined(MA2450)
    plgVisionStereo *Stereo = new (bufStereoPlg) plgVisionStereo(ctx);
#else
    plgVisionStereo *Stereo = new (bufStereoPlg) plgVisionStereo(cfg,res,ctx);

#endif
    Stereo->Create();
    return Stereo;

}
void initializeCache()
{
#if defined(MA2150) || defined(MA2450)
    s32 sc = OsDrvShaveL2CacheInit(SHAVEL2C_MODE_NORMAL);
    if(sc)
        printf("Error Configuring L2C\n");

    sc = OsDrvShaveL2CResetPartitions();
    if(sc)
        printf("Error reset L2C\n");

    for (u32 i = 0; i < 4; i++){
        sc = OsDrvShaveL2CGetPartition(SHAVEPART64KB, &partitions[i]);
        OsDrvShaveL2CachePartitionInvalidate(partitions[i]);

    }
    /* Allocate Shave L2 cache set partitions */
    sc = OsDrvShaveL2CacheAllocateSetPartitions();

    for (u32 i = 0; i < 6; i++){
        /* Assign the partitions defined to each shave */
        sc = OsDrvShaveL2CSetWindowPartition((shaveId_t)i, SHAVEL2CACHEWIN_C, partitions[1]);
        OsDrvShaveL2CSetNonWindowedPartition((shaveId_t)i, partitions[1], NON_WINDOWED_DATA_PARTITION);
        DrvShaveL2CacheSetInstrPartId(i, partitions[0]);
    }
    for (u32 i = 6; i < CFG_NUM_SHAVES; i++){
        /* Assign the partitions defined to each shave */
        sc = OsDrvShaveL2CSetWindowPartition((shaveId_t)i, SHAVEL2CACHEWIN_C, partitions[3]);
        OsDrvShaveL2CSetNonWindowedPartition((shaveId_t)i, partitions[3], NON_WINDOWED_DATA_PARTITION);
        DrvShaveL2CacheSetInstrPartId(i, partitions[2]);
    }
#else
    uint8_t partNo[CFG_NUM_SHAVES];

    for (u32 i = 0; i < 4; i++){
        OsDrvShaveL2cAddPart(4, 0, 1, &partNo[i]);
        partitions[i] = partNo[i]; // trying to keep the chip version ifdefs isolated.
    }
    int i;
    for (i = 0; i < 6; i++)
    {
        OsDrvShaveL2cAssignPart(i, partNo[1], OS_DRV_SHAVE_L2C_WIN_PART, OS_DRV_SHAVE_L2C_WIN_C);
        OsDrvShaveL2cAssignPart(i, partNo[1], OS_DRV_SHAVE_L2C_NON_WIN_DATA_PART, OS_DRV_SHAVE_L2C_WIN_A);
        OsDrvShaveL2cAssignPart(i, partNo[0], OS_DRV_SHAVE_L2C_NON_WIN_INST_PART, OS_DRV_SHAVE_L2C_WIN_A);
    }
    for (i = 6; i < CFG_NUM_SHAVES; i++)
    {
        OsDrvShaveL2cAssignPart(i, partNo[3], OS_DRV_SHAVE_L2C_WIN_PART, OS_DRV_SHAVE_L2C_WIN_C);
        OsDrvShaveL2cAssignPart(i, partNo[3], OS_DRV_SHAVE_L2C_NON_WIN_DATA_PART, OS_DRV_SHAVE_L2C_WIN_A);
        OsDrvShaveL2cAssignPart(i, partNo[2], OS_DRV_SHAVE_L2C_NON_WIN_INST_PART, OS_DRV_SHAVE_L2C_WIN_A);
    }
#endif
}


extern "C"
int *POSIX_Init(void)
{
    OsDrvSvuInit();
    initializeCache();

#ifdef MONOISP
    brdInitMipi();
#endif
    // plugin creation based on dcc context can start here
    // all plugins must be created at this stage

    int id;
    for(id = 0; id < dcc.processingCount; id++){
        if(dcc.where[id] == PROCESS_LEON_RT){
            if (dcc.type[id] == ALGO_VTRACK){
                dcc.processingPlugin[id][0] = createVtrackPlg();
            }else if(dcc.type[id] == ALGO_STEREO){
                dcc.processingPlugin[id][0] = createStereoPlg();
            }
        }else{
        }
    }
    DataCollectorInit(&dcc);

    DrvLeonRTSignalBootCompleted();
    DataCollectorStart(&dcc);
    return NULL;
}
