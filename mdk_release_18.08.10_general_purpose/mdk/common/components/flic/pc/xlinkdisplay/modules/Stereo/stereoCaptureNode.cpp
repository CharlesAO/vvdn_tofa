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
#include "stereoCaptureNode.h"
#include "helper.h"
#include "fe_cvdisplay.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#define  FRAME_WIDTH  1280
#define  FRAME_HEIGHT  720
#define OUTPUT_DIR          "output"

uint8_t frameLeftTmp[FRAME_HEIGHT*FRAME_WIDTH], frameRightTmp[FRAME_HEIGHT*FRAME_WIDTH];
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

 void *stereoCaptureNode::ThreadItf(void *This)
{
    ((stereoNode*)This)->Thread();
}

void *stereoCaptureNode::Thread() {
    mqd_t mqdesI = mq_open(inQuName, O_RDWR);
    if (mqdesI == (mqd_t) - 1) {
        perror("mq_open");
        assert(0);
    }
    int key;
    uint32_t frameCnt = 0;
    system("mkdir " OUTPUT_DIR);
    cv::Mat imgLeft(FRAME_HEIGHT,FRAME_WIDTH,CV_8UC1,frameLeftTmp);
    cv::Mat imgRight(FRAME_HEIGHT,FRAME_WIDTH,CV_8UC1,frameRightTmp);
    while (1) {
        uint16_t *frameLeft, *frameRight;
        int statusL = mq_receive(mqdesI, (char *) &frameLeft, sizeof(uint16_t*), 0);
        assert(statusL == sizeof(uint16_t*));

        int statusR = mq_receive(mqdesI, (char *) &frameRight, sizeof(uint16_t*), 0);
        assert(statusR == sizeof(uint16_t*));

        for(int i=0; i<FRAME_WIDTH*FRAME_HEIGHT; i++)
        {
            frameLeftTmp[i]=(uint8_t)(frameLeft[i]>>2);
            frameRightTmp[i]=(uint8_t)(frameRight[i]>>2);
        }
        frameCnt++;
        cv_display_frame("LeftFrame", FRAME_WIDTH, FRAME_HEIGHT, frameLeftTmp, FRAME_FORMAT_MONOCHROME,1,(int)frameCnt);
        cv_display_frame("RightFrame", FRAME_WIDTH, FRAME_HEIGHT, frameRightTmp, FRAME_FORMAT_MONOCHROME,1,(int)frameCnt);
        cv::imwrite( OUTPUT_DIR "/cam0.png",imgLeft);
        cv::imwrite( OUTPUT_DIR "/cam1.png",imgRight);
        key = cv::waitKey(1);
        if(key == 27)
            {
                printf("user requested exit - images saved\n");
                exit(0);
            }
        
        free(frameLeft);
        free(frameRight);
    }
}

void stereoCaptureNode::Start()
{
    int ret = pthread_create(&thread, NULL, ThreadItf, this);
    if (ret) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", ret);
        exit (EXIT_FAILURE);
    } else {
        printf("thread created\n");
    }
}

void stereoCaptureNode::Stop()
{
    pthread_join(thread, NULL);
}
