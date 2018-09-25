#ifndef __extractDescriptor64_freakA_H__
#define __extractDescriptor64_freakA_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel extracts a binary descriptor from 512 pairs (FREAK), 1 if the first point is greater than the second one, or 0 otherwise 
/// @param[in] in        - smoothed values from a Gaussian filter from all pixels from a given pattern (FREAK)
/// @param[out] out      - binary value that describe the corner using a patern (binary descriptor)

MVCV_FUNC(void, mvcvExtractDescriptor64_freakA, unsigned short *in_pixel_array,  unsigned char *out_descriptor);
//!@}

#endif //__extractDescriptor64_freakA_H__
