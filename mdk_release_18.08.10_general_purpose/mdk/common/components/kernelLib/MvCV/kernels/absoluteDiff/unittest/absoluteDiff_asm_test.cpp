#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.65)
#define EXPECTED_CC_M2 (0.485092)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner absoluteDiffTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int AbsoluteDiffCycleCount;

void AbsoluteDiff_asm_test(unsigned char** in1, unsigned char** in2, unsigned char** out, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	absoluteDiffTestRunner.Init();
	absoluteDiffTestRunner.SetInput("input1", in1, width, maxWidth, 1, SHAVE0);
	absoluteDiffTestRunner.SetInput("input2", in2, width, maxWidth, 1, SHAVE0);
	absoluteDiffTestRunner.SetInput("width", width, SHAVE0);

	absoluteDiffTestRunner.GuardInsert("output", SHAVE0, width, 1920, 1, out);
	absoluteDiffTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		AbsoluteDiffCycleCount = absoluteDiffTestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(AbsoluteDiffCycleCount - 5)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	absoluteDiffTestRunner.GetOutput("output", SHAVE0, width, 1920, 1, out);
	absoluteDiffTestRunner.GuardCheck("output", SHAVE0, width, 1920, 1, out);
}
