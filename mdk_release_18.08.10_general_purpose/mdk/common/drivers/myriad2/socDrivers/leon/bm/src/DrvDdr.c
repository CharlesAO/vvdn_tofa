///
/// @file DrvDdr.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvDdr
/// @{
/// @brief     Driver for DDR Controller and DDR Phy
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <registersMyriad.h>
#include <DrvRegUtils.h>
#include <DrvTimer.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <DrvCpr.h>
#include <stdlib.h>
#include "../src/DrvDdrArch.h"
#include <DrvDdr.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

int ddrControllerClockKhz = 0;
int phyAPBClockKhz = 0;

 /* % safety margin on all timings */
#define MARGIN_PERCENT 10
#define MARGIN_UP(value) (((value) * (100 + MARGIN_PERCENT)) / 100)
#define MARGIN_DOWN(value) (((value) * (100 - MARGIN_PERCENT)) / 100)

// These will eventually be moved either into the internal, or public configuration structure
// for now disable ca training as it causes trouble
#define use_ca_training 0
#define phy_initializes_ddr 0
#define enable_phy_initiated_dfi_updates 1

static const struct ddr_timing_constraint lpddr2_and_3_constraints_ctrl_only[] = {
    {   .name = DEEP_POWER_DOWN_MIN,
        .value = MARGIN_UP(500),
        .unit = UNIT_US,
    },
    {
        .name = T_RAS_MAX,
        .value = MARGIN_DOWN(70),
        .unit = UNIT_US,
    },
    {
        .name = T_RP_PB,
        .value = MARGIN_UP(18),
        .unit = UNIT_NS,
        .value2 = 3,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_CKESR,
        .value = MARGIN_UP(15),
        .unit = UNIT_NS,
        .value2 = 3,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = IDLE_TIME_AFTER_RESET,
        .value = MARGIN_UP(1000),
        .unit = UNIT_NS,
    },
    {
        .name = T_WR,
        .value = MARGIN_UP(15),
        .unit = UNIT_NS,
        .value2 = 3,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_ZQCL,
        .value = MARGIN_UP(360),
        .unit = UNIT_NS,
        .value2 = 6,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_ZQCS,
        .value = MARGIN_UP(90),
        .unit = UNIT_NS,
        .value2 = 6,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_ZQRESET,
        .value = MARGIN_UP(50),
        .unit = UNIT_NS,
        .value2 = 3,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_XP_USED_FOR_T_CKCSX,
        .value = MARGIN_UP(7500),
        .unit = UNIT_PS,
        .value2 = 2,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_DQSCK_MAX_FOR_RD2WR,
        .value = MARGIN_UP(5500),
        .unit = UNIT_PS,
    },
};

static const struct ddr_timing_constraint lpddr2_and_3_constraints_phy_only[] = {
    {
        .name = T_RP_AB,
        .value = MARGIN_UP(18),
        .unit = UNIT_NS,
        .value2 = 3,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    { // MEMC_TRAINING is not enabled
        .name = T_WLMRD,
        .value = MARGIN_UP(40),
        .unit = UNIT_NS,
    },
};

static const struct ddr_timing_constraint lpddr2_constraints_ctrl_and_phy[] = {
    {
        .name = T_RTP,
        .value = MARGIN_UP(7500),
        .unit = UNIT_PS,
        .value2 = 2,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_WTR,
        .value = MARGIN_UP(7500),
        .unit = UNIT_PS,
        .value2 = 2,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
};

static const struct ddr_timing_constraint lpddr3_constraints_ctrl_and_phy[] = {
    {
        .name = T_RTP,
        .value = MARGIN_UP(7500),
        .unit = UNIT_PS,
        .value2 = 4,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_WTR,
        .value = MARGIN_UP(7500),
        .unit = UNIT_PS,
        .value2 = 4,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
};

static const struct ddr_timing_constraint chip_size_gigabits_ge_6_ctrl_and_phy[] = {
    {
        .name = T_XSR,
        .value = MARGIN_UP(10 + 210),
        .unit = UNIT_NS,
        .value2 = 2,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
};

static const struct ddr_timing_constraint chip_size_gigabits_lt_6_ctrl_and_phy[] = {
    {
        .name = T_XSR,
        .value = MARGIN_UP(10 + 130),
        .unit = UNIT_NS,
        .value2 = 2,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
};

static const struct ddr_timing_constraint lpddr2_and_3_constraints_ctrl_and_phy[] = {
    {
        .name = T_FAW,
        .value = MARGIN_UP(50),
        .unit = UNIT_NS,
        .value2 = 8,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RAS_MIN,
        .value = MARGIN_UP(42),
        .unit = UNIT_NS,
        .value2 = 3,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RRD,
        .value = MARGIN_UP(10),
        .unit = UNIT_NS,
        .value2 = 2,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RCD,
        .value = MARGIN_UP(18),
        .unit = UNIT_NS,
        .value2 = 3,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_XP,
        .value = MARGIN_UP(7500),
        .unit = UNIT_PS,
        .value2 = 2,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RC,
        .value = MARGIN_UP(42 + 18),
        .unit = UNIT_NS,
        .value2 = 3 + 3,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = MAX_T_CKE_T_CKESR,
        .value = MARGIN_UP(15),
        .unit = UNIT_NS,
        .value2 = 3,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    { // ctrl: only used for lpddr3 phy: used for lpddr2/lpddr3
        .name = T_MRD,
        .value = MARGIN_UP(14),
        .unit = UNIT_NS,
        .value2 = 10,
        .unit2 = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = CKE_LOW_TIME_WITH_POWER_AND_CLOCK_STABLE,
        .value = MARGIN_UP(100),
        .unit = UNIT_NS,
    },
    {
        .name = CKE_HIGH_TIME_TO_FIRST_COMMAND,
        .value = MARGIN_UP(200),
        .unit = UNIT_US,
    },
    {
        .name = TIME_FROM_RESET_TO_END_OF_AUTO_INIT,
        .value = MARGIN_UP(11),
        .unit = UNIT_US,
    },
    {
        .name = TIME_FROM_ZQ_INIT_TO_FIRST_COMMAND,
        .value = MARGIN_UP(1000),
        .unit = UNIT_NS,
    },
};

const struct timing_register_field_description ctrl_timing_registers_common[] = {
    {
        .name = T_RAS_MIN,
        .register_address = DDRC_DRAMTMG0_ADR,
        .field_lsb = 0,
        .field_msb = 5,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_DIV_2, // NOTE: explicitly checked with synopsys that this is okay.
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RAS_MAX,
        .register_address = DDRC_DRAMTMG0_ADR,
        .field_lsb = 8,
        .field_msb = 14,
        .rounding = DDR_ROUND_DOWN,
        .adjust = DDR_ADJUST_MINUS_1_DIV_2,
        .specified_unit = UNIT_PHY_1024_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_FAW,
        .register_address = DDRC_DRAMTMG0_ADR,
        .field_lsb = 16,
        .field_msb = 21,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_WR,
        .register_address = DDRC_DRAMTMG0_ADR,
        .field_lsb = 24,
        .field_msb = 30,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_BLD2_PLUS_WL_PLUS_1_PLUS_1_D2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RC,
        .register_address = DDRC_DRAMTMG1_ADR,
        .field_lsb = 0,
        .field_msb = 6,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RTP,
        .register_address = DDRC_DRAMTMG1_ADR,
        .field_lsb = 8,
        .field_msb = 12,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_BLD2_MINUS_VAR_BASED_ON_TECH_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_XP,
        .register_address = DDRC_DRAMTMG1_ADR,
        .field_lsb = 16,
        .field_msb = 20,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_WTR,
        .register_address = DDRC_DRAMTMG2_ADR,
        .field_lsb = 0,
        .field_msb = 5,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_BLD2_PLUS_WL_PLUS_1_PLUS_1_D2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_DQSCK_MAX_FOR_RD2WR,
        .register_address = DDRC_DRAMTMG2_ADR,
        .field_lsb = 8,
        .field_msb = 13,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_BLD2_PLUS_RL_PLUS_1_MINUS_WL_PLUS_1_D2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_MRD,
        .register_address = DDRC_DRAMTMG3_ADR,
        .field_lsb = 12,
        .field_msb = 17,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RP_PB,
        .register_address = DDRC_DRAMTMG4_ADR,
        .field_lsb = 0,
        .field_msb = 4,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_D2_RD_PLUS_1,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RRD,
        .register_address = DDRC_DRAMTMG4_ADR,
        .field_lsb = 8,
        .field_msb = 11,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RCD,
        .register_address = DDRC_DRAMTMG4_ADR,
        .field_lsb = 24,
        .field_msb = 28,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = MAX_T_CKE_T_CKESR,
        .register_address = DDRC_DRAMTMG5_ADR,
        .field_lsb = 0,
        .field_msb = 4,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_CKESR,
        .register_address = DDRC_DRAMTMG5_ADR,
        .field_lsb = 8,
        .field_msb = 13,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_XP_USED_FOR_T_CKCSX,
        .register_address = DDRC_DRAMTMG6_ADR,
        .field_lsb = 0,
        .field_msb = 3,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_2_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    { // controller calls it pre_cke_x1024
        .name = CKE_LOW_TIME_WITH_POWER_AND_CLOCK_STABLE,
        .register_address = DDRC_INIT0_ADR,
        .field_lsb = 0,
        .field_msb = 11,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_1024_DDR_OUTPUT_CLOCKS,
    },
    { // controller calls it post_cke_x1024
        .name = CKE_HIGH_TIME_TO_FIRST_COMMAND,
        .register_address = DDRC_INIT0_ADR,
        .field_lsb = 16,
        .field_msb = 25,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_1024_DDR_OUTPUT_CLOCKS,
    },
    { // controller calls it max_auto_init_x1024
        .name = TIME_FROM_RESET_TO_END_OF_AUTO_INIT,
        .register_address = DDRC_INIT5_ADR,
        .field_lsb = 0,
        .field_msb = 9,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_1024_DDR_OUTPUT_CLOCKS,
    },
    { // controller calls it dev_zqinit_x32
        .name = TIME_FROM_ZQ_INIT_TO_FIRST_COMMAND,
        .register_address = DDRC_INIT5_ADR,
        .field_lsb = 16,
        .field_msb = 23,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_32_DDR_OUTPUT_CLOCKS,
    },
    { // controllers calls it idle_after_reset_x32 // TODO: check adjust formula for this, and other INIT register fields.
        .name = IDLE_TIME_AFTER_RESET,
        .register_address = DDRC_INIT2_ADR,
        .field_lsb = 8,
        .field_msb = 15,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_32_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_ZQCL,
        .register_address = DDRC_ZQCTL0_ADR,
        .field_lsb = 16,
        .field_msb = 26,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_ZQCS,
        .register_address = DDRC_ZQCTL0_ADR,
        .field_lsb = 0,
        .field_msb = 9,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_ZQRESET,
        .register_address = DDRC_ZQCTL1_ADR,
        .field_lsb = 20,
        .field_msb = 29,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_ADJUST_PLUS_1_DIV_2,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
};

const struct timing_register_field_description phy_timing_registers_common[] = {
    {
        .name = T_RTP,
        .register_address = DDR_PHY_DTPR0,
        .field_lsb = 0,
        .field_msb = 3,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_WTR,
        .register_address = DDR_PHY_DTPR0,
        .field_lsb = 4,
        .field_msb = 7,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RP_AB,
        .register_address = DDR_PHY_DTPR0,
        .field_lsb = 8,
        .field_msb = 12,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RAS_MIN,
        .register_address = DDR_PHY_DTPR0,
        .field_lsb = 16,
        .field_msb = 21,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RRD,
        .register_address = DDR_PHY_DTPR0,
        .field_lsb = 22,
        .field_msb = 25,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RCD,
        .register_address = DDR_PHY_DTPR0,
        .field_lsb = 26,
        .field_msb = 30,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_FAW,
        .register_address = DDR_PHY_DTPR1,
        .field_lsb = 5,
        .field_msb = 10,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_WLMRD,
        .register_address = DDR_PHY_DTPR1,
        .field_lsb = 20,
        .field_msb = 25,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_XSR,
        .register_address = DDR_PHY_DTPR2,
        .field_lsb = 0,
        .field_msb = 9,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_XP,
        .register_address = DDR_PHY_DTPR2,
        .field_lsb = 10,
        .field_msb = 14,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_RC,
        .register_address = DDR_PHY_DTPR3,
        .field_lsb = 6,
        .field_msb = 12,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = MAX_T_CKE_T_CKESR,
        .register_address = DDR_PHY_DTPR3,
        .field_lsb = 13,
        .field_msb = 17,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    {
        .name = T_MRD,
        .register_address = DDR_PHY_DTPR3,
        .field_lsb = 18,
        .field_msb = 22,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    { // called tDINIT0 in the phy
        .name = CKE_HIGH_TIME_TO_FIRST_COMMAND,
        .register_address = DDR_PHY_PTR3,
        .field_lsb = 0,
        .field_msb = 19,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    { // called tDINIT1 in the phy
        .name = CKE_LOW_TIME_WITH_POWER_AND_CLOCK_STABLE,
        .register_address = DDR_PHY_PTR3,
        .field_lsb = 20,
        .field_msb = 28,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    { // called tDINIT2 in the phy
        .name = TIME_FROM_RESET_TO_END_OF_AUTO_INIT,
        .register_address = DDR_PHY_PTR4,
        .field_lsb = 0,
        .field_msb = 17,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
    { // called tDINIT3 in the phy
        .name = TIME_FROM_ZQ_INIT_TO_FIRST_COMMAND,
        .register_address = DDR_PHY_PTR4,
        .field_lsb = 18,
        .field_msb = 28,
        .rounding = DDR_ROUND_UP,
        .adjust = DDR_DO_NOT_ADJUST,
        .specified_unit = UNIT_PHY_DDR_OUTPUT_CLOCKS,
    },
};

static u32 get_mr1_value(const struct burst_and_latency *burst_and_latency, const struct internalDdrConfig *internalConfig)
{
    u32 val = 0;
    if (internalConfig->lpddr_version == 3)
    {
        assert(burst_and_latency->BL==8);
    }
    switch(burst_and_latency->BL)
    {
        case 4: val |= 2; break;
        case 8: val |= 3; break;
        case 16: val |= 4; break;
        default: assert(0);
    }
    if (internalConfig->lpddr_version == 3)
    {
        assert((burst_and_latency->nWR != 4) &&
               (burst_and_latency->nWR != 5) &&
               (burst_and_latency->nWR != 7));
    }
    else
    {
        assert(burst_and_latency->nWR < 9);
    }

    switch(burst_and_latency->nWR)
    {
        case 10:
            val |= 0 << 5; break;
        case 3:
        case 11:
            val |= 1 << 5; break;
        case 12:
            val |= 2 << 5; break;
        case 6:
        case 14:
            val |= 4 << 5; break;
        case 8:
        case 16:
            val |= 6 << 5; break;
        case 9:
            val |= 7 << 5; break;
        default:
            assert(0);
    }
    return val;
}

static u32 get_mr2_value(const struct burst_and_latency *burst_and_latency, const struct internalDdrConfig *internalConfig)
{
    u32 val = 0;
    if (internalConfig->lpddr_version == 3)
    {
        switch (burst_and_latency->RL)
        {
            case 3:
                val |= 1;
                assert(burst_and_latency->WL == 1);
                break;
            case 6:
                val |= 4;
                assert(burst_and_latency->WL == 3);
                break;
            case 8:
                val |= 6;
                assert(burst_and_latency->WL == 4);
                break;
            case 9:
                val |= 7;
                assert(burst_and_latency->WL == 5);
                break;
            case 10:
                val |= 8;
                if (burst_and_latency->WL != 6)
                {
                    assert(burst_and_latency->WL == 8);
                    val |= 1 << 6; // WL Set B
                }
                break;
            case 11:
                val |= 9;
                if (burst_and_latency->WL != 6)
                {
                    assert(burst_and_latency->WL == 9);
                    val |= 1 << 6; // WL Set B
                }
                break;
            case 12:
                val |= 10;
                if (burst_and_latency->WL != 6)
                {
                    assert(burst_and_latency->WL == 9);
                    val |= 1 << 6; // WL Set B
                }
                break;
            case 14:
                val |= 12;
                if (burst_and_latency->WL != 8)
                {
                    assert(burst_and_latency->WL == 11);
                    val |= 1 << 6; // WL Set B
                }
                break;
            case 16:
                val |= 14;
                if (burst_and_latency->WL != 8)
                {
                    assert(burst_and_latency->WL == 13);
                    val |= 1 << 6; // WL Set B
                }
                break;
            default:
                assert(0);
        }
        if (burst_and_latency->nWR < 9)
        {
            val |= 1 << 4; // Enable nWRE programming > 9
        }
    }
    else
    {
        if (burst_and_latency->RL == 3 && burst_and_latency->WL == 1)
        {
            val |= 1;
        }
        else if (burst_and_latency->RL == 4 && burst_and_latency->WL == 2)
        {
            val |= 2;
        }
        else if (burst_and_latency->RL == 5 && burst_and_latency->WL == 2)
        {
            val |= 3;
        }
        else if (burst_and_latency->RL == 6 && burst_and_latency->WL == 3)
        {
            val |= 4;
        }
        else if (burst_and_latency->RL == 7 && burst_and_latency->WL == 4)
        {
            val |= 5;
        }
        else if (burst_and_latency->RL == 8 && burst_and_latency->WL == 4)
        {
            val |= 6;
        }
        else
        {
            assert(0);
        }
    }
    return val;
}

// 6: Functions Implementation
// ----------------------------------------------------------------------------

u32 DrvDdrConvertUnit(u32 inValue, enum ddr_timing_unit inUnit, enum ddr_timing_unit outUnit, enum ddr_rounding roundUp)
{
    if (inUnit == outUnit)
    {
         return inValue;
    }
    long long multiplier = 1;
    long long divider = 1;
    switch (outUnit)
    {
    // NOTE: some of the units that are not used are left commented out to lower code size.
/*        case UNIT_PHY_HIGH_SPEED_CLOCKS:
            multiplier = 4 * ddrControllerClockKhz;
            break;*/
        case UNIT_PHY_DDR_OUTPUT_CLOCKS:
            multiplier = 2 * ddrControllerClockKhz;
            break;
        case UNIT_PHY_32_DDR_OUTPUT_CLOCKS:
            multiplier = 2 * ddrControllerClockKhz;
            divider = 32;
            break;
        case UNIT_PHY_1024_DDR_OUTPUT_CLOCKS:
            multiplier = 2 * ddrControllerClockKhz;
            divider = 1024;
            break;
        case UNIT_PHY_4096_DDR_OUTPUT_CLOCKS:
            multiplier = 2 * ddrControllerClockKhz;
            divider = 4096;
            break;
/*        case UNIT_PHY_CORE_CLOCKS:
            multiplier = ddrControllerClockKhz;
            break;
        case UNIT_PHY_APB_CLOCKS:
            multiplier = phyAPBClockKhz;
            break;*/
        case UNIT_CTRL_1024_CTL_CLOCKS:
            multiplier = ddrControllerClockKhz;
            divider = 1024;
            break;
        case UNIT_CTRL_4096_CTL_CLOCKS:
            multiplier = ddrControllerClockKhz;
            divider = 4096;
            break;
        case UNIT_CTRL_32_CTL_CLOCKS:
            multiplier = ddrControllerClockKhz;
            divider = 32;
            break;
/*     case UNIT_CTRL_CTL_CLOCKS:
            multiplier = ddrControllerClockKhz;
            break;
        case UNIT_PS:
            multiplier *= 1000 * 1000 * 1000;
            break;
        case UNIT_NS:
            multiplier *= 1000 * 1000;
            break;
        case UNIT_US:
            multiplier *= 1000;
            break;
        case UNIT_MS:
            break;*/
        default:
            printf("out: %d\n", outUnit);
            assert(0);
    }
    assert(multiplier != 0); // most likely will fail in case of initialization error if the driver can't determine the frequency it's running at
    switch (inUnit)
    {
/*        case UNIT_PHY_HIGH_SPEED_CLOCKS:
            divider *= 4 * ddrControllerClockKhz;
            break;*/
        case UNIT_PHY_DDR_OUTPUT_CLOCKS:
            divider *= 2 * ddrControllerClockKhz;
            break;
/*        case UNIT_PHY_1024_DDR_OUTPUT_CLOCKS:
            divider *= 2 * ddrControllerClockKhz;
            multiplier *= 1024;
            break;
        case UNIT_PHY_4096_DDR_OUTPUT_CLOCKS:
            divider *= 2 * ddrControllerClockKhz;
            multiplier *= 4096;
            break;
        case UNIT_PHY_CORE_CLOCKS:
            divider *= ddrControllerClockKhz;
            break;
        case UNIT_PHY_APB_CLOCKS:
            divider *= phyAPBClockKhz;
            break;
        case UNIT_CTRL_1024_CTL_CLOCKS:
            divider *= ddrControllerClockKhz;
            multiplier *= 1024;
            break;
        case UNIT_CTRL_4096_CTL_CLOCKS:
            divider *= ddrControllerClockKhz;
            multiplier *= 4096;
            break;
        case UNIT_CTRL_32_CTL_CLOCKS:
            divider *= ddrControllerClockKhz;
            multiplier *= 32;
            break;
        case UNIT_CTRL_CTL_CLOCKS:
            divider *= ddrControllerClockKhz;
            break;*/
        case UNIT_PS:
            divider *= 1000 * 1000 * 1000;
            break;
        case UNIT_NS:
            divider *= 1000 * 1000;
            break;
        case UNIT_US:
            divider *= 1000;
            break;
/*        case UNIT_MS:
            divider *= 1;
            break;*/
        default:
            assert(0);
    }
    long long outValue = multiplier * inValue;
    if (roundUp) {
        outValue = outValue + divider - 1;
    }
    outValue = outValue / divider;
    assert(outValue < (1LL << 32));

    return (u32)outValue;
}

static u32 RU_ps_div_tCK(u32 ps)
{
    return DrvDdrConvertUnit(ps, UNIT_PS, UNIT_PHY_DDR_OUTPUT_CLOCKS, 1);
}


void DrvDdrApplyTiming(const struct ddr_timing_constraint *constraint,
                          const struct timing_register_field_description *field,
                          const struct internalDdrConfig *internalConfig)
{
    u32 raw = DrvDdrConvertUnit(constraint->value, constraint->unit, field->specified_unit, field->rounding);
    if (constraint->unit2 != UNIT_NULL)
    {
        u32 raw2 = DrvDdrConvertUnit(constraint->value2, constraint->unit2, field->specified_unit, field->rounding);
        if (raw2 > raw)
        {
            raw = raw2;
        }
    }
    switch (field->adjust)
    {
        case DDR_DO_NOT_ADJUST:
            break;
        case DDR_ADJUST_MINUS_1_DIV_2:
            raw = (raw - 1) / 2;
            break;
        case DDR_ADJUST_PLUS_1_DIV_2:
            raw = (raw + 1) / 2;
            break;
        case DDR_ADJUST_PLUS_BLD2_MINUS_VAR_BASED_ON_TECH_DIV_2:
            raw = raw + internalConfig->burst_and_latency->BL / 2;
            if (internalConfig->lpddr_version == 2)
            {
                raw -= 2;
            }
            else
            {
                raw -= 4;
            }
            raw /= 2;
            break;
        case DDR_ADJUST_PLUS_BLD2_PLUS_WL_PLUS_1_PLUS_1_D2:
            raw = ( raw + internalConfig->burst_and_latency->BL / 2 + internalConfig->burst_and_latency->WL + 1 + 1 ) / 2;
            break;
        case DDR_ADJUST_D2_RD_PLUS_1:
            raw = raw / 2 + 1;
            break;
        case DDR_ADJUST_PLUS_2_PLUS_1_DIV_2:
            raw = (raw + 1) / 2;
            break;
        case DDR_ADJUST_PLUS_BLD2_PLUS_RL_PLUS_1_MINUS_WL_PLUS_1_D2:
            raw = ( raw + internalConfig->burst_and_latency->BL / 2 + internalConfig->burst_and_latency->RL - internalConfig->burst_and_latency->WL + 1 + 1 ) / 2;
            break;
        case DDR_ADJUST_DIV_2:
            raw = raw / 2;
            break;
        default:
            assert(0); // unknown adjustment requested
    }
    s32 field_size = field->field_msb - field->field_lsb + 1;
    assert(((u64)raw) < (u64)(1 << field_size));
    DrvRegSetBitField((void*)field->register_address, field->field_lsb, field_size, raw);
}

void DrvDdrApplyTimings(const struct ddr_timing_constraint *constraints, int constraint_count,
                          const struct timing_register_field_description *fields, int field_count,
                          const struct timing_register_field_description *fields2, int field_count2,
                          const struct internalDdrConfig *internalConfig)
{
    int i;
    for (i=0;i<constraint_count;i++)
    {
        int j, applied = 0;
        for (j=0;j<field_count;j++)
        {
            if (constraints[i].name == fields[j].name)
            {
                DrvDdrApplyTiming(&constraints[i], &fields[j], internalConfig);
                applied = 1;
                break;
            }
        }
        if (!applied)
        {
            for (j=0;j<field_count2;j++)
            {
                if (constraints[i].name == fields2[j].name)
                {
                    DrvDdrApplyTiming(&constraints[i], &fields2[j], internalConfig);
                    applied = 1;
                    break;
                }
            }
        }
        if (!applied)
        {
            assert(0);
        }
    }
}

static int isClockSourceAlreadyConfigured( void )
{
    if (!(GET_REG_WORD_VAL(CPR_AUX_CLK_EN_ADR) & (1 << CSS_AUX_DDR_CORE)))
    {
        return 0;
    }

    u32 ddrAuxClockConfig = GET_REG_WORD_VAL(CPR_AUX0_CLK_CTRL_ADR + 4 * CSS_AUX_DDR_CORE);
    tyCprClockSrc ddrAuxClockSource = (tyCprClockSrc) (ddrAuxClockConfig >> 28);

    if (ddrAuxClockSource == (tyCprClockSrc)CPR_PLL0)
    {
        if (((GET_REG_WORD_VAL(CPR_PLL_STAT0_ADR) & 3) == 3) && // update complete && locked
            ((GET_REG_WORD_VAL(CPR_PLL_CTRL0_ADR) & 3) == 0) && // not powered off or bypassed
            ((GET_REG_WORD_VAL(CPR_CLK_BYPASS_ADR) & 1) == 0)) // not master-bypassed
        {
            return 1;
        }
    }
    if (ddrAuxClockSource == (tyCprClockSrc)CPR_PLL1)
    {
        if (((GET_REG_WORD_VAL(CPR_PLL_STAT1_ADR) & 3) == 3) && // update complete && locked
            ((GET_REG_WORD_VAL(CPR_PLL_CTRL1_ADR) & 3) == 0) && // not powered off or bypassed
            ((GET_REG_WORD_VAL(CPR_CLK_BYPASS_ADR) & 2) == 0)) // not master-bypassed
        {
            if (GET_REG_WORD_VAL(CPR_PLL_CTRL1_ADR) & (1 << 18)) // is using refclk1??
            {
                return isSecondOscillatorEnabled();
            }
            return 1;
        }
    }
    return 0;
}

static int getDdrControllerClockKhz( void )
{
    tyClockConfig clkCfg;
    return DrvCprGetClockFreqKhz(AUX_CLK_DDR_CORE_CTRL, &clkCfg);
}

// It is recommended to configure the DDR aux clock using the CPR driver, in which case this function will not be called.
static int setupDdrControllerClockDefaultMHzUsePll1UseRefclk0(const tyDdrConfig *config, const struct internalDdrConfig *internalConfig)
{
    UNUSED(internalConfig);

    #ifndef OVERRIDE_DEFAULT_DDR_CLOCK_FREQUENCY_KHZ
        u32 freqKhz = config->default_frequency_khz;
    #else
        UNUSED(config);
        u32 freqKhz = (OVERRIDE_DEFAULT_DDR_CLOCK_FREQUENCY_KHZ);
    #endif
    if (DrvCprGetClockFreqKhz(REFCLK0, NULL) == 0)
    {
        DrvCprInit();
    }
    DrvCprSetupJustPll(CPR_PLL1, CLK_SRC_REFCLK0, freqKhz);
    DrvCprAuxClocksEnable(AUX_CLK_MASK_DDR_CORE_CTRL, CLK_SRC_PLL1, 1, 1);
    return freqKhz;
}

static int maybeSetupAndGetDdrControllerClockKhz(const tyDdrConfig *config, const struct internalDdrConfig *internalConfig)
{
    if (isClockSourceAlreadyConfigured())
    {
        return getDdrControllerClockKhz();
    }
    else
    {
        return setupDdrControllerClockDefaultMHzUsePll1UseRefclk0(config, internalConfig);
    }
}

static void setupClocksAndPulseAutodeassertingResets(const tyDdrConfig *config, const struct internalDdrConfig *internalConfig)
//common for V1/V2 but it sets global variables for module use, so kept in the same module with all arch dependent functions
{
    ddrControllerClockKhz = maybeSetupAndGetDdrControllerClockKhz(config, internalConfig);
    tyClockConfig clkCfg;
    phyAPBClockKhz = DrvCprGetClockFreqKhz(SYS_CLK_DIV2, &clkCfg);
    DrvCprSysDeviceAction(CSS_DOMAIN, ENABLE_CLKS, DEV_CSS_DSS_APB | DEV_CSS_DSS_BUS);
    DrvCprSysDeviceAction(CSS_DOMAIN, PULSE_RESET, DEV_CSS_DSS_APB_RST_PHY | DEV_CSS_DSS_APB_RST_CTRL | DEV_CSS_DSS_BUS_MAHB | DEV_CSS_DSS_BUS_DXI | DEV_CSS_DSS_BUS_AAXI | DEV_CSS_DSS_BUS_MXI);
}

static void DrvDdrDisableODTForDmPin(int lane)
{
    u32 gcr0_regaddr = DDR_PHY_DX0GCR0 + (DDR_PHY_DX1GCR0 - DDR_PHY_DX0GCR0) * lane;
    u32 gcr3_regaddr = gcr0_regaddr + DDR_PHY_DX0GCR3 - DDR_PHY_DX0GCR0;
    DrvRegSetBitField((void *)(gcr3_regaddr), 12, 2, 2); // DMTEMODE - ODT always off
}

static void ddrPhyWaitForIDONE( void )
{
    DrvTimerSleepTicks(100);
    while ((GET_REG_WORD_VAL(DDR_PHY_PGSR0) & 1) != 1);
    DrvTimerSleepTicks(300); // datasheet required 32 ctl_clk ticks waiting time.
    // @ worst case 166MHz ctl_clk, 800MHz system clock: 154 system ticks should be sufficient
}

static void ddrPhyTriggerInitialisationStepThenWaitForDone(u32 pir_value)
{
    SET_REG_WORD(DDR_PHY_PIR, pir_value);
    ddrPhyWaitForIDONE();
}

static enum ddrc_operating_mode ddrCtrlGetOperatingMode(const struct internalDdrConfig *internalConfig)
{
    u32 ddrc_stat = GET_REG_WORD_VAL(DDRC_STAT_ADR);
    u32 operating_mode = ddrc_stat & 7;
    if (internalConfig->lpddr_version == 2)
    {
        operating_mode = ddrc_stat & 3;
    }
    return (enum ddrc_operating_mode)operating_mode;
}


static int ddrCtrlIsStillInitialising(const struct internalDdrConfig *internalConfig)
{
    return ddrCtrlGetOperatingMode(internalConfig) == DDR_INIT;
}

static void ddrCtrlWaitForInitialisationToComplete(const struct internalDdrConfig *internalConfig)
{
    do {
        DrvTimerSleepTicks(5000);
    } while (ddrCtrlIsStillInitialising(internalConfig));
}

static u32 get_dtpr3_tdqsck( void )
{
    u32 T_DQSCKmin_PS = 2500;
    u32 tdqsck_raw = DrvDdrConvertUnit(T_DQSCKmin_PS, UNIT_PS, UNIT_PHY_DDR_OUTPUT_CLOCKS, DDR_ROUND_DOWN);
    #ifdef GATE_TRAINING_ENABLED
    // TODO implement correctly
    tdqsck_raw++;
    #endif
    return tdqsck_raw;
}

static int are_phy_initiated_dfi_updates_enabled( void )
{
    if (enable_phy_initiated_dfi_updates)
    {
        return DrvDdrArePhyInitiatedDfiUpdatesSupported();
    }
    return 0;
}

// Configure Dfi updates that are requested or serviced by the DDR controller.
void configureDdrCtrlDfiUpdates(const tyDdrConfig *config)
{
    UNUSED(config);
    // Disable generation of controller-initiated DFI updates
    u32 upd0 = GET_REG_WORD_VAL(DDRC_DFIUPD0_ADR);
    SET_REG_WORD(DDRC_DFIUPD0_ADR, upd0 | DDR_CTRL_DFIUPD0_DIS_AUTO_CTRLUPD | DDR_CTRL_DFIUPD0_DIS_AUTO_CTRLUPD_SRX);
    // Disable/Enable the acknowledging of PHY-initiated updates
    DrvRegSetBitField((void *)(DDRC_DFIUPD2_ADR), 31, 1, are_phy_initiated_dfi_updates_enabled());
}

static void configureDdrCtrlRefresh(const tyDdrConfig *config, const struct internalDdrConfig *internalConfig)
{
    u32 refreshBurst = config->refreshBurst;
    if (refreshBurst == REFRESH_BURST_DEFAULT) {
        refreshBurst = REFRESH_BURST_2; // burst of 2 refresh by default
    }
    u32 refreshBurstRaw = refreshBurst - 1;
    u32 refresh_margin = 2;  // Refresh margin = 2 (x32) - recommended unchanged from default
    u32 refresh_to_x32 = 16; // Allow speculative refresh after 16 (x32) clock cycles
                             // (Note that speculative refresh timeout must be < power-down timeout
                             // speculative refresh to be possible before entering power-down)

    // ELPIDA_LPDDR_1Gb_533MHz_1Die / Micron_LPDDR_1Gb_533MHz_1Die
    // --------------------------------------------------------------------
    SET_REG_WORD(DDRC_RFSHCTL0_ADR, (config->perBankRefresh ? 4 : 0) |
                                    (refreshBurstRaw << 4) |
                                    (refresh_margin << 20) |
                                    (refresh_to_x32 << 12));

    SET_REG_WORD(DDRC_RFSHCTL1_ADR, 0x00000000);// Rank 0 and 1 refresh timer start values - unused (only 1 rank)
    SET_REG_WORD(DDRC_RFSHCTL3_ADR, 0x00000000);// Auto-refresh enabled (not disabled)

    u32 averageTimeBetweenRefreshCommandsNs = config->perBankRefresh ? config->averageTimeBetweenPerBankRefreshCommandsNs : config->averageTimeBetweenAllBankRefreshCommandsNs;
    u32 bankRefreshCycleTimeNs = config->perBankRefresh ? config->perBankRefreshCycleTimeNs : config->allBankRefreshCycleTimeNs;

    if (averageTimeBetweenRefreshCommandsNs == DEFAULT_REFRESH_TIMING) {
        if (internalConfig->chip_size_gigabits == 1)
        {
            averageTimeBetweenRefreshCommandsNs = config->perBankRefresh ? 975 : 7800;
        }
        else
        {
            averageTimeBetweenRefreshCommandsNs = config->perBankRefresh ? 487 : 3900;
        }
    }

    if (bankRefreshCycleTimeNs == DEFAULT_REFRESH_TIMING) {
        switch (internalConfig->chip_size_gigabits)
        {
            case 6:
            case 8:
                bankRefreshCycleTimeNs = config->perBankRefresh ? 90 : 210;
                break;
            case 1:
            case 4:
                bankRefreshCycleTimeNs = config->perBankRefresh ? 60 : 130;
                break;
            default:
                assert(0);
        }
    }

    u32 t_rfc_nom_x32_raw = DrvDdrConvertUnit(averageTimeBetweenRefreshCommandsNs, UNIT_NS, UNIT_CTRL_32_CTL_CLOCKS, DDR_ROUND_DOWN);
    u32 t_rfc_min_raw_with_memc_freq_ratio_1 = DrvDdrConvertUnit(bankRefreshCycleTimeNs, UNIT_NS, UNIT_PHY_DDR_OUTPUT_CLOCKS, DDR_ROUND_UP);

    assert((t_rfc_nom_x32_raw>refresh_margin) && "t_rfc_nom_x32 must be bigger then refresh_margin");
    // When derating is enabled, the above rule must also be respected
    if (config -> derating_en)
    {
        assert(((t_rfc_nom_x32_raw >> 2) > refresh_margin ) && "When derating is enabled, (t_rfc_nom_x32 >> 2) must be strictly greater then refresh_margin ");
    }

    u32 t_rfc_min_raw = DrvDdrRecalculateRefreshCycleTimeForFreqRatio12(t_rfc_min_raw_with_memc_freq_ratio_1);

    assert((t_rfc_nom_x32_raw < (1 << 12)) && "t_rfx_nom_x32 does not fit fields size");
    assert((t_rfc_min_raw < (1 << 9)) && "t_rfc_min_raw does not fit field size");
    assert((t_rfc_nom_x32_raw * 32 > t_rfc_min_raw) && "t_rfc_nom_x32 * 32 must be bigger then t_rfc_min_raw");

    SET_REG_WORD(DDRC_RFSHTMG_ADR, (t_rfc_nom_x32_raw << 16) |
                                    t_rfc_min_raw);
}

void configureDdrPhyRefresh(const tyDdrConfig *config, const struct internalDdrConfig *internalConfig)
{
    u32 averageTimeBetweenRefreshCommandsNs = config->averageTimeBetweenAllBankRefreshCommandsNs;

    if (averageTimeBetweenRefreshCommandsNs == DEFAULT_REFRESH_TIMING) {
        if (internalConfig->lpddr_version == 3)
        {
            averageTimeBetweenRefreshCommandsNs = 3900;
        }
        else
        {
            averageTimeBetweenRefreshCommandsNs = 7800;
        }
    }

    u32 DTPR1_tRFC = DrvDdrConvertUnit(averageTimeBetweenRefreshCommandsNs, UNIT_NS, UNIT_PHY_DDR_OUTPUT_CLOCKS, DDR_ROUND_DOWN);

    DrvRegSetBitField((void *)(DDR_PHY_DTPR1), 11, 19-11+1, DTPR1_tRFC);
}

// Set up ports, scheduling, and everything that is independent of the type of memory that is being used.
void configureDdrCtrlBusSide(const tyDdrConfig *config)
{
    UNUSED(config);
    DrvDdrQuasiStart(); // this is needed on MA2x5x because of Bug 26333

    // Transaction queue configuration
    // ------------------------------------------------------------------------
    // There are 3 transaction queues in the DDR controller:
    //
    //   - LPR (low priority read)
    //   - HPR (high priority read)
    //   - W   (write)
    //
    // However, HPR queue is not used (priorities are set by AXI port side-band signals
    // which are tied-off in our implementation) so all reads go to the LPR queue.
    SET_REG_WORD(DDRC_SCHED_ADR, 0x10001f04);  // Read/write idle gap = 16
                                               // Reserve all (32) FIFO entries for LPR queue (no HPR queue)
                                               // Intelligent precharges enabled (pageclose bit set)
    SET_REG_WORD(DDRC_SCHED1_ADR, 0x00000010); // pageclose_timer = 16

//  SET_REG_WORD(DDRC_PERFHPR1_ADR, 0x0f0000ff); // No HPR queue
    SET_REG_WORD(DDRC_PERFLPR1_ADR, 0x18000080); // lpr_xact_run_length = 24
                                                 // lpr_max_starve = 128 (number of cycles queue may be starved before going critical)
    SET_REG_WORD(DDRC_PERFWR1_ADR, 0x100000a0);  // w_xact_run_length = 16
                                                 // w_max_starve = 160
//  SET_REG_WORD(DDRC_PERFVPR1_ADR, 0x00000000); // No variable priority transactions

    SET_REG_WORD(DDRC_DBG0_ADR, 0x00000000);
    SET_REG_WORD(DDRC_DBG1_ADR, 0x00000000);
//  SET_REG_WORD(DDRC_DBGCMD_ADR, 0x00000000);
    SET_REG_WORD(DDRC_PCCFG_ADR, 0x00000010); // Page match four limit enabled

    // Port configuration
    // ------------------------------------------------------------------------
    //   - QoS registers don't have to be programmed as all (AXI) ports have QoS
    //     side-band signals tied-off = 0
    //   - AXI urgent side-band signals are also tied off
    //   - Ports are prioritized via the aging feature when a port has a request
    //     pending a timeout count down starts (programmable 10 bit initial value)
    //   - If a ports aging timeout counter reaches 0 its request becomes the highest
    //     priority, timeout counters are reset when requests are granted
    // Port 0 (Leon OS/RT 128-bit AHB slave)
    SET_REG_WORD(DDRC_PCFGR_0_ADR, 0x0000524a); // Order of reads/writes to same address preserved, because it's an AHB port.
                                                // bit 16 does not have an effect here.
                                                // Read channel page match enabled
                                                // Read channel aging enabled - initial value = 586 (18)
    SET_REG_WORD(DDRC_PCFGW_0_ADR, 0x000012ea); // Write channel aging enabled - initial value = 746 (23)

    // Port 1 (SHAVE L2 cache 128-bit AXI slave)
    SET_REG_WORD(DDRC_PCFGR_1_ADR, 0x00011195); // Order of reads/writes to same address preserved
                                                // Read channel aging enabled - initial value = 405 (12)
    SET_REG_WORD(DDRC_PCFGW_1_ADR, 0x00001205); // Write channel againg enabled - initial value = 517 (16)

    // Port 2 (AMC 64-bit AXI slave)
    SET_REG_WORD(DDRC_PCFGR_2_ADR, 0x0001420d); // Order of reads/writes to same address preserved (paranoia?)
                                                // Read channel page match enabled
                                                // Read channel aging disabled - highest priority
    SET_REG_WORD(DDRC_PCFGW_2_ADR, 0x0000104a); // Write channel aging enabled - initial value = 74 (2)

    // Port 3 (MXI 128-bit AXI slave - CIFs, LCD, NAL, CMXDMA)
    SET_REG_WORD(DDRC_PCFGR_3_ADR, 0x0000018e); // (Order of reads and writes cannot be configured to be preserved for this bus)
                                                // Read channel aging disabled - highest priority
    SET_REG_WORD(DDRC_PCFGW_3_ADR, 0x0000101d); // Write channel aging enabled - initial value = 29 (0)

    #if 0
    // Note temporarly disabling these SET_REG_WORDS, to fix only one issue at a time.
    // These SET_REG_WORDs used to take effect in MA2100.
    // Then later, because of the introduction of SWCTL, these stopped taking effect (potentially causing a performance regression)
    // some PCFGR fields also stopped being written correctly, causing Bug 26333.
    // In the first pull request, I'm only fixing the transaction ordering issue in Bug 26333
    // then I will add another PR to remove the #if 0 from here.

    // Port 3 supports 4 virtual channels in its read reorder buffer
    // These must be statically allocated by AXI ID to MXI bus masters
    // All IDs not allocated to a specific channel map to channel 0
    SET_REG_WORD(DDRC_PCFGIDMASKCH0_3_ADR, 0x00000000);
    SET_REG_WORD(DDRC_PCFGIDVALUECH0_3_ADR, 0x00000000);
    // Map CMXDMA lanes 0 and 2 to channel 1
    SET_REG_WORD(DDRC_PCFGIDMASKCH1_3_ADR, 0x0000007d);
    SET_REG_WORD(DDRC_PCFGIDVALUECH1_3_ADR, (MXI_UPA_MST << 4) | (CFG_NUM_SHAVES << 2) | 0x0);
    // Map CMXDMA lanes 1 and 3 to channel 2
    SET_REG_WORD(DDRC_PCFGIDMASKCH2_3_ADR, 0x0000007d);
    SET_REG_WORD(DDRC_PCFGIDVALUECH2_3_ADR, (MXI_UPA_MST << 4) | (CFG_NUM_SHAVES << 2) | 0x1);
    // Map LCD controller to channel 3
    SET_REG_WORD(DDRC_PCFGIDMASKCH3_3_ADR, 0x00000070);
    SET_REG_WORD(DDRC_PCFGIDVALUECH3_3_ADR, (MXI_LCD_MST << 4));
    #endif
    // DrvDdrQuasiEnd(); // Note: QuasiEnd doesn't seem to work here, because SWSTAT.sw_done_ack does not seem to go high when the ddr controller is not yet active.
}

void configureDdrCtrl(int start_up_in_self_refresh, const tyDdrConfig *config, const struct internalDdrConfig *internalConfig)
{
    // Reset everything in controller(bit 18 and 13)

    DrvCprSysDeviceAction(CSS_DOMAIN, PULSE_RESET, DEV_CSS_DSS_APB_RST_PHY | DEV_CSS_DSS_APB_RST_CTRL);

    // Leave DDR core clock reset (Aux clock 20), and DDR PHY core clock (21)
    DrvCprSysDeviceAction(CSS_AUX_DOMAIN, ASSERT_RESET, AUX_CLK_MASK_DDR_CORE_CTRL | AUX_CLK_MASK_DDR_CORE_PHY);

    DrvTimerSleepTicks(100);

    SET_REG_WORD(DDRC_DBG1_ADR, 0x00000001); // Disable de-queuing of anything from CAM

    u32 MSTR_burst_rdwr = 0;
    switch (internalConfig->burst_and_latency->BL)
    {
        case 2:  MSTR_burst_rdwr = 1; break;
        case 4:  MSTR_burst_rdwr = 2; break;
        case 8:  MSTR_burst_rdwr = 4; break;
        case 16: MSTR_burst_rdwr = 8; break;
        default: assert(0);
    }

    SET_REG_WORD(DDRC_MSTR_ADR,
        ((internalConfig->lpddr_version == 3) ?
            DDR_CTRL_MSTR_SELECT_LPDDR3 :
            DDR_CTRL_MSTR_SELECT_LPDDR2) |
        (MSTR_burst_rdwr << 16) |
        DDR_CTRL_MSTR_DATA_BUS_WIDTH_HALF |
        DDR_CTRL_MSTR_RANK_0_PRESENT
    );

    if (config->derating_en)
    {
        u32 value = 1 | (1 << 4);// Derating enabled, byte lane is "1", derate value is "+1"
        if (ddrControllerClockKhz > 533333) {
            value |= 1 << 1; // potentially increase derate value
        }
        SET_REG_WORD(DDRC_DERATEEN_ADR, value);
    }
    else
    {
        SET_REG_WORD(DDRC_DERATEEN_ADR, 0); // Derating disabled for now
    }

    // Configuration of power saving features
    // ------------------------------------------------------------------------
    u32 pwrctl = (config->en_dfi_dram_clk_disable ? (1 << 3) : 0) |
                 (config->selfref_en ? (1 << 0) : 0) |
                 (config->powerdown_en ? (1 << 1) : 0);
    SET_REG_WORD(DDRC_PWRCTL_ADR, pwrctl);

    u32 powerdown_to_x32 = 31; // Enter power-down after 24 (x32) idle clock cycles @264MHz
    u32 selfref_to_x32 = 255; // Enter self-refresh after 240 (x32) idle clock cycles @264MHz
    u32 t_dpd_x4096 = 80; // Minimum deep power-down of 80, but will be overwritten by set_ctrl_timings(), later.
    assert((powerdown_to_x32 >> 5) == 0);
    assert((selfref_to_x32 >> 8) == 0);
    assert((t_dpd_x4096 >> 8) == 0);
    SET_REG_WORD(DDRC_PWRTMG_ADR, (selfref_to_x32 << 16) |
                                  (t_dpd_x4096 << 8) |
                                  (powerdown_to_x32 << 0));

    SET_REG_WORD(DDRC_HWLPCTL_ADR, 0); // We don't use the hardware low power interface

    configureDdrCtrlRefresh(config, internalConfig);

    SET_REG_WORD(DDRC_INIT3_ADR, (get_mr1_value(internalConfig->burst_and_latency, internalConfig) << 16) | get_mr2_value(internalConfig->burst_and_latency, internalConfig));
    SET_REG_WORD(DDRC_INIT4_ADR, config->dieDriveStrength << 16);

    u32 DRAMTMG2_read_latency = (internalConfig->burst_and_latency -> RL + 1) / 2; // Note: this field used to be incorrect according to docs. Now it's fixed. The field doesn't seem to have any effect.
    u32 DRAMTMG2_write_latency = (internalConfig->burst_and_latency -> WL + 1) / 2;

    DrvRegSetBitField((void *)(DDRC_DRAMTMG2_ADR), 16, 21 - 16 + 1, DRAMTMG2_read_latency);
    DrvRegSetBitField((void *)(DDRC_DRAMTMG2_ADR), 24, 29 - 24 + 1, DRAMTMG2_write_latency);

    DrvRegSetBitField((void *)(DDRC_ZQCTL0_ADR), 30, 1, 1); // dis_srx_zqcl // Disable issuing of ZQCL command at Self-Refresh exit.
    DrvRegSetBitField((void *)(DDRC_ZQCTL1_ADR), 0, 19 - 0 + 1, 0x70); // ZQ calibration inverval
        //^^ currently corresponts to 215 uS @ 533MHz, or 122us @933 Mhz
        // (also doc doesn't mention MEMC_FREQ_RATIO, so the above might be wrong. TODO: check in sims.)
        // TODO: calculate based on frequency

    if (internalConfig->lpddr_version == 3)
    {
        DrvRegSetBitField((void *)(DDRC_DRAMTMG4_ADR), 16, 19 - 16 + 1, 2); // t_ccd
    }
    else
    {
        DrvRegSetBitField((void *)(DDRC_DRAMTMG4_ADR), 16, 19 - 16 + 1, 1); // t_ccd
    }

    u32 dfi_t_ctrl_delay = internalConfig->use_rrmode ? 3 : 2;
    DrvRegSetBitField((void *)(DDRC_DFITMG0_ADR), 24, 28 - 24 + 1, dfi_t_ctrl_delay);
    u32 dfi_rddata_use_sdr = 0;
    DrvRegSetBitField((void *)(DDRC_DFITMG0_ADR), 23, 1, dfi_rddata_use_sdr);
    u32 dfi_wrdata_use_sdr = 0;
    DrvRegSetBitField((void *)(DDRC_DFITMG0_ADR), 15, 1, dfi_wrdata_use_sdr);
    u32 dfi_tphy_wrdata = 0x01;
    DrvRegSetBitField((void *)(DDRC_DFITMG0_ADR), 8, 13 - 8 +1, dfi_tphy_wrdata);

    u32 wl_as_seen_by_phy_for_dfi_timing = internalConfig->burst_and_latency->WL + 1;
    u32 rl_as_seen_by_phy_for_dfi_timing = internalConfig->burst_and_latency->RL + get_dtpr3_tdqsck();
    u32 dfi_tphy_wrlat_tck = ((wl_as_seen_by_phy_for_dfi_timing % 2) == 0) ?
                               wl_as_seen_by_phy_for_dfi_timing - 4 :
                               wl_as_seen_by_phy_for_dfi_timing - 3;
    u32 dfi_tphy_wrlat_raw = (dfi_tphy_wrlat_tck + 1) / 2;
    DrvRegSetBitField((void *)(DDRC_DFITMG0_ADR), 0, 5 - 0 + 1, dfi_tphy_wrlat_raw);

    u32 dfi_t_rddata_en_tck = ((rl_as_seen_by_phy_for_dfi_timing % 2) == 0) ?
                                rl_as_seen_by_phy_for_dfi_timing - 4 :
                                rl_as_seen_by_phy_for_dfi_timing - 3;
    u32 dfi_t_rddata_en_raw = (dfi_t_rddata_en_tck + 1) / 2;
    DrvRegSetBitField((void *)(DDRC_DFITMG0_ADR), 16, 21 - 16 + 1, dfi_t_rddata_en_raw);

    // TODO: review the dfi_t_wrdata_delay setting. Why does it need to be different for lpddr3? should this be WL-dependent?
    if (internalConfig->lpddr_version == 3)
    {
        u32 dfi_t_wrdata_delay = 0x7;
        DrvRegSetBitField((void *)(DDRC_DFITMG1_ADR), 16, 20 - 16 + 1, dfi_t_wrdata_delay);
    }
    else
    {
        u32 dfi_t_wrdata_delay = 0x3;
        DrvRegSetBitField((void *)(DDRC_DFITMG1_ADR), 16, 20 - 16 + 1, dfi_t_wrdata_delay);
    }
    u32 dfi_t_dram_clk_disable = 0x02;
    DrvRegSetBitField((void *)(DDRC_DFITMG1_ADR), 8, 11 - 8 + 1, dfi_t_dram_clk_disable);
    u32 dfi_t_dram_clk_enable = internalConfig->use_rrmode ? 3 : 2;
    DrvRegSetBitField((void *)(DDRC_DFITMG1_ADR), 0, 3 - 0 + 1, dfi_t_dram_clk_enable);

    SET_REG_WORD(DDRC_DFILPCFG0_ADR, 0x07917030);

    DrvRegSetBitField((void *)(DDRC_DRAMTMG5_ADR), 24, 27 - 24 + 1, 2); // t_cksrx (recommended value for LPDDR2/3)
    DrvRegSetBitField((void *)(DDRC_DRAMTMG5_ADR), 16, 19 - 16 + 1, 2); // t_cksre (recommended value for LPDDR2/3)

    configureDdrCtrlDfiUpdates(config);

    DrvDdrSetAddressMap(config);


    if (internalConfig->odt_pin_is_connected && (config->dieOdtStrength != ODT_DISABLED))
    {
        #define T_ODT_ON_MAX_PS 3500
        u32 wr_odt_hold = 7 + RU_ps_div_tCK(T_ODT_ON_MAX_PS);
        int wr_odt_delay = internalConfig->burst_and_latency -> WL - 1 - RU_ps_div_tCK(T_ODT_ON_MAX_PS);
        if (wr_odt_delay < 0)
        {
            wr_odt_delay = 0;
        }
        u32 rd_odt_hold = 4; // rd odt is disabled. leave reset value here.
        u32 rd_odt_delay = 0;
        SET_REG_WORD(DDRC_ODTCFG_ADR, (wr_odt_hold << 24) |
                                      (wr_odt_delay << 16) |
                                      (rd_odt_hold << 8) |
                                      (rd_odt_delay << 0));
        SET_REG_WORD(DDRC_ODTMAP_ADR, 0x00000001); // enable ODT only for rank 0 write
    }
    else
    {
        SET_REG_WORD(DDRC_ODTMAP_ADR, 0x00000000); // disable ODT for lpddr2
    }

    DrvDdrApplyTimings(lpddr2_and_3_constraints_ctrl_only, DRVDDR_ARRAY_SIZE(lpddr2_and_3_constraints_ctrl_only),
        ctrl_timing_registers_arch, ctrl_timing_registers_arch_size,
        ctrl_timing_registers_common, DRVDDR_ARRAY_SIZE(ctrl_timing_registers_common), internalConfig);
    DrvDdrApplyTimings(lpddr2_and_3_constraints_ctrl_and_phy, DRVDDR_ARRAY_SIZE(lpddr2_and_3_constraints_ctrl_and_phy),
        ctrl_timing_registers_arch, ctrl_timing_registers_arch_size,
        ctrl_timing_registers_common, DRVDDR_ARRAY_SIZE(ctrl_timing_registers_common), internalConfig);

    if (internalConfig->lpddr_version == 3)
    {
        DrvDdrApplyTimings(lpddr3_constraints_ctrl_and_phy, DRVDDR_ARRAY_SIZE(lpddr3_constraints_ctrl_and_phy),
            ctrl_timing_registers_arch, ctrl_timing_registers_arch_size,
            ctrl_timing_registers_common, DRVDDR_ARRAY_SIZE(ctrl_timing_registers_common), internalConfig);
    }
    else
    {
        DrvDdrApplyTimings(lpddr2_constraints_ctrl_and_phy, DRVDDR_ARRAY_SIZE(lpddr2_constraints_ctrl_and_phy),
            ctrl_timing_registers_arch, ctrl_timing_registers_arch_size,
            ctrl_timing_registers_common, DRVDDR_ARRAY_SIZE(ctrl_timing_registers_common), internalConfig);
    }

    switch(internalConfig->chip_size_gigabits)
    {
        case 1:
        case 4:
            DrvDdrApplyTimings(chip_size_gigabits_lt_6_ctrl_and_phy, DRVDDR_ARRAY_SIZE(chip_size_gigabits_lt_6_ctrl_and_phy),
                ctrl_timing_registers_arch, ctrl_timing_registers_arch_size,
                ctrl_timing_registers_common, DRVDDR_ARRAY_SIZE(ctrl_timing_registers_common), internalConfig);
            break;
        case 6:
        case 8:
            DrvDdrApplyTimings(chip_size_gigabits_ge_6_ctrl_and_phy, DRVDDR_ARRAY_SIZE(chip_size_gigabits_ge_6_ctrl_and_phy),
                ctrl_timing_registers_arch, ctrl_timing_registers_arch_size,
                ctrl_timing_registers_common, DRVDDR_ARRAY_SIZE(ctrl_timing_registers_common), internalConfig);
            break;
        default:
            assert(0);
    }

    if (start_up_in_self_refresh)
    {
        DrvRegSetBitField((void *)(DDRC_INIT0_ADR), 30, 31 - 30 + 1, 3); // dram starts up in self refresh mode with dram init skipped.
    }
    if (phy_initializes_ddr)
    {
        DrvRegSetBitField((void *)(DDRC_INIT0_ADR), 30, 31 - 30 + 1, 1); // dram starts up in normal mode with dram init skipped.
    }

    if ((IS_PLATFORM_VCS) && !(FORCE_USE_CORRECT_TIMING_IN_SIMS)) {
        // CKE high to start init incorrect value, chosen for faster sims.
        DrvRegSetBitField((void *)(DDRC_INIT0_ADR), 16, 25 - 16 + 1, 1);
    }

    configureDdrCtrlBusSide(config);

    // Enable all ports
    SET_REG_WORD(DDRC_PCTRL_0_ADR, 0x00000001);
    SET_REG_WORD(DDRC_PCTRL_1_ADR, 0x00000001);
    SET_REG_WORD(DDRC_PCTRL_2_ADR, 0x00000001);
    SET_REG_WORD(DDRC_PCTRL_3_ADR, 0x00000001);

    if (start_up_in_self_refresh)
    {
        SET_REG_WORD(DDRC_PWRCTL_ADR, pwrctl | (1 << 5));
    }

    SET_REG_WORD(DDRC_DFIMISC_ADR, 0x00000000);

    DrvCprSysDeviceAction(CSS_AUX_DOMAIN, DEASSERT_RESET, AUX_CLK_MASK_DDR_CORE_CTRL | AUX_CLK_MASK_DDR_CORE_PHY);
}

void configureDdrPhy(int start_up_in_self_refresh, const tyDdrConfig *config, const struct internalDdrConfig *internalConfig)
{
    UNUSED(start_up_in_self_refresh);

    const int use_low_speed_io = 0; // set to 1 for up to 667MHz
    const int slew_rate = 3; // 0,1,2,3 = fast constant, fast minimized crowbar, slow constant, slow minimized crowbar

    // Enable internal VREF generation
    SET_REG_WORD(DDR_PHY_IOVCR0,0x48484848);
    SET_REG_WORD(DDR_PHY_IOVCR1,0x00000048);

    DrvDdrSetupZqSettings(config);

    // Configure the PLL range
    assert(ddrControllerClockKhz >= 166000);
    if (ddrControllerClockKhz < 250000)
    {
        DrvRegSetBitField((void *)(DDR_PHY_PLLCR), 19, 20-19+1, 3);           // Range 166 - 275Mhz
    }
    else if (ddrControllerClockKhz < 465000)
    {
        DrvRegSetBitField((void *)(DDR_PHY_PLLCR), 19, 20-19+1, 1);           // Range 225 - 490Mhz
    }
    else
    {
        DrvRegSetBitField((void *)(DDR_PHY_PLLCR), 19, 20-19+1, 0);           // Range 440 - 533Mhz
    }

    DrvDdrDisableByteLanes(4, 7);

    DrvRegSetBitField((void *)(DDR_PHY_DX0GCR1),  0, 16,  0); // DXPDDMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX0GCR2), 16, 16,  0); // DXOEMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX0GCR3),  0,  2,  0); // DSPDDMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX0GCR3),  6,  2,  0); // DSOEMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX0GCR3),  8,  2,  0); // DMPDDMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX0GCR3), 14,  2,  0); // DMOEMODE

    DrvRegSetBitField((void *)(DDR_PHY_DX1GCR1),  0, 16,  0); // DXPDDMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX1GCR2), 16, 16,  0); // DXOEMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX1GCR3),  0,  2,  0); // DSPDDMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX1GCR3),  6,  2,  0); // DSOEMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX1GCR3),  8,  2,  0); // DMPDDMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX1GCR3), 14,  2,  0); // DMOEMODE

    DrvRegSetBitField((void *)(DDR_PHY_DX2GCR1),  0, 16,  0); // DXPDDMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX2GCR2), 16, 16,  0); // DXOEMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX2GCR3),  0,  2,  0); // DSPDDMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX2GCR3),  6,  2,  0); // DSOEMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX2GCR3),  8,  2,  0); // DMPDDMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX2GCR3), 14,  2,  0); // DMOEMODE

    DrvRegSetBitField((void *)(DDR_PHY_DX3GCR1),  0, 16,  0); // DXPDDMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX3GCR2), 16, 16,  0); // DXOEMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX3GCR3),  0,  2,  0); // DSPDDMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX3GCR3),  6,  2,  0); // DSOEMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX3GCR3),  8,  2,  0); // DMPDDMODE
    DrvRegSetBitField((void *)(DDR_PHY_DX3GCR3), 14,  2,  0); // DMOEMODE

    // ODT is always disabled for DM pins, since they are never inputs to the phy.
    DrvDdrDisableODTForDmPin(0);
    DrvDdrDisableODTForDmPin(1);
    DrvDdrDisableODTForDmPin(2);
    DrvDdrDisableODTForDmPin(3);

    DrvRegSetBitField((void *)(DDR_PHY_ACIOCR0), 0, 1, use_low_speed_io);
    DrvRegSetBitField((void *)(DDR_PHY_ACIOCR0), 30, 2, slew_rate);

    SET_REG_WORD(DDR_PHY_ACIOCR1, 0);
    SET_REG_WORD(DDR_PHY_ACIOCR2, 0);

    DrvRegSetBitField((void *)(DDR_PHY_ACIOCR3), 0, 8, 0xa8); // CKOEMODE = OE always off for index >= 1, dynamic for index 0
    DrvRegSetBitField((void *)(DDR_PHY_ACIOCR4), 0, 8, 0x54); // CKPDDMODE = PDD always on for index >= 1, dynamic for index 0
    DrvRegSetBitField((void *)(DDR_PHY_ACIOCR3), 8, 8, 0xa8); // CSOEMODE = OE always off for index >= 1, dynamic for index 0
    DrvRegSetBitField((void *)(DDR_PHY_ACIOCR4), 8, 8, 0x54); // CSPDDMODE = PDD always on for index >= 1, dynamic for index 0

    if (internalConfig->odt_pin_is_connected)
    {
        // even if die ODT is disabled, if the pin is physically connected, then
        // we want to drive a static value into in, to avoid a floating input
        DrvRegSetBitField((void *)(DDR_PHY_ACIOCR5), 16, 8, 0x54); // ODTPDDMODE = always on = powered down of ODT[i] where i > 0
                                                                   //            = dynamic for ODT[0]
    }
    else
    {
        // Turn off the driver of the ODT pin which is not used by LPDDR2
        DrvRegSetBitField((void *)(DDR_PHY_ACIOCR5), 16, 8, 0x55); // ODTPDDMODE = always on = powered down
    }
    // Turn off the driver for unused CKE pins
    DrvRegSetBitField((void *)(DDR_PHY_ACIOCR5), 24, 8, 0x54); // CKEPDDMODE = always on = powered down for index >=1. Dynamic for index = 0

    DrvRegSetBitField((void *)(DDR_PHY_ACIOCR5), 8, 8, 0xa8); // CKEOEMODE = OE always off for index >= 1, dynamic for index 0

    if (internalConfig->odt_pin_is_connected)
    {
        DrvRegSetBitField((void *)(DDR_PHY_ACIOCR5), 0, 8, 0xa8); // OTD OE always off for index >= 1, dynamic for index 0
    }
    else
    {
        DrvRegSetBitField((void *)(DDR_PHY_ACIOCR5), 0, 8, 0xaa); // OTD OE always off
    }

    if (internalConfig->odt_pin_is_connected && (config->dieOdtStrength != ODT_DISABLED))
    {
        SET_REG_WORD(DDR_PHY_ODTCR, 1 << 16);
    }
    else
    {
        SET_REG_WORD(DDR_PHY_ODTCR, 0);
    }

    const u32 DSGCR_DQSGX = 3; /* 11: Extend the gate earlier by 1/2tCK and later by 3 * tCK (to facilitate
                                  LPDDR2/LPDDR3 usage without training for systems supporting up to
                                  1600Mbps). DQS pulldown/DQS# pullup required and gate training not
                                  supported in this mode */

    SET_REG_WORD(DDR_PHY_DSGCR, 0x0020643e | (DSGCR_DQSGX << 6) | (internalConfig->use_rrmode << 18) | (are_phy_initiated_dfi_updates_enabled() << 0));

    SET_REG_WORD(DDR_PHY_DXCCR, 0x00381884);
    DrvRegSetBitField((void *)(DDR_PHY_DXCCR), 5, 8-5+1, 1); // 2.5k pull-down instead of the default of 620ohm
    DrvRegSetBitField((void *)(DDR_PHY_DXCCR), 9, 12-9+1, 9); // 2.5k pull-up instead of the default of 620ohm
    DrvRegSetBitField((void *)(DDR_PHY_DXCCR), 1, 1, use_low_speed_io);
    DrvRegSetBitField((void *)(DDR_PHY_DXCCR), 13, 2, slew_rate);

    DrvDdrConfigureClockSignals();

    u32 DCR_DDRMD = 0; // LPDDR2
    if (internalConfig -> lpddr_version == 3)
    {
        DCR_DDRMD = 1; // LPDDR3
    }

    u32 DCR_DDR8BNK = 1;
    u32 DCR_PDQ = 0;
    u32 DCR_MPRDQ = 1;
    u32 DCR_BYTEMASK = 0x19;
    u32 DCR_DDR2T = 0;
    SET_REG_WORD(DDR_PHY_DCR, (DCR_DDRMD    <<  0) |
                              (DCR_DDR8BNK  <<  3) |
                              (DCR_PDQ      <<  4) |
                              (DCR_MPRDQ    <<  7) |
                              (DCR_BYTEMASK << 10) |
                              (DCR_DDR2T    << 28));

    u32 averageTimeBetweenRefreshCommandsNs = 7800; // LPDDR2 default
    if (internalConfig -> lpddr_version == 3)
    {
        averageTimeBetweenRefreshCommandsNs = 3900; // LPDDR3 updated value
    }
    u32 DTCR_RFSHDT = GET_REG_WORD_VAL(DDR_PHY_DTCR) >> 28;

    u32 PGCR2_TREFPRD = DTCR_RFSHDT * DrvDdrConvertUnit(averageTimeBetweenRefreshCommandsNs, UNIT_NS, UNIT_PHY_DDR_OUTPUT_CLOCKS, DDR_ROUND_DOWN) - 400;
    u32 PGCR2_NOBUB = 1;
    u32 PGCR2_FXDLAT = 1;
    u32 PGCR2_PUBMODE = 0xf;
    SET_REG_WORD(DDR_PHY_PGCR2, (PGCR2_TREFPRD <<  0) |
                                (PGCR2_NOBUB   << 18) |
                                (PGCR2_FXDLAT  << 19) |
                                (PGCR2_PUBMODE << 20));

    SET_REG_WORD(DDR_PHY_MR1, get_mr1_value(internalConfig->burst_and_latency, internalConfig));
    SET_REG_WORD(DDR_PHY_MR2, get_mr2_value(internalConfig->burst_and_latency, internalConfig));
    SET_REG_WORD(DDR_PHY_MR3, config->dieDriveStrength << 16);

    u32 tdqsck_raw = get_dtpr3_tdqsck();
    DrvRegSetBitField((void *)(DDR_PHY_DTPR3), 0, 2 - 0 + 1, tdqsck_raw); // TDQSCK
    DrvRegSetBitField((void *)(DDR_PHY_DTPR3), 3, 5 - 3 + 1, tdqsck_raw + DSGCR_DQSGX); // tDQSCKmax

    if (internalConfig->lpddr_version == 3)
    {
        DrvRegSetBitField((void *)(DDR_PHY_DTPR1), 26, 29 - 26 + 1, 0xf); // Setting tWLO to maximum. Should be the safest setting.
    }

    if (internalConfig->odt_pin_is_connected && (config->dieOdtStrength != ODT_DISABLED))
    {
        // TODO: correctly set this up if ODT is used.
        DrvRegSetBitField((void *)(DDR_PHY_DTPR1), 30, 31 - 30 + 1, 0); // tAOND_tAOFD
        DrvRegSetBitField((void *)(DDR_PHY_DTPR3), 29, 31 - 29 + 1, 0); // tAOFDx
    }

    configureDdrPhyRefresh(config, internalConfig);

    DrvDdrApplyTimings(lpddr2_and_3_constraints_phy_only, DRVDDR_ARRAY_SIZE(lpddr2_and_3_constraints_phy_only),
        phy_timing_registers_common, DRVDDR_ARRAY_SIZE(phy_timing_registers_common),
        NULL, 0, internalConfig);
    DrvDdrApplyTimings(lpddr2_and_3_constraints_ctrl_and_phy, DRVDDR_ARRAY_SIZE(lpddr2_and_3_constraints_ctrl_and_phy),
        phy_timing_registers_common, DRVDDR_ARRAY_SIZE(phy_timing_registers_common),
        NULL, 0, internalConfig);
    if (internalConfig->lpddr_version == 3)
    {
        DrvDdrApplyTimings(lpddr3_constraints_ctrl_and_phy, DRVDDR_ARRAY_SIZE(lpddr3_constraints_ctrl_and_phy),
            phy_timing_registers_common, DRVDDR_ARRAY_SIZE(phy_timing_registers_common),
            NULL, 0, internalConfig);
    }
    else
    {
        DrvDdrApplyTimings(lpddr2_constraints_ctrl_and_phy, DRVDDR_ARRAY_SIZE(lpddr2_constraints_ctrl_and_phy),
            phy_timing_registers_common, DRVDDR_ARRAY_SIZE(phy_timing_registers_common),
            NULL, 0, internalConfig);
    }

    switch(internalConfig->chip_size_gigabits)
    {
        case 1:
        case 4:
            DrvDdrApplyTimings(chip_size_gigabits_lt_6_ctrl_and_phy, DRVDDR_ARRAY_SIZE(chip_size_gigabits_lt_6_ctrl_and_phy),
                phy_timing_registers_common, DRVDDR_ARRAY_SIZE(phy_timing_registers_common),
                NULL, 0, internalConfig);
            break;
        case 6:
        case 8:
            DrvDdrApplyTimings(chip_size_gigabits_ge_6_ctrl_and_phy, DRVDDR_ARRAY_SIZE(chip_size_gigabits_ge_6_ctrl_and_phy),
                phy_timing_registers_common, DRVDDR_ARRAY_SIZE(phy_timing_registers_common),
                NULL, 0, internalConfig);
            break;
        default:
            assert(0);
    }

    if ((IS_PLATFORM_VCS) && !(FORCE_USE_CORRECT_TIMING_IN_SIMS)) {
        // CKE high to start init incorrect value, chosen for faster sims.
        DrvRegSetBitField((void *)(DDR_PHY_PTR3), 0, 19 - 0 + 1, 1024 * 2); // The value roughly matches the "1" that is written into the DDRC_INIT0_ADR[16:25] register.
    }

    if (use_ca_training && (internalConfig->lpddr_version == 3))
    {
        u32 CAnBYTE0 = 1; // Indicates the PHY byte lane number to which DRAM byte lane 0 is connected
        u32 CAnBYTE1 = 2; // Indicates the PHY byte lane number to which DRAM byte lane 1 is connected
        u32 CAADR = 0x10;
        u32 CACD = 0x14;
        u32 CAENT = 0xa;
        u32 CAEXT = 0xa;
        u32 CACKEL = 0xa;
        u32 CACKEH = 0xa;
        u32 CAMRZ = 0xa;
        SET_REG_WORD(DDR_PHY_CATR0, (CAnBYTE0 <<  0) |
                                    (CAnBYTE1 <<  4) |
                                    (CAADR    <<  8) |
                                    (CACD     << 16));
        SET_REG_WORD(DDR_PHY_CATR1, (CAENT    <<  0) |
                                    (CAEXT    <<  4) |
                                    (CACKEL   <<  8) |
                                    (CACKEH   << 12) |
                                    (CAMRZ    << 16) |
                                    (CAnBYTE0 << 20) |
                                    (CAnBYTE1 << 24));
    }

    DrvRegSetBitField((void *)(DDR_PHY_PGCR1), 7, 8-7+1, 1); // this is IOM[2:1] on the I/O pins. We are using D3MP, so the value means "Differential Receiver mode"
                                                             // speed is selected for by other registers that connect to the IOM[0] bit
}

void DrvDdrPostDcalInitTweaks(const tyDdrConfig *config, const struct internalDdrConfig *internalConfig)
{
    UNUSED(internalConfig);
    // The following should only be pre-defined by test code
    #ifndef AC_TWEAK_PERCENT
    #define AC_TWEAK_PERCENT config->acTweakPercent
    #endif
    #ifndef DQS_TWEAK_PERCENT
    #define DQS_TWEAK_PERCENT config->dqsTweakPercent
    #endif
    DrvRegSetBitField((void *)(DDR_PHY_PGCR1), 26, 1, 1); // PGCR1.INHVT = 1
    int VSTOP;
    do {
        VSTOP = (GET_REG_WORD_VAL(DDR_PHY_PGSR1) >> 30) & 1;
    } while (!VSTOP);

    DrvDdrDecreaseLCDLFieldByPercent(DDR_PHY_ACLCDLR, 0, AC_TWEAK_PERCENT);
    if (DQS_TWEAK_PERCENT != 0)
    {
        DrvDdrDecreaseLCDLFieldByPercent(DDR_PHY_DX0LCDLR1, 8, DQS_TWEAK_PERCENT);
        DrvDdrDecreaseLCDLFieldByPercent(DDR_PHY_DX0LCDLR1, 16, DQS_TWEAK_PERCENT);
        DrvDdrDecreaseLCDLFieldByPercent(DDR_PHY_DX1LCDLR1, 8, DQS_TWEAK_PERCENT);
        DrvDdrDecreaseLCDLFieldByPercent(DDR_PHY_DX1LCDLR1, 16, DQS_TWEAK_PERCENT);
        DrvDdrDecreaseLCDLFieldByPercent(DDR_PHY_DX2LCDLR1, 8, DQS_TWEAK_PERCENT);
        DrvDdrDecreaseLCDLFieldByPercent(DDR_PHY_DX2LCDLR1, 16, DQS_TWEAK_PERCENT);
        DrvDdrDecreaseLCDLFieldByPercent(DDR_PHY_DX3LCDLR1, 8, DQS_TWEAK_PERCENT);
        DrvDdrDecreaseLCDLFieldByPercent(DDR_PHY_DX3LCDLR1, 16, DQS_TWEAK_PERCENT);
    }

    DrvRegSetBitField((void *)(DDR_PHY_PGCR1), 26, 1, 0); // PGCR1.INHVT = 0
}

void configureAndInitialiseDdrPhy(int start_up_in_self_refresh, const tyDdrConfig *config, const struct internalDdrConfig *internalConfig)
{
    DrvDdrPhyStartInitializingAfterResetDeassert();
    DrvCprSysDeviceAction(CSS_DOMAIN, PULSE_RESET, DEV_CSS_DSS_APB_RST_PHY);

    // Try to stop initialization as quickly as possible
    ddrPhyTriggerInitialisationStepThenWaitForDone(DDR_PHY_PIR_ZCALBYP | DDR_PHY_PIR_DCALBYP | DDR_PHY_PIR_LOCKBYP); // bypass certain init steps

    configureDdrPhy(start_up_in_self_refresh, config, internalConfig);

    // Ensure that the DDR I/O signals are not locked in retention mode
    DrvDdrSetDataRetentionSignals(0);

    if ((!start_up_in_self_refresh) && IS_PLATFORM_SILICON)
    {
        DrvTimerSleepMs(1); // This extra sleep is required in rare cases, after system reset, when the memory die is left in a confused state. See Bug 25856.
    }

    u32 trigger_inits = DDR_PHY_PIR_INIT | DDR_PHY_PIR_PLLINIT | DDR_PHY_PIR_DCAL;
    if (config->useCalibratedDriveDqStrength ||
        config->useCalibratedDriveAcStrength ||
        config->useCalibratedOdtStrength)
    {
        trigger_inits |= DDR_PHY_PIR_ZCAL;
    }
    ddrPhyTriggerInitialisationStepThenWaitForDone(trigger_inits);

    DrvDdrPostDcalInitTweaks(config, internalConfig);
    DrvDdrForceZqCalibrationVtUpdateIfPossible(); // NOTE: this is necessary in case automated VT updates are disabled.

    ddrPhyTriggerInitialisationStepThenWaitForDone(DDR_PHY_PIR_INIT |
                                                      (phy_initializes_ddr ?
                                                           DDR_PHY_PIR_DRAMINIT :
                                                           DDR_PHY_PIR_CTLDINIT));

    DrvDdrAllowDdrControllerToContinue(1);
    ddrCtrlWaitForInitialisationToComplete(internalConfig);

    if (use_ca_training && (internalConfig->lpddr_version == 3))
    {
        ddrPhyTriggerInitialisationStepThenWaitForDone(DDR_PHY_PIR_INIT | DDR_PHY_PIR_CA);
    }

    // Re-enable the ports
    SET_REG_WORD(DDRC_PCTRL_0_ADR, 0x00000001);
    SET_REG_WORD(DDRC_PCTRL_1_ADR, 0x00000001);
    SET_REG_WORD(DDRC_PCTRL_2_ADR, 0x00000001);
    SET_REG_WORD(DDRC_PCTRL_3_ADR, 0x00000001);
}

void DrvDdrMrWrite(u32 reg, u32 data)
{
    if (reg >= 256)
    {
        assert(0); // Writing this register is not supported by the controller
    }
    DrvRegSetBitField((void *)(DDRC_MRCTRL1_ADR), 0, 7 - 0 + 1, data);
    DrvRegSetBitField((void *)(DDRC_MRCTRL1_ADR), 8, 15 - 8 + 1, reg);

    DrvRegSetBitField((void *)(DDRC_MRCTRL0_ADR), 31, 1, 0);
    DrvRegSetBitField((void *)(DDRC_MRCTRL0_ADR), 0, 1, 0);
    DrvRegSetBitField((void *)(DDRC_MRCTRL0_ADR), 31, 1, 1);
    while (GET_REG_WORD_VAL(DDRC_MRSTAT_ADR) & 1)
    {
        // supposed to be empty
    }
}

void DrvDdrSetupDieOdt(enum die_odt_value value, enum die_odt_pd_control pd_control)
{
    DrvDdrMrWrite(11, value | (pd_control << 2));
}

void DrvDdrSetupPartialArraySelfRefreshBankMask(u32 mask)
{
    assert((mask >> 8) == 0); // mask is at most 8 bits in size
    DrvDdrMrWrite(16, mask);
}

void DrvDdrSetupPartialArraySelfRefreshSegmentMask(u32 mask)
{
    assert((mask >> 8) == 0); // mask is at most 8 bits in size
    DrvDdrMrWrite(17, mask);
}

// This is the single function you need to call to enable the DDR for a given frequency
void DrvDdrInitialise(const tyDdrConfig * targetDdrConfig)
{
    tyDdrConfig defaultDdrConfig;
    const tyDdrConfig * currentConfig;
    if (targetDdrConfig == NULL)
    {
        DrvDdrGetDefaultConfiguration(&defaultDdrConfig);
        currentConfig = &defaultDdrConfig;
    }
    else
    {
        currentConfig = targetDdrConfig;
    }

    struct internalDdrConfig internalConfig;
    DrvDdrArchInitialiseInternalConfig(&internalConfig);

    assert(currentConfig->version == 7);

    if (!DrvDdrIsInitialized())
    {
        // keep DDR controller and PHY in reset for now
        DrvCprSysDeviceAction(CSS_AUX_DOMAIN, ASSERT_RESET, AUX_CLK_MASK_DDR_CORE_CTRL | AUX_CLK_MASK_DDR_CORE_PHY);
        powerUpDss();
        setupClocksAndPulseAutodeassertingResets(currentConfig, &internalConfig);
        configureDdrCtrl(0, currentConfig, &internalConfig);
        configureAndInitialiseDdrPhy(0, currentConfig, &internalConfig);
    }

    if (internalConfig.odt_pin_is_connected && (currentConfig->dieOdtStrength != ODT_DISABLED))
    {
        DrvDdrSetupDieOdt(currentConfig->dieOdtStrength, ODT_DISABLED_DURING_POWER_DOWN);
        // TODO: if we ever enable write leveling, then we should move this MR register write to a point in time before the write leveling happens.
    }
    DrvDdrCheckDdrType(currentConfig, &internalConfig);
}


// Warning: keep in mind this will destroy all data on DDR
// Also, DO NOT USE this function if you don't understand the consequences
void DrvDdrShutdown(void)
{
    if(!DrvDdrIsInitialized()){
        assert(0); // DDR cannot shutdown if it is not initialized
    }
    DrvDdrEnterDeepPowerDown();
    DrvDdrGoIntoLowPowerDataRetention();
    // prepare for clocks turning off by asserting reset
    DrvCprSysDeviceAction(CSS_AUX_DOMAIN, ASSERT_RESET, AUX_CLK_MASK_DDR_CORE_CTRL | AUX_CLK_MASK_DDR_CORE_PHY  );
    DrvCprSysDeviceAction(CSS_DOMAIN, ASSERT_RESET, DEV_CSS_DSS_APB_RST_PHY);
    // turn off clock
    DrvCprSysDeviceAction(CSS_AUX_DOMAIN, DISABLE_CLKS, AUX_CLK_MASK_DDR_REF | AUX_CLK_MASK_DDR_CORE_CTRL );
    DrvCprSysDeviceAction(CSS_DOMAIN, DISABLE_CLKS, DEV_CSS_DSS_APB | DEV_CSS_DSS_BUS);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_DSS);
}

void DrvDdrShutdownToSelfRefresh(void)
{
    DrvDdrEnterSelfRefresh();
    DrvDdrGoIntoLowPowerDataRetention();
    #ifndef MA2100
        DrvCprPowerTurnOffIsland(POWER_ISLAND_DSS);
    #else
        DrvCprPowerTurnOffIsland(POWER_ISLAND_DSS_ANALOG);
        DrvCprPowerTurnOffIsland(POWER_ISLAND_DSS_DIGITAL);
    #endif
    // prepare for clocks turning off by asserting reset
    DrvCprSysDeviceAction(CSS_AUX_DOMAIN, ASSERT_RESET, AUX_CLK_MASK_DDR_CORE_CTRL | AUX_CLK_MASK_DDR_CORE_PHY  );
    DrvCprSysDeviceAction(CSS_DOMAIN, ASSERT_RESET, DEV_CSS_DSS_APB_RST_PHY);
    // turn off clock
    DrvCprSysDeviceAction(CSS_AUX_DOMAIN, DISABLE_CLKS, AUX_CLK_MASK_DDR_REF | AUX_CLK_MASK_DDR_CORE_CTRL );
    DrvCprSysDeviceAction(CSS_DOMAIN, DISABLE_CLKS, DEV_CSS_DSS_APB | DEV_CSS_DSS_BUS);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_DSS);
}

static const struct autoDetectConfig everythingIsAutodetected =
{
    .forceSubstrateVersion = AUTO_DETECT_SUBSTRATE_VERSION,
};

void DrvDdrGetDefaultConfiguration(tyDdrConfig * outputDdrConfig)
{
    DrvDdrGetDefaultConfigurationNoAutoDetect(outputDdrConfig, &everythingIsAutodetected);
}

void DrvDdrStartUpInSelfRefresh(const tyDdrConfig * targetDdrConfig)
{
    struct internalDdrConfig internalConfig;
    static tyDdrConfig defaultDdrConfig;
    const tyDdrConfig * currentConfig;
    if (targetDdrConfig == NULL)
    {
        DrvDdrGetDefaultConfiguration(&defaultDdrConfig);
        currentConfig = &defaultDdrConfig;
    }
    else
    {
        currentConfig = targetDdrConfig;
    }

    DrvDdrArchInitialiseInternalConfig(&internalConfig);

    // keep DDR controller and PHY in reset for now
    DrvCprSysDeviceAction(CSS_AUX_DOMAIN, ASSERT_RESET, AUX_CLK_MASK_DDR_CORE_CTRL | AUX_CLK_MASK_DDR_CORE_PHY);
    powerUpDss();
    setupClocksAndPulseAutodeassertingResets(currentConfig, &internalConfig);
    configureDdrCtrl(1, currentConfig, &internalConfig);
    configureAndInitialiseDdrPhy(1, currentConfig, &internalConfig);
}

void DrvDdrInitialiseFromSelfRefresh(const tyDdrConfig * targetDdrConfig)
{
    tyDdrConfig defaultDdrConfig;
    const tyDdrConfig * currentConfig;
    if (targetDdrConfig == NULL)
    {
        DrvDdrGetDefaultConfiguration(&defaultDdrConfig);
        currentConfig = &defaultDdrConfig;
    }
    else
    {
        currentConfig = targetDdrConfig;
    }
    struct internalDdrConfig internalConfig;
    DrvDdrStartUpInSelfRefresh(currentConfig);
    DrvDdrExitSelfRefresh();
    DrvDdrCheckDdrType(currentConfig, &internalConfig);
}

void DrvDdrEnterSelfRefresh( void )
{
    u32 pwrctl = GET_REG_WORD_VAL(DDRC_PWRCTL_ADR);
    pwrctl |= 1 << 5; // selfref_sw = 1
    SET_REG_WORD(DDRC_PWRCTL_ADR, pwrctl);

    // wait until we enter self refresh
    u32 stat;
    do {
        stat = GET_REG_WORD_VAL(DDRC_STAT_ADR);
    } while (((stat >> 4) & 3) != 2);
}

void DrvDdrExitSelfRefresh( void )
{
    u32 pwrctl = GET_REG_WORD_VAL(DDRC_PWRCTL_ADR);
    pwrctl &= ~(1 << 5); // selfref_sw = 0
    SET_REG_WORD(DDRC_PWRCTL_ADR, pwrctl);

    // wait until we are out of self refresh
    u32 stat;
    do {
        stat = GET_REG_WORD_VAL(DDRC_STAT_ADR);
    } while (((stat >> 4) & 3) == 2);
}

void DrvDdrEnterDeepPowerDown( void )
{
    SET_REG_WORD(DDRC_PWRCTL_ADR, 1 << 3); // en_dfi_dram_clk_disable, and explicitly disable auto-self-refresh, and auto-power-down,
                                              // because the controller will not automatically enter deep power down in those states
    u32 stat;
    do {
        stat = GET_REG_WORD_VAL(DDRC_STAT_ADR);
    } while (((stat & 7) == 2) || ((stat & 7) == 3)); // wait for the controller to exit power down / self refresh

    SET_REG_WORD(DDRC_PWRCTL_ADR, (1 << 2) | (1 << 3)); // en_dfi_dram_clk_disable, and deeppowerdown_en

    do {
        stat = GET_REG_WORD_VAL(DDRC_STAT_ADR);
    } while ((stat & 4) != 4); // wait for the controller to enter deep power down
}

void DrvDdrDisableByteLane(int lane)
{
    // disable anything that we can disable for these byte lanes

    u32 gcr0_regaddr = DDR_PHY_DX0GCR0 + (DDR_PHY_DX1GCR0 - DDR_PHY_DX0GCR0) * lane;
    DrvRegSetBitField((void *)(gcr0_regaddr), 0, 1, 0); // DX8EN
    DrvRegSetBitField((void *)(gcr0_regaddr), 2, 1, 0); // DQSGOE
    DrvRegSetBitField((void *)(gcr0_regaddr), 4, 1, 1); // DQSGPDD
    DrvRegSetBitField((void *)(gcr0_regaddr), 5, 1, 1); // DQSGPDR
    DrvRegSetBitField((void *)(gcr0_regaddr), 16, 1, 1); // PLLRST
    DrvRegSetBitField((void *)(gcr0_regaddr), 17, 1, 1); // PLLPD
    DrvRegSetBitField((void *)(gcr0_regaddr), 30, 1, 0); // MDLEN
    DrvRegSetBitField((void *)(gcr0_regaddr), 31, 1, 1); // CALBYP

    u32 gcr1_regaddr = gcr0_regaddr + DDR_PHY_DX0GCR1 - DDR_PHY_DX0GCR0;
    DrvRegSetBitField((void *)(gcr1_regaddr), 0, 16, 0x5555); // DXPDDMODE // PDD always ON, i.e. Driver always powered down for each data bit in the byte lane.
    DrvRegSetBitField((void *)(gcr1_regaddr), 16, 16, 0x5555); // DXPDRMODE // PDR always ON, i.e. Receiver always powered down for each data bit in the byte lane.

    u32 gcr2_regaddr = gcr0_regaddr + DDR_PHY_DX0GCR2 - DDR_PHY_DX0GCR0;
    DrvRegSetBitField((void *)(gcr2_regaddr), 0, 16, 0xaaaa); // DXTEMODE // ODT always OFF
    DrvRegSetBitField((void *)(gcr2_regaddr), 16, 16, 0xaaaa); // DXOEMODE // OE always OFF

    u32 gcr3_regaddr = gcr0_regaddr + DDR_PHY_DX0GCR3 - DDR_PHY_DX0GCR0;
    DrvRegSetBitField((void *)(gcr3_regaddr), 0, 2, 1); // DSPDDMODE - always on (powered down)
    DrvRegSetBitField((void *)(gcr3_regaddr), 2, 2, 1); // DSPDRMODE - always on (powered down)
    DrvRegSetBitField((void *)(gcr3_regaddr), 4, 2, 2); // DSTEMODE - ODT always off
    DrvRegSetBitField((void *)(gcr3_regaddr), 6, 2, 2); // DSOEMODE - OE always off
    DrvRegSetBitField((void *)(gcr3_regaddr), 8, 2, 1); // DMPDDMODE - always on (powered down)
    DrvRegSetBitField((void *)(gcr3_regaddr), 10, 2, 1); // DMPDRMODE - always on (powered down)
    DrvRegSetBitField((void *)(gcr3_regaddr), 12, 2, 2); // DMTEMODE - ODT always off
    DrvRegSetBitField((void *)(gcr3_regaddr), 14, 2, 2); // DMOEMODE - OE always off
}

void DrvDdrDisableByteLanes(int first, int last)
{
    int i;
    for (i=first; i<=last; i++)
    {
        DrvDdrDisableByteLane(i);
    }
}

void DrvDdrGoIntoLowPowerDataRetention( void )
{
    SET_REG_WORD(EVENT_REG_ADR, 1);

    DrvDdrDisableByteLanes(0, 7);
    DrvRegSetBitField((void *)(DDR_PHY_DX0GCR0), 0, 1, 1); // Set back DX8EN = 1 because when all byte lanes are disabled, the phy behaves in a special way.

    DrvRegSetBitField((void *)(DDR_PHY_ACIOCR3), 0, 8, 0xaa); // CKOEMODE = OE always off for index >= 1, dynamic for index 0
    DrvRegSetBitField((void *)(DDR_PHY_ACIOCR3), 8, 8, 0xaa); // CSOEMODE = OE always off for index >= 1, dynamic for index 0
    DrvRegSetBitField((void *)(DDR_PHY_ACIOCR4), 0, 8, 0x55); // CKPDDMODE = PDD always on for index >= 1, dynamic for index 0
    DrvRegSetBitField((void *)(DDR_PHY_ACIOCR4), 8, 8, 0x55); // CSPDDMODE = always on
    // don't disable CKE!:
    // DrvRegSetBitField((void *)(DDR_PHY_ACIOCR5), 24, 8, 0x55); // CKEPDDMODE = always on = powered down for index >=1. Dynamic for index = 0
    // DrvRegSetBitField((void *)(DDR_PHY_ACIOCR5), 8, 8, 0xaa); // CKEOEMODE = OE always off for index >= 1, dynamic for index 0

    DrvRegSetBitField((void *)(DDR_PHY_DXCCR), 5, 8-5+1, 0); // dqs resistors off
    DrvRegSetBitField((void *)(DDR_PHY_DXCCR), 9, 12-9+1, 0); // dqs resistors off

    DrvDdrSetupManualZQCalibrationSettingsOnlyWeakPulldownOutputOnAC();

    // Disable all VREF generators
    SET_REG_WORD(DDR_PHY_IOVCR0, 0);
    SET_REG_WORD(DDR_PHY_IOVCR1, 0);

    SET_REG_WORD(DDR_PHY_PLLCR, (1 << 29) | (1 << 31));

    SET_REG_WORD(DDR_PHY_ACIOCR2, 0x55555555);
    SET_REG_WORD(DDR_PHY_ACIOCR1, 0xaaaaaaaa);

    DrvDdrSetDataRetentionSignals(1);
}

void DrvDdrDecreaseLCDLFieldByPercent(u32 reg, u32 ofs, u32 percent_tweak) {
    u32 val = GET_REG_WORD_VAL(reg);
    u32 xv = (val >> ofs) & 0xff;
    val &= ~(0xff << ofs);
    xv = xv * (100 - percent_tweak) / 100;
    assert(xv <= 0xff);
    val |= xv << ofs;
    SET_REG_WORD(reg, val);
}

#undef DRVDDR_C

///@}
