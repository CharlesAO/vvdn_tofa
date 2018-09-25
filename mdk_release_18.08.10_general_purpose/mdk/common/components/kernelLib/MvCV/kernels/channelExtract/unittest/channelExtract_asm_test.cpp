#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (1.50)
#define EXPECTED_CC_M2 (0.5)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner channelExtractTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int channelExtractCycleCount;

void channelExtract_asm_test(unsigned char** in, unsigned char** out,unsigned int width,unsigned char plane)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920;

	channelExtractTestRunner.Init();
	//add padding values at the end of each line
	channelExtractTestRunner.SetInput("input", in, width, maxWidth, 1, SHAVE0);
	channelExtractTestRunner.SetInput("plane", (unsigned int)plane, SHAVE0);
	channelExtractTestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	channelExtractTestRunner.GuardInsert(string("output"), SHAVE0, width / 3, maxWidth, 1, out);
	channelExtractTestRunner.Run(SHAVE0);
	if(width >= 330)
	{
		channelExtractCycleCount = channelExtractTestRunner.GetVariableValue("cycleCount");
		//the value substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo(((float)(channelExtractCycleCount - 45)/ (float)width)/3);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	channelExtractTestRunner.GetOutput(string("output"), SHAVE0, width / 3, maxWidth, 1, out);
	channelExtractTestRunner.GuardCheck(string("output"), SHAVE0, width / 3, maxWidth, 1, out);
}
