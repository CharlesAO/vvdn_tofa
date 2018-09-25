//fast9Kernel test
//
// Test description:
//
//
//Asm function prototype:
//    void mvcvfast9u16score_asm(u8** in_lines, u8* score,  u16 *base, unsigned int thresh, unsigned int width);
//
//C function prototype:
//    void mvcvfast9u16score(u8** in_lines, u8* score,  u16 *base, unsigned int thresh, unsigned int width);
//
//Parameter description:
// in_lines   - array of pointers to input lines
// score      - pointer to corner score buffer ; first element is the number of candidates,
// base       - pointer to corner candidates buffer ; first element is the number of candidates, the rest are the position of coordinates
// thresh     - threshold
// width      - number of pixels to process


#include "gtest/gtest.h"
#include <fast9u16score.h>
#include "fast9u16score_asm_test.h"
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
#define DELTA 1

class fast9Test : public ::testing::TestWithParam<unsigned int> {
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
	u16* scoreC;
	u16* scoreAsm;
	u16 *baseC;
	u16 *baseAsm;
	unsigned int width;
	unsigned int thresh;

	InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;	
    
    virtual void TearDown() {}
};

TEST_F(fast9Test , Test1)
{
    u8 inMatrix[7][48] =        //0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    {//  0   1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
	    {10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 9, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 7, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 9, 0, 9, 0, 0, 7, 0, 0, 9, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 8, 8, 8, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 8, 9, 8, 0, 9, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 8, 8, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 8, 8, 8, 9, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
        {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0}
    };
	u8 **inLinesOffseted;
	width = 32;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 0);
	for(int i = 0; i < 7; i++)
	{
		for(int j = 0; j < 48; j++)
		{
			inLines[i][j] = inMatrix[i][j];
		}
	}	
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);

	scoreC = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	scoreAsm = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	baseC = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	baseAsm = (u16*)inputGen.GetEmptyLine(width * 2 + 4);

	thresh = 1;

	fast9u16score_asm_test(inLines, scoreAsm, baseAsm, thresh, width);
	mvcvfast9u16score(inLinesOffseted, scoreC, baseC, thresh, width);
    
    outputChecker.CompareArrays((u8*)scoreC,(u8*)scoreAsm, (*((unsigned int*)scoreC)) * 2 + 4, (u8)DELTA);
    outputChecker.CompareArrays((u8*)baseC, (u8*)baseAsm, (*((unsigned int*)baseC)) * 2 + 4, (u8)DELTA);
	
	//outputChecker.DumpArrays("refOutput", "refOutput.c", baseC, (*((unsigned int*)baseC)) + 2);
	//outputChecker.DumpArrays("AsmOutput", "AsmOutput.c", baseAsm, (*((unsigned int*)baseAsm)) + 2);	
	//outputChecker.DumpArrays("refScoreOutput", "refScoreOutput.c", scoreC, (*((unsigned int*)scoreC)) + 4);
	//outputChecker.DumpArrays("AsmScoreOutput", "AsmScoreOutput.c", scoreAsm, (*((unsigned int*)scoreAsm)) + 4);	
	
}

TEST_F(fast9Test , Test2)
{
	u8 **inLinesOffseted;
	width = 32;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	scoreC = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	scoreAsm = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	baseC = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	baseAsm = (u16*)inputGen.GetEmptyLine(width * 2 + 4);

	thresh = 48;

	fast9u16score_asm_test(inLines, scoreAsm, baseAsm, thresh, width);
	mvcvfast9u16score(inLinesOffseted, scoreC, baseC, thresh, width);
    
    outputChecker.CompareArrays((u8*)scoreC,(u8*)scoreAsm, (*((unsigned int*)scoreC)) * 2 + 4, (u8)DELTA);
    outputChecker.CompareArrays((u8*)baseC, (u8*)baseAsm, (*((unsigned int*)baseC)) * 2 + 4, (u8)DELTA);
	
	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING * 2, 7);
	//outputChecker.DumpArrays("refOutput", "refOutput.c", baseC, (*((unsigned int*)baseC)) * 2 + 4);
	//outputChecker.DumpArrays("AsmOutput", "AsmOutput.c", baseAsm, (*((unsigned int*)baseC)) * 2 + 4);
}


TEST_F(fast9Test , Test3)
{
	u8 **inLinesOffseted;
	width = 32;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 28, 88);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	scoreC = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	scoreAsm = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	baseC = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	baseAsm = (u16*)inputGen.GetEmptyLine(width * 2 + 4);

	thresh = 18;

	fast9u16score_asm_test(inLines, scoreAsm, baseAsm, thresh, width);
	mvcvfast9u16score(inLinesOffseted, scoreC, baseC, thresh, width);
    
    outputChecker.CompareArrays((u8*)scoreC,(u8*)scoreAsm, (*((unsigned int*)scoreC)) * 2 + 4, (u8)DELTA);
    outputChecker.CompareArrays((u8*)baseC, (u8*)baseAsm, (*((unsigned int*)baseC)) * 2 + 4, (u8)DELTA);
	
	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING * 2, 7);
	//outputChecker.DumpArrays("refOutput", "refOutput.c", baseC, (*((unsigned int*)baseC)) * 2 + 4);
	//outputChecker.DumpArrays("AsmOutput", "AsmOutput.c", baseAsm, (*((unsigned int*)baseC)) * 2 + 4);
}


TEST_F(fast9Test , Test4)
{
	u8 **inLinesOffseted;
	width = 32;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 28, 38);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	scoreC = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	scoreAsm = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	baseC = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	baseAsm = (u16*)inputGen.GetEmptyLine(width * 2 + 4);

	thresh = 18;

	fast9u16score_asm_test(inLines, scoreAsm, baseAsm, thresh, width);
	mvcvfast9u16score(inLinesOffseted, scoreC, baseC, thresh, width);
    
    outputChecker.CompareArrays((u8*)scoreC,(u8*)scoreAsm, (*((unsigned int*)scoreC)) * 2 + 4, (u8)DELTA);
    outputChecker.CompareArrays((u8*)baseC, (u8*)baseAsm, (*((unsigned int*)baseC)) * 2 + 4, (u8)DELTA);
	
	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING * 2, 7);
	//outputChecker.DumpArrays("refOutput", "refOutput.c", baseC, (*((unsigned int*)baseC)) * 2 + 4);
	//outputChecker.DumpArrays("AsmOutput", "AsmOutput.c", baseAsm, (*((unsigned int*)baseC)) * 2 + 4);
}

// //---------------- parameterized tests -------------------------


INSTANTIATE_TEST_CASE_P(RandomInputs, fast9Test,
		Values((unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(fast9Test , TestRandomData)
{
	u8 **inLinesOffseted;
	width = GetParam();
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	scoreC = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	scoreAsm = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	baseC = (u16*)inputGen.GetEmptyLine(width * 2 + 4);
	baseAsm = (u16*)inputGen.GetEmptyLine(width * 2 + 4);

	thresh = 8;

	fast9u16score_asm_test(inLines, scoreAsm, baseAsm, thresh, width);
	mvcvfast9u16score(inLinesOffseted, scoreC, baseC, thresh, width);
    
    outputChecker.CompareArrays((u8*)scoreC,(u8*)scoreAsm, (*((unsigned int*)scoreC)) * 2 + 4, (u8)DELTA);
    outputChecker.CompareArrays((u8*)baseC, (u8*)baseAsm, (*((unsigned int*)baseC)) * 2 + 4, (u8)DELTA);
}

