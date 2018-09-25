///  
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved 
///            For License Warranty see: common/license.txt              
///
/// @brief     Definitions and types needed by the MCP3424 ADC Device Driver API
/// 
/// 
/// 

#ifndef MCP3424_DEF_H
#define MCP3424_DEF_H

// 1: Defines
// ----------------------------------------------------------------------------

#include "DrvI2cMasterDefines.h"
#include "DrvCommon.h"

#define MCP_NUM_CHANNELS    (4)

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

typedef enum {
    DRV_MCP3424_DRV_SUCCESS             = MYR_DRV_SUCCESS,
    DRV_MCP3424_DRV_ERROR               = MYR_DRV_ERROR,    
    DRV_MCP3424_DRV_NOT_INITIALIZED     = MYR_DRV_NOT_INITIALIZED,
    DRV_MCP3424_DRV_ALREADY_INITIALIZED = MYR_DRV_ALREADY_INITIALIZED,
    DRV_MCP3424_I2C_ERR                 = MYR_DRV_CUSTOM_CODE_START_OFFSET, 
} tyMcpErrorCode;

typedef enum 
{
    MCP_DEV_A = 0,
    MCP_DEV_B = 1,
    MCP_DEV_C = 2,
    MCP_DEV_D = 3,
    MCP_DEV_END = 4,    // This marker is used to terminate the list
} tyMcpDeviceId;

typedef enum {
    CHAN_1      = 0,
    CHAN_2      = 1,
    CHAN_3      = 2,
    CHAN_4      = 3,
} tyMcpChannel;

typedef enum {
    GAIN_X1 = 0,
    GAIN_X2 = 1,
    GAIN_X4 = 2,
    GAIN_X8 = 3,
} tyMcpGain;

typedef enum {
    SPS_240  = 0,
    SPS_60   = 1,
    SPS_15   = 2,
    SPS_3_75 = 3,
} tyMcpRate;

typedef enum {
    MODE_ONESHOT      = 0,
    MODE_CONTINUOUS   = 1,
} tyMcpMode;


typedef struct {
    u8  RdyN;
    u8  initialChannel;
    u8  mode;
    u8  gain;
    u8  sampleRate;
} tyMcp3424Config;


typedef struct {
    I2CM_Device *   i2cDev;
    u32             i2cAddr;
    u32             targetChannel;
    tyMcp3424Config mcpChanConfigs[MCP_NUM_CHANNELS];
} tyMcp3424Handle;

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------


#endif // MCP3424_DEF_H 

