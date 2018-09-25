#include "dummy/shared/RANSAC.h"
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>
#define EXPECTED_CC (3)

#ifndef PC
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

EXTERN void RANSAC_Init();
EXTERN void Init();
EXTERN void Run();
unsigned int RANSACAppCycleCount;

TestRunner RANSACAppTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int SampleAppCycleCount;
float p1_out[2*MAX_NUMPOINTS];

void RANSAC_asm_test(float *p1, float *p2, float *fm, int *Inliers, struct ransac_params *Params, struct fundamental_debug *Debug)
{

    //FunctionInfo& functionInfo = FunctionInfo::Instance();

    RANSACAppTestRunner.Init();
    // Write new inputs

    RANSACAppTestRunner.SetInput("p1", p1, 2*MAX_NUMPOINTS, LEON_OS);
    RANSACAppTestRunner.SetInput("p2", p2, 2*MAX_NUMPOINTS, LEON_OS);
    RANSACAppTestRunner.SetInput("Params", (unsigned char*)Params,sizeof(ransac_params), LEON_OS);
    RANSACAppTestRunner.SetInput("cmxInliers", (unsigned char*)Inliers, sizeof(Inliers[0])*MAX_NUMPOINTS, SHAVE0);
    RANSACAppTestRunner.SetInput("fm", fm, 9, LEON_OS);
    RANSACAppTestRunner.SetInput("Debug", (unsigned char*)Debug, sizeof(fundamental_debug), LEON_OS);

    RANSACAppTestRunner.GuardInsert(string("cmxInliers"), SHAVE0, sizeof(Inliers[0])*MAX_NUMPOINTS, (unsigned char*)Inliers);
    RANSACAppTestRunner.GuardInsert(string("fm"), LEON_OS, 9, fm);
    RANSACAppTestRunner.GuardInsert(string("Debug"), LEON_OS, sizeof(fundamental_debug),(unsigned char*)Debug);
    RANSACAppTestRunner.Run(LEON_OS, "startTest");
    RANSACAppTestRunner.Run(LEON_OS, "RANSAC_Init");
    RANSACAppTestRunner.Run(SHAVE0);
    RANSACAppCycleCount = RANSACAppTestRunner.GetVariableValue(std::string("cycleCount"));
//	functionInfo.AddCyclePerPixelInfo((float)(SampleAppCycleCount - 2)/ (float)wcwidth);
//	functionInfo.setExpectedCycles((float)EXPECTED_CC
    RANSACAppTestRunner.GetOutput(string("cmxInliers"), SHAVE0, sizeof(Inliers[0])*MAX_NUMPOINTS, (unsigned char*)Inliers);
    RANSACAppTestRunner.GetOutput(string("fm"), LEON_OS, 9, fm);
    RANSACAppTestRunner.GetOutput(string("Debug"), LEON_OS, sizeof(fundamental_debug),(unsigned char*)Debug);
    
    RANSACAppTestRunner.GuardCheck(string("cmxInliers"), SHAVE0, sizeof(Inliers[0])*MAX_NUMPOINTS, (unsigned char*)Inliers);
    RANSACAppTestRunner.GuardCheck(string("fm"), LEON_OS, 9, fm);
    RANSACAppTestRunner.GuardCheck(string("Debug"), LEON_OS, sizeof(fundamental_debug),(unsigned char*)Debug);

    //printf("RANSACAppCycleCount:%d\n",RANSACAppCycleCount);
    //printf("RANSAC_itterations:%d\n",Debug->RANSAC_itterations);
    //printf("CycleCountperIteration:%d\n",RANSACAppCycleCount/Debug->RANSAC_itterations);


}
