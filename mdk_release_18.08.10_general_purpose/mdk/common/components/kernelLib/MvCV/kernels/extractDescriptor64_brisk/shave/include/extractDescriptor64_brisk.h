#ifndef __extractDescriptor64_brisk_H__
#define __extractDescriptor64_brisk_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel extracts a binary descriptor from 512 pairs (Brisk), 1 if the first point is greater than the second one, or 0 otherwise
/// @param[in] in        - smoothed values from a Gaussian filter from all pixels from a given pattern (Brisk)
/// @param[out] out      - binary value that describe the corner using a pattern (binary descriptor)

MVCV_FUNC(void, mvcvExtractDescriptor64_brisk, unsigned short *in_pixel_array,  unsigned char *out_descriptor)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvExtractDescriptor64_brisk_opt(unsigned short *in_pixel_array,  unsigned char *out_descriptor);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__extractDescriptor64_brisk_H__
