#ifndef __CONVOLUTION3X3_INTR_H__
#define __CONVOLUTION3X3_INTR_H__
#include <mv_types.h>
#include <mvcv_macro.h>



// Convolution3x3 kernel 
// @param[in] in         - array of pointers to input lines
// @param[in] out        - pointer to output line
// @param[in] conv       - array of values from convolution
// @param[in] inWidth    - width of input line
	
extern "C" void Convolution3x3_intr(u8** in, u8** out, half conv[9], u32 inWidth);


#endif //__CONVOLUTION3X3_H__