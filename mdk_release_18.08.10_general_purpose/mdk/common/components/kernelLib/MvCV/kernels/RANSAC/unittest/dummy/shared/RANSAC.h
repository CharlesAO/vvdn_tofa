#ifndef _RANSAC_H
#define _RANSAC_H

#define MAX_NUMPOINTS 1000



struct ransac_params
{
    float inliers_ratio;
    float confidence;
    float dist_threshold;
    int max_iterations;
    int nPoints;
};


struct fundamental_debug
{
    int RANSAC_support;
    int RANSAC_nInliers;
    int RANSAC_itterations;
    float RANSAC_inliers_ratio;

};





typedef struct RANSAC_Inputs
{
    float  *p1;
    float  *p2;
    struct ransac_params *Params;


} RANSAC_input;


typedef struct RANSAC_Outputs
{
    struct fundamental_debug *Debug;
    float *fm;
    int *inliers;

} RANSAC_output;





#endif
