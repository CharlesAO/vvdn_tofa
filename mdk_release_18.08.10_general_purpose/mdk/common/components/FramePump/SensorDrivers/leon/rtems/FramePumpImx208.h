///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief Sensor driver API for IMX208
///

#ifndef _FRAMEPUMPIMX208_H_
#define _FRAMEPUMPIMX208_H_

#include "FramePump.h"

#define IMX208_MAX_WIDTH            (1936)
#define IMX208_MAX_HEIGHT           (1096)

#define IMX208_I2C_ADDRESS_LEFT     (0x6c >> 1) // unused; TODO use these inside the default board I2C discovery
#define IMX208_I2C_ADDRESS_RIGHT    (0x6e >> 1) //

extern const struct FramePumpSensorDriver framePumpimx208_binning_camCfg;
extern const struct FramePumpSensorDriver framePumpimx208_camCfg;

#endif // _FRAMEPUMPIMX208_H_
