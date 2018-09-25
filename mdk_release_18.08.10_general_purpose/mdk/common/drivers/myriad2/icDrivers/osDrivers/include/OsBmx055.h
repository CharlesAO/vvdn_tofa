#ifndef IMU_BMX_BMP_H
#define IMU_BMX_BMP_H

#include <mv_types.h>
#include <rtems.h>
#include <rtems/libi2c.h>
#include "stdio.h"
#include <OsImuHAL.h>

#ifndef COUNT_OF
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((!(sizeof(x) % sizeof(0[x])))))
#endif

#define BMX055_ACCEL_NAME "bmx055_accel"
#define BMX055_GYRO_NAME "bmx055_gyro"
#define BMX055_MAG_NAME "bmx055_mag"

/// IMU slave addresses
static const u8 IMU_BMX055_ACCEL_SLAVE_ADDR[] = {0x19, 0x18};
static const u8 IMU_BMX055_GYRO_SLAVE_ADDR[] = {0x68, 0x69};
static const u8 IMU_BMX055_MAG_SLAVE_ADDR[] = {0x10, 0x11, 0x12, 0x13};

typedef struct bmxInterruptConfig_t{
    s32 gpioNumber;
    s32 gpioPriority;
} bmxInterruptConfig_t;

typedef enum bmxAccelSampleRate_t {
    IMU_BMX055_ACC_SAMPLERATE_16HZ = 0x8,
    IMU_BMX055_ACC_SAMPLERATE_32HZ = 0x9,
    IMU_BMX055_ACC_SAMPLERATE_62HZ = 0xA,
    IMU_BMX055_ACC_SAMPLERATE_125HZ = 0xB,
    IMU_BMX055_ACC_SAMPLERATE_250HZ = 0xC,
    IMU_BMX055_ACC_SAMPLERATE_500HZ = 0xE,
    IMU_BMX055_ACC_SAMPLERATE_1000HZ = 0xF,
} bmxAccelSampleRate_t;

typedef enum bmxAccelRange_t{
    IMU_BMX055_ACC_RANGE2G = 0x3,
    IMU_BMX055_ACC_RANGE4G = 0x5,
    IMU_BMX055_ACC_RANGE8G = 0x8,
    IMU_BMX055_ACC_RANGE16G = 0xC,
} bmxAccelRange_t;

typedef enum bmxGyroSampleRate_t {
    IMU_BMX055_GYRO_100HZ_BW_32HZ = 0x7,
    IMU_BMX055_GYRO_200HZ_BW_64HZ = 0x6,
    IMU_BMX055_GYRO_400HZ_BW_47HZ = 0x3,
    IMU_BMX055_GYRO_1000HZ_BW_116HZ = 0x2,
    IMU_BMX055_GYRO_2000HZ_BW_230HZ = 0x1,
    IMU_BMX055_GYRO_2000HZ_UNFILTERED = 0x0,

} bmxGyroSampleRate_t;

typedef enum bmxGyroRange_t {
    IMU_BMX055_GYRO_RANGE2000 = 0x0,
    IMU_BMX055_GYRO_RANGE1000 = 0x1,
    IMU_BMX055_GYRO_RANGE500 = 0x2,
    IMU_BMX055_GYRO_RANGE250 = 0x3,
    IMU_BMX055_GYRO_RANGE125 = 0x4,
} bmxGyroRange_t;

typedef enum bmxMagSampleRate_t {
    IMU_BMX055_MAG_RATE_HZ_MIN = 1,
    IMU_BMX055_MAG_RATE_HZ_RECOMMENDED = 10,
    IMU_BMX055_MAG_RATE_HZ_MAX_HIGH_ACCURACY = 20,
    IMU_BMX055_MAG_RATE_HZ_MAX_ENHANCED_REGULAR = 60,
    IMU_BMX055_MAG_RATE_HZ_MAX_REGULAR = 100,
    IMU_BMX055_MAG_RATE_HZ_MAX_LOW_POWER = 300,
} bmxMagSampleRate_t;

typedef enum bmxMagAccuracy_t {
    IMU_BMX055_MAG_ACC_LOW_POWER = 0,
    IMU_BMX055_MAG_ACC_REGULAR = 1,
    IMU_BMX055_MAG_ACC_ENHANCED = 2,
    IMU_BMX055_MAG_ACC_HIGH = 3,

    IMU_BMX055_MAG_ACC_LAST = 4
} bmxMagAccuracy_t;


typedef enum {
    BMX_INT_GPIO_SET,
    BMX_SAMPLERATE_SET,
    BMX_RANGE_SET,
} bmxControlType;

typedef struct bmxAccelControl_t {

    union {
        bmxAccelSampleRate_t sampleRate;
        bmxAccelRange_t range;
    };
    s32 bmxIrqGpio;
    s32 bmxIrqPriority;
} bmxAccelControl_t;

typedef struct bmxGyroControl_t {
    union {
        bmxGyroSampleRate_t sampleRate;
        bmxGyroRange_t range;
    };
    s32 bmxIrqGpio;
    s32 bmxIrqPriority;
} bmxGyroControl_t;

typedef struct bmxMagControl_t {
    union {
        bmxMagSampleRate_t sampleRate;
        bmxMagAccuracy_t accuracy;
    };
    s32 bmxIrqGpio;
    s32 bmxIrqPriority;
} bmxMagControl_t;
#ifdef __cplusplus
extern "C" {
#endif
extern const rtems_libi2c_drv_t bmxAccelProtocolDrvTbl;
extern const rtems_libi2c_drv_t bmxGyroProtocolDrvTbl;
extern const rtems_libi2c_drv_t bmxMagProtocolDrvTbl;


#ifdef __cplusplus
}
#endif

#endif
