///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Video demo application context.
///

// 1: Includes
// ----------------------------------------------------------------------------
#ifndef _VIDEODEMO_H_
#define _VIDEODEMO_H_

#ifndef _PROTOVIDEO_H_
# include "protovideo.h"
#endif

#ifndef _UCALLBACK_H_
# include "ucallback.h"
#endif

#include "app_config.h"
#include <rtems.h>
#include <semaphore.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
__TMS_TYPE_DEF_STRUCT(VIDEODEMO_CONTEXT);
__TMS_TYPE_DEF_STRUCT(VIDEODEMO_BUFFER_HDR);
__TMS_TYPE_DEF_STRUCT(VIDEODEMO_BUFFER);
__TMS_TYPE_DEF_STRUCT(PROCESSING_UNIT);
__TMS_TYPE_DEF_STRUCT(PROBE_COMMIT);
__TMS_TYPE_DEF_STRUCT(STILL_PROBE_COMMIT);

#define MAX_FRAME_BUFFS         MAX_USED_BUF
#define VIDEO_PLANES_NUM        1

#ifndef    VIDEODEMO_CONFIG_CONTROLBUFFER_SIZE    /*PARAM*/
# define VIDEODEMO_CONFIG_CONTROLBUFFER_SIZE    (512)
#endif

#ifndef    VIDEODEMO_CONFIG_MAXQUEUED_BUFFER    /*PARAM*/
# define VIDEODEMO_CONFIG_MAXQUEUED_BUFFER    (8)
#endif

#ifndef    VIDEODEMO_CACHE_ALIGNMENT        /*PARAM*/
# define VIDEODEMO_CACHE_ALIGNMENT        (4)
#endif

// The bytes of data between 2 header
#define PAYLOAD_HEADER_SIZE        (12)
#define HDR_BUFFER_COUNT           (2)

#define UNCOMPRESSED_HDR_HLE_OFFSET        0
#define UNCOMPRESSED_HDR_BFH_OFFSET        1
#define UNCOMPRESSED_HDR_PTS_OFFSET        2
#define UNCOMPRESSED_HDR_SCR_STC_OFFSET    6
#define UNCOMPRESSED_HDR_SCR_SOF_OFFSET    10
#define UNCOMPRESSED_HDR_EOH            (1 << 7)
#define UNCOMPRESSED_HDR_ERR            (1 << 6)
#define UNCOMPRESSED_HDR_STI            (1 << 5)
#define UNCOMPRESSED_HDR_RES            (1 << 4)
#define UNCOMPRESSED_HDR_SCR            (1 << 3)
#define UNCOMPRESSED_HDR_PTS            (1 << 2)
#define UNCOMPRESSED_HDR_EOF            (1 << 1)
#define UNCOMPRESSED_HDR_FID            (1 << 0)

// Constants of Processing Unit
#define PU_BACKLIGHT_COMPENSATION_CONTROL_INFO   0x03
#define PU_BACKLIGHT_COMPENSATION_CONTROL_MIN    0x0000
#define PU_BACKLIGHT_COMPENSATION_CONTROL_MAX    0x0003
#define PU_BACKLIGHT_COMPENSATION_CONTROL_RES    0x0001
#define PU_BACKLIGHT_COMPENSATION_CONTROL_DEF    0x0000

#define PU_BRIGHTNESS_CONTROL_INFO       0x03
#define PU_BRIGHTNESS_CONTROL_MIN        0x0000
#define PU_BRIGHTNESS_CONTROL_MAX        0x00FF
#define PU_BRIGHTNESS_CONTROL_RES        0x0001
#define PU_BRIGHTNESS_CONTROL_DEF        0x007F

#define PU_CONTRAST_CONTROL_INFO           0x03
#define PU_CONTRAST_CONTROL_MIN            0x0000
#define PU_CONTRAST_CONTROL_MAX            0x00FF
#define PU_CONTRAST_CONTROL_RES            0x0001
#define PU_CONTRAST_CONTROL_DEF            0x007F

#define PU_POWER_LINE_FREQUENCY_CONTROL_INFO   0x03
#define PU_POWER_LINE_FREQUENCY_CONTROL_MIN    0x00
#define PU_POWER_LINE_FREQUENCY_CONTROL_MAX    0x02
#define PU_POWER_LINE_FREQUENCY_CONTROL_RES    0x01
#define PU_POWER_LINE_FREQUENCY_CONTROL_DEF    0x01

#define PU_HUE_CONTROL_INFO           0x03
#define PU_HUE_CONTROL_MIN            0x0000
#define PU_HUE_CONTROL_MAX            0x0168
#define PU_HUE_CONTROL_RES            0x0064
#define PU_HUE_CONTROL_DEF            0x0000

#define PU_SATURATION_CONTROL_INFO       0x03
#define PU_SATURATION_CONTROL_MIN        0x0000
#define PU_SATURATION_CONTROL_MAX        0x00FF
#define PU_SATURATION_CONTROL_RES        0x0001
#define PU_SATURATION_CONTROL_DEF        0x007F

#define PU_SHARPNESS_CONTROL_INFO       0x03
#define PU_SHARPNESS_CONTROL_MIN        0x0000
#define PU_SHARPNESS_CONTROL_MAX        0x000F
#define PU_SHARPNESS_CONTROL_RES        0x0001
#define PU_SHARPNESS_CONTROL_DEF        0x0000

#define PU_GAMMA_CONTROL_INFO           0x03
#define PU_GAMMA_CONTROL_MIN            0x0064
#define PU_GAMMA_CONTROL_MAX            0x00DC
#define PU_GAMMA_CONTROL_RES            0x003C
#define PU_GAMMA_CONTROL_DEF            0x00A0

#define PU_WHITE_BALANCE_TEMPERATURE_CONTROL_INFO   0x03
#define PU_WHITE_BALANCE_TEMPERATURE_CONTROL_MIN    0x0AFA
#define PU_WHITE_BALANCE_TEMPERATURE_CONTROL_MAX    0x1964
#define PU_WHITE_BALANCE_TEMPERATURE_CONTROL_RES    0x03E8
#define PU_WHITE_BALANCE_TEMPERATURE_CONTROL_DEF    0x16A8

#define PU_HUE_AUTO_CONTROL_INFO        0x1
#define PU_HUE_AUTO_CONTROL_MIN         0x0
#define PU_HUE_AUTO_CONTROL_MAX         0x1
#define PU_HUE_AUTO_CONTROL_RES         0x1
#define PU_HUE_AUTO_CONTROL_DEF         0x0

#define PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL_INFO  0x01
#define PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL_MIN   0x00
#define PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL_MAX   0xFF
#define PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL_RES   0x01
#define PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL_DEF   0x01

// Constants of Proble/Commit
#define PROBE_DEFAULT_bmHint                0x0000
#define PROBE_DEFAULT_bFormatIndex          0x01
#define PROBE_DEFAULT_bFrameIndex           0x01
#define PROBE_DEFAULT_dwFrameInterval       166663
#define PROBE_DEFAULT_wKeyFrameRate         0x0001
#define PROBE_DEFAULT_wPFrameRate           0x0000
#define PROBE_DEFAULT_wCompQuality          0x0000
#define PROBE_DEFAULT_wCompWindowSize       0x0000
#define PROBE_DEFAULT_wDelay                0x000
#define PROBE_DEFAULT_dwMaxVideoFrameSize        \
    (FRAME_SIZE_SS + PAYLOAD_HEADER_SIZE)
#define PROBE_DEFAULT_dwMaxVideoFrameSize_HS     \
    (FRAME_SIZE_HS + PAYLOAD_HEADER_SIZE)
#define PROBE_DEFAULT_dwClockFrequency      0x00000000
#define PROBE_DEFAULT_bmFramingInfo         0x00
#define PROBE_DEFAULT_bPreferedVersion      0x00
#define PROBE_DEFAULT_bMinVersion           0x00
#define PROBE_DEFAULT_bMaxVersion           0x00

// VideoDemo Processing Unit variables
struct __TMS_STRUCTNAME(PROCESSING_UNIT)
{
    uint16_t    wBackLightCompensation;
    uint16_t    wBrightness;
    uint16_t    wContrast;
    uint8_t     bPowerLineFrequency;
    uint16_t    wHue;
    uint16_t    wSaturation;
    uint16_t    wSharpness;
    uint16_t    wGamma;
    uint16_t    wWhiteBalanceTemperature;
    uint8_t     bHueAuto;
    uint8_t     bWhiteBalanceTemperatureAuto;
};



// VideoDemo Probe/Commit variables
struct __TMS_STRUCTNAME(PROBE_COMMIT)
{
    uint16_t    bmHint;
    uint8_t     bFormatIndex;
    uint8_t     bFrameIndex;
    uint32_t    dwFrameInterval;
    uint16_t    wKeyFrameRate;
    uint16_t    wPFrameRate;
    uint16_t    wCompQuality;
    uint16_t    wCompWindowSize;
    uint16_t    wDelay;
    uint32_t    dwMaxVideoFrameSize;
    uint32_t    dwMaxPayloadTransferSize;
    uint32_t    dwClockFrequency;
    uint8_t     bmFramingInfo;
    uint8_t     bPreferedVersion;
    uint8_t     bMinVersion;
    uint8_t     bMaxVersion;
};


//    VideoDemo Still Probe/Commit variables
struct __TMS_STRUCTNAME(STILL_PROBE_COMMIT)
{
    uint8_t     bFormatIndex;
    uint8_t     bFrameIndex;
    uint8_t     bCompressionIndex;
    uint32_t    dwMaxVideoFrameSize;
    uint32_t    dwMaxPayloadTransferSize;
};

typedef struct {
    uint8_t                    *buffer;
    uint32_t                    bufferSize;
    uint8_t                    *headerBuffer;
    uint32_t                    headerBufferSize;
    uint32_t                    lastPacketSize;
    uint16_t                    isoPacketsNum;
    USBPUMP_BUFFER_HANDLE       hBuffer;
    USBPUMP_ISOCH_PACKET_DESCR *pIsochDescr;
    uint32_t                    isoDescrSize;
} VideoFramePlaneConfig;


//    VideoDemo Buffer
struct __TMS_STRUCTNAME(VIDEODEMO_BUFFER)
{
    UCALLBACKCOMPLETION       Event;
    VIDEODEMO_CONTEXT *       pVideoDemoCtx;
    VideoFramePlaneConfig     planes[VIDEO_PLANES_NUM];
    uint8_t                     bufferIdx;
};

// VideoDemo Application Context
struct __TMS_STRUCTNAME(VIDEODEMO_CONTEXT)
{
    UPROTO_VIDEO *       pVideo;
    UPLATFORM *          pPlatform;
    UDEVICE *            pDevice;

    // Videon IN/OUT stream handle
    UPROTO_VIDEO_STREAM_HANDLE hVideoIn;    /* USB OUT */
    UPROTO_VIDEO_STREAM_HANDLE hVideoOut;    /* USB IN */

    uint16_t              wVideoInMaxPacketSize;
    uint16_t              wVideoOutMaxPacketSize;

    uint16_t              wTransportHeaderSize; /* ISO header */

    BOOL                fInputActivate;
    BOOL                fOutputActivate;

    // Current setting of Processing Unit
    PROCESSING_UNIT        ProcUnitCur;

    // Current setting of Probe
    PROBE_COMMIT ProbeCur;

    // Current setting of Commit
    PROBE_COMMIT CommitCur;

    // Current setting of Probe
    PROBE_COMMIT ProbeMin;

    // Current setting of Probe
    PROBE_COMMIT ProbeMax;

    // Current setting of Probe
    PROBE_COMMIT ProbeDef;

    // Current setting of Probe
    PROBE_COMMIT CommitMin;

    // Current setting of Probe
    PROBE_COMMIT CommitMax;

    // Current setting of Probe
    PROBE_COMMIT CommitDef;


    // Current setting of Still Probe
    STILL_PROBE_COMMIT    StillProbeCur;

    // Current setting of Still Commit
    STILL_PROBE_COMMIT    StillCommitCur;

    // Video protocol interface callabck completion
    UCALLBACKCOMPLETION        InitCompletion;
    UCALLBACKCOMPLETION        WriteCallback;

    UCALLBACKCOMPLETION             SetupCompletion;
    UPROTO_VIDEO_CONTROL_REQUEST    ControlRequest;
    UPROTO_VIDEO_STREAM_HANDLE      hStreamRequest;
    UPROTO_VIDEO_STREAM_REQUEST     StreamRequest;

    VIDEODEMO_BUFFER        VideoBuffer[MAX_FRAME_BUFFS];

    // Presentation Time Stamp :
    // a source clock time in native device unit
    uint32_t            dwPTS;

    // SOF Counter :
    // doesn't need to match the SOF token, Just a 1 kHz counter
    uint16_t           wSofCounter;

    uint8_t            headerToggle;
    uint8_t            payloadHeaderSize;

    uint8_t            bStillTrigger;

    uint16_t           isoPacketSize;
    uint32_t           payloadSize;

    sem_t            packetSem;
    uint8_t            bufferIndex;
    BOOL             taskCreated;
};



// VideoDemo Internal Functions
__TMS_BEGIN_DECLS

extern __TMS_CONST UPROTO_VIDEO_OUTSWITCH gk_VideoDemo_OutSwitch;
extern int32_t deviceAttached;

__TMS_VIDEODEMO_CONTEXT *
VideoDemo_UncompressedInit(
        __TMS_UPLATFORM *        pPlatform,
        __TMS_USBPUMP_OBJECT_HEADER *    pVideoFunction
        );

__TMS_VOID
VideoDemo_Start(__TMS_VIDEODEMO_CONTEXT *    pVideoDemoCtx);

UPROTO_VIDEO_CONTROL_STATUS_EVENT_CB_FN   VideoDemo_ControlStatusEvent;
UPROTO_VIDEO_CONTROL_REQUEST_CB_FN        VideoDemo_ProcessingUnitEvent;
UPROTO_VIDEO_STREAM_STATUS_EVENT_CB_FN    VideoDemo_StreamStatusEvent;
UPROTO_VIDEO_STREAM_REQUEST_CB_FN         VideoDemo_StreamInterfaceEvent;

__TMS_END_DECLS

#endif // _VIDEODEMO_H_
