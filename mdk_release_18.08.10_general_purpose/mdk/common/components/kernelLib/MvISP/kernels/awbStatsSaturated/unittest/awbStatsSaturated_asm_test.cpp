#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include "awbStatsSaturated_asm_test.h"

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (0.784028)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner genRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void awbStatsSaturated_asm_test(unsigned char *out, unsigned short *in, unsigned int nPaxelX, unsigned int gapPaxelX, unsigned int widthPaxelX, unsigned int satPaxelX, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	genRunner.Init();
    
	genRunner.SetInput("in", in, width, SHAVE0);
	genRunner.SetInput("nPaxelX", nPaxelX, SHAVE0);
    genRunner.SetInput("gapPaxelX", gapPaxelX, SHAVE0);
    genRunner.SetInput("widthPaxelX", widthPaxelX, SHAVE0);
    genRunner.SetInput("satPaxelX", satPaxelX, SHAVE0);
    genRunner.SetInput("output", out, nPaxelX*2+16, SHAVE0);

	genRunner.GuardInsert(string("output"), SHAVE0, nPaxelX*2+16, out);
	genRunner.Run(SHAVE0);
    if ((nPaxelX >= 64)&&(widthPaxelX >= 64))
    {
        cycleCount = genRunner.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(cycleCount)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }    

	genRunner.GetOutput(string("output"), SHAVE0, nPaxelX*2+16, out);
	genRunner.GuardCheck(string("output"), SHAVE0, nPaxelX*2+16, out);
}
