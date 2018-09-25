#include "mv_types.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include "matTranspose_asm_test.h"
#include <cstdio>

#define EDGE 16

TestRunner matTransposeTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int matTransposeCycleCount;

MAT_test *matTranspose_asm_test(const MAT_test *A, MAT_test *out)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

    matTransposeTestRunner.Init();
    matTransposeTestRunner.SetInput("input1", A->base, A->n * A->m, SHAVE0);
	matTransposeTestRunner.SetInput("output", out->base, out->n * out->m + EDGE, SHAVE0);
	
	
    matTransposeTestRunner.SetInput("width", A->n,        SHAVE0);
    matTransposeTestRunner.SetInput("height", A->m,        SHAVE0);
    
    matTransposeTestRunner.GuardInsert("output", SHAVE0, out->n * out->m + EDGE, out->base);
    matTransposeTestRunner.Run(SHAVE0);
    
    if((A->m >= 10) && (A->n >= 10))
    {
        matTransposeCycleCount = matTransposeTestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(matTransposeCycleCount - 23)/(float(A->n * A->m)));
    }

    matTransposeTestRunner.GetOutput("output", SHAVE0, out->n * out->m + EDGE, out->base);
    matTransposeTestRunner.GuardCheck("output", SHAVE0, out->n * out->m + EDGE, out->base);
	
	return out;
}
