#ifndef __BOXFILTER5x5_H__
#define __BOXFILTER5x5_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel performs average on 5x5 kernel size       
/// @param[in] in        - Input lines      
/// @param[out] out      - Output lines    
/// @param[in] normalize - 1 for normalizing to kernel size, 0 otherwise 
/// @param[in] width     - Width of input line

MVCV_FUNC(void, mvcvBoxfilter5x5, u8** in, u8** out, u32 normalize, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
extern "C" 
{
	void mvcvBoxfilter5x5_opt(u8** in, u8** out, u32 normalize, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__BOXFILTER5x5_H__
