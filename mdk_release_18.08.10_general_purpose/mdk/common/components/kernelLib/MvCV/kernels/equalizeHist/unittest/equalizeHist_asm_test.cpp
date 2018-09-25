#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (4.99)
#define EXPECTED_CC_M2 (5.5)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerEqualizeHist(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int equalizeHistCycleCount;

void equalizeHist_asm_test(unsigned char** in, unsigned char** out, unsigned int* hist, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();

	unsigned int maxWidth = 1920;

	testRunnerEqualizeHist.Init();
	testRunnerEqualizeHist.SetInput("input", in, width, maxWidth, 1, SHAVE0);
	testRunnerEqualizeHist.SetInput("hist", (unsigned char*)hist, 256 * 4, SHAVE0);
	testRunnerEqualizeHist.SetInput("width", width, SHAVE0);
	testRunnerEqualizeHist.GuardInsert("output", SHAVE0, width * 4, maxWidth, 1, out);
	testRunnerEqualizeHist.GuardInsert("hist", SHAVE0, 256 * 4, maxWidth, 1, (unsigned char**)&hist);

	testRunnerEqualizeHist.Run(SHAVE0);
	if(width >= 1280)
	{
		equalizeHistCycleCount = testRunnerEqualizeHist.GetVariableValue("cycleCount");
	
		functionInfo.AddCyclePerPixelInfo((float)(equalizeHistCycleCount - 12)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerEqualizeHist.GetOutput("output", SHAVE0, width * 4, maxWidth, 1, out);
	testRunnerEqualizeHist.GuardCheck("output", SHAVE0, width * 4, maxWidth, 1, out);
	testRunnerEqualizeHist.GetOutput("hist", SHAVE0, 256 * 4, maxWidth, 1, (unsigned char**)&hist);
	testRunnerEqualizeHist.GuardCheck("hist", SHAVE0, 256 * 4, maxWidth, 1, (unsigned char**)&hist);
}

