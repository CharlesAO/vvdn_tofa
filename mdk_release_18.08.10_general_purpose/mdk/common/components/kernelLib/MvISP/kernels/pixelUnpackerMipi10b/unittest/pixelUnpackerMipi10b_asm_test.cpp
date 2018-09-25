#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (0.8)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

#define OUT_PADDING 16



TestRunner pixelUnpackerMipi10bTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int pixelUnpackerMipi10bCycleCount;

void PixelUnpackerMipi10b_asm_test(unsigned int *in, unsigned short *out, unsigned int width, unsigned int lineNo, unsigned int coefs[4], int noMipiRxWorkaround)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	pixelUnpackerMipi10bTestRunner.Init();
	pixelUnpackerMipi10bTestRunner.SetInput("in", (float*)in, width/2, SHAVE0);
	pixelUnpackerMipi10bTestRunner.SetInput("width", width, SHAVE0);
	pixelUnpackerMipi10bTestRunner.SetInput("lineNo", lineNo, SHAVE0);
	pixelUnpackerMipi10bTestRunner.SetInput("noMipiRxWorkaround", noMipiRxWorkaround, SHAVE0);
	pixelUnpackerMipi10bTestRunner.SetInput("coefs", (unsigned char*)coefs, 4 * 4, SHAVE0);
	
	pixelUnpackerMipi10bTestRunner.SetInput("output", out, width + 2 * OUT_PADDING, SHAVE0);
	
	pixelUnpackerMipi10bTestRunner.GuardInsert("output", SHAVE0, width + 2 * OUT_PADDING, out);
	pixelUnpackerMipi10bTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		pixelUnpackerMipi10bCycleCount = pixelUnpackerMipi10bTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(pixelUnpackerMipi10bCycleCount - 14) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	pixelUnpackerMipi10bTestRunner.GetOutput("output", SHAVE0, width + 2 * OUT_PADDING, out);
	pixelUnpackerMipi10bTestRunner.GuardCheck("output", SHAVE0, width + 2 * OUT_PADDING, out);

}
