///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief
///
/// This is the file that contains all the Sensor definitions of constants, typedefs,
/// structures or enums or exported variables from this module

#ifndef _FRAME_PUMP_CAMERA_DEFINES_H_
#define _FRAME_PUMP_CAMERA_DEFINES_H_

#include <mv_types.h>
#include <mvMacros.h>
#ifdef MA2X8X
#include "OsDrvMipi.h"
#else
#include "DrvMipiDefines.h"
#endif
#include "swcFrameTypes.h"

#define FP_EXPGAIN_TBL_MAX_SZ   (8) // Max entries in {Exposure,Gain} regions table

#define FP_MAINS_FREQ_50HZ      (50)
#define FP_MAINS_FREQ_60HZ      (60)

enum FramePumpi2cConfigType_t {
    FRAME_PUMP_FULL_CONFIG,
    FRAME_PUMP_COLORBAR,
    FRAME_PUMP_START_STREAMING,
    FRAME_PUMP_STOP_STREAMING,
    FRAME_PUMP_STANDBY,
    FRAME_PUMP_WAKEUP,
    FRAME_PUMP_INTEGRATION_TIME,
    FRAME_PUMP_GAIN,
    FRAME_PUMP_ENABLE_METADATA,
    FRAME_PUMP_DISABLE_METADATA,
    // to be continued
};

typedef struct FramePumpI2CConfigDescriptor {
    const uint16_t (*regValues)[2];
    enum FramePumpi2cConfigType_t configType;
    uint32_t numberOfRegs;
    uint32_t delayMs;
} FramePumpI2CConfigDescriptor;

enum FramePumpgpioConfigType_t {
    FRAME_PUMP_END = 0,
    FRAME_PUMP_POWER_PIN,
    FRAME_PUMP_SHUTDOWN_PIN,
    FRAME_PUMP_RESET_PIN,
    FRAME_PUMP_MCLK_PIN, //TODO: not yet supported
    // to be continued
};

struct FramePumpGpioConfigDescriptor {
    enum FramePumpgpioConfigType_t configType;
    uint8_t gpioNumber;
    uint8_t activeLevel;
    uint32_t delayMs;
};

typedef struct FramePumpDatapathMipiSpec {
#ifdef MA2X8X
  OsDrvMipiCsi2DataType pixelFormat;
#else
  eDrvMipiDataType pixelFormat;
#endif
    uint32_t              dataRateMbps;
    uint32_t              nbOflanes;
#ifdef MA2X8X
  OsDrvMipiDataMode dataMode;
#else
  eDrvMipiDataMode dataMode;
#endif
} FramePumpDatapathMipiSpec;

typedef struct FramePumpDatapathParallelSpec {
    frameType pixelFormat;
    uint32_t hBackPorch;
    uint32_t hFrontPorch;
    uint32_t vBackPorch;
    uint32_t vFrontPorch;
} FramePumpDatapathParallelSpec;

typedef enum FramePumpDatapathType {
    FP_UNKNOWN = 0,
    FP_MIPI,
    FP_PARALLEL,
    FP_OTHER,
} FramePumpDatapathType;

typedef union FramePumpDatapathSpec {
    FramePumpDatapathParallelSpec parallel;
    FramePumpDatapathMipiSpec mipi;
} FramePumpDatapathSpec;

typedef struct FramePumpCamExpGain_t {
    uint32_t     exposureTimeUs;
    float   gainMultiplier;
} FramePumpCamExpGain;

typedef struct FramePumpCamExpGainInfo_t {
    FramePumpCamExpGain min;
    FramePumpCamExpGain max;
    FramePumpCamExpGain start;
    uint32_t                 gainFractBits;
} FramePumpCamExpGainInfo;

typedef struct FramePumpCamExpGainTable_t {
    FramePumpCamExpGain region[FP_EXPGAIN_TBL_MAX_SZ];
} FramePumpCamExpGainTable;

#endif // _CAMERA_API_DEFINES_H_
