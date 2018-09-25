///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    calcEpipolarDistance kernel function call for unitary test
///

#include "calcEpipolarDistance.h"
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920 

float input1[TEST_FRAME_WIDTH * 2];
float input2[TEST_FRAME_WIDTH * 2];
int output[TEST_FRAME_WIDTH];

int nPoints;
float threshold;
float fm[9];
int count;
float dist;

int main( void )
{
// float mvcvCalcEpipolarDistance(float *p_1, float *p_2, int nPoints, 
//                               float RANSAC_dist_threshold, float *fm, 
//                               int *inliers, int *nInliers)

    #ifdef UNIT_TEST_USE_C_VERSION
    dist = mvcvCalcEpipolarDistance(input1, input2, nPoints, threshold, fm, output, &count);
    #else
    dist = mvcvCalcEpipolarDistance_asm(input1, input2, nPoints, threshold, fm, output, &count);
    #endif

    SHAVE_HALT; 
}
