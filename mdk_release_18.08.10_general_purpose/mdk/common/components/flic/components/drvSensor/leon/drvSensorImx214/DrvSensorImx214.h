///
/// @file      DrvSensorImx214.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Sensor driver API for IMX214.
///

#ifndef _DRVSENSORIMX214_H_
#define _DRVSENSORIMX214_H_

#include "DrvSensorDefines.h"
#include <rtems/libi2c.h>
#include <rtems/libio.h>
//#include <rtems/shell.h>
//#include <sys/ioctl.h>
//#include <fcntl.h> // open, close .


#define IMX214_I2C_ADDRESS_DRV_SENSOR      (0x20 >> 1)

extern const struct DrvSensorCamSpec drvSensorimx214_RAW10_binning_camCfg;
extern const struct DrvSensorCamSpec drvSensorimx214_RAW10_camCfg;

extern struct DrvSensorControlFunctions drvSensorImx214Control;
extern rtems_libi2c_drv_t i2cImx214ProtocolDrvTbl;
#endif // _DRVSENSORIMX214_H_
