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
#include "tile_sampleru8.h"
#include "tile_sampleru8_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

class tile_sampleru8test : public ::testing::Test {
 protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char *tile;
	unsigned char * out_tileC;
	unsigned char * out_tileAsm;
	float xcoord[4];
	float ycoord[4];
	float *xcoord_p;
	float *ycoord_p;
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

TEST_F(tile_sampleru8test , dummy_test)
{
	inputGen.SelectGenerator("random");
	//genfloat.SelectGenerator("random");
	
	tile_width = randGen->GenerateUInt(20, 90, 1);
	tile_height = randGen->GenerateUInt(20, 90, 1);
	
	
	xcoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_width/2);  ycoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_height/2);
	xcoord[0] = *xcoord_p;                                                ycoord[0] = *ycoord_p;                                                 
	xcoord_p = (float *)inputGen.GetLineFloat(1, xcoord[0], tile_width-1);  ycoord_p = (float *)inputGen.GetLineFloat(1, ycoord[0], tile_height-1);
	xcoord[1] = *xcoord_p;                                                ycoord[1] = *ycoord_p;                                                 
	xcoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_width/2);  ycoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_height/2);
	xcoord[2] = *xcoord_p;                                                ycoord[2] = *ycoord_p;                                                 
	xcoord_p = (float *)inputGen.GetLineFloat(1, xcoord[2], tile_width-1);  ycoord_p = (float *)inputGen.GetLineFloat(1, ycoord[2], tile_height-1);
	xcoord[3] = *xcoord_p;                                                ycoord[3] = *ycoord_p;
	
	
	tile = (unsigned char *)inputGen.GetLine((tile_width) * (tile_height), 100, 255);
	
	inputGen.SelectGenerator("uniform");
	out_tileC   = (unsigned char *)inputGen.GetLine((16 * 16), 0);
    out_tileAsm = (unsigned char *)inputGen.GetLine((16 * 16), 0);
	
		 mvcvtile_sampleru8(tile,   out_tileC, xcoord, ycoord, tile_width, tile_height);
	tile_sampleru8_asm_test(tile, out_tileAsm, xcoord, ycoord, tile_width, tile_height);

	//printf("%d %d\n\n", tile_width, tile_height);
	//printf("%f %f %f %f\n\n", xcoord[0], xcoord[1], xcoord[2], xcoord[3]);
	//printf("%f %f %f %f\n\n", ycoord[0], ycoord[1], ycoord[2], ycoord[3]);
	//arrCheck.DumpArrays("tile", "tile.c", tile, tile_width * tile_height);
	//arrCheck.DumpArrays("otile", "otile.c", out_tileAsm, 16*16);
	//exit(0);
	RecordProperty("CyclePerPixel", tile_sampleru8CycleCount / (16 * 16));

	arrCheck.CompareArrays( out_tileC, out_tileAsm,  16 * 16, 10);
	
}
