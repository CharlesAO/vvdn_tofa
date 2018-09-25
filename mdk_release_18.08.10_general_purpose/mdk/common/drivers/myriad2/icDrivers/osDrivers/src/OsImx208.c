#include <rtems.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>
#include <stdio.h>
#include <stdio.h>

#include <brdMv0182Defines.h>
#include <OsImx208.h>
#include <OsDrvI2cBus.h>
#include "OsDrvI2cMyr2.h"

#if 0
#define DPRINTF(...)                        printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif


#define imx208_LEFT_SENSOR_I2C_ADDR         (0x6e >> 1)
#define imx208_RIGHT_SENSOR_I2C_ADDR        (0x6c >> 1)

rtems_status_code i2cControlGenericProtocol(rtems_device_major_number major,
                                            rtems_device_minor_number minor,
                                            void *arg);


static rtems_status_code i2cReadReg(rtems_device_minor_number minor,
		u16 regAddr, u8 *data)
{
    rtems_status_code sc;

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
    if (sc != sizeof(regAddr)) {
        DPRINTF("rtems_libi2c_write failed, sc = %d\n", sc);
        return sc;
    }
    sc = rtems_libi2c_send_addr(minor, (int)1);
    if (sc != RTEMS_SUCCESSFUL) {
        DPRINTF("rtems_libi2c_send_start 1 failed %d\n", sc);
        return sc;
    }
    sc = rtems_libi2c_read_bytes(minor, (u8 *)data, sizeof(*data));
    if (sc != sizeof(*data)) {
        DPRINTF("rtems_libi2c_read failed, sc = %d\n", sc);
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

rtems_status_code imx208Init(rtems_device_major_number major,
                             rtems_device_minor_number minor,
                             void  *arg)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code sc;
    u8 modelIdGolden[2] = {0x02, 0x08};
    u8 modelId[2];

    sc = i2cReadReg(minor, 0x0000, &modelId[0]);
    if(sc) return -sc;
    sc = i2cReadReg(minor, 0x0001, &modelId[1]);
    if(sc) return -sc;


    if(modelIdGolden[0] != modelId[0] ||
    		modelIdGolden[1] != modelId[1])
    {
        DPRINTF("model id: %02x %02x\n", modelId[0], modelId[1]);
    	return -RTEMS_UNSATISFIED;
    }
    else
    {
    	DPRINTF("IMX found model id: %02x %02x\n", modelId[0], modelId[1]);
        return sc;
    }
}

static rtems_driver_address_table imx208ProtocolAdrTbl = {
    .initialization_entry = imx208Init,
    .open_entry = anythingSuccessFunc,
    .close_entry =anythingSuccessFunc,
    .read_entry = anythingSuccessFunc,
    .write_entry = anythingSuccessFunc,
    .control_entry = i2cDeviceGenericControl16BitReg8BitVal,
};

const rtems_libi2c_drv_t imx208ProtocolDrvTbl = {
    .ops = &imx208ProtocolAdrTbl,
    .size = sizeof(imx208ProtocolAdrTbl),
};

