#include "half.h"
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC (0.3)

TestRunner convert_u8_fp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int Convert_u8_fp16CycleCount;

void Convert_u8_fp16_asm_test(unsigned char* in, half* out, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	convert_u8_fp16TestRunner.Init();
	convert_u8_fp16TestRunner.SetInput("input", in, width, SHAVE0);
	convert_u8_fp16TestRunner.SetInput("width", width, SHAVE0);

	convert_u8_fp16TestRunner.GuardInsert("output", SHAVE0, width, out);
	convert_u8_fp16TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		Convert_u8_fp16CycleCount = convert_u8_fp16TestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(Convert_u8_fp16CycleCount - 2)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	convert_u8_fp16TestRunner.GetOutput("output", SHAVE0, width, out);
	convert_u8_fp16TestRunner.GuardCheck("output", SHAVE0, width, out);
}
