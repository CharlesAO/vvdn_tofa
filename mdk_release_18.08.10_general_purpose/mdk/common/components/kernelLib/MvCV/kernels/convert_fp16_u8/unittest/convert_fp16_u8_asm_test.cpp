#include "half.h"
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC (0.3)

TestRunner convert_fp16_u8TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int Convert_fp16_u8CycleCount;

void Convert_fp16_u8_asm_test(half* in, unsigned char* out, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	convert_fp16_u8TestRunner.Init();
	convert_fp16_u8TestRunner.SetInput("input", in, width, SHAVE0);
	convert_fp16_u8TestRunner.SetInput("width", width, SHAVE0);

	convert_fp16_u8TestRunner.GuardInsert("output", SHAVE0, width, out);
	convert_fp16_u8TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		Convert_fp16_u8CycleCount = convert_fp16_u8TestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(Convert_fp16_u8CycleCount - 2)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	convert_fp16_u8TestRunner.GetOutput("output", SHAVE0, width, out);
	convert_fp16_u8TestRunner.GuardCheck("output", SHAVE0, width, out);
}
