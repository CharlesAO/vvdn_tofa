///
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (3.28)
#define EXPECTED_CC_M2 (3.785406)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner LookupTable12to16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int LookupTable12to16CycleCount;

void LUT12to16_asm_test(unsigned short** src, unsigned short** dest, unsigned short* lut, unsigned int width, unsigned int height)
{
    unsigned int maxWidth = 1920;
    
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    LookupTable12to16TestRunner.Init();
    LookupTable12to16TestRunner.SetInput("input", (unsigned char**)src, width * 2, maxWidth, 1, SHAVE0);
    LookupTable12to16TestRunner.SetInput("lut", (unsigned char*)lut, 4096 * 2, SHAVE0);
    LookupTable12to16TestRunner.SetInput("width", width, SHAVE0);
    LookupTable12to16TestRunner.SetInput("height", height, SHAVE0);
    LookupTable12to16TestRunner.GuardInsert("output", SHAVE0, width * 2, maxWidth, height, (unsigned char**)dest);
    LookupTable12to16TestRunner.Run(SHAVE0);
    if(width >= 1280)
	{
		LookupTable12to16CycleCount = LookupTable12to16TestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(LookupTable12to16CycleCount - 11)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    LookupTable12to16TestRunner.GetOutput("output", SHAVE0, width * 2, maxWidth, height, (unsigned char**)dest);
    LookupTable12to16TestRunner.GuardCheck("output", SHAVE0, width * 2, maxWidth, height, (unsigned char**)dest);
}
