#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (4.14)
#define EXPECTED_CC_M2 (2.723438)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner gaussTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int gaussCycleCount;

void gauss_asm_test(unsigned char** in, unsigned char** out, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();

	unsigned int padding = 16;
	unsigned int maxWidth = 1920 + padding;

	gaussTestRunner.Init();
	gaussTestRunner.SetInput("input", in, width + padding, maxWidth, 5, SHAVE0);
	gaussTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	
	gaussTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, out);
	gaussTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		gaussCycleCount = gaussTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(gaussCycleCount - 65)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	gaussTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, out);
	gaussTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, out);
	
}
