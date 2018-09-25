#include "greyDesat.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (3.31875)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner greyDesatTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int greyDesatCycleCount;

void greyDesat_asm_test(unsigned char **input, unsigned char **output, int offset, int slope, int grey[3], unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;

	greyDesatTestRunner.Init();
	greyDesatTestRunner.SetInput("input", input, width, maxWidth, 3, SHAVE0);
	greyDesatTestRunner.SetInput("offset", offset, SHAVE0);
	greyDesatTestRunner.SetInput("slope",  slope, SHAVE0);
	greyDesatTestRunner.SetInput("grey",  grey, 3, SHAVE0);
	greyDesatTestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	
	greyDesatTestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 3, output);
	greyDesatTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		greyDesatCycleCount = greyDesatTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(greyDesatCycleCount - 11) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	

	greyDesatTestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 3, output);
	greyDesatTestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 3, output);
}
