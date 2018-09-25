///
/// @file DrvDdrArchMa215x.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvDdrArchMa215x
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

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

const tyDdrConfig defaultDdrConfigMA215x =
{
    .version = 7,
    .dieDriveStrength = LPDDR2_DIE_DRIVE_80_OHM,
    .dieOdtStrength = ODT_DISABLED,
    .useCalibratedDriveDqStrength = 0,
    .useCalibratedDriveAcStrength = 0,
    .useCalibratedOdtStrength = 0,
    .manualDriveDqPullDownStrength = 0x37,
    .manualDriveDqPullUpStrength = 0x37,
    .manualDriveAcPullDownStrength = 0x37,
    .manualDriveAcPullUpStrength = 0x37,
    .manualOdtPullDownStrength = 0x0,
    .manualOdtPullUpStrength = 0x0,
    .calibratedDriveDqPullDownStrengthTarget = 0xb,
    .calibratedDriveDqPullUpStrengthTarget = 0xb,
    .calibratedDriveAcPullDownStrengthTarget = 0xb,
    .calibratedDriveAcPullUpStrengthTarget = 0xb,
    .calibratedOdtStrengthTarget = 0x2,
    .isCalibratedOdtStrengthPullUpOnly = 0,
    .acTweakPercent = 33,
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
    .address_map = address_map_R12_R0__B2_B0__C8_C0_32_bit_die,
#ifdef OVERRIDE_DEFAULT_FREQUENCY_KHZ_4L
    .default_frequency_khz = OVERRIDE_DEFAULT_FREQUENCY_KHZ_4L,
#else
    .default_frequency_khz = 528000 / 2,
#endif
};

const int * const address_map_B2_B0__rows__columns = address_map_B2_B0__R12_R0__C8_C0_32_bit_die;
const int * const address_map_B2_B1__rows__B0__columns = address_map_B2_B1__R12_R0__B0__C8_C0_32_bit_die;
const int * const address_map_B2__rows__B1_B0__columns = address_map_B2__R12_R0__B1_B0__C8_C0_32_bit_die;
const int * const address_map_rows__B2_B0__columns = address_map_R12_R0__B2_B0__C8_C0_32_bit_die;

static const struct burst_and_latency lpddr2_burst_and_latency = {
    .BL = 8,
    .RL = 8,
    .WL = 4,
    .nWR = 8, // RU(tWR/tCK)  (15ns / tCK = 7.995)
};

void DrvDdrArchInitialiseInternalConfig(struct internalDdrConfig *internalConfig)
{
    internalConfig->lpddr_version = 2;
    internalConfig->chip_size_gigabits = 1;
    internalConfig->odt_pin_is_connected = 0;
    internalConfig->use_rrmode = 0;
    internalConfig->burst_and_latency = &lpddr2_burst_and_latency;
}

void DrvDdrGetDefaultConfigurationNoAutoDetect(tyDdrConfig * outputDdrConfig, const struct autoDetectConfig * autoDetectConfig)
{
    UNUSED(autoDetectConfig);
    memcpy(outputDdrConfig, &defaultDdrConfigMA215x, sizeof(defaultDdrConfigMA215x));
}

///@}
