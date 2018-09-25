///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     power manager: low power API definitions
///

#ifndef LOW_POWER_API_H_
#define LOW_POWER_API_H_

#define LOCKABLE_TEXT    __attribute__((section(".lockable.text")))
#define LOCKABLE_DATA    __attribute__((section(".lockable.data")))
#define LOCKABLE_RODATA  __attribute__((section(".lockable.rodata")))

typedef enum {
    LP_WAKEUP_SOURCE_NONE = 0,
    LP_WAKEUP_SOURCE_GPIO,
} LpSrcWakeup;

typedef enum {
    LP_SRC_DEFAULT_CLK = 0,
    LP_SRC_32KHZ_CLK,
    LP_SRC_SYS_CLK
} LpSrcClk;

typedef struct {
    struct {
        LpSrcWakeup source;
        union {
            struct {
                uint32_t number;
            } gpio;
        } data;
    } wakeup;
    struct {
        LpSrcClk source;
        union {
            struct {
                uint16_t numerator;   //for 12Mhz fractions
                uint16_t denominator; //
            } sys;
        } data;
    } clock;
} LowPowerInfo;

// 1: Includes
// ----------------------------------------------------------------------------

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// Enter the Myriad2 Low Power State. Called by the RTEMS idle loop
/// @param[in]    - none
/// @param[out]   - none
/// @return       - none
extern void pwrManagerEnterLowPower(void);

#endif // LOW_POWER_API_H_
