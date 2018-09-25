///
/// @file      BoardConfig.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
/// @brief     Board configuration header.
///

#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

/// System Includes
/// -------------------------------------------------------------------------------------
#include <rtems/status-checks.h>
/// Application Includes
/// -------------------------------------------------------------------------------------

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------
#ifndef PLL_DESIRED_FREQ_KHZ
#define PLL_DESIRED_FREQ_KHZ        (480000) /// PLL desired frequency
#endif
#define DEFAULT_OSC0_KHZ            (12000)  /// Input OSC frequency


/// I2C
#define I2C_BUS_NAME0                "/dev/i2c0"
#define I2C_BUS_NAME1                "/dev/i2c1"
#define I2C_BUS_NAME2                "/dev/i2c2"
extern int i2cBus0;
extern int i2cBus1;
extern int i2cBus2;

#ifdef __cplusplus
extern "C" {
#endif

/// Exported function headers
/// -------------------------------------------------------------------------------------

rtems_status_code brdInit(void);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // _BOARD_CONFIG_H_
