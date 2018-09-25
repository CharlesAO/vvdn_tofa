#ifndef __CANNY_H__
#define __CANNY_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// The kernel finds edges in the input image image and marks them in the output map edges using the Canny algorithm(9x9 kernel size).
/// @param[in]  srcAddr    - Input lines      
/// @param[out] dstAddr    - Output line    
/// @param[in]  threshold1 - Lower threshold, value between 0-255
/// @param[in]  threshold2 - Upper threshold, value between 0-255
/// @param[in]  width      - Width of input line 


MVCV_FUNC(void, mvcvCanny, u8** srcAddr, u8** dstAddr,  u8* buffer, u32 threshold1, u32 threshold2, u32 width)
//!@}

#endif //__CANNY_H__
