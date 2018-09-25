#ifndef OS_WM_8325_H__
#define OS_WM_8325_H__

#include <mv_types.h>
#include <rtems.h>
#include <rtems/libi2c.h>
#include "stdio.h"

#ifndef COUNT_OF
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((!(sizeof(x) % sizeof(0[x])))))
#endif

#define WM8325_NAME "wm8325"

/// WM8325 slave addresses
#define WM8325_SLAVE_ADDR 0x36
#define WM8325_I2C_SELECT_PIN_NO 3
typedef enum wm8325Control_t
{
    WM8325_GET_GPIO,
    WM8325_SET_GPIO,
    WM8325_MODE_SET_GPIO,
    WM8325_SET_LED,
    WM8325_SET_GPIO_MASK,
} wm8325Control_t;

typedef struct wm8325GpioStruct_t
{
    u8 gpioNr;
    u8 gpioVal;
    enum {
        WM8325_INPUT,
        WM8325_OUTPUT
    } gpioMode;
} wm8325GpioStruct_t;

typedef struct wm8325GpioMaskStruct_t
{
    u16 gpioMask;       /// For when operating on multiple gpios at once with mask
    u8  gpioVal;
} wm8325GpioMaskStruct_t;

typedef struct wm8325LedStruct_t
{
    enum {
        LED1 = 0,
        LED2
    } nr;
    enum {
        WM8325_MODE_OFF = 0x0,
        WM8325_POWER_STATE_STATUS = 0x1,
        WM8325_MANUAL = 0x3,
    } source:2;
    enum {
        WM8325_OFF = 0x0,
        WM8325_ON = 0x1,
        WM8325_CONTINUOUS_PULSE = 0x2,
        WM8325_PULSED_SEQUENCE = 0x3
    } mode:2;
    enum {
        WM8325_1_PULSE = 0x0,
        WM8325_2_PULSE = 0x1,
        WM8325_4_PULSE = 0x2,
        WM8325_7_PULSE = 0x3,
    } sequenceLength:2;
    enum {
        WM8325_1000_MS = 0x0,
        WM8325_250_MS = 0x1,
        WM8325_125_MS = 0x2,
        WM8325_62_5_MS = 0x3,
    }onTime:2;
    enum {
        WM8325_50_0_PERCENT = 0x0,
        WM8325_33_3_PERCENT = 0x1,
        WM8325_25_0_PERCENT = 0x2,
        WM8325_12_5_PERCENT = 0x3,
    }dutyCycle:2;
} wm8325LedStruct_t;

#ifdef __cplusplus
extern "C" {
#endif

extern const rtems_libi2c_drv_t wm8325ProtocolDrvTbl;

#ifdef __cplusplus
}
#endif

#endif
