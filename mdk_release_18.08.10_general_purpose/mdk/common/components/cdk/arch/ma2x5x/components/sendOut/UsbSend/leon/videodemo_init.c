///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon file
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
#include "mv_types.h"
#include "OsDrvTimer.h"
#include "usbpump_application_rtems_api.h"
#include "usbpump_devspeed.h"
#include <assert.h>
#include <stdio.h>
#include "UsbSend.h"
#include "videodemo.h"
#include "usbpump_mdk_uplat_uhilaux.h"
#include "DrvTimer.h"

extern VIDEODEMO_CONTEXT *g_pVidemoDemoContext;

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
#ifdef DBG_USBSEND // define variables for calculation of frame time only if printed
tyTimeStamp testUsbFrmSendTs;
u64			testUsbFrmSendTicks = 0u;
#endif

#if USE_STATIC_FRAME_BUFFER
#define STATIC_FRAME_HEADER_SIZE        (1 * STATIC_FRAME_LINE_SIZE)
#define STATIC_FRAME_IMAGE_SIZE         (STATIC_FRAME_SIZE - STATIC_FRAME_HEADER_SIZE)

static uint8_t sendOutStaticFrame[STATIC_FRAME_SIZE + UVC_PAYLOAD_HEADER_OFFSET] __attribute__((aligned(8)));
static uint8_t *sendOutData = &sendOutStaticFrame[UVC_PAYLOAD_HEADER_OFFSET];

static void __attribute__((optimize("-O3"))) copy_frame_to_send(FrameT *frame)
{
    int row, col;
    int stride = frame->stride[0];
    register uint32_t *dst = (uint32_t *)sendOutData, val;
    register uint8_t *src;
    uint8_t *source_frame =  (uint8_t *)frame->fbPtr[0];

    rtems_cache_invalidate_multiple_data_lines(source_frame, (sizeof(*source_frame) * STATIC_FRAME_BUFFER_WIDTH  * STATIC_FRAME_BUFFER_HEIGHT));

    for (row = 0; row < STATIC_FRAME_BUFFER_HEIGHT; row++){
        src = source_frame;
        source_frame += stride;
        for (col = 0; col < (STATIC_FRAME_BUFFER_WIDTH/16); col++)
        {
            val = (uint32_t)src[0]|0x80008000;
            val |= ((uint32_t)src[1])<<16;
            dst[0] = val;
            val = (uint32_t)src[2]|0x80008000;
            val |= ((uint32_t)src[3])<<16;
            dst[1] = val;
            val = (uint32_t)src[4]|0x80008000;
            val |= ((uint32_t)src[5])<<16;
            dst[2] = val;
            val = (uint32_t)src[6]|0x80008000;
            val |= ((uint32_t)src[7])<<16;
            dst[3] = val;
            val = (uint32_t)src[8]|0x80008000;
            val |= ((uint32_t)src[9])<<16;
            dst[4] = val;
            val = (uint32_t)src[10]|0x80008000;
            val |= ((uint32_t)src[11])<<16;
            dst[5] = val;
            val = (uint32_t)src[12]|0x80008000;
            val |= ((uint32_t)src[13])<<16;
            dst[6] = val;
            val = (uint32_t)src[14]|0x80008000;
            val |= ((uint32_t)src[15])<<16;
            dst[7] = val;

            src+=16;
            dst+=8;
        }
    }
}
#endif // USE_STATIC_FRAME_BUFFER

// Sections decoration is required here for downstream tools

__TMS_TYPE_DEF_STRUCT(UNCOMPRESSED_APP_CONTEXT);

struct __TMS_STRUCTNAME(UNCOMPRESSED_APP_CONTEXT)
{
    VIDEODEMO_CONTEXT *    pVideoDemoCtx;

    // Presentation Time Stamp :
    // a source clock time in native device unit
    UINT32            dwPTS;

    // SOF Counter :
    // doesn't need to match the SOF token, Just a 1 kHz counter
    UINT16            wSofCounter;
    UINT8             HeaderToggle;
    UINT32            wVideoByteIndex;
    sUsbTest         *ptrFrm;

    UINT32            wPayloadSize;

    struct {
        UINT32            wNumberOfPayloads;
        UINT32            wPlSize[SEND_OUT_USB_NO_OF_PAYLOAD_MAX];  // contains size of slices
        UINT8             *pBuf;
        BOOL              fEndofFrame;
        UINT32            wPayloadIdx;
    } transaction;

};

// 4: Static Local Data
// ----------------------------------------------------------------------------
/* can be used on lrt to reconfigure frame source in case of usb2.0 */
static uint32_t UsbSend_CurrentSpeed = 0u;

/* USB headers defined locally */
static u8 UsbSend_UsbHdr[SEND_OUT_USB_NO_OF_PAYLOAD_MAX][PAYLOAD_HEADER_SIZE] = {{0,0}};
/* handlers used by USB _V2 API for buffer transfers */
USBPUMP_BUFFER_HANDLE UsbSend_UsbBufHndl[SEND_OUT_USB_NO_OF_PAYLOAD_MAX] = {0};

// Constant data

CONST UPROTO_VIDEO_CONFIG   gk_VideoDemo_ProtoConfig =
    UPROTO_VIDEO_CONFIG_INIT_V1(
        &gk_UsbPumpVideo_InSwitch,
        VIDEODEMO_CONFIG_CONTROLBUFFER_SIZE,
        VIDEODEMO_CONFIG_MAXQUEUED_BUFFER,
        0,  /* no timeout */
        0   /* no timeout */
        );


CONST TEXT * CONST gk_VideoDemo_ErrorName[] = UPROTO_VIDEO_ERROR__INIT;

CONST TEXT * CONST gk_VideoDemo_StatusName[] = UPROTO_VIDEO_STATUS__INIT;


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static UPROTO_VIDEO_CONTROL_OPEN_CB_FN    VideoClientLibI_ControlOpen_Callback;
static UPROTO_VIDEO_STREAM_OPEN_CB_FN     VideoClientLibI_StreamOpen_Callback;
static UPROTO_VIDEO_STREAM_GET_INFO_CB_FN VideoClientLibI_StreamGetInfo_Callback;
static UPROTO_VIDEO_STREAM_CLOSE_CB_FN    VideoClientLibI_StreamClose_Done;
static UPROTO_VIDEO_STREAM_WRITE_CB_FN    VideoDemo_WriteOneFrame_Done;

static CALLBACKFN    VideoClientLibI_StreamOpen_Sync;

static void makeHeader(PUNCOMPRESSED_APP_CONTEXT CONST pAppContext, u8* pBuffer, BOOL eof);
static void processBuffer(PUNCOMPRESSED_APP_CONTEXT CONST pAppContext, u8 *buf_ptr, const u32 buf_size, const u32 payload_size, UINT32 *no_of_payloads);
static void makePayload(PUNCOMPRESSED_APP_CONTEXT CONST pAppContext, const u32 index, const u8 *pPayload, const u32 size);


// 6: Functions Implementation
// ----------------------------------------------------------------------------


// Name:    VideoDemo_UncompressedDemoInit()
//
// Function:
//     Application instance initialization function, called by VideoDemo_ClientCreate.
//
// Definition:
//     VIDEODEMO_CONTEXT * VideoDemo_UncompressedDemoInit(
//         UPLATFORM *        pPlatform,
//         USBPUMP_OBJECT_HEADER *    pFunctionObject
//         );
//
// Description:
//     This module will allocate memory needed for application instance, give the
//     initial values to application context variables and send Open command to
//     protocol layer.
//
// Returns:
//     pointer to application context.


VIDEODEMO_CONTEXT * VideoDemo_UncompressedDemoInit(
        UPLATFORM *pPlatform,
        USBPUMP_OBJECT_HEADER *pFunctionObject)
{
    PUNCOMPRESSED_APP_CONTEXT        pAppContext;
    VIDEODEMO_CONTEXT        *pVideoDemoCtx;

    UPROTO_VIDEO * CONST        pVideo = __TMS_CONTAINER_OF(
            pFunctionObject,
            UPROTO_VIDEO,
            ObjectHeader
    );

    // Allocate videodemo application context
    pAppContext = UsbPumpPlatform_MallocZero(pPlatform, sizeof(*pAppContext));

    if (pAppContext == NULL)
    {
        TTUSB_OBJPRINTF((&pVideo->ObjectHeader, UDMASK_ERRORS,
                "?VideoDemo_UncompressedDemoInit:"
                " pAppContext allocation fail\n"
        ));

        return NULL;
    }

    // Allocate videodemo context

    pVideoDemoCtx = UsbPumpPlatform_MallocZero(pPlatform, sizeof(*pVideoDemoCtx));

    if (pVideoDemoCtx == NULL)
    {
        UsbPumpPlatform_Free(
                pPlatform,
                pAppContext,
                sizeof(*pAppContext)
        );

        TTUSB_OBJPRINTF((&pVideo->ObjectHeader, UDMASK_ERRORS,
                "?VideoDemo_UncompressedDemoInit:"
                " pVideoDemoCtx allocation fail\n"
        ));
        return NULL;
    }

    pAppContext->pVideoDemoCtx = pVideoDemoCtx;
    pAppContext->dwPTS = 0;
    pAppContext->wSofCounter = 0;

    pVideoDemoCtx->pVideo = pVideo;
    pVideoDemoCtx->pPlatform = pPlatform;
    pVideoDemoCtx->pAppContext = pAppContext;
    pVideoDemoCtx->hVideoIn = UPROTO_VIDEO_STREAM_INVALID_HANDLE;
    pVideoDemoCtx->hVideoOut = UPROTO_VIDEO_STREAM_INVALID_HANDLE;

    // current setting initialization of Processing Unit
    pVideoDemoCtx->ProcUnitCur.wBackLightCompensation =
            PU_BACKLIGHT_COMPENSATION_CONTROL_DEF;
    pVideoDemoCtx->ProcUnitCur.wBrightness =
            PU_BRIGHTNESS_CONTROL_DEF;
    pVideoDemoCtx->ProcUnitCur.wContrast =
            PU_CONTRAST_CONTROL_DEF;
    pVideoDemoCtx->ProcUnitCur.bPowerLineFrequency =
            PU_POWER_LINE_FREQUENCY_CONTROL_DEF;
    pVideoDemoCtx->ProcUnitCur.wHue =
            PU_HUE_CONTROL_DEF;
    pVideoDemoCtx->ProcUnitCur.wSaturation =
            PU_SATURATION_CONTROL_DEF;
    pVideoDemoCtx->ProcUnitCur.wSharpness =
            PU_SHARPNESS_CONTROL_DEF;
    pVideoDemoCtx->ProcUnitCur.wGamma =
            PU_GAMMA_CONTROL_DEF;
    pVideoDemoCtx->ProcUnitCur.wWhiteBalanceTemperature =
            PU_WHITE_BALANCE_TEMPERATURE_CONTROL_DEF;
    pVideoDemoCtx->ProcUnitCur.bHueAuto =
            PU_HUE_AUTO_CONTROL_DEF;
    pVideoDemoCtx->ProcUnitCur.bWhiteBalanceTemperatureAuto =
            PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL_DEF;

    // current setting initialization of Probe/Commit
    pVideoDemoCtx->ProbeCur.bmHint =
            pVideoDemoCtx->CommitCur.bmHint =
            pVideoDemoCtx->ProbeMin.bmHint =
            pVideoDemoCtx->CommitMin.bmHint =
            pVideoDemoCtx->ProbeMax.bmHint =
            pVideoDemoCtx->CommitMax.bmHint =
            pVideoDemoCtx->ProbeDef.bmHint =
            pVideoDemoCtx->CommitDef.bmHint =
                    PROBE_DEFAULT_bmHint;
    pVideoDemoCtx->ProbeCur.bFormatIndex =
            pVideoDemoCtx->CommitCur.bFormatIndex =
            pVideoDemoCtx->ProbeMin.bFormatIndex  =
            pVideoDemoCtx->CommitMin.bFormatIndex =
            pVideoDemoCtx->ProbeMax.bFormatIndex  =
            pVideoDemoCtx->CommitMax.bFormatIndex =
            pVideoDemoCtx->ProbeDef.bFormatIndex  =
            pVideoDemoCtx->CommitDef.bFormatIndex =
                    PROBE_DEFAULT_bFormatIndex;
    pVideoDemoCtx->ProbeCur.bFrameIndex =
            pVideoDemoCtx->CommitCur.bFrameIndex =
            pVideoDemoCtx->ProbeMin.bFrameIndex =
            pVideoDemoCtx->CommitMin.bFrameIndex =
            pVideoDemoCtx->ProbeMax.bFrameIndex =
            pVideoDemoCtx->CommitMax.bFrameIndex =
            pVideoDemoCtx->ProbeDef.bFrameIndex =
            pVideoDemoCtx->CommitDef.bFrameIndex =
                    PROBE_DEFAULT_bFrameIndex;
    pVideoDemoCtx->ProbeCur.dwFrameInterval =
            pVideoDemoCtx->CommitCur.dwFrameInterval =
            pVideoDemoCtx->ProbeMin.dwFrameInterval =
            pVideoDemoCtx->CommitMin.dwFrameInterval =
            pVideoDemoCtx->ProbeMax.dwFrameInterval =
            pVideoDemoCtx->CommitMax.dwFrameInterval =
            pVideoDemoCtx->ProbeDef.dwFrameInterval =
            pVideoDemoCtx->CommitDef.dwFrameInterval =
                    PROBE_DEFAULT_dwFrameInterval;
    pVideoDemoCtx->ProbeCur.wKeyFrameRate =
            pVideoDemoCtx->CommitCur.wKeyFrameRate =
            pVideoDemoCtx->ProbeMin.wKeyFrameRate =
            pVideoDemoCtx->CommitMin.wKeyFrameRate =
            pVideoDemoCtx->ProbeMax.wKeyFrameRate =
            pVideoDemoCtx->CommitMax.wKeyFrameRate =
            pVideoDemoCtx->ProbeDef.wKeyFrameRate =
            pVideoDemoCtx->CommitDef.wKeyFrameRate =
                    PROBE_DEFAULT_wKeyFrameRate;
    pVideoDemoCtx->ProbeCur.wPFrameRate =
            pVideoDemoCtx->CommitCur.wPFrameRate =
            pVideoDemoCtx->ProbeMin.wPFrameRate =
            pVideoDemoCtx->CommitMin.wPFrameRate =
            pVideoDemoCtx->ProbeMax.wPFrameRate =
            pVideoDemoCtx->CommitMax.wPFrameRate =
            pVideoDemoCtx->ProbeDef.wPFrameRate =
            pVideoDemoCtx->CommitDef.wPFrameRate =
                    PROBE_DEFAULT_wPFrameRate;
    pVideoDemoCtx->ProbeCur.wCompQuality =
            pVideoDemoCtx->CommitCur.wCompQuality =
            pVideoDemoCtx->ProbeMin.wCompQuality =
            pVideoDemoCtx->CommitMin.wCompQuality =
            pVideoDemoCtx->ProbeMax.wCompQuality =
            pVideoDemoCtx->CommitMax.wCompQuality =
            pVideoDemoCtx->ProbeDef.wCompQuality =
            pVideoDemoCtx->CommitDef.wCompQuality =
                    PROBE_DEFAULT_wCompQuality;
    pVideoDemoCtx->ProbeCur.wCompWindowSize =
            pVideoDemoCtx->CommitCur.wCompWindowSize =
            pVideoDemoCtx->ProbeMin.wCompWindowSize =
            pVideoDemoCtx->CommitMin.wCompWindowSize =
            pVideoDemoCtx->ProbeMax.wCompWindowSize =
            pVideoDemoCtx->CommitMax.wCompWindowSize =
            pVideoDemoCtx->ProbeDef.wCompWindowSize =
            pVideoDemoCtx->CommitDef.wCompWindowSize =
                    PROBE_DEFAULT_wCompWindowSize;
    pVideoDemoCtx->ProbeCur.wDelay =
            pVideoDemoCtx->CommitCur.wDelay =
            pVideoDemoCtx->ProbeMin.wDelay =
            pVideoDemoCtx->CommitMin.wDelay =
            pVideoDemoCtx->ProbeMax.wDelay =
            pVideoDemoCtx->CommitMax.wDelay =
            pVideoDemoCtx->ProbeDef.wDelay =
            pVideoDemoCtx->CommitDef.wDelay =
                    PROBE_DEFAULT_wDelay;
    pVideoDemoCtx->ProbeCur.dwMaxVideoFrameSize =
            pVideoDemoCtx->CommitCur.dwMaxVideoFrameSize =
            pVideoDemoCtx->ProbeMin.dwMaxVideoFrameSize =
            pVideoDemoCtx->CommitMin.dwMaxVideoFrameSize =
            pVideoDemoCtx->ProbeMax.dwMaxVideoFrameSize =
            pVideoDemoCtx->CommitMax.dwMaxVideoFrameSize =
            pVideoDemoCtx->ProbeDef.dwMaxVideoFrameSize =
            pVideoDemoCtx->CommitDef.dwMaxVideoFrameSize =
                    PROBE_DEFAULT_dwMaxVideoFrameSize;
    pVideoDemoCtx->ProbeCur.dwMaxPayloadTransferSize =
            pVideoDemoCtx->CommitCur.dwMaxPayloadTransferSize =
            pVideoDemoCtx->ProbeMin.dwMaxPayloadTransferSize =
            pVideoDemoCtx->CommitMin.dwMaxPayloadTransferSize =
            pVideoDemoCtx->ProbeMax.dwMaxPayloadTransferSize =
            pVideoDemoCtx->CommitMax.dwMaxPayloadTransferSize =
            pVideoDemoCtx->ProbeDef.dwMaxPayloadTransferSize =
            pVideoDemoCtx->CommitDef.dwMaxPayloadTransferSize =
                    PROBE_DEFAULT_dwMaxPayloadTransferSize;
    pVideoDemoCtx->ProbeCur.dwClockFrequency =
            pVideoDemoCtx->CommitCur.dwClockFrequency =
            pVideoDemoCtx->ProbeMin.dwClockFrequency =
            pVideoDemoCtx->CommitMin.dwClockFrequency =
            pVideoDemoCtx->ProbeMax.dwClockFrequency =
            pVideoDemoCtx->CommitMax.dwClockFrequency =
            pVideoDemoCtx->ProbeDef.dwClockFrequency =
            pVideoDemoCtx->CommitDef.dwClockFrequency =
                    PROBE_DEFAULT_dwClockFrequency;
    pVideoDemoCtx->ProbeCur.bmFramingInfo =
            pVideoDemoCtx->CommitCur.bmFramingInfo =
            pVideoDemoCtx->ProbeMin.bmFramingInfo =
            pVideoDemoCtx->CommitMin.bmFramingInfo =
            pVideoDemoCtx->ProbeMax.bmFramingInfo =
            pVideoDemoCtx->CommitMax.bmFramingInfo =
            pVideoDemoCtx->ProbeDef.bmFramingInfo =
            pVideoDemoCtx->CommitDef.bmFramingInfo =
                    PROBE_DEFAULT_bmFramingInfo;
    pVideoDemoCtx->ProbeCur.bPreferedVersion =
            pVideoDemoCtx->CommitCur.bPreferedVersion =
            pVideoDemoCtx->ProbeMin.bPreferedVersion =
            pVideoDemoCtx->CommitMin.bPreferedVersion =
            pVideoDemoCtx->ProbeMax.bPreferedVersion =
            pVideoDemoCtx->CommitMax.bPreferedVersion =
            pVideoDemoCtx->ProbeDef.bPreferedVersion =
            pVideoDemoCtx->CommitDef.bPreferedVersion =
                    PROBE_DEFAULT_bPreferedVersion;
    pVideoDemoCtx->ProbeCur.bMinVersion =
            pVideoDemoCtx->CommitCur.bMinVersion =
            pVideoDemoCtx->ProbeMin.bMinVersion =
            pVideoDemoCtx->CommitMin.bMinVersion =
            pVideoDemoCtx->ProbeMax.bMinVersion =
            pVideoDemoCtx->CommitMax.bMinVersion =
            pVideoDemoCtx->ProbeDef.bMinVersion =
            pVideoDemoCtx->CommitDef.bMinVersion =
                    PROBE_DEFAULT_bMinVersion;
    pVideoDemoCtx->ProbeCur.bMaxVersion =
            pVideoDemoCtx->CommitCur.bMaxVersion =
            pVideoDemoCtx->ProbeMin.bMaxVersion =
            pVideoDemoCtx->CommitMin.bMaxVersion =
            pVideoDemoCtx->ProbeMax.bMaxVersion =
            pVideoDemoCtx->CommitMax.bMaxVersion =
            pVideoDemoCtx->ProbeDef.bMaxVersion =
            pVideoDemoCtx->CommitDef.bMaxVersion =
                    PROBE_DEFAULT_bMaxVersion;

    // current setting initialization of Still Probe/Commit
    pVideoDemoCtx->StillProbeCur.bFormatIndex =
            pVideoDemoCtx->StillCommitCur.bFormatIndex =
                    STILL_PROBE_DEFAULT_bFormatIndex;
    pVideoDemoCtx->StillProbeCur.bFrameIndex =
            pVideoDemoCtx->StillCommitCur.bFrameIndex =
                    STILL_PROBE_DEFAULT_bFrameIndex;
    pVideoDemoCtx->StillProbeCur.bCompressionIndex =
            pVideoDemoCtx->StillCommitCur.bCompressionIndex =
                    STILL_PROBE_DEFAULT_bCompressionIndex;
    pVideoDemoCtx->StillProbeCur.dwMaxVideoFrameSize =
            pVideoDemoCtx->StillCommitCur.dwMaxVideoFrameSize =
                    STILL_PROBE_DEFAULT_dwMaxVideoFrameSize;
    pVideoDemoCtx->StillProbeCur.dwMaxPayloadTransferSize =
            pVideoDemoCtx->StillCommitCur.dwMaxPayloadTransferSize =
                    STILL_PROBE_DEFAULT_dwMaxPayLoadTransferSize;

    pVideoDemoCtx->bStillTrigger =     STILL_IMAGE_NORMAL_OPERATION;
    pVideoDemoCtx->nVideoFrameIndex = START_VIDEO_FRAME_INDEX;

    // initialize callback completion
    USBPUMP_CALLBACKCOMPLETION_INIT(
            &pVideoDemoCtx->InitCompletion,
            VideoClientLibI_StreamOpen_Sync,
            NULL
    );


    // Open video control
    VideoClientLib_ControlOpen(
            pVideoDemoCtx->pVideo,
            VideoClientLibI_ControlOpen_Callback,
            pAppContext,
            pVideoDemoCtx,
            &gk_VideoDemo_OutSwitch
    );

    USBPUMP_CALLBACKCOMPLETION_INIT(
            &pVideoDemoCtx->WriteCallback,
            VideoDemo_WriteOneFrame,
            (void*)pVideoDemoCtx
            );

    return pVideoDemoCtx;
}

static void VideoClientLibI_ControlOpen_Callback(void *    pCallbackCtx, UINT32 ErrorCode)
{
    PUNCOMPRESSED_APP_CONTEXT CONST    pAppContext = pCallbackCtx;
    VIDEODEMO_CONTEXT * CONST    pVideoDemoCtx = pAppContext->pVideoDemoCtx;

    if (ErrorCode != UPROTO_VIDEO_ERROR_OK)
    {
        TTUSB_OBJPRINTF((&pVideoDemoCtx->pVideo->ObjectHeader, UDMASK_ERRORS,
                "?VideoClientLib_ControlOpen_Sync:"
                " ControlOpen() fail (%s)\n",
                gk_VideoDemo_ErrorName[ErrorCode]
        ));
    }
    else
    {
        UsbPumpPlatform_PostIfNotBusy(
                pVideoDemoCtx->pPlatform,
                &pVideoDemoCtx->InitCompletion,
                pAppContext
        );
    }
}

static void VideoClientLibI_StreamOpen_Sync(void *pClientContext)
{
    PUNCOMPRESSED_APP_CONTEXT CONST    pAppContext = pClientContext;
    VIDEODEMO_CONTEXT *    pVideoDemoCtx = pAppContext->pVideoDemoCtx;

    // Open video output stream
    VideoClientLib_StreamOpen(
            pVideoDemoCtx->pVideo,
            VideoClientLibI_StreamOpen_Callback,
            pAppContext,
            (INT8)(-1),    // InterfaceNumber
            (INT8)(-1),    // AlternateSetting
            (INT8)(-1),    // TerminalLink
            FALSE        // fTrueIfVideoInput
    );
}

static void VideoClientLibI_StreamOpen_Callback(
        void *    pCallbackCtx,
        UINT32    ErrorCode,
        UPROTO_VIDEO_STREAM_HANDLE    hVideoStream
)
{
    PUNCOMPRESSED_APP_CONTEXT CONST    pAppContext = pCallbackCtx;
    VIDEODEMO_CONTEXT *        pVideoDemoCtx = pAppContext->pVideoDemoCtx;

    if (ErrorCode != UPROTO_VIDEO_ERROR_OK)
    {
        TTUSB_OBJPRINTF((&pVideoDemoCtx->pVideo->ObjectHeader, UDMASK_ERRORS,
                "?VideoClientLibI_StreamOpen_Callback:"
                " StreamOpen() fail (%s)\n",
                gk_VideoDemo_ErrorName[ErrorCode]
        ));
    }
    else
    {
        pVideoDemoCtx->hVideoOut = hVideoStream;
    }
}

void VideoDemo_Start(VIDEODEMO_CONTEXT *    pVideoDemoCtx)
{
    VideoClientLib_StreamGetInfo(
            pVideoDemoCtx->pVideo,
            VideoClientLibI_StreamGetInfo_Callback,
            pVideoDemoCtx->pAppContext,
            pVideoDemoCtx->hVideoOut
    );
}

static void VideoClientLibI_StreamGetInfo_Callback(
        void *    pCallbackCtx,
        UINT32    ErrorCode,
        UINT8     bInterfaceNumber,
        UINT8     bAlternateSetting,
        UINT8     bTerminalLink,
        UINT16    wMaxPacketSize,
        UINT16    wTransportHeaderSize
)
{
    (void) wMaxPacketSize;// "use" the variables to hush the compiler warning.
    (void) wTransportHeaderSize;// "use" the variables to hush the compiler warning.

    PUNCOMPRESSED_APP_CONTEXT CONST    pAppContext = pCallbackCtx;
    VIDEODEMO_CONTEXT * CONST    pVideoDemoCtx = pAppContext->pVideoDemoCtx;
    UPROTO_VIDEO * CONST        pVideo = pAppContext->pVideoDemoCtx->pVideo;
    BOOL fResult;

    USBPUMP_TRACE_PARAMETER(pVideo);
    USBPUMP_UNREFERENCED_PARAMETER(bInterfaceNumber);
    USBPUMP_UNREFERENCED_PARAMETER(bAlternateSetting);
    USBPUMP_UNREFERENCED_PARAMETER(bTerminalLink);

    fResult = TRUE;

    if (ErrorCode != UPROTO_VIDEO_ERROR_OK)
    {
        TTUSB_OBJPRINTF((&pVideo->ObjectHeader, UDMASK_ERRORS,
                "?VideoClientLi_StreamGetInfo_Callback:"
                " GetStreamInfo() fail (%s)\n",
                gk_VideoDemo_ErrorName[ErrorCode]
        ));

        fResult = FALSE;
    }

    if (fResult)
    {
        pAppContext->HeaderToggle    = 0;
        pAppContext->wVideoByteIndex = 0;
        pAppContext->wPayloadSize   = pVideoDemoCtx->ProbeCur.dwMaxPayloadTransferSize - PAYLOAD_HEADER_SIZE;

        UDEVICE* pDevice = UsbPump_Rtems_DataPump_GetDevice();
        if (USBPUMP_IS_SUPER_SPEED(pDevice->udev_bCurrentSpeed))
            UsbSend_CurrentSpeed = USB_SUPER_SPEED;
        else
            UsbSend_CurrentSpeed = USB_HIGH_SPEED;

        printf ("Get Stream Info: %s \n", (UsbSend_CurrentSpeed==USB_SUPER_SPEED)?"USB_SUPER_SPEED":"USB_HIGH_SPEED");
    }
    else
    {
        if (pVideoDemoCtx->hVideoOut !=
                UPROTO_VIDEO_STREAM_INVALID_HANDLE)
        {
            VideoClientLib_StreamClose(
                    pVideoDemoCtx->pVideo,
                    VideoClientLibI_StreamClose_Done,
                    pVideoDemoCtx,
                    pVideoDemoCtx->hVideoOut
            );
        }

        if (pVideoDemoCtx)
        {
            UsbPumpPlatform_Free(
                    pVideoDemoCtx->pPlatform,
                    pVideoDemoCtx,
                    sizeof(*pVideoDemoCtx)
            );
        }
    }
}

static void VideoClientLibI_StreamClose_Done(void   *pClientContext, UINT32 ErrorCode)
{
    USBPUMP_UNREFERENCED_PARAMETER(pClientContext);
    USBPUMP_UNREFERENCED_PARAMETER(ErrorCode);
}


static void makeHeader(PUNCOMPRESSED_APP_CONTEXT CONST pAppContext, u8* pBuffer, BOOL eof)
{
    // Process Header Fields
    pBuffer[UNCOMPRESSED_HDR_HLE_OFFSET] = PAYLOAD_HEADER_SIZE;

    pBuffer[UNCOMPRESSED_HDR_BFH_OFFSET] =
            UNCOMPRESSED_HDR_PTS +
            UNCOMPRESSED_HDR_SCR +
            pAppContext->HeaderToggle;

    UHIL_LE_PUTUINT32(pBuffer + UNCOMPRESSED_HDR_PTS_OFFSET, pAppContext->dwPTS);
    UHIL_LE_PUTUINT32(pBuffer + UNCOMPRESSED_HDR_SCR_STC_OFFSET, pAppContext->dwPTS);
    UHIL_LE_PUTUINT16(pBuffer + UNCOMPRESSED_HDR_SCR_SOF_OFFSET, pAppContext->wSofCounter);

    if(eof == TRUE)
    {
        pBuffer[UNCOMPRESSED_HDR_BFH_OFFSET] += UNCOMPRESSED_HDR_EOF;
    }
}


// Name:    
//
// In parameters:
//      pAppContext - pointer to context
//      index       - index of slice (for transmission on bus)
//      pPayload    - pointer to data buffer
//      size        - size of data buffer
//
// Description:
//      creates USB driver related header and buffer handler for a slice
//      Output (side effect):
//          - header and buffer handler are stored in  global variables: UsbSend_UsbHdr[][] and UsbSend_UsbBufHndl[index];
//          - size of slice it's stored in app context pAppContext->transaction.wPlSize[]
//
// Returns:
//      none

static void makePayload(PUNCOMPRESSED_APP_CONTEXT CONST pAppContext, const u32 index, const u8 *pPayload, const u32 size)
{
    u8 *pHeader;
    USBPUMP_BUFFER_HANDLE *pBufHndl;
    u32 status;

    assert(index < (SEND_OUT_USB_NO_OF_PAYLOAD_MAX - 1)); // if failed, increase SEND_OUT_USB_NO_OF_PAYLOAD_MAX

    pHeader = &UsbSend_UsbHdr[index][0];
    pBufHndl = &UsbSend_UsbBufHndl[index];

    pAppContext->transaction.wPlSize[index] = size;

    makeHeader(pAppContext, pHeader, FALSE);

    DBG_PRINTF("cre bh %lu ", index);

    status = UsbPumpMdkUhilAux_HbufferCreate(
                    pAppContext->pVideoDemoCtx->pVideo->pDevice->udev_hUhilAux, //hSession,
                    pBufHndl,                           //phBuffer,
                    1,                                  //numSegments,
                    (ADDRBITS) (pHeader),               //vaBuffer,
                    PAYLOAD_HEADER_SIZE,                //nSegBuffer,
                    (ADDRBITS) pPayload,                //vaBuffer2,
                    size,                               //nSegBuffer2,
                    USBPUMP_UHILAUX_BUFFER_ACCESS_READ  //requestedAccess
                    );
    if (status != USBPUMP_UHILAUX_STATUS_OK)
    {
        printf("error %lu\n", status);
    }
    else
    {
        DBG_PRINTF("@ 0x%8lx...OK\n", (u32) UsbSend_UsbBufHndl[index]);
    }
}


// Name: processBuffer
//
// In parameters:
//      pAppContext     - pointer to context
//      buf_ptr         - pointer to data buffer
//      buf_size        - size of data buffer
//      payload_size    - size of payload
//
// In-Out parameters:
//      no_of_payloads  - pointer to the global number of slices needed to send the frame (total counter).
//
// Description:
//      split the buffer in slices of payload size; last slice may be smaller.
//      no_of_payloads at call time is the number of slices from the previously processed buffers in current frame.
//      no_of_payloads it's used when calling makePayload as offset in global arrays for headers and buffer handlers.
//      makePayload creates for each slice the header & buffer handler.
//      no_of_payloads it's updated before return with the number of slices from the current buffer.
//
// Returns:
//      none

static void processBuffer(PUNCOMPRESSED_APP_CONTEXT CONST pAppContext, u8 *buf_ptr, const u32 buf_size, const u32 payload_size, UINT32 *no_of_payloads)
{
    UINT32 l_no_of_payloads = 0u;
    UINT32 crt_no_of_payloads = *no_of_payloads;  // counted no of payloads up to now
    UINT32 last_pl_size = 0u;
    UINT32 j;
    u8 *pPayload; // point the start of each slice

    l_no_of_payloads = buf_size / payload_size;
    last_pl_size = buf_size - (l_no_of_payloads * payload_size);

    pPayload = buf_ptr;

    for(j = 0; j < l_no_of_payloads; j++)
    {
        makePayload(pAppContext, crt_no_of_payloads + j, pPayload, payload_size);
        pPayload += payload_size;
    }

    if (last_pl_size > 0u)
    {
        makePayload(pAppContext, crt_no_of_payloads + l_no_of_payloads, pPayload, last_pl_size);
        l_no_of_payloads++;
    }

    // update the total number of payloads
    *no_of_payloads += l_no_of_payloads;
}



// Name:    VideoDemo_WriteOneFrame()
//
// Function:
//     Send one frame video data to host side.
//
// Definition:
//     VOID VideoDemo_WriteOneFrame(
//         VIDEODEMO_CONTEXT *    pVideoDemoCtx,
//        );
//
// Description:
//     This function is called once per frame in order to start transmission.
//     Frame is "split" in slices which are sent one by one, then assembled by Host on the other side of the bus.
//     First the frame is prepared for transmission:
//      - toggle the header bit  and increment the PTS/SoF counter (used to fill USB headers)
//      - parse data buffers (cdk header -optional-, Y, UV planes) i.e. split in slices of configured payload size
//      - for each slice fill the USB header and create a buffer handler
//      - after parsing all data buffers, total number of slices is counted in pAppContext->transaction.wNumberOfPayloads
//     Transmission of 1st slice is initiated here by calling VideoClientLib_StreamWriteV2.
//     The rest of the slices are transmitted from callback VideoDemo_WriteOneFrame_Done.
//
// Returns:
//     Nothing
//
// Notes:
//     The callback function triggers the next slice transmission.
//     After sending last slice, the buffer handlers are deleted (free the memory)

void VideoDemo_WriteOneFrame(VOID *ctx)
{
    VIDEODEMO_CONTEXT *pVideoDemoCtx = (VIDEODEMO_CONTEXT*)ctx;
    PUNCOMPRESSED_APP_CONTEXT CONST pAppContext = pVideoDemoCtx->pAppContext;
    u32 plane_size;
    u32 i;
    BOOL fEndofFrame = FALSE;
    FrameT *pBuffer = pAppContext->ptrFrm->task->buffer;
    
    DBG_PRINTF("WOF V2 ");

    // update at start of frame
    pAppContext->HeaderToggle = (pAppContext->HeaderToggle == 0u)? 1u : 0u;
    pAppContext->dwPTS += 1u;
    pAppContext->wSofCounter += 1u;

    pAppContext->transaction.wNumberOfPayloads = 0; // count the total number of slices

    // for CDK, send the mipi header before frame
#ifdef USE_STATIC_HEADER
    plane_size = pBuffer->stride[0] * SEND_OUT_HEADER_HEIGHT;
    if (plane_size > 0u)
    {
        //                context,    pointer to buffer               , buf size  , payload size             , counter of payloads (used as start index/offset))
        processBuffer(pAppContext, (uint8_t *)sendOutGetBufferHeader(), plane_size, pAppContext->wPayloadSize, &(pAppContext->transaction.wNumberOfPayloads));
    }
#endif
    // parse the frame, call processBuffer for every plane
    for(i = 0; i < pBuffer->nPlanes; i++)
    {
        plane_size = pBuffer->stride[i] * pBuffer->height[i];

        if (plane_size > 0u)
        {
            processBuffer(pAppContext, pBuffer->fbPtr[i], plane_size, pAppContext->wPayloadSize, &(pAppContext->transaction.wNumberOfPayloads));
        }
    }

    /* for last payload set EOF header */
    if(pAppContext->transaction.wNumberOfPayloads > 0u)
    {
        makeHeader(pAppContext, &UsbSend_UsbHdr[pAppContext->transaction.wNumberOfPayloads - 1][0], TRUE);
    }
    // global index of the slices; start transmission with 1st slice
    pAppContext->transaction.wPayloadIdx = 0;

    // in case there's a single payload, set EndOfFrame as TRUE
    if(pAppContext->transaction.wNumberOfPayloads <= 1)
    {
        fEndofFrame = TRUE;
    }

    VideoClientLib_StreamWriteV2(
            pVideoDemoCtx->pVideo,
            VideoDemo_WriteOneFrame_Done,
            pAppContext,
            pVideoDemoCtx->hVideoOut,
            &UsbSend_UsbHdr[pAppContext->transaction.wPayloadIdx][0],
            pAppContext->transaction.wPlSize[pAppContext->transaction.wPayloadIdx] + PAYLOAD_HEADER_SIZE,//payload buffer size + PAYLOAD_HEADER_SIZE,
            UsbSend_UsbBufHndl[pAppContext->transaction.wPayloadIdx],
            fEndofFrame
    );
}

// Name:    VideoDemo_WriteOneFrame_Done()
//
// Function:
//     The callback function for VideoDemo_WriteOneFrame()
//
// Definition:
//     VOID  VideoDemo_WriteOneFrame_Done(
//        VOID *        pContext,
//        UINT32        ErrorCode,
//        UINT8 *     pData,
//        BYTES        nData,
//        );
//
// Description:
//     The callback function for VideoDemo_WriteOneFrame() is sending next slice of the frame.
//     After sending last slice, delete the buffer handlers to free memory and release the frame.
//     In case of error, force deletion of buffer handlers and release the frame.
//
// Returns:
//     Nothing
//
// Notes:
//     index of the current slice is pAppContext->transaction.wPayloadIdx
//     size of the slices is stored in array pAppContext->transaction.wPlSize[]

static void VideoDemo_WriteOneFrame_Done(
    void     *pContext,
    UINT32    ErrorCode,
    UINT8    *pData,
    BYTES     nData
    )
{
    PUNCOMPRESSED_APP_CONTEXT pAppContext = (PUNCOMPRESSED_APP_CONTEXT)pContext;
    VIDEODEMO_CONTEXT *    pVideoDemoCtx = pAppContext->pVideoDemoCtx;
    sUsbTest *sUsbData = pAppContext->ptrFrm;

    BOOL fEndofFrame = FALSE;
    u32 status;

    USBPUMP_UNREFERENCED_PARAMETER(nData);
    USBPUMP_UNREFERENCED_PARAMETER(pData);

    DBG_PRINTF("WOF_DnV2");

    if (ErrorCode != UPROTO_VIDEO_ERROR_OK)
    {
        DBG_ERROR_PRINTF("write fail %s in cam %d frame %d %p\n",
                gk_VideoDemo_ErrorName[ErrorCode],
                (int)sUsbData->task->outId,
                (int)sUsbData->task->buffer->seqNo,
                sUsbData->task->buffer);

        /* force release of USB buffer handlers and frame */
        pAppContext->transaction.wPayloadIdx = pAppContext->transaction.wNumberOfPayloads - 1u;
    }

    if( (sUsbData->busy == 0) &&
            (sUsbData->filled > 0) &&
            (sUsbData->sendInProgress > 0) )
    {
        // go to next slice
        pAppContext->transaction.wPayloadIdx++;
        // if last one has been sent, free used buffer handlers
        if (pAppContext->transaction.wPayloadIdx >= pAppContext->transaction.wNumberOfPayloads)
        {
            // TODO: move this in a function
            // release buffer handlers
            while(pAppContext->transaction.wPayloadIdx > 0)
            {
                --(pAppContext->transaction.wPayloadIdx);
                DBG_PRINTF("del bh %lu @ 0x%8lx...", (u32)pAppContext->transaction.wPayloadIdx, (u32)UsbSend_UsbBufHndl[pAppContext->transaction.wPayloadIdx]);
                status = UsbPumpMdkUhilAux_HbufferDelete(
                        pVideoDemoCtx->pVideo->pDevice->udev_hUhilAux,    //hSession,
                        &UsbSend_UsbBufHndl[pAppContext->transaction.wPayloadIdx]);
                if(status != USBPUMP_UHILAUX_STATUS_OK)
                {
                    printf("error %lu (deleting buffer handlers)\n", status);
                }
                else
                {
                    DBG_PRINTF("OK\n       ");
                }
            }

            /* release frame buffers */
            sUsbData->sendInProgress = 0;
            sUsbData->filled = 0;

            DBG_PRINTF("rel frm %lu\n", sUsbData->task->buffer->seqNo);
            if (sUsbData->task->localCallback)
            {
                sUsbData->task->localCallback(sUsbData->task);
            }
        }
        else
        {
            DBG_PRINTF(" ->#%d\n", pAppContext->transaction.wPayloadIdx);
            // if this is the last slice, set fEndofFrame as TRUE
            if (pAppContext->transaction.wPayloadIdx + 1 >= pAppContext->transaction.wNumberOfPayloads)
            {
                fEndofFrame = TRUE;
            }
            VideoClientLib_StreamWriteV2(
                    pVideoDemoCtx->pVideo,
                    VideoDemo_WriteOneFrame_Done,
                    pAppContext,
                    pVideoDemoCtx->hVideoOut,
                    &UsbSend_UsbHdr[pAppContext->transaction.wPayloadIdx][0],
                    pAppContext->transaction.wPlSize[pAppContext->transaction.wPayloadIdx] + PAYLOAD_HEADER_SIZE,//payload buffer size + PAYLOAD_HEADER_SIZE,
                    UsbSend_UsbBufHndl[pAppContext->transaction.wPayloadIdx],
                    fEndofFrame
            );
        }
    }
}


int VideoDemo_SendFrame(sUsbTest *sUsbData)
{
    int ret = 1;
    PUNCOMPRESSED_APP_CONTEXT /*CONST*/     pAppContext;
    VIDEODEMO_CONTEXT * /*CONST*/     pVideoDemoCtx;

    DBG_PRINTF("\nSF ");

    if(g_pVidemoDemoContext != NULL)
    {
        pAppContext = g_pVidemoDemoContext->pAppContext;
        pVideoDemoCtx = pAppContext->pVideoDemoCtx;

        if((sUsbData->busy == 0) && (sUsbData->filled > 0))
        {
            if(pVideoDemoCtx->fOutputActivate)
            {
#if USE_STATIC_FRAME_BUFFER
                copy_frame_to_send(sUsbData->task->buffer);
#endif

                sUsbData->sendInProgress = 1;

                DBG_PRINTF("start tx CAM %lu frm %lu ", sUsbData->task->outId, sUsbData->task->buffer->seqNo);

                pAppContext->ptrFrm = sUsbData;

                UsbPumpPlatform_PostEvent(
                                pVideoDemoCtx->pPlatform,
                                &pVideoDemoCtx->WriteCallback
                );

                ret = 0;
            }
            else
            {
                DBG_PRINTF("err: no v player! %d\n", (int)sUsbData->task->buffer->seqNo);
            }
        }
        else
        {
            DBG_ERROR_PRINTF("Str busy %d\n", (int)sUsbData->task->buffer->seqNo);
        }

        DBG_PRINTF("SF exit %d %p %lu %lu\n", (int)sUsbData->task->buffer->seqNo, sUsbData->task->buffer->fbPtr[0], sUsbData->task->buffer->stride[0], sUsbData->task->buffer->height[0]);
    }
    else
    {
        DBG_ERROR_PRINTF("error: USB Pump not init!\n\n");
    }

    return (ret);
}

