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

TestRunner cvtColorChromaYUV420ToNV12TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int cvtColorChromaYUV420ToNV12CycleCount;

//void mvcvCvtColorKernelChromaYUV420ToNV12(u8* inU, u8* inV, u8* outUV, u32 width)

void ChromaYUV420ToNV12_asm_test(unsigned char *inU, unsigned char *inV ,unsigned char *outUV , unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	cvtColorChromaYUV420ToNV12TestRunner.Init();
	cvtColorChromaYUV420ToNV12TestRunner.SetInput("inU", inU, width/2, SHAVE0);
	cvtColorChromaYUV420ToNV12TestRunner.SetInput("inV", inV, width/2, SHAVE0);
	cvtColorChromaYUV420ToNV12TestRunner.SetInput("width", width, SHAVE0);

	cvtColorChromaYUV420ToNV12TestRunner.GuardInsert("outUV", SHAVE0, width/2*2, outUV);
	cvtColorChromaYUV420ToNV12TestRunner.Run(SHAVE0);

	if(width >= 320)
	{
		cvtColorChromaYUV420ToNV12CycleCount = cvtColorChromaYUV420ToNV12TestRunner.GetVariableValue(std::string("cycleCount"));
		functionInfo.AddCyclePerPixelInfo((float)(cvtColorChromaYUV420ToNV12CycleCount - 2)/ (float)(width));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}

	cvtColorChromaYUV420ToNV12TestRunner.GetOutput("outUV", SHAVE0, width/2*2, outUV);
	if(width/2*2 % 4 == 0) //it'a a pitty, can only check post-pad markers on 32-bit aligned addresses...
		cvtColorChromaYUV420ToNV12TestRunner.GuardCheck("outUV", SHAVE0, width/2*2, outUV);
}
