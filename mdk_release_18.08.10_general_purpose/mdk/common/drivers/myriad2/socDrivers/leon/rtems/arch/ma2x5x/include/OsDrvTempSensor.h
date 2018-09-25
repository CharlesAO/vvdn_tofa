///
/// @file OsDrvTempSensor.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvTempSensor OsTempSensor Driver
/// @{
/// @brief     RTEMS Temperature Sensor Header File 
///
/// The RTEMS driver has the following features:
/// - Read temperature or voltage in continuous or one shot mode
/// - Install a callback which will be executed if the temperature/voltage 
///   rises above a set threshold.
/// - Install a callback and read samples from the sensors concurrently
/// - Get information(sample type, mode) about a particular sensor at any time
///
/// Usage example:
/// @code{.c}
///
/// void* testCb(OsDrvTempSensor sensor, float data, void* context)
/// {
///     printf("OK !!!");
///
///     return NULL;
/// }
///
/// // User has to provide a configuration that will remain unchanged 
/// // and will be used for the entire application
/// struct OsDrvTempSensorConfig initcfg = 
/// {
///     .irq_priority = 1,
///     {
///        {OSDRV_TSENS_CONT_TEMPERATURE}, // CSS sensor
///        {OSDRV_TSENS_CONT_VOLTAGE},     // MSS sensor
///        {OSDRV_TSENS_MANUAL},           // UPA0 sensor
///        {OSDRV_TSENS_DISABLED}          // UPA1 sensor
///     }
/// };
///
/// rtems_io_register_driver(0, &temp_sensor_drv_tbl, &temp_sens_major);
/// rtems_io_initialize(temp_sens_major, 0, (void *)&initcfg);
///
/// // Initialise configuration structures
/// struct OsDrvTempSensorIrqSettings cfg = 
/// {
///    .context   = CONTEXT,
///    .callback  = testCb,
///    .threshold = TEMPERATURE_THRESHOLD; // in degrees Celsius
/// };
///
/// fd = open("/dev/tempsensor0", O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
/// if (fd < 0)
/// {
///     perror("open failed");
///     exit(-1);
/// }
///
/// status = ioctl(fd, IOCTL_READ_TEMP, &value);
/// if (status)
/// {
///   perror("read ioctl failed");
///   exit(-2);
/// }
///
/// status = ioctl(fd, IOCTL_GET_TSENS_STATUS, &info);
/// if(status)
/// {
///     perror("status ioctl failed");
///     exit(-2);
/// }
///
/// status = ioctl(fd, IOCTL_TEMP_THR_CB, (void*)&cfg);
/// if (status)
/// {
///     perror("irq ioctl failed");
///     exit(-2);
/// }
///
/// status = close(fd);
/// if (status)
/// {
///     perror("close failed");
///     exit(-3);
/// }
///
/// @endcode
///

#ifndef OS_DRV_TEMP_SENSOR
#define OS_DRV_TEMP_SENSOR

#include "HglTempSensorDefines.h"
#include "rtems.h"

#define IOCTL_READ_TEMP             0
#define IOCTL_READ_VOLT             1
#define IOCTL_TEMP_THR_CB           2
#define IOCTL_VOLT_THR_CB           3
#define IOCTL_GET_TSENS_STATUS      4

#define TEMPSENS_NUM 4

typedef HglTempSensor OsDrvTempSensor;
typedef HglTempSensorSampleType OsDrvTempSensorSampleType;

typedef enum {
    OSDRV_TSENS_DISABLED,
    OSDRV_TSENS_CONT_VOLTAGE,
    OSDRV_TSENS_CONT_TEMPERATURE,
    OSDRV_TSENS_MANUAL
} OsDrvTempSensorMode;

struct OsDrvTempSensorConfig {
    uint8_t irq_priority;
    struct {
        OsDrvTempSensorMode mode;
    } tsens[TEMPSENS_NUM];
};

struct OsDrvTempSensorInfo {
    OsDrvTempSensorMode mode;
    OsDrvTempSensorSampleType sample_type;
};

// Callback triggered when the temperature/voltage is higher than the set threshold
typedef void* (*OsDrvTempSensorCb)(OsDrvTempSensor, float, void*);

struct OsDrvTempSensorIrqSettings {
    void *context;
    float threshold;
    OsDrvTempSensorCb callback;
};

extern rtems_driver_address_table temp_sensor_drv_tbl;

#endif

///@}

