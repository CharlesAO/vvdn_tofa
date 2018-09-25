///
/// @file HglTempSensorDefines.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglTempSensor
/// @{
/// @brief     Header of HGL definitions for the Temperature Sensor 
///


#ifndef _HGL_TEMPSENSOR_DEFS_H_
#define _HGL_TEMPSENSOR_DEFS_H_

// 1: Defines
// ----------------------------------------------------------------------------
#define DRV_TSENS_SAMPLE_MASK (0x3ff)
#define TEMPSENS_INVALID      (-999.0f)

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef enum tempSmplType
{
    TSENS_SAMPLE_TEMP,
    TSENS_SAMPLE_VOLTAGE,
}HglTempSensorSampleType;

typedef enum tempMode
{
    TSENS_DISABLE,
    TSENS_ONE_SHOT_MODE,
    TSENS_CONTINUOUS_MODE
}HglTempSensorMode;

typedef enum tempSens
{
    TSENS_CSS,
    TSENS_MSS,
    TSENS_UPA0,
    TSENS_UPA1  
}HglTempSensor;

#endif

///@}
