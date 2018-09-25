#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.22)
#define EXPECTED_CC_M2 (9.08)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner calcEpipolarDistanceTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int calcEpipolarDistanceCycleCount;

float calcEpipolarDistance_asm_test(float *p_1, float *p_2, int nPoints, 
                                  float RANSAC_dist_threshold, float *fm, 
                                  int *inliers, int *nInliers)
{
    float dist;    

    FunctionInfo& functionInfo = FunctionInfo::Instance();
    calcEpipolarDistanceTestRunner.Init();
    calcEpipolarDistanceTestRunner.SetInput("input1", p_1, 2*nPoints, SHAVE0);
    calcEpipolarDistanceTestRunner.SetInput("input2", p_2, 2*nPoints, SHAVE0);
    calcEpipolarDistanceTestRunner.SetInput("nPoints", nPoints, SHAVE0);
    calcEpipolarDistanceTestRunner.SetInput("threshold", RANSAC_dist_threshold, SHAVE0);
    calcEpipolarDistanceTestRunner.SetInput("fm", fm, 9, SHAVE0);

    calcEpipolarDistanceTestRunner.Run(SHAVE0);
    if(nPoints >= 1280)
    {
        calcEpipolarDistanceCycleCount = calcEpipolarDistanceTestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(calcEpipolarDistanceCycleCount - 2) / (float)nPoints);
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }

    calcEpipolarDistanceTestRunner.GetOutput("output", SHAVE0, nPoints, (float*)&inliers[0]);
    calcEpipolarDistanceTestRunner.GetOutput("count", SHAVE0, 1, (float*)nInliers);
    calcEpipolarDistanceTestRunner.GetOutput("dist", SHAVE0, 1, &dist);

    return dist;
}
