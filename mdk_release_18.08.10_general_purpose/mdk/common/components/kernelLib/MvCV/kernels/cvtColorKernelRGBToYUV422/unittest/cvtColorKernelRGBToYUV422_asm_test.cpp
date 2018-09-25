#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (3.92)
#define EXPECTED_CC_M2 (4.167188)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner cvtColorKernelRGBToYUV422TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorKernelRGBToYUV422CycleCount;

void cvtColorKernelRGBToYUV422_asm_test(unsigned char **inputR, unsigned char **inputG, unsigned char **inputB, unsigned char **output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	cvtColorKernelRGBToYUV422TestRunner.Init();
	cvtColorKernelRGBToYUV422TestRunner.SetInput("inputR", inputR, width, maxWidth, 1, SHAVE0);
	cvtColorKernelRGBToYUV422TestRunner.SetInput("inputG", inputG, width, maxWidth, 1, SHAVE0);
	cvtColorKernelRGBToYUV422TestRunner.SetInput("inputB", inputB, width, maxWidth, 1, SHAVE0);
	cvtColorKernelRGBToYUV422TestRunner.SetInput("width", width, SHAVE0);

	cvtColorKernelRGBToYUV422TestRunner.GuardInsert("output", SHAVE0, 2 * width, 2 * maxWidth, 1, output);
	cvtColorKernelRGBToYUV422TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		cvtColorKernelRGBToYUV422CycleCount = cvtColorKernelRGBToYUV422TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorKernelRGBToYUV422CycleCount - 18) / (float)width);	
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	cvtColorKernelRGBToYUV422TestRunner.GetOutput("output", SHAVE0, 2 * width, 2 * maxWidth, 1, output);
	cvtColorKernelRGBToYUV422TestRunner.GuardCheck("output", SHAVE0, 2 * width, 2 * maxWidth, 1, output);

}
