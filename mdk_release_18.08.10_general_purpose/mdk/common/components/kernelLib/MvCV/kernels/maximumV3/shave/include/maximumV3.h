#ifndef __MAXIMUMV3_H__
#define __MAXIMUMV3_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{	
/// This kernel performs 1 maximum line from 3 input lines
/// @param[in] in        - 3 Input lines      
/// @param[out] out      - 1 Output line    
/// @param[in] width     - Width of input line

MVCV_FUNC(void, mvcvMaximumV3, half** in, half** out, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvMaximumV3_opt( half** in, half** out, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__MAXIMUMV3_H__
