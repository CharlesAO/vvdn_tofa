#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"

#define EXPECTED_CC_M1 (0.91)
#define EXPECTED_CC_M2 (0.973307)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int minMaxCycleCount;

void minMaxKernel_asm_test(unsigned char** in, unsigned int width, unsigned int height,
		unsigned char* minVal, unsigned char* maxVal, unsigned char* maskAddr)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned char* l = in[0];

	testRunner.Init();
	testRunner.SetInput("input", l, width, SHAVE0);
	testRunner.SetInput("width", width, SHAVE0);
	testRunner.SetInput("height", height, SHAVE0);
	testRunner.SetInput("minVal", *minVal, SHAVE0);
	testRunner.SetInput("maxVal", *maxVal, SHAVE0);
	testRunner.SetInput("mask", maskAddr, width*height, SHAVE0);

	testRunner.GuardInsert("minVal", SHAVE0, minVal);
	testRunner.GuardInsert("maxVal", SHAVE0, maxVal);
	testRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		minMaxCycleCount = testRunner.GetVariableValue(std::string("cycleCount"));
		//value 5 substracted from minMaxCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(minMaxCycleCount - 5)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunner.GetOutput("minVal", SHAVE0, minVal);
	testRunner.GuardCheck("minVal", SHAVE0, minVal);
	testRunner.GetOutput("maxVal", SHAVE0, maxVal);
	testRunner.GuardCheck("maxVal", SHAVE0, maxVal);
}

