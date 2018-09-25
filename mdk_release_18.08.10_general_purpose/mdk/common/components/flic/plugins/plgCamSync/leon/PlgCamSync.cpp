///
/// @file      PlgCamSync.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Plugin for ISP Pipelines
///            Platform(s) supported : ma2x8x
///

#include "PlgCamSync.h"

#include <stdio.h>


//#############################################
void PlgCamSync::Create() {
    inO[0].Create(8); Add(&inO[0]);
    inO[1].Create(8); Add(&inO[1]);

    Add(&out[0]);
    Add(&out[1]);
    Add(&outStereo,".outStereo");
}

//#############################################
void * PlgCamSync::threadFunc(void *) {
    pthread_setname_np(RTEMS_SELF, "PlgIspStereoCtrl");
    TimeSpec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 33333 * 1e3; // frame time
    int64_t thresh = 33333 / 2;

    while(Alive()) {
        StereoImgMsg stereo;
        ImgFramePtr frames[2];

        int er = inO[0].Receive(&frames[0]);
        er    |= inO[1].Receive(&frames[1], &ts);

        while(er == 0 && frames[1].ptr->ts < frames[0].ptr->ts - thresh){
            frames[1] = NULL;
            er |= inO[1].Receive(&frames[1], &ts);
        }

        while(er == 0 && frames[0].ptr->ts < frames[1].ptr->ts - thresh){
            frames[0] = NULL;
            er |= inO[0].Receive(&frames[0], &ts);
        }

        if(er == 0){
            int64_t diff = frames[1].ptr->ts - frames[0].ptr->ts;
            if(diff < 0) diff = -diff;

            if(diff < thresh){
                //We have decided these are synced so set the seqNo the same
                frames[1].ptr->seqNo = frames[0].ptr->seqNo;
                out[0].Send(&frames[0]);
                out[1].Send(&frames[1]);
                stereo.frame[0] = frames[0];
                stereo.frame[1] = frames[1];

                outStereo.Send(&stereo);
            }
            else
                printf("stereo sync - diff too large %lld us!\n",
                       frames[1].ptr->ts - frames[0].ptr->ts);
        }
        else
        {
            printf("stereo sync - missing frames!\n");
        }


    }
    return NULL;
}

