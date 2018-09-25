///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     R0 Leon static power measurement example
///
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <DrvMcp3424.h>
#include <DrvCpr.h>
#ifdef MV0212
#include "MV0212.h"
#include "brdGpioCfgs/brdMv0182R5GpioDefaults.h"
#elif defined(MV0182)
#include <Board182Api.h>
#include <brdMv0182.h>
#endif
#include <brdMv0198.h>
#include "DrvCDCEL.h"

#include <app_config.h>
#include "DrvCmxDma.h"
#include "DrvTimer.h"

#include <UnitTestApi.h>

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#include <DrvTempSensor.h>
#endif

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

#define BUFF_SIZE ( 1024 * 512 * 16 - 32)
#define SIZE_1K     1024
#define DEFAULT_NUM_CHANNELS 16
//priority of the cmx dma task
#define DMA_PRIORITY_ONE    1

#define NO_ERROR    0

#define IS_MA2150 (GET_REG_WORD_VAL(CPR_VERSION_ID_ADR) == 0x3D41221D)

#ifdef MV0212
#define NUM_I2C_DEVS 3
#endif
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
static I2CM_Device * i2c2Handle;

// 4: Static Local Data
// ----------------------------------------------------------------------------

static u8 __attribute__((section(".ddr.bss"))) ddrBuffSrc[BUFF_SIZE];
static u8 __attribute__((section(".ddr.bss"))) ddrBuffDest[BUFF_SIZE];

static dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) task;
static dmaRequesterId id;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static u32 verifyData(u8 *src, u8 *dest, u32 size);

static void fillBuffer(u8 *data, u32 size);

static void initSystem(tyBrd198Handle* statPowerHandle);
static s32 sampleRails(u32 railsGroup, tyBrd198Handle * powerMonHandle,
                       tyAdcContinuousConfig* cfg, tyAdcResultAllRails * result);

// 6: Functions Implementation
// ----------------------------------------------------------------------------


static u32 verifyData(u8 *src, u8 *dest, u32 size)
{
    u32 i;

    for (i = 0; i < size; i++)
        if (src[i] != dest[i])
            return -1;
    return NO_ERROR;
}

static void fillBuffer(u8 *data, u32 size)
{
    u32 i;

    for (i = 0; i < size; i++)
        data[i] = i;
}

static void initSystem(tyBrd198Handle* statPowerHandle)
{
    u32 ret;
    s32 boardStatus;
    initClocksAndMemory();
    swcLeonSetPIL(0);

#ifdef MV0212
    int32_t rc;
    uint32_t rev;
    BoardI2CInfo info[NUM_I2C_DEVS];
    BoardConfigDesc config[] =
    {
        {
            BRDCONFIG_GPIO,
            // use this for the sake of testing as it has the same gpio config as the MV0212 R0
            (void *)brdMV0182R5GpioCfgDefault
        },
        {
            BRDCONFIG_END,
            NULL
        }
    };

    rc = BoardInit(config);
    if (rc!=BRDCONFIG_SUCCESS)
    {
    	printf("Error: board initialization failed with %ld status\n",
    	    			rc);
        exit(rc);
    }

    rc = BoardGetPCBRevision(&rev);
    if (rc!=BRDCONFIG_SUCCESS)
    {
    	printf("Error: board configuration read failed with %ld status\n",
    	    			rc);
        exit(rc);
    }
    printf("Board Mv0212 initialized, revision = %lu \n", rev);

    boardStatus = BoardInitExtPll(EXT_PLL_CFG_148_24_24MHZ);
    if (boardStatus != BRDCONFIG_SUCCESS)
    {
    	printf("Error: board initialization failed with %ld status\n",
    			boardStatus);
        exit(-1);
    }
    rc = BoardGetI2CInfo(info, NUM_I2C_DEVS);
    i2c2Handle=info[2].handler;
#elif defined(MV0182)
    extern tyAppDeviceHandles gAppDevHndls;
    boardStatus = BoardInitialise(EXT_PLL_CFG_148_24_24MHZ);
    i2c2Handle=gAppDevHndls.i2c2Handle;
    if (boardStatus != B_SUCCESS)
    {
    	printf("Error: board initialization failed with %ld status\n",
    			boardStatus);
        exit(-1);
    }
#endif

    ret = Brd198Init(statPowerHandle, i2c2Handle, NULL);
    if (ret != MYR_DRV_SUCCESS)
    {
        printf("MV0198 failed to initialize\n");
        exit(ret);
    }
}

static s32 sampleRails(u32 railsGroup, tyBrd198Handle * powerMonHandle,
                       tyAdcContinuousConfig* cfg, tyAdcResultAllRails * result)
{
    tyAdcContinuousResult   contResult;
    u32 chan;
    if (railsGroup == 0)
    {
        for (chan = 0; chan < DEFAULT_NUM_CHANNELS - 2; chan++)
        {
            result->adcResults[chan] = 0;
        }

        cfg->aChannel = ADC_A_VDDCR_I_MA;
        cfg->bChannel = ADC_B_DRAM_VDD2_I_MA;
        cfg->cChannel = ADC_C_USB_VDD330_I_MA;
        cfg->dChannel = ADC_D_PLL_AVDD_I_MA;
    }
    if (Brd198ConfigureContinuousMode(powerMonHandle, cfg))
        return -1;

    // Must wait here until the ADC is ready to be asked for data otherwise we
    // get wrong (lower) numbers.
    DrvTimerSleepMs(5);

    if (Brd198SampleRailContinuousMode(powerMonHandle, &contResult))
        return -2;

    // The results array holds the rails values in the order defined in tyChannelId
    result->adcResults[contResult.adcAResult.chanId] = contResult.adcAResult.adcValue;
    result->adcResults[contResult.adcBResult.chanId] = contResult.adcBResult.adcValue;
    result->adcResults[contResult.adcCResult.chanId] = contResult.adcCResult.adcValue;
    result->adcResults[contResult.adcDResult.chanId] = contResult.adcDResult.adcValue;

    //configure the next 4 rails
    cfg->aChannel++;
    cfg->bChannel++;
    cfg->cChannel++;
    cfg->dChannel++;

    if (cfg->dChannel == ADC_D_MIPI_VDD_V_MV + 1)
    {
        // If the current channel is last channel, set first channel
        cfg->dChannel = ADC_D_PLL_AVDD_I_MA;
    }


    result->totalMilliWatts = 0.0f;
    result->totalMilliAmps = 0.0f;


    if (IS_MA2150)
        result->adcResults[VDDCC_I_MA] = 0;

    // Parse the results here to assure we've read both mV and mA for VDDCV and MIPI
    // regardless of the reading order. Skip last 2 since we know they are mV.
    for (chan = 0; chan < DEFAULT_NUM_CHANNELS - 2; chan++)
    {
        // Sum current
        result->totalMilliAmps += result->adcResults[chan];

        // Sum power
        switch ((tyChannelId) chan)
        {
        case VDDCV_I_MA:
            result->totalMilliWatts +=
                            (result->adcResults[VDDCV_V_MV] * result->adcResults[chan])/1000;
            break;
        case MIPI_VDD_I_MA:
            result->totalMilliWatts +=
                            (result->adcResults[MIPI_VDD_V_MV] * result->adcResults[chan])/1000;
            break;
        default:
            result->totalMilliWatts +=
                            powerMonHandle->adcConfig[chan].railNominalV * result->adcResults[chan];
            break;
        }
        // printf("RESULTS %f MIPI %f\n",result->adcResults[VDDCV_V_MV] ,result->adcResults[MIPI_VDD_V_MV]);
    }
    return 0;
}

// function used to check for error and report it via printf
void properlyChkRepError(int ErrorCode, char *TextMessage)
{
    if (ErrorCode != 0)
    {
        printf("Error code %d returned while executing %s\n", (int)ErrorCode, TextMessage);
    }
    unitTestAssert((int)(ErrorCode == 0));
}

int main()
{
    u32 i,j;
    tyBrd198Handle statPowerHandle;
    fp32 ddrMw;
    fp32 ddrMa;
    tyAdcResultAllRails powRes;
    fp32 coreMw;
    u32 size = BUFF_SIZE;
    s32 status;
    dmaTransactionList_t *ref;
    tyAdcContinuousConfig powContCfg;
// variables used for temperature measuremnt
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    int32_t tempSensorStat = 0;
    float temp_MSS;
    float temp_CSS;
    float temp_UPA0;
    float temp_UPA1;
#endif

    initSystem(&statPowerHandle);

    unitTestInit();

// temperature sensor only availabe for the following
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    tempSensorStat = DrvTempSensorSetMode(TSENS_CSS, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
    tempSensorStat += DrvTempSensorSetMode(TSENS_MSS, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
    tempSensorStat += DrvTempSensorSetMode(TSENS_UPA0, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
    tempSensorStat += DrvTempSensorSetMode(TSENS_UPA1, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
    properlyChkRepError(tempSensorStat,"Set Mode for Temperature Sensor");
#endif

    printf("Static power measurement with minimal clocks (no DDR)\n");

    for (i = 0; i < 10; i++)
    {
        ddrMw = 0;
        Brd198SampleAllRails(&statPowerHandle, &powRes);
        Brd198GetDdrPowerAndCurrent(&statPowerHandle, &powRes, &ddrMw, &ddrMa);
        coreMw = powRes.totalMilliWatts - ddrMw;
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
        tempSensorStat = DrvTempSensorGetSample(TSENS_CSS, &temp_CSS);
        DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_CPU);
        DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_SIPP);
        tempSensorStat += DrvTempSensorGetSample(TSENS_MSS, &temp_MSS);
        DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_SIPP);
        DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_CPU);
        tempSensorStat += DrvTempSensorGetSample(TSENS_UPA0, &temp_UPA0);
        tempSensorStat += DrvTempSensorGetSample(TSENS_UPA1, &temp_UPA1);
        properlyChkRepError(tempSensorStat,"Read Temperature Sensor value");
        printf("Core mW = %7.03f, DDR mW = %7.03f, Temperature CSS=%2.1fC, MSS=%2.1fC, UPA0=%2.1fC, UPA1=%2.1fC\n", coreMw, ddrMw, temp_CSS,temp_MSS,temp_UPA0,temp_UPA1);
#else
        printf("Core mW = %7.03f, DDR mW = %7.03f\n", coreMw, ddrMw);
#endif
    }

    turnOnDDR();

    printf("\nStatic power measurement with minimal clocks (DDR)\n");

    for (i = 0; i < 10; i++)
    {

        Brd198SampleAllRails(&statPowerHandle, &powRes);
        Brd198GetDdrPowerAndCurrent(&statPowerHandle, &powRes, &ddrMw, &ddrMa);
        coreMw = powRes.totalMilliWatts - ddrMw;
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
        tempSensorStat = DrvTempSensorGetSample(TSENS_CSS, &temp_CSS);
        DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_CPU);
        DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_SIPP);
        tempSensorStat += DrvTempSensorGetSample(TSENS_MSS, &temp_MSS);
        DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_SIPP);
        DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_CPU);
        tempSensorStat += DrvTempSensorGetSample(TSENS_UPA0, &temp_UPA0);
        tempSensorStat += DrvTempSensorGetSample(TSENS_UPA1, &temp_UPA1);
        properlyChkRepError(tempSensorStat,"Read Temperature Sensor value");
        printf("Core mW = %7.03f, DDR mW = %7.03f, Temperature CSS=%2.1fC, MSS=%2.1fC, UPA0=%2.1fC, UPA1=%2.1fC\n", coreMw, ddrMw, temp_CSS,temp_MSS,temp_UPA0,temp_UPA1);
#else
        printf("Core mW = %7.03f, DDR mW = %7.03f\n", coreMw, ddrMw);
#endif
    }

    turnOnDmaClocks();

    // Initialize tasks handle
    status = DrvCmxDmaInitDefault();
    if (status)
    {
        printf("error %ld \n", status);
        exit(-1);
    }

    id = DrvCmxDmaInitRequester(DMA_PRIORITY_ONE);

    printf("\nStatic power measurement with DMA transactions\n");
    for (j = 0; j < 10; j++)
    {
        //make 4 transfers. At each transfer we measure 4 different rails
        for (i = 0; i < 4; i++)
        {
            fillBuffer((u8*) ddrBuffSrc, size);
            memset((u8*) ddrBuffDest, 0, size);

            ref = DrvCmxDmaCreateTransactionFullOptions(id,
                                                        &task,
                                                        (u8*) ddrBuffSrc,
                                                        (u8*) ddrBuffDest,
                                                        BUFF_SIZE,
                                                        SIZE_1K,
                                                        SIZE_1K,
                                                        SIZE_1K,
                                                        SIZE_1K);

            status = DrvCmxDmaStartListTask(ref);
            if (status == CDMA_TASK_ERR)
            {
                printf("error %ld \n", status);
                exit(-2);
            }

            //after this function is called for 4 times, the powRes structure
            //will be populated with all the values associated to channels
            if (sampleRails(i, &statPowerHandle, &powContCfg, &powRes))
            {
                printf("error %ld \n", status);
                exit(-3);
            }

            status = DrvCmxDmaWaitTask(ref);
            if (status)
            {
                printf("error %ld \n", status);
                exit(-4);
            }

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
            if(i == 0)      //get temperature only once
            {
                tempSensorStat = DrvTempSensorGetSample(TSENS_CSS, &temp_CSS);
                DrvCprPowerTurnOnIsland(POWER_ISLAND_MSS_SIPP);
                tempSensorStat += DrvTempSensorGetSample(TSENS_MSS, &temp_MSS);
                DrvCprPowerTurnOffIsland(POWER_ISLAND_MSS_SIPP);
                tempSensorStat += DrvTempSensorGetSample(TSENS_UPA0, &temp_UPA0);
                tempSensorStat += DrvTempSensorGetSample(TSENS_UPA1, &temp_UPA1);
                properlyChkRepError(tempSensorStat,"Read Temperature Sensor value");
            }
#endif
            unitTestAssert(verifyData(ddrBuffSrc, ddrBuffDest, BUFF_SIZE) == NO_ERROR);
        }
        ddrMw = 0;
        ddrMa = 0;
        Brd198GetDdrPowerAndCurrent(&statPowerHandle, &powRes, &ddrMw, &ddrMa);
        coreMw = powRes.totalMilliWatts - ddrMw;
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
        printf("Core mW = %7.03f, DDR mW = %7.03f, Temperature CSS=%2.1fC, MSS=%2.1fC, UPA0=%2.1fC, UPA1=%2.1fC\n", coreMw, ddrMw, temp_CSS,temp_MSS,temp_UPA0,temp_UPA1);
#else
        printf("Core mW = %7.03f, DDR mW = %7.03f\n", coreMw, ddrMw);
#endif
    }

    unitTestFinalReport();

    return 0;
}
