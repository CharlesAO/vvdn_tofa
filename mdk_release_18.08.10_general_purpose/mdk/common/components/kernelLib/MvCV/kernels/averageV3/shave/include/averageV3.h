#ifndef __AVERAGEV3_H__
#define __AVERAGEV3_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{	
/// This kernel performs 1 average line from 3 input lines
/// @param[in] in        - 3 Input lines      
/// @param[out] out      - 1 Output line    
/// @param[in] width     - Width of input line

MVCV_FUNC(void, mvcvAverageV3, half** in, half** out, u32 width)
//!@}

#endif //__AVERAGEV3_H__
