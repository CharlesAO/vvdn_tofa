#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (14)


TestRunner mvcvCensusMin3TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvCensusMin3CycleCount;

void mvcvCensusMin3_asm_test(unsigned char *in1, unsigned char *out, unsigned char *outm, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int disparities = 3 ;
	const unsigned int PADDING = 32 ;
	
	
	mvcvCensusMin3TestRunner.Init();
	mvcvCensusMin3TestRunner.SetInput("input1", in1, width * disparities, SHAVE0);
	mvcvCensusMin3TestRunner.SetInput("output", out, width + PADDING, SHAVE0);
	mvcvCensusMin3TestRunner.SetInput("outputm", outm, width + PADDING, SHAVE0);
    mvcvCensusMin3TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	mvcvCensusMin3TestRunner.GuardInsert(string("output"), SHAVE0, width + PADDING, out);
	mvcvCensusMin3TestRunner.GuardInsert(string("outputm"), SHAVE0, width + PADDING, outm);
	mvcvCensusMin3TestRunner.Run(SHAVE0);
	if(width >= 960)
	{
	mvcvCensusMin3CycleCount = (mvcvCensusMin3TestRunner.GetVariableValue(std::string("cycleCount"))) - 10;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvCensusMin3CycleCount)/ (float)(width));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	mvcvCensusMin3TestRunner.GetOutput(string("output"), SHAVE0, width + PADDING, out);
	mvcvCensusMin3TestRunner.GuardCheck(string("output"), SHAVE0, width + PADDING, out);
	mvcvCensusMin3TestRunner.GetOutput(string("outputm"), SHAVE0, width + PADDING, outm);
	mvcvCensusMin3TestRunner.GuardCheck(string("outputm"), SHAVE0, width + PADDING, outm);
}
