#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.50)
#define DISPARITIES 64
#define PADDING 16 

TestRunner mvcvAggregateAllPaths64TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvAggregateAllPaths64CycleCount;

void mvcvAggregateAllPaths64_asm_test(unsigned char *path0, unsigned char *path1, unsigned char *path2, unsigned char *path3, unsigned char *out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	mvcvAggregateAllPaths64TestRunner.Init();
	mvcvAggregateAllPaths64TestRunner.SetInput(   "path0", path0, width * DISPARITIES, SHAVE0);
	mvcvAggregateAllPaths64TestRunner.SetInput(   "path1", path1, width * DISPARITIES, SHAVE0);
	mvcvAggregateAllPaths64TestRunner.SetInput(   "path2", path2, width * DISPARITIES, SHAVE0);
	mvcvAggregateAllPaths64TestRunner.SetInput(   "path3", path3, width * DISPARITIES, SHAVE0);
	mvcvAggregateAllPaths64TestRunner.SetInput(   "output",  out, width * DISPARITIES + PADDING, SHAVE0);
    mvcvAggregateAllPaths64TestRunner.SetInput(   "width", (unsigned int)width, SHAVE0);

	mvcvAggregateAllPaths64TestRunner.GuardInsert(string("output"), SHAVE0, width * DISPARITIES + PADDING, out);
	mvcvAggregateAllPaths64TestRunner.Run(SHAVE0);

	mvcvAggregateAllPaths64CycleCount = (mvcvAggregateAllPaths64TestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvAggregateAllPaths64CycleCount)/ (float)(width * DISPARITIES));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	mvcvAggregateAllPaths64TestRunner.GetOutput(string("output"), SHAVE0, width * DISPARITIES + PADDING, out);
	mvcvAggregateAllPaths64TestRunner.GuardCheck(string("output"), SHAVE0, width * DISPARITIES + PADDING, out);
}
