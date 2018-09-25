#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>


#define EXPECTED_CC (381)


TestRunner tile_sampleru10TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int tile_sampleru10CycleCount;

void tile_sampleru10_asm_test(unsigned short* input_tile, unsigned short* output_tile, float* xcoord, float* ycoord, unsigned int tile_width, unsigned int tile_height)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();

	tile_sampleru10TestRunner.Init();
	tile_sampleru10TestRunner.SetInput("input_tile", input_tile, (tile_width) * (tile_height), SHAVE0);
	tile_sampleru10TestRunner.SetInput("xcoord", xcoord, 4, SHAVE0);
	tile_sampleru10TestRunner.SetInput("ycoord", ycoord, 4, SHAVE0);

	tile_sampleru10TestRunner.SetInput("tile_width", tile_width, SHAVE0);
	tile_sampleru10TestRunner.SetInput("tile_height", tile_height, SHAVE0);

	tile_sampleru10TestRunner.GuardInsert("output_tile", SHAVE0, 16 * 16, output_tile);
	tile_sampleru10TestRunner.Run(SHAVE0);
	
	tile_sampleru10CycleCount = tile_sampleru10TestRunner.GetVariableValue(std::string("cycleCount"));
	functionInfo.AddCyclePerPixelInfo((float)(tile_sampleru10CycleCount - 5)/ (float)256);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	
	tile_sampleru10TestRunner.GetOutput("output_tile", SHAVE0, 16 * 16, output_tile);
	tile_sampleru10TestRunner.GuardCheck("output_tile", SHAVE0, 16 * 16, output_tile);
}
