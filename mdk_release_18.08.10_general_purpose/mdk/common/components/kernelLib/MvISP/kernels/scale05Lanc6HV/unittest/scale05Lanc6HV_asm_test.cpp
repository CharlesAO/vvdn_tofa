#include "scale05Lanc6HV.h"
#include "scale05Lanc6HV_asm_test.h"
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (3.358507)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner scale05Lanc6HVTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;
#define  PADDING 8
void scale05Lanc6HV_asm_test(u8 **inlines, u8* out, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING * 2;
	scale05Lanc6HVTestRunner.Init();
	scale05Lanc6HVTestRunner.SetInput("input", inlines, width * 2 + PADDING * 2, maxWidth, 6, SHAVE0);
	scale05Lanc6HVTestRunner.SetInput("width", (unsigned int) width, SHAVE0);

	scale05Lanc6HVTestRunner.GuardInsert(string("output"), SHAVE0, width, out);
	scale05Lanc6HVTestRunner.Run(SHAVE0);
	if(width >= 640)
	{
		cycleCount = scale05Lanc6HVTestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(cycleCount)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	scale05Lanc6HVTestRunner.GetOutput(string("output"), SHAVE0, width, out);
	scale05Lanc6HVTestRunner.GuardCheck(string("output"), SHAVE0, width, out);
}
