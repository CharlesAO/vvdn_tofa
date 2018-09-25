#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (2.274479)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner cvtColorRGBtoUV420TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorRGBtoUVCycleCount;

void cvtColorRGBtoUV420_asm_test(unsigned char **inputR, unsigned char **inputG, unsigned char **inputB, unsigned char **Uout, unsigned char **Vout,unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	cvtColorRGBtoUV420TestRunner.Init();
	cvtColorRGBtoUV420TestRunner.SetInput("inputR", inputR, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoUV420TestRunner.SetInput("inputG", inputG, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoUV420TestRunner.SetInput("inputB", inputB, width, maxWidth, 1, SHAVE0);
	cvtColorRGBtoUV420TestRunner.SetInput("width", width, SHAVE0);
	
	cvtColorRGBtoUV420TestRunner.GuardInsert("outputU", SHAVE0, width/2, maxWidth, 1, Uout);
	cvtColorRGBtoUV420TestRunner.GuardInsert("outputV", SHAVE0, width/2, maxWidth, 1, Vout);

	cvtColorRGBtoUV420TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		cvtColorRGBtoUVCycleCount = cvtColorRGBtoUV420TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorRGBtoUVCycleCount - 19) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	cvtColorRGBtoUV420TestRunner.GetOutput("outputU", SHAVE0, width/2, maxWidth, 1, Uout);
	cvtColorRGBtoUV420TestRunner.GuardCheck("outputU", SHAVE0, width/2, maxWidth, 1, Uout);
	cvtColorRGBtoUV420TestRunner.GetOutput("outputV", SHAVE0, width/2, maxWidth, 1, Vout);
	cvtColorRGBtoUV420TestRunner.GuardCheck("outputV", SHAVE0, width/2, maxWidth, 1, Vout);

}
