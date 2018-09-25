#include "mv_types.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include "matMinus_asm_test.h"
#include <cstdio>

#define EDGE 16
TestRunner matMinusTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int matMinusCycleCount;

MAT_test *matMinus_asm_test(const MAT_test *A, const MAT_test *B, MAT_test *out)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

    matMinusTestRunner.Init();
    matMinusTestRunner.SetInput("input1", A->base, A->n * A->m, SHAVE0);
	matMinusTestRunner.SetInput("input2", B->base, B->n * B->m, SHAVE0);
	matMinusTestRunner.SetInput("output", out->base, out->n * out->m + EDGE, SHAVE0);
	
	
    matMinusTestRunner.SetInput("width", A->n,        SHAVE0);
    matMinusTestRunner.SetInput("height", A->m,        SHAVE0);
    
    matMinusTestRunner.GuardInsert("output", SHAVE0, out->n * out->m + EDGE, out->base);
    matMinusTestRunner.Run(SHAVE0);
    
    if((A->m >= 10) && (A->n >= 10))
    {
        matMinusCycleCount = matMinusTestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(matMinusCycleCount - 23)/(float(A->n * A->m)));
    }

    matMinusTestRunner.GetOutput("output", SHAVE0, out->n * out->m + EDGE, out->base);
    matMinusTestRunner.GuardCheck("output", SHAVE0, out->n * out->m + EDGE, out->base);
	
	return out;
}
