///
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (1.344792)
#define EXPECTED_CC_M2 (1.432552)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner PixelPosTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int PixelPosCycleCount;


void pixelPos_asm_test(unsigned char** srcAddr, unsigned char* maskAddr, unsigned int width, 
	      unsigned char pixelValue, unsigned int* pixelPosition, unsigned char* status)

{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;

	PixelPosTestRunner.Init();
	PixelPosTestRunner.SetInput("input", srcAddr, width, maxWidth, 1, SHAVE0);
	PixelPosTestRunner.SetInput("mask", maskAddr, width, SHAVE0);
	PixelPosTestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	PixelPosTestRunner.SetInput("pixelValue", pixelValue, SHAVE0);
	PixelPosTestRunner.SetInput("pixelPosition", (unsigned int)*pixelPosition, SHAVE0);
	PixelPosTestRunner.SetInput("status", (unsigned int)*status, SHAVE0);
	PixelPosTestRunner.GuardInsert("pixelPosition", SHAVE0, pixelPosition);
	PixelPosTestRunner.GuardInsert("status", SHAVE0, status);
	PixelPosTestRunner.Run(SHAVE0);
	
	if(width >= 1280)
	{
		PixelPosCycleCount = PixelPosTestRunner.GetVariableValue("cycleCount");
		//the value substracted from minMaxCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(PixelPosCycleCount - 5) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	PixelPosTestRunner.GetOutput("pixelPosition", SHAVE0, pixelPosition);
	PixelPosTestRunner.GuardCheck("pixelPosition", SHAVE0, pixelPosition);
	PixelPosTestRunner.GetOutput("status", SHAVE0, status);
	PixelPosTestRunner.GuardCheck("status", SHAVE0, status);
}
