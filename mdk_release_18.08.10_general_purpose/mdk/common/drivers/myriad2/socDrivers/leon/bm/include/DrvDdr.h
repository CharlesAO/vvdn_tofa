///
/// @file DrvDdr.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvDdr Ddr Driver
/// @{
/// @brief     API for the DRAM Driver
///


#ifndef DRV_DDR_H
#define DRV_DDR_H 

// #define FORCE_USE_CORRECT_TIMING_IN_SIMS
#ifndef FORCE_USE_CORRECT_TIMING_IN_SIMS
#define FORCE_USE_CORRECT_TIMING_IN_SIMS 0
#else
#undef FORCE_USE_CORRECT_TIMING_IN_SIMS
#define FORCE_USE_CORRECT_TIMING_IN_SIMS 1
#endif

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "DrvDdrDefines.h"
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#include "DrvDdrMa2x5x.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

extern const int * const address_map_B2_B0__rows__columns;
extern const int * const address_map_B2_B1__rows__B0__columns;
extern const int * const address_map_B2__rows__B1_B0__columns;
extern const int * const address_map_rows__B2_B0__columns;

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// Initialise the system DRAM and configure based on specifed config
///
/// This function first checks to see if the DRAM is already initialised.
/// If the DRAM is not initialised, then the driver will reset the DDR controller
/// and initialise the DRAM, and then tune it according to configured parameters
/// 
/// @param[in] *targetDdrConfig - Pointer to DDR Config parameters (If null default config is used)
void DrvDdrInitialise(const tyDdrConfig * targetDdrConfig);

/// Shutdown the system DRAM 
///
/// Warning: keep in mind that all data on the DRAM will be lost
/// Also, DO NOT USE this function if you don't understand the consequences
/// This function first checks to see if the DRAM is already initialised.
/// If the DRAM is initialised, then the driver will deinitialise it and bring it to a shutdown state.
void DrvDdrShutdown(void);

/// Put the DRAM into selfrefresh and shut down the DSS power island, and any clocks to it.
void DrvDdrShutdownToSelfRefresh(void);

/// Write out the default ddr configuration into the structure pointed by the argument
/// This allows customizing only part of the default DDR configuration, without having to
/// maintain other parts of the DDR configuration.
///
/// @param[out] *outputDdrConfig - Pointer to empty DDR Config parameters structure.
void DrvDdrGetDefaultConfiguration(tyDdrConfig * outputDdrConfig);

/// Write out the default ddr configuration into the structure pointed by the argument
/// This allows customizing only part of the default DDR configuration, without having to
/// maintain other parts of the DDR configuration. This variant allows you to disable
/// some autodetected features.
///
/// @param[out] *outputDdrConfig - Pointer to empty DDR Config parameters structure.
/// @param[in] *autoDetectConfig - Pointer to structure describing what autodetect features to turn off
void DrvDdrGetDefaultConfigurationNoAutoDetect(tyDdrConfig * outputDdrConfig, const struct autoDetectConfig * autoDetectConfig);

/// After a system reboot, or after the DSS island has been reset, or power cycled,
/// this Initialises the DDR controller and PHY, while leaving the memory in self-refresh.
/// You must call DrvDdrExitSelfRefresh() after calling this function.
/// You may also call DrvDdrInitialiseFromSelfRefresh() instead of this function.
///
/// @param[in] *targetDdrConfig - Pointer to DDR Config parameters (If null default config is used)
void DrvDdrStartUpInSelfRefresh(const tyDdrConfig * targetDdrConfig);

/// After a system reboot, or after the DSS island has been reset, or power cycled,
/// this is the correct way to exit self-refresh.
///
/// @param[in] *targetDdrConfig - Pointer to DDR Config parameters (If null default config is used)
void DrvDdrInitialiseFromSelfRefresh(const tyDdrConfig * targetDdrConfig);

/// Assumes that the DDR has already been initialised
void DrvDdrEnterSelfRefresh( void );

/// This may only be called after calling DrvDdrExitSelfRefresh. The DSS island may not be
/// power-cycled or reset between the two calls.
void DrvDdrExitSelfRefresh( void );

/// Enters deep power down mode. In this mode the DDR die consumes the least amount
/// of power. Data is not preserved while staying in this mode.
/// Assumes that the DDR has already been initialised
void DrvDdrEnterDeepPowerDown( void );

/// Check if the DDR has already been initialized by a third party (i.e. moviDebug, or a ddrInit elf.)
///
/// @return non-zero if the DDR subsystem is already initialzied.
int  DrvDdrIsInitialized( void );

/// Configure the DDR phy pins to consume the least amount of power while keeping cke low.
/// It is recommended to call DrvDdrEnterSelfRefresh(), or DrvDdrEnterDeepPowerDown() before
/// calling this function
void DrvDdrGoIntoLowPowerDataRetention( void );

/// Before entering self refresh using DrvDdrEnterSelfrefresh(), this function may be called
/// to configure a less power-intentsive self-refresh-operation, that only keeps part of the
/// memory constantly refreshed.
///
/// @param[in] mask - A 8-bit bank mask, one bit per bank
void DrvDdrSetupPartialArraySelfRefreshBankMask(u32 mask);

/// Before entering self refresh using DrvDdrEnterSelfrefresh(), this function may be called
/// to configure a less power-intentsive self-refresh-operation, that only keeps part of the
/// memory constantly refreshed.
///
/// @param[in] mask - A 8-bit segment mask, one bit per segment
void DrvDdrSetupPartialArraySelfRefreshSegmentMask(u32 mask);

/// Write an LPDDR2 or LPDDR3 mode register. Please see the die datasheet, and/or
/// the JEDEC JESD209-2/JESD209-3 specs for a list of commonly supported registers
/// and their fields.
///
/// @param[in] reg - register index, maximum register index that can be accessed is 255.
/// @param[in] data - the 8 bits of data to write to the MR register.
void DrvDdrMrWrite(u32 reg, u32 data);

#ifdef __cplusplus
}
#endif


#endif // DRV_DDR_H  

///@}
