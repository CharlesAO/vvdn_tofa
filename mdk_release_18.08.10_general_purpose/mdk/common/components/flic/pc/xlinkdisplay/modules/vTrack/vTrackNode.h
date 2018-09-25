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

class vTrackNode{
public:
    vTrackNode(const char *inMetaQuName, const char *inFrameQuName, const char *outQuName)
    {
        unsigned int strlInM = strlen(inMetaQuName) + 1;
        unsigned int strlInF = strlen(inFrameQuName) + 1;
        unsigned int strlOut = strlen(outQuName) + 1;

        assert(strlInM <= MAX_LEN_NAME);
        assert(strlInF <= MAX_LEN_NAME);
        assert(strlOut <= MAX_LEN_NAME);

        memcpy(this->inMetaQuName, inMetaQuName, strlInM);
        memcpy(this->inFrameQuName, inFrameQuName, strlInF);
        memcpy(this->outQuName, outQuName, strlOut);
    }
    void Start();
    void Stop();

    static void *ThreadItf(void *This);
    void *Thread();

private:
    pthread_t thread;
    char inMetaQuName[MAX_LEN_NAME];
    char inFrameQuName[MAX_LEN_NAME];
    char outQuName[MAX_LEN_NAME];
};

