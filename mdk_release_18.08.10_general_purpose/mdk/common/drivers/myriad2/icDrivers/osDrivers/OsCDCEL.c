#include <rtems.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>

#include <DrvCdcelConfigs.h>
#include <OsCDCEL.h>

#if 0
#define CDCELPRINTK(...) printf(__VA_ARGS__)
#else
#define CDCELPRINTK(...)
#endif

#undef COUNT_OF
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((!(sizeof(x) % sizeof(0[x])))))

typedef enum cdcelDirection {
    CDCEL_WRITE = 0, // this has to match the values of i2c read/write addr
    CDCEL_READ = 1,
} cdcelDirection;

static rtems_status_code cdcelFullReadWrite(rtems_device_minor_number minor,
                                            u8 regAddr, void *data, u32 size,
                                            cdcelDirection rw)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = rtems_libi2c_send_start(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        CDCELPRINTK("rtems_libi2c_send_start failed %d\n", sc);
        return -sc;
    }

    sc = rtems_libi2c_send_addr(minor, (int)0);
    if (sc != RTEMS_SUCCESSFUL) {
        CDCELPRINTK("rtems_libi2c_send_start 1 failed %d\n", sc);
        return -sc;
    }
    sc = rtems_libi2c_write_bytes(minor, (u8 *)&regAddr, size);
    if (sc < RTEMS_SUCCESSFUL) {
        CDCELPRINTK("rtems_libi2c_read/write failed, sc = %d\n", sc);
        return -sc;
    }

    sc = rtems_libi2c_send_addr(minor, (int)rw);
    if (sc != RTEMS_SUCCESSFUL) {
        CDCELPRINTK("rtems_libi2c_send_start 2 failed %d\n", sc);
        return -sc;
    }

    switch(rw)
    {
    case CDCEL_READ:
        sc = rtems_libi2c_read_bytes(minor, (u8 *) data, size);
        break;
    case CDCEL_WRITE:
        sc = rtems_libi2c_write_bytes(minor, (u8 *) data, size);
        break;
    default:
        return RTEMS_INVALID_ID;
    }
    if (sc < RTEMS_SUCCESSFUL) {
        CDCELPRINTK("rtems_libi2c_read/write failed, sc = %d\n", sc);
        return -sc;
    }

    sc = rtems_libi2c_send_stop(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        CDCELPRINTK("rtems_libi2c_send_stop failed, sc = %d\n", sc);
        return -sc;
    }

    return -sc;
}

static rtems_status_code cdcelInit(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    UNUSED(major); // hush the compiler warning.
    UNUSED(arg); // hush the compiler warning.

    rtems_status_code sc;
    u8 data[1];

    CDCELPRINTK("CDCE asking for ID\n");

    sc = cdcelFullReadWrite(minor, 0, &data[0], 1, CDCEL_READ);

    CDCELPRINTK("CDCE  got 0x%02X | sc %d \n", data[0], sc);

    if(sc)
        return sc;

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code cdcelOpen(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    // TODO: Implement
    return RTEMS_SUCCESSFUL;
}

static rtems_status_code cdcelClose(rtems_device_major_number major,
                                          rtems_device_minor_number minor,
                                          void *arg)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    // TODO: Implement
    return RTEMS_SUCCESSFUL;
}

static rtems_status_code cdcelRead(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    // TODO: Implement
    return RTEMS_NOT_IMPLEMENTED;
}

static rtems_status_code cdcelWrite(rtems_device_major_number major,
                                    rtems_device_minor_number minor,
                                    void *arg)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    // TODO: Implement
    return RTEMS_NOT_IMPLEMENTED;
}

static rtems_status_code cdcelWriteFullCOnfig(rtems_device_major_number major,
                                              rtems_device_minor_number minor,
                                              const config_element_type configList[],
                                              size_t confCount)
{
    UNUSED(major);// hush the compiler warning.

    rtems_status_code sc;
    size_t i;

    for (i = 0 ; i < confCount ; i++)
    {
        sc = cdcelFullReadWrite(minor,
                                configList[i].address | SELECT_BYTE_ACCESS,
                                (void*)&configList[i].value,
                                1, CDCEL_WRITE);
        if(sc)
            return sc;
    }

    return RTEMS_SUCCESSFUL;

}

rtems_status_code cdcelControl(rtems_device_major_number major,
                                     rtems_device_minor_number minor,
                                     void *arg)
{
    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) arg;
    if (!ctl)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    switch (ctl->command)
    {
    case EXT_PLL_CFG_74MHZ:
        return cdcelWriteFullCOnfig(major, minor,
                             icExtPllCfg_74MHz,
                             COUNT_OF(icExtPllCfg_74MHz));
    case EXT_PLL_CFG_111MHZ:
        return cdcelWriteFullCOnfig(major, minor,
                             icExtPllCfg_111MHz,
                             COUNT_OF(icExtPllCfg_111MHz));
    case EXT_PLL_CFG_148MHZ:
        return cdcelWriteFullCOnfig(major, minor,
                             icExtPllCfg_148MHz,
                             COUNT_OF(icExtPllCfg_148MHz));
    case EXT_PLL_CFG_74_24_24MHZ:
        return cdcelWriteFullCOnfig(major, minor,
                             icExtPllCfg_74_24_24MHz,
                             COUNT_OF(icExtPllCfg_74_24_24MHz));
    case EXT_PLL_CFG_148_24_24MHZ:
        return cdcelWriteFullCOnfig(major, minor,
                             icExtPllCfg_148_24_24MHz,
                             COUNT_OF(icExtPllCfg_148_24_24MHz));
    case EXT_PLL_CFG_74_24_16MHZ:
        return cdcelWriteFullCOnfig(major, minor,
                             icExtPllCfg_74_24_16MHz,
                             COUNT_OF(icExtPllCfg_74_24_16MHz));
    default:
        return RTEMS_NOT_DEFINED;
    }

    return RTEMS_INCORRECT_STATE;
}

static rtems_driver_address_table cdcelProtocolAdrTbl = {
    .initialization_entry = cdcelInit,
    .open_entry = cdcelOpen,
    .close_entry = cdcelClose,
    .read_entry = cdcelRead,
    .write_entry = cdcelWrite,
    .control_entry = cdcelControl,
};

const rtems_libi2c_drv_t cdcelProtocolDrvTbl = {
    .ops = &cdcelProtocolAdrTbl,
    .size = sizeof(cdcelProtocolDrvTbl),
};
