#include <rtems.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>
#include <stdio.h>
#include <stdio.h>

#include <brdMv0182Defines.h>
#include <OsOv7750.h>
#include <OsDrvGpio.h>
#include <OsDrvI2cBus.h>
#include "OsDrvI2cMyr2.h"

#if 0
#define DPRINTF(...)                        printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif

#define MVLOG_UNIT_NAME OsOv7750


#define OV7750_LEFT_SENSOR_I2C_ADDR         0x61
#define OV7750_RIGHT_SENSOR_I2C_ADDR        0x62

#define OV7750_CHIP_ID                      0x5077

rtems_status_code i2cControlGenericProtocol(rtems_device_major_number major,
                                            rtems_device_minor_number minor,
                                            void *arg);


// we have two cameras connected with the same address.
// This function will help to reconfigure the sensors with different addresses
static rtems_status_code
i2cWriteRegAtHackedAddress(rtems_device_minor_number minor,
                           u8 hackedAddress, u16 regAddr, u8 data)
{
    rtems_status_code sc;

    minor &= ~0xFF;
    minor |=  hackedAddress;

    regAddr = ((regAddr & 0xFF00) >> 8) | ((regAddr & 0x00FF) << 8);

    sc = rtems_libi2c_send_start(minor);

    if (sc != RTEMS_SUCCESSFUL) {
        DPRINTF("rtems_libi2c_send_start failed %d\n", sc);
        return sc;
    }
    sc = rtems_libi2c_send_addr(minor, (int)0);
    if (sc != RTEMS_SUCCESSFUL) {
        DPRINTF("rtems_libi2c_send_start 1 failed %d\n", sc);
        return sc;
    }
    sc = rtems_libi2c_write_bytes(minor, (u8 *)&regAddr, sizeof(regAddr));
    if (sc < RTEMS_SUCCESSFUL) {
        DPRINTF("rtems_libi2c_read/write failed, sc = %d\n", sc);
        return sc;
    }
    sc = rtems_libi2c_write_bytes(minor, (u8 *)&data, sizeof(data));
    if (sc < RTEMS_SUCCESSFUL) {
        DPRINTF("rtems_libi2c_read/write failed, sc = %d\n", sc);
        return sc;
    }
    sc = rtems_libi2c_send_stop(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        DPRINTF("rtems_libi2c_send_stop failed, sc = %d\n", sc);
        return sc;
    }

    return -sc;
}

static rtems_status_code anythingSuccessFunc(rtems_device_major_number major,
                                               rtems_device_minor_number minor,
                                               void *arg)
{
    (void)major;
    (void)minor;
    (void)arg;
    return RTEMS_SUCCESSFUL;
}

static int checkChipId(rtems_device_major_number major,
                       rtems_device_minor_number minor,
                       const u16 chipID)
{
    int sc;
    u16 regAddr = 0x0A30;
    u16 data = 0xDEAD;

    (void)major;

    sc = rtems_libi2c_send_start(minor);
    if (sc != OS_MYR_DRV_SUCCESS)
        return -sc;

    sc = rtems_libi2c_send_addr(minor, (int)1);
    if (sc != RTEMS_SUCCESSFUL)
        return -sc;

    sc = rtems_libi2c_write_bytes(minor, (u8 *)&regAddr, sizeof(regAddr));
    if (sc != sizeof(regAddr))
        return -RTEMS_IO_ERROR;

    sc = rtems_libi2c_send_addr(minor, (int)0);
    if (sc != RTEMS_SUCCESSFUL)
        return -sc;

    sc = rtems_libi2c_read_bytes(minor, (u8 *)&data, sizeof(data));
    if (sc != sizeof(data))
        return RTEMS_IO_ERROR;

    sc = rtems_libi2c_send_stop(minor);
    if (sc != OS_MYR_DRV_SUCCESS)
        return -RTEMS_IO_ERROR;

    if(data == chipID)
        return 0;
    else
        return -1;
}

rtems_status_code ov7750Init(rtems_device_major_number major,
                             rtems_device_minor_number minor,
                             void  *arg)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code sc;

    rtems_task_wake_after(20);
    switch(minor & 0xFF)
    {
    case OV7750_LEFT_SENSOR_I2C_ADDR:
            DrvGpioSetPin(MV0182_MV0200_SENSOR_LEFT_RST_GPIO, 0);
            DrvGpioSetPin(MV0182_MV0200_SENSOR_LEFT_RST_GPIO, 1);
            DrvGpioSetPin(MV0182_MV0200_SENSOR_RIGHT_RST_GPIO, 0);
        break;
    case OV7750_RIGHT_SENSOR_I2C_ADDR:
            DrvGpioSetPin(MV0182_MV0200_SENSOR_RIGHT_RST_GPIO, 1);
        break;
    default:
        return RTEMS_INVALID_NUMBER;
    }
    rtems_task_wake_after(20);

    sc = i2cWriteRegAtHackedAddress(minor, 0x70, 0x0109, (minor & 0xFF) << 1);
    if(sc) return sc;

    sc = i2cWriteRegAtHackedAddress(minor, 0x70, 0x303B, 0x02);
    if(sc) return sc;

    sc = i2cWriteRegAtHackedAddress(minor, 0x70, 0x302B, 0xE2);
    if(sc) return sc;

    sc = i2cWriteRegAtHackedAddress(minor, 0x71, 0x303B, 0x02);
    if(sc) return sc;

    sc = checkChipId(major, minor, OV7750_CHIP_ID);  //any successful read here would be enough to confirm the broadcast address is working,
                                                     //but a chip validation is even better
    return sc < 0 ? -sc : sc;
}

static rtems_status_code ov7750InitBroadcast(rtems_device_major_number major,
                                          rtems_device_minor_number minor,
                                          void  *arg)
{
    int sc;
    (void)arg;

    sc = checkChipId(major, minor, OV7750_CHIP_ID);  //any successful read here would be enough to confirm the broadcast address is working,
                                                     //but a chip validation is even better
    return sc < 0 ? -sc : sc;
}


static rtems_driver_address_table ov7750ProtocolAdrTbl = {
    .initialization_entry = ov7750Init,
    .open_entry = anythingSuccessFunc,
    .close_entry =anythingSuccessFunc,
    .read_entry = anythingSuccessFunc,
    .write_entry = anythingSuccessFunc,
    .control_entry = i2cDeviceGenericControl16BitReg8BitVal,
};

const rtems_libi2c_drv_t ov7750ProtocolDrvTbl = {
    .ops = &ov7750ProtocolAdrTbl,
    .size = sizeof(ov7750ProtocolAdrTbl),
};

const rtems_driver_address_table ov7750BroadcastProtocolAdrTbl = {
    .initialization_entry = ov7750InitBroadcast,
    .read_entry = anythingSuccessFunc,
    .write_entry = anythingSuccessFunc,
    .open_entry = anythingSuccessFunc,
    .close_entry = anythingSuccessFunc,
    .control_entry = i2cDeviceGenericControl16BitReg8BitVal,
};

const rtems_libi2c_drv_t ov7750BroadcastProtocolDrvTbl = {
    .ops = &ov7750BroadcastProtocolAdrTbl,
    .size = sizeof(ov7750BroadcastProtocolAdrTbl),
};
