#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (1.133823)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner cvtColorRGBtoChromaNV12TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorRGBtoUVCycleCount;

void cvtColorRGBtoChromaNV12_asm_test(unsigned char *input1RGB[3], unsigned char *input2RGB[3], unsigned char *UVout, float mat[3*3], float offset[1*3], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	cvtColorRGBtoChromaNV12TestRunner.Init();
	cvtColorRGBtoChromaNV12TestRunner.SetInput("input1RGB", input1RGB, width, maxWidth, 3, SHAVE0);
	cvtColorRGBtoChromaNV12TestRunner.SetInput("input2RGB", input2RGB, width, maxWidth, 3, SHAVE0);
	cvtColorRGBtoChromaNV12TestRunner.SetInput("width",  width, SHAVE0);
	cvtColorRGBtoChromaNV12TestRunner.SetInput("mat",    (float*)&mat[0],    9, SHAVE0);
	cvtColorRGBtoChromaNV12TestRunner.SetInput("offset", (float*)&offset[0], 3, SHAVE0);
	cvtColorRGBtoChromaNV12TestRunner.SetInput("outputUV", UVout, width + 32, SHAVE0);
	
	cvtColorRGBtoChromaNV12TestRunner.GuardInsert("outputUV", SHAVE0, width + 32, UVout);
	cvtColorRGBtoChromaNV12TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		cvtColorRGBtoUVCycleCount = cvtColorRGBtoChromaNV12TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorRGBtoUVCycleCount - 16) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	
	cvtColorRGBtoChromaNV12TestRunner.GetOutput("outputUV", SHAVE0, width + 32, UVout);
	cvtColorRGBtoChromaNV12TestRunner.GuardCheck("outputUV", SHAVE0, width + 32, UVout);
	
	
	
}
