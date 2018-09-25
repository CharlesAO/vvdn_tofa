#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.27)
#define EXPECTED_CC_M2 (0.26)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner ConvertFrom12BppTo8BppTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int ConvertFrom12BppTo8BppCycleCount;

void ConvertFrom12BppTo8Bpp_asm_test(unsigned char* out, unsigned char* in, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	ConvertFrom12BppTo8BppTestRunner.Init();
	ConvertFrom12BppTo8BppTestRunner.SetInput("input", in, width * 2, SHAVE0);
	ConvertFrom12BppTo8BppTestRunner.SetInput("width", width, SHAVE0);

	ConvertFrom12BppTo8BppTestRunner.GuardInsert("output", SHAVE0, width, out);
	ConvertFrom12BppTo8BppTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		ConvertFrom12BppTo8BppCycleCount = ConvertFrom12BppTo8BppTestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(ConvertFrom12BppTo8BppCycleCount - 5)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	ConvertFrom12BppTo8BppTestRunner.GetOutput("output", SHAVE0, width, out);
	ConvertFrom12BppTo8BppTestRunner.GuardCheck("output", SHAVE0, width, out);
}
