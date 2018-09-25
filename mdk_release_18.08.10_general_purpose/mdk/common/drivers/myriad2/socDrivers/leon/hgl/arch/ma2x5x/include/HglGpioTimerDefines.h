///
/// @file HglGpioTimerDefines.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglGpioTimer
/// @{
/// @brief     Defines to be used by the functionality implementation of the hardware
///
///            mutexes of the Myriad platform.
///

#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_HGL_GPIO_TIMER_DEFINES_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_HGL_GPIO_TIMER_DEFINES_H_

// Maximum configurable IRQ sources for ma2x5x SoCs.
#define HGL_GPIO_MAX_IRQ_SRCS   (4)

// Trigger type of GPIO for timestamp to trigger off.
typedef enum {
    HGL_GPIO_TRIGGER_NONE = 0,
    HGL_GPIO_TRIGGER_EDGE_POSITIVE = 1,
    HGL_GPIO_TRIGGER_EDGE_NEGATIVE = 2,
    HGL_GPIO_TRIGGER_EDGE_BOTH = 3,
} HglGpioTriggerType;

#endif

///@}
