///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Default GPIO configuration for the MV0184 Board
///
/// Using the structure defined by this board it is possible to initialise
/// some of the GPIOS on the MV0184 PCB to good safe initial defauls (to be updated
/// with other GPIOS)
///
#ifndef BRD_MV0184_GPIO_DEFAULTS_H_
#define BRD_MV0184_GPIO_DEFAULTS_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvGpioDefines.h>

// 2: Defines
// ----------------------------------------------------------------------------

// 3: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

// 4: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

const drvGpioInitArrayType brdMv0184GpioCfgDefault =
{
    // -----------------------------------------------------------------------
    // I2C0 Configuration
    // -----------------------------------------------------------------------
    {60, 61 , ACTION_UPDATE_ALL          // I2C0 Pins
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_0            |  // Mode 0 to select I2C Mode
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
            , NULL
    },
    // -----------------------------------------------------------------------
    // I2C1 Configuration
    // -----------------------------------------------------------------------
    {62, 63 , ACTION_UPDATE_ALL          // I2C1 Pins
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_0            |  // Mode 0 to select I2C Mode
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
            , NULL
    },
    // -----------------------------------------------------------------------
    // SPI0 Configuration
    // -----------------------------------------------------------------------
    {74, 77 , ACTION_UPDATE_ALL          //
            ,
              PIN_LEVEL_LOW               //
            ,
              D_GPIO_MODE_0            |  // Mode3 SPI2)
              D_GPIO_DIR_OUT           |  // Drive out
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
            , NULL
    },
    // -----------------------------------------------------------------------
    // CAM Configuration
    //
    // -----------------------------------------------------------------------
    { 0,0, ACTION_UPDATE_ALL           // CAM_MCLK
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_0            |  // Mode 0 to select CAM
              D_GPIO_DIR_OUT           |  // Output selected, not really necessary
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    { 1, 1, ACTION_UPDATE_ALL           // CAM_PCLK
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_0            |  // Mode 0 to select CAM
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    { 2, 2, ACTION_UPDATE_ALL           // CAM_VSYNC
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_0            |  // Mode 0 to select CAM
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_ON       |  // Pin is inverted
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    { 3, 3, ACTION_UPDATE_ALL           // CAM_HSYNC
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_0            |  // Mode 0 to select CAM
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    { 4,15, ACTION_UPDATE_ALL           // CAM Data 0-11
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_0            |  // Mode 0 to select CAM1
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    { 27,27  , ACTION_UPDATE_ALL           // CAM Reset Control
            ,
              PIN_LEVEL_HIGH              // Default to not holding Camera in reset
            ,
              D_GPIO_MODE_7            |  // Mode 7 to select GPIO
              D_GPIO_DIR_OUT           |  // Driven Out
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    { 47 ,47 , ACTION_UPDATE_ALL           // CAM Powerdown
            ,
              PIN_LEVEL_LOW               // Drive Pin Low to make sure camera not in power down by default
            ,
              D_GPIO_MODE_7            |  // Mode 7 to select GPIO
              D_GPIO_DIR_OUT           |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    { 48 ,48 , ACTION_UPDATE_ALL           // CAM Flash(strobe);
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_7            |  // Mode 7 to select GPIO
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    { 57 ,57 , ACTION_UPDATE_ALL           // Cam shutter (manual mode) set as input for now as now used
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_7            |  // Mode 7 to select GPIO
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    // -----------------------------------------------------------------------
    // UART Configuration (Please check the conflict with CAM DAT11-GPIO15)
    // By default, GPIO15 will be used as CAM DAT11.
    // -----------------------------------------------------------------------
/*
    { 12, 15 , ACTION_UPDATE_ALL           // UART
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_1            |  // Mode 1 to select UART
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
              , NULL
    },
*/
    // -----------------------------------------------------------------------
    // LCD Configuration
    //
    // -----------------------------------------------------------------------
    { 65, 65  , ACTION_UPDATE_ALL           // LCD_PCLK
            ,
              PIN_LEVEL_LOW               // Default to driving low
            ,
              D_GPIO_MODE_3            |  // Mode 3 to select LCD
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_ON       |  // Inverted
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    { 66, 66  , ACTION_UPDATE_ALL           // LCD_VSYNC
            ,
              PIN_LEVEL_LOW               // Default to driving low
            ,
              D_GPIO_MODE_3            |  // Mode 3 to select LCD
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_ON       |  // Inverted
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    { 67, 67  , ACTION_UPDATE_ALL           // LCD_HSYNC
            ,
              PIN_LEVEL_LOW               // Default to driving low
            ,
              D_GPIO_MODE_3            |  // Mode 0 to select LCD
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_ON       |  // Inverted
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    { 72, 84  , ACTION_UPDATE_ALL           // LCD D0 -> D12
            ,
              PIN_LEVEL_LOW               // Default to driving low
            ,
              D_GPIO_MODE_3            |  // Mode 3 to select LCD
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS      |  // Uses the default PAD configuration
              D_GPIO_PAD_SLEW_FAST        // But with fast slew rate
      , NULL
    },
    { 49, 51  , ACTION_UPDATE_ALL           // LCD D13 -> D15
            ,
              PIN_LEVEL_LOW               // Default to driving low
            ,
              D_GPIO_MODE_0            |  // Mode 0 to select LCD
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS      |  // Uses the default PAD configuration
              D_GPIO_PAD_SLEW_FAST        // But with fast slew rate
      , NULL
    },
    { 19, 26  , ACTION_UPDATE_ALL           // LCD D16 -> D23
            ,
              PIN_LEVEL_LOW               // Default to driving low
            ,
              D_GPIO_MODE_5            |  // Mode 5 to select LCD
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS      |  // Uses the default PAD configuration
              D_GPIO_PAD_SLEW_FAST        // But with fast slew rate
      , NULL
    },
    { 68, 68  , ACTION_UPDATE_ALL           // LCD_DATAEN
            ,
              PIN_LEVEL_LOW               // Default to driving low
            ,
              D_GPIO_MODE_3            |  // Mode 3 to select LCD
              D_GPIO_DIR_OUT           |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    // TO BE CONTINUED

    // -----------------------------------------------------------------------
    // Finally we terminate the Array
    // -----------------------------------------------------------------------
    {0,0    , ACTION_TERMINATE_ARRAY      // Do nothing but simply termintate the Array
            ,
              PIN_LEVEL_LOW               // Won't actually be updated
            ,
              D_GPIO_MODE_0               // Won't actually be updated
            ,
              D_GPIO_PAD_DEFAULTS         // Won't actually be updated
     , NULL
    }
};

#endif // BRD_MV0184_GPIO_DEFAULTS_H_
