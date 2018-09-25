#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (9.06)
#define EXPECTED_CC_M2 (4.55)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerHistogram(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int histogramCycleCount;

void histogram_asm_test(unsigned char** in, unsigned int* hist, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();

	unsigned int maxWidth = 1920;

	testRunnerHistogram.Init();
	testRunnerHistogram.SetInput("input", in, width, maxWidth, 1, SHAVE0);
	testRunnerHistogram.SetInput("hist", (unsigned char*)hist, 256 * 4, SHAVE0);
	testRunnerHistogram.SetInput("width", width, SHAVE0);
	testRunnerHistogram.Run(SHAVE0);
	if(width >= 1280)
	{
		histogramCycleCount = testRunnerHistogram.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(histogramCycleCount - 7)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerHistogram.GetOutput("hist", SHAVE0, 256 * 4, maxWidth, 1, (unsigned char**)&hist);
}

