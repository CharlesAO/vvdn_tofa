#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include "gtmFp16ToFp16_asm_test.h"

#define PADDING 16
#define EXPECTED_CC (30.784028)

TestRunner gtmFp16ToFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void gtmFp16ToFp16_asm_test(half *in, half *out, unsigned int width, half *scurve, int nPoints)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	gtmFp16ToFp16TestRunner.Init();
    
	gtmFp16ToFp16TestRunner.SetInput("input", in, width + PADDING, SHAVE0);
	gtmFp16ToFp16TestRunner.SetInput("output", out, width + PADDING, SHAVE0);
	gtmFp16ToFp16TestRunner.SetInput("scurve", scurve, width + PADDING, SHAVE0);
    gtmFp16ToFp16TestRunner.SetInput("width", width, SHAVE0);
    gtmFp16ToFp16TestRunner.SetInput("nPoints", nPoints, SHAVE0);

	gtmFp16ToFp16TestRunner.GuardInsert("output", SHAVE0, width + PADDING, out);
	gtmFp16ToFp16TestRunner.Run(SHAVE0);
        cycleCount = gtmFp16ToFp16TestRunner.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(cycleCount)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
  
	gtmFp16ToFp16TestRunner.GetOutput("output", SHAVE0, width + PADDING, out);
	gtmFp16ToFp16TestRunner.GuardCheck("output", SHAVE0, width + PADDING, out);
}
