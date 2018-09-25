#include "mixMedian.h"
#include "mixMedian_asm_test.h"
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (1.403331)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner appTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void mixMedian_asm_test(u8* out[3], u8 *in0[3], u8 *in1[3], 
    u8 *ref, float offset, float slope, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	appTestRunner.Init();
	appTestRunner.SetInput("in0", in0, width, maxWidth, 3, SHAVE0);
	appTestRunner.SetInput("in1", in1, width, maxWidth, 3, SHAVE0);
	appTestRunner.SetInput("reff", ref, width, SHAVE0);
	appTestRunner.SetInput("width", (unsigned int) width, SHAVE0);
	appTestRunner.SetInput("offset", offset, SHAVE0);
	appTestRunner.SetInput("slope", slope, SHAVE0);

	appTestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, (unsigned int)3,  (u8**)out);
	appTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		cycleCount = appTestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(cycleCount - 5)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	appTestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, (unsigned int)3,  (u8**)out);
	appTestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, (unsigned int)3,  (u8**)out);
}
