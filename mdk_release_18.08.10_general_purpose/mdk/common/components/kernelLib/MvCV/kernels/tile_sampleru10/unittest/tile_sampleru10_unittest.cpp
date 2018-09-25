//absoluteDiff test
//Asm function prototype:
//    void mvcvtile_sampleru10(unsigned short *tile, unsigned short * out_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height);
//
//Asm test function prototype:
//    void tile_sampleru10_asm_test(unsigned short *input_tile, unsigned short * output_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height);  
//
//C function prototype:
//    void mvcvtile_sampleru10(unsigned short *tile, unsigned short * out_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height);
//
//Parameter description:
/// @param[in] tile                 - Input tile of data, (u10 format) it has variable width/height size
/// @param[out] out_tile            - Output tile (u16 format) it has a 64*64 fixed dimension
/// @param[in] xcoord               - array of xcoords of the 4 points
/// @param[in] ycoord               - array of ycoords of the 4 points
/// @param[in] tile_width           - tile_width of the input tile + the eventualy padding when making the interpolation
/// @param[in] tile_height          - tile_height of the input tile + the eventualy padding when making the interpolation

#include "gtest/gtest.h"
#include "tile_sampleru10.h"
#include "tile_sampleru10_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

class tile_sampleru10test : public ::testing::Test {
 protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned short* tile;
	unsigned short* out_tileC;
	unsigned short* out_tileAsm;
	float xcoord[4];
	float ycoord[4];
	float* xcoord_p;
	float* ycoord_p;
	unsigned int tile_width;
	unsigned int tile_height;
    
    RandomGenerator dataGenerator;
    InputGenerator inputGen;
	DataGenerator  genfloat;
	ArrayChecker outputChecker;
    
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

  // virtual void TearDown() {}
};
#define DELTA 10
TEST_F(tile_sampleru10test , dummy_test)
{
	//FILE * pFile;
	inputGen.SelectGenerator("random");
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
	
	tile = inputGen.GetLineU16((tile_width) * (tile_height), 0x384, 0x3ff);
	
	//pFile = fopen ("input.hex","wb");
	//fwrite (tile , 2, tile_width*tile_height, pFile);
	//fclose (pFile); 
	inputGen.SelectGenerator("uniform");
	out_tileC = inputGen.GetLineU16(16*16, 0);
	out_tileAsm = inputGen.GetLineU16(16*16, 0);

	

	mvcvtile_sampleru10(tile,   out_tileC, xcoord, ycoord, tile_width, tile_height);
	//pFile = fopen ("outputs_C.hex","wb");
	//fwrite (out_tileC ,2,  16*16, pFile);
	//fclose (pFile); 
	
	tile_sampleru10_asm_test(tile, out_tileAsm, xcoord, ycoord, tile_width, tile_height);
    //pFile = fopen ("outputs_asm.hex","wb");
	//fwrite (out_tileAsm ,2, 16*16, pFile);
	//fclose (pFile); 
	
	
	//printf("%d %d\n\n", tile_width, tile_height);
	//printf("%f %f %f %f\n\n", xcoord[0], xcoord[1], xcoord[2], xcoord[3]);
	//printf("%f %f %f %f\n\n", ycoord[0], ycoord[1], ycoord[2], ycoord[3]);
	//arrCheck.DumpArrays("tile", "tile.c", tile, tile_width * tile_height);
	//arrCheck.DumpArrays("otile", "otile.c", out_tileAsm, 16*16);
	//exit(0);
	RecordProperty("CyclePerPixel", tile_sampleru10CycleCount / (16 * 16));
	
	outputChecker.CompareArrays((unsigned char*)out_tileC, (unsigned char*)out_tileAsm, 16 * 16,DELTA);
	
}
