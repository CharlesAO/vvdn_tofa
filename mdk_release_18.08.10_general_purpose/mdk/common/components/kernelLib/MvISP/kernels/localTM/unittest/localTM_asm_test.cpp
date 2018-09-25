#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (13.07)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner localTMTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int localTMCycleCount;

void localTM_asm_test(half** luma_in, unsigned char** bg8, half** output, half *curves, unsigned int width, unsigned int run_no)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int padding = 16;
	unsigned int maxWidth = 1920 + padding; //adding 32 only for purpose of testing the bounders
	localTMTestRunner.Init();
	localTMTestRunner.SetInput("input1", luma_in, width, maxWidth, 1, SHAVE0);
	localTMTestRunner.SetInput("output", output, width + padding, maxWidth, 1, SHAVE0);
	localTMTestRunner.SetInput("input2", bg8, width + padding, maxWidth, 2, SHAVE0);
	localTMTestRunner.SetInput("cMat", curves, 160, SHAVE0);
	localTMTestRunner.SetInput("width", width, SHAVE0);
	localTMTestRunner.SetInput("run_no", run_no, SHAVE0);
	localTMTestRunner.GuardInsert("output", SHAVE0, width + padding, maxWidth, 1, output); 
	localTMTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		localTMCycleCount = localTMTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(localTMCycleCount - 11) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	localTMTestRunner.GetOutput("output", SHAVE0, width + padding, maxWidth, 1, output); //adding 16 only for purpose of testing the bounders
	localTMTestRunner.GuardCheck("output", SHAVE0, width + padding, maxWidth, 1, output); //adding 16 only for purpose of testing the bounders

}
