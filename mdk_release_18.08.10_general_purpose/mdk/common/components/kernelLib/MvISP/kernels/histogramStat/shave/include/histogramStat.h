///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Calculate histogram statistics for RGB image
///
#ifndef __histogramStat_H__
#define __histogramStat_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// HistogramStats kernel 
/// @param[in]  inputR 	    - pointer to the R component
/// @param[in]  inputG 	    - pointer to the G component
/// @param[in]  inputB 	    - pointer to the B component
/// @param[Out] histR  	    - pointer to histogram for R component
/// @param[Out] histG  	    - pointer to histogram for G component
/// @param[Out] histB  	    - pointer to histogram for B component
/// @param[in]	width		- width of the input line 
/// @param[in]	step		- step*8 will be the number of skipped pixeles
/// @return     Nothing

MVCV_FUNC(void, mvispHistogramStat, u8 *inputR, u8 *inputG, u8 *inputB, u32 *histR, u32 *histG, u32 *histB, u32 width, u32 step) 
//!@}

#endif //__histogramStat_H__
