#include "calcEpipolarDistance.h"
#include <math.h>

float mvcvCalcEpipolarDistance(float *p_1, float *p_2, int nPoints, 
                              float RANSAC_dist_threshold, float *fm, 
                              int *inliers, int *nInliers)
{   
    float ave_dist = 0.0;
    int nin = 0;

    for (int i = 0; i < nPoints; i++) 
    {    
        // epipolar line equation
        float l[3];
        l[0] = p_1[2 * i] * fm[0] + p_1[2 * i + 1] * fm[1] + fm[2];
        l[1] = p_1[2 * i] * fm[3] + p_1[2 * i + 1] * fm[4] + fm[5];
        l[2] = p_1[2 * i] * fm[6] + p_1[2 * i + 1] * fm[7] + fm[8];
        // squared distance to epipolar line

        float tmp = l[0] * p_2[2 * i] + l[1] * p_2[2 * i + 1] + l[2];
        float distsqr = (tmp*tmp)/(l[0] * l[0] + l[1] * l[1]);
        
        if (distsqr < RANSAC_dist_threshold*RANSAC_dist_threshold) 
        {
            inliers[i] = 1;
            nin++;
            ave_dist += distsqr;
        } 
        else
            inliers[i] = 0;

    }
    
    ave_dist = sqrt(ave_dist);
    ave_dist /= (float) nin;
    *nInliers = nin;
    return (ave_dist);
}