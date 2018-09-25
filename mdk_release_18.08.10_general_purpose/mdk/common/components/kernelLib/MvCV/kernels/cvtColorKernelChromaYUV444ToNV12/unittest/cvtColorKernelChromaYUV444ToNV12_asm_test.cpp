#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC_M1 (1.0)
#define EXPECTED_CC_M2 (1.0)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

#define MAX_WIDTH 1920

TestRunner cvtColorChromaYUV444ToNV12TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorChromaYUV444ToNV12CycleCount;

//void mvcvCvtColorKernelChromaYUV444ToNV12(u8* inU[2], u8* inV[2], u8* outUV, u32 width)

void ChromaYUV444ToNV12_asm_test(unsigned char *inU[2], unsigned char *inV[2] ,unsigned char *outUV , unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	cvtColorChromaYUV444ToNV12TestRunner.Init();
	cvtColorChromaYUV444ToNV12TestRunner.SetInput("inU", inU, width, MAX_WIDTH, 2, SHAVE0);
	cvtColorChromaYUV444ToNV12TestRunner.SetInput("inV", inV, width, MAX_WIDTH, 2, SHAVE0);

	cvtColorChromaYUV444ToNV12TestRunner.SetInput("width", width, SHAVE0);

	cvtColorChromaYUV444ToNV12TestRunner.GuardInsert("outUV", SHAVE0, width/2*2, outUV);
	cvtColorChromaYUV444ToNV12TestRunner.Run(SHAVE0);

	if(width >= 320)
	{
		cvtColorChromaYUV444ToNV12CycleCount = cvtColorChromaYUV444ToNV12TestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorChromaYUV444ToNV12CycleCount - 2)/ (float)(width));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}

	cvtColorChromaYUV444ToNV12TestRunner.GetOutput("outUV", SHAVE0, width/2*2, outUV);
	if(width/2*2 % 4 == 0) //it'a a pitty, can only check post-pad markers on 32-bit aligned addresses...
		cvtColorChromaYUV444ToNV12TestRunner.GuardCheck("outUV", SHAVE0, width/2*2, outUV);
}
