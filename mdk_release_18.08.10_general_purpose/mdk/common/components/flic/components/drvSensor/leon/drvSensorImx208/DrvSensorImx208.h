///
/// @file      DrvSensorImx208.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Sensor driver API for IMX208
///

#ifndef _DRVSENSORIMX208_H_
#define _DRVSENSORIMX208_H_

#include "DrvSensorDefines.h"
#include <rtems/libi2c.h>
#include <rtems/libio.h>
//#include <rtems/shell.h>
//#include <sys/ioctl.h>
//#include <fcntl.h> // open, close .


#define IMX208_I2C_ADDRESS_LEFT      (0x6C >> 1)
#define IMX208_I2C_ADDRESS_RIGHT     (0x6E >> 1)

extern const struct DrvSensorCamSpec drvSensorImx208_RAW8_binning_camCfg;
extern const struct DrvSensorCamSpec drvSensorimx208_RAW10_binning_camCfg;
extern const struct DrvSensorCamSpec drvSensorImx208_RAW8_camCfg;
extern const struct DrvSensorCamSpec drvSensorimx208_RAW10_camCfg;

extern struct DrvSensorControlFunctions drvSensorImx208Control;
extern rtems_libi2c_drv_t i2cImx208ProtocolDrvTbl;
#endif // _DRVSENSORIMX208_H_
