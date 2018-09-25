///
/// @file DrvDdrDefines.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvDdr
/// @{
/// @brief     API for the DRAM Driver
///


#ifndef DRV_DDR_DEFINES_H
#define DRV_DDR_DEFINES_H 

#include "mv_types.h"

enum die_drive_strength {
    LPDDR2_DIE_DRIVE_34_3_OHM = 1,
    LPDDR2_DIE_DRIVE_40_OHM = 2,
    LPDDR2_DIE_DRIVE_48_OHM = 3,
    LPDDR2_DIE_DRIVE_60_OHM = 4,
    LPDDR2_DIE_DRIVE_80_OHM = 6,
    LPDDR2_DIE_DRIVE_120_OHM = 7,
    
    LPDDR3_DIE_DRIVE_34_3_OHM = 1,
    LPDDR3_DIE_DRIVE_40_OHM = 2,
    LPDDR3_DIE_DRIVE_48_OHM = 3,
    LPDDR3_DIE_DRIVE_60_OHM = 4,
    LPDDR3_DIE_DRIVE_80_OHM = 6,
    
    LPDDR3_DIE_DRIVE_34_3_OHM_DOWN_40_OHM_UP_240_OHM_TERM = 9,
    LPDDR3_DIE_DRIVE_40_OHM_DOWN_48_OHM_UP_240_OHM_TERM = 10,
    LPDDR3_DIE_DRIVE_34_3_OHM_DOWN_48_OHM_UP_120_OHM_TERM = 11,
};

enum die_odt_value {
    ODT_DISABLED = 0,
    ODT_RZQ_DIV_4 = 1,
    ODT_RZQ_DIV_2 = 2,
    ODT_RZQ_DIV_1 = 3,
        ODT_RZQ = ODT_RZQ_DIV_1,
};

enum refreshBurstTypes {
    REFRESH_BURST_DEFAULT = 0,
    REFRESH_BURST_SINGLE = 1,
    REFRESH_BURST_1 = 1,
    REFRESH_BURST_2 = 2,
    REFRESH_BURST_3 = 3,
    REFRESH_BURST_4 = 4,
    REFRESH_BURST_5 = 5,
    REFRESH_BURST_6 = 6,
    REFRESH_BURST_7 = 7,
    REFRESH_BURST_8 = 8,
};

enum calibrated_phy_drive_strengths {
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_480_OHM = 0,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_240_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_160_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_120_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_96_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_80_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_68_6_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_60_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_53_3_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_48_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_43_6_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_40_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_36_9_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_34_3_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_32_OHM,
    DDR_CALIBRATED_DRIVE_STRENGTH_TARGET_30_OHM,
};

enum calibrated_phy_odt_termination_strengths {
    DDR_CALIBRATED_ODT_STRENGTH_TARGET_240_OHM = 1,
    DDR_CALIBRATED_ODT_STRENGTH_TARGET_150_OHM = 2,
    DDR_CALIBRATED_ODT_STRENGTH_TARGET_120_OHM = 2,
    DDR_CALIBRATED_ODT_STRENGTH_TARGET_75_OHM = 4,
    DDR_CALIBRATED_ODT_STRENGTH_TARGET_60_OHM = 5,
    DDR_CALIBRATED_ODT_STRENGTH_TARGET_40_OHM = 8,
};

/*
 * \typedef tyDdrConfig
 * This is a structure that can be given to DrvDdrInitialise() to configure the DDR driver in a non-default way.
 * Note: When customizing the DDR driver for an application it is not recommended to set up one of these structure
 * from scratch, setting every field of this structure, because some of the fields of this structure might change
 * in the future, and the application might not become compatible with a new MDK.
 * Instead, it is recommended to call DrvDdrGetDefaultConfiguration(&my_structure), to set up a default configuration
 * that would be equivalent to what is used when calling DrvDdrInitialise(NULL). After that desired changes can be
 * made to a subset of the fields of this struct, and DrvDdrInitialise(&my_structure) can be called.
 */
typedef struct
{
    int version; /*< Current version is 7. Note: if an incorrect version is provided,
                  *< the driver may fail.  */
    enum die_drive_strength dieDriveStrength;    /*< This field can be used to set the drive strength of the memory die output pins.
                                                  *< It is not recommended for the end user to change this setting. */
    enum die_odt_value dieOdtStrength;           /*< This field can be used to set the On Die Termination strength of the memory die input pins.
                                                  *< Supported for lpddr3 only. It is not recommended for the end user to change this setting. */
    u32 useCalibratedDriveDqStrength;            /*< When this field is true, the DDR PHY DQ pins output drive strength will be calibrated by
                                                  *< using the external reference resistor. In this case the calibration target is set by
                                                  *< the calibratedDriveDqPull*StrengthTarget fields. Otherwise the DQ pin drive strength is set by
                                                  *< the manualDriveDQPull*Strength fields. It is not recommended for the end user to change this setting. */
    u32 useCalibratedDriveAcStrength;            /*< When this field is true, the DDR PHY AC pins output drive strength will be calibrated by
                                                  *< using the external reference resistor. In this case the calibration target is set by
                                                  *< the calibratedDriveAcPull*StrengthTarget fields. Otherwise the DQ pin drive strength is set by
                                                  *< the manualDriveACPull*Strength fields. It is not recommended for the end user to change this setting. */
    u32 useCalibratedOdtStrength;                /*< When this field is true, the DDR PHY AC pin input on-die termination strength will be calibrated by
                                                  *< using the external reference resistor. In this case the calibration target is set by
                                                  *< the calibratedOdtStrengthTarget field, and also taking into account the isCalibratedOdtStrengthPullUpOnly
                                                  *< field. Otherwise the DQ pin drive strength is set by the manualOdtPull*Strength fields.
                                                  *< It is not recommended for the end user to change this setting. */
    u32 calibratedDriveDqPullDownStrengthTarget; /*< This field sets the PHY DQ pins output drive strength, when useCalibratedDriveDqStrength is enabled,
                                                  *< and the output I/O cell is driving a low value. Please see enum calibrated_phy_drive_strength for
                                                  *< the meaning of the values that can be stored in this field. It is not recommended for the end user
                                                  *< to change this setting. */
    u32 calibratedDriveDqPullUpStrengthTarget;   /*< This field sets the PHY DQ pins output drive strength, when useCalibratedDriveDqStrength is enabled,
                                                  *< and the output I/O cell is driving a high value. Please see enum calibrated_phy_drive_strength for
                                                  *< the meaning of the values that can be stored in this field. It is not recommended for the end user
                                                  * to change this setting. */
    u32 calibratedDriveAcPullDownStrengthTarget; /*< This field sets the PHY AC pins output drive strength, when useCalibratedDriveAcStrength is enabled,
                                                  *< and the output I/O cell is driving a low value. Please see enum calibrated_phy_drive_strength for
                                                  *< the meaning of the values that can be stored in this field. It is not recommended for the end user
                                                  *< to change this setting. */
    u32 calibratedDriveAcPullUpStrengthTarget;   /*< This field sets the PHY AC pins output drive strength, when useCalibratedDriveAcStrength is enabled,
                                                  *< and the output I/O cell is driving a high value. Please see enum calibrated_phy_drive_strength for
                                                  *< the meaning of the values that can be stored in this field. It is not recommended for the end user
                                                  *< to change this setting. */
    u32 calibratedOdtStrengthTarget;             /*< This field sets the PHY DQ pins input termination strength, when useCalibratedOdtStrength is enabled,
                                                  *< and the output I/O cell is driving a high value. Please see enum calibrated_phy_odt_termination_strength for
                                                  *< the meaning of the values that can be stored in this field. It is not recommended for the end user
                                                  *< to change this setting. */
    u32 isCalibratedOdtStrengthPullUpOnly;       /*< When useCalibratedOdtStrength is true, this field determines whether the PHY side input pin ODT is
                                                  *< pull-up only odt, or of it is symmetric, pulling the pin both low and high at the same time.
                                                  *< It is not recommended for the end user to change this setting. */
    u32 manualDriveDqPullDownStrength;           /*< This field is in use when useCalibratedDriveDqStrength=0. This is a raw value that can be controlled in 127 steps.
                                                  *< The actual impedance is roughly equal to 2kOhm/rawValue, but there can be significant variation due to
                                                  *< process, voltage, and temperature. It is not recommended for the end user to change this setting. */
    u32 manualDriveDqPullUpStrength;             /*< This field is in use when useCalibratedDriveDqStrength=0. This is a raw value that can be controlled in 127 steps.
                                                  *< The actual impedance is roughly equal to 2kOhm/rawValue, but there can be significant variation due to
                                                  *< process, voltage, and temperature. It is not recommended for the end user to change this setting. */
    u32 manualDriveAcPullDownStrength;           /*< This field is in use when useCalibratedDriveAcStrength=0. This is a raw value that can be controlled in 127 steps.
                                                  *< The actual impedance is roughly equal to 2kOhm/rawValue, but there can be significant variation due to
                                                  *< process, voltage, and temperature. It is not recommended for the end user to change this setting. */
    u32 manualDriveAcPullUpStrength;             /*< This field is in use when useCalibratedDriveAcStrength=0. This is a raw value that can be controlled in 127 steps.
                                                  *< The actual impedance is roughly equal to 2kOhm/rawValue, but there can be significant variation due to
                                                  *< process, voltage, and temperature. It is not recommended for the end user to change this setting. */
    u32 manualOdtPullDownStrength;               /*< This field is in use when useCalibratedOdtStrength=0. This is a raw value that can be controlled in 127 steps.
                                                  *< The actual impedance is roughly equal to 2kOhm/rawValue, but there can be significant variation due to
                                                  *< process, voltage, and temperature. It is not recommended for the end user to change this setting. */
    u32 manualOdtPullUpStrength;                 /*< This field is in use when useCalibratedOdtStrength=0. This is a raw value that can be controlled in 127 steps.
                                                  *< The actual impedance is roughly equal to 2kOhm/rawValue, but there can be significant variation due to
                                                  *< process, voltage, and temperature. It is not recommended for the end user to change this setting. */
    u32 acTweakPercent;                          /*< The DDR PHY has various built-in delay lines in every DDR I/O cell, that can be used to tweak the timing
                                                  *< of every signal. Without the delay lines the controller would generate the output clock, and the AC (Address/Command)
                                                  *< bus data edges at the same time. However the LPDDR2/3 specification requires the clock to be centered on the AC bus data
                                                  *< eye. The DDR phy achieves this by measuring what delay value is equivalent to one clock period, and applying
                                                  *< a quarter-period delay on the AC bus. This delay setting may be tweaked then by software for optimum operation.
                                                  *< The value in this field is the percentage by which to decrease the auto-calibrated delay. A value of 100
                                                  *< would mean that the clock and AC edges are generated in the same moment. A value of 0 means that the AC data
                                                  *< edges are generated a quarter clock cycle after the clock. It is not recommended for the end user to change this setting. */
    u32 dqsTweakPercent;                         /*< According to the LPDDR2/LPDDR3 specification, When performing read operations, the memory generates the DQS strobe edges,
                                                  *< and the data bits in a completely aligned fashion. The controller does not know how the read data is aligned to the
                                                  *< PHY's output clock, so it must use the DQS signal to sample the data pins. The DQS strobe frequency will be the same as the
                                                  *< clock frequency provided to the memory, so the PHY can delay the DQS strobe by a quarter clock period, to center the DQS
                                                  *< strobe in the DQ* pin data eyes. This field can be used to adjust the calibrated delay. The value describes by how many
                                                  *< percents to decrease the calibrated value. A value of zero means that the DQS input delay is one quarter clock period.
                                                  *< A value of 100 would disable the delay line on the DQS inputs. It is not recommended for the end user to change this setting. */
    u32 perBankRefresh;                          /*< The LPDDR2/LPDDR3 standard allows for two types of refresh commands. An all-bank refresh command refreshes a small number
                                                  *< of rows in all of the banks of the memory at the same time, while a per-bank refresh handles a single bank only.
                                                  *< It is up to the end user to decide which refresh style is more desirable. All-bank refresh commands take a little longer,
                                                  *< But they have to be issued less often, so overall memory bandwidth will be better with all-bank refresh, but also
                                                  *< Worst case latency will be worse with all-bank refresh. In this field value of 0 sets up all-bank refresh, while
                                                  *< a value of 1 sets up per-bank refresh. */
    enum refreshBurstTypes refreshBurst;         /*< This field sets up how many refresh commands to issue one after the other. The default is bursts of 2. Bigger bursts
                                                  *< will improve bandwidth slightly */
    u32 averageTimeBetweenAllBankRefreshCommandsNs; /*< This field can be used to manually set how often on average all-bank refresh commands have to be issued. The default value
                                                     *< DEFAULT_REFRESH_TIMING is a magic value that will automatically select 7800ns for MA215x, and 3900ns for MA245x. */
    u32 averageTimeBetweenPerBankRefreshCommandsNs; /*< This field can be used to manually set how often on average per-bank refresh commands have to be issued. The default value
                                                     *< DEFAULT_REFRESH_TIMING is a magic value that will automatically select 975s for MA215x, and 482ns for MA245x. */
    u32 allBankRefreshCycleTimeNs;                  /*< Sets the all-bank refresh cycle time. The default value of DEFAULT_REFRESH_TIMING is a magic value that sets
                                                     *< the cycle time to 130ns */
    u32 perBankRefreshCycleTimeNs;                  /*< Sets the per-bank refresh cycle time. The default value of DEFAULT_REFRESH_TIMING is a magic value that sets
                                                     *< the cycle time to 60ns */
    u32 en_dfi_dram_clk_disable;                    /*< This fields allows to enable a power optimisation feature that disables the ddr clock from toggling while there
                                                     *< is no activity necessary on the ddr bus */
    u32 powerdown_en;                               /*< This fields allows to enable a power optimisation feature that automatically sends Enter Power-Down commands to
                                                     *< the memory die, when there is no DDR activity, to conserve power. */
    u32 selfref_en;                                 /*< This fields allows to enable a power optimisation feature that automatically sends Enter Self-Refresh commands to
                                                     *< the memory die, when there is no DDR activity, to conserve power. */
    u32 derating_en;                                /*< Derating is a feature of the LPDDR2 or LPDDR3 memory die, where it monitors the memory die temperature, and it
                                                     *< exposes a field in the MR4 mode register, where it tells the DDR controller whether it requests derated refresh
                                                     *< or not. The DDR controller can periodically read this register, and change it's refresh timings in response to
                                                     *< the content of this register. With the current configuration per-bank refresh is not able to work correctly
                                                     *< at the same time with derating being enabled, so to enable derating, all-bank refresh must be selected, otherwise
                                                     *< the DDR driver will fail with an assert(). Enabling All-bank refresh results in a bandwidth improvement, while
                                                     *< enabling derating causes a bandwidth loss at > 85 degrees memory die temperature. If the chip may heat up above
                                                     *< 85 degrees, it is advised to enable derating, or to lower the average time between refresh commands.
                                                     *< One risk with enabling derating is that it will show bandwidth discontinuity while the chip is heated above 85
                                                     *< degrees, so if in a real-time application the user exhausts the entire available bandwidth of the chip under
                                                     *< 85 degC, when the application heats above 85 degC it might fail. */
    const int *address_map;                         /*< This field selects how memory address bits get mapped to LPDDR2 Row/Column/Bank bits.
                                                     *< Here is a list of generic address mappings that exist for both MA215x and MA245x:
                                                     *< address_map_B2_B0__rows__columns
                                                     *< address_map_B2_B1__rows__B0__columns
                                                     *< address_map_B2__rows__B1_B0__columns
                                                     *< address_map_rows__B2_B0__columns
                                                     *< The name of the address map can be read as a description of Row bit/Column bit/Bank bit mappings from in-system
                                                     *< byte address bits goint from high to low. The lowest (rightmost) column bit always matches byte-address bit 2.
                                                     *< This field is a pointer to an array of 32-bit values that will be written directly into DDRC_ADDRMAP*_ADR registers,
                                                     *< so even more flexibility with regard to address mapping can be achieved by calculating new values manually. */
    int default_frequency_khz;                      /*< This field sets the default frequency that the memory will function at. When DrvDdrInitialise() starts it looks at
                                                     *< the state of the CSS_AUX_DDR_CORE aux clock. If it is disabled, then it will configure it to output the frequency
                                                     *< specified in this field. Please note that this frequency is what the DDR PHY must be fed with, and another PLL in
                                                     *< the DDR PHY will further multiply this frequency up. The frequency set in this register is half of the frequency
                                                     *< output on the DDR clock pins, and a quarter of the baud rate of the DDR data pins. */
} tyDdrConfig;

enum forceSubstrateVersion {
    AUTO_DETECT_SUBSTRATE_VERSION = 0,
    FORCE_SUBSTRATE_VERSION_4L,
    FORCE_SUBSTRATE_VERSION_6L,
};

struct autoDetectConfig {
    enum forceSubstrateVersion forceSubstrateVersion;
};

#define DEFAULT_REFRESH_TIMING 0

enum addr_map_type
{
    ADDR_MAP_B2,
    ADDR_MAP_R12
};

#endif // DRV_DDR_DEFINES_H


///@}
