///
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M2 (2.04)

#define EXPECTED_CC EXPECTED_CC_M2


TestRunner LookupTableTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int LookupTableCycleCount;

void LUT8to10_asm_test(unsigned char** src, unsigned short** dest, unsigned short* lut, unsigned int width)
{
    unsigned int maxWidth = 1920;
    
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    LookupTableTestRunner.Init();
    LookupTableTestRunner.SetInput("input", src, width, maxWidth, 1, SHAVE0);
    LookupTableTestRunner.SetInput("lut", lut, 256, SHAVE0);
    LookupTableTestRunner.SetInput("width", width, SHAVE0);
    LookupTableTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, dest);
    LookupTableTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		LookupTableCycleCount = LookupTableTestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(LookupTableCycleCount - 11)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    LookupTableTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, dest);
    LookupTableTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, dest);
}
