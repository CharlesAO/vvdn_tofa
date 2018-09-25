#include "mv_types.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

TestRunner sgemm4x4TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int sgemm4x4CycleCount;

void sgemm4x4_asm_test(s32 k, float *alpha, float *a, float *b, float *beta, float *c, s32 rs_c, s32 cs_c)
{
	//FunctionInfo& functionInfo = FunctionInfo::Instance();

	sgemm4x4TestRunner.Init();
	sgemm4x4TestRunner.SetInput("k",         k,        SHAVE0);
	sgemm4x4TestRunner.SetInput("alpha", alpha,     1, SHAVE0);
    sgemm4x4TestRunner.SetInput("A",         a, 4 * k, SHAVE0);
    sgemm4x4TestRunner.SetInput("B",         b, 4 * k, SHAVE0);
    sgemm4x4TestRunner.SetInput("beta",   beta,     1, SHAVE0);
    sgemm4x4TestRunner.SetInput("C",         c, 4 * 4, SHAVE0);
    sgemm4x4TestRunner.SetInput("rs_c",   rs_c,        SHAVE0);
    sgemm4x4TestRunner.SetInput("cs_c",   cs_c,        SHAVE0);

	sgemm4x4TestRunner.GuardInsert("C", SHAVE0, 4 * 4, c);
	sgemm4x4TestRunner.Run(SHAVE0);
    sgemm4x4CycleCount = sgemm4x4TestRunner.GetVariableValue("cycleCount");
    //functionInfo.AddCyclePerPixelInfo((float)sgemm4x4CycleCount/ (float)(4*k));
	sgemm4x4TestRunner.GetOutput("C", SHAVE0, 4 * 4, c);
	sgemm4x4TestRunner.GuardCheck("C", SHAVE0, 4 * 4, c);
}
