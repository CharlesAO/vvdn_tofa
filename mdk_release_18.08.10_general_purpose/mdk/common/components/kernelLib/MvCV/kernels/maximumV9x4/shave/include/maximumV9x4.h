#ifndef __MAXIMUMV9X4_H__
#define __MAXIMUMV9X4_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{	
/// This kernel performs 4 maximum lines for each 9 input lines, in order,  of 12 input lines
/// @param[in] in        - 9 Input lines      
/// @param[out] out      - 4 Output lines    
/// @param[in] width     - Width of input line

MVCV_FUNC(void, mvcvMaximumV9x4, half** in, half** out, u32 width)
//!@}

#endif //__MAXIMUMV9X4_H__
