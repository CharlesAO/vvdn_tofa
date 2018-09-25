///  
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved 
///            For License Warranty see: common/license.txt              
///
/// @brief     Definitions and types needed by the MV0182 Board Driver API
/// 
/// This header contains all necessary hardware defined constants for this board
/// e.g. GPIO assignments, I2C addresses
/// 

#ifndef BRD_MV0182_DEF_H
#define BRD_MV0182_DEF_H

// 1: Defines
// ----------------------------------------------------------------------------

#define MV0182_OSC_IN_FREQ_KHZ         12000

// ----------------------------------------------------------------------------
// I2C
// ----------------------------------------------------------------------------

// I2C0 -- Master
#define MV0182_I2C0_SCL_PIN            (60)
#define MV0182_I2C0_SDA_PIN            (61)
#define MV0182_I2C0_SPEED_KHZ_DEFAULT  (100)
#define MV0182_I2C0_ADDR_SIZE_DEFAULT  (ADDR_7BIT)

// I2C1 -- Master
#define MV0182_I2C1_SCL_PIN            (12)
#define MV0182_I2C1_SDA_PIN            (13)
#define MV0182_I2C1_SPEED_KHZ_DEFAULT  (100)
#define MV0182_I2C1_ADDR_SIZE_DEFAULT  (ADDR_7BIT)

// I2C2 -- Master
#define MV0182_I2C2_SCL_PIN            (79)
#define MV0182_I2C2_SDA_PIN            (80)
#define MV0182_I2C2_SPEED_KHZ_DEFAULT  (100)
#define MV0182_I2C2_ADDR_SIZE_DEFAULT  (ADDR_7BIT)



// ----------------------------------------------------------------------------
// General GPIO definition
// ----------------------------------------------------------------------------

#define MV0182_WM8325_I2C_ADDR_7BIT    (0x36) 

// GPIO:  Inputs


// GPIO:  Outputs
#define MV0182_PIN_CAM_A_GPIO0_N           (59)
#define MV0182_PIN_CAM_B_GPIO0_N           (15)
#define MV0182_PIN_COM_IO5_N               (56)
#define MV0182_PIN_AP_IRQ                  (22)

// PCB Revision Detection
#define MV0182_REV_DETECT  (9)


//predefined MIPI media devices for mv182 board
#define CAM_A1_MIPICTRL   MIPI_CTRL_0
#define CAM_B1_MIPICTRL   MIPI_CTRL_2
#define CAM_B2_MIPICTRL   MIPI_CTRL_3

//predefined cam receiver devices for mv182 board : commented out, as it introduce dependencies between Board182 component and CamGeneric component;
//still let here as example
//#define CAM_A_CIF_RECEIVER    CIF_DEVICE0
//#define CAM_A_SIPP_RECEIVER   SIPP_DEVICE0
//#define CAM_B1_SIPP_RECEIVER  SIPP_DEVICE1
//#define CAM_B2_CIF_RECEIVER   CIF_DEVICE1

//predefined I2 addressing slots, for paired cameras on daughterboard - mv182
#define CAM_B1_RIGHT_ADDR 0
#define CAM_B2_LEFT_ADDR  1
#define CAM_A_ADDR        0

//predefined camera sensors reset pins for every couple mv182 board - sensor daughter board
#define MV0182_MV0201_SENSOR_RST_GPIO        MV0182_PIN_CAM_A_GPIO0_N
#define MV0182_MV0200_SENSOR_LEFT_RST_GPIO   MV0182_PIN_COM_IO5_N
#define MV0182_MV0200_SENSOR_RIGHT_RST_GPIO  MV0182_PIN_CAM_B_GPIO0_N
//#define MV0182_MV0200_SENSOR_RIGHT_RST_GPIO  (0xFF)  //set to 0xFF (dummy) for mv200 rev.0, in order that the second camera init (init of the right sensor) not to reset the 1st sensor (left)

//predefined GPIO's used for camera VSYNC/HSYNC generation on mv182 board
#define CAMERA_VSYNC_GPIO        0xFF  //none, not allowed
#define CAMERA_HSYNC_GPIO        0xFF  //none, not allowed
#define CAMERA_VSYNC_GPIO_MODE   0xFF  //none, not allowed
#define CAMERA_HSYNC_GPIO_MODE   0xFF  //none, not allowed

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

typedef enum
{
    MV0182_REV_NOT_INIT = 0,    // Initial state of the revision so we know it needs to be detected
    MV0182_R0R1         = 1,    // MV0182-R0 and R1 PCB revisions are electrically and software compatible so they return the same rev.
    MV0182_R2           = 2,
    MV0182_R3           = 3,
    MV0182_R4           = 4,
    MV0182_R5           = 5
}tyMv0182PcbRevision;

typedef enum 
{
    BRD_LED1=1,         // MV0182 LED1 (Part Number: D11)
    BRD_LED2=2          // MV0182 LED2 (Part Number: D12) 
} tyBrdLedId;

typedef enum {
    LED_ON,
    LED_OFF
} tyLedState;

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

#endif // BRD_MV0182_DEF_H

