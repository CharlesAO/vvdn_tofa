/// =====================================================================================
///
///        @file:      be_xlink.h
///        @brief:     
///        @created:   11/13/2017 04:22:44 PM
///        @author:    csoka, attila.csok@movidius.com
///        @copyright: All code copyright Movidius Ltd 2013, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

/// System Includes
/// -------------------------------------------------------------------------------------
#include <pthread.h>
#include <string.h>
#include <assert.h>

#include <stdio.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <highgui.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/opencv.hpp>
/// Application Includes
/// -------------------------------------------------------------------------------------
#define MAX_LEN_NAME 32

#define DEFAULT_FRAME_WIDTH 1280
#define DEFAULT_FRAME_HEIGHT 720
/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------

/// Functions Implementation
/// -------------------------------------------------------------------------------------

class stereoPostProcNode{
public:
    stereoPostProcNode(const char *inQuName, const char *outQuName,
            int width = DEFAULT_FRAME_WIDTH,
            int height = DEFAULT_FRAME_HEIGHT)
    {
        unsigned int strlIn = strlen(inQuName) + 1;
        unsigned int strlOut = strlen(outQuName) + 1;

        assert(strlIn <= MAX_LEN_NAME);
        assert(strlOut <= MAX_LEN_NAME);

        memcpy(this->inQuName, inQuName, strlIn);
        memcpy(this->outQuName, outQuName, strlOut);

        this->frameWidth = width;
        this->frameHeight = height;
    }
    void Start();
    void Stop();

    static void *ThreadItf(void *This);
    void *Thread();
    void stretchDisparityRange(cv::Mat& frame, int disp);
private:
    pthread_t thread;
    char inQuName[MAX_LEN_NAME];
    char outQuName[MAX_LEN_NAME];
    int frameWidth;
    int frameHeight;
};
