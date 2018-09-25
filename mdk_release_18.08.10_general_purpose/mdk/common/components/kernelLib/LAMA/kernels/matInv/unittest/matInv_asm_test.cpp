#include "mv_types.h"
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include "matInv_asm_test.h"
#include <cstdio>

#define EDGE 16
TestRunner matInvTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int matInvCycleCount;

MAT_test *matInv_asm_test(MAT_test *A, MAT_test *out)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    matInvTestRunner.Init();
    matInvTestRunner.SetInput("input1", A->base, A->n * A->m , SHAVE0);
    matInvTestRunner.SetInput("output", out->base, out->n * out->m + EDGE, SHAVE0);
    matInvTestRunner.SetInput("width", A->n,        SHAVE0);
    matInvTestRunner.SetInput("height", A->m,        SHAVE0);
    
    matInvTestRunner.GuardInsert("output", SHAVE0, out->n * out->m + EDGE, out->base);
    matInvTestRunner.Run(SHAVE0);
    
    if((A->m >= 10) && (A->n >= 10))
    {
        matInvCycleCount = matInvTestRunner.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(matInvCycleCount - 23)/(float(A->n * A->m)));
    }

    matInvTestRunner.GetOutput("output", SHAVE0, out->n * out->m + EDGE, out->base);
    matInvTestRunner.GuardCheck("output", SHAVE0, out->n * out->m + EDGE, out->base);
    
    return out;
}
