///
/// @file OsDrvGpio.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvGpio OsGpio Driver
/// @{
/// @brief     GPIO RTEMS API Functions.
///
// --------------------------------------------------------------------------------
///


#ifndef _OS_DRV_GPIO_H_
#define _OS_DRV_GPIO_H_

// 1: Includes
// ----------------------------------------------------------------------------

#include "DrvGpio.h"
#include "OsCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OSDRVGPIO_DEVNAME               "/dev/gpioblock"

// 2:  Exported Global Data
// ----------------------------------------------------------------------------

typedef struct
{
    u32 pinNum;
    void (*handler)(void);
} handlerArguments;

typedef enum {
   GPIO_LOW = 0,
   GPIO_HIGH
} gpioVal_t;

typedef struct {
    u8 priority;
    u8 type;
    void (*handler)(void *context);
    void *context;
} gpioInterruptHndl_t;

typedef struct {
    u8 pinNum;
    void (*handler)(void);
} handlerArguments_t;

typedef struct pwmSettings_t {
    u32 repeat;
    u32 leadIn;
    u32 highCount;
    u32 lowCount;
} pwmSettings_t;

typedef enum tyGpioControl
{
    GPIO_ARRAY_CONFIG,
    GPIO_SET_MODE,
    GPIO_GET_MODE,
    GPIO_CONFIGURE_AND_ENABLE_PWM,
    GPIO_CONFIGURE_PWM,
    GPIO_ENABLE_PWM,
    GPIO_DISABLE_PWM,
    GPIO_IRQ_ENABLE,
    GPIO_IRQ_DISABLE,
    GPIO_IRQ_DISABLE_SPECIFIC,
    GPIO_DEFAULT_TIMEOUT,
    GPIO_GET_RAW,
    GPIO_TIMER_SET_CONFIG,
    GPIO_TIMER_GET,
} gpioControl_t;

// 3:  Exported Functions
// ----------------------------------------------------------------------------

extern rtems_driver_address_table osDrvGpioTblName;

#ifdef __cplusplus
}
#endif


#endif //_OS_DRV_GPIO_H_

