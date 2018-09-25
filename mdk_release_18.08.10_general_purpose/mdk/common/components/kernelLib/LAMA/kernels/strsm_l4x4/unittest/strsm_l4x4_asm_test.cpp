#include "mv_types.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

TestRunner strsm_l4x4TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int strsm_l4x4CycleCount;

void strsm_l4x4_asm_test(float *a, float *b, float *c, int rs_c, int cs_c)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();

	int outputSize = 3 * (rs_c + cs_c) + 1;
	strsm_l4x4TestRunner.Init();
	strsm_l4x4TestRunner.SetInput("a", a, 4 * 4, SHAVE0);
	strsm_l4x4TestRunner.SetInput("b", b, 4 * 4, SHAVE0);
	strsm_l4x4TestRunner.SetInput("c", c, outputSize, SHAVE0);
	strsm_l4x4TestRunner.SetInput("rs_c", rs_c, SHAVE0);
	strsm_l4x4TestRunner.SetInput("cs_c", cs_c, SHAVE0);

	strsm_l4x4TestRunner.GuardInsert("c", SHAVE0, outputSize, c);
	strsm_l4x4TestRunner.Run(SHAVE0);
    strsm_l4x4CycleCount = strsm_l4x4TestRunner.GetVariableValue("cycleCount");
    functionInfo.AddCyclePerPixelInfo(strsm_l4x4CycleCount);

	strsm_l4x4TestRunner.GetOutput("c", SHAVE0, outputSize, c);
	strsm_l4x4TestRunner.GuardCheck("c", SHAVE0, outputSize, c);
}
