#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (1.42)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner scale2xBilinHV025_Fp16ToFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int scale2xBilinHV025_Fp16ToFp16CycleCount;

void scale2xBilinHV025_Fp16ToFp16_asm_test(half **in, half **out, unsigned int width)
{
	unsigned int widthMax = 1920;
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	scale2xBilinHV025_Fp16ToFp16TestRunner.Init();
	scale2xBilinHV025_Fp16ToFp16TestRunner.SetInput("input", in, width/2 + PADDING, widthMax/2 + PADDING, 2, SHAVE0);
	scale2xBilinHV025_Fp16ToFp16TestRunner.SetInput("width", width, SHAVE0);

	scale2xBilinHV025_Fp16ToFp16TestRunner.GuardInsert("output", SHAVE0, width, widthMax, 1, out);
	scale2xBilinHV025_Fp16ToFp16TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		scale2xBilinHV025_Fp16ToFp16CycleCount = scale2xBilinHV025_Fp16ToFp16TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(scale2xBilinHV025_Fp16ToFp16CycleCount - 14) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	scale2xBilinHV025_Fp16ToFp16TestRunner.GetOutput("output", SHAVE0, width, widthMax, 1, out);
	scale2xBilinHV025_Fp16ToFp16TestRunner.GuardCheck("output", SHAVE0, width, widthMax, 1, out);

}
