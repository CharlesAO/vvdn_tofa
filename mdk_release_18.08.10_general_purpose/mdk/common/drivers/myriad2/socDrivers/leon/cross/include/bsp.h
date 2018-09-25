///
/// @file bsp.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup bsp bsp Driver
/// @{
/// @brief     Cross BSP 
///

#ifndef __BSP_H__
#define __BSP_H__

/*
 * BSP Structure
*/
typedef struct 
{
	uint32_t ref0ClockKhz;	/*< Reference oscillator used */
	uint32_t pll0TargetFrequencyKhz; /*< PLL0 Target Frequency */
	uint32_t currentSysClockKhz;	/*< Current System Clock  */
	uint64_t cssClocks; /*< CSS Clocks */		
	uint32_t mssClocks;	/*< MSS Clocks */	
	uint32_t upaClocks;	/*< UPA Clocks */	
	uint32_t sippClocks; /*< SIPP Clocks */		
	uint32_t auxClocks;	/*< AUX Clocks */	
	uint16_t masterDividerNumerator; /*< Master Divider Numerator */	
	uint16_t masterDividerDenominator; /*< Master Divider Denominator */
}osCrossBSPClockConfig_t;
#endif


///@}
