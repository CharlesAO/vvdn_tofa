#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (130)


TestRunner mvcvCensusMin65TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMin65CycleCount;

void mvcvCensusMin65_asm_test(unsigned char *in1, unsigned char *out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int disparities = 65 ;
	const unsigned int PADDING = 32 ;
	
	
	mvcvCensusMin65TestRunner.Init();
	mvcvCensusMin65TestRunner.SetInput("input1", in1, width * disparities, SHAVE0);
	mvcvCensusMin65TestRunner.SetInput("output", out, width + PADDING, SHAVE0);
    mvcvCensusMin65TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	mvcvCensusMin65TestRunner.GuardInsert(string("output"), SHAVE0, width + PADDING, out);
	mvcvCensusMin65TestRunner.Run(SHAVE0);
	if(width >= 960)
	{
	mvcvCensusMin65CycleCount = (mvcvCensusMin65TestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMin65CycleCount)/ (float)(width));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	mvcvCensusMin65TestRunner.GetOutput(string("output"), SHAVE0, width + PADDING, out);
	mvcvCensusMin65TestRunner.GuardCheck(string("output"), SHAVE0, width + PADDING, out);
}
