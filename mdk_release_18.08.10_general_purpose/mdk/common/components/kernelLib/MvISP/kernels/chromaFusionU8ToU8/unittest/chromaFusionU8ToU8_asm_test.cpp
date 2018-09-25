#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (2.192448)
#define padding 16

TestRunner ChromaFusionU8ToU8TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int ChromaFusionU8ToU8CycleCount;

void ChromaFusionU8ToU8_asm_test(unsigned char* inCRshort, unsigned char* inCGshort, 
unsigned char* inCBshort, unsigned char* inCRlong, unsigned char* inCGlong, 
unsigned char* inCBlong, half* inAlpha, unsigned char* outR, unsigned char* outG,
 unsigned char* outB, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	ChromaFusionU8ToU8TestRunner.Init();
	ChromaFusionU8ToU8TestRunner.SetInput("inCRshort", inCRshort, width + padding, SHAVE0);
	ChromaFusionU8ToU8TestRunner.SetInput("inCGshort", inCGshort, width + padding, SHAVE0);
	ChromaFusionU8ToU8TestRunner.SetInput("inCBshort", inCBshort, width + padding, SHAVE0);
	ChromaFusionU8ToU8TestRunner.SetInput("inCRlong",  inCRlong, width + padding, SHAVE0);
	ChromaFusionU8ToU8TestRunner.SetInput("inCGlong",  inCGlong, width + padding, SHAVE0);
	ChromaFusionU8ToU8TestRunner.SetInput("inCBlong",  inCBlong, width + padding, SHAVE0);
	ChromaFusionU8ToU8TestRunner.SetInput("inAlpha",   inAlpha, width + padding, SHAVE0);
	
	ChromaFusionU8ToU8TestRunner.SetInput("outputR", outR, width + padding, SHAVE0);
	ChromaFusionU8ToU8TestRunner.SetInput("outputG", outG, width + padding, SHAVE0);
	ChromaFusionU8ToU8TestRunner.SetInput("outputB", outB, width + padding, SHAVE0);
	ChromaFusionU8ToU8TestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	ChromaFusionU8ToU8TestRunner.GuardInsert(string("outputR"), SHAVE0, width + padding, outR);
	ChromaFusionU8ToU8TestRunner.GuardInsert(string("outputG"), SHAVE0, width + padding, outG);
	ChromaFusionU8ToU8TestRunner.GuardInsert(string("outputB"), SHAVE0, width + padding, outB);
	ChromaFusionU8ToU8TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		ChromaFusionU8ToU8CycleCount = ChromaFusionU8ToU8TestRunner.GetVariableValue("cycleCount");
		//the value substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(ChromaFusionU8ToU8CycleCount - 65)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	ChromaFusionU8ToU8TestRunner.GetOutput(string("outputR"), SHAVE0, width + padding, outR);
	ChromaFusionU8ToU8TestRunner.GuardCheck(string("outputR"), SHAVE0, width + padding, outR);
	ChromaFusionU8ToU8TestRunner.GetOutput(string("outputG"), SHAVE0, width + padding, outG);
	ChromaFusionU8ToU8TestRunner.GuardCheck(string("outputG"), SHAVE0, width + padding, outG);
	ChromaFusionU8ToU8TestRunner.GetOutput(string("outputB"), SHAVE0, width + padding, outB);
	ChromaFusionU8ToU8TestRunner.GuardCheck(string("outputB"), SHAVE0, width + padding, outB);
	}
