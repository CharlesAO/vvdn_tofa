#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (14)


TestRunner mvcvCensusMin7TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMin7CycleCount;

void mvcvCensusMin7_asm_test(unsigned char *in1, unsigned char *out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int disparities = 7 ;
	const unsigned int PADDING = 32 ;
	
	
	mvcvCensusMin7TestRunner.Init();
	mvcvCensusMin7TestRunner.SetInput("input1", in1, width * disparities, SHAVE0);
	mvcvCensusMin7TestRunner.SetInput("output", out, width + PADDING, SHAVE0);
    mvcvCensusMin7TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	mvcvCensusMin7TestRunner.GuardInsert(string("output"), SHAVE0, width + PADDING, out);
	mvcvCensusMin7TestRunner.Run(SHAVE0);
	if(width >= 960)
	{
	mvcvCensusMin7CycleCount = (mvcvCensusMin7TestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMin7CycleCount)/ (float)(width));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	mvcvCensusMin7TestRunner.GetOutput(string("output"), SHAVE0, width + PADDING, out);
	mvcvCensusMin7TestRunner.GuardCheck(string("output"), SHAVE0, width + PADDING, out);
}
