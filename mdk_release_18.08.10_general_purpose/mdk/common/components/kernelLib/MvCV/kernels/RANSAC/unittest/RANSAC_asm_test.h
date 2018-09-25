#ifndef _RANSACAPP_IMPL_TEST_H_
#define _RANSACAPP_IMPL_TEST_H_

extern unsigned int RANSACAppCycleCount;

void RANSAC_asm_test(float *p1, float *p2, float *fm, int *Inliers, struct ransac_params *Params, struct fundamental_debug *Debug);

#endif //_RANSACAPP_IMPL_TEST_H_
