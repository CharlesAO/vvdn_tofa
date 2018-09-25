#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (32)


TestRunner mvcvCensusMin16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMin16CycleCount;

void mvcvCensusMin16_asm_test(unsigned char *in1, unsigned char *out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int disparities = 16 ;
	const unsigned int PADDING = 32 ;
	
	
	mvcvCensusMin16TestRunner.Init();
	mvcvCensusMin16TestRunner.SetInput("input1", in1, width * disparities, SHAVE0);
	mvcvCensusMin16TestRunner.SetInput("output", out, width + PADDING, SHAVE0);
    mvcvCensusMin16TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	mvcvCensusMin16TestRunner.GuardInsert(string("output"), SHAVE0, width + PADDING, out);
	mvcvCensusMin16TestRunner.Run(SHAVE0);
	if(width >= 960)
	{
	mvcvCensusMin16CycleCount = (mvcvCensusMin16TestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMin16CycleCount)/ (float)(width));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	mvcvCensusMin16TestRunner.GetOutput(string("output"), SHAVE0, width + PADDING, out);
	mvcvCensusMin16TestRunner.GuardCheck(string("output"), SHAVE0, width + PADDING, out);
}
