///
/// @file      DrvSensorControl.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     DrvSensor control header.
///

#ifndef MODULES_DRVSENSOR_DRVSENSORCONTROL_H_
#define MODULES_DRVSENSOR_DRVSENSORCONTROL_H_

#include "leon/DrvSensor.h"

extern struct DrvSensorControlFunctions drvSensorDefaultControl;


int drvSensorSensorOpen (struct DrvSensorVideoContext *ctx);
int drvSensorSensorClose (struct DrvSensorVideoContext *ctx);

int drvSensorSensorConfig (struct DrvSensorVideoContext *ctx,
                           struct DrvSensorCotrolConfig *control);

int drvSensorSensorPowerDown (struct DrvSensorVideoContext *ctx);

int drvSensorSensorPowerUp (struct DrvSensorVideoContext *ctx);

#endif /* MODULES_DRVSENSOR_DRVSENSORCONTROL_H_ */
