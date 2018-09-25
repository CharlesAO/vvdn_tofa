///
/// @file DrvDdrArchMa2x5x.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvDdrArchMa2x5x
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
#include <assert.h>
#include <DrvCpr.h>
#include <DrvDdr.h>
#include "DrvDdrArch.h"
#include "DrvDdrAddrMapMa2x5x.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

const struct timing_register_field_description ctrl_timing_registers_arch[] = {
    { // t_dpd_x4096
        .name = DEEP_POWER_DOWN_MIN,
        .register_address = DDRC_PWRTMG_ADR,
        .field_lsb = 8,
        .field_msb = 15,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_CTRL_4096_CTL_CLOCKS,
    },
    { // t_xsr
        .name = T_XSR,
        .register_address = DDRC_DRAMTMG14_ADR,
        .field_lsb = 0,
        .field_msb = 11,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
};

const int ctrl_timing_registers_arch_size = DRVDDR_ARRAY_SIZE(ctrl_timing_registers_arch);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
void powerUpDss( void )
{
    DrvCprPowerTurnOnIsland(POWER_ISLAND_DSS);
}

static void set_address_map(const int map[])
{
    SET_REG_WORD(DDRC_ADDRMAP0_ADR, map[0]);
    SET_REG_WORD(DDRC_ADDRMAP1_ADR, map[1]);
    SET_REG_WORD(DDRC_ADDRMAP2_ADR, map[2]);
    SET_REG_WORD(DDRC_ADDRMAP3_ADR, map[3]);
    SET_REG_WORD(DDRC_ADDRMAP4_ADR, map[4]);
    SET_REG_WORD(DDRC_ADDRMAP5_ADR, map[5]);
    SET_REG_WORD(DDRC_ADDRMAP6_ADR, map[6]);
    SET_REG_WORD(DDRC_ADDRMAP7_ADR, map[7]);
    SET_REG_WORD(DDRC_ADDRMAP8_ADR, map[8]);
    SET_REG_WORD(DDRC_ADDRMAP9_ADR, map[9]);
    SET_REG_WORD(DDRC_ADDRMAP10_ADR, map[10]);
    SET_REG_WORD(DDRC_ADDRMAP11_ADR, map[11]);
}

void DrvDdrSetAddressMap(const tyDdrConfig *config)
{
    set_address_map(config->address_map);
}

int isSecondOscillatorEnabled( void )
{
    u32 reg0 = GET_REG_WORD_VAL(AON_RETENTION0_ADR);
    u32 refClk1_status = (reg0 >> 18) & 0x1;
    return refClk1_status;
}

void DrvDdrSetDataRetentionSignals(int value)
{
    DrvRegSetBitField((void *) AON_RETENTION0_ADR, 8, 1, value);
    GET_REG_WORD_VAL(AON_RETENTION0_ADR); // flush write buffer for safety
}

void DrvDdrQuasiStart( void )
{
    SET_REG_WORD(DDRC_SWCTL_ADR, 0);
}

void DrvDdrQuasiEnd( void )
{
    SET_REG_WORD(DDRC_SWCTL_ADR, 1);
    while (!(GET_REG_WORD_VAL(DDRC_SWSTAT_ADR) & 1));
}

static int cfg_clk_to_pgwait(int clkKhz) {
    if (clkKhz <= 100000)
        return 0;
    if (clkKhz <= 200000)
        return 1;
    if (clkKhz <= 267000)
        return 2;
    if (clkKhz <= 300000)
        return 3;
    if (clkKhz <= 333000)
        return 4;
    if (clkKhz <= 400000)
        return 5;
    if (clkKhz <= 467000)
        return 6;
    if (clkKhz <= 533000)
        return 7;
    assert(0);
    return 0;
}

#define ZQCR_FORCE_ZCAL_VT_UPDATE (1 << 27)

void DrvDdrSetupZqSettings(const tyDdrConfig *config)
{
    if ((config->useCalibratedDriveDqStrength == 0) &&
        (config->useCalibratedDriveAcStrength == 0) &&
        (config->useCalibratedOdtStrength == 0))
    {
        DrvRegSetBitField((void *) DDR_PHY_ZQCR, 2, 1, 1); //ZQPD=1
    }
    else
    {
        u32 apb_freq = DrvCprGetClockFreqKhz(SYS_CLK_DIV2, NULL);
        DrvRegSetBitField((void *) DDR_PHY_ZQCR, 8, 3, cfg_clk_to_pgwait(apb_freq)); //PGWAIT
        
        if ((config->calibratedDriveAcPullDownStrengthTarget != config->calibratedDriveAcPullUpStrengthTarget) ||
            (config->calibratedDriveDqPullDownStrengthTarget != config->calibratedDriveDqPullUpStrengthTarget))
        {
            DrvRegSetBitField((void *) DDR_PHY_ZQCR, 24, 1, 1); //ASYM_DRV_EN
            // enable assymetric drive strength calibration, only if requested
            assert(config->useCalibratedOdtStrength == 0); // calibrated ODT strength doesn't currently work with calibrated assymetric drive strength
        }
        
        DrvRegSetBitField((void *) DDR_PHY_ZQCR, 25, 2, config->isCalibratedOdtStrengthPullUpOnly ? 1   //DIS_NON_LIN_COMP=0, PU_ODT_ONLY=1
                                                                                                  : 2); //DIS_NON_LIN_COMP=1, PU_ODT_ONLY=0
    }
    
    DrvRegSetBitField((void *) DDR_PHY_ZQ0PR, 0, 4, config->calibratedDriveAcPullDownStrengthTarget); // this is used only in case the pull-up and pull-down values are equal
    DrvRegSetBitField((void *) DDR_PHY_ZQ0PR, 12, 4, config->calibratedDriveAcPullDownStrengthTarget);
    DrvRegSetBitField((void *) DDR_PHY_ZQ0PR, 8, 4, config->calibratedDriveAcPullUpStrengthTarget);
    DrvRegSetBitField((void *) DDR_PHY_ZQ1PR, 0, 4, config->calibratedDriveDqPullDownStrengthTarget); // this is used only in case the pull-up and pull-down values are equal
    DrvRegSetBitField((void *) DDR_PHY_ZQ1PR, 12, 4, config->calibratedDriveDqPullDownStrengthTarget);
    DrvRegSetBitField((void *) DDR_PHY_ZQ1PR, 8, 4, config->calibratedDriveDqPullUpStrengthTarget);
    DrvRegSetBitField((void *) DDR_PHY_ZQ1PR, 4, 4, config->calibratedOdtStrengthTarget);
    DrvRegSetBitField((void *) DDR_PHY_ZQ1PR, 16, 4, config->calibratedOdtStrengthTarget);

    DrvRegSetBitField((void *) DDR_PHY_ZQ0PR, 4, 4, config->calibratedOdtStrengthTarget);
    DrvRegSetBitField((void *) DDR_PHY_ZQ0PR, 16, 4, config->calibratedOdtStrengthTarget);

    // AC ODT is always disabled
    DrvRegSetBitField((void *) DDR_PHY_ZQ0DR, 30, 1, 1);
    DrvRegSetBitField((void *) DDR_PHY_ZQ0DR, 14, 27 - 14 + 1, 0);

    if (config->useCalibratedOdtStrength == 0)
    {
        DrvRegSetBitField((void *) DDR_PHY_ZQ1DR, 30, 1, 1);
        DrvRegSetBitField((void *) DDR_PHY_ZQ1DR, 14, 27 - 14 + 1, config->manualOdtPullDownStrength | (config->manualOdtPullUpStrength << 7));
    }

    if (config->useCalibratedDriveAcStrength == 0)
    {
        DrvRegSetBitField((void *) DDR_PHY_ZQ0DR, 31, 1, 1);
        DrvRegSetBitField((void *) DDR_PHY_ZQ0DR, 0, 14, config->manualDriveAcPullDownStrength | (config->manualDriveAcPullUpStrength << 7));
    }

    if (config->useCalibratedDriveDqStrength == 0)
    {
        DrvRegSetBitField((void *) DDR_PHY_ZQ1DR, 31, 1, 1);
        DrvRegSetBitField((void *) DDR_PHY_ZQ1DR, 0, 14, config->manualDriveDqPullDownStrength | (config->manualDriveDqPullUpStrength << 7));
    }
}

void DrvDdrForceZqCalibrationVtUpdateIfPossible( void )
{
    u32 zqcr = GET_REG_WORD_VAL(DDR_PHY_ZQCR);
    SET_REG_WORD(DDR_PHY_ZQCR, zqcr | ZQCR_FORCE_ZCAL_VT_UPDATE);
    SET_REG_WORD(DDR_PHY_ZQCR, zqcr & ~(ZQCR_FORCE_ZCAL_VT_UPDATE));
}

void DrvDdrSetupManualZQCalibrationSettingsOnlyWeakPulldownOutputOnAC( void )
{
    u32 zqcr = GET_REG_WORD_VAL(DDR_PHY_ZQCR);
    zqcr |= 4 /*ZQPD=1*/ | 2 /*TERM_OFF=1*/;
    u32 manualSegmentSettingAC = 1 | (1 << 31) | (1 << 30); // Set pull-down output impedance to highest possible. Disable the remaining pull-up/pull-downs
    u32 manualSegmentSettingDX = (1 << 31) | (1 << 30); // ZIOH is all zero (all pull-down/pull-ups disabled)
    SET_REG_WORD(DDR_PHY_ZQCR, zqcr);
    SET_REG_WORD(DDR_PHY_ZQ0DR, manualSegmentSettingAC);
    SET_REG_WORD(DDR_PHY_ZQ1DR, manualSegmentSettingDX);
    DrvDdrForceZqCalibrationVtUpdateIfPossible();
}

void DrvDdrAllowDdrControllerToContinue(int allow)
{
    DrvDdrQuasiStart();
    SET_REG_WORD(DDRC_DFIMISC_ADR, allow ? 1 : 0);
    DrvDdrQuasiEnd();
}

void DrvDdrConfigureClockSignals( void )
{
    // ck[0] toggling with normal polarity. Otherwise disabled.
    SET_REG_WORD(DDR_PHY_PGCR3,0x00020060);
}

void DrvDdrPhyStartInitializingAfterResetDeassert( void )
{
    SET_REG_WORD(DDR_WR_PHY_SMODE, 0); // quick fix, TODO: need to review how to correctly start phy initialization if this is not called
    GET_REG_WORD_VAL(DDR_WR_PHY_SMODE);
}

u32 DrvDdrRecalculateRefreshCycleTimeForFreqRatio12(u32 value)
{
    return (value + 1) / 2; // Divide by 2, rounding up.
}

// MR Read is only supported by MA2x5x
// Nothing is using these for now, left here for convenience:

// return all byte lanes
u64 DrvDdrMrReadRaw(u32 reg)
{
    if (reg >= 256) {
        assert(0); // Reading this register is not supported by the controller
    }
    DrvRegSetBitField((void *)(DDRC_MRCTRL1_ADR), 0, 7 - 0 + 1, 0);
    DrvRegSetBitField((void *)(DDRC_MRCTRL1_ADR), 8, 15 - 8 + 1, reg);

    DrvRegSetBitField((void *)(DDRC_MRCTRL0_ADR), 31, 1, 0);
    DrvRegSetBitField((void *)(DDRC_MRCTRL0_ADR), 0, 1, 1);
    DrvRegSetBitField((void *)(DDRC_MRCTRL0_ADR), 31, 1, 1);
    while (GET_REG_WORD_VAL(DDRC_MRSTAT_ADR) & 1)
    {
        // supposed to be empty
    }
    return (((u64)GET_REG_WORD_VAL(DDR_RD_MRR1_DATA)) << 32) | GET_REG_WORD_VAL(DDR_RD_MRR0_DATA);
}

// Double check that we initialized the we used the correct MV_SOC_REV setting for this chip
void DrvDdrCheckDdrType(const tyDdrConfig *currentConfig, const struct internalDdrConfig *internalConfig)
{
    UNUSED(currentConfig);

    u32 mr5 = DrvDdrMrRead(5);
    // mr5 contains the Manufacturer ID. Double-check first that we were able to read it successfully
    UNUSED(mr5); // assert() might not be enabled
    assert(mr5 == 3); // Elpida/Micron is the manufacturer ID.

    u32 mr8 = DrvDdrMrRead(8);
    UNUSED(mr8); // assert() might not be enabled
    // Check Density, ddr type, and IO width
    switch (internalConfig->lpddr_version)
    {
        case 2:
            assert(mr8 == 0x10);
            break;
        case 3:
            assert(mr8 == 0x1b);
            break;
        default:
            assert(0);
    }
}

int DrvDdrArePhyInitiatedDfiUpdatesSupported()
{
    if (GET_REG_WORD_VAL(CPR_VERSION_ID_ADR) == 0x3D41221D)
    {
        return 0;
    }
    return 1;
}

///@}
