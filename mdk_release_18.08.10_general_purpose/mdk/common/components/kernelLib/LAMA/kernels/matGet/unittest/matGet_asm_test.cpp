#include "mv_types.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include "matGet_asm_test.h"
#include <cstdio>

TestRunner matGetTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int matGetCycleCount;

#define EDGE 16

MAT_test *matGet_asm_test(const MAT_test *in, MAT_test *out, int start_row, int start_col)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();

	unsigned int length;
	length = in->m;
	if(length < in->n)
		length = in->n;
	
    matGetTestRunner.Init();
    matGetTestRunner.SetInput("input1", in->base, length * length, SHAVE0);
	matGetTestRunner.SetInput("output", out->base, length * length + EDGE, SHAVE0);
	
	
    matGetTestRunner.SetInput("width", in->n,        SHAVE0);
    matGetTestRunner.SetInput("height", in->m,        SHAVE0);    
	
	matGetTestRunner.SetInput("width_out", out->n,        SHAVE0);
    matGetTestRunner.SetInput("height_out", out->m,        SHAVE0);
	
	matGetTestRunner.SetInput("start_row", start_row,        SHAVE0);
    matGetTestRunner.SetInput("start_col", start_col,        SHAVE0);
    
    matGetTestRunner.GuardInsert("output", SHAVE0, length * length + EDGE, out->base);
    matGetTestRunner.Run(SHAVE0);
    
    if((out->m >= 10) && (out->n >= 10))
    {
        matGetCycleCount = matGetTestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(matGetCycleCount - 23)/(float(out->n * out->m)));
    }

    matGetTestRunner.GetOutput("output", SHAVE0, length * length + EDGE, out->base);
    matGetTestRunner.GuardCheck("output", SHAVE0, length * length + EDGE, out->base);
	
	return out;
}
