#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"

#define EXPECTED_CC_M1 (4.05)
#define EXPECTED_CC_M2 (12.219)
#define PADDING 16


#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void localMaxMin3x3_s16_asm_test(short** inBuffer, unsigned int  candidateLocationIn[], unsigned int countIn, unsigned int minLocationList[],
        unsigned int maxLocationList[], unsigned int* minCount,  unsigned int* maxCount)
{
	unsigned int maxWidth = 640 + PADDING;
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    testRunner.Init();
    testRunner.SetInput("input", (unsigned char **)inBuffer, (countIn + PADDING)*sizeof(short), maxWidth*sizeof(short), 3, SHAVE0);
    testRunner.SetInput("countIn", countIn, SHAVE0);
	testRunner.SetInput("candidateLocationIn", (unsigned char*)candidateLocationIn, 4*(countIn +2), SHAVE0);
    testRunner.SetInput("maxLocationList", (unsigned char*)maxLocationList, 4*(countIn +2), SHAVE0);
    testRunner.SetInput("minLocationList", (unsigned char*)minLocationList, 4*(countIn +2), SHAVE0);
    testRunner.SetInput("minCount", *minCount, SHAVE0);
    testRunner.SetInput("maxCount", *maxCount, SHAVE0);

    testRunner.GuardInsert("minCount", SHAVE0, minCount);
    testRunner.GuardInsert("maxCount", SHAVE0, maxCount);
    testRunner.GuardInsert("minLocationList", SHAVE0, 4*countIn, (unsigned char*)minLocationList);
    testRunner.GuardInsert("maxLocationList", SHAVE0, 4*countIn, (unsigned char*)maxLocationList);
    testRunner.Run(SHAVE0);


	//if(countIn >= 1280)
	{
	cycleCount = testRunner.GetVariableValue("cycleCount");
	functionInfo.AddCyclePerPixelInfo((float)(cycleCount - 12)/ (float)countIn);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}




    testRunner.GetOutput("minCount", SHAVE0, minCount);
    testRunner.GuardCheck("minCount", SHAVE0, minCount);
    testRunner.GetOutput("maxCount", SHAVE0, maxCount);
    testRunner.GuardCheck("maxCount", SHAVE0, maxCount);
    testRunner.GetOutput("minLocationList", SHAVE0, 4*countIn, (unsigned char*)minLocationList);
    testRunner.GuardCheck("minLocationList", SHAVE0, 4*countIn, (unsigned char*)minLocationList);
    testRunner.GetOutput("maxLocationList", SHAVE0, 4*countIn, (unsigned char*)maxLocationList);
    testRunner.GuardCheck("maxLocationList", SHAVE0, 4*countIn, (unsigned char*)maxLocationList);
}

