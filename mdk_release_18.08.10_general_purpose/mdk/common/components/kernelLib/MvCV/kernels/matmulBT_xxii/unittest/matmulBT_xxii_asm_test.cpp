#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (10)



TestRunner mvcvMatmulBT_xxiiTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvMatmulBT_xxiiCycleCount;

void mvcvMatmulBT_xxii_asm_test(unsigned char *A, unsigned char *B, unsigned int *C, unsigned int m, unsigned int k, unsigned int n,
    unsigned int wA, unsigned int wB, unsigned int wC)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    
    mvcvMatmulBT_xxiiTestRunner.Init();
    mvcvMatmulBT_xxiiTestRunner.SetInput("matA", A, m * k , SHAVE0);
    mvcvMatmulBT_xxiiTestRunner.SetInput("matB", B, k * n , SHAVE0);
    mvcvMatmulBT_xxiiTestRunner.SetInput("matC", (unsigned char *)C, m * n * sizeof(unsigned int), SHAVE0);

    mvcvMatmulBT_xxiiTestRunner.SetInput("m", (unsigned int)m, SHAVE0);
    mvcvMatmulBT_xxiiTestRunner.SetInput("k", (unsigned int)k, SHAVE0);
    mvcvMatmulBT_xxiiTestRunner.SetInput("n", (unsigned int)n, SHAVE0);
    mvcvMatmulBT_xxiiTestRunner.SetInput("wA", (unsigned int)wA, SHAVE0);
    mvcvMatmulBT_xxiiTestRunner.SetInput("wB", (unsigned int)wB, SHAVE0);
    mvcvMatmulBT_xxiiTestRunner.SetInput("wC", (unsigned int)wC, SHAVE0);


    mvcvMatmulBT_xxiiTestRunner.GuardInsert(string("matC"), SHAVE0, m * n * sizeof(unsigned int), (unsigned char*)C);
    mvcvMatmulBT_xxiiTestRunner.Run(SHAVE0);
    if ((m==96) && (n==96))
    {
    mvcvMatmulBT_xxiiCycleCount = (mvcvMatmulBT_xxiiTestRunner.GetVariableValue(std::string("cycleCount"))) ;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvMatmulBT_xxiiCycleCount)/ (float)(m * n));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    mvcvMatmulBT_xxiiTestRunner.GetOutput(string("matC"), SHAVE0, m * n * sizeof(unsigned int), (unsigned char*)C);
    mvcvMatmulBT_xxiiTestRunner.GuardCheck(string("matC"), SHAVE0, m * n * sizeof(unsigned int), (unsigned char*)C);
}
