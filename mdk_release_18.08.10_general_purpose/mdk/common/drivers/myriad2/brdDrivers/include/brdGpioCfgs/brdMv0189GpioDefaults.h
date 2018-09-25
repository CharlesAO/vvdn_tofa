///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Default GPIO configuration for the MV0189 Board
///
/// Using the structure defined by this board it is possible to initialise
/// some of the GPIOS on the MV0189 PCB to good safe initial defauls (to be updated
/// with other GPIOS)
///
#ifndef BRD_MV0189_GPIO_DEFAULTS_H_
#define BRD_MV0189_GPIO_DEFAULTS_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvGpioDefines.h>

// 2: Defines
// ----------------------------------------------------------------------------

// 3: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

// 4: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

const drvGpioInitArrayType brdMv0189GpioCfgDefault =
{
    // -----------------------------------------------------------------------
    // ENABLE I2C PULL UPS
    // -----------------------------------------------------------------------
    {22, 22 , ACTION_UPDATE_ALL
            ,
              PIN_LEVEL_HIGH
            ,
              D_GPIO_MODE_7            |  // Mode 0 to select I2C Mode
              D_GPIO_DIR_OUT            |
              D_GPIO_DATA_INV_OFF       |
              D_GPIO_WAKEUP_OFF
            ,
             D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
    // -----------------------------------------------------------------------
    // I2C0 Configuration
    // -----------------------------------------------------------------------
    {20, 21 , ACTION_UPDATE_ALL          // I2C0 Pins
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_4            |  // Mode 4 to select I2C Mode
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
    // -----------------------------------------------------------------------
    // WARNING: THIS HAS NO EFFECT SINCE SPI RECONFIGURES GPIO62 as SPI SCK.
    // The application is responsible to configure this GPIO as I2C1 SCL (mode0)
    // I2C1 Configuration
    // SCL
    // -----------------------------------------------------------------------
    {62, 62 , ACTION_UPDATE_ALL
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_0            |  // Mode 0 to select I2C Mode
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF       |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
    // -----------------------------------------------------------------------
    // SDA
    // -----------------------------------------------------------------------
    {69, 69 , ACTION_UPDATE_ALL
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_0            |  // Mode 0 to select I2C Mode
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF       |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
    // -----------------------------------------------------------------------
    // SPI2 Interface used as master
    // -----------------------------------------------------------------------
    {60, 63 , ACTION_UPDATE_ALL          //
            ,
              PIN_LEVEL_LOW               //
            ,
              D_GPIO_MODE_3            |  // Mode3 SPI2)
              D_GPIO_DIR_OUT           |  // Drive out
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
    // -----------------------------------------------------------------------
    // SPI0 Interface used as slave
    // SS pin should be input
    // -----------------------------------------------------------------------
    {16, 19 , ACTION_UPDATE_ALL          //
            ,
              PIN_LEVEL_LOW               //
            ,
              D_GPIO_MODE_4            |  // Mode4 SPI0)
              D_GPIO_DIR_IN            |  // Drive out
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
    // -----------------------------------------------------------------------
    // I2S0 Interface used as master
    // SCLK and WS
    // -----------------------------------------------------------------------
    {23, 24 , ACTION_UPDATE_ALL          //
            ,
              PIN_LEVEL_LOW               //
            ,
              D_GPIO_MODE_4            |  // Mode4 SPI0)
              D_GPIO_DIR_OUT           |  // Drive out
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
    // -----------------------------------------------------------------------
    // I2S0 Interface used as master
    // SDO0
    // -----------------------------------------------------------------------
    {26, 26 , ACTION_UPDATE_ALL          //
            ,
              PIN_LEVEL_LOW               //
            ,
              D_GPIO_MODE_4            |
              D_GPIO_DIR_OUT           |  // Drive out
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
    // -----------------------------------------------------------------------
    // I2S0 Interface used as master
    // SDO1, SDO2
    // -----------------------------------------------------------------------
    {71, 72 , ACTION_UPDATE_ALL          //
            ,
              PIN_LEVEL_LOW               //
            ,
              D_GPIO_MODE_0            |
              D_GPIO_DIR_OUT           |  // Drive out
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },

    // -----------------------------------------------------------------------
    // I2S2 Interface used as slave
    // -----------------------------------------------------------------------
    {47, 51 , ACTION_UPDATE_ALL          //
            ,
              PIN_LEVEL_LOW               //
            ,
              D_GPIO_MODE_2            |
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },

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

#endif // BRD_MV0189_GPIO_DEFAULTS_H_
