#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (0.7)


TestRunner mvcvCensusMatching16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMatching16CycleCount;

void mvcvCensusMatching16_asm_test(unsigned int *in1, unsigned int *in2, unsigned char *out, unsigned int width)
{
    unsigned int disparities = 16;
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int padding = 16 ;
	
	
	mvcvCensusMatching16TestRunner.Init();
	mvcvCensusMatching16TestRunner.SetInput("input1", in1, width + padding, SHAVE0);
	mvcvCensusMatching16TestRunner.SetInput("input2", in2, width + padding, SHAVE0);
	mvcvCensusMatching16TestRunner.SetInput("output", out, width * disparities + padding, SHAVE0);
    mvcvCensusMatching16TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	mvcvCensusMatching16TestRunner.GuardInsert(string("output"), SHAVE0, width * disparities + padding, out);
	mvcvCensusMatching16TestRunner.Run(SHAVE0);
	if(width >= 960)
	{
	mvcvCensusMatching16CycleCount = (mvcvCensusMatching16TestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMatching16CycleCount)/ (float)(width * disparities));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	mvcvCensusMatching16TestRunner.GetOutput(string("output"), SHAVE0, width * disparities + padding, out);
	mvcvCensusMatching16TestRunner.GuardCheck(string("output"), SHAVE0, width * disparities + padding, out);
}
