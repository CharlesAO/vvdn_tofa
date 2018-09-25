#include <rtems.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>
#include <rtems/bspIo.h>
#include "OsCommon.h"
#include "OsCross.h"
#include <DrvTimer.h>
#include <DrvCpr.h>
#include <OsDrvGpio.h>
#include <OsWm8325.h>

#include <DrvWm8325Defines.h>

#if 1
#define WMPRINTK(...) printf(__VA_ARGS__)
#else
#define WMPRINTK(...)
#endif


typedef enum wm8325Direction {
    WM8325_WRITE = 0, // this has to match the values of i2c read/write addr
    WM8325_READ = 1,
} wm8325Direction;

static rtems_status_code wm8325ModeSetGpio(rtems_device_minor_number minor,
                                           wm8325GpioStruct_t *arg);
static rtems_status_code wm8325SetGpio(rtems_device_minor_number minor,
                                       wm8325GpioStruct_t *arg);
static rtems_status_code wm8325WriteMask(rtems_device_minor_number minor,
                                       wm8325GpioMaskStruct_t *arg);

static rtems_status_code wm8325FullReadWrite(rtems_device_minor_number minor,
                                             u16 address, u16 *data, u32 count,
                                             wm8325Direction rw)
{
    rtems_status_code sc;
    u16 wrdata;
    sc = rtems_libi2c_send_start(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        WMPRINTK("rtems_libi2c_send_start failed %d\n", sc);
        return -sc;
    }
    sc = rtems_libi2c_send_addr(minor, 0);
    if (sc != RTEMS_SUCCESSFUL){
        return sc;
    }
    address = (((address&0xFF00) >> 8) | ((address&0x00FF) << 8));
    sc = rtems_libi2c_write_bytes(minor, (u8 *)&address, sizeof(u16));
    if (sc < 0){
        return -sc;
    }
    switch(rw)
    {
    case WM8325_READ:
        sc = rtems_libi2c_send_addr(minor, 1);
        if (sc != RTEMS_SUCCESSFUL) {
            WMPRINTK("rtems_libi2c_send_start failed %d\n", sc);
            return -sc;
        }
        sc = rtems_libi2c_read_bytes(minor, (u8*)data, count * sizeof(u16));
        break;
    case WM8325_WRITE:
        wrdata = (((*data&0xFF00) >> 8) | ((*data&0x00FF) << 8));
        sc = rtems_libi2c_write_bytes(minor, (u8*)&wrdata, count * sizeof(u16));
        break;
    default:
        return RTEMS_INVALID_NUMBER;
    }
    if (sc < RTEMS_SUCCESSFUL) {
        WMPRINTK("rtems_libi2c_read/write failed, sc = %d\n", sc);
        return -sc;
    }
    sc = rtems_libi2c_send_stop(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        WMPRINTK("rtems_libi2c_send_stop failed, sc = %d\n", sc);
        return -sc;
    }

    *data = (((*data&0xFF00) >> 8) | ((*data&0x00FF) << 8));

    return -sc;
}

static rtems_status_code wm8325Init(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    return RTEMS_SUCCESSFUL;
}


static rtems_status_code wm8325Open(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code wm8325Close(rtems_device_major_number major,
                                          rtems_device_minor_number minor,
                                          void *arg)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code wm8325Read(rtems_device_major_number major,
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

static rtems_status_code wm8325Write(rtems_device_major_number major,
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

static rtems_status_code wm8325ModeSetGpio(rtems_device_minor_number minor,
                                           wm8325GpioStruct_t *arg)
{
    rtems_status_code sc;
    u16 val = 0;
    u16 reg;

    if (arg->gpioNr > 13)
        return RTEMS_INVALID_ID;

    reg = WM8325_GPIO1_CONTROL +  (arg->gpioNr - 1);

    switch(arg->gpioMode)
    {
    case WM8325_INPUT:
        val = WM8325_GPIO_NORMAL_IN_MODE;
        break;
    case WM8325_OUTPUT:
        val = WM8325_GPIO_NORMAL_OUT_MODE;
        break;
    default:
        return RTEMS_INVALID_ID;
    }

    sc = wm8325FullReadWrite(minor, reg, &val, 1, WM8325_WRITE);
    return sc;
}

static rtems_status_code wm8325SetGpio(rtems_device_minor_number minor,
                                       wm8325GpioStruct_t *arg)
{
    rtems_status_code sc;
    u16 val = 0;
    u16 gpioMask;
    u16 gpioLevel;

    if (arg->gpioNr > 13)
        return RTEMS_INVALID_NUMBER;


    sc = wm8325FullReadWrite(minor, WM8325_GPIO_LEVEL, &val, 1, WM8325_READ);
    if(sc)
    {
        return sc;
    }

    gpioMask = 1 << (arg->gpioNr - 1);
    gpioLevel = arg->gpioVal ? gpioMask : 0;

    val = val & ~gpioMask;
    val = val | gpioLevel;

    sc = wm8325FullReadWrite(minor, WM8325_GPIO_LEVEL, &val, 1, WM8325_WRITE);
    return sc;
}

static rtems_status_code wm8325GetGpio(rtems_device_minor_number minor,
                                       wm8325GpioStruct_t *arg)
{
    rtems_status_code sc;
    u16 val = 0;
    u16 gpioMask;

    if (arg->gpioNr > 13)
        return RTEMS_INVALID_NUMBER;

    sc = wm8325FullReadWrite(minor, WM8325_GPIO_LEVEL, &val, 1, WM8325_READ);
    if(sc)
        return sc;

    gpioMask = 1 << (arg->gpioNr - 1);

    arg->gpioVal =  val & gpioMask ? 1 : 0;

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code wm8325SetLed(rtems_device_minor_number minor,
                                      wm8325LedStruct_t *arg)
{
    rtems_status_code sc;
    u16 reg;
    u16 val = 0;

    if (arg->nr > LED2)
        return RTEMS_INVALID_NUMBER;
    reg = WM8325_STATUS_LED_1 + (u16)arg->nr;

    val |= ((u16)arg->source) << WM8325_LED1_SRC_SHIFT;
    val |= ((u16)arg->mode) << WM8325_LED1_MODE_SHIFT;
    val |= ((u16)arg->sequenceLength) << WM8325_LED1_SEQ_LEN_SHIFT;
    val |= ((u16)arg->onTime) << WM8325_LED1_DUR_SHIFT;
    val |= ((u16)arg->dutyCycle) << WM8325_LED1_DUR_SHIFT;

    sc = wm8325FullReadWrite(minor, reg, &val, 1, WM8325_WRITE);

    return sc;
}

static rtems_status_code wm8325WriteMask(rtems_device_minor_number minor,
                                       wm8325GpioMaskStruct_t *arg)
{
    rtems_status_code sc;
    u16 oldval = 0;
    u16 newval = 0;
    u16 gpioMask;

    sc = wm8325FullReadWrite(minor, WM8325_GPIO_LEVEL, &oldval, 1, WM8325_READ);
    if(sc)
        return sc;

    gpioMask  = arg->gpioMask;

    newval = (oldval & ~gpioMask) |  (arg->gpioVal&1)*gpioMask;

    sc = wm8325FullReadWrite(minor, WM8325_GPIO_LEVEL, &newval, 1, WM8325_WRITE);
    if(sc)
        return sc;

    return RTEMS_SUCCESSFUL;
}


rtems_status_code wm8325Control(rtems_device_major_number major,
                                     rtems_device_minor_number minor,
                                     void *arg)
{
    UNUSED(major);// hush the compiler warning.

    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) arg;

    if (!ctl || !ctl->buffer)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    switch (ctl->command)
    {
    case WM8325_GET_GPIO:
        ctl->ioctl_return = wm8325GetGpio(minor, (wm8325GpioStruct_t*)ctl->buffer);
        return ctl->ioctl_return;
        break;
    case WM8325_SET_GPIO:
        ctl->ioctl_return = wm8325SetGpio(minor, (wm8325GpioStruct_t*)ctl->buffer);
        return ctl->ioctl_return;
        break;
    case WM8325_MODE_SET_GPIO:
        ctl->ioctl_return =  wm8325ModeSetGpio(minor, (wm8325GpioStruct_t*)ctl->buffer);
        return ctl->ioctl_return;
        break;
    case WM8325_SET_LED:
        ctl->ioctl_return =  wm8325SetLed(minor, (wm8325LedStruct_t*)ctl->buffer);
        return ctl->ioctl_return;
        break;
    case WM8325_SET_GPIO_MASK:
        ctl->ioctl_return =  wm8325WriteMask(minor, (wm8325GpioMaskStruct_t*)ctl->buffer);
        return ctl->ioctl_return;
        break;
    default:
        ctl->ioctl_return = RTEMS_INVALID_ID;
        return RTEMS_INVALID_ID;
    }

    return RTEMS_UNSATISFIED;
}

static const rtems_driver_address_table wm8325ProtocolAdrTbl = {
    .initialization_entry = wm8325Init,
    .open_entry = wm8325Open,
    .close_entry = wm8325Close,
    .read_entry = wm8325Read,
    .write_entry = wm8325Write,
    .control_entry = wm8325Control,
};

const rtems_libi2c_drv_t wm8325ProtocolDrvTbl = {
    .ops = &wm8325ProtocolAdrTbl,
    .size = sizeof(wm8325ProtocolDrvTbl),
};
