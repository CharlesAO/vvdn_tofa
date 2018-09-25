/// =====================================================================================
///
///        @file:      demo1.cpp
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
#include "fe_hevc.h"
#include "RecMq.h"
#include "RecFifo.h"
#include "Rec2File.h"
#include "imgShow.h"
#include "stereoPostProcNode.h"
#include "vTrackNode.h"
#include "helper.h"
#include <mqueue.h>

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------
static const char* h264_bitstream_file = "stream4k.h265";
static const char* fifo_prev_bitstream = "/tmp/bitstreamfifo_prev";
static const char* dirname = "Captures";
#define DISPLAY_MQ_NAME "/display"
#define STEREO_MQ_NAME "/stereo"
#define VTRACK_MQ_NAME "/vtrack"
#define VTRACK_MQ_FRM_NAME "/vtrackfrm"
#define PREVIEW_MQ_NAME "/preview"

static float rightWarp[] = {1.0, 0.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 0.0, 1.0};

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------

/// Functions Implementation
/// -------------------------------------------------------------------------------------
// #define JPEG_NAME_FORMAT "stillcapt_%Y%m%d_%H%M%S.jpeg"
// #define JPEG_NAME_SIZE 60

// static int openjpeg(void)
// {
//     static char name[JPEG_NAME_SIZE];
//     time_t now = time(0);
//     strftime(name, sizeof(name), JPEG_NAME_FORMAT, localtime(&now));
//     return open(name, O_RDWR | O_CREAT, 0666);
// }

static void *monoStreamThread(void* arg)
{
    int streamId;

    while((streamId = XLinkOpenStream(0, (const char*)arg, 0)) == 0xDEADDEAD);//readonly
    printf("XLinkOpenStream done - stream Id %d\n", streamId);

    streamPacketDesc_t * packet;

    mqd_t mqdesO = mq_open(DISPLAY_MQ_NAME, O_RDWR);
    if (mqdesO == (mqd_t) - 1) {
        perror("mq_open");
        assert(0);
    }

    int vtrackEnable = 0;
    if(strcmp( (const char*)arg, "rightMono") == 0 )
    {
        vtrackEnable = 1;
    }

    /// Mono for vtrack
    mqd_t mqdes1;
    if(vtrackEnable)
    {
        mqdes1 = mq_open(VTRACK_MQ_FRM_NAME, O_RDWR);
        if (mqdes1 == (mqd_t) - 1) {
            perror("mq_open");
            assert(0);
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
            printf("myriad get data failed: %x\n", status);
        }
         printf ( "StreamId #%d | Name %10s | Iter %6d | Packet size: %8u |\n",
                streamId, (const char*)arg, iters++, packet->length );
        uint8_t *buff = (uint8_t*)malloc(packet->length);
        memcpy(buff, packet->data, packet->length);

        cvShowMsg_t request;
        request.data = buff;
        request.windowname = (const char*)arg;
        request.spec.width = 1280;
        request.spec.height = 720;
        request.spec.format = FRAME_FORMAT_RAW8;
        request.scaling = 0.5;
        request.mode = CV_IMAGE_DISPLAY;
        request.seqNo = iters;
        int status = mq_send(mqdesO, (char *) &request, sizeof(cvShowMsg_t), 0);
        if (status) {
            perror("mq_send");
            assert(0);
        }

        /// Mono for vtrack
        if(vtrackEnable)
        {
            uint8_t *buffvtr = (uint8_t*)malloc(packet->length);
            memcpy(buffvtr, packet->data, packet->length);
            int status = mq_send(mqdes1, (char *) &buffvtr, sizeof(uint8_t*), 0);
            if (status) {
                perror("mq_send");
                assert(0);
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
}

static void *previewStreamThread(void* arg)
{
    uint32_t iters = 0;

    mqd_t mqdisp = mq_open(DISPLAY_MQ_NAME, O_RDWR);
    if (mqdisp == (mqd_t) - 1) {
        perror("mq_open");
        assert(0);
    }

    mqd_t mqstream = mq_open(PREVIEW_MQ_NAME, O_RDWR);
    if (mqstream == (mqd_t) - 1) {
        perror("mq_open");
        assert(0);
    }

    while(1)
    {
        uint8_t* buff;
        int status = mq_receive(mqstream, (char *) &buff, sizeof(uint8_t*), 0);
        assert(status = sizeof(uint8_t*));

        cvShowMsg_t request;
        request.data = buff;
        request.windowname = (const char*)arg;
        request.spec.width = 640;
        request.spec.height = 360;
        request.spec.format = FRAME_FORMAT_YUV420P;
        request.scaling = 1.0;
        request.mode = CV_IMAGE_DISPLAY;
        request.seqNo = iters++;
        status = mq_send(mqdisp, (char *) &request, sizeof(cvShowMsg_t), 0);
        if (status) {
            perror("mq_send");
            assert(0);
        }
    }
}

// }

int demo1(char *calibfile)
{
    int h264Stream, prevStream, jpegStream, dispStream, calibStream, vtrackStream;
    /// Init Xlink
    XLinkGlobalHandler_t glHandler = {.loglevel = 0, .profEnable = 0,  .protocol = USB_VSC};
    XLinkInitialize(&glHandler);

    char name[32];
    XLinkError_t rc = XLinkGetDeviceName(0, name, sizeof(name));
    if (rc != X_LINK_SUCCESS)
        printf("Could not get device name: %d \n\n", rc);



    XLinkHandler_t handler = { name, NULL, 0 };
    rc = X_LINK_COMMUNICATION_UNKNOWN_ERROR;
    while (rc != X_LINK_SUCCESS) {
        rc = XLinkConnect(&handler);
    }

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
    while((calibStream = XLinkOpenStream(handler.linkId, "homography",sizeof(rightWarp))) == 0xDEADDEAD);//writeonly
    printf("XLinkOpenStream done - stream Id %d\n", calibStream);
    printf("Writing homography\n");
    XLinkWriteData(calibStream, (const uint8_t*)&rightWarp[0], 9 * sizeof(float));
    XLinkCloseStream(calibStream);

    // input channels
    while((h264Stream = XLinkOpenStream(handler.linkId, "h265out", 0)) == 0xDEADDEAD);//readonly
    printf("XLinkOpenStream done - stream Id %d\n", h264Stream);
    while((jpegStream = XLinkOpenStream(handler.linkId, "jpegout", 0)) == 0xDEADDEAD);//readonly
    printf("XLinkOpenStream done - stream Id %d\n", jpegStream);
    while((prevStream = XLinkOpenStream(handler.linkId, "previewout", 0)) == 0xDEADDEAD);//readonly
    printf("XLinkOpenStream done - stream Id %d\n", prevStream);
    while((dispStream = XLinkOpenStream(handler.linkId, "disparity", 0)) == 0xDEADDEAD);//readonly
    printf("XLinkOpenStream done - stream Id %d\n", dispStream);
    while((vtrackStream = XLinkOpenStream(handler.linkId, "vtrack", 0)) == 0xDEADDEAD);//readonly
    printf("XLinkOpenStream done - stream Id %d\n", dispStream);

    createMsgQueue(DISPLAY_MQ_NAME, 10, sizeof(cvShowMsg_t));
    createMsgQueue(STEREO_MQ_NAME, 10, sizeof(uint8_t*));
    createMsgQueue(VTRACK_MQ_NAME, 10, sizeof(uint8_t*));
    createMsgQueue(VTRACK_MQ_FRM_NAME, 10, sizeof(uint8_t*));
    createMsgQueue(PREVIEW_MQ_NAME, 10, sizeof(uint8_t*));

    /// Creating file
    int fd = open(h264_bitstream_file, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if(fd == -1)
        perror("open h265 file");

    /// Creating fifo
    remove(fifo_prev_bitstream);
    printf("Creating fifo as %s\n", fifo_prev_bitstream);
    int mkret = mkfifo(fifo_prev_bitstream, 0666);
    if (mkret) {
        printf("Failed to create fifo \n");
    }

    RecFifo recH265(h264Stream, h264_bitstream_file);
    recH265.Start();

    Rec2File recJPEG(jpegStream, dirname);
    recJPEG.Start();

    RecMq stereoRec(dispStream, STEREO_MQ_NAME);
    stereoRec.Start();

    RecMq previewRec(prevStream, PREVIEW_MQ_NAME);
    previewRec.Start();

    RecMq vTrackRec(vtrackStream, VTRACK_MQ_NAME);
    vTrackRec.Start();

    stereoPostProcNode stereo(STEREO_MQ_NAME, DISPLAY_MQ_NAME);
    stereo.Start();

    vTrackNode vTrack(VTRACK_MQ_NAME, VTRACK_MQ_FRM_NAME, DISPLAY_MQ_NAME);
    vTrack.Start();

    ImgShow imgShow(DISPLAY_MQ_NAME);
    imgShow.Start();

    pthread_t thread0;
    int ret = pthread_create( &thread0, NULL, previewStreamThread, (void*)"preview");
    if(ret)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",ret);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf ( "preview thread created\n" );
    }

    const char *xlinkChNameMono[] = {
        "rightMono",
        "leftMono"
    };

    pthread_t thread1;
    ret = pthread_create( &thread1, NULL, monoStreamThread, (void*)xlinkChNameMono[0]);
    if(ret)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",ret);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf ( "thread created\n" );
    }

    pthread_t thread2;
    ret = pthread_create( &thread2, NULL, monoStreamThread, (void*)xlinkChNameMono[1]);
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

    recH265.Stop();
    recJPEG.Stop();

    imgShow.Stop();

    mq_unlink(DISPLAY_MQ_NAME);
    mq_unlink(VTRACK_MQ_NAME);
    mq_unlink(STEREO_MQ_NAME);

    /// Delete fifo
    printf ( "Deleting fifos\n" );
    remove(fifo_prev_bitstream);

    return 0;
}
