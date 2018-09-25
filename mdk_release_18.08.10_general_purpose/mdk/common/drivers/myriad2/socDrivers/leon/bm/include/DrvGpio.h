///
/// @file DrvGpio.h
/// @copyright All code copyright Movidius Srl 2014, Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvGpio Gpio Driver
/// @{
/// @brief GPIO Driver.
///
/// This module contains the helper functions necessary to control the
/// general purpose IO block in the Myriad Soc
///

#ifndef _DRV_GPIO_H_
#define _DRV_GPIO_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "DrvIcb.h"
#include "DrvCommon.h"
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
 #include "DrvGpioMa2x5x.h"
#endif
#include "DrvGpioDefines.h"
// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

/// Function to initialise GPIO IRQs
///
/// This functions is used to disable IRQ generation of all GPIOs
/// It's a necessary step to using GPIO IRQs since the expected effect
/// is that only the configured GPIOs should generate interrupts.
/// Call this just once before any/all the DrvGpioIrqConfig calls
/// @return    void
/// @deprecated use DrvGpioIrqSrcDisable instead.

void DrvGpioIrqResetAll( void ) __Deprecated__("Use DrvGpioIrqSrcDisable instead");

/// Function to setup a GPIO IRQ
///
/// This is all that should be needed to setup a simple
/// irq callback from a toggling pin
/// @param[in] gpio0 GPIO number corresponding to the pin, if the GPIO no is greater or equal with the number of GPIOs then this param doesn't matter
/// @param[in] gpio1 GPIO number corresponding to the pin, if the GPIO no is greater or equal with the number of GPIOs then this param doesn't matter
/// @param[in] gpio2 GPIO number corresponding to the pin, if the GPIO no is greater or equal with the number of GPIOs then this param doesn't matter
/// @param[in] gpio3 GPIO number corresponding to the pin, if the GPIO no is greater or equal with the number of GPIOs then this param doesn't matter
/// @param[in] irqSrc DrvGpioDefines.h: 0, 1, 2, 3
///            any/more GPIOs can trigger an interrupt on any/both of these lines
///            you can Logical OR them to trigger both
/// @param[in] irqType DrvIcbDefines.h: POS_EDGE_INT, NEG_EDGE_INT, POS_LEVEL_INT, NEG_LEVEL_INT
/// @param[in] priority
/// @param[in] callback pointer to a callback function, this will be treated as an ISR
/// @return    void

void DrvGpioIrqConfig( u32 gpio0, u32 gpio1, u32 gpio2, u32 gpio3, u32 irSrc, u32 irqType,
                       u32 priority, irq_handler callback)
                                __Deprecated__("Use DrvGpioIrqConfigAnyGpio instead");

/// Function to set a GPIO High or Low
///
/// The function takes full responsibility for ensuring the pin is configured as a GPIO output
/// @param[in]  gpioNum number of the GPIO to change state
/// @param[in]  value value to set pin to 1=high, 0=low
/// @return    void
/// @note no need to set the target GPIO as output. the function does it

void DrvGpioSetPin(u32 gpioNum, u32 value);

/// Function to get a GPIO's value
///
/// The function takes full responsibility for ensuring the pin is configured as a GPIO input
/// @param[in]  gpioNum number of the GPIO to read
/// @return     0 if pin is low, 1 if pin is high
/// @note       no need to configure the target GPIO as input. the function does it

u32 DrvGpioGetPin(u32 gpioNum);

/// Function to toggle a GPIO's value
///
/// The function takes full responsibility for ensuring the pin is configured as a GPIO input
/// @param[in] gpioNum GPIO number
/// @return     void
/// @note       no need to configure the target GPIO as input. the function does it

void DrvGpioToggleState(u32 gpioNum);

/// Function to set a pin HIGH, when in mode 7
///
/// The function will set a pin HIGH if previously configured in mode 7
/// @param[in] gpioNum - number of the GPIO pin to set HIGH
/// @return     void
/// @note       user needs to configure the target GPIO in mode 7. the function doesn't do it

void DrvGpioSetPinHi(u32 gpioNum);

/// Function to set a pin LOW, when in mode 7
///
/// The function will set a pin LOW if previously configured in mode 7
/// @param[in] gpioNum - number of the GPIO pin to set LOW
/// @return     void
/// @note       user needs to configure the target GPIO in mode 7. the function doesn't do it

void DrvGpioSetPinLo(u32 gpioNum);

/// Function to get the state of a GPIO pin configured as input.
///
/// The function will get the value of the target pin
/// @param[in] gpioNum - number of the GPIO to get the value from
/// @return     value of the pin on 1 bit
/// @note       user needs to set first the GPIO in mode 7 and set the GPIO as input

u32 DrvGpioGet(u32 gpioNum);

/// Function to get the get the raw state of a GPIO configured as input.
///
/// The function will get the get the raw state of the target GPIO
/// @param[in] gpioNum - number of the GPIO pin
/// @return     value of the pin on 1 bit
/// @note       user needs to set first the GPIO in mode 7 and set the GPIO as input

u32 DrvGpioGetRaw(u32 gpioNum);

/// Function to get all the GPIO's to a table
///
/// The function will save to the target table the values from the GIO pad config scan and scan out register, for each GPIO
/// @param[in] table[] - values of the GPIOs
/// @return    void
/// @note      this function should be used instead of the deprecated GpioPadGetAllTable

void DrvGpioPadGetAllTable(u32 table[]);

/// Function to set an array of GPIOs to a value
///
/// The function will set all the GPIOs from the given array to a specific given value
/// @param[in] gpioArray[] - array of GPIOs to set
/// @param[in] size - size of array-number of GPIOs
/// @param[in] val - value to set
/// @return    void
/// @deprecated This function is deprecated. Use DrvGpioPadSetArrayVal instead. Names should start with DrvGpio* and not Gpio*

void GpioPadSetArrayVal(u32 gpioArray[],u32 size, u32 val) __Deprecated__("Use DrvGpioPadSetArrayVal instead!");

/// Function to set an array of GPIOs to a value
///
/// The function will set all the GPIOs from the given array to a specific given value
/// @param[in] gpioArray[] - array of GPIOs to set
/// @param[in] size - size of array-number of GPIOs
/// @param[in] val - value to set
/// @return    void
/// @note      this function should be used instead of the deprecated GpioPadSetArrayVal

void DrvGpioPadSetArrayVal(u32 gpioArray[],u32 size, u32 val);

/// Function to set an array of GPIOs to an array of values
///
/// The function will set all the GPIOs from the given array to a specific given value from another array
/// @param[in] gpioArray[] - array of GPIOs to set
/// @param[in] size - size of array-number of GPIOs
/// @param[in] valArray[] - array of values to set
/// @return    void
/// @deprecated This function is deprecated. Use DrvGpioPadSetArrayArray instead. Names should start with DrvGpio* and not Gpio*

void GpioPadSetArrayArray(u32 gpioArray[], u32 size, u32 valArray[])
                __Deprecated__("Use DrvGpioPadSetArrayArray instead!");

/// Function to set an array of GPIOs to an array of values
///
/// The function will set all the GPIOs from the given array to a specific given value from another array
/// @param[in] gpioArray[] - array of GPIOs to set
/// @param[in] size - size of array-number of GPIOs
/// @param[in] valArray[] - array of values to set
/// @return    void
/// @note      this function should be used instead of the deprecated GpioPadSetArrayArray

void DrvGpioPadSetArrayArray(u32 gpioArray[], u32 size, u32 valArray[]);

/// Function to get the mode of a gpio
///
/// The function will get the mode register value (0 to 7) of the target GPIO
/// @param[in] gpioNum - GPIO number
/// @return    mode of the GPIO's mode register (0 to 7)

u32 DrvGpioGetMode(u32 gpioNum);

/// Function to configure pwm0 block
///
/// The function will configure pwm0 block, without enabling it
/// @param[in] repeat - number of repetitions, 0 means continuous generation
/// @param[in] leadIn - number of cc before the signal goes HI
/// @param[in] hiCnt  - number of cc during which the signal is high
/// @param[in] lowCnt - number of cc during which the signal goes low
/// @return    void
/// @note      enabling of generation is done after this function was called , by calling DrvGpioEnPwm0
/// @note      The given number is system's cc is in reality halved because the GPIO block is running on x0.5 of system's Clock Speed
/// @deprecated This function is deprecated. Use DrvGpioSetPwm instead. There is no need to use one function for each PWM block

void DrvGpioSetPwm0(u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt)
                __Deprecated__("Use DrvGpioSetPwm instead!");

/// Function to configure pwm1 block
///
/// The function will configure pwm1 block, without enabling it
/// @param[in] repeat - number of repetitions, 0 means continuous generation
/// @param[in] leadIn - number of cc before the signal goes HI
/// @param[in] hiCnt  - number of cc during which the signal is high
/// @param[in] lowCnt - number of cc during which the signal goes low
/// @return    void
/// @note      enabling of generation is done after this function was called , by calling DrvGpioEnPwm1
/// @note      The given number is system's cc is in reality halved because the GPIO block is running on x0.5 of system's Clock Speed
/// @deprecated This function is deprecated. Use DrvGpioSetPwm instead. There is no need to use one function for each PWM block

void DrvGpioSetPwm1(u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt)
                __Deprecated__("Use DrvGpioSetPwm instead!");

/// Function to configure pwm2 block
///
/// The function will configure pwm2 block, without enabling it
/// @param[in] repeat - number of repetitions, 0 means continuous generation
/// @param[in] leadIn - number of cc before the signal goes HI
/// @param[in] hiCnt  - number of cc during which the signal is high
/// @param[in] lowCnt - number of cc during which the signal goes low
/// @return    void
/// @note      enabling of generation is done after this function was called , by calling DrvGpioEnPwm2
/// @note      The given number is system's cc is in reality halved because the GPIO block is running on x0.5 of system's Clock Speed
/// @deprecated This function is deprecated. Use DrvGpioSetPwm instead. There is no need to use one function for each PWM block

void DrvGpioSetPwm2(u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt)
                __Deprecated__("Use DrvGpioSetPwm instead!");

/// Function to configure pwm3 block
///
/// The function will configure pwm3 block, without enabling it
/// @param[in] repeat - number of repetitions, 0 means continuous generation
/// @param[in] leadIn - number of cc before the signal goes HI
/// @param[in] hiCnt  - number of cc during which the signal is high
/// @param[in] lowCnt - number of cc during which the signal goes low
/// @return    void
/// @note      enabling of generation is done after this function was called , by calling DrvGpioEnPwm3
/// @note      The given number is system's cc is in reality halved because the GPIO block is running on x0.5 of system's Clock Speed
/// @deprecated This function is deprecated. Use DrvGpioSetPwm instead. There is no need to use one function for each PWM block

void DrvGpioSetPwm3(u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt)
                __Deprecated__("Use DrvGpioSetPwm instead!");

/// Function to configure pwm4 block
///
/// The function will configure pwm4 block, without enabling it
/// @param[in] repeat - number of repetitions, 0 means continuous generation
/// @param[in] leadIn - number of cc before the signal goes HI
/// @param[in] hiCnt  - number of cc during which the signal is high
/// @param[in] lowCnt - number of cc during which the signal goes low
/// @return    void
/// @note      enabling of generation is done after this function was called , by calling DrvGpioEnPwm4
/// @note      The given number is system's cc is in reality halved because the GPIO block is running on x0.5 of system's Clock Speed
/// @deprecated This function is deprecated. Use DrvGpioSetPwm instead. There is no need to use one function for each PWM block

void DrvGpioSetPwm4(u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt)
                __Deprecated__("Use DrvGpioSetPwm instead!");

/// Function to configure pwm5 block
///
/// The function will configure pwm5 block, without enabling it
/// @param[in] repeat - number of repetitions, 0 means continuous generation
/// @param[in] leadIn - number of cc before the signal goes HI
/// @param[in] hiCnt  - number of cc during which the signal is high
/// @param[in] lowCnt - number of cc during which the signal goes low
/// @return    void
/// @note      enabling of generation is done after this function was called , by calling DrvGpioEnPwm5
/// @note      The given number is system's cc is in reality halved because the GPIO block is running on x0.5 of system's Clock Speed
/// @deprecated This function is deprecated. Use DrvGpioSetPwm instead. There is no need to use one function for each PWM block

void DrvGpioSetPwm5(u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt)
                __Deprecated__("Use DrvGpioSetPwm instead!");

/// Function to configure any pwm block
///
/// The function will configure any pwm block, without enabling it
/// @param[in] pwmBlock - pwm block
/// @param[in] repeat   - number of repetitions, 0 means continuous generation
/// @param[in] leadIn   - number of cc before the signal goes HI
/// @param[in] hiCnt    - number of cc during which the signal is high
/// @param[in] lowCnt   - number of cc during which the signal goes low
/// @return    void
/// @note      enabling of generation is done after this function was called , by calling DrvGpioEnPwm
/// @note      This function should be used instead of DrvGpioEnPwmx
/// @note      The given number is system's cc is in reality halved because the GPIO block is running on x0.5 of system's Clock Speed

void DrvGpioSetPwm(u32 pwmBlock, u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt);

/// Function to enable or disable pwm0 block
///
/// The function will enable or disable pwm0 block
/// @param[in] en - 0 used for disable, 1 for enable
/// @return    void
/// @note      enabling must be done after config, since config overrides the enable bit
/// @deprecated This function is deprecated. Use DrvGpioEnPwm instead. There is no need to use one function for each PWM block

void DrvGpioEnPwm0(u32 en) __Deprecated__("Use DrvGpioEnPwm instead!");

/// Function to enable or disable pwm1 block
///
/// The function will enable or disable pwm1 block
/// @param[in] en - 0 used for disable, 1 for enable
/// @return    void
/// @note      enabling must be done after config, since config overrides the enable bit
/// @deprecated This function is deprecated. Use DrvGpioEnPwm instead. There is no need to use one function for each PWM block

void DrvGpioEnPwm1(u32 en) __Deprecated__("Use DrvGpioEnPwm instead!");

/// Function to enable or disable pwm2 block
///
/// The function will enable or disable pwm2 block
/// @param[in] en - 0 used for disable, 1 for enable
/// @return    void
/// @note      enabling must be done after config, since config overrides the enable bit
/// @deprecated This function is deprecated. Use DrvGpioEnPwm instead. There is no need to use one function for each PWM block

void DrvGpioEnPwm2(u32 en) __Deprecated__("Use DrvGpioEnPwm instead!");

/// Function to enable or disable pwm3 block
///
/// The function will enable or disable pwm3 block
/// @param[in] en - 0 used for disable, 1 for enable
/// @return    void
/// @note      enabling must be done after config, since config overrides the enable bit
/// @deprecated This function is deprecated. Use DrvGpioEnPwm instead. There is no need to use one function for each PWM block

void DrvGpioEnPwm3(u32 en) __Deprecated__("Use DrvGpioEnPwm instead!");

/// Function to enable or disable pwm4 block
///
/// The function will enable or disable pwm4 block
/// @param[in] en - 0 used for disable, 1 for enable
/// @return    void
/// @note      enabling must be done after config, since config overrides the enable bit
/// @deprecated This function is deprecated. Use DrvGpioEnPwm instead. There is no need to use one function for each PWM block

void DrvGpioEnPwm4(u32 en) __Deprecated__("Use DrvGpioEnPwm instead!");

/// Function to enable or disable pwm5 block
///
/// The function will enable or disable pwm5 block
/// @param[in] en - 0 used for disable, 1 for enable
/// @return    void
/// @note      enabling must be done after config, since config overrides the enable bit
/// @deprecated This function is deprecated. Use DrvGpioEnPwm instead. There is no need to use one function for each PWM block

void DrvGpioEnPwm5(u32 en) __Deprecated__("Use DrvGpioEnPwm instead!");

/// Function to enable or disable a given pwm block
///
/// The function will enable or disable any pwm block
/// @param[in] en - 0 used for disable, 1 for enable
/// @return    void
/// @note      enabling must be done after config, since config overrides the enable bit
/// @note      This function should be used instead of DrvGpioEnPwmx

void DrvGpioEnPwm(u32 pwmBlock, u32 en);

/// Function to disable gpio to trigger interrupt
///
/// The function will disable the given gpio to trigger an interrupt on the irqSrc
/// @param[in] gpioNum - number of the gpio
/// @param[in] irqSrc  - one of the four interrupt sources
/// @return    < 0 if it fails, 0 on success

s32 DrvGpioIrqDisable(u32 gpioNum, u32 irqSrc);

/// Function to disable gpio to trigger interrupt
///
/// The function will disable the given gpio to trigger an interrupt on any irqSrc
/// @param[in] gpioNum - number of the gpio
/// @return    < 0 if it fails, 0 on success

s32 DrvGpioIrqDisableAny(u32 gpioNum);

/// Function to disable an interrupt source
///
/// The function will disable the given interrupt source
/// @param[in] irqSrc - one of the four interrupt sources
/// @return    void

void DrvGpioIrqSrcDisable(u32 irqSrc);

/// Function to return GPIO unused interrupt sources
///
/// The function will disable the given interrupt source
/// @param[in] void
/// @return    < 0 if it fails or [0,3] available (not yet used) gpio irq source.

s32 DrvGpioAvailableAnyIrqSrc( void );

/// Function to check if a GPIO interrupt source is used
///
/// The function will check if any of the available triggering GPIOs of the given interrupt source is available
/// @param[in] irqSrc - one of the four interrupt sources
/// @return    0 if all 4 GPIOs sources are used, 1 if any is available

u8 DrvGpioAvailableIrqSrc(u32 irqSrc);

/// Function to check if a GPIO interrupt source is used
///
/// The function will check if all triggering GPIOs of the given interrupt source are available
/// @param[in] irqSrc - one of the four interrupt sources
/// @return    0 if at least one of the triggering GPIOs is used, 1 if none is used

u8 DrvGpioIrqSrcNotUsed(u32 irqSrc);

/// Function to configure any GPIO to trigger interrupt and configure ICB and irq handler
///
/// The function will configure any given gpio to trigger an interrupt on the irqSrc (if not all four GPIOs are already used)
/// @param[in] gpioNum - gpio to trigger this interrupt to irqSrc
/// @param[in] irqSrc  - gpio int source
/// @param[in] irqType - level or edge interrupt
/// @param[in] priority - interrupt level from 0 to 15
/// @param[in] callback - interrupt callback
/// @return    < 0 if it fails, 0 on success

s32 DrvGpioIrqConfigAnyGpio(u32 gpioNum, u32 irqSrc, u32 irqType, u32 priority,
                            irq_handler callback);

/// Function to configure any GPIO to trigger interrupt
///
/// The function will configure any given gpio to trigger an interrupt on the irqSrc (if not all four GPIOs are already used)
/// @param[in] gpioNum - gpio to trigger this interrupt to irqSrc
/// @param[in] irqSrc  - gpio int source
/// @return    < 0 if it fails, 0 on success

s32 DrvGpioIrqJustConfigAnyGpio(u32 gpioNum, u32 irqSrc);

/// Function to set the edge behaviour of a GPIO timestamp.
///
/// This function will set which edge to measure the timestamp on for a
/// configured GPIO IRQ source.
/// Timestamp N records the timestamp for state changes in GPIO IRQ source N.
/// @param[in] timestamp_num - Which timestamp to set the edge configuration of.
/// @param[in] trigger - Trigger type to record the timestamp on.
/// @return    < 0 if it fails, 0 on success

s32 DrvGpioTimerSetConfig(const u32 timestamp_num,
                          const DrvGpioTriggerType trigger);

/// Function to get a GPIO timestamp.
///
/// This function will return the timestamp of the last configured edge seen on
/// a configured GPIO IRQ source.
/// Timestamp N records the timestamp for state changes in GPIO IRQ source N.
/// @param[in] timestamp_num - Which timestamp to get.
/// @param[in] timestamp    - The timestamp of the most recent trigger event.
/// @return    < 0 if it fails, 0 on success

s32 DrvGpioTimerGet(const u32 timestamp_num, u64 *const timestamp);

#ifdef __cplusplus
}
#endif

#endif // _DRV_GPIO_H_

///@}
