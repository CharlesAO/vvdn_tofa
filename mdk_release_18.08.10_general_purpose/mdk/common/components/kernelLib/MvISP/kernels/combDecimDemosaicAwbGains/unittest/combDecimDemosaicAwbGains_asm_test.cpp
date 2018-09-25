#include "combDecimDemosaicAwbGains_asm_test.h"
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (2.546578)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner appTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void combDecimDemosaicAwbGains_asm_test(unsigned char ** out, unsigned short **inlines, unsigned short gains[3], unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 4208;
	appTestRunner.Init();
	appTestRunner.SetInput("input", inlines, width * 2, maxWidth, 4, SHAVE0);
    appTestRunner.SetInput("gains", gains, 3 * 2, SHAVE0);
	appTestRunner.SetInput("width", (unsigned int) width, SHAVE0);

	appTestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 3, out);
	appTestRunner.Run(SHAVE0);
	if(width >= 1052)
	{
		cycleCount = appTestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(cycleCount - 5)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}		
	appTestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 3, out);
	appTestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 3, out);
}
