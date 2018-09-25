#ifndef __ACCUMULATEWEIGHTED_H__
#define __ACCUMULATEWEIGHTED_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel calculates the weighted sum of the input image (srcAddr) and the accumulator (destAddr) so that accumulator becomes a running average of frame sequence
/// @param[in] srcAddr         - The input image, 8-bit unsigned integer   
/// @param[in] maskAddr        - Operation mask     
/// @param[out] destAddr       - The accumulator image, 32-bit floating-point    
/// @param[in] width           - Width of input image
/// @param[in] alpha           - Weight of the input image must be a fp32 between 0 and 1

MVCV_FUNC (void, mvcvAccumulateWeighted, u8** srcAddr, u8** maskAddr, fp32** destAddr,u32 width, fp32 alpha)
//!@}	

#endif //__ACCUMULATEWEIGHTED_H__