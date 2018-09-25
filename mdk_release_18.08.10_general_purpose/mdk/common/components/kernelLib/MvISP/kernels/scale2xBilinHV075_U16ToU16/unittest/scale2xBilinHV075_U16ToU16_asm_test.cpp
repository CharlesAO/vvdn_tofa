#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (1.905)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner scale2xBilinHV075_U16ToU16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int scale2xBilinHV075_U16ToU16CycleCount;

void scale2xBilinHV075_U16ToU16_asm_test(unsigned short **in, unsigned short **out, unsigned int width)
{
	unsigned int widthMax = 1920;
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	scale2xBilinHV075_U16ToU16TestRunner.Init();
	scale2xBilinHV075_U16ToU16TestRunner.SetInput("input", in, width/2 + PADDING, widthMax/2 + PADDING, 2, SHAVE0);
	scale2xBilinHV075_U16ToU16TestRunner.SetInput("width", width, SHAVE0);

	scale2xBilinHV075_U16ToU16TestRunner.GuardInsert("output", SHAVE0, width, widthMax, 1, out);
	scale2xBilinHV075_U16ToU16TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		scale2xBilinHV075_U16ToU16CycleCount = scale2xBilinHV075_U16ToU16TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(scale2xBilinHV075_U16ToU16CycleCount - 14) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	scale2xBilinHV075_U16ToU16TestRunner.GetOutput("output", SHAVE0, width, widthMax, 1, out);
	scale2xBilinHV075_U16ToU16TestRunner.GuardCheck("output", SHAVE0, width, widthMax, 1, out);

}
