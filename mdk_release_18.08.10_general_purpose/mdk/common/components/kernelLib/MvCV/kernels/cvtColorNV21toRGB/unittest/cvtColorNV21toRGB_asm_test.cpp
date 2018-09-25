#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (2.27)
#define EXPECTED_CC_M2 (2.519792)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner cvtColorNV21toRGBTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorNV21toRGBCycleCount;
			 

void cvtColorNV21toRGB_asm_test(unsigned char **inputY,unsigned char **inputUV, unsigned char **rOut, unsigned char **gOut, unsigned char **bOut, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	cvtColorNV21toRGBTestRunner.Init();
	cvtColorNV21toRGBTestRunner.SetInput("inputY", inputY, width, maxWidth, 1, SHAVE0);
	cvtColorNV21toRGBTestRunner.SetInput("inputUV", inputUV, width, maxWidth, 1, SHAVE0);
	cvtColorNV21toRGBTestRunner.SetInput("width", width, SHAVE0);

	cvtColorNV21toRGBTestRunner.GuardInsert("outputR", SHAVE0, width, maxWidth, 1, rOut);
	cvtColorNV21toRGBTestRunner.GuardInsert("outputG", SHAVE0, width, maxWidth, 1, gOut);
	cvtColorNV21toRGBTestRunner.GuardInsert("outputB", SHAVE0, width, maxWidth, 1, bOut);
	cvtColorNV21toRGBTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		cvtColorNV21toRGBCycleCount = cvtColorNV21toRGBTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorNV21toRGBCycleCount - 23) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	cvtColorNV21toRGBTestRunner.GetOutput("outputR", SHAVE0, width, maxWidth, 1, rOut);
	cvtColorNV21toRGBTestRunner.GuardCheck("outputR", SHAVE0, width, maxWidth, 1, rOut);
	cvtColorNV21toRGBTestRunner.GetOutput("outputG", SHAVE0, width, maxWidth, 1, gOut);
	cvtColorNV21toRGBTestRunner.GuardCheck("outputG", SHAVE0, width, maxWidth, 1, gOut);
	cvtColorNV21toRGBTestRunner.GetOutput("outputB", SHAVE0, width, maxWidth, 1, bOut);
	cvtColorNV21toRGBTestRunner.GuardCheck("outputB", SHAVE0, width, maxWidth, 1, bOut);
}
