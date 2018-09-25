//scale05Lanc6HV test
//
// Test description:
//
//
//Asm function prototype:
//    void scale05Lanc6HV_asm(UInt8 *in[6], UInt8 *out,unsigned int width)
//
//C function prototype:
//    void scale05Lanc6HV(UInt8 *in[6], UInt8 *out,unsigned int width)
//
//Parameter description:
// in   	  - array of pointers to input lines
// output     - pointer to output buffer 
// width      - number of pixels to process


#include "gtest/gtest.h"
#include "combDecimDemosaic.h"
#include "combDecimDemosaic_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 8
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

	u16 **inBuff;
	u16** inLines;
	u16** outC;
	u16** outAsm;
	unsigned int width;

	InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;	
    
    virtual void TearDown() {}
};

TEST_F(appTest , Test1)
{
    u16 inMatrix[4][48] =        //0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    {//  0   1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
	    {30, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 9, 0, 9, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {31, 11, 12, 13, 14, 15, 16, 17, 18, 19, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {32, 21, 22, 23, 24, 25, 26, 27, 28, 29, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {33, 31, 32, 33, 34, 35, 36, 37, 38, 39, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 0, 9, 0, 0, 7, 0, 0, 9, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0}
    };
	u16 **inLinesOffseted;
	width = 16;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesU16(width * 2 + PADDING * 2, 4, 0);
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 48; j++)
		{
			inLines[i][j] = inMatrix[i][j];
		}
	}	
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 4, 8);

	outC = inputGen.GetEmptyLinesU16(width, 3);
	outAsm = inputGen.GetEmptyLinesU16(width, 3);

	combDecimDemosaic_asm_test(outAsm, inLines, width);
	mvispCombDecimDemosaic(outC, inLinesOffseted, width);
    
	outputChecker.CompareArrays(outC, outAsm, width, 3, DELTA);
    
//	outputChecker.DumpArrays("outC","outC.h",outC, width, 3);
//	outputChecker.DumpArrays("outAsm","outAsm.h",outAsm, width, 3);
//	outputChecker.DumpArrays("inLines","inLines.h",inLines, width * 2 + PADDING * 2, 3);
}


TEST_F(appTest , TestRandom1)
{
	u16 **inLinesOffseted;
	width = 528;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width * 2 + PADDING * 2, 4, 0, 4095);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 4, 8);
	
	outC = inputGen.GetEmptyLinesU16(width, 3);
	outAsm = inputGen.GetEmptyLinesU16(width, 3);	
	
	combDecimDemosaic_asm_test(outAsm, inLines, width);
	mvispCombDecimDemosaic(outC, inLinesOffseted, width);
	
    outputChecker.CompareArrays(outC, outAsm, width, 3, DELTA);
}


INSTANTIATE_TEST_CASE_P(RandomInputs, appTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960);
);

TEST_P(appTest , TestRandomData)
{
	u16 **inLinesOffseted;
	width = GetParam();
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width * 2 + PADDING * 2, 4, 0, 4095);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 4, 8);
	
	outC = inputGen.GetEmptyLinesU16(width, 3);
	outAsm = inputGen.GetEmptyLinesU16(width, 3);	
	
	combDecimDemosaic_asm_test(outAsm, inLines, width);
	mvispCombDecimDemosaic(outC, inLinesOffseted, width);
	
    outputChecker.CompareArrays(outC, outAsm, width, 3, DELTA);
}

