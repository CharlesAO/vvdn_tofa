#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (2.16)
#define EXPECTED_CC_M2 (2.41276)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner cvtColorRGBtoNV21TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorRGBtoNV21CycleCount;

void cvtColorRGBtoNV21_asm_test(unsigned char **inputR, unsigned char **inputG, unsigned char **inputB, unsigned char **Yout, unsigned char **UVout,  unsigned int width, unsigned int line)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	cvtColorRGBtoNV21TestRunner.Init();
	cvtColorRGBtoNV21TestRunner.SetInput("inputR", inputR, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoNV21TestRunner.SetInput("inputG", inputG, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoNV21TestRunner.SetInput("inputB", inputB, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoNV21TestRunner.SetInput("width", width, SHAVE0);
	cvtColorRGBtoNV21TestRunner.SetInput("line", line, SHAVE0);

	cvtColorRGBtoNV21TestRunner.GuardInsert("outputY", SHAVE0, width, maxWidth, 1, Yout);
	cvtColorRGBtoNV21TestRunner.GuardInsert("outputUV", SHAVE0, width, maxWidth, 1, UVout);
	cvtColorRGBtoNV21TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		cvtColorRGBtoNV21CycleCount = cvtColorRGBtoNV21TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorRGBtoNV21CycleCount - 23) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	cvtColorRGBtoNV21TestRunner.GetOutput("outputY", SHAVE0, width, maxWidth, 1, Yout);
	cvtColorRGBtoNV21TestRunner.GuardCheck("outputY", SHAVE0, width, maxWidth, 1, Yout);
	cvtColorRGBtoNV21TestRunner.GetOutput("outputUV", SHAVE0, width, maxWidth, 1, UVout);
	cvtColorRGBtoNV21TestRunner.GuardCheck("outputUV", SHAVE0, width, maxWidth, 1, UVout);
}
