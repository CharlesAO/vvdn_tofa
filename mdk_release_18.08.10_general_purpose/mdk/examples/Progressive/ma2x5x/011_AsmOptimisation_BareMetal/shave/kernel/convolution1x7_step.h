#ifndef __CONVOLUTION1x7_STEP_H__
#define __CONVOLUTION1x7_STEP_H__
#include <mv_types.h>
#include <mvcv_macro.h>


// This kernel performs a convolution on the input image using the given 1x7 matrix
// @param[in] in         - Input lines
// @param[in] out        - Output line
// @param[in] conv       - 7 elements array with fp16 values containing the 1x7 convolution matrix
// @param[in] inWidth    - Width of input line

void Convolution1x7_step0(u8** in, u8** out, half conv[7], u32 inWidth);
extern "C" {
void Convolution1x7_step1(u8** in, u8** out, half conv[7], u32 inWidth);
void Convolution1x7_step2(u8** in, u8** out, half conv[7], u32 inWidth);
void Convolution1x7_step3(u8** in, u8** out, half conv[7], u32 inWidth);
void Convolution1x7_step4(u8** in, u8** out, half conv[7], u32 inWidth);
}


#endif //__CONVOLUTION1x7_STEP_H__
