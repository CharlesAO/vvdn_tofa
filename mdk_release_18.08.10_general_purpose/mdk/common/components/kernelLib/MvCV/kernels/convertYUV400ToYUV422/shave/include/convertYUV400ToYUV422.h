#ifndef __CONVERTFROMYUV400TOYUV422_H__
#define __CONVERTFROMYUV400TOYUV422_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{ 
///This kernel converts from yuv400 to yuv422
///@param[in]   in          - Input line
///@param[out]  out         - Output line
///@param[in]   width       - Width of the input line

MVCV_FUNC(void, mvcvConvertYUV400ToYUV422, u8* in, u16* out, u32 width)
//!@}


#endif //__CONVERTFROMYUV400TOYUV422_H__