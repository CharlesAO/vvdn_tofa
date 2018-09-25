/// =====================================================================================
///
///        @file:      simpleDisplayNv12.cpp
///        @brief:     
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

/// Application Includes
/// -------------------------------------------------------------------------------------
#include "be_xlink.h"
#include "fe_cvdisplay.h"

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------
#define SMNV12_WRITE_TO_FILE

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------
static unsigned img_w = 0;
static unsigned img_h = 0;
static unsigned write_to_file = 0;

/// Functions Implementation
/// -------------------------------------------------------------------------------------
void *smNv12xlinkReadThread(void*)
{
    FILE *fileptr;
    int streamId;

    while((streamId = XLinkOpenStream(0, "h265out", 0)) == 0xDEADDEAD);//readonly
    printf("XLinkOpenStream done - stream Id %d\n", streamId);

    streamPacketDesc_t * packet;

    if(write_to_file)
    {
        fileptr=fopen("output.yuv","wb+");
    }

    int iters = 0;
    XLinkError_t status;
    while(status != X_LINK_COMMUNICATION_NOT_OPEN)
    {
        printf("xlink - iterations: %d \n", iters++);

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

        cv_display_frame("simpleDisplayNv12", img_w, img_h, packet->data,
                        FRAME_FORMAT_NV12, 0.5, iters, NULL);

        if(write_to_file)
        {
            printf ( "writing to file\n" );
            fwrite(packet->data,1,packet->length, fileptr);
        }

        status = XLinkReleaseData(streamId);
        if (status == 0)
        {
        }
        else
        {
            printf("release data failed: %x\n", status);
        }
    }

    if(write_to_file)
    {
        fclose(fileptr);
    }
}

int simpleDisplayNv12(unsigned w, unsigned h, unsigned wr_to_file)
{
    img_w = w;
    img_h = h;
    write_to_file = wr_to_file;

    /// Init Xlink
    char name[32];
    XLinkError_t xerr = XLinkGetDeviceName(0, name, 32);
    if (!xerr)
    {
        printf("Device Found name %s \n", name);
    }
    else
    {
        printf("ERROR couldn't find devices rc %d\n", xerr);
        exit(1);
    }
    XLinkGlobalHandler_t ghandler;
    XLinkHandler_t handler =
    {
        .devicePath = name,
    };
    while(XLinkInitialize(&ghandler) != X_LINK_SUCCESS);
    printf("Initialize done\n");

    while(XLinkConnect(&handler) != X_LINK_SUCCESS);
    printf("XLinkConnect done - link Id %d\n", handler.linkId);

    pthread_t thread2;

    int ret = pthread_create( &thread2, NULL, smNv12xlinkReadThread, NULL);
    if(ret)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",ret);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf ( "thread created\n" );
    }

    pthread_join(thread2, NULL);

    return 0;
}
