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

#include <math.h>

// #include "iAlgRunner.h"
// #include "iDrawer.h"
// #include "algConfig.h"
#include "stereoPostProcNode.h"
#include "helper.h"
#include "fe_cvdisplay.h"

#define DISPARITY 96
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

 void *stereoPostProcNode::ThreadItf(void *This)
{
    ((stereoPostProcNode*)This)->Thread();
}

 void stereoPostProcNode::stretchDisparityRange(cv::Mat& frame, int disp)
 {
     const int scaleFactorLowerD = 2;
     const int scaleFactorUpperD = 1;

     int halfIntervalD = disp/2;

     for (int row = 0; row < frame.rows; row++)
         for (int col = 0; col < frame.step; col++)
         {
             // if disparity value = [0, D/2), multiply it with 4
             if (frame.data[col + row * frame.step] < halfIntervalD)
                 frame.data[col + row * frame.step] *= scaleFactorLowerD;
             else
                 // if disparity value = [D/2, D), multiply it with 2
                 //frame.data[col + row * frame.step] *= scaleFactorUpperD;
                 frame.data[col + row * frame.step] =
             frame.data[col + row * frame.step] * scaleFactorUpperD +
             scaleFactorLowerD * halfIntervalD;
         }
 }


void *stereoPostProcNode::Thread() {
    mqd_t mqdesI = mq_open(inQuName, O_RDWR);
    if (mqdesI == (mqd_t) - 1) {
        perror("mq_open");
        assert(0);
    }
    mqd_t mqdesO = mq_open(outQuName, O_RDWR | O_NONBLOCK);
    if (mqdesO == (mqd_t) - 1) {
        perror("mq_open");
        assert(0);
    }

    int seqNo = 0;

    while (1) {
        uint8_t *frame;
        int status = mq_receive(mqdesI, (char *) &frame, sizeof(uint8_t*), 0);
        assert(status == sizeof(uint8_t*));

        cv::Mat disparityMapMyriad = cv::Mat(frameHeight, frameWidth, CV_8UC1, frame);
        cv::Mat *outputImageJet = new cv::Mat();

        medianBlur(disparityMapMyriad, disparityMapMyriad, 5);

        stretchDisparityRange(disparityMapMyriad, DISPARITY);

        applyColorMap(disparityMapMyriad, *outputImageJet, cv::COLORMAP_JET);

        //cv::flip(*outputImageJet,*outputImageJet,1);

        free(frame);

        cvShowMsg_t request;
        request.data   = outputImageJet;
        request.windowname  = "disparity";
        request.spec.width  = frameWidth;
        request.spec.height = frameHeight;
        request.mode   = CV_MAT_DISPLAY;
        request.scaling = 1.0;
        request.seqNo = seqNo;
        request.resultString = nullptr;
        status = mq_send(mqdesO, (char *) &request, sizeof(cvShowMsg_t), 0);
        if (status) {
            perror("mq_send");
            //assert(0);
            outputImageJet->release();
        }

        seqNo ++;
    }
}

void stereoPostProcNode::Start()
{
    int ret = pthread_create(&thread, NULL, ThreadItf, this);
    if (ret) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", ret);
        exit (EXIT_FAILURE);
    } else {
        printf("thread created\n");
    }
}

void stereoPostProcNode::Stop()
{
    pthread_join(thread, NULL);
}
