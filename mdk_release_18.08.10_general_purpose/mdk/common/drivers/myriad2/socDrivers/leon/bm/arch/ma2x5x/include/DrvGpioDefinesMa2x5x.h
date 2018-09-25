///
/// @file DrvGpioDefinesMa2x5x.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvGpioMa2x5x
/// @{
/// @brief     Definitions and types needed by GPIO Driver
///

#ifndef DRV_GPIO_DEFINES_H_
#define DRV_GPIO_DEFINES_H_

// 1: Defines
// ----------------------------------------------------------------------------
#define GPIO_PWM_BLOCKS            6

/// Bit position of GPIO_PAD_RD_WR fields
#define ACTION_UPDATE_LEVEL        (1 << 0)
#define ACTION_UPDATE_PIN          (1 << 1)
#define ACTION_UPDATE_PAD          (1 << 2)
#define ACTION_TERMINATE_ARRAY     (1 << 3) ///< When this is set we treat this as last element of array

#define ACTION_UPDATE_ALL          (ACTION_UPDATE_LEVEL  | \
                                    ACTION_UPDATE_PIN    | \
                                    ACTION_UPDATE_PAD        )

#define PIN_LEVEL_LOW              (0x0) ///< We want to set this pin to Low
#define PIN_LEVEL_HIGH             (0x1) ///< We want to set this pin to High
#define PIN_LEVEL_NA               (0x3) ///< Pin Level Not Applicable (e.g. Input )

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef struct
{
    unsigned char  startGpio;
    unsigned char  endGpio;
    unsigned char  action;
    unsigned char  pinLevel;
    unsigned int   pinConfig;
    unsigned int   padConfig;
    char          *name;
} drvGpioConfigRangeType;

typedef drvGpioConfigRangeType drvGpioInitArrayType[];

// ---------------------------------------------------------------------------------------------
//                                  GPIO PIN Definitions
// ---------------------------------------------------------------------------------------------
//bit 2:0   - MODE_SELECT
#define D_GPIO_MODE_0               (0x00000)
#define D_GPIO_MODE_1               (0x00001)
#define D_GPIO_MODE_2               (0x00002)
#define D_GPIO_MODE_3               (0x00003)
#define D_GPIO_MODE_4               (0x00004)
#define D_GPIO_MODE_5               (0x00005)
#define D_GPIO_MODE_6               (0x00006)
#define D_GPIO_MODE_7               (0x00007)

//bit 3     - GPIO_DIR
#define D_GPIO_BIT_SHIFT_DIR_IN     (3)
#define D_GPIO_DIR_IN               (1 << D_GPIO_BIT_SHIFT_DIR_IN)
#define D_GPIO_DIR_OUT              (0)

//bit 4     - GPIO_DATA_INV
#define D_GPIO_DATA_INV_ON          (1 << 4)
#define D_GPIO_DATA_INV_OFF         (0)

//bit 5     - GPIO_EN_INV
#define D_GPIO_OE_INVERT_ON         (1 << 5)
#define D_GPIO_OE_INVERT_OFF        (0)

//bit 6     - GPIO_WAKE_EN
//For ma2x5x, GPIO_WAKEUP is active low, because it is inverted (from myriad1)
#define D_GPIO_WAKEUP_ON            (1 << 6)
#define D_GPIO_WAKEUP_OFF           (0)

// ---------------------------------------------------------------------------------------------
//                                  GPIO PAD Definitions in GPIO_MODE
// ---------------------------------------------------------------------------------------------
// the following bits are shifted to the left by 7 in DrvGpioPadSet for historical reasons.
// bits 8:7
#define D_GPIO_PAD_DRIVE_2mA    (0x0000)
#define D_GPIO_PAD_DRIVE_4mA    (0x0001)
#define D_GPIO_PAD_DRIVE_8mA    (0x0002)
#define D_GPIO_PAD_DRIVE_12mA   (0x0003)

//bit 9
#define D_GPIO_PAD_SLEW_FAST    (1 << 2)  // Approx 100Mhz
#define D_GPIO_PAD_SLEW_SLOW    (0)       // Approx 50Mhz

//bit 3
#define D_GPIO_PAD_SCHMITT_ON   (1 << 3)
#define D_GPIO_PAD_SCHMITT_OFF  (0)

//bit 11
#define D_GPIO_PAD_RECEIVER_ON  (1 << 4)  // Receive buffer in IO Pad enabled (default)
#define D_GPIO_PAD_RECEIVER_OFF (0)       // Receive buffer in IO Pad disabled to save power (output only cfg)

//bit 12:13 - common defines for GPIO block I/O control and Local Control
#define D_GPIO_PAD_NO_PULL      (0 << 5)
#define D_GPIO_PAD_PULL_UP      (1 << 5)
#define D_GPIO_PAD_PULL_DOWN    (2 << 5)
#define D_GPIO_PAD_BUS_KEEPER   (3 << 5)

// ---------------------------------------------------------------------------------------------
//                                  GPIO PAD Definitions in GPIO_PAD_CFG
// ---------------------------------------------------------------------------------------------

// bit 5, applicable when D_GPIO_PAD_LOCAL_PIN_OUT is selected
// To solve the overlapping with PAD pull config, an unused bit (9) is defined here,
// used by DrvGpioPadSet function to write the right bit into the HW register
#define D_GPIO_PAD_LOCALDATA_HI  (1 << 9) // When LOCAL_CTRL is ON; this bit selects the output value of the pin
#define D_GPIO_PAD_LOCALDATA_LO  (0     )

#define D_GPIO_PAD_LOCAL_PIN_IN  (1 << 7) // When LOCAL_CTRL is ON; this setting makes the pin an input
#define D_GPIO_PAD_LOCAL_PIN_OUT (0     ) // When LOCAL_CTRL is ON; this setting makes the pin an output

#define D_GPIO_PAD_LOCALCTRL_ON  (1 << 8) // When this bit is set, gpio controled from scan chain via LocalData/Oen
#define D_GPIO_PAD_LOCALCTRL_OFF (0     )

// Used by software to decide whether Local Pad Control have to be written instead of GPIO block register
// Local Control can be used when the main power island is disabled to ensure pin stays in desired configuration
#define D_GPIO_PAD_LOCALCTRL_UPDATE (1 << 10)

// Some default configurations
#define D_GPIO_PAD_DEFAULTS  (D_GPIO_PAD_NO_PULL       | \
                              D_GPIO_PAD_DRIVE_2mA     | \
                              D_GPIO_PAD_SLEW_SLOW     | \
                              D_GPIO_PAD_SCHMITT_OFF   | \
                              D_GPIO_PAD_RECEIVER_ON   | \
                              D_GPIO_PAD_LOCALCTRL_OFF | \
                              D_GPIO_PAD_LOCALDATA_LO  | \
                              D_GPIO_PAD_LOCAL_PIN_OUT )

// gpio int registers  ============================================================================================================
#define D_GPIO_INT_EN_0 (1<<7)
#define D_GPIO_INT_EN_1 (1<<15)
#define D_GPIO_INT_EN_2 (1<<23)
#define D_GPIO_INT_EN_3 (1<<31)
#define D_GPIO_INT_GPIO_MASK_0 (0x7f<<0)
#define D_GPIO_INT_GPIO_MASK_1 (0x7f<<8)
#define D_GPIO_INT_GPIO_MASK_2 (0x7f<<16)
#define D_GPIO_INT_GPIO_MASK_3 (0x7f<<24)

//Masks
#define D_GPIO_PWM_LEADIN_REPEAT_MASK 0xFFFF
#define D_GPIO_PWM_LEADIN_LEADIN_MASK 0x7FFF0000
#define D_GPIO_PWM_LEADIN_ENABLE_MASK 0x80000000

#define D_GPIO_PWM_HIGHLOW_HIGH_MASK 0xFFFF0000
#define D_GPIO_PWM_HIGHLOW_LOW_MASK 0xFFFF

#define D_GPIO_MODE_MASK 0xFFFF

#endif // DRV_GPIO_DEF_H_

///@}
