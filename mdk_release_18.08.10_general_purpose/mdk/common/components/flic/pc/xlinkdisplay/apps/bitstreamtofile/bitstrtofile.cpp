/// =====================================================================================
///
///        @file:      bitstrtofile.cpp
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
#include "imgShow.h"
#include "helper.h"
#include <mqueue.h>

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

#define MAX_FILENAME_SIZE 60
#define MAX_STREAMS       5

#define JPEG_NAME_FORMAT "stillcapt_%Y%m%d_%H%M%S.jpeg"
#define BSTBIG_NAME_FORMAT "streamBig_%Y%m%d_%H%M%S.h264"
#define RIGHTMONO_NAME_FORMAT "rightMono_%Y%m%d_%H%M%S_1280x720_8BPP_GRAY.yuv"
#define LEFTMONO_NAME_FORMAT "leftMono_%Y%m%d_%H%M%S_1280x720_8BPP_GRAY.yuv"
#define DISPARITY_NAME_FORMAT "disparity_%Y%m%d_%H%M%S_1280x720_8BPP_GRAY.yuv"
#define VTRACK_NAME_FORMAT "vtrackmeta_%Y%m%d_%H%M%S.bin"

static float rightWarp[] = {1.0, 0.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 0.0, 1.0};

typedef struct{
    const char *channelname;
    const char *fileformat;
    bool still;
    bool savetofile;
}videoCtx_t;

int openfile(const char *format)
{
    static char name[MAX_FILENAME_SIZE];
    time_t now = time(0);
    strftime(name, sizeof(name), format, localtime(&now));
    return open(name, O_RDWR | O_CREAT, 0666);
}

void *streamtofileThread(void* arg)
{
    int streamId, fd;
    videoCtx_t *ctx = (videoCtx_t*)arg;

    while((streamId = XLinkOpenStream(0, ctx->channelname, 0)) == 0xDEADDEAD);//readonly
    printf("XLinkOpenStream done - stream Id %d\n", streamId);

    streamPacketDesc_t * packet;

    /// Open/create file
    if(ctx->savetofile && !ctx->still)
    {
        fd = openfile(ctx->fileformat);
        if(fd == -1)
        {
            perror("openfile");
         }
    }

    int iters = 0;
    XLinkError_t status;
    while(status != X_LINK_COMMUNICATION_NOT_OPEN)
    {
        fflush(stdout);
        status = XLinkReadData(streamId, &packet);

        if (status != X_LINK_SUCCESS)
        {
            printf("Chan #%d myriad get data failed: %x\n", streamId, status);
        }
        printf ( "StreamId #%d | Name %10s | Iter %6d | Packet size: %8u |\n",
                streamId, ctx->channelname, iters++, packet->length );

        if(ctx->savetofile && ctx->still)
            fd = openfile(ctx->fileformat);

        if(ctx->savetofile)
        {
            int ret_wr = write(fd, packet->data, packet->length);
            if(ret_wr != packet->length)
            {
                printf ( "Chan #%d Failed to write chunk!\n", streamId );
            }
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

    printf("Closing file \n");
    close(fd);
}

int bitstrtofile(char *calibfile, bool monotofile)
{
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

    if(calibfile)
    {
        int fd = open(calibfile, O_RDONLY);
        if(fd == -1) {
            perror ("open calib");
        }


        printf ( "Reading homography: %s\n", calibfile);
        int ret_read = read(fd, (char*)rightWarp, sizeof(rightWarp));
        if(ret_read <= 0){
            printf ( "failed to read\n" );
        }
    }

    // output channels
    int calibStream;
    while((calibStream = XLinkOpenStream(handler.linkId, "homography",sizeof(rightWarp))) == 0xDEADDEAD);//writeonly
    XLinkWriteData(calibStream, (const uint8_t*)&rightWarp[0], 9 * sizeof(float));
    XLinkCloseStream(calibStream);

    pthread_t thread[MAX_STREAMS];
    videoCtx_t vctx[] = {
        {
            .channelname = "h265out",
            .fileformat = BSTBIG_NAME_FORMAT,
            .still = false,
            .savetofile = true,
        },
        {
            .channelname = "rightMono",
            .fileformat = RIGHTMONO_NAME_FORMAT,
            .still = false,
            .savetofile = monotofile,
        },
        {
            .channelname = "leftMono",
            .fileformat = LEFTMONO_NAME_FORMAT,
            .still = false,
            .savetofile = monotofile,
        },
        {
            .channelname = "disparity",
            .fileformat = DISPARITY_NAME_FORMAT,
            .still = false,
            .savetofile = monotofile,
        },
        {
            .channelname = "jpegout",
            .fileformat = JPEG_NAME_FORMAT,
            .still = true,
            .savetofile = true,
        },
        {
            .channelname = "vtrack",
            .fileformat = VTRACK_NAME_FORMAT,
            .still = false,
            .savetofile = monotofile,
        },
    };

    int numstreams = sizeof(vctx)/sizeof(videoCtx_t);
    for(int stream = 0; stream < numstreams ; ++stream)
    {
        int ret = pthread_create( &thread[stream], NULL, streamtofileThread, (void*)&vctx[stream]);
        if(ret)
        {
            fprintf(stderr,"Error - pthread_create() return code: %d\n",ret);
            exit(EXIT_FAILURE);
        }
        else
        {
            printf ( "thread created\n" );
        }
    }

    for(int stream = 0; stream < numstreams ; ++stream)
        pthread_join(thread[stream], NULL);

    return 0;
}
