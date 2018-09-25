///
/// @file OsDrvCpr.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvCpr OsCpr Driver
/// @{
/// @brief     RTEMS CPR Header File 
///

#ifndef OSDRVCPR_H
#define OSDRVCPR_H

// Bare-metal layer include
#include <OsDrvCprDefines.h>
#ifdef __cplusplus
extern "C" {
#endif

// 1: Defines
// ----------------------------------------------------------------------------

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

// 3: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

// 4: Export Functions
// ----------------------------------------------------------------------------

/// Initialize CPR Driver access
///
/// This function has to be called prior to any other call to this API
/// it is responsible to allocate OS resources for this driver
/// @return OS_MYR_DRV_SUCCESS on success, non-zero otherwise
int OsDrvCprInit(void);

/// Open CPR Driver
///
/// This function initializes CPR Driver variables
/// This function is to be called when the cpr driver is to be used after calling cpr_init
/// It is responsible to initialize internal driver status
/// @return OS_MYR_DRV_SUCCESS on success, non-zero otherwise
int OsDrvCprOpen(void);

/// Request system clock change notification
///
/// This function enables notification on system clock change
/// @param[in] notification New callback to be executed on system clock change
/// @param[in] extraInfo void pointer to any information required by the user
/// @return OS_MYR_DRV_SUCCESS on success, non-zero otherwise
int OsDrvCprRequestSysClockChangeNotificationRegister(osDrvCprNotification notification, void *extraInfo);

/// Request system clock change notification
///
/// This function enables notification on system clock change
/// @param[in] notification callback on system clock change to be removed
/// @return OS_MYR_DRV_SUCCESS on success, non-zero otherwise
int OsDrvCprRequestSysClockChangeNotificationUnregister(osDrvCprNotification notification);



/// Table driven configuration of all device clocks
///
/// This function may be used to configure the entire clock tree for the system
/// In many applications calling DrvCprInit and this function is all that will be needed
/// @param[in] pSocClockConfig - pointer to clock configuration structure
/// @return OS_MYR_DRV_SUCCESS on access to the resource, non-zero otherwise
int OsDrvCprSetupClocks(const tySocClockConfig * pSocClockConfig);

/// Update the system frequency by modifiying the PLL value and the system clock divider
///
/// Most applications will not need to call this function as it will be handled by DrvCprSetupClocks
/// The resultant system clock will be
/// achievedPllKhz * clockdivnumerator / clockdivdenominator
/// Note: this may not be the same as plltargetfreqkhz * clockdivnumerator / clockdivdenominator
/// The function will return the actual system clock in Khz or a negative value on error
/// @param[in] plltargetfreqkhz    - desired PLL frequency in Khz
/// @param[in] clockdivnumerator   - numerator for system clock divider
/// @param[in] clockdivdenominator - denominator for system clock divider
/// @param[out] negative value on error or returns the achieved system clock if positive
/// @return OS_MYR_DRV_SUCCESS on access to the resource, non-zero otherwise
int OsDrvCprConfigureSystemClock(u32 plltargetfreqkhz, u32 clockdivnumerator, u32 clockdivdenominator);

/// Enable one or more auxillary clocks with a specific divider value
///
/// @param[in] auxClksMask - 32 bit mask where set bits enable the corresponding clock
/// @param[in] clkSource   - clock source to use for the clocks specified in mask
/// @param[in] numerator   - clock divider numerator   (max 12 bits)
/// @param[in] denominator - clock divider denominator (max 12 bits)
/// @param[out] 0 on operation success, non-zero otherwise
/// @return OS_MYR_DRV_SUCCESS on access to the resource, non-zero otherwise
int OsDrvCprAuxClockEnable(u32 auxClksMask,tyCprClockSrc clkSource,u32 numerator, u32 denominator);


/// Configure the auxilary clocks using a null terminated array of
/// Auxilary clock configurations
///
/// Normally this is handled by DrvCprConfigureSystemClk for most users
/// @param[in] pAuxClkCfg[] - null terminated array of tyAuxClkDividerCfg
/// @param[out] 0 on success, non-zero otherwise
/// @return OS_MYR_DRV_SUCCESS on access to the resource, non-zero otherwise
int  OsDrvCprAuxClockArrayConfig(const tyAuxClkDividerCfg  pAuxClkCfg[]);


/// Passed an Enum of different clock types, SYSTEM, AUX clocks
int OsDrvCprGetClockFreqKhz(tyClockType clockType,tyClockConfig * clkCfg, s32 *result);

/// Bypass PLL or re-enable it
///
/// It is not expected that this function would be needed in normal
/// Operation.
/// @param[in] pll0Bypass: BYPASS_ENABLE, BYPASS_DISABLE, BYPASS_NOCHANGE
/// @param[in] pll1Bypass: BYPASS_ENABLE, BYPASS_DISABLE, BYPASS_NOCHANGE
/// @return OS_MYR_DRV_SUCCESS on success, non-zero otherwise
int OsDrvCprPllBypass(tyPllBypassOption pll0Bypass,tyPllBypassOption pll1Bypass);

/// Enable, Disable or reset one or more system devices
/// @param[in] domain  - Subgroup of referenced devices CSS_DOMAIN,MSS_DOMAIN or UPA_DOMAIN
/// @param[in] action  - type of action ENABLE_CLKS, DISABLE_CLKS or  RESET_DEVICES
/// @param[in] devices - 64 bit bitfield representing the devices on which the action is applied
/// @return OS_MYR_DRV_SUCCESS on success, non-zero otherwise
int OsDrvCprSysDeviceAction(tyCprClockDomain clkDomain,tyCprDeviceAction action,uint64_t devices);

/// Fast function to get the current system clock in Khz
/// @param[out] result system clock frequency in Khz
/// @return OS_MYR_DRV_SUCCESS on access to the resource, non-zero otherwise
int OsDrvCprGetSysClockKhz(u32 *result);

/// Returns the number of system clocks per microsecond
/// @param[out] result  number of system clocks taken for 1 microsecond
/// @return OS_MYR_DRV_SUCCESS on access to the resource, non-zero otherwise
int OsDrvCprGetSysClockPerUs(u32 *result);

/// Turn on any combination of SHAVEs, enable clocks and pulse reset
/// @param[in] shave_mask: bit mask for shaves to turn on
int OsDrvCprTurnOnShaveMask(u32 shave_mask);

/// Turn off any combination of SHAVEs, disable clock and assert reset
/// @param[in] shave_mask: bit mask for shaves to turn off
int OsDrvCprTurnOffShaveMask(u32 shave_mask);

/// Enable all clocks.
///
/// Function is dedicated for tests. Should never be used in real projects
/// Will generate extra and unnecessary power consumption
/// @return OS_MYR_DRV_SUCCESS on success, non-zero otherwise
int OsDrvCprStartAllClocks(void);

/// Turn on one or more power islands, giving explicit trickle, and enable delay values
/// in clock ticks.
/// @param[in] islands_mask: the index of each bit that is set to 1 tells which islands should be turned on
/// @param[in] trickle_ticks: number of clock cycles to wait after enabling trickle power, but before turning on full power
/// @param[in] enable_ticks: number of clock cycles to wait after enabling full power, but before turning of isolation.
///              If the number of ticks is 0, then it will wait until power island status is active.
/// @return OS_MYR_DRV_SUCCESS on success, non-zero otherwise
int OsDrvCprPowerTurnOnIslandRaw(u32 islands_mask, u32 trickle_ticks, u32 enable_ticks);

/// Turn off one or more power islands, giving explicit isolation enable, and island disable delay values
/// in clock ticks.
/// @param[in] islands_mask: the index of each bit that is set to 1 tells which islands should be turned off
/// @param[in] iso_ticks: the number of clock cycles to wait after enabling isolation, but before turning off power
/// @param[in] disable_ticks: the number of clock cycles to wait after disabling power, before the function returns.
///              If the number of ticks is 0 then it will wait until the power island status becomes inactive.
/// @return OS_MYR_DRV_SUCCESS on success, non-zero otherwise
int OsDrvCprPowerTurnOffIslandRaw(u32 islands_mask, u32 iso_ticks, u32 disable_ticks);

/// Turn on a single given power island, taking care of any delays that are needed.
/// NOTE: this function does not reset anything in the power island after turning it on, the user must do that manually.
/// @return OS_MYR_DRV_SUCCESS on success, non-zero otherwise
int OsDrvCprPowerTurnOnIsland(enum PowerIslandIndex island_index);

/// Turn off a single given power island, taking care of any delays that are needed.
/// NOTE: this function does not reset anything in the power island before turning it off. It is recommended to reset peripherals,
/// and to remove the clock to peripherals before turning the island they are in off.
/// @return OS_MYR_DRV_SUCCESS on success, non-zero otherwise
int OsDrvCprPowerTurnOffIsland(enum PowerIslandIndex island_index);

/// Returns the CPR version id
///
/// @return
/// @param[out] cprVersionId: 32 bits value containing CPR version id
///
/// @return OS_MYR_DRV_SUCCESS on success, non-zero otherwise
///
int OsDrvCprGetCprVersionId(u32* cprVersionId);

#ifdef __cplusplus
}
#endif


#endif // OSDRVCPR_H

///@}

