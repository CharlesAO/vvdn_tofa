#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (42)


TestRunner mvcvCensusMin64TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMin64CycleCount;

void mvcvCensusMin64_asm_test(unsigned char *in1, unsigned char *out, unsigned char *outm, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int disparities = 64 ;
	const unsigned int PADDING = 32 ;
	
	
	mvcvCensusMin64TestRunner.Init();
	mvcvCensusMin64TestRunner.SetInput("input1", in1, width * disparities, SHAVE0);
	mvcvCensusMin64TestRunner.SetInput("output", out, width + PADDING, SHAVE0);
	mvcvCensusMin64TestRunner.SetInput("outputm", outm, width + PADDING, SHAVE0);
    mvcvCensusMin64TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	mvcvCensusMin64TestRunner.GuardInsert(string("output"), SHAVE0, width + PADDING, out);
	mvcvCensusMin64TestRunner.GuardInsert(string("outputm"), SHAVE0, width + PADDING, outm);
	mvcvCensusMin64TestRunner.Run(SHAVE0);
	if(width >= 960)
	{
	mvcvCensusMin64CycleCount = (mvcvCensusMin64TestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMin64CycleCount)/ (float)(width));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	mvcvCensusMin64TestRunner.GetOutput(string("output"), SHAVE0, width + PADDING, out);
	mvcvCensusMin64TestRunner.GuardCheck(string("output"), SHAVE0, width + PADDING, out);
	mvcvCensusMin64TestRunner.GetOutput(string("outputm"), SHAVE0, width + PADDING, outm);
	mvcvCensusMin64TestRunner.GuardCheck(string("outputm"), SHAVE0, width + PADDING, outm);
}
