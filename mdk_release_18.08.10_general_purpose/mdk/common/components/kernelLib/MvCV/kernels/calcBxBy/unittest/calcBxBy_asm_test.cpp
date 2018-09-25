#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include <mv_types.h>
#include <mvcv_types.h>

#define EXPECTED_CC_M1 (999)
#define EXPECTED_CC_M2 (4.37)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner calcBxByTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int calcBxByCycleCount;

float* calcBxBy_asm_test(unsigned char* patchI, unsigned int* isz, float* Ix, float* Iy, 
                unsigned int* minI, unsigned char* patchJ, unsigned int* jsz, unsigned int* minJ, float* errorPtr)

{
    float *retVal_fp32;
    retVal_fp32 = (float*)malloc(sizeof(float));
    
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    unsigned int maxWidth = 1920;
    calcBxByTestRunner.Init();
    calcBxByTestRunner.SetInput("patchI", patchI, maxWidth, SHAVE0);
    calcBxByTestRunner.SetInput("patchJ", patchJ, maxWidth, SHAVE0);
    calcBxByTestRunner.SetInput("isz_u32", isz, 2, SHAVE0);
    calcBxByTestRunner.SetInput("jsz_u32", jsz, 2, SHAVE0);
    calcBxByTestRunner.SetInput("Ix", Ix, maxWidth, SHAVE0);
    calcBxByTestRunner.SetInput("Iy", Iy, maxWidth, SHAVE0);
    calcBxByTestRunner.SetInput("minI_u32", minI, 2, SHAVE0);
    calcBxByTestRunner.SetInput("minJ_u32", minJ, 2, SHAVE0);
    
    calcBxByTestRunner.GuardInsert("retVal_fp32", SHAVE0, 2, retVal_fp32);
    calcBxByTestRunner.GuardInsert("errorPtr", SHAVE0, 1, errorPtr);
    calcBxByTestRunner.Run(SHAVE0);
    calcBxByCycleCount = calcBxByTestRunner.GetVariableValue("cycleCount");
    functionInfo.AddCyclePerPixelInfo((float)(calcBxByCycleCount - 23) / (float)(jsz[0]*jsz[1]));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);

    calcBxByTestRunner.GetOutput("retVal_fp32", SHAVE0, 2, retVal_fp32);
    calcBxByTestRunner.GuardCheck("retVal_fp32", SHAVE0, 2, retVal_fp32);
    calcBxByTestRunner.GetOutput("errorPtr", SHAVE0, 1, errorPtr);
    calcBxByTestRunner.GuardCheck("errorPtr", SHAVE0, 1, errorPtr);

    return retVal_fp32;
}
