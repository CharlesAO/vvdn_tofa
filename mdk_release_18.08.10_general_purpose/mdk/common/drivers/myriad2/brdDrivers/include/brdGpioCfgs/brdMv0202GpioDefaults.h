///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Default GPIO configuration for the MV0202 Board
///
/// Using the structure defined by this board it is possible to initialize
/// some of the GPIOS on the MV0202 PCB to good safe initial defaults
///
#ifndef BRD_MV0202_GPIO_DEFAULTS_H_
#define BRD_MV0202_GPIO_DEFAULTS_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvGpioDefines.h>
// 2: Defines
// ----------------------------------------------------------------------------

// 3: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

// 4: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

const drvGpioInitArrayType brdMV0202GpioCfgDefault =
{
    // -----------------------------------------------------------------------
    // I2C0 Configuration  (I2C Slave from AP)
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
    // I2C1 Configuration  (I2C Master for camera control)
    // -----------------------------------------------------------------------
    {12, 13 , ACTION_UPDATE_ALL          // I2C1 Pins
            ,
              PIN_LEVEL_LOW
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
    // SPI0 Configuration (Boot and AP messaging)
    // -----------------------------------------------------------------------
    {74, 74 , ACTION_UPDATE_ALL          //
            ,
              PIN_LEVEL_LOW               //
            ,
              D_GPIO_MODE_0            |  //
              D_GPIO_DIR_IN           |  // Drive out
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
     , NULL
    },
	{75, 75 , ACTION_UPDATE_ALL          //
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
	{76, 76 , ACTION_UPDATE_ALL          //
            ,
              PIN_LEVEL_LOW               //
            ,
              D_GPIO_MODE_0            |  //
              D_GPIO_DIR_IN           |  // Drive out
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
         , NULL
    },
	{77, 77 , ACTION_UPDATE_ALL          //
            ,
              PIN_LEVEL_HIGH               //
            ,
              D_GPIO_MODE_0            |  //
              D_GPIO_DIR_IN            |  // Drive out (SPI slave select requires this)
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
         , NULL
    },
    // -----------------------------------------------------------------------
    // CAM Master Clocks
    //
    // -----------------------------------------------------------------------
    { 41,41, ACTION_UPDATE_ALL           // MCLK for IMX214
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_4            |  // Mode 4 --> cpr_io_out_clk_3
              D_GPIO_DIR_OUT           |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    { 57,57, ACTION_UPDATE_ALL           // MCLK for both IMX208s
            ,
              PIN_LEVEL_LOW
            ,
              D_GPIO_MODE_2            |  // Mode 2 --> cpr_io_out_clk_1
              D_GPIO_DIR_OUT           |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
      , NULL
    },
    // -----------------------------------------------------------------------
    //
    // GPIOs used as Inputs
    //
    // gpio_1	7	I	CAM_MOV_SYNC
    // gpio_50	7	I	RFC2MV_STROBE
    // gpio_62	7	I	CAM_RFC_STROBE
    // gpio_63	7	I	CAM_MOV_IR_STROBE
    // gpio_72	7	I	MV_XVS_FSYNC
    // -----------------------------------------------------------------------
    { 1, 1, ACTION_UPDATE_ALL           // CAM_MOV_SYNC
            ,
              PIN_LEVEL_LOW             // not relevant for input
            ,
              D_GPIO_MODE_7            |
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS
      , NULL
    },
    {50,50, ACTION_UPDATE_ALL           // RFC2MV_STROBE
            ,
              PIN_LEVEL_LOW             // not relevant for input
            ,
              D_GPIO_MODE_7            |
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS
     , NULL
    },
    {62,62, ACTION_UPDATE_ALL           // CAM_RFC_STROBE
            ,
              PIN_LEVEL_LOW             // not relevant for input
            ,
              D_GPIO_MODE_7            |
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS
     , NULL
    },
    {63,63, ACTION_UPDATE_ALL           // CAM_MOV_IR_STROBE
            ,
              PIN_LEVEL_LOW             // not relevant for input
            ,
              D_GPIO_MODE_7            |
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS
     , NULL
    },
    {72,72, ACTION_UPDATE_ALL           // MV_XVS_FSYNC
            ,
              PIN_LEVEL_LOW             // not relevant for input
            ,
              D_GPIO_MODE_7            |
              D_GPIO_DIR_IN            |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS
     , NULL
    },
    // -----------------------------------------------------------------------
    //
    // GPIOs used as Outputs
    //
    // gpio_5	7	O	MV2RRA_RST_N
    // gpio_19	7	O	MV2RFC_EXT_AVDD_LDO_EN
    // gpio_29	7	O	MV202_TP1
    // gpio_40	7	O	MV2RFC_RST_N
    // gpio_42	7	O	MV2RFC_EEPROM_WP
    // gpio_59	7	O	CAM_MOV_INT
    // gpio_67	7	O	MV2RA_EEPROM_WP
    // gpio_71	7	O	MV2LRA_RST_N
    // -----------------------------------------------------------------------
    { 5, 5, ACTION_UPDATE_ALL           // MV2RRA_RST_N
            ,
              PIN_LEVEL_LOW             // Drive Low initially  (camera reset)
            ,
              D_GPIO_MODE_7            |   // direct drive mode
              D_GPIO_DIR_OUT           |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS
      , NULL
    },
    {19,19, ACTION_UPDATE_ALL           // MV2RFC_EXT_AVDD_LDO_EN
            ,
              PIN_LEVEL_LOW             // Drive Low initially (i.e. LDO is disabled)
            ,
              D_GPIO_MODE_7            |   // direct drive mode
              D_GPIO_DIR_OUT           |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS
     , NULL
    },
    {29,29, ACTION_UPDATE_ALL           // MV202_TP1
            ,
              PIN_LEVEL_HIGH             // Drive High (can use to validate booted etc..)
            ,
              D_GPIO_MODE_7            |   // direct drive mode
              D_GPIO_DIR_OUT           |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS
     , NULL
    },
    {40,40, ACTION_UPDATE_ALL           // MV2RFC_RST_N
            ,
              PIN_LEVEL_LOW             // Drive Low initially  (camera reset)
            ,
              D_GPIO_MODE_7            |   // direct drive mode
              D_GPIO_DIR_OUT           |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS
     , NULL
    },
    {42,42, ACTION_UPDATE_ALL           // MV2RFC_EEPROM_WP
            ,
              PIN_LEVEL_HIGH           // Drive High so EEPROM write protected by default
            ,
              D_GPIO_MODE_7            |   // direct drive mode
              D_GPIO_DIR_OUT           |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS
     , NULL
    },
    {59,59, ACTION_UPDATE_ALL           // CAM_MOV_INT
            ,
              PIN_LEVEL_HIGH            // Drive High, we drive LOW only when SPI is ready (same as SS levels)
            ,
              D_GPIO_MODE_7            |   // direct drive mode
              D_GPIO_DIR_OUT           |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS
     , NULL
    },
    {67,67, ACTION_UPDATE_ALL          // MV2RA_EEPROM_WP
            ,
              PIN_LEVEL_HIGH           // Drive High so EEPROM write protected by default
            ,
              D_GPIO_MODE_7            |   // direct drive mode
              D_GPIO_DIR_OUT           |
              D_GPIO_DATA_INV_OFF      |
              D_GPIO_WAKEUP_OFF
            ,
              D_GPIO_PAD_DEFAULTS
     , NULL
    },
    {71,71, ACTION_UPDATE_ALL           // MV2LRA_RST_N
                ,
                  PIN_LEVEL_LOW             // Drive Low initially (camera reset)
                ,
                  D_GPIO_MODE_7            |   // direct drive mode
                  D_GPIO_DIR_OUT           |
                  D_GPIO_DATA_INV_OFF      |
                  D_GPIO_WAKEUP_OFF
                ,
                  D_GPIO_PAD_DEFAULTS
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

#endif // BRD_MV0202_GPIO_DEFAULTS_H_
