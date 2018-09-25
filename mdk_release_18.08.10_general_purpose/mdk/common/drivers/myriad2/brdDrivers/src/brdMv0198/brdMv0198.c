///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Driver for the MV0198 Power Mesaurement Board
///            This is a daughtercard for the MV0182-R2 or later

// 1: Includes
// ----------------------------------------------------------------------------
#include "brdMv0198.h"
#include "assert.h"
#include "DrvMcp3424.h"
#include <DrvTimer.h>
#include <DrvCpr.h>
#include <DrvRegUtils.h>
#include "string.h"
#include <stdio.h>
#include <dbgTracerApi.h>
#include "brdMv0182.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define DEF_MODE                    (MODE_ONESHOT)
#define DEF_GAIN                    (GAIN_X1)
#define DEF_RATE                    (SPS_240)       // 12 Bits
#define UNIT_NANO_TO_MILI           (0.000001)

#define IS_MA2150 (GET_REG_WORD_VAL(CPR_VERSION_ID_ADR) == 0x3D41221D)
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
tyRailLookup railLookup[] =
{
    {VDDCV_I_MA       , "VDDCV_I_MA     " },
    {VDDIO_B_I_MA     , "VDDIO_B_I_MA   " }, //MA2150
    {VDDCR_I_MA       , "VDDCR_I_MA     " },
    {VDDIO_I_MA       , "VDDIO_I_MA     " },
    {MIPI_VDD_I_MA    , "MIPI_VDD_I_MA  " },
    {PLL_AVDD_I_MA    , "PLL_AVDD_I_MA  " },
    {DRAM_MVDDQ_I_MA  , "DRAM_MVDDQ_I_MA" },
    {DRAM_MVDDA_I_MA  , "DRAM_MVDDA_I_MA" },
    {DRAM_VDD1_I_MA   , "DRAM_VDD1_I_MA " },
    {DRAM_VDD2_I_MA   , "DRAM_VDD2_I_MA " },
    {DRAM_VDDQ_I_MA   , "DRAM_VDDQ_I_MA " },
    {USB_VDD330_I_MA  , "USB_VDD330_I_MA" },
    {USB_VP_VDD_I_MA  , "USB_VP_VDD_I_MA" },
    {VDDCV_V_MV       , "VDDCV_V_MV     " },
    {MIPI_VDD_V_MV    , "MIPI_VDD_V_MV  " },
};

tyAdcChannelConfig  brd198DefaultAdcChannelConfig[] =
{
    {MCP_DEV_A  , CHAN_2, VDDCV_I_MUL       , 0.9, NULL },   // VDDCV_I_MA
    {MCP_DEV_C  , CHAN_4, VDDCC_I_MUL       , 0.9, NULL },   // VDDCC_I_MA   (MA2100)
                                                             // deprecated should be deleted
    {MCP_DEV_C  , CHAN_4, VDDIO_B_I_MUL     , 1.8, NULL },   // VDDIO_B_I_MA (MA2150)
    {MCP_DEV_A  , CHAN_1, VDDCR_I_MUL       , 0.9, NULL },   // VDDCR_I_MA
    {MCP_DEV_C  , CHAN_3, VDDIO_I_MUL       , 1.8, NULL },   // VDDIO_I_MA
    {MCP_DEV_A  , CHAN_4, MIP_VDD_I_MUL     , 1.8, NULL },   // MIPI_VDD_I_MA
    {MCP_DEV_D  , CHAN_2, PLL_AVDD_I_MUL    , 1.8, NULL },   // PLL_AVDD_I_MA
    {MCP_DEV_B  , CHAN_3, DRAM_MVDDQ_I_MUL  , 1.2, NULL },   // DRAM_MVDDQ_I_MA
    {MCP_DEV_B  , CHAN_4, DRAM_MVDDA_I_MUL  , 1.8, NULL },   // DRAM_MVDDA_I_MA
    {MCP_DEV_A  , CHAN_3, DRAM_VDD1_I_MUL   , 1.8, NULL },   // DRAM_VDD1_I_MA
    {MCP_DEV_B  , CHAN_1, DRAM_VDD2_I_MUL   , 1.2, NULL },   // DRAM_VDD2_I_MA
    {MCP_DEV_B  , CHAN_2, DRAM_VDDQ_I_MUL   , 1.2, NULL },   // DRAM_VDDQ_I_MA
    {MCP_DEV_C  , CHAN_1, USB_VDD330_I_MUL  , 3.3, NULL },   // USB_VDD330_I_MA
    {MCP_DEV_C  , CHAN_2, USB_VP_VDD_I_MUL  , 3.3, NULL },   // USB_VP_VDD_I_MA
    {MCP_DEV_D  , CHAN_3, VDDCV_V_MUL       , 0.9, NULL },   // VDDCV_V_V
    {MCP_DEV_D  , CHAN_4, MIPI_VDD_V_MUL    , 1.8, NULL },   // MIPI_VDD_V_V
    {MCP_DEV_END, 0     , 0                 , 0  , NULL },   // MCP_DEV_END marker used to terminate the list
};

// 4: Static Local Data
// ----------------------------------------------------------------------------

static float lookupGainReciprocal[GAIN_X8+1] =
{
    (1.0 / 1.0),            // GAIN_X1 = 0,
    (1.0 / 2.0),            // GAIN_X2 = 1,
    (1.0 / 4.0),            // GAIN_X4 = 2,
    (1.0 / 8.0),            // GAIN_X8 = 3,
};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static int getAdcResult(tyBrd198Handle * brdHandle,tyMcpDeviceId mcpDevId,tyMcpChannel mcpChannel,tyAdcResult * result);
static float calculateAdcResult(u32 resultNv,int gain,double multiplier);
static tyChannelId lookupAdcChannelId(tyBrd198Handle * brdHandle,tyMcpDeviceId mcpDevId,tyMcpChannel mcpChannel);
static int adcChannelCfg(tyAdcContinuousConfig contCfg, tyMcpDeviceId dev);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

int Brd198Init(tyBrd198Handle * brdHandle,I2CM_Device * i2cDev,tyAdcChannelConfig * optChannelConfigOverride)
{
    int retVal = DRV_BRD198_DRV_SUCCESS;
    int index;
    tyMcpDeviceId mcpDeviceId;
    assert(brdHandle != NULL);

        if(IS_MA2150)
            memset(&brd198DefaultAdcChannelConfig[1], 0x0, sizeof(tyAdcChannelConfig));

    if (optChannelConfigOverride != NULL)
        brdHandle->adcConfig = optChannelConfigOverride;
    else
        brdHandle->adcConfig = brd198DefaultAdcChannelConfig;

    // Initialise the 4 ADC devies on the MV0198
    retVal = Mcp3424Init(&brdHandle->mcpHandle[MCP_DEV_A],i2cDev,MV0198_MCP_DEV_A_7ADDR);
    if (retVal)
        return retVal;

    retVal = Mcp3424Init(&brdHandle->mcpHandle[MCP_DEV_B],i2cDev,MV0198_MCP_DEV_B_7ADDR);
    if (retVal)
        return retVal;

    retVal = Mcp3424Init(&brdHandle->mcpHandle[MCP_DEV_C],i2cDev,MV0198_MCP_DEV_C_7ADDR);
    if (retVal)
        return retVal;

    retVal = Mcp3424Init(&brdHandle->mcpHandle[MCP_DEV_D],i2cDev,MV0198_MCP_DEV_D_7ADDR);
    if (retVal)
        return retVal;

    // Check for any custom mcp channel configs and apply them if applicable
    index = 0;
    while (brdHandle->adcConfig[index].mcpDeviceId != MCP_DEV_END) // ADC config is a list terminated by MCP_DEV_END
    {
        if (brdHandle->adcConfig[index].optMcpChanCfg != NULL)
        {
            mcpDeviceId = brdHandle->adcConfig[index].mcpDeviceId;
            retVal = Mcp3424ApplyCustomConfig(&brdHandle->mcpHandle[mcpDeviceId],brdHandle->adcConfig[index].optMcpChanCfg);
            if (retVal)
                return retVal;
        }
        index++; // Count the elements while we are at it
    }
    brdHandle->numAdcChannels = index;

    return retVal;
}

int Brd198GetRailValueOneShot(tyBrd198Handle * brdHandle,tyChannelId chan,float * railValue)
{
    int                retVal;
    int                mcpDevice;
    tyMcp3424Handle   *mcpHandle;
    int                mcpChan;
    u32                resultNv;
    tyMcp3424Config    returnedChanCfg;
    u32                data;
    float              railValueMicro;
    mcpDevice = brdHandle->adcConfig[chan].mcpDeviceId;
    mcpHandle = &brdHandle->mcpHandle[mcpDevice];
    mcpChan   = brdHandle->adcConfig[chan].mcpChannel;
    retVal = Mcp3424ReadChannelOneShot(mcpHandle,mcpChan,&resultNv,&returnedChanCfg);
    if (retVal != MYR_DRV_SUCCESS)
        return retVal;

    *railValue = calculateAdcResult(resultNv,returnedChanCfg.gain,brdHandle->adcConfig[chan].channelMultiplier);

    //mA * 1000 -> uA
    railValueMicro = *railValue * 1000;
    data = (u32)(railValueMicro);
    dbgLogEvent(CHANNEL_TO_SIGNAL(chan), data, DEBUG_LOG_LEVEL_HIGH);


    return retVal;
}

int Brd198AdjustRailConfig(tyBrd198Handle * brdHandle,tyChannelId chan,tyMcp3424Config * channelConfig)
{
    int                retVal;
    int                mcpDevice;
    tyMcp3424Handle   *mcpHandle;

    mcpDevice = brdHandle->adcConfig[chan].mcpDeviceId;
    mcpHandle = &brdHandle->mcpHandle[mcpDevice];
    // The channel selection is passed in as part of the config structure
    channelConfig->initialChannel = brdHandle->adcConfig[chan].mcpChannel;
    retVal = Mcp3424ApplyCustomConfig(mcpHandle, channelConfig);
    if (retVal)
        return retVal;

    return retVal;
}

int Brd198LookupRailDescription(tyBrd198Handle *brdHandle, tyChannelId chan, char *channelName, tyChannelType *railType)
{
    int  retVal = MYR_DRV_SUCCESS;

    // This function only works if the default ADC configuration Table is used
    if (brdHandle->adcConfig != brd198DefaultAdcChannelConfig)
        return DRV_BRD198_UNSUPPORTED_ADCCONFG;

    if (chan > MIPI_VDD_V_MV)
        return DRV_BRD198_INVALID_RAIL;

    strcpy(channelName, railLookup[chan].railName);

    if ((chan == VDDCV_V_MV) || (chan == MIPI_VDD_V_MV))
        *railType = VOLTAGE_MEASUREMENT;
    else
        *railType = CURRENT_MEASUREMENT;

    return retVal;
}

int Brd198ConfigureContinuousMode(tyBrd198Handle * brdHandle,tyAdcContinuousConfig * cfg)
{
    int retVal;
    tyMcp3424Handle   *mcpHandle;
    tyMcp3424Config    mcpConfig;

    brdHandle->continuousCfg = *cfg;

    if (cfg->aChannel != ADC_A_NOT_USED)
    {
        //if one shot mode is selected,then configure
        //if continuous mode is selected, configure only if wanted channel
        //is different from the current channel
        if((brdHandle->mcpHandle[MCP_DEV_A].mcpChanConfigs[(cfg->aChannel-1)].mode == MODE_ONESHOT )
                    ||  (brdHandle->mcpHandle[MCP_DEV_A].mcpChanConfigs[(cfg->aChannel-1)].mode == MODE_CONTINUOUS
                                    && (cfg->aChannel-1) != brdHandle->mcpHandle[MCP_DEV_A].targetChannel))
        {
            mcpHandle       = &brdHandle->mcpHandle[MCP_DEV_A];
            mcpConfig       =  mcpHandle->mcpChanConfigs[(cfg->aChannel -1)];
            mcpConfig.mode  =  MODE_CONTINUOUS;
            // Apply custom config while enabling continuous mode
            retVal = Mcp3424ApplyCustomConfig(mcpHandle, &mcpConfig);
            if (retVal)
                return retVal;
        }
    }
    if (cfg->bChannel != ADC_B_NOT_USED)
    {
        //if one shot mode is selected,then configure
        //if continuous mode is selected, configure only if wanted channel
        //is different from the current channel
        if((brdHandle->mcpHandle[MCP_DEV_B].mcpChanConfigs[(cfg->bChannel -1)].mode == MODE_ONESHOT )
                ||  (brdHandle->mcpHandle[MCP_DEV_B].mcpChanConfigs[(cfg->bChannel -1)].mode == MODE_CONTINUOUS
                     && (cfg->bChannel-1) != brdHandle->mcpHandle[MCP_DEV_B].targetChannel))
        {
            mcpHandle       = &brdHandle->mcpHandle[MCP_DEV_B];
            mcpConfig       =  mcpHandle->mcpChanConfigs[(cfg->bChannel -1)];
            mcpConfig.mode  =  MODE_CONTINUOUS;
            // Apply custom config while enabling continuous mode
            retVal = Mcp3424ApplyCustomConfig(mcpHandle, &mcpConfig);
            if (retVal)
                return retVal;
        }
    }
    if (cfg->cChannel != ADC_C_NOT_USED)
    {
        //if one shot mode is selected,then configure
        //if continuous mode is selected, configure only if wanted channel
        //is different from the current channel
        if((brdHandle->mcpHandle[MCP_DEV_C].mcpChanConfigs[(cfg->cChannel -1)].mode == MODE_ONESHOT )
                ||  (brdHandle->mcpHandle[MCP_DEV_C].mcpChanConfigs[(cfg->cChannel -1)].mode == MODE_CONTINUOUS
                     && (cfg->cChannel-1) != brdHandle->mcpHandle[MCP_DEV_C].targetChannel))
        {
            mcpHandle       = &brdHandle->mcpHandle[MCP_DEV_C];
            mcpConfig       =  mcpHandle->mcpChanConfigs[(cfg->cChannel -1)];
            mcpConfig.mode  =  MODE_CONTINUOUS;
            // Apply custom config while enabling continuous mode
            retVal = Mcp3424ApplyCustomConfig(mcpHandle, &mcpConfig);
            if (retVal)
                return retVal;
        }
    }
    if (cfg->dChannel != ADC_D_NOT_USED)
    {
        //if one shot mode is selected,then configure
        //if continuous mode is selected, configure only if wanted channel
        //is different from the current channel
        if((brdHandle->mcpHandle[MCP_DEV_D].mcpChanConfigs[(cfg->dChannel -1)].mode == MODE_ONESHOT )
                ||  (brdHandle->mcpHandle[MCP_DEV_D].mcpChanConfigs[(cfg->dChannel -1)].mode == MODE_CONTINUOUS
                     && (cfg->dChannel-1) != brdHandle->mcpHandle[MCP_DEV_D].targetChannel))
        {
            mcpHandle       = &brdHandle->mcpHandle[MCP_DEV_D];
            mcpConfig       =  mcpHandle->mcpChanConfigs[(cfg->dChannel -1)];
            mcpConfig.mode  =  MODE_CONTINUOUS;
            // Apply custom config while enabling continuous mode
            retVal = Mcp3424ApplyCustomConfig(mcpHandle, &mcpConfig);
            if (retVal)
                return retVal;
        }
    }

    return MYR_DRV_SUCCESS;
}

int Brd198SampleRailContinuousMode(tyBrd198Handle * brdHandle,tyAdcContinuousResult * contResult)
{
    int                 retVal;
    tyChannelId         chan;
    u32                 data;
    float channelAMicro;
    float channelBMicro;
    float channelCMicro;
    float channelDMicro;

    if (brdHandle->continuousCfg.aChannel != ADC_A_NOT_USED)
    {
    	chan = adcChannelCfg(brdHandle->continuousCfg, MCP_DEV_A);

        retVal = getAdcResult(brdHandle,MCP_DEV_A,(tyMcpChannel)(brdHandle->continuousCfg.aChannel-1), &contResult->adcAResult);

        if(contResult->adcAResult.bNewSample != 0)
        {
            //record value
            //mA * 1000 -> uA
            channelAMicro = contResult->adcAResult.adcValue * 1000;
            data = (u32)channelAMicro;
            dbgLogEvent(CHANNEL_TO_SIGNAL(chan), data, DEBUG_LOG_LEVEL_HIGH);
        }

        contResult->adcAResult.chanId = chan;

        if (retVal)
            return retVal;
    }

    if (brdHandle->continuousCfg.bChannel != ADC_B_NOT_USED)
    {
    	chan = adcChannelCfg(brdHandle->continuousCfg, MCP_DEV_B);

        retVal = getAdcResult(brdHandle,MCP_DEV_B,(tyMcpChannel)(brdHandle->continuousCfg.bChannel-1), &contResult->adcBResult);

        if(contResult->adcBResult.bNewSample != 0)
        {
            //record value
            //mA * 1000 -> uA
            channelBMicro = contResult->adcBResult.adcValue * 1000;
            data = (u32)channelBMicro;
            dbgLogEvent(CHANNEL_TO_SIGNAL(chan), data, DEBUG_LOG_LEVEL_HIGH);
        }

        contResult->adcBResult.chanId = chan;

        if (retVal)
            return retVal;
    }

    if (brdHandle->continuousCfg.cChannel != ADC_C_NOT_USED)
    {
    	chan = adcChannelCfg(brdHandle->continuousCfg, MCP_DEV_C);

        retVal = getAdcResult(brdHandle,MCP_DEV_C,(tyMcpChannel)(brdHandle->continuousCfg.cChannel-1), &contResult->adcCResult);
        if (retVal != MYR_DRV_SUCCESS)
        {
            contResult->adcCResult.bNewSample =0;
        }

        if ((contResult->adcCResult.bNewSample) != (int)(0))
        {
            //record value
            //mA * 1000 -> uA
            channelCMicro = contResult->adcCResult.adcValue * 1000;
            data = (u32) channelCMicro ;
            dbgLogEvent(CHANNEL_TO_SIGNAL(chan), data,	DEBUG_LOG_LEVEL_HIGH);
        }

        contResult->adcCResult.chanId = chan;

        if (retVal)
            return retVal;
    }

    if (brdHandle->continuousCfg.dChannel != ADC_D_NOT_USED)
    {
    	chan = adcChannelCfg(brdHandle->continuousCfg, MCP_DEV_D);

        retVal = getAdcResult(brdHandle,MCP_DEV_D,(tyMcpChannel)(brdHandle->continuousCfg.dChannel-1), &contResult->adcDResult);
        if (retVal != MYR_DRV_SUCCESS)
        {
            contResult->adcCResult.bNewSample =0;
        }

        if ((int)(contResult->adcDResult.bNewSample) != 0)
        {
            //record value
            //mA * 1000 -> uA
            channelDMicro = contResult->adcDResult.adcValue * 1000;
            data = (u32) channelDMicro;
            dbgLogEvent(CHANNEL_TO_SIGNAL(chan), data,	DEBUG_LOG_LEVEL_HIGH);
        }

        contResult->adcDResult.chanId = chan;

        if (retVal)
            return retVal;
    }

    return MYR_DRV_SUCCESS;
}

int Brd198SampleAllRails(tyBrd198Handle * brdHandle, tyAdcResultAllRails * result)
{
    tyAdcContinuousConfig   cfg;
    tyAdcContinuousResult   contResult;
    int                     retVal;
    int                     chan;

    // Set first channel of each ADC as defined in the enums
    cfg.aChannel = ADC_A_VDDCR_I_MA;
    cfg.bChannel = ADC_B_DRAM_VDD2_I_MA;
    cfg.cChannel = ADC_C_USB_VDD330_I_MA;
    cfg.dChannel = ADC_D_PLL_AVDD_I_MA;

    // Step through all channels of each ADC
    for (chan = 0; chan < 4; chan++)
    {
        // Non-blocking call to reprogram the ADCs to read a different channel
        retVal = Brd198ConfigureContinuousMode(brdHandle, &cfg);
        if (retVal != MYR_DRV_SUCCESS)
        {
            return retVal;
        }

        // Must wait here until the ADC is ready to be asked for data otherwise we
        // get wrong (lower) numbers.
        DrvTimerSleepMs(5);

        // Sample each set channel from each ADC
        retVal = Brd198SampleRailContinuousMode(brdHandle, &contResult);
        if (retVal != MYR_DRV_SUCCESS)
        {
            return retVal;
        }

        // The results array holds the rails values in the order defined in tyChannelId
        result->adcResults[contResult.adcAResult.chanId] = contResult.adcAResult.adcValue;
        result->adcResults[contResult.adcBResult.chanId] = contResult.adcBResult.adcValue;
        result->adcResults[contResult.adcCResult.chanId] = contResult.adcCResult.adcValue;
        result->adcResults[contResult.adcDResult.chanId] = contResult.adcDResult.adcValue;

        cfg.aChannel++;
        cfg.bChannel++;
        cfg.cChannel++;
        cfg.dChannel++;
        if (cfg.dChannel == ADC_D_MIPI_VDD_V_MV + 1)
        {
            // If the current channel is last channel, set first channel
            cfg.dChannel = ADC_D_PLL_AVDD_I_MA;
        }
    }

    result->totalMilliWatts = 0.0f;
    result->totalMilliAmps = 0.0f;

    if (IS_MA2150)
        result->adcResults[VDDCC_I_MA] = 0;


    // Parse the results here to assure we've read both mV and mA for VDDCV and MIPI
    // regardless of the reading order. Skip last 2 since we know they are mV.
    // Take into consideration the additional tyChannelId enum entry.
    for (chan = 0; chan < (DEFAULT_NUM_RAILS - 2); chan++)
    {
        // Sum current
        result->totalMilliAmps += result->adcResults[chan];

        // Sum power
        switch ((tyChannelId)chan)
        {
        case VDDCV_I_MA:
            result->totalMilliWatts += brdHandle->adcConfig[VDDCV_V_MV].railNominalV * result->adcResults[chan];
        break;
        case MIPI_VDD_I_MA:
            result->totalMilliWatts += brdHandle->adcConfig[MIPI_VDD_V_MV].railNominalV * result->adcResults[chan];
        break;
        default:
            result->totalMilliWatts += brdHandle->adcConfig[chan].railNominalV * result->adcResults[chan];
        break;
        }
    }

    dbgLogEvent(LOG_EVENT_198_TOTAL_CURRENT, result->totalMilliAmps, DEBUG_LOG_LEVEL_HIGH);
    dbgLogEvent(LOG_EVENT_198_TOTAL_POWER, result->totalMilliWatts, DEBUG_LOG_LEVEL_HIGH);
    return MYR_DRV_SUCCESS;
}

void Brd198GetDdrPowerAndCurrent(tyBrd198Handle * brdHandle, tyAdcResultAllRails * samples,
   float * power, float * current)
{
    int chan;
    float totalPower = 0.0f;
    float totalCurrent = 0.0f;

    // DDR has 5 current rails
    for (chan = DRAM_MVDDQ_I_MA; chan <= DRAM_VDDQ_I_MA; chan++)
    {
        totalPower += brdHandle->adcConfig[chan].railNominalV * samples->adcResults[chan];
        totalCurrent += samples->adcResults[chan];
    }

    *power = totalPower;
    *current = totalCurrent;

    dbgLogEvent(LOG_EVENT_198_DDR_CURRENT, totalCurrent, DEBUG_LOG_LEVEL_HIGH);
    dbgLogEvent(LOG_EVENT_198_DDR_POWER, totalPower, DEBUG_LOG_LEVEL_HIGH);
}


static int adcChannelCfg(tyAdcContinuousConfig contCfg, tyMcpDeviceId dev)
{
    tyChannelId cnl;
    switch(dev)
    {
        case MCP_DEV_A:
        {
            switch(contCfg.aChannel)
            {
            case ADC_A_VDDCR_I_MA:
                cnl = VDDCR_I_MA;
                break;
            case ADC_A_VDDCV_I_MA:
                cnl = VDDCV_I_MA;
                break;
            case ADC_A_DRAM_VDD1_I_MA:
                cnl = DRAM_VDD1_I_MA;
                break;
            case ADC_A_MIPI_VDD_I_MA:
                cnl = MIPI_VDD_I_MA;
                break;
            default :
                return MYR_DRV_ERROR;
                break;
            }
            return cnl;
        }
        case MCP_DEV_B:
        {
            switch (contCfg.bChannel)
            {
            case ADC_B_DRAM_VDD2_I_MA:
                cnl = DRAM_VDD2_I_MA;
                break;
            case ADC_B_DRAM_VDDQ_I_MA:
                cnl = DRAM_VDDQ_I_MA;
                break;
            case ADC_B_DRAM_MVDDQ_I_MA:
                cnl = DRAM_MVDDQ_I_MA;
                break;
            case ADC_B_DRAM_MVDDA_I_MA:
                cnl = DRAM_MVDDA_I_MA;
                break;
            default:
                return MYR_DRV_ERROR;
                break;
            }
            return cnl;
        }
        case MCP_DEV_C:
        {
            switch(contCfg.cChannel)
            {
            case ADC_C_USB_VDD330_I_MA:
                cnl = USB_VDD330_I_MA;
                break;
            case ADC_C_USB_VP_VDD_I_MA:
                cnl = USB_VP_VDD_I_MA;
                break;
            case ADC_C_VDDIO_I_MA:
                cnl = VDDIO_I_MA;
                break;
            case ADC_C_VDD_CC_IOB_MA: // ADC_C_VDDCC_MA for MA2100, which is
                                      // deprecated - should be removed
                                      // ADC_C_VDDIO_B_I_MA for MA2150
                if (IS_MA2150)
                    cnl = VDDIO_B_I_MA;
                else
                    cnl = VDDCC_I_MA;
                break;
            default:
                return MYR_DRV_ERROR;
                break;
            }
            return cnl;
        }
        case MCP_DEV_D:
        {
            switch (contCfg.dChannel)
            {
            case ADC_D_PLL_AVDD_I_MA:
                cnl = PLL_AVDD_I_MA;
                break;
            case ADC_D_VDDCV_V_MV:
                cnl = VDDCV_V_MV;
                break;
            case ADC_D_MIPI_VDD_V_MV:
                cnl = MIPI_VDD_V_MV;
                break;
            default:
                return MYR_DRV_ERROR;
            }
            return cnl;
        }
        default :
            return MYR_DRV_ERROR;
            break;
    }
}

static tyChannelId lookupAdcChannelId(tyBrd198Handle * brdHandle,tyMcpDeviceId mcpDevId,tyMcpChannel mcpChannel)
{
    tyChannelId chanId=0;
    tyAdcChannelConfig *adcCfg;
    adcCfg = brdHandle->adcConfig;
    do
    {
        if ((adcCfg[chanId].mcpDeviceId == mcpDevId) && (adcCfg[chanId].mcpChannel == mcpChannel))
            return (chanId);

        chanId++;
    } while (adcCfg[chanId].mcpDeviceId !=  MCP_DEV_END);

    // If the code makes it to here we were passed invalid params
    assert(adcCfg[chanId].mcpDeviceId !=  MCP_DEV_END);
    return -1;
}

static int getAdcResult(tyBrd198Handle * brdHandle,tyMcpDeviceId mcpDevId,tyMcpChannel mcpChannel,tyAdcResult * result)
{
    int                retVal;
    int                gain;
    double             multiplier;
    tyChannelId        adcChanId;

    tyMcp3424Handle   *mcpHandle;
    tyMcp3424Config    chanCfgReturn;
    u32                resultNv;

    mcpHandle = &brdHandle->mcpHandle[mcpDevId];

    retVal = Mcp3424SampleAdcResult(mcpHandle,&resultNv,&chanCfgReturn);
    if (retVal)
        return retVal;

    // Do some checks to make sure the configuration is good
    if (chanCfgReturn.initialChannel != mcpChannel)
        return DRV_BRD198_INVALID_CHAN_CONFIG;
    if (chanCfgReturn.mode != MODE_CONTINUOUS)
        return DRV_BRD198_INVALID_CONT_MODE_NOT_SELECTED;

    adcChanId  = lookupAdcChannelId(brdHandle,mcpDevId,mcpChannel);
    gain       = chanCfgReturn.gain;
    multiplier = brdHandle->adcConfig[adcChanId].channelMultiplier;

    result->adcValue    = calculateAdcResult(resultNv,gain,multiplier);
    result->bNewSample  = !chanCfgReturn.RdyN;

    return MYR_DRV_SUCCESS;
}

static float calculateAdcResult(u32 resultNv,int gain,double multiplier)
{
    float     railValue;
    double    result;
    double    gainReciprocal;

    gainReciprocal = (double)lookupGainReciprocal[gain];

    result = (double)resultNv * multiplier * UNIT_NANO_TO_MILI;
    result = result * gainReciprocal;  // Compensate for any gain applied at ADC level
    railValue = (float)result;         // Keep max precision until calculation complete
    return railValue;
}
