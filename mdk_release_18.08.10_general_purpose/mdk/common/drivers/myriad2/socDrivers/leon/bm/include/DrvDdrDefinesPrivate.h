///
/// @file DrvDdrDefinesPrivate.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvDdrPrivate
/// @{
/// @brief     Private header for DDR driver.
///


#ifndef DRV_DDR_DEFINES_PRIVATE_H
#define DRV_DDR_DEFINES_PRIVATE_H 

#include "mv_types.h"

enum die_odt_pd_control {
    ODT_DISABLED_DURING_POWER_DOWN = 0,
    ODT_ENABLED_DURING_POWER_DOWN = 1,
};

#define DDR_PHY_PIR_INIT      (1 <<  0)
#define DDR_PHY_PIR_ZCAL      (1 <<  1)
#define DDR_PHY_PIR_CA        (1 <<  2)
//  Reserved                    (1 <<  3)
#define DDR_PHY_PIR_PLLINIT   (1 <<  4)
#define DDR_PHY_PIR_DCAL      (1 <<  5)
#define DDR_PHY_PIR_PHYRST    (1 <<  6)
#define DDR_PHY_PIR_DRAMRST   (1 <<  7)
#define DDR_PHY_PIR_DRAMINIT  (1 <<  8)
#define DDR_PHY_PIR_WL        (1 <<  9)
#define DDR_PHY_PIR_QSGATE    (1 << 10)
#define DDR_PHY_PIR_WLADJ     (1 << 11)
#define DDR_PHY_PIR_RDDSKW    (1 << 12)
#define DDR_PHY_PIR_WRDSKW    (1 << 13)
#define DDR_PHY_PIR_RDEYE     (1 << 14)
#define DDR_PHY_PIR_WREYE     (1 << 15)
#define DDR_PHY_PIR_ICPC      (1 << 16)
#define DDR_PHY_PIR_PLLBYP    (1 << 17)
#define DDR_PHY_PIR_CTLDINIT  (1 << 18)
#define DDR_PHY_PIR_RDIMMINIT (1 << 19)
//  Reserved                    [26:20]
#define DDR_PHY_PIR_CLRSR     (1 << 27)
#define DDR_PHY_PIR_LOCKBYP   (1 << 28)
#define DDR_PHY_PIR_DCALBYP   (1 << 29)
#define DDR_PHY_PIR_ZCALBYP   (1 << 30)
#define DDR_PHY_PIR_INITBYP   (1 << 31)

#define DDR_CTRL_DFIUPD0_DIS_AUTO_CTRLUPD (1 << 31)
#define DDR_CTRL_DFIUPD0_DIS_AUTO_CTRLUPD_SRX (1 << 30)

#define DDR_CTRL_MSTR_SELECT_LPDDR3          (1 << 3)
#define DDR_CTRL_MSTR_SELECT_LPDDR2          (1 << 2)
#define DDR_CTRL_MSTR_DATA_BUS_WIDTH_FULL    (0 << 12)
#define DDR_CTRL_MSTR_DATA_BUS_WIDTH_HALF    (1 << 12)
#define DDR_CTRL_MSTR_DATA_BUS_WIDTH_QUARTER (2 << 12)
#define DDR_CTRL_MSTR_RANK_0_PRESENT         (1 << 24)
#define DDR_CTRL_MSTR_RANK_1_PRESENT         (2 << 24)

enum ddr_timing_parameter {
    CKE_LOW_TIME_WITH_POWER_AND_CLOCK_STABLE,
        T_INIT1 = CKE_LOW_TIME_WITH_POWER_AND_CLOCK_STABLE,
    CKE_HIGH_TIME_TO_FIRST_COMMAND,
        T_INIT3 = CKE_HIGH_TIME_TO_FIRST_COMMAND,
    IDLE_TIME_AFTER_RESET,
        T_INIT4 = IDLE_TIME_AFTER_RESET,
    TIME_FROM_RESET_TO_END_OF_AUTO_INIT,
        T_INIT5 = TIME_FROM_RESET_TO_END_OF_AUTO_INIT,
    TIME_FROM_ZQ_INIT_TO_FIRST_COMMAND,
        TZQINIT = TIME_FROM_ZQ_INIT_TO_FIRST_COMMAND,
    DEEP_POWER_DOWN_MIN,
    T_FAW,
    T_RTP,
    T_WTR,
    T_RP_PB,
    T_RP_AB,
    T_RAS_MIN,
    T_RAS_MAX,
    T_RRD,
    T_RCD,
    T_WLMRD,
    T_XP,
    T_XSR,
    T_RC,
    MAX_T_CKE_T_CKESR,
    T_CKESR,
    T_MRD,
    T_WR,
    T_ZQCL,
    T_ZQCS,
    T_ZQRESET,
    T_XP_USED_FOR_T_CKCSX,
    T_DQSCK_MAX_FOR_RD2WR,
};

enum ddr_timing_unit {
    UNIT_NULL,
    UNIT_SECONDS,
        UNIT_S = UNIT_SECONDS,
    UNIT_MILLISECONDS,
        UNIT_MS = UNIT_MILLISECONDS,
    UNIT_MICROSECONDS,
        UNIT_US = UNIT_MICROSECONDS,
    UNIT_NANOSECONDS,
        UNIT_NS = UNIT_NANOSECONDS,
    UNIT_PICOSECONDS,
        UNIT_PS = UNIT_PICOSECONDS,
    UNIT_PHY_CORE_CLOCKS,
    UNIT_PHY_HIGH_SPEED_CLOCKS, // typically 4x the core clock
    UNIT_PHY_DDR_OUTPUT_CLOCKS, // typically half of PHY_HIGH_SPEED_CLOCKS
    UNIT_PHY_32_DDR_OUTPUT_CLOCKS,
    UNIT_PHY_1024_DDR_OUTPUT_CLOCKS,
    UNIT_PHY_4096_DDR_OUTPUT_CLOCKS,
    UNIT_PHY_APB_CLOCKS,
    UNIT_CUSTOM, // no conversion is done
    UNIT_CTRL_1024_CTL_CLOCKS,
    UNIT_CTRL_4096_CTL_CLOCKS,
    UNIT_CTRL_32_CTL_CLOCKS,
    UNIT_CTRL_CTL_CLOCKS, // should be the same as UNIT_PHY_CORE_CLOCKS
};

enum ddr_rounding {
    DDR_DONT_ROUND_UP = 0,
    DDR_ROUND_DOWN = 0,
    DDR_ROUND_UP = 1,
};

enum ddr_adjust {
    DDR_DO_NOT_ADJUST = 0,
    DDR_ADJUST_MINUS_1_DIV_2,
    DDR_ADJUST_PLUS_1_DIV_2,
    DDR_ADJUST_PLUS_BLD2_MINUS_VAR_BASED_ON_TECH_DIV_2,
    DDR_ADJUST_PLUS_BLD2_PLUS_WL_PLUS_1_PLUS_1_D2,
    DDR_ADJUST_D2_RD_PLUS_1,
    DDR_ADJUST_PLUS_2_PLUS_1_DIV_2,
    DDR_ADJUST_PLUS_BLD2_PLUS_RL_PLUS_1_MINUS_WL_PLUS_1_D2,
    DDR_ADJUST_DIV_2,
};

enum ddrc_operating_mode {
    DDR_INIT,
    DDR_NORMAL,
    DDR_POWER_DOWN,
    DDR_SELF_REFRESH,
    DDR_DEEP_POWER_DOWN,
};

struct ddr_timing_constraint {
    s32 value;
    s32 value2;
    enum ddr_timing_parameter name;
    enum ddr_timing_unit unit;
    enum ddr_timing_unit unit2;
} __attribute__((__packed__));

struct timing_register_field_description {
    u32 register_address;
    u32 (*calculate_custom_unit)(s32 value, enum ddr_timing_unit unit);
    char field_lsb;
    char field_msb;
    enum ddr_rounding rounding;
    enum ddr_adjust adjust;
    enum ddr_timing_unit specified_unit;
    enum ddr_timing_parameter name;
} __attribute__((__packed__));

struct burst_and_latency {
    u32 BL;
    u32 RL;
    u32 WL;
    u32 nWR;
};

struct init_mode_register_settings {
    u32 mr1;
    u32 mr2;
    u32 mr3;
};

struct internalDdrConfig {
    int lpddr_version; // 2 or 3
    int chip_size_gigabits; // 1, 4, 6, 8, 16
    int odt_pin_is_connected;
    int use_rrmode;
    const struct burst_and_latency *burst_and_latency;
};

#endif // DRV_DDR_DEFINES_PRIVATE_H


///@}
