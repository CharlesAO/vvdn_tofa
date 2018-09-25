///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref equalizeHist SIPP filter API.
///    

#ifndef __SIPP_EQUALIZEHIST_H__
#define __SIPP_EQUALIZEHIST_H__ 

#include <sipp.h>

/// @sf_definition equalizeHist  Equalize Histogram
/// @sf_description  This filter makes an equalization through an image with a given histogram.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/equalizeHist/arch/$(MV_SOC_REV_DIR)/shave/src/equalizeHist.asm
/// @{

/// @brief Parameter structure of the @ref equalizeHist filter.
typedef struct
{
      /// @sf_desc pointer to an input array that indicates the cumulative histogram of the image
	  UInt32 *cum_hist;

}
EqualizeHistParam;

/// @brief Shave function of the @ref equalizeHist filter.
void SVU_SYM(svuEqualizeHist)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuEqualizeHist);

/// @}

#endif //__SIPP_EQUALIZEHIST_H__
