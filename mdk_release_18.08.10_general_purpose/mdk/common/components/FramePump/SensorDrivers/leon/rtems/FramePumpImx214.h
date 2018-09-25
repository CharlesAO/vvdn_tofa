///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief Sensor driver API for IMX214
///

#ifndef _FRAMEPUMPIMX214_H_
#define _FRAMEPUMPIMX214_H_

#include "FramePump.h"

#define IMX214_MAX_WIDTH    (4208)
#define IMX214_MAX_HEIGHT   (3120)

#define IMX214_I2C_ADDRESS  (0x20 >> 1)

extern const struct FramePumpSensorDriver framePumpimx214_binning_camCfg;
extern const struct FramePumpSensorDriver framePumpimx214_camCfg;

#endif // _FRAMEPUMPIMX214_H_
