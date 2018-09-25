#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (2.53)
#define EXPECTED_CC_M2 (3.155729)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner cvtColorKernelYUV422ToRGBTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorKernelYUV422ToRGBCycleCount;

void cvtColorKernelYUV422ToRGB_asm_test(unsigned char **input, unsigned char **rOut, unsigned char **gOut, unsigned char **bOut, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	cvtColorKernelYUV422ToRGBTestRunner.Init();
	cvtColorKernelYUV422ToRGBTestRunner.SetInput("input", input, 2 * width, 2 * maxWidth, 1, SHAVE0);
	cvtColorKernelYUV422ToRGBTestRunner.SetInput("width", width, SHAVE0);

	cvtColorKernelYUV422ToRGBTestRunner.GuardInsert("outputR", SHAVE0, width, maxWidth, 1, rOut);
	cvtColorKernelYUV422ToRGBTestRunner.GuardInsert("outputG", SHAVE0, width, maxWidth, 1, gOut);
	cvtColorKernelYUV422ToRGBTestRunner.GuardInsert("outputB", SHAVE0, width, maxWidth, 1, bOut);
	cvtColorKernelYUV422ToRGBTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
	cvtColorKernelYUV422ToRGBCycleCount = cvtColorKernelYUV422ToRGBTestRunner.GetVariableValue("cycleCount");
	functionInfo.AddCyclePerPixelInfo((float)(cvtColorKernelYUV422ToRGBCycleCount - 18) / (float)width);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	cvtColorKernelYUV422ToRGBTestRunner.GetOutput("outputR", SHAVE0, width, maxWidth, 1, rOut);
	cvtColorKernelYUV422ToRGBTestRunner.GuardCheck("outputR", SHAVE0, width, maxWidth, 1, rOut);
	cvtColorKernelYUV422ToRGBTestRunner.GetOutput("outputG", SHAVE0, width, maxWidth, 1, gOut);
	cvtColorKernelYUV422ToRGBTestRunner.GuardCheck("outputG", SHAVE0, width, maxWidth, 1, gOut);
	cvtColorKernelYUV422ToRGBTestRunner.GetOutput("outputB", SHAVE0, width, maxWidth, 1, bOut);
	cvtColorKernelYUV422ToRGBTestRunner.GuardCheck("outputB", SHAVE0, width, maxWidth, 1, bOut);
}
