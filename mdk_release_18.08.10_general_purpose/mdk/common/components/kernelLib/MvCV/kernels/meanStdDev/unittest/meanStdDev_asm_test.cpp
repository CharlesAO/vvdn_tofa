///
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.51)
#define EXPECTED_CC_M2 (0.539749)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner meanstddevTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int meanstddevCycleCount;

void meanstddev_asm_test(unsigned char** in, float *mean, float *stdev, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	meanstddevTestRunner.Init();
	meanstddevTestRunner.SetInput("input", in, width, maxWidth, 1, SHAVE0);
	meanstddevTestRunner.SetInput("width", width, SHAVE0);
	meanstddevTestRunner.SetInput("mean", mean, 1, SHAVE0);
	meanstddevTestRunner.SetInput("stdev", stdev, 1, SHAVE0);
	meanstddevTestRunner.GuardInsert("mean", SHAVE0, 1, mean);
	meanstddevTestRunner.GuardInsert("stdev", SHAVE0, 1, stdev);
	meanstddevTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		meanstddevCycleCount = meanstddevTestRunner.GetVariableValue(std::string("cycleCount"));
		//the value substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(meanstddevCycleCount - 7)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	meanstddevTestRunner.GetOutput("mean", SHAVE0, 1, mean);
	meanstddevTestRunner.GuardCheck("mean", SHAVE0, 1, mean);
	meanstddevTestRunner.GetOutput("stdev", SHAVE0, 1, stdev);
	meanstddevTestRunner.GuardCheck("stdev", SHAVE0, 1, stdev);
}
