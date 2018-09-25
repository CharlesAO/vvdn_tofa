#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (2.156771)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner cvtColorRGBtoUVTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorRGBtoUVCycleCount;


void cvtColorRGBtoUV_asm_test(unsigned char **inputR, unsigned char **inputG, unsigned char **inputB, unsigned char **UVout, unsigned int width,  unsigned int line)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	cvtColorRGBtoUVTestRunner.Init();
	cvtColorRGBtoUVTestRunner.SetInput("inputR", inputR, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoUVTestRunner.SetInput("inputG", inputG, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoUVTestRunner.SetInput("inputB", inputB, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoUVTestRunner.SetInput("width", width, SHAVE0);
	cvtColorRGBtoUVTestRunner.SetInput("line", line, SHAVE0);

	cvtColorRGBtoUVTestRunner.GuardInsert("outputUV", SHAVE0, width, maxWidth, 1, UVout);
	cvtColorRGBtoUVTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		cvtColorRGBtoUVCycleCount = cvtColorRGBtoUVTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorRGBtoUVCycleCount - 19) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	cvtColorRGBtoUVTestRunner.GetOutput("outputUV", SHAVE0, width, maxWidth, 1, UVout);
	cvtColorRGBtoUVTestRunner.GuardCheck("outputUV", SHAVE0, width, maxWidth, 1, UVout);

}
