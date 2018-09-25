///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief Sensor driver API for OmniVision 7750
///
/// 640x480p @ 30 fps RAW10

#ifndef _FRAMEPUMPOV7750_H_
#define _FRAMEPUMPOV7750_H_

#include "FramePumpDefines.h"
#include <rtems/libi2c.h>

#define OV7750_I2C_ADDRESS_LEFT         (0x61)
#define OV7750_I2C_ADDRESS_RIGHT        (0x62)
#define OV7750_I2C_ADDRESS_BROADCAST    (0x71)

#define OV7750_WIDTH                    (640)
#define OV7750_HEIGHT                   (480)

extern const struct FramePumpSensorDriver framePumpOv7750Raw10_camCfg;
extern const struct FramePumpSensorDriver framePumpOv7750Raw8_camCfg;

extern const struct FramePumpSensorDriver framePumpOv7750Raw10_left_camCfg;
extern const struct FramePumpSensorDriver framePumpOv7750Raw10_right_camCfg;

extern const struct FramePumpSensorDriver framePumpOv7750Raw8_left_camCfg;
extern const struct FramePumpSensorDriver framePumpOv7750Raw8_right_camCfg;

extern struct FramePumpControlFunctions framePumpOv7750Control;

extern rtems_libi2c_drv_t libi2cOv7750BroadcastProtocolDrvTbl;
extern rtems_libi2c_drv_t libi2cOv7750ProtocolDrvTbl;

#endif // _IMGOV7750CFGVGA100RAW10CB_H_
