#ifndef __HAMMING_DISTANCE_H__
#define __HAMMING_DISTANCE_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel find matches between two descriptors
/// @param[in] d1              - First descriptor
/// @param[in] d2              - Second descriptor
/// @param[in] array_size      - Size of array used to compute the distances
/// @param[in] descriptor_size - Size of the descriptor (32, 64 or 128 bytes)
/// @param[out] out            - Pointer to distances
/// @return    Nothing

MVCV_FUNC(void, mvcvHammingDistance, unsigned char *d1, unsigned char *d2, int array_size, int descriptor_size, unsigned short *distances)    
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvHammingDistance_opt(unsigned char *d1, unsigned char *d2, int array_size, int descriptor_size, unsigned short *distances);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__HAMMING_DISTANCE_H__
