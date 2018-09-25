///
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (1.52)
#define EXPECTED_CC_M2 (1.71224)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner LookupTableTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int LookupTableCycleCount;

void LUT8to8_asm_test(unsigned char** src, unsigned char** dest, unsigned char* lut, unsigned int width, unsigned int height)
{
    unsigned int maxWidth = 1920;
    
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    LookupTableTestRunner.Init();
    LookupTableTestRunner.SetInput("input", src, width, maxWidth, 1, SHAVE0);
    LookupTableTestRunner.SetInput("lut", lut, 256, SHAVE0);
    LookupTableTestRunner.SetInput("width", width, SHAVE0);
    LookupTableTestRunner.SetInput("height", height, SHAVE0);
    LookupTableTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, height, dest);
    LookupTableTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		LookupTableCycleCount = LookupTableTestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(LookupTableCycleCount - 11)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    LookupTableTestRunner.GetOutput("output", SHAVE0, width, maxWidth, height, dest);
    LookupTableTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, height, dest);
}
