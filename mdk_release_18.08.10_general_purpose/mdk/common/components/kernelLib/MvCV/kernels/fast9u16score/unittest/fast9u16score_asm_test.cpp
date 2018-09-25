#include "fast9u16score.h"
#include "fast9u16score_asm_test.h"
#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (18.429035)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner fast9u16scoreTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;
#define  PADDING 8
void fast9u16score_asm_test(u8 **inlines, u16* score, u16* base, unsigned int tresh, unsigned int width)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + PADDING * 2;
	fast9u16scoreTestRunner.Init();
	fast9u16scoreTestRunner.SetInput("input", inlines, width + PADDING * 2, maxWidth, 7, SHAVE0);
	fast9u16scoreTestRunner.SetInput("width", (unsigned int) width, SHAVE0);
	fast9u16scoreTestRunner.SetInput("tresh",(unsigned int) tresh, SHAVE0);

	//DllMoviDebugParseString("t s0");
	//DllMoviDebugParseString("get base 20");
	//printf("\n");
	//DllMoviDebugParseString("get score 20");

	fast9u16scoreTestRunner.GuardInsert(string("score"), SHAVE0, width * 2 + 4,(u8*)score);
	fast9u16scoreTestRunner.GuardInsert(string("base"),  SHAVE0, width * 2 + 4,(u8*)base);
	fast9u16scoreTestRunner.Run(SHAVE0);
    if (width >= 1280)
    {
        cycleCount = fast9u16scoreTestRunner.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(cycleCount)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }

	//fast9u16scoreTestRunner.GetOutput("base", SHAVE0, base);
//	DllMoviDebugParseString("t s0");
//	DllMoviDebugParseString("get base 20");
//	printf("\n");
//	DllMoviDebugParseString("get score 20");
	//GetOutput(string("output"),0, width, maxWidth, 1, out);
	fast9u16scoreTestRunner.GetOutput(string("score"), SHAVE0, width * 2 + 4,(u8*)score);
	fast9u16scoreTestRunner.GuardCheck(string("score"), SHAVE0, width * 2 + 4,(u8*)score);
	fast9u16scoreTestRunner.GetOutput(string("base"),  SHAVE0, width * 2 + 4,(u8*)base);
	fast9u16scoreTestRunner.GuardCheck(string("base"),  SHAVE0, width * 2 + 4,(u8*)base);
}
