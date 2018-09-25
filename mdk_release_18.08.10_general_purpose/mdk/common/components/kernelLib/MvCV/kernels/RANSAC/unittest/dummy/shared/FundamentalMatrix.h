#ifndef FUNDAMENTALMATRIX_H
#define FUNDAMENTALMATRIX_H

#include"RANSAC.h"

#define POINTS_PER_SAMPLE 8



void findFundamentalMat(float *p_1,					// [IN] image 1 input points
                        float *p_2,						// [IN] image 2 input points
                        int nPoints,					// [IN] number of points
                        float *fm,						// [OUT] fundamental matrix
                        int *inliers,					// [OUT] inliers
                        struct fundamental_debug *Debug,
                        float RANSAC_inliers_ratio = 0.5,		// [IN] precentage of inliers
                        float RANSAC_confidence = 0.999,			// [IN] level of confidence
                        float RANSAC_dist_threshold = 1,// [IN] max dist error for a point to be declared inlier
                        int RANSAC_max_iterations = 100);   // [IN] max number of iterations






#endif// FUNDAMENTALMATRIX_H
