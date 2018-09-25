///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///


// 1: Includes
// ----------------------------------------------------------------------------
#include "DataCollector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rtems.h>
#include <rtems/status-checks.h>
#include <rtems/libio.h>
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <fcntl.h>
#include <OsDrvCpr.h>


#include <AecNode.h>

#include <assert.h>

#include "outStreamNode.h"
#include "rotationMatrixPlg.h"

#include <FramePump.h>


#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>
//#include <PlgPool.h>
#include <MemAllocator.h>
#include <FlicRmt.h>
#include <RmtBarr.h>
#include "PlgCamSync.h"
#include "PlgFramePumpCam.h"
#include "PlgImu.h"
#include "visionBasePlg.h"
#include "PlgXlinkConfig.h"




#define MVLOG_UNIT_NAME dataCollector
#include <mvLog.h>
// 2:  Source Specific #defines and types  (typedef, enum, struct)
#define BUFF_COUNT_IN 12
#define BUFF_COUNT_OUT 12
// -----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// -----------------------------------------------------------------------------
// 4: Static Local Data
// -----------------------------------------------------------------------------


//Remote plugin inputs
extern RmtI<ConfigMessage> RemConfigIn;
RmtO<ConfigMessage> RemConfigOut SECTION(".cmx_direct.data");
extern RmtI<ImgFramePtr> RemLeftIn;
RmtO<ImgFramePtr> RemLeftOut SECTION(".cmx_direct.data");
extern RmtI<ImgFramePtr> RemRightIn;
RmtO<ImgFramePtr> RemRightOut SECTION(".cmx_direct.data");
extern RmtI<RotMatMsg> RemRotIn;
RmtO<RotMatMsg> RemRotOut SECTION(".cmx_direct.data");

#ifdef MONOISP
#include "ic_main.h"
#include "MonoPipe_OS.h"
#include "poolSizes.h"

MonoPipe_OS MonoPipe_OS             PLUGINS_MEM_SECT;
MonoPipeConfig grpMonoCfg      PLUGINS_MEM_SECT;
extern MonoPipe_RT    MonoPipe_RT;
const char* mqRecName   = "mqRecName";
#endif

//TODO Move somewhere more suitable
#define ISP_FIRST_SLICE (9)

#define NR_OF_MONO 2

#define CAM_B_IMG_BITRATE   (10)
#define CAM_B_NOEXPOSURE    (1)

#define ISP_MONO_NROFSLICES  4



//Remote plugin outputs
extern RmtO<StereoImgAndMetaMsg> RemOutSyncOut[2];
RmtI<StereoImgAndMetaMsg> RemOutSyncIn[2] SECTION(".cmx_direct.data");

extern RmtBarr        bLRT;
       RmtBarr        bLOS  SECTION(".cmx_direct.data");;


PlgPool<ImgFrame>      plgPoolSrc[MAX_CAMERA_COUNT]     SECTION(".cmx_direct.data");
PlgPool<ImgFrame>      plgPoolMeta    SECTION(".cmx_direct.data");
PlgPool<ImgFrame>      plgPoolIMU    SECTION(".cmx_direct.data");
PlgFramePumpCam  plgCam[MAX_CAMERA_COUNT];
PlgOutStream  plgOutStrm;
PlgAec plgAec;
PlgCamSync plgSync;
PlgXlinkConfig plgCfg;
PlgImu plgImu;
PlgRotationMat plgRot;
Pipeline p;
customProcCtx cpctx[MAX_PROCESS_THREADS];
extern "C" void gzz_init (void);

int lrt_ae_camera_count;
bool outRemotesAdded = false;
bool outRemotesLinked = false;
bool outIMULinked= false;

pthread_barrier_t barrier;
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------


void* DataCollectorInit(dataCollectorContext* ctx){
    mvLogLevelSet(MVLOG_INFO);

    mvLog(MVLOG_INFO, "GONE WITH THE WIND !!!!!!!!!!!!!!\n");
    FlicRmt::Init();

    RgnAlloc.Create(RgnBuff, DEF_POOL_SZ);
#ifndef MONOISP
    int sc = pthread_barrier_init(&barrier, NULL, ctx->camCnt);
    if(sc){
        mvLog(MVLOG_FATAL, "Barrier creation failed !!!!!!!!!!!!!! %d \n",sc);
    }
#endif    
    plgOutStrm.camCnt = ctx->camCnt;
    plgOutStrm.lowLatencyEnable = ctx->latencyControl;
    plgOutStrm.Create();
    p.Add(&plgOutStrm);

#ifndef MONOISP    
    plgCam[0].isMaster = 1;
    plgSync.Create();
    p.Add(&plgSync);
#endif

    bLOS.Create(&bLRT);

#ifndef MONOISP
    lrt_ae_camera_count = ctx->camCnt;
    plgAec.aecCtx.camCnt = ctx->camCnt;
    plgAec.Create();
    p.Add(&plgAec);
#else        
    for(int cam = 0; cam < N_MONO_CAMS; ++cam )
    {
        grpMonoCfg.camId[cam]                           = cam + 1;
        grpMonoCfg.spSrcCfg[cam].nrFrmsSrc              = POOL_MONO_SRC_COUNT;
        grpMonoCfg.spSrcCfg[cam].maxImgSz               = POOL_MONO_SRC_FSZ * CAM_B_NOEXPOSURE;
        grpMonoCfg.spIspVdoCfg[cam].nrFrmsIsp           = POOL_ISP_VDO_COUNT;
        grpMonoCfg.spIspVdoCfg[cam].maxImgSz            = POOL_MONO_ISP_VDO_FSZ;
        grpMonoCfg.spIspVdoCfg[cam].downscale2xOn       = 0;
        grpMonoCfg.spIspVdoCfg[cam].firstCmxSliceUsed   = ISP_FIRST_SLICE;
        grpMonoCfg.spIspVdoCfg[cam].nrOfCmxSliceUsed    = ISP_MONO_NROFSLICES;
        grpMonoCfg.spIspVdoCfg[cam].monoIrqRatePow      = 5;
        grpMonoCfg.mesageQueueEventName[cam]    = mqRecName;
        grpMonoCfg.rgnAlloc                = &RgnAlloc;
        grpMonoCfg.ipcThreadPriority[cam] = 240; // set a hi priority for very small ipc threads
    }


    MonoPipe_OS.Create(&grpMonoCfg, &MonoPipe_RT);
    MonoPipe_OS.AddTo(&p);
#endif
    // plugin for xlink configuration
    plgCfg.Create(2, "config");
    p.Add(&plgCfg);
    plgPoolMeta.Create(&RgnAlloc, BUFF_COUNT_OUT, sizeof(frameMeta));
    p.Add(&plgPoolMeta);
#ifndef MONOISP        
    mvLog(MVLOG_DEBUG,"MONOISP\n");
    int i;
    for(i = 0; i < ctx->camCnt; i++){
        //camera Nodes
        plgCam[i].camCtx.camName = ctx->camName[i];
        plgCam[i].camCtx.camFrameRate = ctx->camFrameRate;
        plgCam[i].camCtx.camCnt = ctx->camCnt;
        plgCam[i].camCtx.barrier = &barrier;
        plgCam[i].Create();
        p.Add(&plgCam[i]);
        plgCam[i].populateFrameSpec(&ctx->frSpec);
        plgPoolSrc[i].Create(&RgnAlloc, BUFF_COUNT_IN,
                ctx->frSpec.height*ctx->frSpec.stride, true); //RAW
        p.Add(&plgPoolSrc[i]);
        //creating links between camera nodes, sync, and AEC
        plgCam[i].out.Link(&plgSync.inO[i]);
        plgPoolSrc[i].out.Link(&plgCam[i].emptyIn);
        plgSync.out[i].Link(&plgAec.frameIn);
    }
    plgSync.outStereo.Link(&plgOutStrm.inFrames);
#else        
    mvLog(MVLOG_DEBUG,"FRAMEPUMP\n");
    MonoPipe_OS.osRiIspOut.out.Link(&plgOutStrm.inFrames);
#endif    
    mvLog(MVLOG_DEBUG,"CAM_SYNC_LINK_COMPL\n");
//
//    //IMU specific plugins and connections
    plgImu.Create();
    plgPoolIMU.Create(&RgnAlloc, 100, sizeof(mvImuData_t));
    p.Add(&plgPoolIMU);
    p.Add(&plgImu);
    plgPoolIMU.out.Link(&plgImu.emptyIn);
    plgImu.out.Link(&plgOutStrm.inIMU);
    plgRot.Create();
    p.Add(&plgRot);
    plgImu.out.Link(&plgRot.inIMU);
#ifndef MONOISP        
    plgSync.out[0].Link(&plgRot.timeStampIn);
#endif    
    return 0;
}

void AddProcPlugin(visionBasePlg* plg, swcProcessorType target, uint32_t procId)
{
    if(target == PROCESS_LEON_OS)
    {
        printf("adding native plugin on LOS\n");
        p.Add(plg);
    }
    else
    {

        printf("adding remote outputs on LOS\n");
        if(!outRemotesAdded)
        {
            // add config plugin
            RemConfigOut.Create(&RemConfigIn);
#ifndef MONOISP
            RemLeftOut.Create(&RemLeftIn);
            RemRightOut.Create(&RemRightIn);
#endif  
            RemRotOut.Create(&RemRotIn);
#ifndef MONOISP
            p.Add(&RemLeftOut);
            p.Add(&RemRightOut);
#endif         
            p.Add(&RemRotOut);
            p.Add(&RemConfigOut);
            outRemotesAdded = true;
        }
        printf("adding remote outputs on LOS\n");
        RemOutSyncIn[procId].Create(&RemOutSyncOut[procId]);
        p.Add(&RemOutSyncIn[procId]);

    }

}

void LinkProcPlugin(visionBasePlg* plg, uint32_t camCnt, swcProcessorType target, dataCollectorAlgoType type,uint32_t procId)
{

    //TODO Needs to run on both LOS & LEON_RT
    //Currently running on LOS Only.
    //Needs to add the plugin only on correct target
    //Needs to add IPC plugins if using wrapper
    MsgGid gid;

    switch(type){
    case ALGO_VTRACK:
        gid = GID_VTRACK;
        break;
    case ALGO_STEREO:
        gid = GID_STEREO;
        break;
    case ALGO_NONE:
    default:
        assert(0 && "Unknown algorithm\n");
    }

    int outSyncId = plgOutStrm.AddMetaInput(gid);

    if(target == PROCESS_LEON_OS)
    {
#ifdef MONOISP
        (void)plg;
        (void)camCnt;
        assert(0 && "NOT SUPPORTED\n");
#else            
        plgPoolMeta.out.Link(&plg->emptyMeta);
        plgSync.out[0].Link(&plg->frameLeft);
        plgCfg.out.Link(&plg->cfg);
        if (camCnt > 1){
            plgSync.out[1].Link(&plg->frameRight);
        }
        plg->synchronizedOut.Link(&plgOutStrm.syncIn[outSyncId]);
#endif
    }
    else
    {
        if (!outRemotesLinked){
            outRemotesLinked = true;
#ifndef MONOISP
            plgCfg.out.Link(&RemConfigOut.in);
            plgSync.out[0].Link(&RemLeftOut.in);
            if (camCnt > 1){
                plgSync.out[1].Link(&RemRightOut.in);
            }
#endif        
        }

        RemOutSyncIn[procId].out.Link(&plgOutStrm.syncIn[outSyncId]);
    }
}

void LinkIMU(visionBasePlg* plg, swcProcessorType target)
{

    if(target == PROCESS_LEON_OS)
    {
        plgRot.out.Link(&plg->rotMatIn);
    }
    else
    {
        if(!outIMULinked){
            outIMULinked = true;
            plgRot.out.Link(&RemRotOut.in);
        }
    }
}
void* DataCollectorStart(dataCollectorContext* ctx)
{
    int i;


    int proc;
    for(proc = 0; proc < ctx->processingCount; proc++){
        mvLog(MVLOG_DEBUG,"processing node type %d running on %d threads\n", proc, ctx->processingthreadCnt[proc]);
        for(i = 0; i < ctx->processingthreadCnt[proc]; i++){
            AddProcPlugin(ctx->processingPlugin[proc][i],ctx->where[proc],proc);
            LinkProcPlugin(ctx->processingPlugin[proc][i],ctx->camCnt,ctx->where[proc],ctx->type[proc],proc);
            mvLog(MVLOG_DEBUG,"PROC_PLUG_LINK_COMPL\n");
        }
    }
    for(proc = 0; proc < ctx->processingCount; proc++){
        for(i = 0; i < ctx->processingthreadCnt[proc]; i++){
            LinkIMU(ctx->processingPlugin[proc][i],ctx->where[proc]);
        }
    }
    mvLog(MVLOG_DEBUG,"IMU_SYNC_COMPL\n");

    bLOS.Wait(); //wait for LRT to hit the barrier

    p.Start();
#ifdef MONOISP        
    startAppLevelEventsReceiver(mqRecName);
    gzz_init();
#endif    

    mvLog(MVLOG_WARN,"PIPELINE_STARTED\n");
    p.Wait();

    while(1)
        sleep(1);
    return NULL;
}
