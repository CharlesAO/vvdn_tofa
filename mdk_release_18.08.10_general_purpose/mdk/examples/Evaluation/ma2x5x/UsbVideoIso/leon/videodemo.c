///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Home for VideoDemo_UncompressedInit()
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

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
static UPROTO_VIDEO_CONTROL_OPEN_CB_FN      VideoClientLibI_ControlOpen_Callback;
static UPROTO_VIDEO_STREAM_OPEN_CB_FN      VideoClientLibI_StreamOpen_Callback;

static CALLBACKFN    VideoClientLibI_StreamOpen_Sync;

//Name:    VideoDemo_UncompressedInit
//
//Function:
//    Application instance initializaton function, called by
//    VideoDemo_ClientCreate.
//
//Definition:
//    VIDEODEMO_CONTEXT *
//    VideoDemo_UncompressedInit(
//        UPLATFORM *        pPlatform,
//        USBPUMP_OBJECT_HEADER *    pFunctionObject
//        );
//
//Description:
//    This module will allocate memory needed for application instance, give
//    the initial values to application context variables and send Open
//    command to protocol layer.
//
//Returns:
//    pointer to application context.

#define    FUNCTION "VideoDemo_UncompressedInit"

VIDEODEMO_CONTEXT *
VideoDemo_UncompressedInit(
        UPLATFORM *        pPlatform,
        USBPUMP_OBJECT_HEADER *    pFunctionObject
)
{
    VIDEODEMO_CONTEXT *    pVideoDemoCtx;
    UPROTO_VIDEO * CONST    pVideo = __TMS_CONTAINER_OF(
            pFunctionObject,
            UPROTO_VIDEO,
            ObjectHeader
    );

    // Allocate videodemo context
    pVideoDemoCtx = UsbPumpPlatform_MallocZero(
            pPlatform,
            sizeof(*pVideoDemoCtx)
    );

    if (pVideoDemoCtx == NULL)
    {
        TTUSB_OBJPRINTF((
                &pVideo->ObjectHeader,
                UDMASK_ERRORS,
                "?" FUNCTION ":"
                " pVideoDemoCtx allocation fail\n"
        ));
        return NULL;
    }

    pVideoDemoCtx->pVideo = pVideo;
    pVideoDemoCtx->pPlatform = pPlatform;
    pVideoDemoCtx->pDevice = pVideo->pDevice;
    pVideoDemoCtx->hVideoIn = UPROTO_VIDEO_STREAM_INVALID_HANDLE;
    pVideoDemoCtx->hVideoOut = UPROTO_VIDEO_STREAM_INVALID_HANDLE;

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
        // dwMaxPayloadTransferSize depends on the value of maxPacketSize;
        // it will be calculated at StreamGetInfo event when the value of
        // maxPacketSize becomes available
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

    // Open video control
    VideoClientLib_ControlOpen(
            pVideoDemoCtx->pVideo,
            VideoClientLibI_ControlOpen_Callback,
            pVideoDemoCtx,
            pVideoDemoCtx,
            &gk_VideoDemo_OutSwitch
    );

    return pVideoDemoCtx;
}

#undef    FUNCTION

#define    FUNCTION "VideoClientLibI_ControlOpen_Callback"

static void
VideoClientLibI_ControlOpen_Callback(
        void *    pCallbackCtx,
        UINT32    ErrorCode
)
{
    VIDEODEMO_CONTEXT * CONST pVideoDemoCtx = pCallbackCtx;

    if (ErrorCode != UPROTO_VIDEO_ERROR_OK)
    {
        TTUSB_OBJPRINTF((
                &pVideoDemoCtx->pVideo->ObjectHeader,
                UDMASK_ERRORS,
                "?" FUNCTION ":"
                " ControlOpen() fail (%s)\n",
                UsbPumpVideo_GetErrorName(ErrorCode)
        ));
    }
    else
    {
        // initialize callback completion
        USBPUMP_CALLBACKCOMPLETION_INIT(
                &pVideoDemoCtx->InitCompletion,
                VideoClientLibI_StreamOpen_Sync,
                pVideoDemoCtx
        );

        UsbPumpPlatform_PostEvent(
                pVideoDemoCtx->pPlatform,
                &pVideoDemoCtx->InitCompletion
        );
    }
}

#undef    FUNCTION

static void
VideoClientLibI_StreamOpen_Sync(
        void *    pClientContext
)
{
    VIDEODEMO_CONTEXT * CONST pVideoDemoCtx = pClientContext;

    // Open video output stream
    VideoClientLib_StreamOpen(
            pVideoDemoCtx->pVideo,
            VideoClientLibI_StreamOpen_Callback,
            pVideoDemoCtx,
            (INT8)(-1),    /* InterfaceNumber */
            (INT8)(-1),    /* AlternateSetting */
            (INT8)(-1),    /* TerminalLink */
            FALSE        /* fTrueIfVideoInput */
    );
}

static void
VideoClientLibI_StreamOpen_Callback(
        void *                pCallbackCtx,
        UINT32                ErrorCode,
        UPROTO_VIDEO_STREAM_HANDLE    hVideoStream
)
{
    VIDEODEMO_CONTEXT * CONST pVideoDemoCtx = pCallbackCtx;

    if (ErrorCode != UPROTO_VIDEO_ERROR_OK)
    {
        TTUSB_OBJPRINTF((
                &pVideoDemoCtx->pVideo->ObjectHeader,
                UDMASK_ERRORS,
                "?VideoClientLibI_StreamOpen_Callback:"
                " StreamOpen() fail (%s)\n",
                UsbPumpVideo_GetErrorName(ErrorCode)
        ));
    }
    else
    {
        pVideoDemoCtx->hVideoOut = hVideoStream;
    }
}
