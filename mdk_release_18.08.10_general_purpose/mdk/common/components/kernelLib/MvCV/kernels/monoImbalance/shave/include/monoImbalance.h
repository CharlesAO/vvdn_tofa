#ifndef __MONOIMBALANCE_H__
#define __MONOIMBALANCE_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
///  * Input is 16 bit integer.  Perform local pixel imbalance correction,
///  * and output FP16.
/// @param[in]  in         		- Input lines
/// @param[in]  out       		- Output line
/// @param[in]  thrDark         - Dark threshold
/// @param[in]  thrBright       - Bright threshold
/// @param[in]  inputBits
/// @param[in] inWidth    		- Width of input line
	
MVCV_FUNC(void, mvcvMonoImbalance, u16** in, half** out, int thrDark, int thrBright, int inputBits, u32 inWidth)
//!@}

#endif //__MONOIMBALANCE_H__