///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     API to the Driver for the MCP3424 ADC chip.
///
///
///
///
///

#ifndef IC_MCP3424_H
#define IC_MCP3424_H

// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvMcp3424Defines.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// Initialise the MCP3424 IC Driver
///
/// Passed a handle for a preconfigured I2C device and the address of the specific 
/// MCP3424 device on that bus
/// The driver applys a default ADC configuration to the device
/// Returns an initialised handle to the MCP3424 driver
/// @param[out] pointer to storage for a handle for this MCP3424 device of type tyMcp3424Handle
/// @param[in]  pointer to a preinitialised I2C device that is connected to the IC
/// @param[in]  I2C address of the specific target MCP3424 device on this I2C bus
/// @return  0 on Success
int Mcp3424Init(tyMcp3424Handle * mcpHandle, I2CM_Device * i2cDev, u32 i2cAddr);

/// Perform a single ADC operation on the target Channel (one shot mode)
///
/// Passed a handle for a pre-initialised MCP device 
/// Returns an integer value containing the raw ADC result in units of nano-volts
/// Can also optionally return a channel configuration so the caller can determine gain and sample rate
/// @param[out] pointer to preinitialised handle for this MCP3424 device of type tyMcp3424Handle
/// @param[in]  ID of target ADC channel to measure
/// @param[out] Pointer to storage for the result of the measurement in NanoVolts
/// @param[out] Pointer to storage used to return configuration of the ADC Channel (NULL if not needed)
/// @return  0 on Success
int Mcp3424ReadChannelOneShot(tyMcp3424Handle * mcpHandle,tyMcpChannel channel,u32 * resultNV,tyMcp3424Config * chanCfgReturn);

/// Sample the targeted ADC to get see if it has a sample ready
///
/// Passed a handle for a pre-initialised MCP device 
/// Returns an integer value containing the raw ADC result in units of nano-volts
/// Can also optionally return a channel configuration so the caller can determine gain and sample rate, targetted channel and 
/// also the RdyN field which is low when the sample returned is a new sample
/// @param[out] pointer to preinitialised handle for this MCP3424 device of type tyMcp3424Handle
/// @param[out] Pointer to storage for the result of the measurement in NanoVolts
/// @param[out] Pointer to storage used to return configuration of the ADC Channel (NULL if not needed)
/// @return  0 on Success
int Mcp3424SampleAdcResult(tyMcp3424Handle * mcpHandle,u32 * resultNV,tyMcp3424Config * chanCfgReturn);

/// Adjust the configuration of a signle channel of a preinitialised MCP ADC device
///
/// Passed a handle for a pre-initialised MCP device and a pointer to an Mcp3424 config
/// structure containing the desired MCP config change
/// This configuration change will apply to a single channel of the ADC 
/// as defined by the chan member of the configuration structure.
/// @param[in] pointer to preinitialised handle for this MCP3424 device of type tyMcp3424Handle
/// @param[in] pointer to config structure containing the desired configuration modification
/// @return  0 on Success
int Mcp3424ApplyCustomConfig(tyMcp3424Handle * mcpHandle, tyMcp3424Config * cfg);


#ifdef __cplusplus
}
#endif

#endif // IC_MCP3424_H
