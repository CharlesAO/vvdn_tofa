#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (0.657292)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner cvtColorRGBtoLumaTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorRGBtoLumaCycleCount;


void cvtColorRGBtoLuma_asm_test(unsigned char **inputR, unsigned char **inputG, unsigned char **inputB, unsigned char **Yout, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	cvtColorRGBtoLumaTestRunner.Init();
	cvtColorRGBtoLumaTestRunner.SetInput("inputR", inputR, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoLumaTestRunner.SetInput("inputG", inputG, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoLumaTestRunner.SetInput("inputB", inputB, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoLumaTestRunner.SetInput("width", width, SHAVE0);

	cvtColorRGBtoLumaTestRunner.GuardInsert("outputY", SHAVE0, width, maxWidth, 1, Yout);
	cvtColorRGBtoLumaTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		cvtColorRGBtoLumaCycleCount = cvtColorRGBtoLumaTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorRGBtoLumaCycleCount - 19) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	cvtColorRGBtoLumaTestRunner.GetOutput("outputY", SHAVE0, width, maxWidth, 1, Yout);
	cvtColorRGBtoLumaTestRunner.GuardCheck("outputY", SHAVE0, width, maxWidth, 1, Yout);

}
