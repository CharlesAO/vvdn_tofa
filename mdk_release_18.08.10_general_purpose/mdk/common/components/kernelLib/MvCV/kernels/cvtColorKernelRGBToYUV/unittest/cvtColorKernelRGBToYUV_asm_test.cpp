#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (11.17)
#define EXPECTED_CC_M2 (13.263803)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner cvtColorRGBtoYUVTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorRGBtoYUVCycleCount;

void RGBtoYUV_asm_test(unsigned char *rgb, unsigned char *y ,unsigned char *u ,unsigned char *v , unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	cvtColorRGBtoYUVTestRunner.Init();
	cvtColorRGBtoYUVTestRunner.SetInput("width", width, SHAVE0);
	cvtColorRGBtoYUVTestRunner.SetInput("rgb", rgb, width*2*3, SHAVE0);

	cvtColorRGBtoYUVTestRunner.GuardInsert("y", SHAVE0, width*2, 1920, 1, &y);
	cvtColorRGBtoYUVTestRunner.GuardInsert("u", SHAVE0, width*2/4, 1920, 1, &u);
	cvtColorRGBtoYUVTestRunner.GuardInsert("v", SHAVE0, width*2/4, 1920, 1, &v);
	cvtColorRGBtoYUVTestRunner.Run(SHAVE0);
	//if(width >= 1280)
	//{
		cvtColorRGBtoYUVCycleCount = cvtColorRGBtoYUVTestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorRGBtoYUVCycleCount - 5)/ (float)(2 * width));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	//}
	cvtColorRGBtoYUVTestRunner.GetOutput("y", SHAVE0, width*2, 1920, 1, &y);
	cvtColorRGBtoYUVTestRunner.GuardCheck("y", SHAVE0, width*2, 1920, 1, &y);
	cvtColorRGBtoYUVTestRunner.GetOutput("u", SHAVE0, width*2/4, 1920, 1, &u);
	cvtColorRGBtoYUVTestRunner.GuardCheck("u", SHAVE0, width*2/4, 1920, 1, &u);
	cvtColorRGBtoYUVTestRunner.GetOutput("v", SHAVE0, width*2/4, 1920, 1, &v);
	cvtColorRGBtoYUVTestRunner.GuardCheck("v", SHAVE0, width*2/4, 1920, 1, &v);
}
