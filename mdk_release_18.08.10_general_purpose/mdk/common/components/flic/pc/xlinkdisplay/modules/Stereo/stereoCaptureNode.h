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

class stereoCaptureNode{
public:
    stereoCaptureNode(const char *inQuName)
    {
        unsigned int strlIn = strlen(inQuName) + 1;

        assert(strlIn <= MAX_LEN_NAME);

        memcpy(this->inQuName, inQuName, strlIn);

    }
    void Start();
    void Stop();

    static void *ThreadItf(void *This);
    void *Thread();
private:
    pthread_t thread;
    char inQuName[MAX_LEN_NAME];

};

