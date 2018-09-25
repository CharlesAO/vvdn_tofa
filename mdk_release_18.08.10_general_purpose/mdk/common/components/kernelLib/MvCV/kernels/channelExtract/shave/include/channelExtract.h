#ifndef __CHANNELEXTRACT_H__
#define __CHANNELEXTRACT_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel extracts one of the R, G, B, plane from an interleaved RGB line 
/// @param[in] in         - Input line      
/// @param[out]out        - Output line    
/// @param[in] width      - Width of input line
/// @param[in] plane      - Number 0 for extracting plane R, 1 for extracting G, and 2 for extracting B

MVCV_FUNC(void, mvcvChannelExtract, u8** in, u8** out, u32 width, u32 plane)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvChannelExtract_opt(u8** in, u8** out, u32 width, u32 plane);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__CHANNELEXTRACT_H__
