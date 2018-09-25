#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

//TODO: get rid of FramePump dependency
#include <FramePump.h>

#include "AecNode.h"

#define MVLOG_UNIT_NAME aecNode
#include <mvLog.h>

#include <aeApi.h>

frameBuffer aeFrame;

typedef struct aeExposure {
    unsigned integrationTime;
    float gainCode;
} aeExposure_t;

void *PlgAec::threadFunc(void *){
    aecThreadFunc(&aecCtx);
    return NULL;
}

void * PlgAec::aecThreadFunc(aecContext * context)
{
    u32 camCnt = context->camCnt;
    u32 i;
    bool initialized[2] = {false};

    simpleAEpostInit();

    while(1)
    {
        int sc;
        ImgFramePtr msg[camCnt];
        uint64_t ts[2];
        for(i = 0; i < camCnt; i++)
        {
            sc = frameIn.Receive(&msg[i]);
            if (sc != 0)
            {
                mvLog(MVLOG_WARN,"AEC TIMEOUT. No frame received\n");
                continue;
            }

            if(!initialized[i])
            {
                initialized[i] = true;
                simpleAEstart(msg[0].ptr->instNo, i, "cam");
            }

            FramePumpBuffer fpbIn;
            frameBuffer fb;
            ImgFrame* frm = (ImgFrame*) msg[i].ptr;
            fb = frm->fb;
            fpbIn.fb = fb;
            fpbIn.cam_id = i;
            fpbIn.frameCount = frm->seqNo;

            mvLog(MVLOG_DEBUG, "Received %d", frm->fb.spec.bytesPP);

            simpleAEprocessFrame(i, &fpbIn);
            ts[i] = frm->ts;
        }

        aeMessage_t received = simpleAEget();
        mvLog(MVLOG_DEBUG, "Received AE value");

        FramePumpCamExpGain camExpGainPair;

        camExpGainPair.exposureTimeUs = received.integrationTimeUs;

        for (i = 0; i < camCnt; i++)
        {
            camExpGainPair.gainMultiplier = received.gainCode[i];
            mvLog(MVLOG_DEBUG, "Cam %d. Exposure: %d Gain %f\n", msg[i].ptr->instNo,
                  camExpGainPair.exposureTimeUs, camExpGainPair.gainMultiplier);
            sc = ioctl(msg[i].ptr->instNo, FRAME_PUMP_CONFIG_SEND_EXPGAIN,
                       &camExpGainPair);
            uint64_t currentTs = rtems_clock_get_uptime_nanoseconds ();
            if((int)(currentTs - ts[i]) > 33000000){
                mvLog(MVLOG_WARN, "Exposure written with latency %d ns. Potential AEC problem.\n", (int)(currentTs - ts[i]));
            }
            if(sc)
            {
                mvLog(MVLOG_ERROR, "failed to set exposure and gain !!!");
            }
        }
    }


    return NULL;
}

int aecInit(void)
{
    int status = 0;

    status = simpleAEinit();

    return status;
}
