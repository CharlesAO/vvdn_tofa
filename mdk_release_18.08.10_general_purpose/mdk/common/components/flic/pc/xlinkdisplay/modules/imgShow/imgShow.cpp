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
#include "imgShow.h"

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

// #include "iAlgRunner.h"
// #include "iDrawer.h"
// #include "algConfig.h"
#include "helper.h"

#include "fe_cvdisplay.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

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

 void *ImgShow::ThreadItf(void *This)
{
    ((ImgShow*)This)->Thread();
}

void *ImgShow::Thread() {
    /// Open fifo
    mqd_t mqdes = mq_open(inQuName, O_RDWR);
    if (mqdes == (mqd_t) - 1) {
        perror("mq_open");
        assert(0);
    }

    while (1) {
        cvShowMsg_t request;
        int status = mq_receive(mqdes, (char *) &request, sizeof(request), 0);
        assert(status = sizeof(request));

        switch(request.mode )
        {
        case CV_IMAGE_DISPLAY:
            cv_display_frame(request.windowname, request.spec.width,
                               request.spec.height, request.data, request.spec.format,
                               request.scaling, request.seqNo, request.resultString);
            free(request.data);
            break;
        case CV_MAT_DISPLAY:
            cv_display_mat(request.windowname, request.data, request.seqNo);
            ((cv::Mat*)request.data)->release();
            break;
        default:
            assert(0);
        }

    }
}

void ImgShow::Start()
{
    int ret = pthread_create(&thread, NULL, ThreadItf, this);
    if (ret) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", ret);
        exit (EXIT_FAILURE);
    } else {
        printf("thread created\n");
    }
}

void ImgShow::Stop()
{
    pthread_join(thread, NULL);
}
