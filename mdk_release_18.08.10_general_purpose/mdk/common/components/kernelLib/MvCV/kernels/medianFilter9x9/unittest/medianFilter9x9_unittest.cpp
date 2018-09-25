//medianFilter9x9 kernel test

//Asm function prototype:
//	void medianFilter9x9_asm(u32 widthLine, u8 **outLine, u8 ** inLine);

//Asm test function prototype:
//    	void medianFilter9x9_asm_test(unsigned char width, unsigned char **outLine, unsigned char **inLine);

//C function prototype:
//  	void mvcvMedianFilter9x9(u32 widthLine, u8 **outLine, u8 ** inLine);

//widthLine	- width of input line
//outLine		- array of pointers for output lines
//inLine 		 - array of pointers to input lines

#include "gtest/gtest.h"
#include <medianFilter9x9.h>
#include "medianFilter9x9_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define PADDING 8



class medianFilter9x9Test : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned char** inLines;
	unsigned char** outLinesC;
	unsigned char** outLinesAsm;
	unsigned char** inLinesC;
	unsigned int width;
	unsigned int height;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(medianFilter9x9Test, TestUniformInputLines32)
{
	width = 32;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 9, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 9, 8);
	inputGen.FillLine(inLines[0], width + PADDING * 2, 9);

	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);


	mvcvMedianFilter9x9(width, outLinesC, inLinesOffseted);
	medianFilter9x9_asm_test(width, outLinesAsm, inLines);
	RecordProperty("CyclePerPixel", medianFilter9x9CycleCount / width);
	
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(medianFilter9x9Test, TestAllValuesZero)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 9, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 9, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvMedianFilter9x9(width, outLinesC, inLinesOffseted);
	medianFilter9x9_asm_test(width, outLinesAsm, inLines);
	RecordProperty("CyclePerPixel", medianFilter9x9CycleCount / width);
	
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(medianFilter9x9Test, TestAllValues255)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 9, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 9, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvMedianFilter9x9(width, outLinesC, inLinesOffseted);
	medianFilter9x9_asm_test(width, outLinesAsm, inLines);
	RecordProperty("CyclePerPixel", medianFilter9x9CycleCount / width);
	
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(medianFilter9x9Test, TestMinimumWidthSize)
{
	width = 16;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 9, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 9, 8);
	inputGen.FillLine(inLines[0], width + PADDING * 2, 25);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvMedianFilter9x9(width, outLinesC, inLinesOffseted);
	medianFilter9x9_asm_test(width, outLinesAsm, inLines);
	RecordProperty("CyclePerPixel", medianFilter9x9CycleCount / width);
	
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(medianFilter9x9Test, TestSimpleNotEqualInputLines)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 9, 1);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 9, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	for(int i = 0; i < 9; i++)
	{
		inputGen.FillLine(inLines[i], width + PADDING * 2, i + 1);
	}

	mvcvMedianFilter9x9(width, outLinesC, inLinesOffseted);
	medianFilter9x9_asm_test(width, outLinesAsm, inLines);
	RecordProperty("CyclePerPixel", medianFilter9x9CycleCount / width);
	
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(medianFilter9x9Test, TestRandomWidthRandomData)
{
	unsigned char **inLinesOffseted;
	width = randGen->GenerateUInt(8, 1921, 8);
	width = (width >> 3)<<3; //(line width have to be mutiple of 8)
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 9,0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 9, 8);

	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvMedianFilter9x9(width, outLinesC, inLinesOffseted);
	medianFilter9x9_asm_test(width, outLinesAsm, inLines);
	RecordProperty("CyclePerPixel", medianFilter9x9CycleCount / width);
	
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);


}
