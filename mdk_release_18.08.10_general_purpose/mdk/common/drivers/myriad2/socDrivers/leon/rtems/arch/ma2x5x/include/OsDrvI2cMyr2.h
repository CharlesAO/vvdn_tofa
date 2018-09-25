///
/// @file OsDrvI2cMyr2.h
/// 
/// 
/// @defgroup OsDrvI2cMyr2 OsI2cMyr2 Driver
/// @{
/// @brief I2cMyr2 Driver API.
///

#ifndef I2C_MYR2_H
#define I2C_MYR2_H

#include <rtems/libi2c.h>
#include "OsDrvI2cBusDefines.h"
#include "stdio.h"

enum i2cDeviceControlCmd_t
{
    CTL_WRITE_REGISTER=1,
    CTL_READ_REGISTER,
    CTL_WRITE_MULTI_REGISTERS,
    CTL_READ_MULTI_REGISTERS,

    CTL_WRITE_8BITREG_8BITVAL,
    CTL_WRITE_8BITREG_16BITVAL,
    CTL_WRITE_16BITREG_8BITVAL,
    CTL_WRITE_16BITREG_16BITVAL,

    CTL_READ_8BITREG_8BITVAL,
    CTL_READ_8BITREG_16BITVAL,
    CTL_READ_16BITREG_8BITVAL,
    CTL_READ_16BITREG_16BITVAL,
};

typedef struct
{
    enum i2cDeviceControlCmd_t cmd;
    s32 regAddress;  //
    void *regValue;
    s32 wordCount;   // for *MULTI* transfers
} i2cDeviceControl_t;

typedef struct i2cStruct_t
{
    s32 regAddress;
    void *regValue;
    s32 wordCount;   // for *MULTI* transfers
} i2cStruct_t;

#ifdef __cplusplus
extern "C" {
#endif

/* pass this to rtems_libi2c_register_drv() */
extern rtems_libi2c_drv_t *i2cMyr2DriverDescriptor;

    extern rtems_libi2c_drv_t const *i2cGenericProtocolDriverDescriptor;
    extern rtems_libi2c_drv_t genericProtocolDrvTbl;

    extern rtems_libi2c_drv_t genericProtocol16BitReg16BitValDrvTbl;

    rtems_status_code i2cDeviceGenericControl16BitReg8BitVal(
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *arg);

    rtems_status_code i2cDeviceGenericControl16BitReg16BitVal(
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *arg);


extern rtems_libi2c_drv_t genericProtocol16BitReg16BitValDrvTbl;
extern rtems_libi2c_drv_t genericProtocol16BitReg8BitValDrvTbl;

rtems_status_code i2cDeviceGenericControl16BitReg8BitVal(
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *arg);

rtems_status_code i2cDeviceGenericControl16BitReg16BitVal(
        rtems_device_major_number major,
        rtems_device_minor_number minor,
        void *arg);

#ifdef __cplusplus
}
#endif

#endif

///@}
