#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (0.657812)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner cvtColorRGBfp16ToLumaU8TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorRGBfp16ToLumaU8CycleCount;


void cvtColorRGBfp16ToLumaU8_asm_test(half **inputRGB, unsigned char *Yout, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	cvtColorRGBfp16ToLumaU8TestRunner.Init();
	cvtColorRGBfp16ToLumaU8TestRunner.SetInput("input", inputRGB, width, maxWidth, 3, SHAVE0);
	cvtColorRGBfp16ToLumaU8TestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	cvtColorRGBfp16ToLumaU8TestRunner.GuardInsert(string("output"), SHAVE0, width, Yout);
	cvtColorRGBfp16ToLumaU8TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		cvtColorRGBfp16ToLumaU8CycleCount = cvtColorRGBfp16ToLumaU8TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorRGBfp16ToLumaU8CycleCount - 19) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	cvtColorRGBfp16ToLumaU8TestRunner.GetOutput(string("output"), SHAVE0, width, Yout);
	cvtColorRGBfp16ToLumaU8TestRunner.GuardCheck(string("output"), SHAVE0, width, Yout);

}
