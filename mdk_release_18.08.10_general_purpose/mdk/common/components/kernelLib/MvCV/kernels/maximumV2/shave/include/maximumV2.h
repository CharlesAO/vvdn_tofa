#ifndef __MAXIMUMV2_H__
#define __MAXIMUMV2_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{	
/// This kernel performs 1 maximum line from 2 input lines
/// @param[in] in        - 2 Input lines      
/// @param[out] out      - 1 Output line    
/// @param[in] width     - Width of input line

MVCV_FUNC(void, mvcvMaximumV2, half** in, half** out, u32 width)
//!@}

#endif //__MAXIMUMV2_H__
