#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (0.78)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner cvtColorRGBtoLumaNV12TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorRGBtoLumaNV12CycleCount;


void cvtColorRGBtoLumaNV12_asm_test(unsigned char *inputR, unsigned char *inputG, unsigned char *inputB, unsigned char *Yout, float mat[3*3], float offset[1*3], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	cvtColorRGBtoLumaNV12TestRunner.Init();
	cvtColorRGBtoLumaNV12TestRunner.SetInput("inputR", inputR, width, SHAVE0);
	cvtColorRGBtoLumaNV12TestRunner.SetInput("inputG", inputG, width, SHAVE0);
	cvtColorRGBtoLumaNV12TestRunner.SetInput("inputB", inputB, width, SHAVE0);
	cvtColorRGBtoLumaNV12TestRunner.SetInput("width", width, SHAVE0);
	cvtColorRGBtoLumaNV12TestRunner.SetInput("mat",   (float*)&mat[0], 9, SHAVE0);
	cvtColorRGBtoLumaNV12TestRunner.SetInput("offset",(float*)&offset[0], 3, SHAVE0);
	cvtColorRGBtoLumaNV12TestRunner.SetInput("outputY", Yout, width + 32, SHAVE0);
	
	
	cvtColorRGBtoLumaNV12TestRunner.GuardInsert("outputY", SHAVE0, width + 32, Yout);
	cvtColorRGBtoLumaNV12TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		cvtColorRGBtoLumaNV12CycleCount = cvtColorRGBtoLumaNV12TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorRGBtoLumaNV12CycleCount - 2) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	cvtColorRGBtoLumaNV12TestRunner.GetOutput("outputY", SHAVE0, width + 32, Yout);
	cvtColorRGBtoLumaNV12TestRunner.GuardCheck("outputY", SHAVE0, width + 32, Yout);

}
