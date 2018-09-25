///
/// @file DrvDdrArchMa245x.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvDdrArchMa245x
/// @{
/// @brief     TODO_ADD_ONE_LINE_TEST_DESCRIPTION_HERE
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <registersMyriad.h>
#include <DrvRegUtils.h>
#include <DrvTimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <DrvCpr.h>
#include <DrvDdr.h>
#include "DrvDdrArch.h"
#include "DrvDdrAddrMapMa2x5x.h"
#include <DrvEfuse.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

const tyDdrConfig defaultDdrConfigMA245x_4L =
{
    .version = 7,
    .dieDriveStrength = LPDDR3_DIE_DRIVE_48_OHM,
    .dieOdtStrength = ODT_RZQ_DIV_1,
    .useCalibratedDriveDqStrength = 0,
    .useCalibratedDriveAcStrength = 0,
    .useCalibratedOdtStrength = 0,
    .manualDriveDqPullDownStrength = 0x70,
    .manualDriveDqPullUpStrength = 0x70,
    .manualDriveAcPullDownStrength = 0x70,
    .manualDriveAcPullUpStrength = 0x70,
    .manualOdtPullDownStrength = 0xa,
    .manualOdtPullUpStrength = 0xa,
    .calibratedDriveDqPullDownStrengthTarget = 0xf,
    .calibratedDriveDqPullUpStrengthTarget = 0xf,
    .calibratedDriveAcPullDownStrengthTarget = 0xf,
    .calibratedDriveAcPullUpStrengthTarget = 0xf,
    .calibratedOdtStrengthTarget = 0x2,
    .isCalibratedOdtStrengthPullUpOnly = 0,
    .acTweakPercent = 50,
    .dqsTweakPercent = 13,
    .perBankRefresh = 1,
    .refreshBurst = REFRESH_BURST_DEFAULT,
    .averageTimeBetweenAllBankRefreshCommandsNs = DEFAULT_REFRESH_TIMING,
    .averageTimeBetweenPerBankRefreshCommandsNs = DEFAULT_REFRESH_TIMING,
    .allBankRefreshCycleTimeNs = DEFAULT_REFRESH_TIMING,
    .perBankRefreshCycleTimeNs = DEFAULT_REFRESH_TIMING,
    .en_dfi_dram_clk_disable = 1,
    .powerdown_en = 1,
    .selfref_en = 1,
    .derating_en = 0,
    .address_map = address_map_R13_R0__B2_B0__C9_C0_32_bit_die,
#ifdef OVERRIDE_DEFAULT_FREQUENCY_KHZ_4L
    .default_frequency_khz = OVERRIDE_DEFAULT_FREQUENCY_KHZ_4L,
#else
    .default_frequency_khz = 732000 / 2,
#endif
};

// Not yet customized 6L version.
const tyDdrConfig defaultDdrConfigMA245x_6L =
{
    .version = 7,
    .dieDriveStrength = LPDDR3_DIE_DRIVE_48_OHM,
    .dieOdtStrength = ODT_DISABLED,
    .useCalibratedDriveDqStrength = 1,
    .useCalibratedDriveAcStrength = 1,
    .useCalibratedOdtStrength = 0,
    .manualDriveDqPullDownStrength = 0x70,
    .manualDriveDqPullUpStrength = 0x70,
    .manualDriveAcPullDownStrength = 0x70,
    .manualDriveAcPullUpStrength = 0x70,
    .manualOdtPullDownStrength = 0xa,
    .manualOdtPullUpStrength = 0xa,
    .calibratedDriveDqPullDownStrengthTarget = 0x9,
    .calibratedDriveDqPullUpStrengthTarget = 0x9,
    .calibratedDriveAcPullDownStrengthTarget = 0x9,
    .calibratedDriveAcPullUpStrengthTarget = 0x9,
    .calibratedOdtStrengthTarget = 0x2,
    .isCalibratedOdtStrengthPullUpOnly = 0,
    .acTweakPercent = 0,
    .dqsTweakPercent = 0,
    .perBankRefresh = 1,
    .refreshBurst = REFRESH_BURST_DEFAULT,
    .averageTimeBetweenAllBankRefreshCommandsNs = DEFAULT_REFRESH_TIMING,
    .averageTimeBetweenPerBankRefreshCommandsNs = DEFAULT_REFRESH_TIMING,
    .allBankRefreshCycleTimeNs = DEFAULT_REFRESH_TIMING,
    .perBankRefreshCycleTimeNs = DEFAULT_REFRESH_TIMING,
    .en_dfi_dram_clk_disable = 1,
    .powerdown_en = 1,
    .selfref_en = 1,
    .derating_en = 0,
    .address_map = address_map_R13_R0__B2_B0__C9_C0_32_bit_die,
#ifdef OVERRIDE_DEFAULT_FREQUENCY_KHZ_6L
    .default_frequency_khz = OVERRIDE_DEFAULT_FREQUENCY_KHZ_6L,
#else
    .default_frequency_khz = 912000 / 2,
#endif
};

const int * const address_map_B2_B0__rows__columns = address_map_B2_B0__R13_R0__C9_C0_32_bit_die;
const int * const address_map_B2_B1__rows__B0__columns = address_map_B2_B1__R13_R0__B0__C9_C0_32_bit_die;
const int * const address_map_B2__rows__B1_B0__columns = address_map_B2__R13_R0__B1_B0__C9_C0_32_bit_die;
const int * const address_map_rows__B2_B0__columns = address_map_R13_R0__B2_B0__C9_C0_32_bit_die;

// ddr controller MR registers were set to RL14/WL8. PHY was set up RL16, WL8.  nWR used to be set to 12
// now everything recalculated for 933MHz
static const struct burst_and_latency lpddr3_burst_and_latency = {
    .BL = 8,
    .RL = 14,
    .WL = 8,
    .nWR = 14, // RU(tWR/tCK)  (15ns / tCK = 13.995)
};

static volatile int efuseValueReadOnBoot=-1;

void DrvDdrArchInitialiseInternalConfig(struct internalDdrConfig *internalConfig)
{
    internalConfig->lpddr_version = 3;
    internalConfig->chip_size_gigabits = 4;
    internalConfig->odt_pin_is_connected = 1;
    // rrmode needs to be '1' for the LPDDR3 driver speeds, however vcs didn't like it in the past (some commands were issued twice).
    // TODO: need to check if this is still true, and if yes, then investigate.
    // TODO: any reason why not to use rrmode=1 for lpddr2?
    internalConfig->use_rrmode = 1;
    internalConfig->burst_and_latency = &lpddr3_burst_and_latency;
}

int DrvDdrReadSubstrateVersionFromEfuse( void )
{
    tySystemClockSettings previousSystemClockSettings;
    int restorePreviousSystemClock = 0;
    if (DrvCprGetClockFreqKhz(REFCLK0, NULL) == 0)
    {
        DrvCprInit();
    }
    if (DrvCprGetSysClockKhz() < 200000)
    {
        // Note, currently in efuse driver minimum clk frequency is 100MHz, but here we picked a higer value to make sure we're not too much on the edge
        DrvCprSaveAndSetSystemClock(&previousSystemClockSettings, 200000);
        restorePreviousSystemClock = 1;
    }

    efuseSet edata;
    int rc = DrvEfuseReadEFuses(&edata, APPLY_FIX, NORMAL_MODE);
    assert(rc == 0);
    int retval = 4;
    if (DrvEfuseGetValue1(&edata, 543))
    {
        retval = 6;
    }

    if (restorePreviousSystemClock)
    {
        DrvCprRestoreSystemClock(&previousSystemClockSettings);
    }

    return retval;
}

void DrvDdrGetDefaultConfigurationNoAutoDetect(tyDdrConfig * outputDdrConfig, const struct autoDetectConfig * autoDetectConfig)
{
    const tyDdrConfig *selectedDdrConfig = &defaultDdrConfigMA245x_4L;
    switch (autoDetectConfig -> forceSubstrateVersion)
    {
        case FORCE_SUBSTRATE_VERSION_4L:
            selectedDdrConfig = &defaultDdrConfigMA245x_4L;
            break;
        case FORCE_SUBSTRATE_VERSION_6L:
            selectedDdrConfig = &defaultDdrConfigMA245x_6L;
            break;
        case AUTO_DETECT_SUBSTRATE_VERSION:
            {
                //Avoid reading EFUSE values each time we go here.
                //This is because if our application uses low power
                //states triggering, let's say, every second, that would
                //effectively mean a chip would get bricked in 25 days of staying
                //turned on. The EFUSE reading is guaranteed to about 2 million reads
                volatile int efuseValue;
                if (efuseValueReadOnBoot==-1){
                    efuseValue=DrvDdrReadSubstrateVersionFromEfuse();
                    efuseValueReadOnBoot=efuseValue;
                }else{
                    efuseValue=efuseValueReadOnBoot;
                }
                switch (efuseValue)
                {
                    case 4:
                        selectedDdrConfig = &defaultDdrConfigMA245x_4L;
                        break;
                    case 6:
                        selectedDdrConfig = &defaultDdrConfigMA245x_6L;
                        break;
                    default:
                        assert(0);
                }
            }
            break;
        default:
            assert(0);
    }
    memcpy(outputDdrConfig, selectedDdrConfig, sizeof(defaultDdrConfigMA245x_4L));
}

///@}
