///
/// @file OsDrvI2cMyr2.c
/// 
/// 
/// @ingroup OsDrvI2cMyr2
/// @{
///

#include <rtems.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>
#include "OsCommon.h"
#include "OsCross.h"
#include "OsDrvI2cMyr2.h"

static int i2cMyr2Write(rtems_device_major_number major,
                        rtems_device_minor_number minor,
                        void *arg)
{
    UNUSED(major);// hush the compiler warning.

    rtems_libio_rw_args_t *rwargs = arg;
    s32 cnt = rwargs->count, sc;
    u8 *buf = (u8 *) rwargs->buffer;

    sc = rtems_libi2c_send_start(minor);
    if (sc != OS_MYR_DRV_SUCCESS){
        OS_I2C_DPRINTF1("rtems_libi2c_send_start failed %d\n", sc);
        return sc;
    }
    /// First, send the address, and prepare for write
    sc = rtems_libi2c_send_addr(minor, 0);
    if (sc != OS_MYR_DRV_SUCCESS){
        OS_I2C_DPRINTF1("rtems_libi2c_send_addr failed %d\n", sc);
        return sc;
    }

    sc = rtems_libi2c_write_bytes(minor, buf, cnt);
    if (sc < 0){
        OS_I2C_DPRINTF1("rtems_libi2c_write_bytes failed\n");
        return -sc;
    }
    else{
        rwargs->bytes_moved = sc;
    }

    sc = rtems_libi2c_send_stop(minor);
    if (sc != OS_MYR_DRV_SUCCESS){
        OS_I2C_DPRINTF1("rtems_libi2c_send_stop failed\n");
        return sc;
    }
    return OS_MYR_DRV_SUCCESS;
}

static int i2cMyr2Read(rtems_device_major_number major,
                       rtems_device_minor_number minor,
                       void *arg)
{
    UNUSED(major);// hush the compiler warning.

    s32 sc;
    rtems_libio_rw_args_t *rwargs = arg;

    sc = rtems_libi2c_send_start(minor);
    if (sc != OS_MYR_DRV_SUCCESS){
        OS_I2C_DPRINTF1("rtems_libi2c_send_start failed %d\n", sc);
        return sc;
    }
    /// First, send the address, and prepare for read
    sc = rtems_libi2c_send_addr(minor, 1);
    if (sc != OS_MYR_DRV_SUCCESS){
        OS_I2C_DPRINTF1("rtems_libi2c_send_start failed %d\n", sc);
        return sc;
    }

    sc = rtems_libi2c_read_bytes(minor, (u8 *) rwargs->buffer,
                                 rwargs->count);

    if (sc < 0){
        return sc;
    }
    else{
        rwargs->bytes_moved = sc;
    }

    // This will take effect in the next transaction (stop bit is incorporated in the the last byte sent/recieved
    sc = rtems_libi2c_send_stop(minor);
    if (sc != OS_MYR_DRV_SUCCESS){
        OS_I2C_DPRINTF1("rtems_libi2c_send_stop failed\n");
        return sc;
    }
    return OS_MYR_DRV_SUCCESS;
}

static rtems_driver_address_table myr2Ops = {
    .read_entry = (rtems_status_code (*)(
                       rtems_device_major_number,
                       rtems_device_minor_number, void *)) i2cMyr2Read,
    .write_entry = (rtems_status_code (*)(
                        rtems_device_major_number,
                        rtems_device_minor_number, void *)) i2cMyr2Write,
};

static rtems_libi2c_drv_t myr2DrvTbl = {
    .ops = &myr2Ops,
    .size = sizeof(myr2DrvTbl),
};

rtems_libi2c_drv_t *i2cMyr2DriverDescriptor = &myr2DrvTbl;




typedef enum
{
    REG_8BITS = 1,
    REG_16BITS = 2,
} regSize_t;

rtems_status_code i2cReadRegisters(rtems_device_major_number major,
                                   rtems_device_minor_number minor,
                                   regSize_t regAddrSize, regSize_t regValSize,
                                   s32 regAddr, void *regValue, s32 wordCount)
{
    UNUSED(major);// hush the compiler warning.

    s32 sc;

    sc = rtems_libi2c_send_start(minor);
    if (sc != OS_MYR_DRV_SUCCESS){
        return sc;
    }
    sc = rtems_libi2c_send_addr(minor, 0);
    if (sc != OS_MYR_DRV_SUCCESS){
        return sc;
    }
    // Fix endianness if necessary
    if(regAddrSize == REG_16BITS)
    {
        regAddr = (((regAddr&0xFF00) >> 8) | ((regAddr&0x00FF) << 8));
    }
    // Write register address
    sc = rtems_libi2c_write_bytes(minor, (u8*)&regAddr, regAddrSize);
    if (sc < 0){
        return -sc;
    }

    sc = rtems_libi2c_send_addr(minor, 1);
    if (sc != OS_MYR_DRV_SUCCESS){
        return sc;
    }
    sc = rtems_libi2c_read_bytes(minor, regValue, wordCount * regValSize);
    if (sc < 0)
    {
        return sc;
    }
    sc = rtems_libi2c_send_stop(minor);
    if (sc != OS_MYR_DRV_SUCCESS)
    {
        return sc;
    }

    if(regValSize == REG_16BITS)
    {
        int i;
        u16 *src, *dst;
        src = regValue;
        dst = regValue;
        for(i = 0 ; i < wordCount ; i++)
        {
            dst[i] = (((src[i]&0xFF00) >> 8) | ((src[i]&0x00FF) << 8));
        }
    }

    return OS_MYR_DRV_SUCCESS;
}

rtems_status_code i2cWriteRegisters(rtems_device_major_number major,
                                    rtems_device_minor_number minor,
                                    regSize_t regAddrSize, regSize_t regValSize,
                                    s32 regAddr, void *regValue, s32 wordCount)
{
    UNUSED(major);// hush the compiler warning.

    s32 sc;
    u16   regValueEndianFixed[wordCount];

    sc = rtems_libi2c_send_start(minor);
    if (sc != OS_MYR_DRV_SUCCESS){
        return sc;
    }
    sc = rtems_libi2c_send_addr(minor, 0);
    if (sc != OS_MYR_DRV_SUCCESS){
        return sc;
    }
    // Fix endianness if necessary
    if(regAddrSize == REG_16BITS)
    {
        regAddr = (((regAddr&0xFF00) >> 8) | ((regAddr&0x00FF) << 8));
    }
    // Write register address
    sc = rtems_libi2c_write_bytes(minor, (u8*)&regAddr, regAddrSize);
    if (sc < 0){
        return -sc;
    }
    // Fix endianness if necessary
    if(regValSize == REG_16BITS)
    {
        int i;
        u16 *src;
        src = regValue;
        for(i = 0 ; i < wordCount ; i++)
        {
            regValueEndianFixed[i] = (((src[i]&0xFF00) >> 8) | ((src[i]&0x00FF) << 8));
        }

        // Write register value
        sc = rtems_libi2c_write_bytes(minor, (u8*)regValueEndianFixed,
                                      wordCount * regValSize);
        if (sc < 0){
            return -sc;
        }
        sc = rtems_libi2c_send_stop(minor);
        if (sc != OS_MYR_DRV_SUCCESS){
            return sc;
        }
    }
    else if(regValSize == REG_8BITS)
    {
        // Write register value
        sc = rtems_libi2c_write_bytes(minor, regValue,
                                      wordCount * regValSize);
        if (sc < 0){
            return -sc;
        }
        sc = rtems_libi2c_send_stop(minor);
        if (sc != OS_MYR_DRV_SUCCESS){
            return sc;
        }
    }
    else
    {
        return RTEMS_INVALID_ID;
    }

    return OS_MYR_DRV_SUCCESS;
}

rtems_status_code i2cControlGenericProtocol(rtems_device_major_number major,
                                            rtems_device_minor_number minor,
                                            void *arg)
{
    i2cDeviceControl_t *c = (i2cDeviceControl_t *)arg;
    regSize_t regAddrSize;
    regSize_t regValSize;

    if(c->regValue == NULL)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    if(c->wordCount == 0)
    {
        return RTEMS_INVALID_NUMBER;
    }

    switch(c->cmd)
    {
    case CTL_WRITE_8BITREG_8BITVAL:
        regAddrSize = REG_8BITS;
        regValSize = REG_8BITS;
        break;
    case CTL_WRITE_8BITREG_16BITVAL:
        regAddrSize = REG_8BITS;
        regValSize = REG_16BITS;
        break;
    case CTL_WRITE_16BITREG_8BITVAL:
        regAddrSize = REG_16BITS;
        regValSize = REG_8BITS;
        break;
    case CTL_WRITE_16BITREG_16BITVAL:
        regAddrSize = REG_16BITS;
        regValSize = REG_16BITS;
        break;

    case CTL_READ_8BITREG_8BITVAL:
        regAddrSize = REG_8BITS;
        regValSize = REG_8BITS;
        break;
    case CTL_READ_8BITREG_16BITVAL:
        regAddrSize = REG_8BITS;
        regValSize = REG_16BITS;
        break;
    case CTL_READ_16BITREG_8BITVAL:
        regAddrSize = REG_16BITS;
        regValSize = REG_8BITS;
        break;
    case CTL_READ_16BITREG_16BITVAL:
        regAddrSize = REG_16BITS;
        regValSize = REG_16BITS;
        break;

    default:
        return RTEMS_NOT_IMPLEMENTED;
    }

    switch(c->cmd)
    {
    case CTL_WRITE_8BITREG_8BITVAL:
    case CTL_WRITE_8BITREG_16BITVAL:
    case CTL_WRITE_16BITREG_8BITVAL:
    case CTL_WRITE_16BITREG_16BITVAL:
        return i2cWriteRegisters(major, minor, regAddrSize, regValSize,
                                 c->regAddress, c->regValue, c->wordCount);
    case CTL_READ_8BITREG_8BITVAL:
    case CTL_READ_8BITREG_16BITVAL:
    case CTL_READ_16BITREG_8BITVAL:
    case CTL_READ_16BITREG_16BITVAL:
        return i2cReadRegisters(major, minor, regAddrSize, regValSize,
                                c->regAddress, c->regValue, c->wordCount);
    default:
        return RTEMS_NOT_IMPLEMENTED;
    }
}

rtems_status_code i2cDeviceGenericControl16BitReg8BitVal(
                rtems_device_major_number major,
                rtems_device_minor_number minor,
                void *arg)
{
    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) arg;
    struct i2cStruct_t *i2cStr;
    regSize_t regAddrSize = REG_16BITS;
    regSize_t regValSize = REG_8BITS;
    rtems_status_code sc;

    if (!ctl)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    i2cStr = (i2cStruct_t *)ctl->buffer;

    if(i2cStr->regValue == NULL)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    if(i2cStr->wordCount == 0)
    {
        return RTEMS_INVALID_NUMBER;
    }

    switch(ctl->command)
    {
    case CTL_WRITE_REGISTER:
        sc = i2cWriteRegisters(major, minor, regAddrSize, regValSize,
                               i2cStr->regAddress, i2cStr->regValue, i2cStr->wordCount);
        ctl->ioctl_return = i2cStr->wordCount;
        return sc;
    case CTL_READ_REGISTER:
        sc =  i2cReadRegisters(major, minor, regAddrSize, regValSize,
                               i2cStr->regAddress, i2cStr->regValue, i2cStr->wordCount);
        ctl->ioctl_return = i2cStr->wordCount;
        return sc;
    default:
        return RTEMS_NOT_IMPLEMENTED;
    }
}

rtems_status_code i2cDeviceGenericControl16BitReg16BitVal(
                rtems_device_major_number major,
                rtems_device_minor_number minor,
                void *arg)
{
    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) arg;
    struct i2cStruct_t *i2cStr;
    regSize_t regAddrSize = REG_16BITS;
    regSize_t regValSize = REG_16BITS;
    rtems_status_code sc;

    if (!ctl)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    i2cStr = (i2cStruct_t *)ctl->buffer;

    if(i2cStr->regValue == NULL)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    if(i2cStr->wordCount == 0)
    {
        return RTEMS_INVALID_NUMBER;
    }

    switch(ctl->command)
    {
    case CTL_WRITE_REGISTER:
        sc = i2cWriteRegisters(major, minor, regAddrSize, regValSize,
                               i2cStr->regAddress, i2cStr->regValue, i2cStr->wordCount);
        ctl->ioctl_return = i2cStr->wordCount;
        return sc;
    case CTL_READ_REGISTER:
        sc =  i2cReadRegisters(major, minor, regAddrSize, regValSize,
                               i2cStr->regAddress, i2cStr->regValue, i2cStr->wordCount);
        ctl->ioctl_return = i2cStr->wordCount;
        return sc;
    default:
        return RTEMS_NOT_IMPLEMENTED;
    }
}

static const rtems_driver_address_table genericProtocolAdrTbl = {
    .read_entry = (rtems_status_code (*)(
                       rtems_device_major_number,
                       rtems_device_minor_number, void *)) i2cMyr2Read,
    .write_entry = (rtems_status_code (*)(
                        rtems_device_major_number,
                        rtems_device_minor_number, void *)) i2cMyr2Write,
    .control_entry = i2cControlGenericProtocol,
};

rtems_libi2c_drv_t genericProtocolDrvTbl = {
    .ops = &genericProtocolAdrTbl,
    .size = sizeof(genericProtocolDrvTbl),
};

rtems_libi2c_drv_t const *i2cGenericProtocolDriverDescriptor = &genericProtocolDrvTbl;

static rtems_driver_address_table genericProtocol16BitReg16BitValAdrTbl = {
                    .read_entry = (rtems_status_code (*)(
                                    rtems_device_major_number,
                                    rtems_device_minor_number, void *)) i2cMyr2Read,
                    .write_entry = (rtems_status_code (*)(
                                    rtems_device_major_number,
                                    rtems_device_minor_number, void *)) i2cMyr2Write,
                    .control_entry = i2cDeviceGenericControl16BitReg16BitVal,
                };

rtems_libi2c_drv_t genericProtocol16BitReg16BitValDrvTbl = {
    .ops = &genericProtocol16BitReg16BitValAdrTbl,
    .size = sizeof(genericProtocol16BitReg16BitValDrvTbl),
};

static rtems_status_code anythingSuccessFunc(rtems_device_major_number major,
                                               rtems_device_minor_number minor,
                                               void *arg)
{
    (void)major;
    (void)minor;
    (void)arg;
    return RTEMS_SUCCESSFUL;
}

static rtems_driver_address_table genericProtocol16BitReg8BitValAdrTbl = {
    .initialization_entry = NULL,
    .open_entry = anythingSuccessFunc,
    .close_entry =anythingSuccessFunc,
    .read_entry = anythingSuccessFunc,
    .write_entry = anythingSuccessFunc,
    .control_entry = i2cDeviceGenericControl16BitReg8BitVal,
};

rtems_libi2c_drv_t genericProtocol16BitReg8BitValDrvTbl = {
    .ops = &genericProtocol16BitReg8BitValAdrTbl,
    .size = sizeof(genericProtocol16BitReg8BitValDrvTbl),
};

///@}
