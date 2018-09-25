///
/// @file DrvDdrArchMa245xMa2150Compat.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvDdrArchMa245xMa2150Compat
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

const tyDdrConfig defaultDdrConfigMA245x_215x_compat =
{
    .version = 7,
    .dieDriveStrength = LPDDR3_DIE_DRIVE_48_OHM,
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
    .acTweakPercent = 0,
    .dqsTweakPercent = 0,
    .perBankRefresh = 1,
    .refreshBurst = REFRESH_BURST_DEFAULT,
    .averageTimeBetweenAllBankRefreshCommandsNs = DEFAULT_REFRESH_TIMING,// Note: Default refresh timing is different due to the larger chip, so while this looks like it's the same as ma2150, it's not really the same.
    .averageTimeBetweenPerBankRefreshCommandsNs = DEFAULT_REFRESH_TIMING,
    .allBankRefreshCycleTimeNs = DEFAULT_REFRESH_TIMING,
    .perBankRefreshCycleTimeNs = DEFAULT_REFRESH_TIMING,
    .en_dfi_dram_clk_disable = 1,
    .powerdown_en = 1,
    .selfref_en = 1,
    .derating_en = 0,
    .address_map = address_map_R12_R0__B2_B0__C8_C0_32_bit_die,
    .default_frequency_khz = 264000,
};

const int * const address_map_B2_B0__rows__columns = address_map_B2_B0__R12_R0__C8_C0_32_bit_die;
const int * const address_map_B2_B1__rows__B0__columns = address_map_B2_B1__R12_R0__B0__C8_C0_32_bit_die;
const int * const address_map_B2__rows__B1_B0__columns = address_map_B2__R12_R0__B1_B0__C8_C0_32_bit_die;
const int * const address_map_rows__B2_B0__columns = address_map_R12_R0__B2_B0__C8_C0_32_bit_die;

static const struct burst_and_latency lpddr3_burst_and_latency_in_ma2150_compatibility_mode = {
    .BL = 8,
    .RL = 8,
    .WL = 4,
    .nWR = 8, // RU(tWR/tCK)  (15ns / tCK = 7.995)
};

void DrvDdrArchInitialiseInternalConfig(struct internalDdrConfig *internalConfig)
{
    internalConfig->lpddr_version = 3;
    internalConfig->chip_size_gigabits = 4;
    internalConfig->odt_pin_is_connected = 1;
    internalConfig->use_rrmode = 0;
    internalConfig->burst_and_latency = &lpddr3_burst_and_latency_in_ma2150_compatibility_mode;
}

void DrvDdrGetDefaultConfigurationNoAutoDetect(tyDdrConfig * outputDdrConfig, const struct autoDetectConfig * autoDetectConfig)
{
    UNUSED(autoDetectConfig);
    memcpy(outputDdrConfig, &defaultDdrConfigMA245x_215x_compat, sizeof(defaultDdrConfigMA245x_215x_compat));
}

///@}
