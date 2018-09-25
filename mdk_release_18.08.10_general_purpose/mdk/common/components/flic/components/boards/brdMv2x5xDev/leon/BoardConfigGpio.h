///
/// @file      BoardConfigGpio.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Default GPIO configuration for the MV0182/MV212 Ma2x5x Development Board.
///            Using the structure defined by this board it is possible to initialize
///            some of the GPIOS on the MV0182/MV0212 PCB to good safe initial defaults.
///

#ifndef __BOARD_CONFIG_GPIO_H__
#define __BOARD_CONFIG_GPIO_H__

#include <DrvGpio.h>

// 1: Includes
// ----------------------------------------------------------------------------
// 2: Defines
// ----------------------------------------------------------------------------
/// GPIO
#define MVDEV_CAMA_RST_GPIO   (56) //59
#define MVDEV_CAMB_RST_GPIO   (59) //56
#define MVDEV_CAMC_RST_GPIO   (27) //15
// 3: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

// 4: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

const drvGpioInitArrayType brdMVGpioCfgDefault =
{
// -----------------------------------------------------------------------
    // PCB Revision detect, set weak pullups on the necessary pins
    // -----------------------------------------------------------------------
    {
        9,  9,
        ACTION_UPDATE_PAD         // Only updating the PAD configuration
        ,
        PIN_LEVEL_LOW
        ,
        D_GPIO_MODE_0
        ,
        D_GPIO_PAD_PULL_DOWN     | // Enable weak pullups so that we can detect revision
        D_GPIO_PAD_DRIVE_2mA     |
        D_GPIO_PAD_SLEW_SLOW     |
        D_GPIO_PAD_SCHMITT_OFF   |
        D_GPIO_PAD_RECEIVER_ON   |
        D_GPIO_PAD_LOCALCTRL_OFF |
        D_GPIO_PAD_LOCALDATA_LO  |
        D_GPIO_PAD_LOCAL_PIN_IN
        , NULL
    },

    // -----------------------------------------------------------------------
    // InfraRed Sensor Pin
    // -----------------------------------------------------------------------

    {
        14, 14,
        ACTION_UPDATE_ALL           // Infrared Sensor Pin
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
    {
        60, 61,
        ACTION_UPDATE_ALL          // I2C0 Pins
        ,
        PIN_LEVEL_HIGH
        ,
        D_GPIO_MODE_0            |  // Mode 0 to select I2C Mode
        D_GPIO_DIR_OUT            |
        D_GPIO_DATA_INV_OFF      |
        D_GPIO_WAKEUP_OFF
        ,
        D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
        , NULL
    },
    // -----------------------------------------------------------------------
    // I2C1 Configuration  (AP Uplink)
    // -----------------------------------------------------------------------
    {
        12, 13,
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
        , NULL
    },
    // -----------------------------------------------------------------------
    // I2C2 Configuration  (General Use)
    // -----------------------------------------------------------------------
    {
        79, 80,
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
        , NULL
    },
#ifdef SPI_SLAVE
    // -----------------------------------------------------------------------
    // SPI0 Slave Configuration (Boot SPI/AP Uplink)
    // -----------------------------------------------------------------------
    {
        74, 74,
        ACTION_UPDATE_ALL          //
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

    {
        75, 75,
        ACTION_UPDATE_ALL          //
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
    {
        76, 77,
        ACTION_UPDATE_ALL          //
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
    {
        74, 77,
        ACTION_UPDATE_ALL          //
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
    {
        30, 30,
        ACTION_UPDATE_ALL           // LCD_VSYNC
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
    {
        29, 29,
        ACTION_UPDATE_ALL           // LCD_PCLK
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
    {
        31, 31,
        ACTION_UPDATE_ALL           // LCD_HSYNC
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
    {32,32 , ACTION_UPDATE_ALL           // LCD_DATAEN
     ,
     PIN_LEVEL_LOW               // Default to driving low
     ,
     D_GPIO_MODE_0            |  // Mode 0 to select LCD
     D_GPIO_DIR_OUT
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
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
    // HDMI RESET
    {58, 58 , ACTION_UPDATE_ALL           // HDMI_RESET_N GPIO
     ,
     PIN_LEVEL_HIGH              // Default to holding in reset for now
     ,
     D_GPIO_MODE_7            |  // Mode 7 to select GPIO
     D_GPIO_DIR_OUT           |  // Drive out
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },

    // cam A reset gpio
    {MVDEV_CAMA_RST_GPIO, MVDEV_CAMA_RST_GPIO , ACTION_UPDATE_ALL        // CAM_A_REST_GPIO
     ,
     PIN_LEVEL_HIGH              // Default to holding in reset for now
     ,
     D_GPIO_MODE_7            |  // Mode 7 to select GPIO
     D_GPIO_DIR_OUT           |  // Drive out
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , "CamAReset"
    },
    // cam B Left reset gpio
    {MVDEV_CAMB_RST_GPIO, MVDEV_CAMB_RST_GPIO , ACTION_UPDATE_ALL        // CAB_B_LEFT_REST_GPIO
     ,
     PIN_LEVEL_HIGH              // Default to holding in reset for now
     ,
     D_GPIO_MODE_7            |  // Mode 7 to select GPIO
     D_GPIO_DIR_OUT           |  // Drive out
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , "CamBLeftReset"
    },
    // cam B Right reset gpio
    {MVDEV_CAMC_RST_GPIO, MVDEV_CAMC_RST_GPIO , ACTION_UPDATE_ALL        // CAB_B_Right_REST_GPIO
     ,
     PIN_LEVEL_HIGH              // Default to holding in reset for now
     ,
     D_GPIO_MODE_7            |  // Mode 7 to select GPIO
     D_GPIO_DIR_OUT           |  // Drive out
     D_GPIO_DATA_INV_OFF      |
     D_GPIO_WAKEUP_OFF
     ,
     D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , "CamBRightReset"
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

#endif //__BOARD_CONFIG_GPIO_H__
