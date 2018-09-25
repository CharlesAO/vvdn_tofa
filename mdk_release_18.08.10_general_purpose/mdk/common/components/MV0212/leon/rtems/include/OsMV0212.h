///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @defgroup MV0212RTEMS MV0212 RTEMS API
/// @ingroup  MV0212
/// @{
/// @brief     API specific for the MV0212 RTEMS Board Component
///
/// The RTEMS component has the following features:
/// - Initialise the GPIOs and I2C devices on the MV0212 board
///     - The user can choose in which mode to configure the I2C bus (default is master, slave not yet supported)\n
///       Moreover, the user can choose which I2C bus shall control the CAM B on the board (I2C0 - default, or I2C1)
///     - Custom GPIO configuration can be provided by the user
///     - Custom I2C device configuration is not yet supported
/// - Retrieve the board revision
/// 
/// Usage example:
/// @code{.c}
/// int32_t status;
/// uint32_t revision;
/// BoardGPIOConfig gpioConfig = {
///                                 { 0, 14, ACTION_UPDATE_ALL, PIN_LEVEL_LOW, 
///                                   D_GPIO_MODE_7       | D_GPIO_DIR_IN    | 
///                                   D_GPIO_DATA_INV_OFF | D_GPIO_WAKEUP_OFF,
///                                   D_GPIO_PAD_DEFAULTS                    },
///                                 { 0, 0 , ACTION_TERMINATE_ARRAY, 0, 0, 0 }
///                              };
/// 
/// BoardI2CDevConfiguration brdCfg[] = 
/// {
///     { MV212_END, 0 }
/// };
/// 
/// BoardConfigDesc config[] = 
/// {
///     {
///         BRDCONFIG_GPIO,
///         (void *)gpioConfig
///     },
///     {
///         BRDCONFIG_I2CDEVLIST,
///         (void *)brdCfg,
///     },
///     {
///         BRDCONFIG_END,
///         NULL
///     }
/// };
///
/// status = BoardInit(&config);
/// if(status)
///     exit(status);
///
/// status = BoardGetPCBRevision(&revision);
/// if(status)
///     exit(status);
///
/// @endcode
///

#ifndef _OS_MV0212_
#define _OS_MV0212_

#include "MV0212Common.h"
#include "OsMV0212Defines.h"

/// @}

#endif
