#include "mv_types.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include "matSet_asm_test.h"
#include <cstdio>

#define EDGE 16

TestRunner matSetTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int matSetCycleCount;

MAT_test *matSet_asm_test(float scalar, MAT_test *in, int start_row, int start_col, int length_row, int length_col)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

	unsigned int length;
	length = in->m;
	if (in->n > in->m)
		length = in->n;

    matSetTestRunner.Init();
    matSetTestRunner.SetInput("input1", in->base, length * length + EDGE, SHAVE0);
	
    matSetTestRunner.SetInput("width", in->n,        SHAVE0);
    matSetTestRunner.SetInput("height", in->m,        SHAVE0);
	
    matSetTestRunner.SetInput("scalar", scalar,        SHAVE0);
    matSetTestRunner.SetInput("start_row", start_row,        SHAVE0);
    matSetTestRunner.SetInput("start_col", start_col,        SHAVE0);
    matSetTestRunner.SetInput("length_row", length_row,        SHAVE0);
    matSetTestRunner.SetInput("length_col", length_col,        SHAVE0);
    
    matSetTestRunner.GuardInsert("input1", SHAVE0, length * length + EDGE, in->base);
    matSetTestRunner.Run(SHAVE0);
    
    if((in->m >= 10) && (in->n >= 10))
    {
        matSetCycleCount = matSetTestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(matSetCycleCount - 23)/(float(in->n * in->m)));
    }

    matSetTestRunner.GetOutput("input1", SHAVE0, length * length + EDGE, in->base);
    matSetTestRunner.GuardCheck("input1", SHAVE0, length * length + EDGE, in->base);
	
	return in;
}
