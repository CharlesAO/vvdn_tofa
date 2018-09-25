#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define OUT_PADDING 16

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (0.58624)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner scale05BilinHV_U8ToU8TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int scale05BilinHV_U8ToU8CycleCount;

void scale05BilinHV_U8ToU8_asm_test(unsigned char **in, unsigned char *out, unsigned int width)
{
	unsigned int widthMax = 1920;
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	scale05BilinHV_U8ToU8TestRunner.Init();
	scale05BilinHV_U8ToU8TestRunner.SetInput("input", in, width * 2, widthMax * 2, 2, SHAVE0);
	scale05BilinHV_U8ToU8TestRunner.SetInput("width", width, SHAVE0);

	scale05BilinHV_U8ToU8TestRunner.GuardInsert("output", SHAVE0, width + 2 * OUT_PADDING, out);
	scale05BilinHV_U8ToU8TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		scale05BilinHV_U8ToU8CycleCount = scale05BilinHV_U8ToU8TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(scale05BilinHV_U8ToU8CycleCount - 7) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	scale05BilinHV_U8ToU8TestRunner.GetOutput("output", SHAVE0, width + 2 * OUT_PADDING, out);
	scale05BilinHV_U8ToU8TestRunner.GuardCheck("output", SHAVE0, width + 2 * OUT_PADDING, out);

}
