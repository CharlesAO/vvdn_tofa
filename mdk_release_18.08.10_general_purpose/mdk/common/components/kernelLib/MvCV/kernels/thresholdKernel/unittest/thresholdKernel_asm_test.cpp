///
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.77)
#define EXPECTED_CC_M2 (0.911631)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner thresholdKernelTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int thresholdKernelCycleCount;

void thresholdKernel_asm_test(unsigned char** in, unsigned char** out, unsigned int width, unsigned int height, unsigned char thresh, unsigned int thresh_type)
{
	
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	
	thresholdKernelTestRunner.Init();

	thresholdKernelTestRunner.SetInput("input", in, width, maxWidth, height, SHAVE0);
	thresholdKernelTestRunner.SetInput("width", width, SHAVE0);
	thresholdKernelTestRunner.SetInput("height", height, SHAVE0);
	thresholdKernelTestRunner.SetInput("thresh", thresh, SHAVE0);
	thresholdKernelTestRunner.SetInput("thresh_type", thresh_type, SHAVE0);
	
	thresholdKernelTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, height, out);
	thresholdKernelTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		thresholdKernelCycleCount = thresholdKernelTestRunner.GetVariableValue("cycleCount");
		//value substracted from minMaxCycleCount is the value of measured cycles
		//for computations not included in thresholdKernel_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(thresholdKernelCycleCount - 46)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	thresholdKernelTestRunner.GetOutput("output", SHAVE0, width, maxWidth, height, out);
	thresholdKernelTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, height, out);
}
