#ifndef __BOXFILTER7x7_H__
#define __BOXFILTER7x7_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel performs average on 7x7 kernel size       
/// @param[in] in        - Input lines      
/// @param[out] out      - Output lines    
/// @param[in] normalize - 1 for normalizing to kernel size, 0 otherwise 
/// @param[in] width     - Width of input line

MVCV_FUNC(void, mvcvBoxfilter7x7, u8** in, u8** out, u32 normalize, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
extern "C" 
{
	void mvcvBoxfilter7x7_opt(u8** in, u8** out, u32 normalize, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__BOXFILTER7x7_H__
	