#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <FramePump.h>
#include "PlgFramePumpCam.h"

#define MVLOG_UNIT_NAME cameraNode
#include <mvLog.h>

#define CAM_LEFT  0
#define CAM_RIGHT 1

void* PlgFramePumpCam::threadFunc(void* context){
    mvLogLevelSet(MVLOG_INFO);
    UNUSED(context);
    vgaThreadFunc((cameraNodeContext*) &camCtx);
    return NULL;
}
typedef struct buffStreamerCtx{
    int camDesc;
    MReceiver<ImgFramePtr>* emptyIn;
    ImgFramePtr* frameMsgList;
    int maxMsg;
}buffStreamerCtx;

void* writeAllFreeToFP(void* ctx){
    buffStreamerCtx* c = (buffStreamerCtx*) ctx;
    MReceiver<ImgFramePtr>* emptyIn = c->emptyIn;
    int camDesc = c->camDesc;
    ImgFramePtr* frameMsgList =  c->frameMsgList;

    struct FramePumpBuffer framePumpBuff;
    int sc;
    int writeIdx = 0;
    while(1){
        int rc = emptyIn->Receive(&frameMsgList[writeIdx]);
        UNUSED(rc);
        memset(&framePumpBuff, 0, sizeof(struct FramePumpBuffer));

        framePumpBuff.fbMem.p1    = (unsigned char*)(frameMsgList[writeIdx].ptr->base);
        frameMsgList[writeIdx].ptr->fb.p1 = framePumpBuff.fbMem.p1;
        framePumpBuff.cam_id = 0;
        sc = write(camDesc, &framePumpBuff, sizeof(struct FramePumpBuffer));
        if(sc != sizeof(struct FramePumpBuffer))
        {
            mvLog(MVLOG_ERROR, "failed to write !!!\n");
            perror("");
        }
        else
        {
            mvLog(MVLOG_DEBUG, "%x buffer @ %p \n", pthread_self(), framePumpBuff.fb.p1);
        }
        writeIdx++;
        writeIdx %= c->maxMsg;
    }
}

void PlgFramePumpCam::populateFrameSpec(frameSpec* fs) {
    int sc = ioctl(camDesc, FRAME_PUMP_GET_FRAME_SPEC, fs);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_ERROR, "Failed to get frame spec\n");
    }

}

void * PlgFramePumpCam::vgaThreadFunc(cameraNodeContext * context)
{
    ImgFramePtr nullObj;
    cameraNodeContext *tc = (cameraNodeContext*)context;
    int sc;
    int readOutUs;
    int cam = 0;
    mvLog(MVLOG_DEBUG, "%s initializing %s \n", __func__, tc->camName);

    pthread_t emptyFrmStreamer;
    buffStreamerCtx streamerCtx;
    streamerCtx.camDesc = camDesc;
    streamerCtx.emptyIn = &emptyIn;
    streamerCtx.frameMsgList = frameMsgList;
    streamerCtx.maxMsg = MAX_MESSAGES;
    pthread_attr_t attr;
    struct sched_param sp;
    if (pthread_attr_init(&attr) != 0)
    {
        mvLog(MVLOG_ERROR, "pthread_attr_init error\n");
    }
    if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0)
    {
        mvLog(MVLOG_ERROR, "pthread_attr_setinheritsched error\n");
    }
    if (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0)
    {
        mvLog(MVLOG_ERROR, "pthread_attr_setschedpolicy error\n");
    }
    sp.sched_priority = 200;
    if(pthread_attr_setschedparam(&attr, &sp) != 0) {
        mvLog(MVLOG_ERROR, "pthread_attr_setschedparam error");
    }
    sc = pthread_create(&emptyFrmStreamer, &attr, writeAllFreeToFP, &streamerCtx);
    if (sc)
    {
        mvLog(MVLOG_ERROR, "Control Thread creation failed\n");
    }
    if (!tc->camFrameRate){
        tc->camFrameRate = 30;
    }
    u32 desiredFrameRate = tc->camFrameRate;
    u32 sharedControl;

    sharedControl = FP_SHARED_CTL(FRAME_PUMP_SET_FRAMERATE);
    sc = ioctl(camDesc, sharedControl, &desiredFrameRate);
    if(sc)
    {
        mvLog(MVLOG_ERROR, "failed to set framerate %s !!!", strerror(errno));
    }

    sc = ioctl(camDesc, FRAME_PUMP_CONFIG_GET_READOUT, &readOutUs);
    if(sc)
    {
        mvLog(MVLOG_ERROR, "failed to get readout time !!!\n");
        perror("");
    }

    mvLog(MVLOG_DEBUG, "readout:::::::::::::::::::::::::::: %d us\n", readOutUs);


    sc = ioctl(camDesc, FRAME_PUMP_GET_FRAMERATE, &desiredFrameRate);
    if(sc)
    {
        perror("failed to get frame rate !!!");
    }
    mvLog(MVLOG_DEBUG, "Frame rate set to:::::::::::::::::::::::::::: %ld Hz\n", desiredFrameRate);

    pthread_barrier_wait(camCtx.barrier);
    sc = ioctl(camDesc, FP_SHARED_CTL(FRAME_PUMP_START));
    if(sc)
    {
        mvLog(MVLOG_ERROR, "failed to start cameras %s !!!", strerror(errno));
        assert(0);
    }


    mvLog(MVLOG_DEBUG, "%s running %d\n", __func__, cam);
    while(1)
    {
        struct FramePumpBuffer fpb;
        sc = read(camDesc, &fpb, sizeof(struct FramePumpBuffer));
        if(sc != sizeof(struct FramePumpBuffer))
        {
            mvLog(MVLOG_WARN, "failed to read %s probably timeout !!!", camCtx.camName);
            continue;
        }

        mvLog(MVLOG_DEBUG, "%s\t new frame %lx %lx %d ---------------------%d %lu : %lluns\n",
              tc->camName,(u32) &fpb, (u32) fpb.fb.p1,fpb.fb.spec.width * fpb.fb.spec.height,
              cam, fpb.frameCount, fpb.timestampNs);
        if (previousCnt && previousCnt != fpb.frameCount -1){
            assert(0);
        }
        previousCnt = fpb.frameCount;
        assert(frameMsgList[readIdx].ptr->fb.p1 == fpb.fb.p1);
        fpb.timestampNs -= readOutUs * 1000;
        u64 ts = rtems_clock_get_uptime_nanoseconds ();
        mvLog(MVLOG_DEBUG,"Latency in camera node %llu\n", ts - fpb.timestampNs);
#ifdef CONFIG_TEARING_CHECK
        assert(*((u32*)(fpb.fb.p1)) == fpb.frameCount);
        assert( *((u32*)(fpb.fb.p1 + fpb.fb.spec.height *
                        fpb.fb.spec.stride - sizeof(u32))) == fpb.frameCount);
#endif
        //construct ImgFrame
        ImgFrame* imgFrame = (ImgFrame*) frameMsgList[readIdx].ptr;
        imgFrame->fb = fpb.fb;
        imgFrame->instNo = camDesc; // TODO: probably we don't want to do this on the long term
        imgFrame->seqNo = fpb.frameCount;
        imgFrame->ts = fpb.timestampNs;
        out.Send(&frameMsgList[readIdx]);
        frameMsgList[readIdx++] = NULL;
        readIdx %= MAX_MESSAGES;
    }
    return NULL;
}
