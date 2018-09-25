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

TestRunner cvtColorRGBtoNV12TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorRGBtoNV12CycleCount;

void cvtColorRGBtoNV12_asm_test(unsigned char **inputR, unsigned char **inputG, unsigned char **inputB, unsigned char **Yout, unsigned char **UVout,  unsigned int width, unsigned int line)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	cvtColorRGBtoNV12TestRunner.Init();
	cvtColorRGBtoNV12TestRunner.SetInput("inputR", inputR, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoNV12TestRunner.SetInput("inputG", inputG, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoNV12TestRunner.SetInput("inputB", inputB, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoNV12TestRunner.SetInput("width", width, SHAVE0);
	cvtColorRGBtoNV12TestRunner.SetInput("line", line, SHAVE0);

	cvtColorRGBtoNV12TestRunner.GuardInsert("outputY", SHAVE0, width, maxWidth, 1, Yout);
	cvtColorRGBtoNV12TestRunner.GuardInsert("outputUV", SHAVE0, width, maxWidth, 1, UVout);
	cvtColorRGBtoNV12TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		cvtColorRGBtoNV12CycleCount = cvtColorRGBtoNV12TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorRGBtoNV12CycleCount - 23) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	cvtColorRGBtoNV12TestRunner.GetOutput("outputY", SHAVE0, width, maxWidth, 1, Yout);
	cvtColorRGBtoNV12TestRunner.GuardCheck("outputY", SHAVE0, width, maxWidth, 1, Yout);
	cvtColorRGBtoNV12TestRunner.GetOutput("outputUV", SHAVE0, width, maxWidth, 1, UVout);
	cvtColorRGBtoNV12TestRunner.GuardCheck("outputUV", SHAVE0, width, maxWidth, 1, UVout);
}
