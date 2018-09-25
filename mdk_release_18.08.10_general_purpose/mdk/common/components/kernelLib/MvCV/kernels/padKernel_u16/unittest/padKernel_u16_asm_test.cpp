#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define PADDING 32
 
#define EXPECTED_CC_M1 (999)
#define EXPECTED_CC_M2 (1.1)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner padKernel_u16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int padKernel_u16CycleCount;

void padKernel_u16_asm_test(unsigned short* iBuf, unsigned int iBufLen, unsigned short* oBuf, unsigned int padSz, 
							unsigned int padMode, unsigned int padType, unsigned short pixValue)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	padKernel_u16TestRunner.Init();
	padKernel_u16TestRunner.SetInput("iBuf", iBuf, iBufLen + PADDING, SHAVE0);
	padKernel_u16TestRunner.SetInput("iBufLen", iBufLen, SHAVE0);
	padKernel_u16TestRunner.SetInput("padSz", padSz, SHAVE0);
	padKernel_u16TestRunner.SetInput("padMode", padMode, SHAVE0);
	padKernel_u16TestRunner.SetInput("padType", padType, SHAVE0);
	padKernel_u16TestRunner.SetInput("pixValue", pixValue, SHAVE0);

	padKernel_u16TestRunner.GuardInsert("output", SHAVE0, iBufLen + PADDING, oBuf);
	padKernel_u16TestRunner.Run(SHAVE0);
	
	if(iBufLen >= 1280)
	{
		padKernel_u16CycleCount = padKernel_u16TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(padKernel_u16CycleCount - 2) / (float)(iBufLen+padSz));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	padKernel_u16TestRunner.GetOutput("output", SHAVE0, iBufLen + PADDING, oBuf);
	padKernel_u16TestRunner.GuardCheck("output", SHAVE0, iBufLen + PADDING, oBuf);
}
