
#include "../../../unittest/dummy/shared/FundamentalMatrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "../../../unittest/dummy/shared/RANSAC.h"

#include "fm8Point.h"



#include <mvcv_types.h>
#include "calcEpipolarDistance.h"



//#if 0
#ifdef __PC__
#define mvcvCalcEpipolarDistance_asm  mvcvCalcEpipolarDistance
#endif



#define printf(...)
#define rand PRNG


float pp_1[2 * POINTS_PER_SAMPLE];
float pp_2[2 * POINTS_PER_SAMPLE];
float est_fMatrix[9];
int inlierIndexes_tmp[MAX_NUMPOINTS];




unsigned int PRNG()
{
    // our initial starting seed is 5323
    static unsigned int nSeed = 5323;

    // Take the current seed and generate a new value from it
    // Due to our use of large constants and overflow, it would be
    // very hard for someone to predict what the next number is
    // going to be from the previous one.
    nSeed = (8253729 * nSeed + 2396403);

    // Take the seed and return a value between 0 and 32767
    return nSeed  % 32767;
}


void getInliers(float* pnts1, float* pnts2, int* inliers, int nPoints,
                int& nInliers, float* new_pnts1, float* new_pnts2)
{
    //Error handling
    if ((pnts1 == NULL) || (pnts2 == NULL) || (inliers == NULL)
            || (new_pnts1 == NULL) || (new_pnts2 == NULL))
    {

        exit(0);
    }
    nInliers = 0;
    for (int ii = 0; ii < nPoints; ii++)
    {
        if (inliers[ii] == 1)
        {
            new_pnts1[2 * nInliers] = pnts1[2 * ii];
            new_pnts1[2 * nInliers + 1] = pnts1[2 * ii + 1];

            new_pnts2[2 * nInliers] = pnts2[2 * ii];
            new_pnts2[2 * nInliers + 1] = pnts2[2 * ii + 1];

            nInliers++;
        }
    }

}


void findFundamentalMat(float *p_1,					// [IN] image 1 input points
                        float *p_2,						// [IN] image 2 input points
                        int nPoints,					// [IN] number of points
                        float *fm,						// [OUT] fundamental matrix
                        int *inliers,				// [OUT] inliers
                        struct fundamental_debug *Debug,
                        float RANSAC_inliers_ratio,		// [IN] precentage of inliers
                        float RANSAC_confidence,			// [IN] level of confidence
                        float RANSAC_dist_threshold,// [IN] max dist error for a point to be declared inlier
                        int RANSAC_max_iterations)   // [IN] max number of iterations
{
    srand (1);
    // 0. Config
    float inl = RANSAC_inliers_ratio * RANSAC_inliers_ratio;
    inl *= inl;
    inl *= inl;

    const int RANSAC_iterations = (int) (log(1 - RANSAC_confidence) / log(1 - inl) + 0.5);
    int RANSAC_support = (int) ((RANSAC_inliers_ratio + 0.1) * nPoints + 0.5);
    int RANSAC_nInliers = 0;

    Debug->RANSAC_support = RANSAC_support;
    Debug->RANSAC_inliers_ratio = RANSAC_inliers_ratio;


    // 1. Point normalization
//	float T1[9];
//	float T2[9];
//	auxiliaryFuncs::normalizePts(p_1, new_1, nPoints, T1);
//	auxiliaryFuncs::normalizePts(p_2, new_2, nPoints, T2);
    // Optimization: Compute T2 transpose only once
//	auxiliaryFuncs::mat3x3Transpose(T2);

    // 2. Run RANSAC
    int nInliers = 0;
    int iteration = 0;

    while ((iteration < RANSAC_iterations)
            && (iteration < RANSAC_max_iterations))
    {


        int randPoints[POINTS_PER_SAMPLE];

        for (int cnt = 0; cnt < POINTS_PER_SAMPLE; cnt++)
        {
            randPoints[cnt] = rand() % nPoints;

        }


        for (int j = 0; j < POINTS_PER_SAMPLE; j++)
        {
            pp_1[2 * j] = p_1[2 * randPoints[j]];
            pp_1[2 * j + 1] = p_1[2 * randPoints[j] + 1];
            pp_2[2 * j] = p_2[2 * randPoints[j]];
            pp_2[2 * j + 1] = p_2[2 * randPoints[j] + 1];
        }
        //

        fm8Point(pp_1, pp_2, est_fMatrix);

        // Fundamental matrix denormalize: F = T2^t * nF * T1
        // T2 is already trasposed
//		auxiliaryFuncs::mat3x3Mult(T2, est_fMatrix, tmp);  // tmp <-- T2^t * nFm
//		auxiliaryFuncs::mat3x3Mult(tmp, T1, est_fMatrix); // Fm <-- T2^t * nFm * T1


        // Compute reprojection error and number of inliers
        nInliers = 0;
        mvcvCalcEpipolarDistance_asm(&p_1[0], &p_2[0], nPoints,
                                     RANSAC_dist_threshold, &est_fMatrix[0], &inlierIndexes_tmp[0],
                                     &nInliers);

        if (nInliers > RANSAC_nInliers)
        {

            for (int cnt = 0; cnt < 9; cnt++)
            {
                fm[cnt] = est_fMatrix[cnt];
            }

            memcpy((int*) (inliers), (int*) (inlierIndexes_tmp),
                   nPoints * sizeof(inliers[0]));

            RANSAC_nInliers = nInliers;


            if (RANSAC_nInliers > RANSAC_support)
            {
                iteration ++;
                break;
            }
        }
        iteration++;

    }


    Debug->RANSAC_nInliers = RANSAC_nInliers;
    Debug->RANSAC_itterations = iteration;



}
