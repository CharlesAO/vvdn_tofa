#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>


#define EXPECTED_CC (1734)
#define TILE_WIDTH 8
#define TILE_HEIGHT 8
#define IMG_WIDTH 128
#define IMG_HEIGHT 128

TestRunner JpegGetBlockY420TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int jpegGetBlockY420CycleCount;

void jpegGetBlockY420_asm_test(unsigned int mode, unsigned char *imginfo_Y, unsigned int imginfo_width, unsigned int imgspec_x, unsigned int imgspec_y, short imgspecY[8][8])

{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	JpegGetBlockY420TestRunner.Init();
	JpegGetBlockY420TestRunner.SetInput("mode", mode, SHAVE0);
	JpegGetBlockY420TestRunner.SetInput("imginfo_Y", imginfo_Y, ((IMG_HEIGHT+1)*IMG_WIDTH), SHAVE0);
	JpegGetBlockY420TestRunner.SetInput("imginfo_width", imginfo_width, SHAVE0);
	JpegGetBlockY420TestRunner.SetInput("imgspec_x", imgspec_x, SHAVE0);
	JpegGetBlockY420TestRunner.SetInput("imgspec_y", imgspec_y, SHAVE0);
	JpegGetBlockY420TestRunner.GuardInsert("imgspecY", SHAVE0, (2*TILE_WIDTH*TILE_HEIGHT), (unsigned char*)imgspecY);
	JpegGetBlockY420TestRunner.Run(SHAVE0);
	jpegGetBlockY420CycleCount = JpegGetBlockY420TestRunner.GetVariableValue(std::string("cycleCount"));
	functionInfo.AddCyclePerPixelInfo((float)(jpegGetBlockY420CycleCount -15));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	
   JpegGetBlockY420TestRunner.GetOutput("imgspecY", SHAVE0, (2*TILE_WIDTH*TILE_HEIGHT), (unsigned char*)imgspecY);
   JpegGetBlockY420TestRunner.GuardCheck("imgspecY", SHAVE0, (2*TILE_WIDTH*TILE_HEIGHT), (unsigned char*)imgspecY);

	
}
