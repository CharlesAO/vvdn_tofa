#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>


#define EXPECTED_CC (295)
#define TILE_HEIGHT 8
#define TILE_WIDTH  8



TestRunner DCT16bitTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int dct16bitCycleCount;

void dct16bit_asm_test(short input_tile[8][8], short output_tile[8][8])
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	DCT16bitTestRunner.Init();
	DCT16bitTestRunner.SetInput("input_tile", (unsigned char*)input_tile,(TILE_WIDTH * TILE_HEIGHT*2), SHAVE0);

	DCT16bitTestRunner.GuardInsert("output_tile", SHAVE0, (TILE_WIDTH * TILE_HEIGHT*2), (unsigned char*)output_tile);
	DCT16bitTestRunner.Run(SHAVE0);
	
	dct16bitCycleCount = DCT16bitTestRunner.GetVariableValue(std::string("cycleCount"));
	functionInfo.AddCyclePerPixelInfo((float)(dct16bitCycleCount -15));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	DCT16bitTestRunner.GetOutput("output_tile", SHAVE0, (TILE_WIDTH * TILE_HEIGHT*2), (unsigned char*)output_tile);
	DCT16bitTestRunner.GuardCheck("output_tile", SHAVE0, (TILE_WIDTH * TILE_HEIGHT*2), (unsigned char*)output_tile);
	
}
