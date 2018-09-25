#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0.40)
#define EXPECTED_CC_M2 (0.40)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner arithmeticSubFp16ToFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int arithmeticSubFp16ToFp16CycleCount;

void arithmeticSubFp16ToFp16_asm_test(half** input1, half** input2, half** output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + 32;
	arithmeticSubFp16ToFp16TestRunner.Init();
	arithmeticSubFp16ToFp16TestRunner.SetInput("input1", input1, width, maxWidth, 1, SHAVE0);
	arithmeticSubFp16ToFp16TestRunner.SetInput("input2", input2, width, maxWidth, 1, SHAVE0);
	arithmeticSubFp16ToFp16TestRunner.SetInput("width", width, SHAVE0);
	arithmeticSubFp16ToFp16TestRunner.SetInput("output", output, width + 32, maxWidth, 1, SHAVE0);	
	arithmeticSubFp16ToFp16TestRunner.GuardInsert("output", SHAVE0, width + 32, maxWidth, 1, output);	
	arithmeticSubFp16ToFp16TestRunner.Run(SHAVE0);
	
	if(width >= 1280)
	{
		arithmeticSubFp16ToFp16CycleCount = arithmeticSubFp16ToFp16TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(arithmeticSubFp16ToFp16CycleCount - 14) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	arithmeticSubFp16ToFp16TestRunner.GetOutput("output", SHAVE0, width + 32, maxWidth, 1, output);	
	arithmeticSubFp16ToFp16TestRunner.GuardCheck("output", SHAVE0, width + 32, maxWidth, 1, output);	
}
