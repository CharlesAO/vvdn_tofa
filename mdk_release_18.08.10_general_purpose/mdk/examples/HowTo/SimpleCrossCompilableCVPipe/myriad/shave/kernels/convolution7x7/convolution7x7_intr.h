#ifndef __CONVOLUTION7x7_INTR_H__
#define __CONVOLUTION7x7_INTR_H__
#include <mv_types.h>
#include <mvcv_macro.h>



// Convolution7x7 kernel 
// @param[in] in         - array of pointers to input lines
// @param[in] out        - pointer to output line
// @param[in] conv       - array of values from convolution
// @param[in] inWidth    - width of input line

extern "C" void   Convolution7x7_intr( u8** in, u8** out, half conv[49], unsigned int inWidth);


#endif
