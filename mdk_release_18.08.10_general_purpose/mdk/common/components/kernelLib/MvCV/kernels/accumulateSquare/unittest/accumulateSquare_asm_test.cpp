///
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (1.83)
#define EXPECTED_CC_M2 (1.831944)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif


TestRunner AccumulateSquareTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int AccumulateSquareCycleCount;

void AccumulateSquare_asm_test(unsigned char** srcAddr, unsigned char** maskAddr, float** destAddr, unsigned int width, unsigned int height)
{
	
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	AccumulateSquareTestRunner.Init();
	AccumulateSquareTestRunner.SetInput("input", srcAddr, width, maxWidth, height, SHAVE0);
	AccumulateSquareTestRunner.SetInput("mask", maskAddr, width, maxWidth, height, SHAVE0);
	AccumulateSquareTestRunner.SetInput("output", (unsigned char**)destAddr, width * 4, maxWidth, height, SHAVE0);
	AccumulateSquareTestRunner.SetInput("width", width, SHAVE0);
	AccumulateSquareTestRunner.SetInput("height", height, SHAVE0);

	AccumulateSquareTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, destAddr);
	AccumulateSquareTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		AccumulateSquareCycleCount = AccumulateSquareTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(AccumulateSquareCycleCount - 42)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);

	}	
	AccumulateSquareTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, destAddr);
	AccumulateSquareTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, destAddr);
}