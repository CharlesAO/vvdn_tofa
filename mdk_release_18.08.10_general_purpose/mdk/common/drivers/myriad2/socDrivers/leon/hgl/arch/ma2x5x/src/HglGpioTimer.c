///
/// @file HglTimerGpio.c
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglTimerGpio
/// @{
/// @brief     HGL common source file for Timer GPIO
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "HglGpioTimer.h"
#include "HglMutexDefines.h"
#include <stdint.h>
#include <assert.h>
#include "registersMyriad.h"
#include "DrvRegUtils.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// From timer driver.
// Private Register Bitfield descriptions
#define TIMER_GENCFG_FREE_CNT_EN         (1 << 0)
#define TIMER_GENCFG_PRESCALER_ENABLE    (1 << 2)
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation

s32 HglGpioTimerSetConfig(const u32 timestamp_num,
                              const HglGpioTriggerType trigger) {
    u32 cfg;

    // Ensure GPIO number is in range.
    assert(timestamp_num < HGL_GPIO_MAX_IRQ_SRCS);

    // Get the current configuration.
    cfg = GET_REG_WORD_VAL(TIM0_GPIO_TIMESTAMP_CFG_ADR);

    // Update the configuration.
    cfg &= ~(3 << (timestamp_num*2)); // Clear the current configuration for the GPIO.
    cfg |= trigger << (timestamp_num*2); // Apply the new configuration.

    // Set the new configuration.
    SET_REG_WORD(TIM0_GPIO_TIMESTAMP_CFG_ADR, cfg);

    // Return success.
    return HGLMUTEX_SUCCESS;
}

s32 HglGpioTimerGet(const u32 timestamp_num, u64 *const timestamp) {
    u32 lo, hi;
    u32 tim0_config;

    // Ensure GPIO number is in range.
    assert(timestamp_num < HGL_GPIO_MAX_IRQ_SRCS);

    // Ensure real pointer provided
    assert(timestamp != NULL);

    // Ensure tim0 (timer 0) is running.
    tim0_config = GET_REG_WORD_VAL(TIM0_BASE_ADR + TIM_GEN_CONFIG_OFFSET);
    if(!(tim0_config & TIMER_GENCFG_FREE_CNT_EN) ||
       !(tim0_config & TIMER_GENCFG_PRESCALER_ENABLE)) {
        // Timer isn't running.
        return HGLMUTEX_ERROR;
    }

    // Read the timestamp registers
    lo = GET_REG_WORD_VAL(TIM0_GPIO_TIMESTAMP_0LO_ADR + (0x8 * timestamp_num));
    hi = GET_REG_WORD_VAL(TIM0_GPIO_TIMESTAMP_0HI_ADR + (0x8 * timestamp_num));

    // Save timestamp to user buffer
    *timestamp = ((u64)hi << 32) | ((u64)lo);

    // Return success.
    return HGLMUTEX_SUCCESS;
}

///@}
