//JpegGetBlockY420
//Asm function prototype:
//     void mvcvjpegGetBlockY420_asm(u32 mode, u8 *imginfo_Y, u32 imginfo_width, u32 imgspec_x,u32 imgspec_y, short imgspecY[8][8]);
//
//Asm test function prototype:
//    void jpegGetBlockY420_asm_test(unsigned int mode, unsigned char *imginfo_Y, unsigned int imginfo_width, unsigned int imgspec_x, unsigned int imgspec_y, short imgspecY[8][8]);
//
//C function prototype:
//     void mvcvjpegGetBlockY420(u32 mode, u8 *imginfo_Y, u32 imginfo_width, u32 imgspec_x,u32 imgspec_y, short imgspecY[8][8]);
//
//Parameter description:
//mode		 	- has one of the following values: 0, 1, 2, 3 - selects one 8x8 block from the 16x16 macroblock
//imginfo_Y		- input buffer with  imginfo_width*(imginfo_height + 1) dimension, where (imginfo_height)modulo 16 =0 
//imginfo_width - width of input buffer
//imgspec_x     - macroblock x coordinate 
//imgspec_y     - macroblock y coordinate   
//imgspecY[8][8]- output 8x8 block
#include "gtest/gtest.h"
#include "jpegGetBlockY420.h"
#include "jpegGetBlockY420_asm_test.h"
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
class jpegGetBlockY420test : public ::testing::Test {
 protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned int mode;
	unsigned int imginfo_width, imgspec_x, imgspec_y;
	unsigned char *imginfo_Y;
	short  imgspecY_C[8][8];
	short  imgspecY_Asm[8][8];
	short  *out_C;
	short  *out_Asm;
    RandomGenerator dataGenerator;
    InputGenerator inputGen;
	DataGenerator  genfloat;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

};

TEST_F(jpegGetBlockY420test , dummy_test)
{   
		
	inputGen.SelectGenerator("random");
	mode = randGen->GenerateUInt(0, 3, 1);
	imginfo_width =128;
	imgspec_x = randGen->GenerateUInt(0, 122, 16);
	imgspec_y = randGen->GenerateUInt(0, 122, 16);
	imginfo_Y = (unsigned char *)inputGen.GetLine((imginfo_width+1)*imginfo_width, 0, 255);
		
		
	mvcvjpegGetBlockY420(mode, imginfo_Y, imginfo_width, imgspec_x, imgspec_y, imgspecY_C);
	jpegGetBlockY420_asm_test(mode, imginfo_Y, imginfo_width, imgspec_x, imgspec_y, imgspecY_Asm);
		
	RecordProperty("CyclePerPixel", jpegGetBlockY420CycleCount );
	arrCheck.CompareArrays( (unsigned char*)imgspecY_C,(unsigned char*)imgspecY_Asm,TILE_WIDTH*TILE_HEIGHT*2,0);
	
	
	
}
