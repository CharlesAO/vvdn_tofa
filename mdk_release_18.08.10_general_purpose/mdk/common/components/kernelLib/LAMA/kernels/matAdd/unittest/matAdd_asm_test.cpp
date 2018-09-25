#include "mv_types.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include "matAdd_asm_test.h"
#include <cstdio>

#define EDGE 16
TestRunner matAddTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int matAddCycleCount;

MAT_test *matAdd_asm_test(const MAT_test *A, const MAT_test *B, MAT_test *out)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

    matAddTestRunner.Init();
    matAddTestRunner.SetInput("input1", A->base, A->n * A->m, SHAVE0);
	matAddTestRunner.SetInput("input2", B->base, B->n * B->m, SHAVE0);
	matAddTestRunner.SetInput("output", out->base, out->n * out->m + EDGE, SHAVE0);
	
	
    matAddTestRunner.SetInput("width", A->n,        SHAVE0);
    matAddTestRunner.SetInput("height", A->m,        SHAVE0);
    
    matAddTestRunner.GuardInsert("output", SHAVE0, out->n * out->m + EDGE, out->base);
    matAddTestRunner.Run(SHAVE0);
    
    if((A->m >= 10) && (A->n >= 10))
    {
        matAddCycleCount = matAddTestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(matAddCycleCount - 23)/(float(A->n * A->m)));
    }

    matAddTestRunner.GetOutput("output", SHAVE0, out->n * out->m + EDGE, out->base);
    matAddTestRunner.GuardCheck("output", SHAVE0, out->n * out->m + EDGE, out->base);
	
	return out;
}
