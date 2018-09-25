#ifndef __CONVOLUTION3X3S2XHHX_H__
#define __CONVOLUTION3X3S2XHHX_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a convolution with stride of 2 on the input image using the given 3x3 matrix
/// stride = 2(s2); input data type = unsigned8(u8); coefficient data type = half(h); computation/accumulation datatype = half(h); output data type = unsigned8(u8);
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] conv       - 9 elements array with half(h) values containing the 3x3 convolution matrix
/// @param[in] inWidth    - Width of input line


MVCV_FUNC(void, mvcvConvolution3x3s2xhhx, u8** in, u8** out, half conv[9], u32 inWidth)
//!@}


#endif //__CONVOLUTION3X3S2XHHX_H__
