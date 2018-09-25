///  
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved 
///            For License Warranty see: common/license.txt              
///
/// @brief     Definitions and types needed by the MV0198 Board Driver API
/// 
/// This header contains all necessary hardware defined constants for this board
/// 

#ifndef BRD_MV0198_DEF_H
#define BRD_MV0198_DEF_H

#include "mv_types.h"
#include "DrvMcp3424.h"
#include "DrvI2cMaster.h"
#include "DrvCommon.h"

// 1: Defines
// ----------------------------------------------------------------------------

#define MAX_RAIL_STRING_LEN  (20)
#define DEFAULT_NUM_RAILS (16)

#define MV0198_MCP_DEV_A_7ADDR    (0x6E)
#define MV0198_MCP_DEV_B_7ADDR    (0x6D)
#define MV0198_MCP_DEV_C_7ADDR    (0x6A)
#define MV0198_MCP_DEV_D_7ADDR    (0x6B)

// The following definitions describe the multipler which must be used to 
// convert the number of millivolts reported by the ADC to a number of mA (or mV)
// that corresponds to the measured current (or Voltage) on the target rail
// The values are taken from the MV0182 Schematics
// For example the schematics state that the board ouputs 0.6mV per mA of VDDCV current
// This driver will then take the VDDCV_I_MUL which is the reciprocal of this value and multiply
// it by the reported ADC voltage 
// resultMa = (resultNv * UNIT_NANO_TO_MILI * VDDCV_I_MUL)
#define VDDCV_I_MUL               (1 /    0.6)
#define VDDCC_I_MUL               (1 /    2.0)  // MA2100_DEPRECATED - 
                                                //  should be later on removed
#define VDDIO_B_I_MUL             (1 /   20.0)  // MA2150
#define VDDCR_I_MUL               (1 /   20.0)
#define VDDIO_I_MUL               (1 /   10.0)
#define MIP_VDD_I_MUL             (1 /   20.0)
#define PLL_AVDD_I_MUL            (1 /   20.0)
#define DRAM_MVDDQ_I_MUL          (1 /    2.0)
#define DRAM_MVDDA_I_MUL          (1 /   20.0)
#define DRAM_VDD1_I_MUL           (1 /   20.0)
#define DRAM_VDD2_I_MUL           (1 /   10.0)
#define DRAM_VDDQ_I_MUL           (1 /   20.0)
#define USB_VDD330_I_MUL          (1 /   20.0)
#define USB_VP_VDD_I_MUL          (1 /   20.0)
#define VDDCV_V_MUL               (1 /    1.0)  // Voltage rail, Already in mV units
#define MIPI_VDD_V_MUL            (1 /    1.0)  // Voltage rail, Already in mV units

// for all channels, we have an associated signal which is
// calculated: 100 + channel number. Channels differ
// for MA2100 and MA2x5x, so we have two signals on the same channel.
// CHANNEL_TO_SIGNAL define, makes that channel 1 is associated to signal
// 115 for MA2x5x or it is associated to signal 101 for MA2100
#define CHANNEL_TO_SIGNAL(chan) ((chan <= VDDCC_I_MA) ? (LOG_EVENT_198_RAIL_BASE + chan) : \
                                 ((chan == VDDIO_B_I_MA) ? (LOG_EVENT_198_RAIL_VDDIO_B_I_MUL_I_MA_MA2150) : \
                                 (LOG_EVENT_198_RAIL_BASE + chan - 1)))

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

typedef enum
{
    VDDCV_I_MA,
    VDDCC_I_MA,     // MA2100_DEPRECATED - should be later on removed
    VDDIO_B_I_MA,   // MA2150
    VDDCR_I_MA,  
    VDDIO_I_MA,  
    MIPI_VDD_I_MA,
    PLL_AVDD_I_MA,
    DRAM_MVDDQ_I_MA,
    DRAM_MVDDA_I_MA,
    DRAM_VDD1_I_MA,
    DRAM_VDD2_I_MA,
    DRAM_VDDQ_I_MA,
    USB_VDD330_I_MA,
    USB_VP_VDD_I_MA,
    VDDCV_V_MV,
    MIPI_VDD_V_MV,
} tyChannelId;

typedef struct {
    u32    railId;
    char   railName[MAX_RAIL_STRING_LEN];
} tyRailLookup;

typedef enum {
    DRV_BRD198_DRV_SUCCESS             = MYR_DRV_SUCCESS,
    DRV_BRD198_DRV_ERROR               = MYR_DRV_ERROR,    
    DRV_BRD198_DRV_NOT_INITIALIZED     = MYR_DRV_NOT_INITIALIZED,
    DRV_BRD198_DRV_ALREADY_INITIALIZED = MYR_DRV_ALREADY_INITIALIZED,
    DRV_BRD198_INVALID_RAIL            = MYR_DRV_CUSTOM_CODE_START_OFFSET, 
    DRV_BRD198_UNSUPPORTED_ADCCONFG,
    DRV_BRD198_INVALID_CHAN_CONFIG,
    DRV_BRD198_INVALID_CONT_MODE_NOT_SELECTED,
} tyBrt198ErrorCode;


typedef enum 
{
    ADC_A_NOT_USED          = 0,
    ADC_A_VDDCR_I_MA        = 1,
    ADC_A_VDDCV_I_MA        = 2,
    ADC_A_DRAM_VDD1_I_MA    = 3,
    ADC_A_MIPI_VDD_I_MA     = 4,
} tyAdcAChannel;

typedef enum 
{
    ADC_B_NOT_USED          = 0,
    ADC_B_DRAM_VDD2_I_MA    = 1,
    ADC_B_DRAM_VDDQ_I_MA    = 2,
    ADC_B_DRAM_MVDDQ_I_MA   = 3,
    ADC_B_DRAM_MVDDA_I_MA   = 4,
} tyAdcBChannel;

typedef enum 
{
    ADC_C_NOT_USED          = 0,
    ADC_C_USB_VDD330_I_MA   = 1,
    ADC_C_USB_VP_VDD_I_MA   = 2,
    ADC_C_VDDIO_I_MA        = 3,
    ADC_C_VDD_CC_IOB_MA     = 4
} tyAdcCChannel;

typedef enum 
{
    ADC_D_NOT_USED          = 0,
    ADC_D_RESERVED          = 1,
    ADC_D_PLL_AVDD_I_MA     = 2,
    ADC_D_VDDCV_V_MV        = 3,
    ADC_D_MIPI_VDD_V_MV     = 4,
} tyAdcDChannel;

typedef struct
{
    float           adcResults[DEFAULT_NUM_RAILS];
    float           totalMilliAmps;
    float           totalMilliWatts;
} tyAdcResultAllRails;

typedef struct
{
    tyAdcAChannel      aChannel;    
    tyAdcBChannel      bChannel;
    tyAdcCChannel      cChannel;
    tyAdcDChannel      dChannel;
} tyAdcContinuousConfig;

typedef struct
{
    float           adcValue;
    int             bNewSample;
    tyChannelId     chanId;
}tyAdcResult;

typedef struct
{
    tyAdcResult   adcAResult;
    tyAdcResult   adcBResult;
    tyAdcResult   adcCResult;
    tyAdcResult   adcDResult;
} tyAdcContinuousResult;


typedef enum
{
    VOLTAGE_MEASUREMENT,
    CURRENT_MEASUREMENT
} tyChannelType;

typedef struct 
{
    tyMcpDeviceId   mcpDeviceId;
    tyMcpChannel    mcpChannel;
    double          channelMultiplier;
    double          railNominalV;
    tyMcp3424Config *optMcpChanCfg;          // Optional ADC configuration Override
} tyAdcChannelConfig;

typedef struct 
{
    I2CM_Device             *i2cDev;
    tyMcp3424Handle         mcpHandle[4];
    tyAdcChannelConfig      *adcConfig;
    u32                     numAdcChannels;
    tyAdcContinuousConfig   continuousCfg;
}tyBrd198Handle;

typedef char tyChannelName[MAX_RAIL_STRING_LEN];

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

#endif // BRD_MV0198_DEF_H


