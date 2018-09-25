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
#include <assert.h>

/// Application Includes
/// -------------------------------------------------------------------------------------
#include "be_xlink.h"

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

void *be_xlink_thread(void*)
{
    int streamId;

    while((streamId = XLinkOpenStream(0, "plgout", 0)) == 0xDEADDEAD);//readonly
    printf("XLinkOpenStream done - stream Id %d\n", streamId);
    // try to open communication. Myriad is not yet booted, so this will fail
    streamPacketDesc_t * packet;

    int recieved_packets = 0;
    XLinkError_t status;
    while(status != X_LINK_COMMUNICATION_NOT_OPEN)
    {

        printf("*********************************\n");
        printf("*****iterations: %d \n", recieved_packets++);

        fflush(stdout);
        status = XLinkReadData(streamId, &packet);

        if (status == X_LINK_SUCCESS)
        {
            printf ( "read success\n" );
        }
        else
        {
            printf("myriad get data failed: %x\n", status);
        }
        printf("Data read size = %u\n", packet->length);

        status = XLinkReleaseData(streamId);
        if (status == 0)
        {
        }
        else
        {
            printf("release data failed: %x\n", status);
        }
        printf("*********************************\n");
    }
}
