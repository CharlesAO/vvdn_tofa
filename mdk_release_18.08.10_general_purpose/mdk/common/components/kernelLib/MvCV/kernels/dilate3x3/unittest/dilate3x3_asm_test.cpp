#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC_M1 (1.38)
#define EXPECTED_CC_M2 (1.890625)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner Dilate3x3TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int Dilate3x3CycleCount;

void Dilate3x3_asm_test(unsigned char** src, unsigned char** dst, unsigned char** kernel, unsigned int width)
{
    unsigned int maxWidth = 1920 + PADDING;
    unsigned int kernelSize = 3;

    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    Dilate3x3TestRunner.Init();
    Dilate3x3TestRunner.SetInput("input", src, width + PADDING, maxWidth, kernelSize, SHAVE0);
    Dilate3x3TestRunner.SetInput("matrix", kernel, kernelSize, 4, kernelSize, SHAVE0);
    Dilate3x3TestRunner.SetInput("width", width, SHAVE0);
    
    Dilate3x3TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, dst);
    Dilate3x3TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		Dilate3x3CycleCount = Dilate3x3TestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(Dilate3x3CycleCount - 16)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    Dilate3x3TestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, dst);
    Dilate3x3TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, dst);
}
