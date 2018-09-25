#include "half.h"
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC (4)

TestRunner disp2depthTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int disp2depthCycleCount;

void disp2depth_asm_test(unsigned char* in, half* out, half* lut_table, unsigned int flip_disp2depth, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	disp2depthTestRunner.Init();
	disp2depthTestRunner.SetInput("input", in, width, SHAVE0);
	disp2depthTestRunner.SetInput("output", out, width * 2, SHAVE0);
	disp2depthTestRunner.SetInput("lut_table", lut_table, 256, SHAVE0);
	disp2depthTestRunner.SetInput("flip_disp2depth", flip_disp2depth, SHAVE0);
	disp2depthTestRunner.SetInput("width", width, SHAVE0);

	disp2depthTestRunner.GuardInsert("output", SHAVE0, width*2, out);
	disp2depthTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		disp2depthCycleCount = disp2depthTestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(disp2depthCycleCount - 2)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	disp2depthTestRunner.GetOutput("output", SHAVE0, width*2, out);
	disp2depthTestRunner.GuardCheck("output", SHAVE0, width*2, out);
}
