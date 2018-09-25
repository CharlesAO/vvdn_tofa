///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref boxFilter SIPP filter API.
///

#ifndef __SIPP_BOXFILTER_H__
#define __SIPP_BOXFILTER_H__ 

#include <sipp.h>

//User can override max vertical size (which dictates the number of pointers allocated on stack)

#ifndef BOX_MAX_V_SZ
#define BOX_MAX_V_SZ 16
#endif

/// @sf_definition boxFilter Generic Box Filter
/// @sf_description This filter calculates average on variable kernel size, on kernel size number of input lines.
/// @sf_group CV
/// @sf_outdatatype UInt8, UInt16, UInt32, half, float
/// @sf_inputs
/// 	- datatypes: UInt8, UInt16, UInt32, half, float; kernels:1x1, 3x3, 5x5, 7x7, 9x9, 11x11, 13x13, 15x15
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/boxFilter/arch/$(MV_SOC_REV_DIR)/shave/src/boxFilter.asm
/// @{


//User can override max vertical size (which dictates the number of pointers allocated on stack)
#ifndef BOX_MAX_V_SZ
#define BOX_MAX_V_SZ 16
#endif

typedef enum {
    FMT_U8,
    FMT_U16,
    FMT_U32,
    FMT_F16,
    FMT_F32,
}boxDataFmt;

//For H-padding test purposes, H and V sizes need to be flexible (e.g. sizeH= 64, sizeV = 3)
/// @brief Parameter structure of the @ref boxFilter filter.
typedef struct
{
	/// @sf_desc data format from enum boxDataFmt
    boxDataFmt dataFormat;
	/// @sf_desc kernel width
    UInt32     filterSizeH;
	/// @sf_desc kernel height
    UInt32     filterSizeV;
	/// @sf_desc 1 to normalize to kernel size, 0 otherwise
    UInt32     normalize;
}
BoxFilterParam;

/// @brief Shave function of the @ref boxFilter filter
void SVU_SYM(svuBoxFilter)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuBoxFilter);

/// @}

#endif // __SIPP_BOXFILTER_H__ 
