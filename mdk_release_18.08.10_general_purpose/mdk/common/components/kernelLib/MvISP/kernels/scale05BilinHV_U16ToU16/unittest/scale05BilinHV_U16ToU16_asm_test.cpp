#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define OUT_PADDING 16

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (0.662109)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner scale05BilinHV_U16ToU16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int scale05BilinHV_U16ToU16CycleCount;

void scale05BilinHV_U16ToU16_asm_test(unsigned short **in, unsigned short *out, unsigned int width)
{
	unsigned int widthMax = 1920;
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	scale05BilinHV_U16ToU16TestRunner.Init();
	scale05BilinHV_U16ToU16TestRunner.SetInput("input", in, width * 2, widthMax * 2, 2, SHAVE0);
	scale05BilinHV_U16ToU16TestRunner.SetInput("width", width, SHAVE0);
	scale05BilinHV_U16ToU16TestRunner.SetInput("output", out, width + 2 * OUT_PADDING, SHAVE0);

	
	scale05BilinHV_U16ToU16TestRunner.GuardInsert("output", SHAVE0, width + 2 * OUT_PADDING, out);
	scale05BilinHV_U16ToU16TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		scale05BilinHV_U16ToU16CycleCount = scale05BilinHV_U16ToU16TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(scale05BilinHV_U16ToU16CycleCount - 7) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	scale05BilinHV_U16ToU16TestRunner.GetOutput("output", SHAVE0, width + 2 * OUT_PADDING, out);
	scale05BilinHV_U16ToU16TestRunner.GuardCheck("output", SHAVE0, width + 2 * OUT_PADDING, out);

}
