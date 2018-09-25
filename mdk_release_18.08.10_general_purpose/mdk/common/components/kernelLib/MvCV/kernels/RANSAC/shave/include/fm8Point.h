///
/// @file fm8Point.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Fundamental matrix computation interface for 8 Point algo
///

#ifndef __8POINT_ALGO_H__
#define __8POINT_ALGO_H__


/// @brief: Compute fundamental matrix using 8-Point algo
/// @param[in] p_1 - corners from the first image
/// @param[in] p_2 - corresponding points in the second image
/// @param[out] fm - fundamental matrix
///
///
void fm8Point( float *p_1, float *p_2, float *fm);
void halffm8Point( float *p_1, float *p_2, float *fm);
#endif
