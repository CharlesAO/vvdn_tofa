#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 16

#define EXPECTED_CC (2.14)

TestRunner scale2xBilinHV_Fp16ToFp16_oddTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int scale2xBilinHV_Fp16ToFp16_oddCycleCount;

void scale2xBilinHV_Fp16ToFp16_odd_asm_test(half **in, half **out, unsigned int width)
{
    unsigned int widthMax = 1920;
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    scale2xBilinHV_Fp16ToFp16_oddTestRunner.Init();
    scale2xBilinHV_Fp16ToFp16_oddTestRunner.SetInput("input", in, width/2 + PADDING, widthMax/2 + PADDING, 2, SHAVE0);
    scale2xBilinHV_Fp16ToFp16_oddTestRunner.SetInput("width", width, SHAVE0);

    scale2xBilinHV_Fp16ToFp16_oddTestRunner.GuardInsert("output", SHAVE0, width, widthMax, 1, out);
    scale2xBilinHV_Fp16ToFp16_oddTestRunner.Run(SHAVE0);
    if(width >= 1280)
    {
        scale2xBilinHV_Fp16ToFp16_oddCycleCount = scale2xBilinHV_Fp16ToFp16_oddTestRunner.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(scale2xBilinHV_Fp16ToFp16_oddCycleCount - 14) / (float)width);
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }
    scale2xBilinHV_Fp16ToFp16_oddTestRunner.GetOutput("output", SHAVE0, width, widthMax, 1, out);
    scale2xBilinHV_Fp16ToFp16_oddTestRunner.GuardCheck("output", SHAVE0, width, widthMax, 1, out);

}
