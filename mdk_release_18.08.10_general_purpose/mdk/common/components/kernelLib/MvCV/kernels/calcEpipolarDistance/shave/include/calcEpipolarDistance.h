#ifndef __CALC_EPIPOLAR_DISTANCE_H__
#define __CALC_EPIPOLAR_DISTANCE_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// The kernel is used for calculating Epipolar distance
///@param[in]  p_1                      - pointer to image 1 input points
///@param[in]  p_2                      - pointer to image 2 input points
///@param[in]  nPoints                  - number of points
///@param[in]  RANSAC_dist_threshold    - inlier threshold
///@param[in]  fm                       - fundamental matrix
///@param[out] inliers                  - inliers vector
///@param[out] nInliers                 - number of inliers

MVCV_FUNC (float, mvcvCalcEpipolarDistance, float *p_1, float *p_2, int nPoints,
           float RANSAC_dist_threshold, float *fm, int *inliers, int *nInliers)
//!@}

#endif //__CALC_EPIPOLAR_DISTANCE_H__

