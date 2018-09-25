#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.50)
#define DISPARITIES 64
#define PADDING 16 

TestRunner mvcvAggregateThreePaths64TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvAggregateThreePaths64CycleCount;

void mvcvAggregateThreePaths64_asm_test(unsigned char *path0, unsigned char *path1, unsigned char *path2, unsigned char *out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	mvcvAggregateThreePaths64TestRunner.Init();
	mvcvAggregateThreePaths64TestRunner.SetInput(   "path0", path0, width * DISPARITIES, SHAVE0);
	mvcvAggregateThreePaths64TestRunner.SetInput(   "path1", path1, width * DISPARITIES, SHAVE0);
	mvcvAggregateThreePaths64TestRunner.SetInput(   "path2", path2, width * DISPARITIES, SHAVE0);
	mvcvAggregateThreePaths64TestRunner.SetInput(     "out", out, width * DISPARITIES + PADDING, SHAVE0);
    mvcvAggregateThreePaths64TestRunner.SetInput(   "width", (unsigned int)width, SHAVE0);

	mvcvAggregateThreePaths64TestRunner.GuardInsert(string("out"), SHAVE0, width * DISPARITIES + PADDING, out);
	mvcvAggregateThreePaths64TestRunner.Run(SHAVE0);

	mvcvAggregateThreePaths64CycleCount = (mvcvAggregateThreePaths64TestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvAggregateThreePaths64CycleCount)/ (float)(width * DISPARITIES));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	mvcvAggregateThreePaths64TestRunner.GetOutput(string("out"), SHAVE0, width * DISPARITIES + PADDING, out);
	mvcvAggregateThreePaths64TestRunner.GuardCheck(string("out"), SHAVE0, width * DISPARITIES + PADDING, out);
}
