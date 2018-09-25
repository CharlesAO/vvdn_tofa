///
/// @file DrvDdrArch.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvDdrArch DdrArch Driver
/// @{
/// @brief     internal functions (chip version dependent) for the DRAM Driver
///


#ifndef DRV_DDR_ARCH_H
#define DRV_DDR_ARCH_H

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "DrvDdrDefines.h"
#include "DrvDdrDefinesPrivate.h"

// 2: Defines
// ----------------------------------------------------------------------------
#define DRVDDR_ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#ifdef __cplusplus
extern "C" {
#endif

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
extern const struct timing_register_field_description ctrl_timing_registers_arch[];
extern const int ctrl_timing_registers_arch_size;
extern int ddrControllerClockKhz;
extern int phyAPBClockKhz;

// 4:  internal functions (not exported to applications)
// ----------------------------------------------------------------------------

void powerUpDss(void);
void DrvDdrSetAddressMap(const tyDdrConfig *config);
void DrvDdrAllowDdrControllerToContinue(int allow);
void DrvDdrConfigureClockSignals( void );
void DrvDdrPhyStartInitializingAfterResetDeassert( void );
int isSecondOscillatorEnabled( void );
void DrvDdrSetDataRetentionSignals(int value);
void DrvDdrDisableByteLane(int lane);
void DrvDdrDisableByteLanes(int first, int last);
void DrvDdrSetupZqSettings(const tyDdrConfig *config);
void DrvDdrForceZqCalibrationVtUpdateIfPossible( void );
void DrvDdrSetupManualZQCalibrationSettingsOnlyWeakPulldownOutputOnAC( void );
u32 DrvDdrRecalculateRefreshCycleTimeForFreqRatio12(u32 value);
void DrvDdrArchInitialiseInternalConfig(struct internalDdrConfig *internalConfig);
void DrvDdrCheckDdrType(const tyDdrConfig *currentConfig, const struct internalDdrConfig *internalConfig);
int DrvDdrArePhyInitiatedDfiUpdatesSupported( void );
void DrvDdrDecreaseLCDLFieldByPercent(u32 reg, u32 ofs, u32 percent_tweak);
void DrvDdrQuasiStart( void );
void DrvDdrQuasiEnd( void );
#ifdef __cplusplus
}
#endif


#endif // DRV_DDR_ARCH_H

///@}
