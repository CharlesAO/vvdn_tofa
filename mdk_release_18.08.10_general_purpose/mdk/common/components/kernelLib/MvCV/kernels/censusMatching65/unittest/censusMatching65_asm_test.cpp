#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (0.73)


TestRunner mvcvCensusMatching65TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMatching65CycleCount;

void mvcvCensusMatching65_asm_test(unsigned int *in1, unsigned int *in2, unsigned char *out, unsigned int width)
{
    unsigned int disparities = 65;
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int padding = 65 ;
	
	
	mvcvCensusMatching65TestRunner.Init();
	mvcvCensusMatching65TestRunner.SetInput("input1", in1, width + padding, SHAVE0);
	mvcvCensusMatching65TestRunner.SetInput("input2", in2, width + padding, SHAVE0);
	mvcvCensusMatching65TestRunner.SetInput("output", out, width * disparities + padding, SHAVE0);
    mvcvCensusMatching65TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	mvcvCensusMatching65TestRunner.GuardInsert(string("output"), SHAVE0, width * disparities + padding, out);
	mvcvCensusMatching65TestRunner.Run(SHAVE0);
	if(width >= 960)
	{
	mvcvCensusMatching65CycleCount = (mvcvCensusMatching65TestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMatching65CycleCount)/ (float)(width * disparities));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	mvcvCensusMatching65TestRunner.GetOutput(string("output"), SHAVE0, width * disparities + padding, out);
	mvcvCensusMatching65TestRunner.GuardCheck(string("output"), SHAVE0, width * disparities + padding, out);
}
