///
/// @file      DrvSensorOv7251.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Sensor driver API for OmniVision 7750 & 7251.
///            640x480p @ 30 fps RAW10
///

#ifndef _FRAMEPUMPOV7251_H_
#define _FRAMEPUMPOV7251_H_

#include "DrvSensorDefines.h"

#define OV7750_I2C_ADDRESS_LEFT      0x61
#define OV7750_I2C_ADDRESS_RIGHT     0x62
#define OV7750_I2C_ADDRESS_BROADCAST 0x71

#define OV7750_WIDTH  640
#define OV7750_HEIGHT 480

#define DRV_SENSOR_CONFIG_GET_READOUT 0x23

#define DRV_SENSOR_CONFIG_FLIP   0x24
#define DRV_SENSOR_CONFIG_MIRROR 0x25

extern const struct DrvSensorCamSpec drvSensorOv7251Raw10_camCfg;
extern const struct DrvSensorCamSpec drvSensorOv7251Raw8_camCfg;

extern const struct DrvSensorCamSpec drvSensorOv7251Raw10_left_camCfg;
extern const struct DrvSensorCamSpec drvSensorOv7251Raw10_right_camCfg;

extern const struct DrvSensorCamSpec drvSensorOv7251Raw8_left_camCfg;
extern const struct DrvSensorCamSpec drvSensorOv7251Raw8_right_camCfg;

extern struct DrvSensorControlFunctions drvSensorOv7750Control;

#endif // _IMGOV7750CFGVGA100RAW10CB_H_
