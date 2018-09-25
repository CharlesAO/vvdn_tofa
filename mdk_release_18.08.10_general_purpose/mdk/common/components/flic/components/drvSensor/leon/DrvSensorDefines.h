///
/// @file      DrvSensorDefines.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     This is the file that contains all the Sensor definitions of constants,
///            typedefs, structures, enums or exported variables from this module.
///

#ifndef _DRV_SENSOR_CAMERA_DEFINES_H_
#define _DRV_SENSOR_CAMERA_DEFINES_H_

#include <mv_types.h>
#include <mvMacros.h>
#include "DrvMipiDefines.h"
#include "swcFrameTypes.h"


enum DrvSensori2cConfigType_t
  {
    DRV_SENSOR_FULL_CONFIG,
    DRV_SENSOR_COLORBAR,
    DRV_SENSOR_START_STREAMING,
    DRV_SENSOR_STOP_STREAMING,
    DRV_SENSOR_STANDBY,
    DRV_SENSOR_WAKEUP,
    DRV_SENSOR_INTEGRATION_TIME,
    DRV_SENSOR_GAIN,
    // to be continued
};

typedef struct DrvSensorI2CConfigDescriptor
{
  const u16 (*regValues)[2];
  enum DrvSensori2cConfigType_t configType;
  u32 numberOfRegs;
  u32 delayMs;
} DrvSensorI2CConfigDescriptor;

enum DrvSensorgpioConfigType_t
{
    DRV_SENSOR_END = 0,
    DRV_SENSOR_POWER_PIN,
    DRV_SENSOR_SHUTDOWN_PIN,
    DRV_SENSOR_RESET_PIN,
    DRV_SENSOR_MCLK_PIN, //TODO: not yet supported
    // to be continued
};

struct DrvSensorGpioConfigDescriptor
{
    enum DrvSensorgpioConfigType_t configType;
    u8 gpioNumber;
    u8 activeLevel;
    u32 delayMs;
};

struct DrvSensorMipiSpec
{
   eDrvMipiDataType pixelFormat;
   u32              dataRateMbps;
   u32              nbOflanes;
   eDrvMipiDataMode dataMode;
};                      //set all fields to 0 for parallel interface

// TODO: move it in common  type areea
typedef struct {
  int32_t     x1;
  int32_t     y1;
  int32_t     x2;
  int32_t     y2;
} RectT;

typedef struct {
    uint32_t    w;
    uint32_t    h;
} SizeT;

struct DrvSensorCamSpec
{
    RectT            outFOV;
    SizeT            outSize;
    u32              hBackPorch;
    u32              hFrontPorch;
    u32              vBackPorch;
    u32              vFrontPorch;
    const struct DrvSensorMipiSpec   *mipiCfg;
    frameType        internalPixelFormat;
    u32              bitsPerPixel;
    u32              bytesPerPixel;
    u32              idealRefFreq;

    u32              sensorI2CAddress1;
    u32              sensorI2CAddress2;
    u32              regSize;

    const DrvSensorI2CConfigDescriptor *i2cConfigSteps;
    u32              nbOfI2CConfigSteps;
};

#endif // _DRV_SENSOR_CAMERA_DEFINES_H_
