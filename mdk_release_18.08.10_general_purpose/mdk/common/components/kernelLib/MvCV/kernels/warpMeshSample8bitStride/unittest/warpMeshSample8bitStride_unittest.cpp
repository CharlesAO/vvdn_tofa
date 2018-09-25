//absoluteDiff test
//Asm function prototype:
//    void mvcvAbsoluteDiff_asm(u8** in1, u8** in2, u8** out, u32 width);
//
//Asm test function prototype:
//    void absoluteDiff_asm_test(unsigned char** in1, unsigned char** in2, unsigned char** out, unsigned int width);
//
//C function prototype:
//    void mvcvAbsoluteDiff(u8** in1, u8** in2, u8** out, u32 width);
//
//Parameter description:
// in1     - array of pointers to input lines of the first image
// in2     - array of pointers to input lines of the second image
// out     - array of pointers to output lines
// width   - width of the input line

#include "gtest/gtest.h"
#include "warpMeshSample8bitStride.h"
#include "warpMeshExpand.h"
#include "warpMeshSample8bitStride_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

#define TILE_SIZE 16
#define PADDING 32

class warpMeshSample8bitStridetest : public ::testing::Test {
 protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char **tile;
	unsigned char * out_tileC;
	unsigned char * out_tileAsm;
	half xcoord[4];
	half ycoord[4];
	float *xcoord_p;
	float *ycoord_p;
	half *mx;
	half *my;
	unsigned int tile_width;
	unsigned int tile_height;

    RandomGenerator dataGenerator;
    InputGenerator inputGen;
	DataGenerator  genfloat;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

  // virtual void TearDown() {}
};

TEST_F(warpMeshSample8bitStridetest , dummy_test)
{
	inputGen.SelectGenerator("random");
	//genfloat.SelectGenerator("random");
	
	tile_width = randGen->GenerateUInt(20, 90, 1);
	tile_height = randGen->GenerateUInt(20, 90, 1);
	//tile_width = 60;
	//tile_height = 38;
	
	xcoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_width/2);  ycoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_height/2);
	xcoord[0] = *xcoord_p;                                                ycoord[0] = *ycoord_p;                                                 
	xcoord_p = (float *)inputGen.GetLineFloat(1, xcoord[0], tile_width-1);  ycoord_p = (float *)inputGen.GetLineFloat(1, ycoord[0], tile_height-1);
	xcoord[1] = *xcoord_p;                                                ycoord[1] = *ycoord_p;                                                 
	xcoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_width/2);  ycoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_height/2);
	xcoord[2] = *xcoord_p;                                                ycoord[2] = *ycoord_p;                                                 
	xcoord_p = (float *)inputGen.GetLineFloat(1, xcoord[2], tile_width-1);  ycoord_p = (float *)inputGen.GetLineFloat(1, ycoord[2], tile_height-1);
	xcoord[3] = *xcoord_p;                                                ycoord[3] = *ycoord_p;
	
	
	tile = inputGen.GetLines(tile_width, tile_height, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	out_tileC   = (unsigned char *)inputGen.GetLine((TILE_SIZE * tile_width), 0);
    out_tileAsm = (unsigned char *)inputGen.GetLine((TILE_SIZE * tile_width), 0);
	mx   = inputGen.GetLineFloat16(TILE_SIZE * (TILE_SIZE + PADDING), 0.0);
	my   = inputGen.GetLineFloat16(TILE_SIZE * (TILE_SIZE + PADDING), 0.0);
	
	
	
		 mvcvWarpMeshExpand(xcoord, ycoord,   mx,   my);
		 mvcvWarpMeshSample8bitStride(tile,   out_tileC, mx, my, tile_width, tile_height);
	warpMeshSample8bitStride_asm_test(tile, out_tileAsm, mx, my, tile_width, tile_height);

	//printf("%d %d\n\n", tile_width, tile_height);
	//printf("%f %f %f %f\n\n", xcoord[0], xcoord[1], xcoord[2], xcoord[3]);
	//printf("%f %f %f %f\n\n", ycoord[0], ycoord[1], ycoord[2], ycoord[3]);
	//arrCheck.DumpArrays("tile", "tile.c", tile, tile_width * tile_height);
	//arrCheck.DumpArrays("otileA", "otileA.c", out_tileAsm, TILE_SIZE*TILE_SIZE);
	//arrCheck.DumpArrays("otileC", "otileC.c", out_tileC, TILE_SIZE*TILE_SIZE);
	//arrCheck.DumpArrays("mx", "mx.c", mx, TILE_SIZE*TILE_SIZE);
	//arrCheck.DumpArrays("my", "my.c", my, TILE_SIZE*TILE_SIZE);
	//exit(0);
	
	//arrCheck.DumpArrays("refInput", "refInput.c", tile, tile_width, tile_height);
	
	RecordProperty("CyclePerPixel", warpMeshSample8bitStrideCycleCount / (TILE_SIZE * TILE_SIZE));

	arrCheck.CompareArrays( out_tileC, out_tileAsm,  TILE_SIZE * TILE_SIZE , 1);
	
}
