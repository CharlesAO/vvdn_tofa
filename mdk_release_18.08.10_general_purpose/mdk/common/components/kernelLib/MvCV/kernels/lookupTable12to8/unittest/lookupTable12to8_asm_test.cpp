///
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (3.28)
#define EXPECTED_CC_M2 (3.783205)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner LookupTable12to8TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int LookupTable12to8CycleCount;

void LUT12to8_asm_test(unsigned short** src, unsigned char** dest, unsigned char* lut, unsigned int width, unsigned int height)
{
    unsigned int maxWidth = 1920;
    
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    LookupTable12to8TestRunner.Init();
    LookupTable12to8TestRunner.SetInput("input", (unsigned char**)src, width * 2, maxWidth, 1, SHAVE0);
    LookupTable12to8TestRunner.SetInput("lut", lut, 4096, SHAVE0);
    LookupTable12to8TestRunner.SetInput("width", width, SHAVE0);
    LookupTable12to8TestRunner.SetInput("height", height, SHAVE0);
    LookupTable12to8TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, height, dest);
    LookupTable12to8TestRunner.Run(SHAVE0);
    if(width >= 1280)
	{
		LookupTable12to8CycleCount = LookupTable12to8TestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(LookupTable12to8CycleCount - 11)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    LookupTable12to8TestRunner.GetOutput("output", SHAVE0, width, maxWidth, height, dest);
    LookupTable12to8TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, height, dest);
}
