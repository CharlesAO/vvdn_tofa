///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief Sensor driver API for OmniVision 9282
///
/// 640x480p @ 30 fps RAW10

#ifndef _FRAMEPUMPOV9282_H_
#define _FRAMEPUMPOV9282_H_

#include "FramePumpDefines.h"
#include <rtems/libi2c.h>

#define OV9282_I2C_ADDRESS_LEFT                 (0x61)
#define OV9282_I2C_ADDRESS_RIGHT                (0x62)
#define OV9282_I2C_ADDRESS_BROADCAST            (0x71)
#define OV9282_I2C_ADDRESS_LEFT_SID             (0x10)
#define OV9282_I2C_ADDRESS_RIGHT_SID            (0x60)
#define OV9282_I2C_ADDRESS_BROADCAST_DEFAULT    (0x70)

#define OV9282_WIDTH                            (1280)
#define OV9282_HEIGHT                           (720)
#define OV9282_BINNED_WIDTH                            (640)
#define OV9282_BINNED_HEIGHT                           (400)

extern const struct FramePumpSensorDriver framePumpOv9282Raw10_camCfg;
extern const struct FramePumpSensorDriver framePumpOv9282Raw8_camCfg;

extern const struct FramePumpSensorDriver framePumpOv9282Raw10_left_camCfg;
extern const struct FramePumpSensorDriver framePumpOv9282Raw10_right_camCfg;

extern const struct FramePumpSensorDriver framePumpOv9282Raw8_left_camCfg;
extern const struct FramePumpSensorDriver framePumpOv9282Raw8_right_camCfg;

extern const struct FramePumpSensorDriver framePumpOv9282Raw10_720P_camCfg;
extern const struct FramePumpSensorDriver framePumpOv9282Raw8_720P_camCfg;

extern const struct FramePumpSensorDriver framePumpOv9282Raw10_720P_left_camCfg;
extern const struct FramePumpSensorDriver framePumpOv9282Raw10_720P_right_camCfg;

extern const struct FramePumpSensorDriver framePumpOv9282Raw8_720P_left_camCfg;
extern const struct FramePumpSensorDriver framePumpOv9282Raw8_720P_right_camCfg;

extern struct FramePumpControlFunctions framePumpOv9282Control;

extern rtems_libi2c_drv_t libi2cOv9282BroadcastProtocolDrvTbl;
extern rtems_libi2c_drv_t libi2cOv9282ProtocolDrvTbl;

#endif // _IMGOV9282CFGVGA100RAW10CB_H_
