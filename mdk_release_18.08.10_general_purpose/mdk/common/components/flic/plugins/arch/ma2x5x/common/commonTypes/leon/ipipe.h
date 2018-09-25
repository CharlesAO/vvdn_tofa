///
/// @file      ipipe.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     IPIPE Client API definitions.
///

#ifndef _IPIPE_H
#define _IPIPE_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mv_types.h"
#include <FrameTypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#if defined(__GNUC__)
#include <pthread.h> //PC-gcc
#endif

/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/
// myriad2 maximum hw possible number of mipi and cif inputs
#define IPIPE_MAX_SOURCES_ALLOWED  6

#ifndef IPIPE_MAX_ISP_ALlOWED
#define IPIPE_MAX_ISP_ALlOWED 12
#endif

#ifndef IPIPE_MAX_OUTPUTS_ALlOWED
#define IPIPE_MAX_OUTPUTS_ALlOWED 36
#endif

#ifndef IPIPE_MAX_USERPLUG_ALlOWED
#define IPIPE_MAX_USERPLUG_ALlOWED 4
#endif
#if defined(__sparc)   //Myriad2
#ifndef INTER_CPU_INT
#define INTER_CPU_INT IRQ_DYNAMIC_0
#endif
#define INTER_CPU_INT_LEVEL 5
#endif


/// L2 Cache bypass macro
#ifndef BYPASS_L2_CACHE
#   define L2THROUGHCACHE(x) ((uint32_t)(x) & ~0x08000000)
#else
#   define L2THROUGHCACHE(x) ((uint32_t)(x) | 0x08000000)
#endif


// icStatus
typedef enum {
    IPIPE_UNINITIALIZED         = 0,
    IPIPE_INITIALIZED           = 1,
    IPIPE_CONFIGURED            = 2,
}icStatus;


/* API usage errors (returned by ic*() calls that return int32_t) */
typedef enum {
    IC_ERROR_NONE                            = 0,  /* Happy days */
    IC_ERROR_WRONG_STATE                     = 1,
    IC_ERROR_BAD_PARAMETER                   = 3,
    IC_ERROR_RT_CFG_MISSING                  = 5,
    IC_ERROR_RT_CFG_SKIPPED                  = 6,
    IC_ERROR_RT_OUT_BUFFERS_NOT_AVAILABLE    = 7,
    IC_ERROR_SENSOR_SOURCE_STOP_TIMEOUT      = 8,
    IC_ERROR_NO_ZSL_BUFFS_AVAILABLE          = 9,
    IC_ERROR_TRIGGER_NOT_EXISTING_BUFF       = 10,
    IC_ERROR_ISP_OUT_SKIP_3A_REQUEST         = 11,
} icError;

typedef enum {
    IC_STATS_SUCCESS,
    IC_STATS_OUT_OF_MEM,
    IC_STATS_BAD_GLOBAL,
    IC_STATS_BAD_SOURCE,
    IC_STATS_NO_ZSL
}icStatusCode;

typedef enum {
    IC_ADD_ZSL_SUCCESS,
    IC_ADD_ZSL_NOT_ENOUGH_MEM,
    IC_ADD_ZSL_BAD_STATE,
    IC_ADD_ZSL_FIND_STATUS
}icAddZSLStatusCode;
/* Severity of exception or error */
typedef enum {
    IC_SEVERITY_NORMAL =                0,  /* Normal severity - system will continue to function */
    IC_SEVERITY_RESET =                 1,  /* System halted - reset required to recover */
    IC_SEVERITY_CATASTROPHIC =          2,  /* Catastrophic failure - system halted - reset unlikely to recover */
} icSeverity;

/* High priority events */
typedef enum {
    IC_HP_CMD_NONE =                    0,
    IC_HP_CMD_HALT =                    1,  /* Tell LeonRT app to stop everything it's doing and halt.  LeonRT needs to be restarted afterwards. */
    IC_HP_CMD_QUIESCE =                 2,  /* Tell LeonRT app to stop everything it's doing and sleep until futher commanrd arrive */
} icHpCmd;

/* Bayer pattern order */
typedef enum {
    IC_BAYER_FORMAT_GRBG = 0,
    IC_BAYER_FORMAT_RGGB = 1,
    IC_BAYER_FORMAT_GBRG = 2,
    IC_BAYER_FORMAT_BGGR = 3,
} icBayerFormat;

/**/

/* ISP parameter groups */
typedef enum {
    IC_ISP_GROUP_BLC =              0x00000001,
    IC_ISP_GROUP_RAW =              0x00000002,
    IC_ISP_GROUP_LSC =              0x00000004,
    IC_ISP_GROUP_DEMOSAIC =         0x00000008,
    IC_ISP_GROUP_CHROMA_GEN =       0x00000010,
    IC_ISP_GROUP_LUMA_DNS =         0x00000020,
    IC_ISP_GROUP_LUMA_DNS_REF =     0x00000040,
    IC_ISP_GROUP_CHROMA_DNS =       0x00000080,
    IC_ISP_GROUP_MEDIAN =           0x00000100,
    IC_ISP_GROUP_MEDIAN_MIX =       0x00000200,
    IC_ISP_GROUP_LOWPASS =          0x00000400,
    IC_ISP_GROUP_SHARPEN =          0x00000800,
    IC_ISP_GROUP_RANDOM_NOISE =     0x00001000,
    IC_ISP_GROUP_COLOR_COMBINE =    0x00002000,
    IC_ISP_GROUP_GAMMA =            0x00004000,
    IC_ISP_GROUP_CROP_RESIZE =      0x00008000,
    IC_ISP_GROUP_PIPELINE_CTL =     0x00010000,
} icIspGroup;

#define DEF_PRV_ISP_GROUP_FLAGS (IC_ISP_GROUP_BLC           | \
                                 IC_ISP_GROUP_RAW           | \
                                 IC_ISP_GROUP_LSC           | \
                                 IC_ISP_GROUP_DEMOSAIC      | \
                                 IC_ISP_GROUP_CHROMA_GEN    | \
                                 IC_ISP_GROUP_LUMA_DNS      | \
                                 IC_ISP_GROUP_LUMA_DNS_REF  | \
                                 IC_ISP_GROUP_CHROMA_DNS    | \
                                 IC_ISP_GROUP_MEDIAN        | \
                                 IC_ISP_GROUP_MEDIAN_MIX    | \
                                 IC_ISP_GROUP_LOWPASS       | \
                                 IC_ISP_GROUP_SHARPEN       | \
                                 IC_ISP_GROUP_RANDOM_NOISE  | \
                                 IC_ISP_GROUP_COLOR_COMBINE | \
                                 IC_ISP_GROUP_GAMMA)

#define DEF_CAPT_ISP_GROUP_FLAGS (DEF_PRV_ISP_GROUP_FLAGS)
#define DEF_ZSL_ISP_GROUP_FLAGS (DEF_PRV_ISP_GROUP_FLAGS)

/* ISP function enable flags */
typedef enum {
    IC_ISP_ENABLE_DPC =             0x00000001,
    IC_ISP_ENABLE_GRGB_IMBALANCE =  0x00000002,
    IC_ISP_ENABLE_AE_AWB_STATS =    0x00000004,
    IC_ISP_ENABLE_AF_STATS =        0x00000008,
    IC_ISP_ENABLE_LSC =             0x00000010,
    IC_ISP_ENABLE_LUMA_DENOISE =    0x00000020,
    IC_ISP_ENABLE_SHARPEN =         0x00000040,
    IC_ISP_ENABLE_CHROMA_DENOISE =  0x00000080,
    IC_ISP_ENABLE_LOWPASS =         0x00000100,
    IC_ISP_ENABLE_GAMMA =           0x00000200,
} icIspEnable;

#define DEF_PRV_ISP_ENABLE_FLAGS (IC_ISP_ENABLE_DPC            |  \
                                  IC_ISP_ENABLE_GRGB_IMBALANCE |  \
                                  IC_ISP_ENABLE_AE_AWB_STATS   |  \
                                  IC_ISP_ENABLE_AF_STATS       |  \
                                  IC_ISP_ENABLE_LSC            |  \
                                  IC_ISP_ENABLE_LUMA_DENOISE   |  \
                                  IC_ISP_ENABLE_SHARPEN        |  \
                                  IC_ISP_ENABLE_CHROMA_DENOISE |  \
                                  IC_ISP_ENABLE_LOWPASS        |  \
                                  IC_ISP_ENABLE_GAMMA)

#define DEF_CAPT_ISP_ENABLE_FLAGS (DEF_PRV_ISP_ENABLE_FLAGS)
#define DEF_ZSL_ISP_ENABLE_FLAGS (DEF_PRV_ISP_ENABLE_FLAGS)


/* ISP function enable flags */
// 6 hw mipi/cif input device supported by myriad2
typedef enum {
    IC_SOURCE_0 =         0,
    IC_SOURCE_1 =         1,
    IC_SOURCE_2 =         2,
    IC_SOURCE_3 =         3,
    IC_SOURCE_4 =         4,
    IC_SOURCE_5 =         5,
} icSourceInstance;

/* flags passed to icLockZSL() */
typedef enum {
    IC_LOCKZSL_TS_RELATIVE =    0x00000001,

} icLockZSLFlags;

/* flags passed to icTriggerCapture() */
typedef enum {
    IC_CAPTURE_KEEP_ZSL_LOCKED  =    0x00000001,
    IC_CAPTURE_SEND_RAW         =    0x00000002,
} icCaptureFlags;

/* icIspConfig.pipeControl */
typedef enum {
    IC_PIPECTL_ZSL_OUT_ENABLE =         0x00000001,
    IC_PIPECTL_PREPROC_OUT_ENABLE  =    0x00000002,
    IC_PIPECTL_MIPI_ENABLE =            0x00000004,
    IC_PIPECTL_ZSL_LOCK =               0x00000008,
} icPipeCtlFlags;


#define DEF_PRV_PIPE_CTRL_FLAGS (IC_PIPECTL_ZSL_OUT_ENABLE      | \
        IC_PIPECTL_PREPROC_OUT_ENABLE  | \
        IC_PIPECTL_MIPI_ENABLE)

#define DEF_CAPT_PIPE_CTRL_FLAGS (DEF_PRV_PIPE_CTRL_FLAGS | IC_PIPECTL_ZSL_LOCK)
#define DEF_ZSL_PIPE_CTRL_FLAGS  (DEF_PRV_PIPE_CTRL_FLAGS)

typedef enum {
    IPIPE_SRC_UNINITIALIZED         = 0,
    IPIPE_SRC_AVAILABLE             = 1,
    IPIPE_SRC_SETUP                 = 2,
    IPIPE_SRC_CONFIGURED            = 3,
    IPIPE_SRC_IN_USE                = 4
}icSourceStatus;

//list of myriad receiver Id's  (unified list) for a specific sensor
// read the documentation, as IC_MIPI_CTRL_0 can be connected just to 2 different receivers
typedef enum {
    IC_SIPP_DEVICE0 = 0,
    IC_SIPP_DEVICE1 = 1,
    IC_SIPP_DEVICE2 = 2,
    IC_SIPP_DEVICE3 = 3,
    IC_CIF0_DEVICE4 = 4,
    IC_CIF1_DEVICE5 = 5
}IcMipiRxCtrlRecNoT;

// all mipi controller from chip
typedef enum {
    IC_MIPI_CTRL_0 = 0 ,
    IC_MIPI_CTRL_1,
    IC_MIPI_CTRL_2,
    IC_MIPI_CTRL_3,
    IC_MIPI_CTRL_4,
    IC_MIPI_CTRL_5
} IcMipiRxCtrlNoT;

// all sported raw, sensor input formats
typedef enum {
    IC_IPIPE_YUV_420_B8       = 0x18,
    IC_IPIPE_RAW_8            = 0x2A, // feet to mipi CSI standard
    IC_IPIPE_RAW_10           = 0x2B,
    IC_IPIPE_RAW_12           = 0x2C,
    IC_IPIPE_RAW_14           = 0x2D
}IcMipiRxDataTypeT;

// mipiRx, receiver Configuration structure
typedef struct {
    IcMipiRxCtrlNoT     controllerNo;
    uint32_t            noLanes;
    uint32_t            laneRateMbps;
    IcMipiRxDataTypeT   dataType;
    uint32_t            dataMode;
    IcMipiRxCtrlRecNoT  recNrl;
} icMipiConfig;

enum { // icSourceSetup.appSpecificInfo information. Any other informations can be added here
    SOURCE_SETUP_NO_APP_COMMAND = 0,
    QUARTER_MODE_FOR_VIDEO_PIPE = 0xabcd0001,
};
typedef struct icSourceSetupS{
    uint32_t maxWidth;
    uint32_t maxHeight;
    uint32_t maxBpp;
    uint32_t maxPixels;
    uint32_t maxHorizN; //max horizontal scale ratio
    uint32_t maxHorizD;
    uint32_t maxVertN;  //max Vertical scale ratio
    uint32_t maxVertD;
    // app specific extra informations
    uint32_t appSpecificInfo;
}icSourceSetup;

typedef struct {
    ImgSize          cameraOutputSize;
    ImgRect          cropWindow;

    /*
     * Bayer Format - Raw, Demosaic and LSC blocks should be programmed
     * to match the Bayer order specified here.
     */
    icBayerFormat   bayerFormat;
    uint32_t        bitsPerPixel;

    /* mipi RX data configuration     */
    icMipiConfig    mipiRxData;
    // used just for pc run version, and contain input file name
    const char *inFileName;
} icSourceConfig;

/*
 * Per-source configuration of parameters which can be modified dynamically
 * (the source does not need to be stopped).  Settings will take effect during
 * the next blanking interval.
 */
typedef struct {
    /*
     * Line number upon which IC_EVENT_TYPE_LINE REACHED will be sent
     * to the Leon OS.  Set to -1 to disable notification.
     */
    int32_t     notificationLine;
} icSourceConfigDynamic;

// Source full configuration structure
typedef struct {
    icSourceStatus          sourceStatus;
    icSourceSetup           sourceSetup;
    icSourceConfig          sourceConfig;
    icSourceConfigDynamic   sourceConfigDynamic;
}icSource;


#define IC_EVENT_CTRL_TYPE_MASK 0x000000ff
#define IC_EVENT_CTRL_OWNED     0x80000000  /* Set means event is owned *by consumer* */

typedef enum {
    /* IPIPE -> Client */
    IC_EVENT_TYPE_READOUT_START             = 0,
    IC_EVENT_TYPE_READOUT_END               = 1,
    IC_EVENT_TYPE_LINE_REACHED              = 2,
    IC_EVENT_TYPE_ISP_START                 = 3,
    IC_EVENT_TYPE_ISP_END                   = 4,
    IC_EVENT_TYPE_ZSL_LOCKED                = 5, //TODO: working
    IC_EVENT_TYPE_STATS_READY               = 6,
    IC_EVENT_TYPE_SOURCE_STOPPED            = 7,
    IC_EVENT_TYPE_ISP_CONFIG_ACCEPTED       = 8, // TODO:
    IC_EVENT_TYPE_ERROR                     = 9, // TODO:
    IC_EVENT_TYPE_LEON_RT_READY             = 10,
    IC_EVENT_TYPE_SOURCE_READY              = 11,
    IC_EVENT_TYPE_SETUP_SOURCES_RESULT      = 20, //TODO:
    IC_EVENT_TYPE_ZSL_ADD_RESULT            = 21,
    IC_EVENT_TYPE_ALLOC_MEM                 = 22, //TODO
    IC_EVENT_TYPE_SEND_OUTPUT_DATA          = 18,
    IC_EVENT_TYPE_TORN_DOWN                 = 15,

    /* Client -> IPIPE */
    IC_EVENT_TYPE_SETUP_SOURCE              = 32,
    IC_EVENT_TYPE_SETUP_SOURCES_COMMIT      = 33,
    IC_EVENT_TYPE_CONFIG_SOURCE             = 42,
    IC_EVENT_TYPE_CONFIG_SOURCE_DYNAMIC     = 43,
    //IC_EVENT_TYPE_CONFIG_OUTPUT           = 52 // TODO: found how to use it
    IC_EVENT_TYPE_CONFIG_ISP                = 44,
    IC_EVENT_TYPE_START_SOURCE              = 45,
    IC_EVENT_TYPE_STOP_SOURCE               = 46,
    IC_EVENT_TYPE_LOCK_ZSL                  = 47, //TODO: working
    IC_EVENT_TYPE_CAPTURE                   = 48, //TODO
    IC_EVENT_TYPE_UNLOCK_ZSL                = 49, //TODO working
    IC_EVENT_TYPE_ZSL_ADD                   = 51, //TODO
    IC_EVENT_TYPE_TEAR_DOWN                 = 80,
    IC_EVENT_TYPE_MEM_ALLOCATED             = 52,
    IC_EVENT_TYPE_OUTPUT_DATA_RECEIVED      = 50,
} icEventType;

typedef struct icEventS{
    union {
        /*------------------------------------------------------------------*/
        /* Events: IPIPE -> Client */
        struct {
            /*
             * This struct is used for the following events:
             * - IC_EVENT_TYPE_READOUT_START
             * - IC_EVENT_TYPE_READOUT_END
             * - IC_EVENT_TYPE_LINE_REACHED
             */
            icSourceInstance   sourceInstance;
            void               *userData;
            uint32_t           seqNo;
            uint64_t        ts;
        } lineEvent;
        struct {
            /*
             * This struct is used for the following events:
             * - IC_EVENT_TYPE_ISP_START
             * - IC_EVENT_TYPE_ISP_END
             * - IC_EVENT_TYPE_ISP_CONFIG_ACCEPTED
             */
            uint32_t            ispInstance;
            void                *userData;
            uint32_t            seqNo;
        } ispEvent;
        struct {
            /* IC_EVENT_TYPE_STATS_READY */
            uint32_t            ispInstance;
            void                *userData;
            uint32_t            seqNo;
        } statsReady;
        struct {
            /* IC_EVENT_TYPE_SOURCE_STOPPED */
            /* IC_EVENT_TYPE_SOURCE_READY */
            uint32_t            sourceInstance;
        } sourceEvent;
        struct {
            /* IC_EVENT_TYPE_ERROR */
            uint32_t            sourceInstance;
            icSeverity          severity;
            icError             errorNo;
            void                *userData;
        } error;
        struct {
            // IC_EVENT_TYPE_ZSL_LOCKED
            uint32_t            sourceInstance;
            void                *userData;
            void                *buffZsl;
            uint32_t            seqNo;
            uint64_t            ts;
        } buffLockedZSL;
        struct {
            /* IC_EVENT_TYPE_ZSL_ADD_RESULT */
            uint32_t            memFree;
            uint32_t            totalNoOfZSLframes;
            icAddZSLStatusCode  status;
        } addZSLresult;

        /*------------------------------------------------------------------*/
        /* Events: Client -> IPIPE */
        struct {
            /* IC_EVENT_TYPE_CONFIG_SOURCES_RESULT */
            icStatusCode        status;
            uint32_t            memFree;
        }configSourcesResult;
        struct {
            /* IC_EVENT_TYPE_CONFIG_SOURCE_DYNAMIC */
            uint32_t            sourceInstance;
            icSourceConfigDynamic *pDynConfig;
        } configureSourceDynamic;
        struct {
            /* IC_EVENT_TYPE_START_SOURCE */
            uint32_t            sourceInstance;
        } start;
        struct {
            /* IC_EVENT_TYPE_STOP_SOURCE */
            uint32_t            sourceInstance;
            int32_t             stopImmediate;
        } stop;
        struct {
            /* IC_EVENT_TYPE_CONFIG_ISP */
            uint32_t            ispInstance;
            uint32_t            pConfigStruct;
            uint32_t            seqNo;
        } configureIsp;
        struct {
            /* IC_EVENT_TYPE_LOCK_ZSL */
            uint32_t            sourceInstance;
            uint64_t            frameSel;
            uint32_t            flags;
        } lockZSL;
        struct {
            /* IC_EVENT_TYPE_CAPTURE */
            uint32_t            sourceInstance;
            void                *buff;
            void                *pConfigStruct;
            icCaptureFlags      flags;
        } capture;
        struct {
            /* IC_EVENT_TYPE_UNLOCK_ZSL */
            uint32_t            sourceInstance;
            FrameT              *buff;
        } unlockZSL;
        struct {
            /* IC_EVENT_TYPE_ZSL_ADD */
            uint32_t            sourceInstance;
            uint32_t            addZSLframesNo;
        } addZSL;
        struct {
            //IC_EVENT_TYPE_SEND_OUTPUT_DATA
            FrameT             *dataBufStruct;
            uint32_t            outputId;
        }sentOutput;
        struct {
            //IC_EVENT_TYPE_OUTPUT_DATA_RECEIVED
            FrameT              *dataBufStruct;
        } dataWasSent;
        struct {
            void                *eventStruct;
            uint32_t            id;
        } generalEvent;

        struct {
            /* IC_EVENT_TYPE_SETUP_SOURCE */
            uint32_t        sourceInstance;
            icSourceSetup  *srcSetup;
        } setupSourceMsg;
        struct {
            /* IC_EVENT_TYPE_CONFIG_SOURCE */
            uint32_t        sourceInstance;
            icSourceConfig  *srcCfg;
        } configureSource;

    } u;

    volatile uint32_t   ctrl;   /* See IC_EVENT_CTRL_* */
} icEvent;

enum { // OutputAtributType
    IC_OUTPUT_FRAME_DATA_TYPE_PREVIEW,
    IC_OUTPUT_FRAME_DATA_TYPE_STILL,
    IC_OUTPUT_FRAME_DATA_TYPE_STILL_RAW,
    IC_OUTPUT_FRAME_DATA_TYPE_METADATA_PRV,
    IC_OUTPUT_FRAME_DATA_TYPE_METADATA_STILL,
    // this one is not directly linked with 1 specific source, can depend by multiple source
    // previous defines are isp specific defines, directly reported to 1 single source
    IC_OUTPUT_FRAME_DATA_TYPE_USER1,
    IC_OUTPUT_FRAME_DATA_TYPE_USER2,
    IC_OUTPUT_FRAME_DATA_TYPE_USER3,
};

/* Primary IPIPE control structure shared between IPIPE and Client */
    //
typedef struct icCtrlStruct {
    int32_t appId;
    //
    icStatus status;
    icSource                source[IPIPE_MAX_SOURCES_ALLOWED];
} icCtrl;

/* Assert/Debug/Panic/Warn macros */
#if !defined(__sparc)
#define IC_ASSERT(x)    { if (!(x)) { IC_PRINTF("IC_ASSERT fail: \"%s\" @ line %d in %s\n", #x, __LINE__, __FILE__); abort(); } }
#define IC_PANIC(m, x)  { IC_PRINTF("IC_PANIC: %s (0x%08x) @ line %d in %s\n", (m), (x), __LINE__, __FILE__); abort(); }
#define IC_PRINTF   printf
#else //Myriad:
#define IC_ASSERT(x)    { if (!(x)) { IC_PRINTF("IC_ASSERT fail: \"%s\" @ line %d in %s\n", #x, __LINE__, __FILE__); asm("ta 1"); } }
#define IC_PANIC(m, x)  { IC_PRINTF("IC_PANIC fail: %s (0x%08x) @ line %d in %s\n", (m), (x), __LINE__, __FILE__); asm("ta 1"); }
#define IC_PRINTF   printf
#endif



typedef struct {
    uint32_t status;
    uint32_t memFree;
}icSetupSourcesResult;

#define IC_WARN(m, x)   { IC_PRINTF("IC_WARN: %s (0x%08x) @ line %d in %s\n", (m), (x), __LINE__, __FILE__); }

icCtrl  *icSetup                (int32_t appID,    uint32_t ddrStart,  uint32_t ddrSize);
//void    *icQuery                (icCtrl *ctrl, icQueryType queryType, int32_t index);
void    icSetupSource           (icCtrl *ctrl, icSourceInstance srcIdx, icSourceSetup *setup);
void    icSetupSourcesCommit    (icCtrl *ctrl);
void    icConfigureSource       (icCtrl *ctrl, icSourceInstance srcIdx, icSourceConfig *config);
//int32_t icConfigureSourceDynamic(icCtrl *ctrl, icSourceInstance source, icSourceConfigDynamic *config);
int32_t icStartSource           (icCtrl *ctrl, icSourceInstance source);
int32_t icStopSource            (icCtrl *ctrl, icSourceInstance source);
int32_t icConfigureIsp          (icCtrl *ctrl, int32_t ispIdx, void *cfg);
void    icDataReceived          (icCtrl *ctrl, FrameT   *dataBufStruct);
int32_t icLockZSL               (icCtrl *ctrl, icSourceInstance source, uint64_t frameSel, icLockZSLFlags flags); //TODO: implement it
int32_t icTriggerCapture        (icCtrl *ctrl, icSourceInstance source, void * buff,void *cfg, icCaptureFlags flags); //TODO: implement it
int32_t icUnlockZSL             (icCtrl *ctrl, icSourceInstance source, FrameT *buff); // TODO: implement it
int32_t icTeardown              (icCtrl *ctrl);
int32_t icIsEventPending        (icCtrl *ctrl);
int32_t icGetEvent              (icCtrl *ctrl, icEvent *ev);
void    icSendHpCommand         (icCtrl *ctrl, icHpCmd cmd); // TODO: probably delete it
//void    icConfigureOutput       (icCtrl *ctrl,int32_t outputIdx, icOutputConfig *config); //TODO: implement it !
int32_t icZSLAdd                (icCtrl *ctrl, icSourceInstance source, uint32_t ZSLframesToAdd);



static inline int getTimeStamps(uint64_t *time) {
    struct timeval tv;
    int ret;
    ret = gettimeofday(&tv, NULL);
    *time = (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
    return ret;
}


#ifdef __cplusplus
}
#endif



#endif  /* _IPIPE_H */
