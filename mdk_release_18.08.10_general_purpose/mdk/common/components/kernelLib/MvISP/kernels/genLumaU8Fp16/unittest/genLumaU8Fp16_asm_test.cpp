#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (0.79)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner genLumaU8Fp16Runner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void genLumaU8Fp16_asm_test(unsigned char* inR, unsigned char* inG, unsigned char *inB, half *outY, half coefs[3],  unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	genLumaU8Fp16Runner.Init();

	genLumaU8Fp16Runner.SetInput("inR", inR, width, SHAVE0);
    genLumaU8Fp16Runner.SetInput("inG", inG, width, SHAVE0);
    genLumaU8Fp16Runner.SetInput("inB", inB, width, SHAVE0);
	genLumaU8Fp16Runner.SetInput("width", width, SHAVE0);
	genLumaU8Fp16Runner.SetInput("coefs", (half*)&coefs[0], 3, SHAVE0);

	genLumaU8Fp16Runner.GuardInsert(string("output"), SHAVE0, width, outY);
	genLumaU8Fp16Runner.Run(SHAVE0);
    if (width >= 1280)
    {
        cycleCount = genLumaU8Fp16Runner.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(cycleCount)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }    

	genLumaU8Fp16Runner.GetOutput(string("output"), SHAVE0, width, outY);
	genLumaU8Fp16Runner.GuardCheck(string("output"), SHAVE0, width, outY);
}
