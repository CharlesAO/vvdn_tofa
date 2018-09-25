#include "scale05BilinHVFp16.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (0.66)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner scale05BilinHVFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int scale05BilinHVFp16CycleCount;

void scale05BilinHVFp16_asm_test(half** input, half** output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	scale05BilinHVFp16TestRunner.Init();
	scale05BilinHVFp16TestRunner.SetInput("input", input, width, maxWidth, 2, SHAVE0);
	scale05BilinHVFp16TestRunner.SetInput("width", width, SHAVE0);

	scale05BilinHVFp16TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, output);
	scale05BilinHVFp16TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		scale05BilinHVFp16CycleCount = scale05BilinHVFp16TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(scale05BilinHVFp16CycleCount - 11) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	scale05BilinHVFp16TestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, output);
	scale05BilinHVFp16TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, output);

}
