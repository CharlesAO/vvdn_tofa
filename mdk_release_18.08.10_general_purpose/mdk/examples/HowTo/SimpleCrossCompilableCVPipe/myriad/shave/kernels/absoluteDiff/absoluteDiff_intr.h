#ifndef __ABSOLUTEDIFF_INTR_H__
#define __ABSOLUTEDIFF_INTR_H__
#include <mv_types.h>
#include <mvcv_macro.h>


// AbsoluteDiff kernel  computes the absolute difference of two images given as parameters(used to estimate motion)
// @param[in] in1             - array of pointers to input lines of the first image
// @param[in] in2             - array of pointers to input lines of the second image
// @param[out] out            - array of pointers to output line
// @param[in] width           - width of the input lines
// @return    Nothing

extern "C" void  AbsoluteDiff_intr( u8** in1, u8** in2, u8** out, u32 width);


#endif 