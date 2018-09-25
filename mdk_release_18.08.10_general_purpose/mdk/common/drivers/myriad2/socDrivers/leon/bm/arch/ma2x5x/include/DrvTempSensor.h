///
/// @file DrvTempSensor.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvTempSensor TempSensor Driver
/// @{
/// @brief     Temperature sensor header for the BM driver
///


#ifndef _DRV_TEMP_SENSOR_H_
#define _DRV_TEMP_SENSOR_H_

#include <stdint.h>
#include "DrvCommon.h"
#include "DrvTempSensorDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Initialize temperature sensors
/// @note Temperature sensor also needs an auxiliary clock setup which is not done here
/// @return
///
void DrvTempSensorInit(void) __Deprecated__("Use DrvTempSensorInitialise instead!");

/// @brief Initialize temperature sensors
/// @note Temperature sensor also needs an auxiliary clock setup which is not done here
/// @param[in] params initialisation user defined parameters
/// @return
///
void DrvTempSensorInitialise(DrvTempSensConfig *params);

/// @brief Get current die temperature values
/// for now this just gets temp value of the CSS island.  In time 
/// this should perhaps be expanded to read temp sensors of all power islands that are currently on
///
/// @param      temperature   floating point temperature in Celsius. -999.0f when invalid
/// @return    MYR_DRV_SUCCESS on success, MYR_DRV_ERROR otherwise
///
int DrvTempSensorGetTemp(float *temperature) __Deprecated__("Use DrvTempSensorGetSample instead!");


/// @brief Get the temperature values for a particular sensor (One Shot Mode)
///
/// @param[in] sensorToRead   type of sensor to be read
/// @param[in] temperature    floating point temperature in degrees Celsius. -999.0f when invalid
/// @return    
///         - MYR_DRV_SUCCESS on success
///         - MYR_DRV_ERROR otherwise  
///
int32_t DrvTempSensorGetIndividualTemp(DrvTempSensor sensorToRead, float *temperature) __Deprecated__("Use DrvTempSensorGetSample instead!");

/// @brief Set the measurement mode and sample type for a particular sensor
///
/// @param[in] sensorToRead   type of sensor to be read
/// @param[in] mode           measurement mode(manual/automatic)
/// @param[in] sampleType     sample voltage or temperature
/// @return    
///         - MYR_DRV_SUCCESS
///         - MYR_DRV_ERROR
///         - MYR_DRV_NOT_INITIALIZED
///
int32_t DrvTempSensorSetMode(DrvTempSensor sensor, DrvTempSensorMode mode, DrvTempSensorSampleType sampleType);

/// @brief Get measurement sample from a particular sensor
///
/// @param[in] sensorToRead   type of sensor to be read
/// @param[in] value          floating point temperature in degrees Celsius or voltage in Volts.
///                           -999.0f when invalid
/// @note value depends on the sample type set in DrvTempSensorSetMode
/// @return    
///         - MYR_DRV_SUCCESS
///         - MYR_DRV_ERROR
///         - MYR_DRV_NOT_INITIALIZED
///
int32_t DrvTempSensorGetSample(DrvTempSensor sensor, float *value);

/// @brief Convert raw data to actual voltage in Volts
/// @param[in] N data to be converted
/// @return Converted value
///
float DrvTempSensorConvertRawToVolt(float N);

/// @brief Convert raw data to actual temperature in degrees Celsius
/// @param[in] N data to be converted
/// @return Converted value
///
float DrvTempSensorConvertRawtoTemp(float N);

/// @brief Enable irq for a specific sensor
///
/// @param[in] sensor     one of the four temperature sensors
///                       for which to enable the threshold interrupt
/// @param[in] threshold  temperature/voltage value for which to trigger the interrupt
/// @param[in] callback   function to be called when the threshold value is reached
/// @return
///         - MYR_DRV_SUCCESS
///         - MYR_DRV_ERROR
///         - MYR_DRV_NOT_INITIALIZED
///
int32_t DrvTempSensorEnableIrq(DrvTempSensor sensor, float threshold, DrvTempSensorCb callback);

/// @brief Disable irq for a specific sensor
///
/// Uninstalls the callback and invalidates the threshold
/// @param[in] sensor     one of the four temperature sensors
///                       for which to disable the threshold interrupt
/// @return
///         - MYR_DRV_SUCCESS
///         - MYR_DRV_NOT_INITIALIZED
///
int32_t DrvTempSensorDisableIrq(DrvTempSensor sensor);

#ifdef __cplusplus
}
#endif

#endif


///@}
