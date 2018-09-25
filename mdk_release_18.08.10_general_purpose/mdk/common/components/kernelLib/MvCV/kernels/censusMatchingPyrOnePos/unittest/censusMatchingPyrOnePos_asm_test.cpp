#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (3.68)



TestRunner mvcvCensusMatchingPyrOnePosTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMatchingPyrOnePosCycleCount;

void mvcvCensusMatchingPyrOnePos_asm_test(unsigned int *in1, unsigned int *in2, unsigned char *predicted, unsigned char *out, unsigned int width)
{
    unsigned int disparities = 3;
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int padding = 32 ;
	
	
	mvcvCensusMatchingPyrOnePosTestRunner.Init();
	mvcvCensusMatchingPyrOnePosTestRunner.SetInput("input1", in1, width + padding, SHAVE0);
	mvcvCensusMatchingPyrOnePosTestRunner.SetInput("input2", in2, width + padding, SHAVE0);
	mvcvCensusMatchingPyrOnePosTestRunner.SetInput("predicted", predicted, width + padding, SHAVE0);
	mvcvCensusMatchingPyrOnePosTestRunner.SetInput("output", out, width * disparities + padding, SHAVE0);
    mvcvCensusMatchingPyrOnePosTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	mvcvCensusMatchingPyrOnePosTestRunner.GuardInsert(string("output"), SHAVE0, width * disparities + padding, out);
	mvcvCensusMatchingPyrOnePosTestRunner.Run(SHAVE0);
	if (width>=960)
	{
	mvcvCensusMatchingPyrOnePosCycleCount = (mvcvCensusMatchingPyrOnePosTestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMatchingPyrOnePosCycleCount)/ (float)(width * disparities));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	mvcvCensusMatchingPyrOnePosTestRunner.GetOutput(string("output"), SHAVE0, width * disparities + padding, out);
	mvcvCensusMatchingPyrOnePosTestRunner.GuardCheck(string("output"), SHAVE0, width * disparities + padding, out);
}
