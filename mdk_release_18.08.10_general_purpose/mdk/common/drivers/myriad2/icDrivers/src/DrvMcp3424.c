///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     IC Driver for the MC2434 ADC
///
/// Driver Limitations:
/// This driver is limited to support the single-ended configuration of this ADC chip
/// It makes an inherent assumption that all voltages measured are positive
/// Currently only provides Bare Metal Support (RTEMS TODO)

// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvMcp3424.h"
#include "DrvRegUtils.h"
#include "mv_types.h"
#include "DrvMcp3424.h"
#include "DrvI2cMaster.h"
#include "stdio.h"
#include "assert.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
//#define DRV_MCP3424_DEBUG1
//#define DRV_MCP3424_DEBUG2

#define MCP_DEFAULT_MODE  (MODE_ONESHOT)
#define MCP_DEFAULT_GAIN  (GAIN_X1)
#define MCP_DEFAULT_RATE  (SPS_240)       // 12 Bits
                  
// BIT Offsets within the Config word             
#define MCP_CFG_GAIN_OFFSET             (0)
#define MCP_CFG_RATE_OFFSET             (2)
#define MCP_CFG_MODE_OFFSET             (4)
#define MCP_CFG_CHAN_OFFSET             (5)
#define MCP_CFG_RDYN_OFFSET             (7)

#define MCP_CFG_GAIN_BITS               (2)
#define MCP_CFG_CHAN_BITS               (2)
#define MCP_CFG_MODE_BITS               (1)
#define MCP_CFG_RATE_BITS               (2)
#define MCP_CFG_RDYN_BITS               (1)

// Reference: Table 4-1 in MCP3424 datasheet
#define LSB_RESOLUTION_12BIT_NV         (1     * 1000 * 1000) // 1mV in nanoVolts
#define LSB_RESOLUTION_14BIT_NV         (250   * 1000       ) // 250uV in nanoVolta
#define LSB_RESOLUTION_16BIT_NV         (62500              ) // 62.5uV in nanoVolts
#define LSB_RESOLUTION_18BIT_NV         (15625              ) // 15.625uV in nanoVolts 

// Diagnostic print utiltity
#ifdef DRV_MCP3424_DEBUG1
#   include <stdio.h>
#   define DPRINTF1(...) printf(__VA_ARGS__)
#else
#   define DPRINTF1(...)
#endif

#ifdef DRV_MCP3424_DEBUG2
#   include <stdio.h>
#   define DPRINTF2(...) printf(__VA_ARGS__)
#else
#   define DPRINTF2(...)
#endif


typedef struct {
    u8  rawAdcBytes[4];   // Copy of the full raw I2C bytes
    u32 adcResultRaw; 
    u32 adcResultNv;      // Result of the ADC operation in Nano-volts
    u32 cfgGain;
    u32 cfgRate;
    u32 cfgMode;
    u32 cfgChan;
    u32 cfgRdyN;
} tyMcp3424AdcResult;

// 4: Static Local Data
// ----------------------------------------------------------------------------

// This table returns the number of nanovolts represented by the LSB
// for each sample-rate
static u32 adcLsbResolutionLookupNv[] = 
{
    LSB_RESOLUTION_12BIT_NV,
    LSB_RESOLUTION_14BIT_NV,
    LSB_RESOLUTION_16BIT_NV,
    LSB_RESOLUTION_18BIT_NV,
};

static u8 protocolWrite[] =
{
        S_ADDR_WR,
        DATAW,
        LOOP_MINUS_1
};

static u8 protocolRead[] =
{
        S_ADDR_RD,
        DATAR,
        LOOP_MINUS_1
};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

static u8 calculateCfgByte(tyMcpGain gain, tyMcpChannel channel, tyMcpMode mode, tyMcpRate rate,u32 rdyN);
static int decodeAdcResult(tyMcp3424Handle * mcpHandle,tyMcp3424AdcResult * adcResult);
static int mcp3424WriteI2C(tyMcp3424Handle * mcpHandle, unsigned char dataByte);
static int mcp3424ReadI2C(tyMcp3424Handle * mcpHandle, unsigned char * pBytes, u32 numBytes);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
int Mcp3424Init(tyMcp3424Handle * mcpHandle, I2CM_Device * i2cDev, u32 i2cAddr)
{
    int retVal = 0;
    int chan;
    u8 cfgByte = 0;

    assert(mcpHandle != NULL);
    assert(i2cDev    != NULL);

    // Update the device handle with the necessary configuration data 
    mcpHandle->i2cDev                = i2cDev;
    mcpHandle->i2cAddr               = i2cAddr;

    for (chan=0;chan<MCP_NUM_CHANNELS;chan++)
    {
        // Apply default configuration during Init
        // User may specify custom config later
        mcpHandle->mcpChanConfigs[chan].initialChannel = chan;
        mcpHandle->mcpChanConfigs[chan].gain           = MCP_DEFAULT_GAIN;
        mcpHandle->mcpChanConfigs[chan].mode           = MCP_DEFAULT_MODE;
        mcpHandle->mcpChanConfigs[chan].sampleRate     = MCP_DEFAULT_RATE;
    }

    // Apply the config for to Channel 1 by default
    cfgByte = calculateCfgByte(MCP_DEFAULT_GAIN,CHAN_1,MCP_DEFAULT_MODE,MCP_DEFAULT_RATE,1);

    // Configure the targetted MCP3424 device for the selected configuration
    retVal = mcp3424WriteI2C(mcpHandle,cfgByte);

    return retVal;
}

int Mcp3424ReadChannelOneShot(tyMcp3424Handle * mcpHandle,tyMcpChannel channel,u32 * resultNV,tyMcp3424Config * chanCfgReturn)
{
    int retVal;
    u32 numBytesToRead;
    tyMcp3424Config * pChanConfig;
    u8 cfgByte;
    tyMcp3424AdcResult adcResult;

    // First we need to trigger a oneshot measurement
    // To do this we first write the configuration byte and then read back the result
    // Generate a new configuration based on initial config plus overridden channel and mode
    pChanConfig = &mcpHandle->mcpChanConfigs[channel];
    mcpHandle->targetChannel = channel;

    cfgByte = calculateCfgByte(pChanConfig->gain,channel,MODE_ONESHOT,pChanConfig->sampleRate,1);

    // Writing the config byte triggers the start of the oneshot ADC transaction
    retVal = mcp3424WriteI2C(mcpHandle,cfgByte);
    if (retVal != DRV_MCP3424_DRV_SUCCESS)
        return retVal;

    if (pChanConfig->sampleRate == SPS_3_75) // 18 bit mode requires 3 bytes for result
        numBytesToRead = 4;
    else 
        numBytesToRead = 3;

    // Loop until the RDY Bit goes Low to indicate the transaction has completed
    // TODO: Consider timeout on this 
    // TODO: Consider a calibrated delay followed by polling instead of this.
    do
    {
        retVal = mcp3424ReadI2C(mcpHandle,(u8*)&adcResult.rawAdcBytes,numBytesToRead);
        if (retVal != DRV_MCP3424_DRV_SUCCESS)
            return retVal;
        decodeAdcResult(mcpHandle,&adcResult);
    } while (adcResult.cfgRdyN != 0);

    *resultNV = adcResult.adcResultNv;

    if (chanCfgReturn != NULL)
    {
        chanCfgReturn->gain = pChanConfig->gain;
        chanCfgReturn->sampleRate = pChanConfig->sampleRate;
    }

    return DRV_MCP3424_DRV_SUCCESS;
}


int Mcp3424SampleAdcResult(tyMcp3424Handle * mcpHandle,u32 * resultNV,tyMcp3424Config * chanCfgReturn)
{
    int retVal;
    u32 numBytesToRead;
    tyMcp3424Config * pChanConfig;
    tyMcp3424AdcResult adcResult;

    pChanConfig = &mcpHandle->mcpChanConfigs[mcpHandle->targetChannel];

    if (pChanConfig->sampleRate == SPS_3_75) // 18 bit mode requires 3 bytes for result
        numBytesToRead = 4;
    else 
        numBytesToRead = 3;

    retVal = mcp3424ReadI2C(mcpHandle,(u8*)&adcResult.rawAdcBytes,numBytesToRead);
    if (retVal != DRV_MCP3424_DRV_SUCCESS)
        return retVal;
    decodeAdcResult(mcpHandle,&adcResult);

    *resultNV = adcResult.adcResultNv;

    if (chanCfgReturn != NULL)
    {
        chanCfgReturn->initialChannel = pChanConfig->initialChannel;
        chanCfgReturn->gain = pChanConfig->gain;
        chanCfgReturn->sampleRate = pChanConfig->sampleRate;
        chanCfgReturn->mode = pChanConfig->mode;
        chanCfgReturn->RdyN = adcResult.cfgRdyN;
    }

    return DRV_MCP3424_DRV_SUCCESS;
}


int Mcp3424ApplyCustomConfig(tyMcp3424Handle * mcpHandle, tyMcp3424Config * cfg)
{
    int retVal = 0;
    int chan;
    u8 cfgByte = 0;

    assert(mcpHandle != NULL);
    assert(cfg    != NULL);

    chan = cfg->initialChannel;

    // Update the config params in the device handle
    mcpHandle->targetChannel         = chan;
    mcpHandle->mcpChanConfigs[chan]  = *cfg;

    // Apply the config for to Channel 1 by default
    cfgByte = calculateCfgByte(cfg->gain,cfg->initialChannel,cfg->mode,cfg->sampleRate,1);

    DPRINTF1("Mcp3424ApplyCustomConfig I2C=%02X chan=%d cfg=%02X\n",mcpHandle->i2cAddr,chan,cfgByte);

    // Configure the targetted MCP3424 device for the selected configuration
    retVal = mcp3424WriteI2C(mcpHandle,cfgByte);

    return retVal;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Static Function Implementations
//////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: These I2C functions need to be extended to RTEMS support

// Write byte to MCP3424 (The only available register is the CFG byte)
static int mcp3424WriteI2C(tyMcp3424Handle * mcpHandle, unsigned char dataByte)
{
    I2CM_StatusType retVal;

    // Write one byte to configuration register
    DPRINTF2("I2C_W I2C=%02X CFG:=0x%02X\n",mcpHandle->i2cAddr,dataByte);
    retVal = DrvI2cMTransaction(mcpHandle->i2cDev,mcpHandle->i2cAddr, 0, protocolWrite, &dataByte, 1);
    if (retVal == I2CM_STAT_OK)
        return DRV_MCP3424_DRV_SUCCESS;
    else
        return DRV_MCP3424_I2C_ERR; // TODO: If the I2C driver was at an appropriate OFFSET we could just return the result
}


// Read bytes from MCP3424
static int mcp3424ReadI2C(tyMcp3424Handle * mcpHandle, unsigned char * pBytes, u32 numBytes)
{
    I2CM_StatusType retVal;

    DPRINTF2("I2C_R I2C=%02X numBytes=%d\n",mcpHandle->i2cAddr,numBytes);
    retVal = DrvI2cMTransaction(mcpHandle->i2cDev,mcpHandle->i2cAddr, 0, protocolRead, pBytes, numBytes);
    if (retVal == I2CM_STAT_OK)
        return DRV_MCP3424_DRV_SUCCESS;
    else
        return DRV_MCP3424_I2C_ERR; // TODO: If the I2C driver was at an appropriate OFFSET we could just return the result
}

static u8 calculateCfgByte(tyMcpGain gain, tyMcpChannel channel, tyMcpMode mode, tyMcpRate rate,u32 rdyN)
{
    u8 cfgByte = 0;
    // Build the configuration word
    cfgByte |= ((u8)gain       ) << MCP_CFG_GAIN_OFFSET;
    cfgByte |= ((u8)channel    ) << MCP_CFG_CHAN_OFFSET;
    cfgByte |= ((u8)mode       ) << MCP_CFG_MODE_OFFSET;
    cfgByte |= ((u8)rate       ) << MCP_CFG_RATE_OFFSET;
    cfgByte |= ((u8)rdyN       ) << MCP_CFG_RDYN_OFFSET;

    return cfgByte;
}

static int decodeAdcResult(tyMcp3424Handle * mcpHandle,tyMcp3424AdcResult * adcResult)
{
    u8 * dataBytes = (u8 *)&adcResult->rawAdcBytes;
    u32 cfgByte;        // Deliberately extended to 32 bits
    int sampleRate;

    UNUSED(dataBytes); // Avoid warning if dataBytes is otherwise unused.
    assert(mcpHandle != NULL);
    assert(adcResult != NULL);

    adcResult->adcResultRaw = 0;

    sampleRate = mcpHandle->mcpChanConfigs[mcpHandle->targetChannel].sampleRate;
    if (sampleRate == SPS_3_75) // 18 bit mode requires 3 bytes for result + cfg byte
    {
        cfgByte = (u32)adcResult->rawAdcBytes[3];
        adcResult->adcResultRaw = (adcResult->rawAdcBytes[0] << 16) | (adcResult->rawAdcBytes[1] << 8) |  (adcResult->rawAdcBytes[2] << 0);
        DPRINTF1("AdcResult [%02X]:[%08X] 18Bit cfg=%02X result=%08X\n",mcpHandle->i2cAddr,*(u32*)dataBytes,cfgByte,adcResult->adcResultRaw);
    }
    else
    {
        cfgByte = (u32)adcResult->rawAdcBytes[2];
        adcResult->adcResultRaw = (adcResult->rawAdcBytes[0] << 8) |  (adcResult->rawAdcBytes[1] << 0);
        DPRINTF1("AdcResult [%02X]:[%08X]       cfg=%02X result=%08X\n",mcpHandle->i2cAddr,(*(u32*)dataBytes&0x00FFFFFF),cfgByte,adcResult->adcResultRaw);
    }

    // Extract the parts of the cfg word too
    // TODO: Consider drop the bits of the cfgReg which we don't care about..
    adcResult->cfgGain = DrvRegGetBitField(&cfgByte,MCP_CFG_GAIN_OFFSET,MCP_CFG_GAIN_BITS);
    adcResult->cfgChan = DrvRegGetBitField(&cfgByte,MCP_CFG_CHAN_OFFSET,MCP_CFG_CHAN_BITS); 
    adcResult->cfgMode = DrvRegGetBitField(&cfgByte,MCP_CFG_MODE_OFFSET,MCP_CFG_MODE_BITS);  
    adcResult->cfgRate = DrvRegGetBitField(&cfgByte,MCP_CFG_RATE_OFFSET,MCP_CFG_RATE_BITS);  
    adcResult->cfgRdyN = DrvRegGetBitField(&cfgByte,MCP_CFG_RDYN_OFFSET,MCP_CFG_RDYN_BITS);  

    adcResult->adcResultNv = adcResult->adcResultRaw * adcLsbResolutionLookupNv[sampleRate];

    return DRV_MCP3424_DRV_SUCCESS;
}
