///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @defgroup MV0212BM MV0212 BM API
/// @ingroup  MV0212
/// @{
/// @brief     API specific for the MV0212 BM Board Component
///
/// The bare metal component has the following features:
/// - Initialise the GPIOs and I2C devices on the MV0212 board
///     - Custom GPIO configuration can be provided by the user
///     - Custom I2C device configuration is not yet supported
/// - Retrieve the board revision
/// - Read-out I2C interface information
/// - Set the external PLL (mainly used to provide clock for cameras on the board)
/// 
/// Usage example:
/// @code{.c}
/// #include "DrvCDCEL.h"
///
/// uint32_t revision;
/// int32_t status;
///
/// // The number of I2C information handlers matches with the wanted  
/// // number of bus interfaces we want to get information from
/// BoardI2CInfo info[3];
/// BoardGPIOConfig gpioConfig = {
///                                 { 0, 14, ACTION_UPDATE_ALL, PIN_LEVEL_LOW, 
///                                   D_GPIO_MODE_7       | D_GPIO_DIR_IN    | 
///                                   D_GPIO_DATA_INV_OFF | D_GPIO_WAKEUP_OFF,
///                                   D_GPIO_PAD_DEFAULTS                    },
///                                 { 0, 0 , ACTION_TERMINATE_ARRAY, 0, 0, 0 }
///                              };
/// BoardConfigDesc config[] = 
///    {
///        {
///            BRDCONFIG_GPIO,
///            (void *)gpioConfig
///        },
///        {
///            BRDCONFIG_END,
///            NULL
///        }
///    };
///
/// status = BoardInit(&config);
/// if(status)
///     exit(status);
/// 
/// status = BoardGetPCBRevision(&revision);
/// if(status)
///     exit(status);
///
/// status = BoardGetI2CInfo(info, 3);
/// if(status)
///     exit(status);
///
/// status = BoardInitExtPll(EXT_PLL_CFG_74_24_24MHZ);
/// if(status)
///     exit(status);
///
/// @endcode
///

#ifndef _MV0212_
#define _MV0212_

#include "MV0212Common.h"
#include "MV0212Defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Configures the External PLL to a given frequency
///
/// @param[in] clock_config (See "DrvCDCEL.h" for usable indexes)
/// @return    
///         - BRDCONFIG_SUCCESS
///         - non-zero on fail
///
int32_t BoardInitExtPll(uint32_t clock_config);

/// @brief Initialise the default configuration for I2C0, I2C1, I2C2 on the MV0212 Board 
/// and return the device information
///
/// @param[out] info        pointer to the I2C device configuration strucure
/// @param[in]  interfaces  minimum number of I2C interfaces
/// @return  
///         - BRDCONFIG_SUCCESS
///         - BRDCONFIG_ERROR
///
int32_t BoardGetI2CInfo(BoardI2CInfo *info, uint32_t interfaces);
/// @}

#ifdef __cplusplus
}
#endif

#endif
