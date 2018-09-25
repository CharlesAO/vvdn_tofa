///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     API for the MV0182 Board Driver
///
///
///
///
///
#ifndef BRD_MV0182_H
#define BRD_MV0182_H

// 1: Includes
// ----------------------------------------------------------------------------
#include <brdMv0182Defines.h>
#include <DrvI2cMaster.h>

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// Initialise the default configuration for I2C0,I2C1, I2C2 on the MV0182 Board
/// @param[in] pointer to an I2C configuration structure for I2C0 (OR NULL to use board defaults)
/// @param[in] pointer to an I2C configuration structure for I2C1 (OR NULL to use board defaults)
/// @param[in] pointer to an I2C configuration structure for I2C2 (OR NULL to use board defaults)
/// @param[out] pointer to storage for an *I2CM_Device Handle for I2C Device 0
/// @param[out] pointer to storage for an *I2CM_Device Handle for I2C Device 1
/// @param[out] pointer to storage for an *I2CM_Device Handle for I2C Device 2
/// @return  0 on Success
s32 brd182InitialiseI2C(tyI2cConfig * i2c0Cfg, tyI2cConfig * i2c1Cfg,tyI2cConfig * i2c2Cfg,I2CM_Device ** i2c0Dev,I2CM_Device ** i2c1Dev,I2CM_Device ** i2c2Dev);

/// Returns the revision number of the PCB
s32 brd182GetPcbRevison(tyMv0182PcbRevision *);


/// Configures the External PLL to a given frequency
/// @param[in] config_index (See "DrvCDCEL.h" for usable indexes)
/// @return    0 on success, non-zero on fail
s32 brd182ExternalPllConfigure(u32 configIndex);

/// Helper function to control LEDS on MV0182
/// @param[in] *I2CM_Device Handle for I2C Device 2
/// @param[in] BRD_LED1 or BRD_LED2
/// @param[in] LED_ON or LED_OFF
/// @return  0 on Success
s32 brd182SetLed(I2CM_Device * i2cDevice,tyBrdLedId ledNum,tyLedState ledState);

#ifdef __cplusplus
}
#endif

#endif // BRD_MV0182_H
