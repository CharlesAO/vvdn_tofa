///
/// @file DrvGpioMa2x5x.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
///
/// This module contains the helper functions necessary to control the
/// general purpose IO block in the MA2x5x Myriad
///

#ifndef _DRV_GPIOMA2x5x_H_
#define _DRV_GPIOMA2x5x_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "DrvGpioDefinesMa2x5x.h"
#include "DrvIcb.h"
#include "DrvCommon.h"

// 2:  Defines
// ----------------------------------------------------------------------------
// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

/// Function to initialise multiple ranges of GPIOs with Pad, Pin and Level configuration
///
/// This function is typically used to enable the default GPIO configuration for a particular board
/// It allows the caller to optionally configure each of the Pin Config; Pad Config; and default level of multiple
/// ranges of GPIOS.
/// For details of the configuration structure see drvGpioInitArrayType
/// This array is of variable length and is terminted by a ACTION_TERMINATE_ARRAY in the action field
/// If Pad configuration change is needed this is performed in a memory effiecient way

/// @param[in] initArray array of type drvGpioInitArrayType which contains a structure defining the desired GPIO config changes
/// @return    void

void DrvGpioInitialiseRange(const drvGpioInitArrayType initArray);

/// Function to set the mode of a gpio
///
/// The function will set the mode register of the target GPIO to a given value (0 to 7)
/// @param[in] gpioNum - GPIO number
/// @param[in] val - value to write
/// @return    void
/// @note      this function should be used instead of the deprecated DrvGpioMode

void DrvGpioSetMode(u32 gpioNum, u32 val);

/// Function to set the mode of a gpio
///
/// The function will set the mode register of the target GPIO to a given value (0 to 7)
/// @param[in] gpioNum - GPIO number
/// @param[in] val - value to write
/// @return    void
/// @deprecated This function is deprecated. Use DrvGpioSetMode instead.

void DrvGpioMode(u32 gpioNum, u32 val) __Deprecated__("Use DrvGpioSetMode instead!");

/// Function to set the mode register of some GPIOs
///
/// The function will get the mode register value (0 to 7) of the target GPIO
/// @param[in] gpioNum1 - GPIO to start from
/// @param[in] gpioNum2 - GPIO to end with
/// @param[in] val - value to write
/// @return    void
/// @note      the mode of the gpio must be set first and gpioNum2 > gpioNum1

void DrvGpioModeRange(u32 gpioNum1, u32 gpioNum2, u32 val);

/// Function to set the mode register of a list of gpios
///
/// The function will set the mode register, from a given list, to a given value
/// @param[in] pList - pointer to a list of gpios
/// @param[in] size - size of the list of gpios
/// @param[in] val - value to be written in each gpio mode register
/// @return    void
/// @note      the mode of the gpio must be set first and max size of list is 85

void DrvGpioModeListVal(u32 *pList, u32 size, u32 val);

/// Function to set gpios to given modes
///
/// The function will set the mode register of a list of gpios, with a list of values
/// @param[in] *gpioList - pointer to a list of gpios
/// @param[in] size - size of the list of gpios
/// @param[in] *valList - pointer to a list of values
/// @return    void
/// @note      the mode of the gpio must be set first and max size of lists is 85

void DrvGpioModeListList(u32 *gpioList, u32 size, u32 *valList);

/// Function to set all the GPIO's from a table
///
/// The function will configure all GPIOs (0 to 84) to a specific value from the given table
/// @param[in] table[] - values to set to the GPIOs
/// @return     void
/// @deprecated This function is deprecated. Use DrvGpioPadSetAllTable instead. Names should start with DrvGpio* and not Gpio*

void GpioPadSetAllTable(u32 table[]) __Deprecated__("Use DrvGpioPadSetAllTable instead");

/// Function to set all the GPIO's from a table
///
/// The function will configure all GPIOs (0 to 84) to a specific value from the given table
/// @param[in] table[] - values to set to the GPIOs
/// @return    void
/// @note      this function should be used instead of the deprecated GpioPadSetAllTable

void DrvGpioPadSetAllTable(u32 table[]);

/// Function to get all the GPIO's to a table
///
/// The function will save to the target table the values from the GIO pad config scan and scan out register, for each GPIO
/// @param[in]  table[] - values of the GPIOs
/// @return     void
/// @deprecated This function is deprecated. Use DrvGpioPadGetAllTable instead. Names should start with DrvGpio* and not Gpio*

void GpioPadGetAllTable(u32 table[]);

/// Function to set a value to a target GPIOs PAD
///
/// The function will set to the target GPIO (GIO pad config scan and scan out register) the given value
/// @param[in] gpioNum - number of the GPIO pin
/// @param[in] value - value to write to the GPIOs pad
/// @return    void
/// @deprecated This function is deprecated. Use DrvGpioPadSet instead. Names should start with DrvGpio* and not Gpio*

void GpioPadSet(u32 gpioNum, u32 val) __Deprecated__("Use DrvGpioPadSet instead!");

/// Function to set a value to a target GPIOs PAD
///
/// The function will set to the target GPIO (GIO pad config scan and scan out register) the given value
/// @param[in] gpioNum - number of the GPIO pin
/// @param[in] value - value to write to the GPIOs pad
/// @return    void
/// @note      this function should be used instead of the deprecated GpioPadSet

void DrvGpioPadSet(u32 gpioNum, u32 val);

/// Function to set a value to a target GPIOs PAD using fast shifts
///
/// The function will fast set to the target GPIO (GIO pad config scan and scan out register) the given value
/// @param[in] gpioNum - number of the GPIO pin
/// @param[in] value - value to write to the GPIOs pad
/// @return    void
/// @deprecated This function is deprecated. Use DrvGpioPadSetFast instead. Names should start with DrvGpio* and not Gpio*

void GpioPadSetFast(u32 gpioNum, u32 val) __Deprecated__("Use DrvGpioPadSetFast instead!");

/// Function to set a value to a target GPIOs PAD using fast shifts
///
/// The function will fast set to the target GPIO (GIO pad config scan and scan out register) the given value
/// @param[in] gpioNum - number of the GPIO pin
/// @param[in] value - value to write to the GPIOs pad
/// @return    void
/// @note      this function should be used instead of the deprecated GpioPadSetFast

void DrvGpioPadSetFast(u32 gpioNum, u32 val);

/// Function to set a value to a target GPIOs PAD using fast shifts and masks
///
/// The function will fast set with masks the target GPIO (GIO pad config scan and scan out register)
/// @param[in] gpioNum - number of the GPIO pin
/// @param[in] maskClear - clear the bits set in maskClear
/// @param[in] maskSet  -  set bits set in maskSet
/// @return    void
/// @deprecated This function is deprecated. Use DrvGpioPadSetFastMask instead. Names should start with DrvGpio* and not Gpio*

void GpioPadSetFastMask(u32 gpioNum, u32 maskClear, u32 maskSet)
                __Deprecated__("Use DrvGpioPadSetFastMask instead!");

/// Function to set a value to a target GPIOs PAD using fast shifts and masks
///
/// The function will fast set with masks the target GPIO (GIO pad config scan and scan out register)
/// @param[in] gpioNum - number of the GPIO pin
/// @param[in] maskClear - clear the bits set in maskClear
/// @param[in] maskSet  -  set bits set in maskSet
/// @return    void
/// @note      this function should be used instead of the deprecated GpioPadSetFastMask

void DrvGpioPadSetFastMask(u32 gpioNum, u32 maskClear, u32 maskSet);

/// Function to get a value of a target GPIO's PAD
///
/// The function will get the target GPIO PAD value
/// @param[in] gpioNum - number of the GPIO pin
/// @return    PAD value of the GPIO
/// @deprecated This function is deprecated. Use DrvGpioPadGet instead. Names should start with DrvGpio* and not Gpio*

u32 GpioPadGet(u32 gpioNum) __Deprecated__("Use DrvGpioPadGet instead!");

/// Function to get a value of a target GPIO's PAD
///
/// The function will get the target GPIO PAD value
/// @param[in] gpioNum - number of the GPIO pin
/// @return    PAD value of the GPIO
/// @note      this function should be used instead of the deprecated GpioPadGet

u32 DrvGpioPadGet(u32 gpioNum);

/// Function to set a range of GPIOs to a value
///
/// The function will set a range of GPIOs to a specific given value
/// @param[in] gpioStart - GPIO number to start from
/// @param[in] GPIO number to end with
/// @param[in] val - value to write
/// @return    void
/// @deprecated This function is deprecated. Use DrvGpioPadSetRangeVal instead. Names should start with DrvGpio* and not Gpio*

void GpioPadSetRangeVal(u32 gpioStart,u32 gpioEnd, u32 val) __Deprecated__("Use DrvGpioPadSetRangeVal instead!");

/// Function to set a range of GPIOs to a value
///
/// The function will set a range of GPIOs to a specific given value
/// @param[in] gpioStart - GPIO number to start from
/// @param[in] GPIO number to end with
/// @param[in] val - value to write
/// @return    void
/// @note      this function should be used instead of the deprecated GpioPadSetRangeVal

void DrvGpioPadSetRangeVal(u32 gpioStart,u32 gpioEnd, u32 val);

#ifdef __cplusplus
}
#endif

#endif // _DRV_GPIOMA2x5x_H_
