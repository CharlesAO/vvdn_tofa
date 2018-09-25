///
/// @file HglTempSensor.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup HglTempSensor HglTempSensor Driver
/// @{
/// @brief     API Header File for the HGL layer of the Temperature Sensor Driver
///


#ifndef _HGL_TEMP_SENSOR_H_
#define _HGL_TEMP_SENSOR_H_

#include <stdint.h>
#include "HglTempSensorDefines.h"

// 1: Defines
// ----------------------------------------------------------------------------

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef float (*HglTempRawToAct)(float);

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

// 4: Export Functions 
// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Initialise all temperature sensors
/// @return
///
void HglTempSensorInit(void);

/// @brief Set the sample measurement type
/// @param[in] sensor       one of the four temperature sensors 
///                         for which the measurement type is set
/// @param[in] sampleType   sample voltage or temperature
/// @return
///         - HGL_SUCCESS
///         - HGL_ERROR
///
int32_t HglTempSensorSetSampleType(HglTempSensor sensor, HglTempSensorSampleType sampleType);

/// @brief Set the measurement mode for a particular sensor
/// @param[in] sensorToRead   type of sensor to be read
/// @param[in] mode           measurement mode(manual/automatic/disable)
/// @return    
///         - HGL_SUCCESS on success
///         - HGL_ERROR otherwise
///
int32_t HglTempSensorSetMode(HglTempSensor sensor, HglTempSensorMode mode);

/// @brief Continuously get temperature/voltage sample, until the sample is valid
/// @param[in] sensorToRead one of the four temperature sensors
///                         for which to get data
/// @return Valid raw data or invalid zero value, if reading loop exhausted
///
uint16_t HglTempSensorGetData(HglTempSensor sensorToRead);

/// @brief Get one temperature/voltage sample
/// @param[in] sensorToRead one of the four temperature sensors
///                         for which to get data
/// @return Valid raw data or invalid zero value
///
uint16_t HglTempSensorSingleDataRead(HglTempSensor sensorToRead);

/// @brief Convert raw data to actual temperature in degrees Celsius
/// @param[in] N data to be converted
/// @return Converted value
///
float HglTempSensorConvertRawtoTemp(float N);

/// @brief Convert temperature in degrees Celsius to raw data
/// @param[in] T temperature to be converted
/// @return Converted data
///
float HglTempSensorConvertTempToRaw(float T);

/// @brief Convert raw data to actual voltage in Volts
/// @param[in] N data to be converted
/// @return Converted value
///
float HglTempSensorConvertRawToVolt(float N);

/// @brief Convert actual voltage in Volts to raw data
/// @param[in] V data to be converted
/// @return Converted data
///
float HglTempSensorConvertVoltToRaw(float V);

/// @brief Get the actual value for the desired measurement
/// @param[in] sensorToRead one of the four temperature sensors
///                         for which to get the value
/// @param[in] convertFunc  conversion function
/// @return Actual measured value
///
float HglTempSensorGetActualVal(HglTempSensor sensorToRead, HglTempRawToAct convertFunc);

/// @brief Clear temperature sensors' interrupts
/// @param[in] sensor one of the four temperature sensors
///                   for which to clear the interrupt
/// @return
///
void HglTempSensorIrqClr(HglTempSensor sensor);

/// @brief Enable interrupts for a particular sensor
/// @param[in] sensor one of the four temperature sensors
///                   for which to enable the interrupt
/// @return
///
int32_t HglTempSensorEnableDeviceIrq(HglTempSensor sensor);

/// @brief Disable interrupts for a particular sensor
/// @param[in] sensor one of the four temperature sensors
///                   for which to disable the interrupt
/// @return
///
int32_t HglTempSensorDisableDeviceIrq(HglTempSensor sensor);

/// @brief Set the threshold level that will trigger the temperature sensor interrupt
/// @param[in] sensor       one of the four temperature sensors
///                         for which to set the interrupt
/// @param[in] levelThresh  temperature/voltage level for which to trigger the interrupt
/// @return
///         - HGL_SUCCESS on success
///         - HGL_ERROR otherwise
///
int32_t HglTempSensorSetIrqThreshold(HglTempSensor sensor, uint32_t levelThresh);

#ifdef __cplusplus
}
#endif

#endif

///@}
