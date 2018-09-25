#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include "lumaFusionFp16ToFp16_asm_test.h"

#define PADDING 32
#define EXPECTED_CC (30.784028)

TestRunner lumaFusionFp16ToFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void lumaFusionFp16ToFp16_asm_test(half* inLumaShort, half* inLumaLong, half* outputAlpha, half* outputLumaFusion, unsigned int width, LumaFusionParamT *params)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	lumaFusionFp16ToFp16TestRunner.Init();
    
	lumaFusionFp16ToFp16TestRunner.SetInput("inLumaShort", inLumaShort, width + PADDING, SHAVE0);
	lumaFusionFp16ToFp16TestRunner.SetInput("inLumaLong", inLumaLong, width + PADDING, SHAVE0);
	lumaFusionFp16ToFp16TestRunner.SetInput("outputAlpha", outputAlpha, width + PADDING, SHAVE0);
	lumaFusionFp16ToFp16TestRunner.SetInput("outputLumaFusion", outputLumaFusion, width + PADDING, SHAVE0);
	lumaFusionFp16ToFp16TestRunner.SetInput("params", (float *)params, 4, SHAVE0);
    lumaFusionFp16ToFp16TestRunner.SetInput("width", width, SHAVE0);

	lumaFusionFp16ToFp16TestRunner.GuardInsert("outputAlpha", SHAVE0, width + PADDING, outputAlpha);
	lumaFusionFp16ToFp16TestRunner.GuardInsert("outputLumaFusion", SHAVE0, width + PADDING, outputLumaFusion);
	lumaFusionFp16ToFp16TestRunner.GuardInsert("inLumaShort", SHAVE0, width + PADDING, inLumaShort);
	lumaFusionFp16ToFp16TestRunner.Run(SHAVE0);
        cycleCount = lumaFusionFp16ToFp16TestRunner.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(cycleCount - 20)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
  
	lumaFusionFp16ToFp16TestRunner.GetOutput("outputAlpha", SHAVE0, width + PADDING, outputAlpha);
	lumaFusionFp16ToFp16TestRunner.GuardCheck("outputAlpha", SHAVE0, width + PADDING, outputAlpha);
	lumaFusionFp16ToFp16TestRunner.GetOutput("outputLumaFusion", SHAVE0, width + PADDING, outputLumaFusion);
	lumaFusionFp16ToFp16TestRunner.GuardCheck("outputLumaFusion", SHAVE0, width + PADDING, outputLumaFusion);
	lumaFusionFp16ToFp16TestRunner.GetOutput("inLumaShort", SHAVE0, width + PADDING, inLumaShort);
	lumaFusionFp16ToFp16TestRunner.GuardCheck("inLumaShort", SHAVE0, width + PADDING, inLumaShort);
}
