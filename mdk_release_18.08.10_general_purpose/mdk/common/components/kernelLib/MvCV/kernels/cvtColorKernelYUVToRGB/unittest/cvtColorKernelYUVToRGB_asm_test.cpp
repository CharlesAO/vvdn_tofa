#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (9.91)
#define EXPECTED_CC_M2 (2.653436)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner cvtColorKernelYUVToRGBTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int cvtColorKernelYUVToRGBCycleCount;

void cvtColorKernelYUVToRGB_asm_test(unsigned char *y, unsigned char *u ,unsigned char *v ,unsigned char *rgb , unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	cvtColorKernelYUVToRGBTestRunner.Init();
	cvtColorKernelYUVToRGBTestRunner.SetInput("width", width, SHAVE0);
	cvtColorKernelYUVToRGBTestRunner.SetInput("y", y, width, SHAVE0);
	cvtColorKernelYUVToRGBTestRunner.SetInput("u", u, width/2, SHAVE0);
	cvtColorKernelYUVToRGBTestRunner.SetInput("v", v, width/2, SHAVE0);

	cvtColorKernelYUVToRGBTestRunner.GuardInsert("rgb", SHAVE0, width*3 , 1920, 1, &rgb);
	cvtColorKernelYUVToRGBTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		cvtColorKernelYUVToRGBCycleCount = cvtColorKernelYUVToRGBTestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorKernelYUVToRGBCycleCount - 5)/ (float)(width));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	cvtColorKernelYUVToRGBTestRunner.GetOutput("rgb", SHAVE0, width*3 , 1920, 1, &rgb);
	cvtColorKernelYUVToRGBTestRunner.GuardCheck("rgb", SHAVE0, width*3 , 1920, 1, &rgb);

}
