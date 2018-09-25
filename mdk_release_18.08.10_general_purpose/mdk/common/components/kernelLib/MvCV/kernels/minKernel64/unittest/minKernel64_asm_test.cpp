#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (37)
#define WIDTH 64

TestRunner mvcvMinKernel64TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvMinKernel64CycleCount;

void mvcvMinKernel64_asm_test(unsigned char *input, unsigned char* min)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	mvcvMinKernel64TestRunner.Init();
	mvcvMinKernel64TestRunner.SetInput(   "input", input, WIDTH, SHAVE0);

	mvcvMinKernel64TestRunner.GuardInsert(string("min"), SHAVE0, 1, min);
	mvcvMinKernel64TestRunner.Run(SHAVE0);

	mvcvMinKernel64CycleCount = (mvcvMinKernel64TestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvMinKernel64CycleCount));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	mvcvMinKernel64TestRunner.GetOutput(string("min"), SHAVE0, 1, min);
	mvcvMinKernel64TestRunner.GuardCheck(string("min"), SHAVE0, 1, min);
}
