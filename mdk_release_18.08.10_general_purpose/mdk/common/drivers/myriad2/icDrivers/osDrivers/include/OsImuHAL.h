#ifndef OSIMUHAL_H__
#define OSIMUHAL_H__

#include <stdio.h>

typedef enum imuType_t
{
    IMU_TYPE_INVALID,
    IMU_TYPE_GYROSCOPE,          // rad/sec
    IMU_TYPE_ACCELEROMETER, // m/s^2
    IMU_TYPE_MAGNETOMETER, // uT
    IMU_TYPE_RAW_GYROSCOPE,      // register value
    IMU_TYPE_RAW_ACCELEROMETER,  // register value
    IMU_TYPE_RAW_MAGNETOMETER,   // register value
    IMU_TYPE_TEMPERATURE,    // Kelvin
} imuType_t;

typedef struct imuRawAxisData_t {
    int32_t x;
    int32_t y;
    int32_t z;
} imuRawAxisData_t;

typedef struct imuAxisData_t {
    float x;
    float y;
    float z;
} imuAxisData_t;

typedef struct imuTempData_t {
    float t;
} imuTempData_t;

typedef __attribute__((aligned(4)))struct mvImuData_t
{
    uint32_t type;
    uint64_t timestampNs;
    union {
        imuAxisData_t    axisData;
        imuRawAxisData_t rawAxisData;
        imuTempData_t    temperatureData;
    };
} mvImuData_t;


#endif
