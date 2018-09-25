#include "fast9ScoreCv.h"
#include "fast9ScoreCv_asm_test.h"
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (180.429035)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner fast9ScoreCvTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;
#define  PADDING 8
void fast9ScoreCv_asm_test(u8 **inlines, u8* score, u16* base, unsigned int tresh, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING * 2;
	fast9ScoreCvTestRunner.Init();
	fast9ScoreCvTestRunner.SetInput("input", inlines, width + PADDING * 2, maxWidth, 7, SHAVE0);
	fast9ScoreCvTestRunner.SetInput("width", (unsigned int) width, SHAVE0);
	fast9ScoreCvTestRunner.SetInput("tresh",(unsigned int) tresh, SHAVE0);

	fast9ScoreCvTestRunner.GuardInsert(string("score"), SHAVE0, width + 4,(u8*)score);
	fast9ScoreCvTestRunner.GuardInsert(string("base"),  SHAVE0, width * 2 + 4,(u8*)base);
	fast9ScoreCvTestRunner.Run(SHAVE0);
    if (width == 1280-2*8)
    {
        cycleCount = fast9ScoreCvTestRunner.GetVariableValue(std::string("cycleCount"));
        cycleCount -= (4.284019 * width);
        functionInfo.AddCyclePerPixelInfo((float)(cycleCount)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }

	fast9ScoreCvTestRunner.GetOutput(string("score"), SHAVE0, width + 4,(u8*)score);
	fast9ScoreCvTestRunner.GuardCheck(string("score"), SHAVE0, width + 4,(u8*)score);
	fast9ScoreCvTestRunner.GetOutput(string("base"),  SHAVE0, width * 2 + 4,(u8*)base);
	fast9ScoreCvTestRunner.GuardCheck(string("base"),  SHAVE0, width * 2 + 4,(u8*)base);
}
