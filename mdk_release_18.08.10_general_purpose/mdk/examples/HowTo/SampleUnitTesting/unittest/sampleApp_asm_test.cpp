#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC (3)

TestRunner sampleAppTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int SampleAppCycleCount;

void SampleApp_asm_test(unsigned char *in, unsigned int width, unsigned char value, unsigned char operation, unsigned int pxToChange, unsigned char *out)
{

    FunctionInfo& functionInfo = FunctionInfo::Instance();
    sampleAppTestRunner.Init();
    sampleAppTestRunner.SetInput("in", in, width, SHAVE0);
    sampleAppTestRunner.SetInput("width", width, SHAVE0);
    sampleAppTestRunner.SetInput("value", value, SHAVE0);
    sampleAppTestRunner.SetInput("op", operation, SHAVE0);
    sampleAppTestRunner.SetInput("pxToChange", pxToChange, SHAVE0);

    sampleAppTestRunner.Run(SHAVE0);
    SampleAppCycleCount = sampleAppTestRunner.GetVariableValue(std::string("cycleCount"));
	functionInfo.AddCyclePerPixelInfo((float)(SampleAppCycleCount - 2)/ (float)width);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	sampleAppTestRunner.GetOutput(string("out"), SHAVE0, width, out);
}
