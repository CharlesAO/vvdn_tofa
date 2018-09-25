#include "mv_types.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include "matQRDecomp_asm_test.h"
#include <cstdio>

#define EDGE 16
TestRunner matQRDecompTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int matQRDecompCycleCount;

void matQRDecomp_asm_test(MAT_test *A, MAT_test *Q, MAT_test *R)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

	unsigned int max = A->n;
	if (A->m > max ) max = A->m;

    matQRDecompTestRunner.Init();
    matQRDecompTestRunner.SetInput("input",   A->base,  max * max + EDGE, SHAVE0);
    matQRDecompTestRunner.SetInput("output1", Q->base,  max * max + EDGE, SHAVE0);
    matQRDecompTestRunner.SetInput("output2", R->base,  max * max + EDGE, SHAVE0);

    matQRDecompTestRunner.SetInput("width", A->n,        SHAVE0);
    matQRDecompTestRunner.SetInput("height",A->m,        SHAVE0);
    

    matQRDecompTestRunner.GuardInsert("output1", SHAVE0, max * max + EDGE, Q->base);
    matQRDecompTestRunner.GuardInsert("output2", SHAVE0, max * max + EDGE, R->base);
    matQRDecompTestRunner.Run(SHAVE0);
    
    if((A->m >= 10) && (A->n >= 10))
    {
        matQRDecompCycleCount = matQRDecompTestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(matQRDecompCycleCount - 23)/(float(A->n * A->m)));
    }

    matQRDecompTestRunner.GetOutput("output1", SHAVE0, max * max + EDGE, Q->base);
    matQRDecompTestRunner.GuardCheck("output1", SHAVE0, max * max + EDGE, Q->base);
    matQRDecompTestRunner.GetOutput("output2", SHAVE0, max * max + EDGE, R->base);
    matQRDecompTestRunner.GuardCheck("output2", SHAVE0, max * max + EDGE, R->base);
	
}
