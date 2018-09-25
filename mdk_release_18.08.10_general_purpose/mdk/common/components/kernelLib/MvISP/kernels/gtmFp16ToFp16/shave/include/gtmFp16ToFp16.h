///
/// @file gtmFp16ToFp16.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     
///

#ifndef __gtmFp16ToFp16_H__
#define __gtmFp16ToFp16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// gtmFp16ToFp16 kernel - 
/// @param[in]  iline 	    - pointer to input line
/// @param[Out] output  	- pointer to output line
/// @param[in]  width       - width in pixels
/// @param[in]  scurve      - pointer to scurve line
/// @param[in]  nPoints     - number of points
/// @return     Nothing

MVCV_FUNC(void, mvispGtmFp16ToFp16, half* iline, half* output, u32 width, half* scurve, int nPoints) 
//!@}

#endif //__gtmFp16ToFp16_H__
