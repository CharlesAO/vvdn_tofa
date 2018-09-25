#include "mv_types.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

TestRunner matVecMulTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int matVecMulCycleCount;

void matVecMul_asm_test(float *a, float *b, float *c, int k, int n)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

    matVecMulTestRunner.Init();
    matVecMulTestRunner.SetInput("k", k,        SHAVE0);
    matVecMulTestRunner.SetInput("n", n,        SHAVE0);
    matVecMulTestRunner.SetInput("A", a, k,     SHAVE0);
    matVecMulTestRunner.SetInput("B", b, k * n, SHAVE0);
    matVecMulTestRunner.SetInput("C", c, n,     SHAVE0);

    matVecMulTestRunner.GuardInsert("C", SHAVE0, n, c);
    matVecMulTestRunner.Run(SHAVE0);

    if((k == 128) & (n == 128))
    {
        matVecMulCycleCount = matVecMulTestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)matVecMulCycleCount);
    }

    matVecMulTestRunner.GetOutput("C", SHAVE0, n, c);
    matVecMulTestRunner.GuardCheck("C", SHAVE0, n, c);
}
