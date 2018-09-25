///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Video demo streaming start
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "videodemo.h"

#include "udevice.h"
#include "protovideo.h"
#include "videoclientlib.h"
#include "uplatformapi.h"
#include "usbpumpdebug.h"
#include "usbpumplist.h"
#include "usbpump_types.h"
#include "uisobufhdr.h"
#include "usbpumplib.h"
#include "usbpumpapi.h"
#include "uhilalign.h"
#include "usbpump_application_rtems_api.h"
#include "usbpump_devspeed.h"
#include "usbpump_mdk_uplat_uhilaux.h"

#include "app_config.h"
#include "DrvLeonL2C.h"
#include <stdio.h>
#include <stdlib.h>
#include <bsp.h>
#include <rtems.h>
#include <pthread.h>
#include <semaphore.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define MAX_PACKETS_NUM 256

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern uint8_t inputFrame1[FRAME_SIZE_SS];
extern uint8_t inputFrame2[FRAME_SIZE_SS];
extern uint8_t inputFrame3[FRAME_SIZE_HS];
extern uint8_t inputFrame4[FRAME_SIZE_HS];

// 4: Static Local Data
// ----------------------------------------------------------------------------
static uint8_t *usbBufPtrSS[MAX_USED_BUF] = {inputFrame1, inputFrame1, inputFrame2, inputFrame2};
static uint8_t *usbBufPtrHS[MAX_USED_BUF] = {inputFrame3, inputFrame3, inputFrame4, inputFrame4};
static uint8_t headerBuffer[MAX_USED_BUF][PAYLOAD_HEADER_SIZE * MAX_PACKETS_NUM];
static USBPUMP_ISOCH_PACKET_DESCR isochDescriptors[MAX_USED_BUF][MAX_PACKETS_NUM];

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static UPROTO_VIDEO_STREAM_GET_INFO_CB_FN VideoDemo_StreamGetInfo_Callback;
static VOID VideoDemo_StartVideoStream(VIDEODEMO_CONTEXT *    pVideoDemoCtx);
static UPROTO_VIDEO_STREAM_WRITE_ISOCH_CB_FN VideoDemo_SendVideoData_Done;
static UPROTO_VIDEO_STREAM_WRITE_ISOCH_CB_FN VideoDemo_SendVideoData_Dummy;
static void *createPackets(void *pContext);
static void fillHeaderData(UINT8* headerBuffer, UINT32 headerSize,
        UINT32 numIsoFrames, BOOL setEof, void *pContext);
static void fillPayloadData(VIDEODEMO_BUFFER *pVideoBuf, UINT32 planeNum, BOOL toggle);

typedef VOID (*isochCback)(VOID *, UINT32, UINT8*, BYTES, USBPUMP_ISOCH_PACKET_DESCR*, BYTES);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

//Name:    VideoDemo_Start
//
//Function:
//    Start video demo
//
//Definition:
//    VOID
//    VideoDemo_Start(
//        VIDEODEMO_CONTEXT *    pVideoDemoCtx
//        );
//
//Description:
//    This function starts video demo application.
//
//Returns:
//    No explicit result.
VOID VideoDemo_Start(VIDEODEMO_CONTEXT *    pVideoDemoCtx)
{
    VideoClientLib_StreamGetInfo(
            pVideoDemoCtx->pVideo,
            VideoDemo_StreamGetInfo_Callback,
            pVideoDemoCtx,
            pVideoDemoCtx->hVideoOut
    );
}

static void startProcessingTask(void *pContext)
{
    pthread_attr_t attr;
    pthread_t thread;
    struct sched_param param;
    int sc;
    VIDEODEMO_CONTEXT *pVideoDemoCtx = pContext;

    sc = pthread_attr_init(&attr);
    if (sc)
    {
        printf("pthread_attr_init error");
        exit(sc);
    }
    sc = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if (sc)
    {
        printf("pthread_attr_setinheritsched error");
        exit(sc);
    }
    sc = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if (sc)
    {
        printf("pthread_attr_setschedpolicy error");
        exit(sc);
    }
    param.sched_priority = 10;
    sc = pthread_attr_setschedparam(&attr, &param);
    if(sc)
    {
        printf("set priority error\n");
        exit(sc);
    }
    sc = sem_init(&pVideoDemoCtx->packetSem, 0, 0);
    if (sc == -1)
    {
        perror("sem_init");
        exit(sc);
    }

    sc = pthread_create(&thread, &attr, createPackets, pContext);
    if (sc)
    {
        printf("Thread 1 creation failed: %d\n", sc);
        exit(sc);
    }
}

static int videoBufConfig(VIDEODEMO_BUFFER *pVideoBuf)
{
    UINT32 i;
    INT32 status;

    VideoFramePlaneConfig *planes = pVideoBuf->planes;
    VIDEODEMO_CONTEXT *pVideoDemoCtx = pVideoBuf->pVideoDemoCtx;
    UINT32 payloadSize = pVideoDemoCtx->payloadSize;
    UDEVICE *pDevice = pVideoDemoCtx->pDevice;

    if (USBPUMP_IS_SUPER_SPEED(pDevice->udev_bCurrentSpeed)) {
        planes[0].buffer = usbBufPtrSS[pVideoBuf->bufferIdx];
        planes[0].bufferSize = FRAME_SIZE_SS;
    }
    else {
        planes[0].buffer = usbBufPtrHS[pVideoBuf->bufferIdx];
        planes[0].bufferSize = FRAME_SIZE_HS;
    }

    for (i = 0; i < VIDEO_PLANES_NUM; i++)
    {
        planes[i].isoPacketsNum =
                (planes[i].bufferSize + payloadSize - 1) / payloadSize;
        planes[i].headerBufferSize = PAYLOAD_HEADER_SIZE * planes[i].isoPacketsNum;
        planes[i].headerBuffer = headerBuffer[i];
        planes[i].lastPacketSize = planes[i].bufferSize % payloadSize + PAYLOAD_HEADER_SIZE;

        // create buffer handle
        status = (*pDevice->udev_pUhilAuxInCall->InCall.pCreateHbufferWithSegmentFn)(
                    pDevice->udev_hUhilAux,
                    &planes[i].hBuffer,
                    planes[i].isoPacketsNum,
                    (ADDRBITS) planes[i].headerBuffer,
                    pVideoDemoCtx->payloadHeaderSize,
                    (ADDRBITS) planes[i].buffer,
                    pVideoDemoCtx->payloadSize,
                    USBPUMP_UHILAUX_BUFFER_ACCESS_READ
                );
        if (status != USBPUMP_UHILAUX_STATUS_OK)
        {
            printf("error creating buffer handle: %d\n", status);
            return -1;
        }
        planes[i].pIsochDescr = isochDescriptors[i];
    }

    return 0;
}

static VOID
VideoDemo_StreamGetInfo_Callback(
        VOID *    pCallbackCtx,
        UINT32    ErrorCode,
        UINT8     bInterfaceNumber,
        UINT8     bAlternateSetting,
        UINT8     bTerminalLink,
        UINT16    wMaxPacketSize,
        UINT16    wTransportHeaderSize
)
{
    VIDEODEMO_CONTEXT * CONST    pVideoDemoCtx = pCallbackCtx;

    USBPUMP_UNREFERENCED_PARAMETER(bInterfaceNumber);
    USBPUMP_UNREFERENCED_PARAMETER(bAlternateSetting);
    USBPUMP_UNREFERENCED_PARAMETER(bTerminalLink);

    if (ErrorCode != UPROTO_VIDEO_ERROR_OK)
    {
        printf("GetStreamInfo() fail (%s)\n",
                UsbPumpVideo_GetErrorName(ErrorCode));
    }
    else if (wTransportHeaderSize == 0)
    {
        printf("wTransportHeaderSize == 0, not isoch endpoint!\n");
    }
    else
    {
        pVideoDemoCtx->wVideoOutMaxPacketSize = wMaxPacketSize;
        pVideoDemoCtx->wTransportHeaderSize = wTransportHeaderSize;
        pVideoDemoCtx->headerToggle = 0;

        if (deviceAttached)
        {
            if (USBPUMP_IS_SUPER_SPEED(pVideoDemoCtx->pDevice->udev_bCurrentSpeed))
            {
                printf("USB 3.0 video device detected\n");
                printf("Streaming resolution: 2104x1560\n");
                printf("Expected frame rate 50fps\n");
            }
            else
            {
                printf("USB 2.0 video device detected\n");
                printf("Streaming resolution: 640x480\n");
                printf("Expected frame rate 40fps\n");
            }
            deviceAttached = 0;
        }
        VideoDemo_StartVideoStream(pVideoDemoCtx);
    }
}

static VOID VideoDemo_StartVideoStream(VIDEODEMO_CONTEXT *    pVideoDemoCtx)
{
    BYTES    payloadSize;
    VIDEODEMO_BUFFER *pVideoBuf = NULL;
    UINT    i;
    UDEVICE* pDevice = pVideoDemoCtx->pDevice;

    payloadSize = pVideoDemoCtx->wVideoOutMaxPacketSize - PAYLOAD_HEADER_SIZE;

    pVideoDemoCtx->isoPacketSize = pVideoDemoCtx->wVideoOutMaxPacketSize;
    pVideoDemoCtx->payloadHeaderSize = PAYLOAD_HEADER_SIZE;
    pVideoDemoCtx->payloadSize = payloadSize;

    if (USBPUMP_IS_SUPER_SPEED(pDevice->udev_bCurrentSpeed)) {
        pVideoDemoCtx->ProbeCur.dwMaxVideoFrameSize =
                pVideoDemoCtx->CommitCur.dwMaxVideoFrameSize =
                pVideoDemoCtx->ProbeMin.dwMaxVideoFrameSize =
                pVideoDemoCtx->CommitMin.dwMaxVideoFrameSize =
                pVideoDemoCtx->ProbeMax.dwMaxVideoFrameSize =
                pVideoDemoCtx->CommitMax.dwMaxVideoFrameSize =
                pVideoDemoCtx->ProbeDef.dwMaxVideoFrameSize =
                pVideoDemoCtx->CommitDef.dwMaxVideoFrameSize =
                        PROBE_DEFAULT_dwMaxVideoFrameSize;
    }
    else {
        pVideoDemoCtx->ProbeCur.dwMaxVideoFrameSize =
                pVideoDemoCtx->CommitCur.dwMaxVideoFrameSize =
                pVideoDemoCtx->ProbeMin.dwMaxVideoFrameSize =
                pVideoDemoCtx->CommitMin.dwMaxVideoFrameSize =
                pVideoDemoCtx->ProbeMax.dwMaxVideoFrameSize =
                pVideoDemoCtx->CommitMax.dwMaxVideoFrameSize =
                pVideoDemoCtx->ProbeDef.dwMaxVideoFrameSize =
                pVideoDemoCtx->CommitDef.dwMaxVideoFrameSize =
                        PROBE_DEFAULT_dwMaxVideoFrameSize_HS;
    }

    pVideoDemoCtx->ProbeCur.dwMaxPayloadTransferSize =
        pVideoDemoCtx->CommitCur.dwMaxPayloadTransferSize =
        pVideoDemoCtx->ProbeMin.dwMaxPayloadTransferSize =
        pVideoDemoCtx->CommitMin.dwMaxPayloadTransferSize =
        pVideoDemoCtx->ProbeMax.dwMaxPayloadTransferSize =
        pVideoDemoCtx->CommitMax.dwMaxPayloadTransferSize =
        pVideoDemoCtx->ProbeDef.dwMaxPayloadTransferSize =
        pVideoDemoCtx->CommitDef.dwMaxPayloadTransferSize = pVideoDemoCtx->wVideoOutMaxPacketSize;

    for (i = 0; i < MAX_FRAME_BUFFS; i++)
    {
        pVideoBuf = &pVideoDemoCtx->VideoBuffer[i];
        pVideoBuf->bufferIdx = i;
        pVideoBuf->pVideoDemoCtx = pVideoDemoCtx;
        videoBufConfig(pVideoBuf);
    }
    if (!pVideoDemoCtx->taskCreated)
    {
        pVideoDemoCtx->taskCreated = TRUE;
        startProcessingTask(pVideoDemoCtx);
    }
    // start the processing thread
    sem_post(&pVideoDemoCtx->packetSem);
}

void UsbIsoWriteCallback(VOID *ctx) {
    VIDEODEMO_BUFFER * pVideoBuf;
    UINT32 toggle;
    UINT32 i;
    VIDEODEMO_CONTEXT *pVideoDemoCtx = (VIDEODEMO_CONTEXT*)ctx;
    isochCback callback;

    pVideoBuf = &pVideoDemoCtx->VideoBuffer[pVideoDemoCtx->bufferIndex];
    toggle = FALSE;
    callback = VideoDemo_SendVideoData_Dummy;
    // send each plane separately
    for (i = 0; i < VIDEO_PLANES_NUM; i++)
    {
        if (i == VIDEO_PLANES_NUM - 1)
        {
            toggle = TRUE;
            callback = VideoDemo_SendVideoData_Done;
        }
        fillHeaderData(pVideoBuf->planes[i].headerBuffer,
                pVideoDemoCtx->payloadHeaderSize,
                pVideoBuf->planes[i].isoPacketsNum, TRUE, pVideoBuf);
        fillPayloadData(pVideoBuf, i, toggle);

        VideoClientLib_StreamWriteIsochV2(
                pVideoDemoCtx->pVideo,
                callback,
                pVideoBuf,
                pVideoDemoCtx->hVideoOut,
                pVideoBuf->planes[i].headerBuffer,
                pVideoDemoCtx->isoPacketSize * (pVideoBuf->planes[i].isoPacketsNum - 1)
                                + pVideoBuf->planes[i].lastPacketSize,
                pVideoBuf->planes[i].hBuffer,
                pVideoBuf->planes[i].pIsochDescr,
                pVideoBuf->planes[i].isoDescrSize
        );
    }
    pVideoDemoCtx->bufferIndex = (pVideoDemoCtx->bufferIndex + 1) % MAX_USED_BUF;
    //switch to the next buffer
    pVideoBuf = &pVideoDemoCtx->VideoBuffer[pVideoDemoCtx->bufferIndex];
}

static void *createPackets(void *pContext)
{
    VIDEODEMO_CONTEXT * pVideoDemoCtx = (VIDEODEMO_CONTEXT*)pContext;
    pVideoDemoCtx->bufferIndex = 0;

    USBPUMP_CALLBACKCOMPLETION_INIT(
            &pVideoDemoCtx->WriteCallback,
            UsbIsoWriteCallback,
            (void*)pVideoDemoCtx
            );

    while (1)
    {
        sem_wait(&pVideoDemoCtx->packetSem);
        UsbPumpPlatform_PostEvent(
                pVideoDemoCtx->pPlatform,
                &pVideoDemoCtx->WriteCallback
                );
    }
    return pContext;
}

static void fillHeaderData(UINT8* headerBuffer, UINT32 headerSize,
        UINT32 numIsoFrames, BOOL setEof, void *pContext)
{
    VIDEODEMO_BUFFER * CONST  pVideoBuf = pContext;
    VIDEODEMO_CONTEXT * CONST pVideoDemoCtx = pVideoBuf->pVideoDemoCtx;
    UINT i;

    for (i = 0; i < numIsoFrames; i++)
    {
        headerBuffer[UNCOMPRESSED_HDR_HLE_OFFSET] =
                pVideoDemoCtx->payloadHeaderSize;

        headerBuffer[UNCOMPRESSED_HDR_BFH_OFFSET] =
                UNCOMPRESSED_HDR_PTS + UNCOMPRESSED_HDR_SCR + UNCOMPRESSED_HDR_EOH +
                pVideoDemoCtx->headerToggle;

        pVideoDemoCtx->dwPTS += 1;
        pVideoDemoCtx->wSofCounter += 1;

        UHIL_LE_PUTUINT32(
                headerBuffer + UNCOMPRESSED_HDR_PTS_OFFSET,
                pVideoDemoCtx->dwPTS
        );
        UHIL_LE_PUTUINT32(
                headerBuffer + UNCOMPRESSED_HDR_SCR_STC_OFFSET,
                pVideoDemoCtx->dwPTS
        );
        UHIL_LE_PUTUINT16(
                headerBuffer + UNCOMPRESSED_HDR_SCR_SOF_OFFSET,
                pVideoDemoCtx->wSofCounter
        );
        if ((i == numIsoFrames - 1) && setEof)
        {
            headerBuffer[UNCOMPRESSED_HDR_BFH_OFFSET] += UNCOMPRESSED_HDR_EOF;
        }

        headerBuffer += headerSize;
    }
}

static void fillPayloadData(VIDEODEMO_BUFFER *pVideoBuf,
        UINT32 planeIdx, BOOL toggle)
{
    VIDEODEMO_CONTEXT * CONST pVideoDemoCtx = pVideoBuf->pVideoDemoCtx;
    BYTES        offset;
    USBPUMP_ISOCH_PACKET_DESCR *pIsochDescr;
    INT i;

    pIsochDescr = pVideoBuf->planes[planeIdx].pIsochDescr;

    offset = 0;
    for (i = 0; i < pVideoBuf->planes[planeIdx].isoPacketsNum - 1; i++)
    {
        pIsochDescr->uOffset = offset;
        offset += pVideoDemoCtx->isoPacketSize;
        pIsochDescr->usLength = pVideoDemoCtx->isoPacketSize;

        ++pIsochDescr;
    }
    // last packet may be shorter
    pIsochDescr->uOffset = offset;
    pIsochDescr->usLength = pVideoBuf->planes[planeIdx].lastPacketSize;
    if (toggle)
        pVideoDemoCtx->headerToggle = 1 - pVideoDemoCtx->headerToggle;
    ++pIsochDescr;

    pVideoBuf->planes[planeIdx].isoDescrSize = (UINT8*)pIsochDescr -
        (UINT8*)pVideoBuf->planes[planeIdx].pIsochDescr;
}

static VOID VideoDemo_SendVideoData_Dummy(
        VOID *               pContext,
        UINT32               ErrorCode,
        UINT8 *              pData,
        BYTES                nData,
        USBPUMP_ISOCH_PACKET_DESCR *    pIsochDescr,
        BYTES                IsochDescrSize
)
{
    USBPUMP_UNREFERENCED_PARAMETER(ErrorCode);
    USBPUMP_UNREFERENCED_PARAMETER(pContext);
    USBPUMP_UNREFERENCED_PARAMETER(pData);
    USBPUMP_UNREFERENCED_PARAMETER(nData);
    USBPUMP_UNREFERENCED_PARAMETER(pIsochDescr);
    USBPUMP_UNREFERENCED_PARAMETER(IsochDescrSize);
}

//Name:    VideoDemo_SendVideoData_Done
//
//Function:
//    The callback function for VideoDemo_SendVideoData()
//
//Definition:
//    VOID  VideoDemo_SendVideoData_Done(
//        VOID *                pContext,
//        UINT32                ErrorCode,
//        UINT8 *             pData,
//        BYTES                nData,
//        USBPUMP_ISOCH_PACKET_DESCR *    pIsochDescr,
//        BYTES                IsochDescrSize
//        );
//
//Description:
//    This is callback function for VideoDemo_SendVideoData() and it will
//    send a request to the camer driver to read video frame data.
//    If video stream is not activate or error code is NOTCFG, it will
//    free allocated buffer.
//
//Returns:
//    No explicit result.
static VOID VideoDemo_SendVideoData_Done(
        VOID *               pContext,
        UINT32               ErrorCode,
        UINT8 *              pData,
        BYTES                nData,
        USBPUMP_ISOCH_PACKET_DESCR *    pIsochDescr,
        BYTES                IsochDescrSize
)
{
    VIDEODEMO_BUFFER * CONST  pVideoBuf = pContext;
    VIDEODEMO_CONTEXT * CONST pVideoDemoCtx = pVideoBuf->pVideoDemoCtx;

    USBPUMP_UNREFERENCED_PARAMETER(pData);
    USBPUMP_UNREFERENCED_PARAMETER(nData);
    USBPUMP_UNREFERENCED_PARAMETER(pIsochDescr);
    USBPUMP_UNREFERENCED_PARAMETER(IsochDescrSize);

    if (ErrorCode != UPROTO_VIDEO_ERROR_OK)
    {
        TTUSB_OBJPRINTF((
                    &pVideoDemoCtx->pVideo->ObjectHeader,
                    UDMASK_ERRORS,
                    "?VideoDemo_SendVideoData_Done:"
                    " write fail %s\n",
                    UsbPumpVideo_GetErrorName(ErrorCode)
                    ));
    }

    if (! pVideoDemoCtx->fOutputActivate ||
            ErrorCode == UPROTO_VIDEO_ERROR_IO_NOTCFG ||
            ErrorCode == UPROTO_VIDEO_ERROR_INVALID_REQUEST)
    {
        return;
    }

    sem_post(&pVideoDemoCtx->packetSem);
}

