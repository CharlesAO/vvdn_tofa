#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

//there is no asmM1 implementation for nonMax3x3_fp32
#define EXPECTED_CC_M1 (999)
#define EXPECTED_CC_M2 (14.1)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif


TestRunner nonMax3x3_fp32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int nonMax3x3_fp32CycleCount;

void nonMax3x3_fp32_asm_test(unsigned short* candPos, unsigned int candCount, float* corners1, float* corners2,
						float* corners3, unsigned short* candOutPos, unsigned int* candOutCount)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	nonMax3x3_fp32TestRunner.Init();
	nonMax3x3_fp32TestRunner.SetInput("corners1", corners1, maxWidth, SHAVE0);
	nonMax3x3_fp32TestRunner.SetInput("corners2", corners2, maxWidth, SHAVE0);
	nonMax3x3_fp32TestRunner.SetInput("corners3", corners3, maxWidth, SHAVE0);
	nonMax3x3_fp32TestRunner.SetInput("candPos", candPos, candCount, SHAVE0);
	nonMax3x3_fp32TestRunner.SetInput("candCount", candCount, SHAVE0);

	nonMax3x3_fp32TestRunner.GuardInsert("candOutCount", SHAVE0, 1, (float*)candOutCount);
	nonMax3x3_fp32TestRunner.GuardInsert("candOutPos", SHAVE0, candCount, candOutPos);
	nonMax3x3_fp32TestRunner.Run(SHAVE0);
	if(candCount >= 1280)
	{
		nonMax3x3_fp32CycleCount = nonMax3x3_fp32TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(nonMax3x3_fp32CycleCount - 2) / (float)candCount);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	nonMax3x3_fp32TestRunner.GetOutput("candOutCount", SHAVE0, 1, (float*)candOutCount);
	nonMax3x3_fp32TestRunner.GuardCheck("candOutCount", SHAVE0, 1, (float*)candOutCount);
	nonMax3x3_fp32TestRunner.GetOutput("candOutPos", SHAVE0, candCount, candOutPos);
	nonMax3x3_fp32TestRunner.GuardCheck("candOutPos", SHAVE0, candCount, candOutPos);
}
