#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (2.272917)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner cvtColorRGBfp16ToUV420U8(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorRGBfp16ToUV420U8CycleCount;

void cvtColorRGBfp16ToUV420U8_asm_test(half** inputRGB, unsigned char *Uout, unsigned char *Vout,unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	cvtColorRGBfp16ToUV420U8.Init();
	cvtColorRGBfp16ToUV420U8.SetInput("inputRGB", inputRGB, width, maxWidth, 3, SHAVE0);
	cvtColorRGBfp16ToUV420U8.SetInput("width", width, SHAVE0);
	cvtColorRGBfp16ToUV420U8.GuardInsert(string("outputU"), SHAVE0, width/2, Uout);
	cvtColorRGBfp16ToUV420U8.GuardInsert(string("outputV"), SHAVE0, width/2, Vout);
	cvtColorRGBfp16ToUV420U8.Run(SHAVE0);
	if(width >= 1280)
	{
		cvtColorRGBfp16ToUV420U8CycleCount = cvtColorRGBfp16ToUV420U8.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorRGBfp16ToUV420U8CycleCount - 19) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	cvtColorRGBfp16ToUV420U8.GetOutput(string("outputU"), SHAVE0, width/2, Uout);
	cvtColorRGBfp16ToUV420U8.GuardCheck(string("outputU"), SHAVE0, width/2, Uout);
	cvtColorRGBfp16ToUV420U8.GetOutput(string("outputV"), SHAVE0, width/2, Vout);
	cvtColorRGBfp16ToUV420U8.GuardCheck(string("outputV"), SHAVE0, width/2, Vout);

}
