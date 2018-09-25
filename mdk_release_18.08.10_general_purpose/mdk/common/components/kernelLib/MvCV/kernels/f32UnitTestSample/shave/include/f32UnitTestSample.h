#ifndef __F32UNITTESTSAMPLE_H__
#define __F32UNITTESTSAMPLE_H__

#include <mv_types.h>

/// This kernel computes the dot product between vec_a and each vector from vec_b_list
/// (it uses the mvuDot built-in function from Vector Utility Library)
/// @param[in]	vec_a			- Array of floats
/// @param[in]	vec_b_list		- Array of pointers to floats
/// @param[out]	out				- Array of floats for dot product results
/// @param[in] 	num_vectors		- Number of vectors for vec_b_list
extern "C" void mvuDotProductStreaming(fp32 *vec_a, fp32 **vec_b_list, fp32 *out, u32 num_vectors);

/*///computes the dot product between vec_a and each vector from vec_b_list
///@param[in]	vec_a			- array of floats
///@param[in]	vec_b_list		- array of pointers to floats
///@param[out]	out				- array of floats for dot product results
///@param[in] 	num_vectors		- number of vectors for vec_b_list*/
extern "C" void dotProductStreaming   (fp32 *vec_a, fp32 **vec_b_list, fp32 *out, u32 num_vectors);

#endif //__F32UNITTESTSAMPLE_H__
