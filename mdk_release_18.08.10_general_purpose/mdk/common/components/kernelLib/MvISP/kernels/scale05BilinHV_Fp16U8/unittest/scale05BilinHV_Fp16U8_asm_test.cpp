#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (0.908854)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner scale05BilinHV_Fp16U8TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int scale05BilinHV_Fp16U8CycleCount;

void scale05BilinHV_Fp16U8_asm_test(half** in, unsigned char** out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;

	scale05BilinHV_Fp16U8TestRunner.Init();
	
	scale05BilinHV_Fp16U8TestRunner.SetInput("input", in, width * 2, maxWidth * 2, 2, SHAVE0);
	scale05BilinHV_Fp16U8TestRunner.SetInput("width", width, SHAVE0);
	scale05BilinHV_Fp16U8TestRunner.SetInput("output", out, width + 32, maxWidth + 32, 1, SHAVE0);
	scale05BilinHV_Fp16U8TestRunner.GuardInsert(string("output"), SHAVE0, width + 32, maxWidth + 32, 1, out);
	scale05BilinHV_Fp16U8TestRunner.Run(SHAVE0);

	if (width >=1280) {
		scale05BilinHV_Fp16U8CycleCount = scale05BilinHV_Fp16U8TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(scale05BilinHV_Fp16U8CycleCount - 12)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
 scale05BilinHV_Fp16U8TestRunner.GetOutput(string("output"), SHAVE0, width + 32, maxWidth + 32, 1, out);
 scale05BilinHV_Fp16U8TestRunner.GuardCheck(string("output"), SHAVE0, width + 32, maxWidth + 32, 1, out);
}
