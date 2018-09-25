#ifndef _LIBCHIP_SPI_IMU_BMI160_H
#define _LIBCHIP_SPI_IMU_BMI160_H

#include <mv_types.h>
#include <rtems.h>
#include <rtems/libi2c.h>
#include <OsImuHAL.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BMI160_DEVICE_NAME "BMI160"

#define IOCTL_STANDARD_UI_9DOF_FIFO 0
#define IOCTL_STANDARD_UI_IMU_FIFO 1
#define IOCTL_STANDARD_UI_IMU 2
#define IOCTL_STANDARD_UI_ADVANCEPOWERSAVE 3
#define IOCTL_ACCEL_PEDOMETER 4
#define IOCTL_APPLICATION_HEAD_TRACKING 5
#define IOCTL_APPLICATION_NAVIGATION 6
#define IOCTL_APPLICATION_REMOTE_CONTROL 7
#define IOCTL_APPLICATION_INDOOR_NAVIGATION 8
#define IOCTL_SET_AP_TIMESTAMP_OFFSET 9
#define IOCTL_SET_ROTATION_MATRIX 10
#define IOCTL_BMI160_GET_TEMP 11

typedef struct {
    uint32_t baudrate;           /* tfr rate, bits per second     */
} OsSpiBMI160Param_t;

typedef struct {
    rtems_libi2c_drv_t  libi2c_drv_entry;  /* general i2c/spi params */
    OsSpiBMI160Param_t  BMI160_param;  /* private parameters     */
} OsSpiBMI160_t;

/*
 * pass this descriptor pointers to rtems_libi2c_register_drv
 */
    extern rtems_libi2c_drv_t OsBMI160DriverDescriptor;
    extern OsSpiBMI160_t OsSpiBMI160RwDrv;

#ifdef __cplusplus
}
#endif

#endif
