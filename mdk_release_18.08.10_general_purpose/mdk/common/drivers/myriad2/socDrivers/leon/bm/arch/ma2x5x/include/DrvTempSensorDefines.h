///
/// @file DrvTempSensorDefines.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvTempSensor
/// @{
/// @brief     Header of BM definitions for the Temperature Sensor 
///


#ifndef _DRV_TEMP_SENSOR_DEFS_H_
#define _DRV_TEMP_SENSOR_DEFS_H_

#include <stdint.h>
#include "HglTempSensorDefines.h"

// 1: Defines
// ----------------------------------------------------------------------------
// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef HglTempSensor           DrvTempSensor;
typedef HglTempSensorMode       DrvTempSensorMode;
typedef HglTempSensorSampleType DrvTempSensorSampleType;

/// @brief Callback triggered when the temperature/voltage is higher than the set threshold
/// @note The raw sample is provided as a parameter to the callback function.
typedef void* (*DrvTempSensorCb)(DrvTempSensor, uint16_t);

typedef struct
{
    uint32_t priority;
}DrvTempSensConfig;

#endif

///@}
