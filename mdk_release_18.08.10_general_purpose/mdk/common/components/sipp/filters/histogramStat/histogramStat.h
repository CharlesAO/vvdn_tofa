///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref histogramStat SIPP filter API.
///    

#ifndef __SIPP_HISTOGRAM_STAT_H__
#define __SIPP_HISTOGRAM_STAT_H__ 

#include <sipp.h>

/// @sf_definition histogramStat histogramStat
/// @sf_description  This filter makes histogram for each RGB component input.
/// @sf_group ISP
/// @sf_preserve imgSize
/// @sf_outdatatype UInt32
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/histogramStat/arch/$(MV_SOC_REV_DIR)/shave/src/histogramStat.asm
/// @{

/// @brief Parameter structure of the @ref histogramStat filter.
typedef struct
{ 
	//step*8 will be the number of skipped pixeles
	UInt32 step;

}
HistogramStatParam;

/// @brief Shave function of the @ref histogramStat filter.
void SVU_SYM(svuHistogramStat)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuHistogramStat);

/// @}

#endif //__SIPP_HISTOGRAM_STAT_H__
