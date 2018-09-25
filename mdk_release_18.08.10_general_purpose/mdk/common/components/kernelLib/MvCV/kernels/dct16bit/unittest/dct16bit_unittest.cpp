//JpegEncoderDCT16bit
//Asm function prototype:
//    void mvcvDCT16bit_asm(short **in_tile, short **output_tile);
//
//Asm test function prototype:
//    void dct16bit_asm_test(short **in_tile, short **output_tile);
//
//C function prototype:
//    void mvcvDCT16bit(short **in_tile, short **output_tile);
//
//Parameter description:
//
// mvcvDCT16bit  - 
// in_tile                 	- array of pointers to input lines 
// out_tile            		- array of pointers to output lines
// Nothing

#include "gtest/gtest.h"
#include "dct16bit.h"
#include "dct16bit_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"


#define TILE_WIDTH  8
#define TILE_HEIGHT 8
#define DELTA 1.0
class DCT16bittest : public ::testing::Test {
 protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	short  input_tile[TILE_HEIGHT][TILE_WIDTH];
    short  output_tileC[TILE_HEIGHT][TILE_WIDTH];
	short  output_tileAsm[TILE_HEIGHT][TILE_WIDTH];
		
	
	short *in_tile;
	half *data_buffer;

    RandomGenerator dataGenerator;
    InputGenerator inputGen;
	DataGenerator  genfloat;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

};

TEST_F(DCT16bittest , dummy_test)
{
	inputGen.SelectGenerator("random");

    in_tile = (short *)inputGen.GetLineU16(TILE_WIDTH*TILE_HEIGHT*2, 0, 0x3ff);
    data_buffer = inputGen.GetLineFloat16(TILE_WIDTH*TILE_HEIGHT*2, -1024, 1024);
	
	for (int i = 0; i < TILE_WIDTH*TILE_HEIGHT*2; ++i)
    {
        in_tile[i]= (short)data_buffer[i];
    }

	for(int i = 0; i < TILE_HEIGHT; i++)
	{			
		input_tile[i][0] = in_tile[i*8+0];
		input_tile[i][1] = in_tile[i*8+1];
		input_tile[i][2] = in_tile[i*8+2];
		input_tile[i][3] = in_tile[i*8+3];
		input_tile[i][4] = in_tile[i*8+4];
		input_tile[i][5] = in_tile[i*8+5];
		input_tile[i][6] = in_tile[i*8+6];
		input_tile[i][7] = in_tile[i*8+7];
		
	}
	
	mvcvDCT16bit(input_tile, output_tileC);
	dct16bit_asm_test(input_tile, output_tileAsm);
	
	RecordProperty("CyclePerPixel", dct16bitCycleCount );
	arrCheck.CompareArrays( (unsigned char*)output_tileC,(unsigned char*)output_tileAsm,TILE_WIDTH*TILE_HEIGHT*2,0);
	
}
