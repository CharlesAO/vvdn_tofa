#ifndef _CALC_EPIPOLAR_DIST_ASM_TEST_H_
#define _CALC_EPIPOLAR_DIST_ASM_TEST_H_

extern unsigned int calcEpipolarDistanceCycleCount;

float calcEpipolarDistance_asm_test(float *p_1, float *p_2, int nPoints, 
                                  float RANSAC_dist_threshold, float *fm, 
                                  int *inliers, int *nInliers);
// float calcEpipolarDistance_asm_test(float *p_1, float *p_2, int nPoints, 
//                                        float RANSAC_dist_threshold, float *fm, 
//                                        int *inliers, int *nInliers);
// float calcEpipolarDistance_asm_test(float* a, float* b, int  c, float d, float* e, int* f, int* g);
#endif //_CALC_EPIPOLAR_DIST_ASM_TEST_H_
