#include "integralImageSumU16U32.h"
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (1.287037)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerIntegral(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int integralImageSumU16U32CycleCount;

void integralImageSumU16U32_asm_test(unsigned int* out, unsigned short* in, unsigned int runNr, unsigned int **previsionOutLnPointer, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();

    previsionOutLnPointer[0]+=1;
	testRunnerIntegral.Init();
	testRunnerIntegral.SetInput("in", (unsigned char*)in, width * 2, SHAVE0);
	testRunnerIntegral.SetInput("width", width, SHAVE0);
	testRunnerIntegral.SetInput("runNr", runNr, SHAVE0);
	testRunnerIntegral.GuardInsert("output", SHAVE0, width * 4,  (unsigned char * ) out);
	testRunnerIntegral.Run(SHAVE0);
	if(width >= 1280)
	{
	integralImageSumU16U32CycleCount = testRunnerIntegral.GetVariableValue("cycleCount");
	functionInfo.AddCyclePerPixelInfo((float)(integralImageSumU16U32CycleCount - 3)/ (float)width);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerIntegral.GetOutput("output", SHAVE0, width * 4,  (unsigned char * ) out);
	testRunnerIntegral.GuardCheck("output", SHAVE0, width * 4,  (unsigned char * ) out);
	
}

