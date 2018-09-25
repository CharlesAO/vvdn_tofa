#include <pthread.h>
#include <time.h>
#include <string.h>
#include <mvMacros.h>
#include "superFrame.h"
#include "swcFrameTypes.h"
#include "visionBasePlg.h"

#define MVLOG_UNIT_NAME customProcNode
#include <mvLog.h>

#define NUMBER_OF_SHAVE_THREADS 2

#define CMX_NOCACHE __attribute__((section(".cmx_direct.data")))
static float timespec_diff(struct timespec *start, struct timespec *stop)
{
    assert(start);
    assert(stop);
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        start->tv_sec = stop->tv_sec - start->tv_sec - 1;
        start->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        start->tv_sec = stop->tv_sec - start->tv_sec;
        start->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }

    return start->tv_nsec/ 1000000000.0 + start->tv_sec;
}


void visionBasePlg::Create(){
    assert(ctx.camCnt <= MAX_CAMERA_COUNT);
    Add(&synchronizedOut, ".synchronizedOut");
    frameLeft.Create(2);
    frameRight.Create(2);
    rotMatIn.Create(2);
    cfg.Create(2);
    Add(&cfg, ".cfg");
    Add(&frameLeft, ".frameLeft");
    Add(&frameRight, ".frameRight");
    Add(&rotMatIn, ".rotMatIn");
    Add(&emptyMeta, ".emptyMeta");
    Add(&meta0, ".meta0");
    Add(&meta1, ".meta1");
    pluginSpecificInit();
}

int visionBasePlg::ReceiveInputs() {
    int sc;

    ConfigMessage pc_message;
    if( OK == cfg.TryReceive(&pc_message)){
        messageAvailable = true;
        memcpy(&messageReceived, &pc_message, sizeof(ConfigMessage));
    }
    //receive frames
    sc = frameLeft.Receive(&frames[0]);
    if(ctx.camCnt > 1){
        sc += frameRight.Receive(&frames[1]);
    }else{
        frames[1] = nullptr;
    }
    if(sc){
        mvLog(MVLOG_WARN, "Processing timeout. No frames to process\n");
        return -1;
    }
    int camCnt = ctx.camCnt;
    // receive empty frame/metas if the frame is not to be dropped 
    while(camCnt--)
    {
        if ( frames[camCnt].ptr->seqNo % ctx.frameRateDivider != ctx.dropCondition){
            return -1;
        }
        metaOut[camCnt] = &meta[camCnt];
        sc = emptyMeta.Receive(metaOut[camCnt]);
        metaOut[camCnt]->ptr->fb.p1 = (u8*)metaOut[camCnt]->ptr->base;

        synchronizedMsg.frame[camCnt] = frames[camCnt];
        frameToProcess[camCnt] = &frames[camCnt];
        if(sc){
            mvLog(MVLOG_WARN, "Out of memory?\n");
            return -1;
        }
    }

    // receive gyro input if applicable
    if(gyroAssist){
        RotMatMsg rotMat;
        int rc = rotMatIn.Receive(&rotMat);

        rotationVector[0] = rotMat.rotationMatrix[0];
        rotationVector[1] = rotMat.rotationMatrix[1];
        rotationVector[2] = rotMat.rotationMatrix[2];

        while(rotMat.timeStamp < (uint64_t)frames[0].ptr->ts){
            int rc = rotMatIn.Receive(&rotMat);
            UNUSED(rc);
            rotationVector[0] += rotMat.rotationMatrix[0];
            rotationVector[1] += rotMat.rotationMatrix[1];
            rotationVector[2] += rotMat.rotationMatrix[2];
            mvLog(MVLOG_WARN, "Rotation matrix ts is smaller than frame ts. Getting new rot. mat\n");

        }
        if(rotMat.timeStamp != (uint64_t)frames[0].ptr->ts){
            rotationVector[0] = 0;
            rotationVector[1] = 0;
            rotationVector[2] = 0;
            mvLog(MVLOG_WARN, "Rotation matrix can't be calculated due to frame drop\n");
        }
        UNUSED(rc);
    }


    return 0;
}

void* visionBasePlg::threadFunc(void *context)
{
    UNUSED(context);
    mvLogLevelSet(MVLOG_INFO);
    while(Alive())
    {
        int camCnt = ctx.camCnt;
        assert(camCnt <= 2); // we support max two cameras to ease integration with MIG plugins
        assert(camCnt > 0); // we support max two cameras to ease integration with MIG plugins
        //struct timespec start, end;

        // Receive inputs
        int sc = ReceiveInputs();
        if (sc != 0)
        {
            continue;
        }
        (void)timespec_diff;
        //clock_gettime(CLOCK_REALTIME, &end);
        //mvLog(MVLOG_DEBUG, "Latency in customProc %llu %x\n", timespec_diff(&start, &end), pthread_self());
        pluginSpecificProcess();
        
        synchronizedMsg.count = ctx.camCnt;
        for(int i=0; i<camCnt; i++) {
            ((frameMeta*)metaOut[i]->ptr->fb.p1)->genericMeta.frameCount = frames[i].ptr->seqNo;
            ((frameMeta*)metaOut[i]->ptr->fb.p1)->genericMeta.timestampNs = frames[i].ptr->ts;
            ((frameMeta*)metaOut[i]->ptr->fb.p1)->customMetaSize =  metaOut[i]->ptr->size;
            synchronizedMsg.meta[i] = meta[i];
        }

        synchronizedOut.Send(&synchronizedMsg);
        meta0.Send(metaOut[0]);
        meta1.Send(metaOut[1]);

    }
    releaseAllFrms();
    return 0;
}
