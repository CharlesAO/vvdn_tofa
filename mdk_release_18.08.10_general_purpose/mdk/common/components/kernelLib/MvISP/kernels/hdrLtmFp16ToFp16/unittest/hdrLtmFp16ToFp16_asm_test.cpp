#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (4.29)

TestRunner hdrLtmFp16ToFp16(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cycleCount;

void hdrLtmFp16ToFp16_asm_test(half* inLumaFusion, half* inGaussUp, half* output, unsigned int  width, float* params)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    hdrLtmFp16ToFp16.Init();

    hdrLtmFp16ToFp16.SetInput("inLumaFusion", inLumaFusion, width, SHAVE0);
    hdrLtmFp16ToFp16.SetInput("inGaussUp", inGaussUp, width, SHAVE0);
    hdrLtmFp16ToFp16.SetInput("output", output, width, SHAVE0);
    hdrLtmFp16ToFp16.SetInput("parameters", params, 3, SHAVE0);
    hdrLtmFp16ToFp16.SetInput("width", (unsigned int)width, SHAVE0);

    hdrLtmFp16ToFp16.Run(SHAVE0);
    if (width >= 1280)
    {
        cycleCount = hdrLtmFp16ToFp16.GetVariableValue(std::string("cycleCount"));
        functionInfo.AddCyclePerPixelInfo((float)(cycleCount)/ (float)width);
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }    
    hdrLtmFp16ToFp16.GetOutput(string("output"), SHAVE0, width, output);
    hdrLtmFp16ToFp16.GetOutput(string("inGaussUp"), SHAVE0, width, inGaussUp);
}
