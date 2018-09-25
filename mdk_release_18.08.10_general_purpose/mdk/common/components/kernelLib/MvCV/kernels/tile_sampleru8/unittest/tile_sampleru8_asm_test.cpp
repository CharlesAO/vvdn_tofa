#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>


#define EXPECTED_CC (381)


TestRunner tile_sampleru8TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int tile_sampleru8CycleCount;

void tile_sampleru8_asm_test(unsigned char *input_tile, unsigned char * output_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();

	tile_sampleru8TestRunner.Init();
	tile_sampleru8TestRunner.SetInput("input_tile", input_tile, (tile_width) * (tile_height), SHAVE0);
	tile_sampleru8TestRunner.SetInput("xcoord", xcoord, 4, SHAVE0);
	tile_sampleru8TestRunner.SetInput("ycoord", ycoord, 4, SHAVE0);

	tile_sampleru8TestRunner.SetInput("tile_width", tile_width, SHAVE0);
	tile_sampleru8TestRunner.SetInput("tile_height", tile_height, SHAVE0);

	tile_sampleru8TestRunner.GuardInsert("output_tile", SHAVE0, 16 * 16, output_tile);
	tile_sampleru8TestRunner.Run(SHAVE0);
	
	tile_sampleru8CycleCount = tile_sampleru8TestRunner.GetVariableValue(std::string("cycleCount"));
	functionInfo.AddCyclePerPixelInfo((float)(tile_sampleru8CycleCount - 5)/ (float)256);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	
	tile_sampleru8TestRunner.GetOutput("output_tile", SHAVE0, 16 * 16, output_tile);
	tile_sampleru8TestRunner.GuardCheck("output_tile", SHAVE0, 16 * 16, output_tile);
}
