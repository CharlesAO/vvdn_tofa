///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief FramePump main API
///

#ifndef FRAMEPUMP_H__
#define FRAMEPUMP_H__

#include <rtems.h>
#include <mv_types.h>
#include <FramePumpDefines.h>
#ifndef MA2X8X
#include <DrvMipi.h>
#else
#include "OsDrvMipi.h"
#endif

struct FramePumpHwContext {
    uint32_t irqVector;              // IRQ vector number
    uint32_t lineIrqVector;          // IRQ line vector number (for SIPP)
    uint32_t clk;                    // HW clock bit
#ifndef MA2X8X
    enum drvMipiCtrlNo ctrlNo; // MIPI controller number
#else
#endif
  uint32_t frameCount;             // Track frame count
};

typedef struct FramePumpBuffer {
    frameBuffer     fbMem;          // Memory frame buffer
    frameBuffer     fb;             // Normal frame buffer
    uint64_t        timestampNs;
    uint32_t        frameCount;
    uint32_t        exposureNs;
    float           gain;
    uint32_t        cam_id;
} FramePumpBuffer;

#define FP_SHARED_CTL_ID    (0x80000000)

#define FP_SHARED_CTL(x)    ((x) | FP_SHARED_CTL_ID)
#define FP_IS_SHARED_CTL(x) ((x) & FP_SHARED_CTL_ID)
#define FP_STRIP_CTL(x)     ((x) & (~FP_SHARED_CTL_ID))

typedef enum {
    FRAME_PUMP_CONFIG_SEND_CONFIG       = 0x01, // framePumpSensorConfig()
    FRAME_PUMP_START                    = 0x02, // FramePumpControl()
    FRAME_PUMP_STOP                     = 0x03, // FramePumpControl()
    FRAME_PUMP_CONFIG_SEND_EXPOSURE     = 0x04, // framePumpSensorOv7750Config() & framePumpSensorOv9282Config()
    FRAME_PUMP_CONFIG_SEND_GAIN         = 0x05, // framePumpSensorOv7750Config() & framePumpSensorOv9282Config()
    FRAME_PUMP_CONFIG_TESTPATTERN       = 0x06, // framePumpSensorConfig()
    FRAME_PUMP_GET_TESTPATTERN_CRC      = 0x07, // framePumpSensorImx208Config(), framePumpSensorImx214Config(), framePumpSensorImx378Config(), framePumpSensorOv7750Config() & framePumpSensorOv9282Config()
    FRAME_PUMP_GET_MEMORY_FRAME_SPEC    = 0x08, // FramePumpControl()
    FRAME_PUMP_GET_FRAME_SPEC           = 0x09, // FramePumpControl()
    FRAME_PUMP_SET_FRAME_SPEC           = 0x10, // TODO ?
    FRAME_PUMP_GET_FRAMERATE            = 0x11, // framePumpSensorOv7750Config() & framePumpSensorOv9282Config()
    FRAME_PUMP_SET_FRAMERATE            = 0x12, // framePumpSensorOv7750Config() & framePumpSensorOv9282Config()
    FRAME_PUMP_CONFIG_GET_READOUT       = 0x13, // framePumpSensorOv7750Config() & framePumpSensorOv9282Config()
    FRAME_PUMP_GET_MAX_EXPOSURE         = 0x14, // framePumpSensorOv7750Config() & framePumpSensorOv9282Config()
    FRAME_PUMP_SET_MAINS_FREQ           = 0x20, // TODO ? On Mains Frequency set, the sensor frame-rate is changed respectively to avoid flicker
    FRAME_PUMP_CONTROL_START            = 0x22, // framePumpSensorConfig()
    FRAME_PUMP_CONTROL_STOP             = 0x23, // framePumpSensorConfig()
    FRAME_PUMP_CONFIG_FLIP              = 0x24, // framePumpSensorOv7750Config() & framePumpSensorOv9282Config()
    FRAME_PUMP_CONFIG_MIRROR            = 0x25, // framePumpSensorOv7750Config() & framePumpSensorOv9282Config()
    FRAME_PUMP_CONFIG_ENABLE_METADATA   = 0x30, // framePumpSensorConfig()
    FRAME_PUMP_CONFIG_DISABLE_METADATA  = 0x31, // TODO ?
    FRAME_PUMP_GET_EXPGAIN_TABLE        = 0x44, // framePumpSensorOv7750Config() & framePumpSensorOv9282Config()
    FRAME_PUMP_CONFIG_SEND_EXPGAIN      = 0x45, // Set new Exposure & Gain simultaneously in the camera. framePumpSensorOv7750Config() & framePumpSensorOv9282Config()
    FRAME_PUMP_GET_EXPGAIN_INFO         = 0x46  // Set new Exposure & Gain simultaneously in the camera. framePumpSensorOv7750Config() & framePumpSensorOv9282Config()
} FramePumpCommand;

struct FramePumpControlConfig {
    uint32_t cfgType;
    void *buffer;
};

struct FramePumpGpioDescriptor {
    char * name;
    uint8_t gpioNumber;
    enum FramePumpgpioConfigType_t configType;
    uint8_t activeLevel;
    uint32_t delayUs;
};

struct FramePumpOutSpec {
    frameSpec spec;
    uint32_t x0;
    uint32_t y0;
    uint32_t hasMetadata;
};

struct FramePumpVideoDevice {
    char *name;

    char *controlDevice;
    char *sharedControlDevice;

    const struct FramePumpHwFunc *hwFnc;

    uint32_t interruptLevel;
};

struct FramePumpVideoDeviceSensor {
  struct FramePumpVideoDevice vd;
  int ctrlNo;
  struct FramePumpGpioDescriptor *gpioa;
  const struct FramePumpSensorDriver *spec;
};

// RTEMS device driver function table
extern rtems_driver_address_table FramePumpDrvTblName;

typedef void *FramePumpSensorSpecificData;

#endif
