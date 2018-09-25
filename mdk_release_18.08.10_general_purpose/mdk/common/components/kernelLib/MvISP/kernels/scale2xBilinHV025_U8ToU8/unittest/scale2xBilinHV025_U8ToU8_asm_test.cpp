#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (1.26)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner scale2xBilinHV025_U8ToU8TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int scale2xBilinHV025_U8ToU8CycleCount;

void scale2xBilinHV025_U8ToU8_asm_test(unsigned char **in, unsigned char **out, unsigned int width)
{
	unsigned int widthMax = 1920;
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	scale2xBilinHV025_U8ToU8TestRunner.Init();
	scale2xBilinHV025_U8ToU8TestRunner.SetInput("input", in, width/2 + PADDING, widthMax/2 + PADDING, 2, SHAVE0);
	scale2xBilinHV025_U8ToU8TestRunner.SetInput("width", width, SHAVE0);

	scale2xBilinHV025_U8ToU8TestRunner.GuardInsert("output", SHAVE0, width, widthMax, 1, out);
	scale2xBilinHV025_U8ToU8TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		scale2xBilinHV025_U8ToU8CycleCount = scale2xBilinHV025_U8ToU8TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(scale2xBilinHV025_U8ToU8CycleCount - 11) / (float)width/2);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	scale2xBilinHV025_U8ToU8TestRunner.GetOutput("output", SHAVE0, width, widthMax, 1, out);
	scale2xBilinHV025_U8ToU8TestRunner.GuardCheck("output", SHAVE0, width, widthMax, 1, out);

}
