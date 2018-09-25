///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Default GPIO configuration for the MV0212 Board
///
/// Using the structure defined by this board it is possible to initialize
/// some of the GPIOS on the MV0212 PCB to good safe initial defaults
///

#include <DrvGpio.h>

const drvGpioInitArrayType brdMV0212GpioCfgDefault =
{
    // -----------------------------------------------------------------------
    // SD Configuration
    // -----------------------------------------------------------------------
    {16, 16 , ACTION_UPDATE_ALL                 // SDIO interface
     ,
     0
     ,
     D_GPIO_MODE_3                              // SDIO Mode
     ,
     D_GPIO_PAD_DRIVE_4mA | D_GPIO_PAD_RECEIVER_ON
     , NULL
    },

    {17, 17 , ACTION_UPDATE_ALL                 // SDIO interface
     ,
     0
     ,
     D_GPIO_MODE_3                              // SDIO Mode
     ,
     D_GPIO_PAD_DRIVE_4mA
     , NULL
    },

    {18, 21 , ACTION_UPDATE_ALL                 // SDIO interface
    ,
     0
     ,
     D_GPIO_MODE_3                              // SDIO Mode
     ,
     D_GPIO_PAD_DRIVE_4mA | D_GPIO_PAD_RECEIVER_ON
     , NULL
    },

    // -----------------------------------------------------------------------
    // InfraRed Sensor Pin
    // -----------------------------------------------------------------------

    {14, 14 , ACTION_UPDATE_ALL           // Infrared Sensor Pin
     ,
     PIN_LEVEL_LOW               //
     ,
     D_GPIO_MODE_7            |  // Direct GPIO mode
     D_GPIO_DIR_IN            |  // Input
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },

    // -----------------------------------------------------------------------
    // I2C0 Configuration  (General use)
    // -----------------------------------------------------------------------
    {60, 60, ACTION_UPDATE_ALL          // I2C0 Pins
     ,
     PIN_LEVEL_HIGH
     ,
     D_GPIO_MODE_0            |  // Mode 0 to select I2C Mode
     D_GPIO_DIR_OUT            |
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , "i2c0_scl"
    },
    {61, 61, ACTION_UPDATE_ALL          // I2C0 Pins
     ,
     PIN_LEVEL_HIGH
     ,
     D_GPIO_MODE_0            |  // Mode 0 to select I2C Mode
     D_GPIO_DIR_OUT            |
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , "i2c0_sda"
    },
    // -----------------------------------------------------------------------
    // I2C1 Configuration  (AP Uplink)
    // -----------------------------------------------------------------------
    {
        12, 12,
        ACTION_UPDATE_ALL          // I2C1 Pins
        ,
        PIN_LEVEL_HIGH
        ,
        D_GPIO_MODE_2            |
        D_GPIO_DIR_OUT            |
        D_GPIO_DATA_INV_OFF      |
        D_GPIO_WAKEUP_OFF
        ,
        D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
        , "i2c1_scl"
    },
    {
        13, 13,
        ACTION_UPDATE_ALL          // I2C1 Pins
        ,
        PIN_LEVEL_HIGH
        ,
        D_GPIO_MODE_2            |
        D_GPIO_DIR_OUT            |
        D_GPIO_DATA_INV_OFF      |
        D_GPIO_WAKEUP_OFF
        ,
        D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
        , "i2c1_sda"
    },
    // -----------------------------------------------------------------------
    // I2C2 Configuration  (General Use)
    // -----------------------------------------------------------------------
    {
        79, 79,
        ACTION_UPDATE_ALL          // I2C2 Pins
        ,
        PIN_LEVEL_HIGH
        ,
        D_GPIO_MODE_2            |
        D_GPIO_DIR_OUT           |
        D_GPIO_DATA_INV_OFF      |
        D_GPIO_WAKEUP_OFF
        ,
        D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
        , "i2c2_scl"
    },
    {
        80, 80,
        ACTION_UPDATE_ALL          // I2C2 Pins
        ,
        PIN_LEVEL_HIGH
        ,
        D_GPIO_MODE_2            |
        D_GPIO_DIR_OUT           |
        D_GPIO_DATA_INV_OFF      |
        D_GPIO_WAKEUP_OFF
        ,
        D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
        , "i2c2_sda"
    },
#ifdef SPI_SLAVE
    // -----------------------------------------------------------------------
    // SPI0 Slave Configuration (Boot SPI/AP Uplink)
    // -----------------------------------------------------------------------
    {74, 74, ACTION_UPDATE_ALL          //
     ,
     PIN_LEVEL_LOW               //
     ,
     D_GPIO_MODE_0            |  //
     D_GPIO_DIR_IN            |  // Drive in
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },

    {75, 75, ACTION_UPDATE_ALL          //
     ,
     PIN_LEVEL_LOW               //
     ,
     D_GPIO_MODE_0            |  //
     D_GPIO_DIR_OUT           |  // Drive out
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
    {76, 77, ACTION_UPDATE_ALL          //
     ,
     PIN_LEVEL_HIGH              //
     ,
     D_GPIO_MODE_0            |  //
     D_GPIO_DIR_IN            |  // Drive in
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
#else
    // SPI0 Configuration (Boot SPI/AP Uplink)
    // -----------------------------------------------------------------------
    {74, 77, ACTION_UPDATE_ALL          //
     ,
     PIN_LEVEL_LOW               //
     ,
     D_GPIO_MODE_0            |  //
     D_GPIO_DIR_OUT           |  // Drive out
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
#endif

    // -----------------------------------------------------------------------
    // LCD Configuration
    //
    // -----------------------------------------------------------------------
    {30,30 , ACTION_UPDATE_ALL           // LCD_VSYNC
     ,
     PIN_LEVEL_LOW               // Default to driving low
     ,
     D_GPIO_MODE_0            |  // Mode 0 to select LCD
     D_GPIO_DIR_OUT           |  // WARNING: NOT PER THE ORGINAL
     D_GPIO_DATA_INV_ON
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
    {29,29 , ACTION_UPDATE_ALL           // LCD_PCLK
     ,
     PIN_LEVEL_LOW               // Default to driving low
     ,
     D_GPIO_MODE_0            |  // Mode 0 to select LCD  (Note: It will be reconfigured by ext PLL driver)
     D_GPIO_DIR_IN |
     D_GPIO_DATA_INV_ON
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
    {31,31 , ACTION_UPDATE_ALL           // LCD_HSYNC
     ,
     PIN_LEVEL_LOW               // Default to driving low
     ,
     D_GPIO_MODE_0            |  // Mode 0 to select LCD
     D_GPIO_DIR_OUT           |  // WARNING: NOT PER THE ORGINAL
     D_GPIO_DATA_INV_ON
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
    {32,32 , ACTION_UPDATE_ALL
     ,
     PIN_LEVEL_LOW
     ,
     D_GPIO_MODE_7            |
     D_GPIO_DIR_IN
     ,
     D_GPIO_PAD_DEFAULTS
     , "bmi160_int1"
    },
    {34,34 , ACTION_UPDATE_ALL
     ,
     PIN_LEVEL_LOW
     ,
     D_GPIO_MODE_7            |
     D_GPIO_DIR_IN
     ,
     D_GPIO_PAD_DEFAULTS
     , "bmi160_int2"
    },
    {36,45 , ACTION_UPDATE_ALL           // LCD D0->D9
     ,
     PIN_LEVEL_LOW               // Default to driving low
     ,
     D_GPIO_MODE_0            |  // Mode 0 to select LCD
     D_GPIO_DIR_OUT            | // WARNING: NOT PER THE ORGINAL
     D_GPIO_DATA_INV_OFF
     ,
     D_GPIO_PAD_DEFAULTS      |  // Uses the default PAD configuration
     D_GPIO_PAD_SLEW_FAST        // But with fast slew rate
     , NULL
    },
    {82,84 , ACTION_UPDATE_ALL           // LCD D10 -> D12
     ,
     PIN_LEVEL_LOW               // Default to driving low
     ,
     D_GPIO_MODE_3            |  // Mode 3 to select LCD
     D_GPIO_DIR_OUT            |  // WARNING: NOT PER THE ORGINAL
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF//        |              D_GPIO_IRQ_SRC_NONE
     ,
     D_GPIO_PAD_DEFAULTS      |  // Uses the default PAD configuration
     D_GPIO_PAD_SLEW_FAST        // But with fast slew rate
     , NULL
    },
    {49,51 , ACTION_UPDATE_ALL           // LCD D13-> D15
     ,
     PIN_LEVEL_LOW               // Default to driving low
     ,
     D_GPIO_MODE_0            |  // Mode 0 to select LCD
     D_GPIO_DIR_OUT            | // WARNING: NOT PER THE ORGINAL
     D_GPIO_DATA_INV_OFF
     ,
     D_GPIO_PAD_DEFAULTS      |  // Uses the default PAD configuration
     D_GPIO_PAD_SLEW_FAST        // But with fast slew rate
     , NULL
    },
    // cdcel ENABLE
    {58, 58 , ACTION_UPDATE_ALL           // cdcel ENABLE
     ,
     PIN_LEVEL_HIGH
     ,
     D_GPIO_MODE_7            |  // Mode 7 to select GPIO
     D_GPIO_DIR_OUT           |  // Drive out
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },

    {15, 15 , ACTION_UPDATE_ALL           // cdcel ENABLE
     ,
     PIN_LEVEL_LOW
     ,
     D_GPIO_MODE_7            |  // Mode 7 to select GPIO
     D_GPIO_DIR_OUT           |  // Drive out
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , "ov7750_bus0_reset_right"
    },
    {33, 33 , ACTION_UPDATE_ALL           // cdcel ENABLE
     ,
     PIN_LEVEL_LOW
     ,
     D_GPIO_MODE_7            |  // Mode 7 to select GPIO
     D_GPIO_DIR_OUT           |  // Drive out
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , "ov7750_bus0_reset_left"
    },

    // -----------------------------------------------------------------------
    // Finally we terminate the Array
    // -----------------------------------------------------------------------
    {0,0   , ACTION_TERMINATE_ARRAY      // Do nothing but simply termintate the Array
     ,
     PIN_LEVEL_LOW               // Won't actually be updated
     ,
     D_GPIO_MODE_0               // Won't actually be updated
     ,
     D_GPIO_PAD_DEFAULTS         // Won't actually be updated
     , NULL
    }
};
