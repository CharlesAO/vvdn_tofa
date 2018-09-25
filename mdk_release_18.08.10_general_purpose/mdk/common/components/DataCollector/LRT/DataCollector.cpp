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
#include <DataCollector.h>
#include "aeThread.h"

#include "DataCollector.h"
#include <MemAllocator.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <assert.h>

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>
#include <FlicRmt.h>
#include <RmtBarr.h>

#include "visionBasePlg.h"
#include "superFrame.h"




#define MVLOG_UNIT_NAME dataCollector
#include <mvLog.h>

//TODO SHOULD THIS BE DEFINED SOMEWHERE BETTER
#define BUFF_COUNT_OUT 12

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// -----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// -----------------------------------------------------------------------------
// 4: Static Local Data
// -----------------------------------------------------------------------------

bool inRemotesAdded = false;


Pipeline p;
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

//Pools
PlgPool<ImgFrame>      plgPoolMeta    SECTION(".cmx_direct.data");

//Remote plugin inputs
RmtI<ConfigMessage> RemConfigIn SECTION(".cmx_direct.data");
extern RmtO<ConfigMessage> RemConfigOut;
RmtI<ImgFramePtr> RemLeftIn SECTION(".cmx_direct.data");
extern RmtO<ImgFramePtr> RemLeftOut;
RmtI<ImgFramePtr> RemRightIn SECTION(".cmx_direct.data");
extern RmtO<ImgFramePtr> RemRightOut;
RmtI<RotMatMsg> RemRotIn SECTION(".cmx_direct.data");
extern RmtO<RotMatMsg> RemRotOut;

#ifdef MONOISP
#include "MonoPipe_RT.h"
#include "poolSizes.h" 
extern MonoPipe_OS   MonoPipe_OS;
MonoPipe_RT     MonoPipe_RT    PLUGINS_MEM_SECT;
#endif

//Remote plugin outputs
RmtO<StereoImgAndMetaMsg> RemOutSyncOut[2] SECTION(".cmx_direct.data");
extern RmtI<StereoImgAndMetaMsg> RemOutSyncIn[2];

extern RmtBarr     bLOS;
       RmtBarr     bLRT  SECTION(".cmx_direct.data");;

void AddProcPlugin(visionBasePlg* plg, swcProcessorType target, uint32_t procId)
{
    if(target == PROCESS_LEON_RT)
    {
        //Note: plg pools added in DataCollectorStart
        p.Add(plg,"LRT_procPlug");
        if(!inRemotesAdded)
        {
#ifndef MONOISP
            RemConfigIn.Create(&RemConfigOut);
            RemLeftIn.Create(&RemLeftOut);
            RemRightIn.Create(&RemRightOut);
#endif            
            RemRotIn.Create(&RemRotOut);
#ifndef MONOISP
            p.Add(&RemConfigIn,"LRT_RemConfigIn");
            p.Add(&RemLeftIn,"LRT_RemLeftIn");
            p.Add(&RemRightIn,"LRT_RemRightIn");
#endif            
            p.Add(&RemRotIn,"LRT_RemRotIn");
            inRemotesAdded = true;
        }
        RemOutSyncOut[procId].Create(&RemOutSyncIn[procId]);
        p.Add(&RemOutSyncOut[procId],"LRT_RemOutSyncOut");
    }

}

void LinkProcPlugin(visionBasePlg* plg, swcProcessorType target, uint32_t procId)
{

    if(target == PROCESS_LEON_RT)
    {
        plgPoolMeta.out.Link(&plg->emptyMeta);
        
#ifndef MONOISP
        RemLeftIn.out.Link(&plg->frameLeft);
        RemConfigIn.out.Link(&plg->cfg);
        if (plg->ctx.camCnt > 1){
            RemRightIn.out.Link(&plg->frameRight);
        }
#else
        MonoPipe_RT.plgSync.out[0].Link(&plg->frameRight);
        MonoPipe_RT.plgSync.out[1].Link(&plg->frameLeft);
#endif
        if (plg->gyroAssist){
            RemRotIn.out.Link(&plg->rotMatIn);
        }
        plg->synchronizedOut.Link(&RemOutSyncOut[procId].in);

    }
}


void* DataCollectorInit(dataCollectorContext* ctx){
#ifndef MONOISP
    aeThreadInit();
#endif    
    RgnAlloc.Create(RgnBuff, DEF_POOL_SZ);
    int i;
    int proc;
    FlicRmt::Init();
#ifdef MONOISP
    MonoPipe_RT.Create (&MonoPipe_OS , &RgnAlloc);
    MonoPipe_RT.AddTo(&p);
#endif

    plgPoolMeta.Create(&RgnAlloc, BUFF_COUNT_OUT, sizeof(frameMeta), true);
    p.Add(&plgPoolMeta);
    bLRT.Create(&bLOS);

    for(proc = 0; proc < ctx->processingCount; proc++){
        mvLog(MVLOG_DEBUG,"processing node type %d running on %d threads\n", proc, ctx->processingthreadCnt[proc]);
        for(i = 0; i < ctx->processingthreadCnt[proc]; i++){
            printf("ABOUT_TO_LINK_PROC %d %d\n", proc, i);
            AddProcPlugin(ctx->processingPlugin[proc][i], ctx->where[proc],proc);
            LinkProcPlugin(ctx->processingPlugin[proc][i], ctx->where[proc],proc);
        }
    }
    return 0;
}

void* DataCollectorStart(dataCollectorContext* ctx)
{
    UNUSED(ctx);
    bLRT.Wait();
    p.Start();
    p.Wait();
    return(0);
}
