#include <rtems.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>

#include <OsADV7513.h>
#include "DrvADV7513Configs.h"

#if 0
#define ADV7513PRINTK(...) printf(__VA_ARGS__)
#else
#define ADV7513PRINTK(...)
#endif

#undef COUNT_OF
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((!(sizeof(x) % sizeof(0[x])))))

typedef enum adv7513Direction {
    ADV7513_WRITE = 0, // this has to match the values of i2c read/write addr
    ADV7513_READ = 1,
} adv7513Direction;

static rtems_status_code adv7513FullReadWrite(rtems_device_minor_number minor,
                                              u8 regAddr, void *data, u32 size,
                                              adv7513Direction rw)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    sc = rtems_libi2c_send_start(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        ADV7513PRINTK("rtems_libi2c_send_start failed %d\n", sc);
        return -sc;
    }

    sc = rtems_libi2c_send_addr(minor, (int)0);
    if (sc != RTEMS_SUCCESSFUL) {
        ADV7513PRINTK("rtems_libi2c_send_start 1 failed %d\n", sc);
        return -sc;
    }
    sc = rtems_libi2c_write_bytes(minor, (u8 *)&regAddr, size);
    if (sc < RTEMS_SUCCESSFUL) {
        ADV7513PRINTK("rtems_libi2c_read/write failed, sc = %d\n", sc);
        return -sc;
    }

    sc = rtems_libi2c_send_addr(minor, (int)rw);
    if (sc != RTEMS_SUCCESSFUL) {
        ADV7513PRINTK("rtems_libi2c_send_start 2 failed %d\n", sc);
        return -sc;
    }

    switch(rw)
    {
    case ADV7513_READ:
        sc = rtems_libi2c_read_bytes(minor, (u8 *) data, size);
        break;
    case ADV7513_WRITE:
        sc = rtems_libi2c_write_bytes(minor, (u8 *) data, size);
        break;
    default:
        return RTEMS_INVALID_ID;
    }
    if (sc < RTEMS_SUCCESSFUL) {
        ADV7513PRINTK("rtems_libi2c_read/write failed, sc = %d\n", sc);
        return -sc;
    }

    sc = rtems_libi2c_send_stop(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        ADV7513PRINTK("rtems_libi2c_send_stop failed, sc = %d\n", sc);
        return -sc;
    }

    return -sc;
}

static rtems_status_code adv7513Init(rtems_device_major_number major,
                                     rtems_device_minor_number minor,
                                     void *arg)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code sc;
    u8 data;

    ADV7513PRINTK("ADV7513 asking for ID\n");

    sc = adv7513FullReadWrite(minor, 0, &data, 1, ADV7513_READ);

    ADV7513PRINTK("ADV7513  got 0x%02X | sc %d \n", data, sc);

    if(sc)
        return sc;

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code adv7513Open(rtems_device_major_number major,
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

static rtems_status_code adv7513Close(rtems_device_major_number major,
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

static rtems_status_code adv7513WriteConfig(rtems_device_major_number major,
                                            rtems_device_minor_number minor,
                                            const adv7513ElementType configList[],
                                            size_t confCount)
{
    UNUSED(major);// hush the compiler warning.

    rtems_status_code sc;
    size_t i;
    for (i = 0 ; i < confCount ; i++)
    {
        sc = adv7513FullReadWrite(minor,
                                  configList[i].address,
                                  (void*)&configList[i].value,
                                  1, ADV7513_WRITE);
        if(sc)
            return sc;
    }
    return RTEMS_SUCCESSFUL;
}

rtems_status_code adv7513Control(rtems_device_major_number major,
                                 rtems_device_minor_number minor,
                                 void *arg)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) arg;
    adv7513ElementType* extraRegs = NULL;
    u32 extraRegsCount = 0;

    if (ctl != NULL)
    {
        switch (ctl->command)
        {
            case ADV7513_CFG_720P30:
                extraRegs = extra720Adv513Regs;
                extraRegsCount = COUNT_OF(extra720Adv513Regs);
                break;
            case ADV7513_CFG_1080P60:
                extraRegs = extra1080Adv513Regs;
                extraRegsCount = COUNT_OF(extra1080Adv513Regs);
                break;
            case ADV7513_CFG_DEFAULT: /* default configuration also returns an error */
            default:
                sc = RTEMS_NOT_DEFINED;
        }

        if(sc == RTEMS_SUCCESSFUL)
        {
            sc = adv7513WriteConfig(major, minor,
                         initAdv513Regs,
                         COUNT_OF(initAdv513Regs));
        }

        if(sc == RTEMS_SUCCESSFUL)
        {
            sc = adv7513WriteConfig(major, minor,
                                    extraRegs, extraRegsCount);
        }
    }
    else
    {
        sc = RTEMS_INVALID_ADDRESS;
    }

    ctl->ioctl_return = sc;
    return sc;
}

rtems_status_code adv7513Dummy(rtems_device_major_number major,
                                 rtems_device_minor_number minor,
                                 void *arg)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    return RTEMS_NOT_IMPLEMENTED;
}
static const rtems_driver_address_table adv7513ProtocolAdrTbl = {
    .initialization_entry = adv7513Init,
    .open_entry = adv7513Open,
    .close_entry = adv7513Close,
    .read_entry = adv7513Dummy,
    .write_entry = adv7513Dummy,
    .control_entry = adv7513Control,
};

const rtems_libi2c_drv_t adv7513ProtocolDrvTbl = {
    .ops = &adv7513ProtocolAdrTbl,
    .size = sizeof(adv7513ProtocolDrvTbl),
};
