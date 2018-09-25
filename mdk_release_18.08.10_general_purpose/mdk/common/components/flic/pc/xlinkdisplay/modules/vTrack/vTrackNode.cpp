/// =====================================================================================
///
///        @file:      be_xlink.cpp
///        @brief:     flicpc Xlink Backend
///        @author:    csoka, attila.csok@movidius.com
///        @copyright: All code copyright Movidius Ltd 2013, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

/// System Includes
/// -------------------------------------------------------------------------------------
#include "vTrackNode.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <mqueue.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "helper.h"

#include "superFrame.h"
#include "vTrackOutput.h"
#include "VtrackDrawer.h"
#include "VtrackDrawerMoviOf.h"

#define  FRAME_WIDTH  1280
#define  FRAME_HEIGHT  720

/// Application Includes
/// -------------------------------------------------------------------------------------

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------3

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------

/// Functions Implementation
/// -------------------------------------------------------------------------------------

 void *vTrackNode::ThreadItf(void *This)
{
    ((vTrackNode*)This)->Thread();
}

void  *vTrackNode::Thread()
{
    VtrackDrawer* vtrackDrawer = new VtrackDrawerMoviOf(FRAME_WIDTH, FRAME_HEIGHT,
                                                        1024, 0);
    vtrackDrawer->initDrawer();

    mqd_t mqdes = mq_open(inMetaQuName, O_RDWR);
    if (mqdes == (mqd_t) - 1) {
        perror("mq_open");
        assert(0);
    }

    mqd_t mqdesO = mq_open(outQuName, O_RDWR | O_NONBLOCK);
    if (mqdesO == (mqd_t) - 1) {
        perror("mq_open");
        assert(0);
    }

    mqd_t mqdes1 = mq_open(inFrameQuName, O_RDWR);
    if (mqdes == (mqd_t) - 1) {
        perror("mq_open");
        assert(0);
    }

    int seqNo = 0;
    while (1) 
    {
        int status;
        VisionFrame* frameData = new VisionFrame(FRAME_WIDTH, FRAME_HEIGHT);

        uint8_t *frame;
        status = mq_receive(mqdes1, (char*)&frame, sizeof(uint8_t*), 0);
        if (status != sizeof(uint8_t*)) {
            printf("status: %d %d\n" , status, sizeof(uint8_t*) );
            perror("mq_receive");
            assert(0);
        }
        memcpy(frameData->p1, frame, FRAME_WIDTH * FRAME_HEIGHT);
        free(frame);

        frameMeta *metaPtr;
        status = mq_receive(mqdes, (char*)&metaPtr, sizeof(uint8_t*), 0);
        if(status != sizeof(uint8_t*))
        {
            printf("status: %d\n" , status );
            perror("mq_receive");
            assert(0);
        }

        vTrackBulkResult bulkResult;
        bulkResult.setAddrAndPtrs((uint8_t*)metaPtr->customMetadata);
        t_vTrackResultSF* vData;
        bulkResult.getConfigAndResults(NULL, &vData);

        cvShowMsg_t request;
        request.data = (void*)vtrackDrawer->draw(frameData,(uint8_t*)vData);
        request.windowname   = "vTrack";
        request.spec.width  = FRAME_WIDTH;
        request.spec.height = FRAME_HEIGHT;
        request.mode   = CV_MAT_DISPLAY;
        request.scaling = 1.0;
        request.seqNo = seqNo++;
        request.resultString = nullptr;

        status = mq_send(mqdesO, (char *) &request, sizeof(cvShowMsg_t), 0);
        if (status) {
            perror("mq_send");
            //assert(0);
            ((cv::Mat*)request.data)->release();
        }

        free(metaPtr);
        delete(frameData);
    }

    free(vtrackDrawer);
}

void vTrackNode::Start()
{
    int ret = pthread_create(&thread, NULL, ThreadItf, this);
    if (ret) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", ret);
        exit (EXIT_FAILURE);
    } else {
        printf("thread created\n");
    }
}

void vTrackNode::Stop()
{
    pthread_join(thread, NULL);
}
