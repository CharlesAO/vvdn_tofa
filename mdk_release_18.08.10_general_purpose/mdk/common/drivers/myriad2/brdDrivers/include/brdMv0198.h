///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     API for the MV0198 Power Monitor Board Driver
///
///  Allows the user to query the current on any Myriad power rail
///  when used with MV0182
///
///
#ifndef BRD_MV0198_H
#define BRD_MV0198_H

// 1: Includes
// ----------------------------------------------------------------------------
#include "brdMv0198Defines.h"
#include "DrvMcp3424Defines.h"

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

/// Initialise the MV0198 Power Measurement Board
///
/// Passed a handle for a preconfigured I2C device and an optional override for the ADC configuration
/// Table
/// If the ADC Channel config paramater is NULL, then the default embedded ADC configuration will 
/// be used
/// This Init function is a necessary prerequisite for all the other functions in this API
/// This API has both blocking and non-blocking methods to query the system power
/// Specifically the Brd198GetRailValueOneShot() call is a blocking function which 
/// both initiates a new measurement and doesn't return until the result is available
/// By default the OneShot mode will take approx 4mS to complete and as such isn't suituable 
/// when there are any real-time constraints.
/// The alternative is the ContinuousMode Api which allows the user to poll for new 
/// valid data. This API has two methods Brd198ConfigureContinuousMode to start the measurement
/// and Brd198SampleRailContinuousMode to check for updated results.
/// Once in continuous mode, the user should not attempt OneShot measurements.
/// @param[out] pointer to storage for a handle for this MV0198 of type tyBrd198Handle
/// @param[in]  pointer to a preinitialised I2C device that is connected to the MV0198
/// @param[in]  pointer to optional override for the default ADC config table, (OR NULL to use boad defaults)
/// @return  0 on Success
int Brd198Init(tyBrd198Handle * brdHandle,I2CM_Device * i2cDev,tyAdcChannelConfig * optChannelConfigOverride);

/// Retrieve the value of current (or voltage) for a specific Channel
///
/// See tyChannelId for the list of supported channels
/// Most channels return a current in mA, but there are two channels that return 
/// a rail voltage (VDDCV_V_MV, MIPI_VDD_V_MV)
/// The function returns a value in either mV or mA depending on the rail
/// e.g. VDDCV_I_MA returns the measured current for the VDDCV rail in mA
/// VDDCV_V_MV returns the measured voltage for the VDDCV rail in mV
/// @note This is a blocking function which typically takes 4ms to complete.
/// See ContinuousMode API for an alternative non-blocking mechanism
/// @param[in]  pointer to the preinitialised handle for this MV0198 of type tyBrd198Handle
/// @param[in]  ID of Channel to be read
/// @param[out] pointer to storage for result of the measurement
/// @return  0 on Success
int Brd198GetRailValueOneShot(tyBrd198Handle * brdHandle,tyChannelId chan,float * railValue);


/// Configure the MV0198 board in continuous capture mode
///
/// Continuous capture mode is an alternative to OneShot mode
/// In this mode up to 4 of ADCs on the platform can be configured
/// to continously capture data. This data can then be polled using the
/// Brd198SampleRailContinuousMode API call
/// This function takes a pointer to an tyAdcContinuousConfig structure
/// which will allows the caller to specify which rail to select on each of the 4
/// ADCs (A,B,C,D). Optionally the user can also specify that a specific rail is NOT_USED
/// @param[in]  Pointer to the preinitialised handle for this MV0198 of type tyBrd198Handle
/// @param[out] Pointer to tyAdcContinuousConfig structure describing which channels to monitor
/// @return  0 on Success
int Brd198ConfigureContinuousMode(tyBrd198Handle * brdHandle,tyAdcContinuousConfig * continuousCfg);

/// Query the MV0198 platform for any updated sample results when configured in continuous mode
///
/// This function can be called one or more times after Brd198ConfigureContinuousMode
/// to check if any ADC has an updated valid sample result 
/// It takes a pointer to storage for a tyAdcContinuousResult which will be populated
/// with any new data. 
/// Specificaly for each of the 4 ADCS it has two members, one contains a float representing 
/// the data value and the other is a boolean flag which indicates that the sample is valid
/// when TRUE.
/// Most channels return a current, but there are two channels that return 
/// a rail voltage (VDDCV_V_MV, MIPI_VDD_V_MV)
/// The function returns a value in either mV or mA depending on the rail
/// e.g. VDDCV_I_MA returns the measured current for the VDDCV rail in mA
/// VDDCV_V_MV returns the measured voltage for the VDDCV rail in mV
/// @param[in]  pointer to the preinitialised handle for this MV0198 of type tyBrd198Handle
/// @param[out] pointer to storage for result of the measurement
/// @return  0 on Success
int Brd198SampleRailContinuousMode(tyBrd198Handle * brdHandle,tyAdcContinuousResult * contResult);

/// Adjust the SampleRate, Gain and Mode of a specific ADC channel
///
/// See tyChannelId for the list of supported channels
/// This function takes a pointer to a channelConfig structure
/// which will be used to update the config
/// This function applies to both the OneShot and Continuous Mechanisms however care must 
/// be taken to correctly configure the mode paramter accordingly.
/// Note: the channel number within this config will be overwritten with the correct channel number
/// @param[in]  pointer to the preinitialised handle for this MV0198 of type tyBrd198Handle
/// @param[in]  ID of Channel to be read
/// @param[in] pointer to MCP channel config structure which is used to modify the configuration
/// @return  0 on Success
int Brd198AdjustRailConfig(tyBrd198Handle * brdHandle,tyChannelId chan,tyMcp3424Config * channelConfig);

/// Return string containing name of channel and Channel type
///
/// This is an optional helper function which allows the user to query 
/// the name and type (CURRENT_MEASUREMENT or VOLTAGE_MEASUREMENT) based on the
/// the channel ID.
/// The returned string is padded with Spaces to provide alignment
/// @note       This function can only be used when no channel override has been provided to Brd198Init
/// @param[in]  pointer to the preinitialised handle for this MV0198 of type tyBrd198Handle
/// @param[in]  ID of Channel to be queried
/// @param[out] pointer to a string (type tyChannelName) which can be used to store the name of the channel
/// @param[out] pointer to storage for the type of the measurement (current or voltage)
/// @return  0 on Success
int Brd198LookupRailDescription(tyBrd198Handle *brdHandle, tyChannelId chan, char *channelName, tyChannelType *railType);

/// Read all rails once and compute the total number of mA and mW over all rails
///
/// @param[in]  pointer to the preinitialised handle for this MV0198 of type tyBrd198Handle
/// @param[out] pointer to a tyAdcResultAllRails structure where the rail values will be found
/// @return  0 on Success
int Brd198SampleAllRails(tyBrd198Handle * brdHandle, tyAdcResultAllRails * result);

/// Extract the power and current values only for DDR from a set of samples already taken
///
/// @param[in]  pointer to the preinitialised handle for this MV0198 of type tyBrd198Handle
/// @param[in]  pointer to a tyAdcResultAllRails structure where the rail values will be found 
/// @param[out] pointer to the variable where the power value will be returned
/// @param[out] pointer to the variable where the current value will be returned
/// @return  no return
void Brd198GetDdrPowerAndCurrent(tyBrd198Handle * brdHandle, tyAdcResultAllRails * samples, float * power, float * current);


#ifdef __cplusplus
}
#endif

#endif // BRD_MV0198_H
