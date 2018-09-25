#ifndef __INTEGRALIMAGESUMU16U32_H__
#define __INTEGRALIMAGESUMU16U32_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel makes the sum of all pixels before current pixel 
/// (columns to the left and rows above)
/// @param[out] out        - Output lines,  unsigned int format
/// @param[in] in          - Input lines, unsigned short data format     
/// @param[runNr]in        - Output line number
/// @param[in] sum         - Pointer to prevision generated line, will be used for calculate current pixels, and will be updated at the end to point to new output generated line
/// @param[in] width       - Width of input line

MVCV_FUNC(void, mvcvIntegralImageSumU16U32, unsigned int* out, unsigned short* in, unsigned int runNr, unsigned int **previsionOutLnPointer, unsigned int width)
//!@}

#endif //__INTEGRALIMAGESUMU16U32_H__