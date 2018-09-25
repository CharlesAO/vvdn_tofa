#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (905)

TestRunner mvcvMinKernelTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvMinKernelCycleCount;

void mvcvMinKernel_asm_test(unsigned char *input, unsigned char* min, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	mvcvMinKernelTestRunner.Init();
	mvcvMinKernelTestRunner.SetInput(   "input", input, width, SHAVE0);
	mvcvMinKernelTestRunner.SetInput(   "width", width, SHAVE0);
	mvcvMinKernelTestRunner.GuardInsert(string("min"), SHAVE0, 1, min);
	mvcvMinKernelTestRunner.Run(SHAVE0);

	mvcvMinKernelCycleCount = (mvcvMinKernelTestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvMinKernelCycleCount));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	mvcvMinKernelTestRunner.GetOutput(string("min"), SHAVE0, 1, min);
	mvcvMinKernelTestRunner.GuardCheck(string("min"), SHAVE0, 1, min);
}
