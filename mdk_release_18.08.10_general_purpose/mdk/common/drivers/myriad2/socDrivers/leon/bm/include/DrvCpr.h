///
/// @file DrvCpr.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvCpr Cpr Driver
/// @{
/// @brief     API for the Clock Power Reset Module
///
/// CPR Driver Core Functions
/// - DrvCprSetupClocks function which provides a table driven mechanism for configuring all clocks in the system
///   This API call replaces the previously used drv_sys_config
/// - Input Osc clock config is handled using callback functionality which will be routed to the appropriate board driver
///   function on boards which support variable input clocks (NO #define OSC_IN!!)
/// - Optional Callback mechanism to notifiy applications of system clock changes so that user actions can be performed on clock change
/// - Automatic update of DDR timings on ddr clock frequency change (Not implemented yet)
/// - Unified API for sysdevice action (BLOCK reset, enabling clocks, disable clocks)
/// - Function to query clock frequency
/// 


#ifndef DRV_CPR_H
#define DRV_CPR_H 

// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvCprDefines.h"
#include "registersMyriadMa2x5x.h"
#include <DrvRegUtilsDefines.h>

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// Initialise the Clock Power Reset Controller
/// 
/// This function has 2 optional function callback parameters both of which may be NULL if not needed
/// This function must be called prior to using any other CPR function
/// @param[in] pfGetSetOscFreqKhzCb - Callback to support changing system Osc In frequency on boards that support it 
/// @param[in] tySysClkFreqChangeCb - Callback which is used to notify the application when system clock frequency has changed. 
/// @return    0 on success, non-zero otherwise  
int DrvCprInit(void);

/// Initialise the Clock Power Reset Controller
/// 
/// This function has 2 optional function callback parameters both of which may be NULL if not needed
/// This function must be called prior to using any other CPR function
/// @param[in] ref0InClkKhz OSC0 Used 
/// @param[in] ref1InClkKhz OSC1 Used 
/// @param[in] unused0 Deprecated
/// @param[in] unused1 Deprecated
/// @param[in] unused2 Deprecated
/// @return    0 on success, non-zero otherwise 
int DrvCprInitClocks(u32 ref0InClkKhz, u32 ref1InClkKhz, u32 unused0, u32 unused1, u32 unused2);

/// Request notification on system clock changes
/// 
/// This function is use to request a callback each time the system frequency is changed
/// @param[in] tySysClkFreqChangeCb - Callback which is used to notify the application when system clock frequency has changed. 
/// @return    0 on success, non-zero otherwise  
void DrvCprRequestSysClkChangeNotification(tySysClkFreqChangeCb pfSysClockFreqChange);


/// Update the system frequency by modifiying the PLL value and the system clock divider
///
/// Most applications will not need to call this function as it will be handled by DrvCprSetupClocks
/// The resultant system clock will be
/// achievedPllKhz * clockDivNumerator / clockDivDenominator
/// Note: this may not be the same as pllTargetFreqKhz * clockDivNumerator / clockDivDenominator  
/// The function will return the actual system clock in Khz or a negative value on error
/// @param[in] pllTargetFreqKhz    - desired PLL frequency in Khz
/// @param[in] clockDivNumerator   - numerator for system clock divider
/// @param[in] clockDivDenominator - denominator for system clock divider
/// @return    negative value on error or returns the achieved system clock if positive  
int DrvCprConfigureSystemClk(int pllTargetFreqKhz, int clockDivNumerator, int clockDivDenominator);

/// Configure the auxilary clocks using a null terminated array of 
/// Auxilary clock configurations
/// 
/// Normally this is handled by DrvCprConfigureSystemClk for most users
/// @param[in] pAuxClkCfg[] - null terminated array of tyAuxClkDividerCfg
/// @return    0 on success, non-zero otherwise  
int DrvCprAuxClockArrayConfig(const tyAuxClkDividerCfg  pAuxClkCfg[]);

// Passed an Enum of different clock types, SYSTEM, AUX clocks
double DrvCprGetClockFreqKhzFloat(tyClockType clockType,tyClockConfig * clkCfg);

// Passed an Enum of different clock types, SYSTEM, AUX clocks
tyCprRational DrvCprGetClockFreqKhzRational(tyClockType clockType,tyClockConfig * clkCfg);

// Passed an Enum of different clock types, SYSTEM, AUX clocks
int DrvCprGetClockFreqKhz(tyClockType clockType,tyClockConfig * clkCfg);

/// Bypass PLL or re-enable it
/// 
/// It is not expected that this function would be needed in normal
/// Operation. 
/// @param[in] pll0Bypass: BYPASS_ENABLE, BYPASS_DISABLE, BYPASS_NOCHANGE
/// @param[in] pll1Bypass: BYPASS_ENABLE, BYPASS_DISABLE, BYPASS_NOCHANGE
/// @return    0 on success, non-zero otherwise  
int DrvCprPllBypass(tyPllBypassOption pll0Bypass,tyPllBypassOption pll1Bypass);

/// Fast function to get the current system clock in Khz
/// @return system clock frequency in Khs
u32 DrvCprGetSysClockKhz(void);

/// Returns the number of system clocks per microsecond
/// @return number of system clocks taken for 1 microsecond
u32 DrvCprGetSysClocksPerUs(void);

/// Turn on any combination of SHAVEs, enable clocks and pulse reset
/// @param[in] shave_mask: bit mask for shaves to turn on
void DrvCprTurnOnShaveMask(u32 shave_mask);

/// Turn off any combination of SHAVEs, disable clock and assert reset
/// @param[in] shave_mask: bit mask for shaves to turn off
void DrvCprTurnOffShaveMask(u32 shave_mask);

/// Table driven configuration of all device clocks
///
/// This function may be used to configure the entire clock tree for the system
/// In many applications calling DrvCprInit and this function is all that will be needed
/// @param[in] pSocClockConfig - pointer to clock configuration structure
/// @return    0 on success, non-zero otherwise
int DrvCprSetupClocks(const tySocClockConfig * pSocClockConfig);

/// Enable all clocks.
///
/// Function is dedicated for tests. Should never be used in real projects
/// Will generate extra and unnecessary power consumption
void DrvCprStartAllClocks(void);

/// Enable one or more auxillary clocks with a specific divider value
///
/// @param[in] auxClksMask - 32 bit mask where set bits enable the corresponding clock
/// @param[in] clkSource   - clock source to use for the clocks specified in mask
/// @param[in] numerator   - clock divider numerator   (max 12 bits)
/// @param[in] denominator - clock divider denominator (max 12 bits)
/// @return    0 on success, non-zero otherwise
int DrvCprAuxClocksEnable(u32 auxClksMask,tyCprClockSrc clkSource,u32 numerator, u32 denominator);

/// Enable, Disable or reset one or more system devices
/// @param[in] domain  - Subgroup of referenced devices CSS_DOMAIN,MSS_DOMAIN or UPA_DOMAIN
/// @param[in] action  - type of action ENABLE_CLKS, DISABLE_CLKS or  RESET_DEVICES
/// @param[in] devices - 64 bit bitfield representing the devices on which the action is applied
/// @return    void
void DrvCprSysDeviceAction(tyCprClockDomain clkDomain,tyCprDeviceAction action,u64 devices);

/// Configure and unbypass specified PLL, don't make any other changes.
/// @param[in] pll: CPR_PLL0 or CPR_PLL1
/// @param[in] clkSrcPll: CLK_SRC_REFCLK0, or for CPR_PLL1 CLK_SRC_REFCLK1 is also allowed
/// @param[in] pllTargetFreqKhz: Pll target frequency in KHz
int DrvCprSetupJustPll(tyCprPll pll, tyCprClockSrc clkSrcPll, int pllTargetFreqKhz);

/// Save current system clock settings into frist argument, and set system clock to the second argument, using 1/1 master divider
/// @param[out] systemClockSettings: where to store current system clock settings
/// @param[in] sysClockTargetFreqKHz: what system clock to set
void DrvCprSaveAndSetSystemClock(tySystemClockSettings *systemClockSettings, int sysClockTargetFreqKhz);

/// Restore system clock settings from structure saved by DrvCprSaveAndSetSystemClock()
/// @param[out] systemClockSettings: system clock settings to restore
void DrvCprRestoreSystemClock(const tySystemClockSettings *systemClockSettings);

/// Returns the CPR version id
///
///@return
///     32 bits value containing the version id
///
u32 DrvCprGetCprVersionId(void);

/// Turn on one or more power islands, giving explicit trickle, and enable delay values
/// in clock ticks.
/// @param[in] islands_mask: the index of each bit that is set to 1 tells which islands should be turned on
/// @param[in] trickle_ticks: number of clock cycles to wait after enabling trickle power, but before turning on full power
/// @param[in] enable_ticks: number of clock cycles to wait after enabling full power, but before turning of isolation.
///              If the number of ticks is 0, then it will wait until power island status is active.
void DrvCprPowerTurnOnIslandsRaw(u32 islands_mask, u32 trickle_ticks, u32 enable_ticks);

/// Turn off one or more power islands, giving explicit isolation enable, and island disable delay values
/// in clock ticks.
/// @param[in] islands_mask: the index of each bit that is set to 1 tells which islands should be turned off
/// @param[in] iso_ticks: the number of clock cycles to wait after enabling isolation, but before turning off power
/// @param[in] disable_ticks: the number of clock cycles to wait after disabling power, before the function returns.
///              If the number of ticks is 0 then it will wait until the power island status becomes inactive.
void DrvCprPowerTurnOffIslandsRaw(u32 islands_mask, u32 iso_ticks, u32 disable_ticks);

/// Turn on a single given power island, taking care of any delays that are needed.
/// NOTE: this function does not reset anything in the power island after turning it on, the user must do that manually.
void DrvCprPowerTurnOnIsland(enum PowerIslandIndex island_index);

/// Turn off a single given power island, taking care of any delays that are needed.
/// NOTE: this function does not reset anything in the power island before turning it off. It is recommended to reset peripherals,
/// and to remove the clock to peripherals before turning the island they are in off.
void DrvCprPowerTurnOffIsland(enum PowerIslandIndex island_index);

/// Return a 1-active bitmap of the power islands that are on.
u32 DrvCprPowerGetStatus( void );

#ifdef POWER_AWARE

int  DrvCprGetUpaClk(u32 id);
void DrvCprCheckandstopISR(u32 id);
void DrvCprEnableUpa(u32 id);
void DrvCprClearUpa(u32 id);

#endif

/// Trigger a reset of the entire chip
/// NOTE: this function does reset everything even the CPR it is executing from
inline void DrvSocReset(void)
{
	// Reset the chip
	SET_REG_WORD(CPR_MAS_RESET_ADR, 0x00);
}

// --------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------
// Test Code below, do not use!!
// --------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------

/// This function is a common clock configuration for Myriad2 used by the silicon verification team in certain tests
/// 
void DrvCprCommonTestConfigureClocks(void);

#ifdef __cplusplus
}
#endif

#endif // DRV_CPR_H  


///@}
