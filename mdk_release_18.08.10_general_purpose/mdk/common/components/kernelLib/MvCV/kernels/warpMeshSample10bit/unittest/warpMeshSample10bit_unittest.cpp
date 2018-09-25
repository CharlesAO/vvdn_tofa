//WarpMeshSample10bit
//Asm function prototype:
//    void mvcvWarpMeshSample10bit_asm(unsigned short **tile, unsigned short * out_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height);
//
//Asm test function prototype:
//  void  warpMeshSample10bit_asm_test(unsigned short **input_tile, unsigned short * output_tile, half *mx, half *my, unsigned int tile_width, unsigned int tile_height);
//
//C function prototype:
//    void mvcvWarpMeshSample10bit(unsigned short **tile, unsigned short * out_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height);
//
//Parameter description:
//
// mvcvWarpMeshSample10bit  - in order to compute everything ok we need one extra pixel to the right and one extra line at the bottom for interpolation
// tile                 	- array of pointers to input lines 
// out_tile            		- Output tile (u16 format) it has a 64*64 fixed dimension
// mx                   	- array of x coordinates relative to input tile
// my                   	- array of y coordinates relative to input tile
// tile_width           	- tile_width of the input tile + the eventualy padding when making the interpolation
// tile_height          	- tile_height of the input tile + the eventualy padding when making the interpolation
// Nothing

#include "gtest/gtest.h"
#include "warpMeshSample10bit.h"
#include "warpMeshExpand.h"
#include "warpMeshSample10bit_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

#define TILE_SIZE 16
#define PADDING 32

class warpMeshSample10bittest : public ::testing::Test {
 protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned short **tile;
	unsigned short *out_tileC;
	unsigned short *out_tileAsm;
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

TEST_F(warpMeshSample10bittest , dummy_test)
{
	inputGen.SelectGenerator("random");
	//genfloat.SelectGenerator("random");
	
	tile_width = randGen->GenerateUInt(20, 90, 1);
	tile_height = randGen->GenerateUInt(20, 90, 1);
	tile_width = 60;
	tile_height = 38;
	
	xcoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_width/2);  ycoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_height/2);
	xcoord[0] = *xcoord_p;                                                ycoord[0] = *ycoord_p;                                                 
	xcoord_p = (float *)inputGen.GetLineFloat(1, xcoord[0], tile_width-1);  ycoord_p = (float *)inputGen.GetLineFloat(1, ycoord[0], tile_height-1);
	xcoord[1] = *xcoord_p;                                                ycoord[1] = *ycoord_p;                                                 
	xcoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_width/2);  ycoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_height/2);
	xcoord[2] = *xcoord_p;                                                ycoord[2] = *ycoord_p;                                                 
	xcoord_p = (float *)inputGen.GetLineFloat(1, xcoord[2], tile_width-1);  ycoord_p = (float *)inputGen.GetLineFloat(1, ycoord[2], tile_height-1);
	xcoord[3] = *xcoord_p;                                                ycoord[3] = *ycoord_p;
	
	
	
	tile = inputGen.GetLinesU16(tile_width, tile_height, 0x0, 0xff);
	
	//pFile = fopen ("in.hex","wb");
	//fwrite (tile ,2, TILE_SIZE * TILE_SIZE, pFile);
	//fclose (pFile); 
	inputGen.SelectGenerator("uniform");
	out_tileC = inputGen.GetLineU16((TILE_SIZE * TILE_SIZE), 0);
	out_tileAsm = inputGen.GetLineU16((TILE_SIZE * TILE_SIZE), 0);
	
	
	mx   = inputGen.GetLineFloat16(TILE_SIZE * (TILE_SIZE + PADDING), 0.0);
	my   = inputGen.GetLineFloat16(TILE_SIZE * (TILE_SIZE + PADDING), 0.0);
	
	
	mvcvWarpMeshExpand(xcoord, ycoord,   mx,   my);
	mvcvWarpMeshSample10bit(tile,   out_tileC, mx, my, tile_width, tile_height);
	warpMeshSample10bit_asm_test(tile, out_tileAsm, mx, my, tile_width, tile_height);

	
	//pFile = fopen ("outputs_mono_asm.hex","wb");
	//for (n = 0; n < width; n++)
    //fwrite (out_tileAsm ,2, TILE_SIZE * TILE_SIZE, pFile);
	//fclose (pFile); 
	
	//pFile = fopen ("outputs_mono_C.hex","wb");
	//for (n = 0; n < width; n++)
    //fwrite (out_tileC, 2, TILE_SIZE * TILE_SIZE, pFile);
	//fclose (pFile); 
	
	
	RecordProperty("CyclePerPixel", warpMeshSample10bitCycleCount / (TILE_SIZE * TILE_SIZE));

	arrCheck.CompareArrays( (unsigned char*)out_tileC, (unsigned char*)out_tileAsm,  TILE_SIZE * TILE_SIZE, 1);
	
}
