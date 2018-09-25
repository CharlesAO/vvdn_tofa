#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#ifdef MYRIAD2
#define EXPECTED_CC (4.25)
#endif

TestRunner meshGenerateTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int meshGenerateCycleCount;

void meshGenerate_asm_test(half *mesh, float *mat, unsigned int width, unsigned int posx, unsigned int posy)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	meshGenerateTestRunner.Init();
	meshGenerateTestRunner.SetInput("mat", (float*)mat, 6, SHAVE0);
	meshGenerateTestRunner.SetInput("width", width, SHAVE0);
	meshGenerateTestRunner.SetInput("posx", posx, SHAVE0);
	meshGenerateTestRunner.SetInput("posy", posy, SHAVE0);
	meshGenerateTestRunner.GuardInsert(string("output_mesh"), SHAVE0, width*2, mesh);
	meshGenerateTestRunner.Run(SHAVE0);
		
	if(width >= 1280)
	{
		meshGenerateCycleCount = meshGenerateTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(meshGenerateCycleCount - 140) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	meshGenerateTestRunner.GetOutput(string("output_mesh"), SHAVE0, width*2, mesh);
	meshGenerateTestRunner.GuardCheck(string("output_mesh"), SHAVE0, width*2, mesh);
	
}
