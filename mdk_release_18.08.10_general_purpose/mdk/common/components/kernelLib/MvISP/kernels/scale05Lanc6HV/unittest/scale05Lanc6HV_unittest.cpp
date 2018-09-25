//scale05Lanc6HV test
//
// Test description:
//
//
//Asm function prototype:
//    void mvispScale05Lanc6HV_asm(UInt8 *in[6], UInt8 *out,unsigned int width)
//
//C function prototype:
//    void mvispScale05Lanc6HV(UInt8 *in[6], UInt8 *out,unsigned int width)
//
//Parameter description:
// in   	  - array of pointers to input lines
// output     - pointer to output buffer 
// width      - number of pixels to process


#include "gtest/gtest.h"
#include "scale05Lanc6HV.h"
#include "scale05Lanc6HV_asm_test.h"
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
#define DELTA 0

class scale05Lanc6HVTest : public ::testing::TestWithParam<unsigned int> {
 protected:
	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	u8 **inBuff;
	u8** inLines;
	u8* outC;
	u8* outAsm;
	unsigned int width;

	InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;	
    
    virtual void TearDown() {}
};

TEST_F(scale05Lanc6HVTest , Test1)
{
    u8 inMatrix[6][48] =        //0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    {//  0   1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
	    {10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 56, 56, 56, 56, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 9, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 56, 56, 56, 56, 56, 1, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 56, 56, 56, 56, 56, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 56, 56, 56, 56, 56, 0, 9, 0, 0, 0, 0, 0, 0, 0, 9, 0, 9, 0, 0, 7, 0, 0, 9, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 56, 56, 56, 56, 56, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 8, 9, 8, 0, 9, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 56, 56, 56, 56, 56, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 8, 8, 8, 9, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0}
    };
	u8 **inLinesOffseted;
	width = 16;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width * 2 + PADDING * 2, 6, 0);
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 48; j++)
		{
			inLines[i][j] = inMatrix[i][j];
		}
	}	
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 6, 8);

	outC = inputGen.GetEmptyLine(width);
	outAsm = inputGen.GetEmptyLine(width);

	scale05Lanc6HV_asm_test(inLines, outAsm, width);
	mvispScale05Lanc6HV(inLinesOffseted, outC, width);
	
    outputChecker.CompareArrays((u8*)outC, (u8*)outAsm, width, (u8)DELTA);
}


INSTANTIATE_TEST_CASE_P(RandomInputs, scale05Lanc6HVTest,
		Values((unsigned int)12, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960);
);

TEST_P(scale05Lanc6HVTest , TestRandomData)
{
	u8 **inLinesOffseted;
	width = GetParam();
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width * 2 + PADDING * 2, 6, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 6, 8);
	
	outC = inputGen.GetEmptyLine(width);
	outAsm = inputGen.GetEmptyLine(width);	
	
	scale05Lanc6HV_asm_test(inLines, outAsm, width);
	mvispScale05Lanc6HV(inLinesOffseted, outC, width);
	
    outputChecker.CompareArrays((u8*)outC, (u8*)outAsm, width, (u8)DELTA);
}

