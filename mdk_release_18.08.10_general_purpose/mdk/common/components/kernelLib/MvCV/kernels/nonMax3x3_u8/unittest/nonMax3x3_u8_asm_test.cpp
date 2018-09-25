#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16
 
//there is no asmM1 implementation for nonMax3x3_fp32
#define EXPECTED_CC_M1 (999)
#define EXPECTED_CC_M2 (14.1)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner nonMax3x3_u8TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int nonMax3x3_u8CycleCount;

void nonMax3x3_u8_asm_test(unsigned int width, unsigned char* corners1, unsigned char* corners2,
						unsigned char* corners3, unsigned short* candOutPos, unsigned int* candOutCount)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	nonMax3x3_u8TestRunner.Init();
	nonMax3x3_u8TestRunner.SetInput("c1", corners1, width + PADDING, SHAVE0);
	nonMax3x3_u8TestRunner.SetInput("c2", corners2, width + PADDING, SHAVE0);
	nonMax3x3_u8TestRunner.SetInput("c3", corners3, width + PADDING, SHAVE0);
	nonMax3x3_u8TestRunner.SetInput("width", width, SHAVE0);

	nonMax3x3_u8TestRunner.GuardInsert("candOutCount", SHAVE0, 1, (float*)candOutCount);
	nonMax3x3_u8TestRunner.GuardInsert("candOutPos", SHAVE0, width + PADDING, candOutPos);
	nonMax3x3_u8TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		nonMax3x3_u8CycleCount = nonMax3x3_u8TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(nonMax3x3_u8CycleCount - 2) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	nonMax3x3_u8TestRunner.GetOutput("candOutCount", SHAVE0, 1, (float*)candOutCount);
	nonMax3x3_u8TestRunner.GuardCheck("candOutCount", SHAVE0, 1, (float*)candOutCount);
	nonMax3x3_u8TestRunner.GetOutput("candOutPos", SHAVE0, width + PADDING, candOutPos);
	nonMax3x3_u8TestRunner.GuardCheck("candOutPos", SHAVE0, width + PADDING, candOutPos);
}
