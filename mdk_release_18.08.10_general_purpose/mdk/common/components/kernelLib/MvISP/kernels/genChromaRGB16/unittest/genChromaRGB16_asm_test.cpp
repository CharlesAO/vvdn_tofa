#include "genChromaRGB16.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (2.415104)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner genChromaRGB16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int genChromaRGB16CycleCount;

void genChromaRGB16_asm_test(unsigned short **input, unsigned char *inputY, unsigned char **output, unsigned int width, unsigned int eps, unsigned int nBits, float *chromaScale)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	genChromaRGB16TestRunner.Init();
	genChromaRGB16TestRunner.SetInput("input", input, width, maxWidth, 3, SHAVE0);
	genChromaRGB16TestRunner.SetInput("inputY", inputY, width, SHAVE0);
	genChromaRGB16TestRunner.SetInput("chromaScale1", chromaScale, 3, SHAVE0);
	//genChromaRGB16TestRunner.SetInput("inputY", (&inputY), width, maxWidth, 1, SHAVE0);
	genChromaRGB16TestRunner.SetInput("width", width, SHAVE0);
	genChromaRGB16TestRunner.SetInput("eps", eps, SHAVE0);
	genChromaRGB16TestRunner.SetInput("nBits",  nBits, SHAVE0);
	
	
	genChromaRGB16TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 3, output);
	genChromaRGB16TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		genChromaRGB16CycleCount = genChromaRGB16TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(genChromaRGB16CycleCount - 14) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	genChromaRGB16TestRunner.GetOutput("output", SHAVE0, width, maxWidth, 3, output);
	genChromaRGB16TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 3, output);
}
