#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"

#define EXPECTED_CC_M1 (4.623047)
#define EXPECTED_CC_M2 (4.998047)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerPos(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int minMaxCycleCountPos;

void minMaxPos_asm_test(unsigned char** in, unsigned int width,
		unsigned char* minVal, unsigned char* maxVal, unsigned int* minPos, unsigned int* maxPos, unsigned char* maskAddr)
{
	unsigned int maxWidth = 1920;
	FunctionInfo& functionInfo = FunctionInfo::Instance();

	testRunnerPos.Init();
	testRunnerPos.SetInput("input", in, width, maxWidth, 1, SHAVE0);
	testRunnerPos.SetInput("width", width, SHAVE0);
	testRunnerPos.SetInput("minVal", minVal, 1, SHAVE0);
	testRunnerPos.SetInput("maxVal", maxVal, 1, SHAVE0);
	testRunnerPos.SetInput("minPos", (unsigned int)*minPos, SHAVE0);
	testRunnerPos.SetInput("maxPos", (unsigned int)*maxPos, SHAVE0);
	testRunnerPos.SetInput("mask", maskAddr, width, SHAVE0);

	testRunnerPos.GuardInsert("minVal", SHAVE0, minVal);
	testRunnerPos.GuardInsert("maxVal", SHAVE0, maxVal);
	testRunnerPos.GuardInsert("minPos", SHAVE0, minPos);
	testRunnerPos.GuardInsert("maxPos", SHAVE0, maxPos);
	testRunnerPos.Run(SHAVE0);
	if(width >= 1280)
	{
		minMaxCycleCountPos = testRunnerPos.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(minMaxCycleCountPos - 5)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerPos.GetOutput("minVal", SHAVE0, minVal);
	testRunnerPos.GuardCheck("minVal", SHAVE0, minVal);
	testRunnerPos.GetOutput("maxVal", SHAVE0, maxVal);
	testRunnerPos.GuardCheck("maxVal", SHAVE0, maxVal);
	testRunnerPos.GetOutput("minPos", SHAVE0, minPos);
	testRunnerPos.GuardCheck("minPos", SHAVE0, minPos);
	testRunnerPos.GetOutput("maxPos", SHAVE0, maxPos);
	testRunnerPos.GuardCheck("maxPos", SHAVE0, maxPos);
}
