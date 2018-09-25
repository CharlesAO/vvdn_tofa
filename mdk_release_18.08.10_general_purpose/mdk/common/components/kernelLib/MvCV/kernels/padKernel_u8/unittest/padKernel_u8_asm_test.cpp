#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 32
 
#define EXPECTED_CC_M1 (999)
#define EXPECTED_CC_M2 (0.6)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner padKernel_u8TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int padKernel_u8CycleCount;

void padKernel_u8_asm_test(unsigned char* iBuf, unsigned int iBufLen, unsigned char* oBuf, unsigned int padSz, 
							unsigned int padMode, unsigned int padType, unsigned char pixValue)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	padKernel_u8TestRunner.Init();
	padKernel_u8TestRunner.SetInput("iBuf", iBuf, iBufLen + PADDING, SHAVE0);
	padKernel_u8TestRunner.SetInput("iBufLen", iBufLen, SHAVE0);
	padKernel_u8TestRunner.SetInput("padSz", padSz, SHAVE0);
	padKernel_u8TestRunner.SetInput("padMode", padMode, SHAVE0);
	padKernel_u8TestRunner.SetInput("padType", padType, SHAVE0);
	padKernel_u8TestRunner.SetInput("pixValue", pixValue, SHAVE0);

	padKernel_u8TestRunner.GuardInsert("output", SHAVE0, iBufLen + PADDING, oBuf);
	padKernel_u8TestRunner.Run(SHAVE0);
	
	if(iBufLen >= 1280)
	{
		padKernel_u8CycleCount = padKernel_u8TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(padKernel_u8CycleCount - 2) / (float)(iBufLen+padSz));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	padKernel_u8TestRunner.GetOutput("output", SHAVE0, iBufLen + PADDING, oBuf);
	padKernel_u8TestRunner.GuardCheck("output", SHAVE0, iBufLen + PADDING, oBuf);
}
