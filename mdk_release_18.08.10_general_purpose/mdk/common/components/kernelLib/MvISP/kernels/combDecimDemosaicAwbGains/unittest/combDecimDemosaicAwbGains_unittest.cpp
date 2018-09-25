//combDecimDemosaicAwbGains test
//
// Test description:
//
//
//Asm function prototype:
//    void mvispCombDecimDemosaicAwbGains_asm(unsigned char **output, unsigned short **iline, unsigned short gains[3], unsigned int width)
//
//C function prototype:
//    void mvispCombDecimDemosaicAwbGains(unsigned char **output, unsigned short **iline, unsigned short gains[3], unsigned int width)
//
//Parameter description:
// in   	  - array of pointers to input lines, 4 lines necessary 
// output     - pointer to output buffer 
// gains      - white balance gains for rgb channels
// width      - number of pixels to process


#include "gtest/gtest.h"
#include "combDecimDemosaicAwbGains.h"
#include "combDecimDemosaicAwbGains_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

using ::testing::TestWithParam;
using ::testing::Values;

const unsigned short DELTA = 0;

class appTest : public ::testing::TestWithParam<unsigned int> {
 protected:
	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned short** inLines;
	unsigned char** outC;
	unsigned char** outAsm;
    unsigned short gains[3];
	unsigned int width;

	InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;	
    
    virtual void TearDown() {}
};

TEST_F(appTest , Test1)
{
    unsigned short inMatrix[4][48] =        //0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    {//  0   1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
	    {30,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 9, 0, 9, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {31, 11, 12, 13, 14, 15, 16, 17, 18, 19, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {32, 21, 22, 23, 24, 25, 26, 27, 28, 29, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {33, 31, 32, 33, 34, 35, 36, 37, 38, 39, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 0, 9, 0, 0, 7, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
	width = 16;
	gains[0] = 0x0500;
    gains[1] = 0x0040;
    gains[2] = 0x0800;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesU16(width * 2 + 16, 4, 0);
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < (int)width * 2; j++) {
			inLines[i][j] = inMatrix[i][j];
		}
	}	
	outC = inputGen.GetEmptyLines(width+16, 3);
	outAsm = inputGen.GetEmptyLines(width+16, 3);
	combDecimDemosaicAwbGains_asm_test(outAsm, inLines, gains, width);
	mvispCombDecimDemosaicAwbGains(outC, inLines, gains, width);
	outputChecker.CompareArrays(outC, outAsm, width, 3, DELTA);
//	outputChecker.DumpArrays("outC","outC.h",outC, width, 3);
//	outputChecker.DumpArrays("outAsm","outAsm.h",outAsm, width, 3);
//	outputChecker.DumpArrays("inLines","inLines.h",inLines, width * 2, 4);
}

INSTANTIATE_TEST_CASE_P(RandomInputs, appTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)2104);
);

TEST_P(appTest , TestRandomData)
{
	width = GetParam();;
	gains[0] = randGen->GenerateUInt(0x0040, 0x06FF, 0);
    gains[1] = randGen->GenerateUInt(0x0040, 0x06FF, 0);
    gains[2] = randGen->GenerateUInt(0x0040, 0x06FF, 0);
    
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width * 2 + 32, 4, 0, 4095);
	
	outC = inputGen.GetEmptyLines(width+16, 3);
	outAsm = inputGen.GetEmptyLines(width+16, 3);	
	
	combDecimDemosaicAwbGains_asm_test(outAsm, inLines, gains, width);
	mvispCombDecimDemosaicAwbGains(outC, inLines, gains, width);
	outputChecker.CompareArrays(outC, outAsm, width, 3, DELTA);
}

