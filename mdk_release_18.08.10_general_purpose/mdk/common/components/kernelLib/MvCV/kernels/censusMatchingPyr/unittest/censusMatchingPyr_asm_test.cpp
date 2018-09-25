#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (1.86)



TestRunner mvcvCensusMatchingPyrTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMatchingPyrCycleCount;

void mvcvCensusMatchingPyr_asm_test(unsigned int *in1, unsigned int *in2, unsigned char *predicted, unsigned char *out, unsigned int width)
{
    unsigned int disparities = 7;
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int padding = 32 ;
	
	
	mvcvCensusMatchingPyrTestRunner.Init();
	mvcvCensusMatchingPyrTestRunner.SetInput("input1", in1, width + padding, SHAVE0);
	mvcvCensusMatchingPyrTestRunner.SetInput("input2", in2, width + padding, SHAVE0);
	mvcvCensusMatchingPyrTestRunner.SetInput("predicted", predicted, width + padding, SHAVE0);
	mvcvCensusMatchingPyrTestRunner.SetInput("output", out, width * disparities + padding, SHAVE0);
    mvcvCensusMatchingPyrTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	mvcvCensusMatchingPyrTestRunner.GuardInsert(string("output"), SHAVE0, width * disparities + padding, out);
	mvcvCensusMatchingPyrTestRunner.Run(SHAVE0);
	if (width>=960)
	{
	mvcvCensusMatchingPyrCycleCount = (mvcvCensusMatchingPyrTestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMatchingPyrCycleCount)/ (float)(width * disparities));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	mvcvCensusMatchingPyrTestRunner.GetOutput(string("output"), SHAVE0, width * disparities + padding, out);
	mvcvCensusMatchingPyrTestRunner.GuardCheck(string("output"), SHAVE0, width * disparities + padding, out);
}
