///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief Sensor driver API for IMX378
///

#ifndef _FRAMEPUMPIMX378_H_
#define _FRAMEPUMPIMX378_H_

#include <rtems/libi2c.h>
#include "FramePump.h"

#define IMX378_MAX_WIDTH    (4056)
#define IMX378_MAX_HEIGHT   (3040)

#define IMX378_I2C_ADDRESS  (0x34 >> 1)

extern const struct FramePumpSensorDriver framePumpimx378_camCfg;
extern const rtems_libi2c_drv_t imx378ProtocolDrvTbl;

#endif // _FRAMEPUMPIMX378_H_
