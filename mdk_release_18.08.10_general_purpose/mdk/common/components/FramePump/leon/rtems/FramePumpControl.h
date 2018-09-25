///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief FramePump control functions API
///

#ifndef MODULES_FRAMEPUMP_FRAMEPUMPCONTROL_H_
#define MODULES_FRAMEPUMP_FRAMEPUMPCONTROL_H_

#include <FramePumpPrivate.h>

extern struct FramePumpControlFunctions framePumpDefaultControl;

rtems_status_code framePumpSensorOpen (struct FramePumpVideoContext *ctx);
rtems_status_code framePumpSensorClose (struct FramePumpVideoContext *ctx);

rtems_status_code framePumpSensorConfig (struct FramePumpVideoContext *ctx,
                                        struct FramePumpControlConfig *control);

rtems_status_code framePumpSensorPowerDown (struct FramePumpVideoContext *ctx);

rtems_status_code framePumpSensorPowerUp (struct FramePumpVideoContext *ctx);

#endif /* MODULES_FRAMEPUMP_FRAMEPUMPCONTROL_H_ */
