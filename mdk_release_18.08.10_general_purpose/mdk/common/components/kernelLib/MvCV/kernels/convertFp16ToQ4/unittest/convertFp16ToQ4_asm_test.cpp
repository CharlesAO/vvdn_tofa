#include "half.h"
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC (0.3)

TestRunner convertFp16ToQ4TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int ConvertFp16ToQ4CycleCount;

void ConvertFp16ToQ4_asm_test(half* in, unsigned short* out, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	convertFp16ToQ4TestRunner.Init();
	convertFp16ToQ4TestRunner.SetInput("input", in, width, SHAVE0);
	convertFp16ToQ4TestRunner.SetInput("output", out, width, SHAVE0);
	convertFp16ToQ4TestRunner.SetInput("width", width, SHAVE0);

	convertFp16ToQ4TestRunner.GuardInsert("output", SHAVE0, width, out);
	convertFp16ToQ4TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		ConvertFp16ToQ4CycleCount = convertFp16ToQ4TestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(ConvertFp16ToQ4CycleCount - 2)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	convertFp16ToQ4TestRunner.GetOutput("output", SHAVE0, width, out);
	convertFp16ToQ4TestRunner.GuardCheck("output", SHAVE0, width, out);
}
