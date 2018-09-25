//medianFilter15x15 kernel test

//Asm function prototype:
//	void mvcvMedianFilter15x15_asm(u32 widthLine, u8 **outLine, u8 ** inLine);

//Asm test function prototype:
// 	void medianFilter15x15_asm_test(unsigned char width, unsigned char **outLine, unsigned char **inLine);

//C function prototype:
//	void mvcvMedianFilter15x15(u32 widthLine, u8 **outLine, u8 ** inLine);

//widthLine	- width of input line
//outLine		- array of pointers for output lines
//inLine 		 - array of pointers to input lines

#include "gtest/gtest.h"
#include <medianFilter15x15.h>
#include "medianFilter15x15_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define PADDING 8



class medianFilter15x15Test : public ::testing::Test {
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

TEST_F(medianFilter15x15Test, TestUniformInputLines32)
{
	width = 1920;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 15, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 15, 8);
	inputGen.FillLine(inLines[0], width + PADDING * 2, 9);

	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);


	mvcvMedianFilter15x15(width, outLinesC, inLinesOffseted);
	medianFilter15x15_asm_test(width, outLinesAsm, inLines);
	RecordProperty("CyclePerPixel", medianFilter15x15CycleCount / width);
	
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(medianFilter15x15Test, TestAllValuesZero)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 15, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 15, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvMedianFilter15x15(width, outLinesC, inLinesOffseted);
	medianFilter15x15_asm_test(width, outLinesAsm, inLines);
	RecordProperty("CyclePerPixel", medianFilter15x15CycleCount / width);
	
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(medianFilter15x15Test, TestAllValues255)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 15, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 15, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvMedianFilter15x15(width, outLinesC, inLinesOffseted);
	medianFilter15x15_asm_test(width, outLinesAsm, inLines);
	RecordProperty("CyclePerPixel", medianFilter15x15CycleCount / width);
	
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(medianFilter15x15Test, TestMinimumWidthSize)
{
	width = 16;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 15, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 15, 8);
	inputGen.FillLine(inLines[0], width + PADDING * 2, 25);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvMedianFilter15x15(width, outLinesC, inLinesOffseted);
	medianFilter15x15_asm_test(width, outLinesAsm, inLines);
	RecordProperty("CyclePerPixel", medianFilter15x15CycleCount / width);
	
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(medianFilter15x15Test, TestSimpleNotEqualInputLines)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 15, 1);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 15, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	for(int i = 0; i < 3; i++)
	{
		inputGen.FillLine(inLines[i], width + PADDING * 2, i + 1);
	}

	mvcvMedianFilter15x15(width, outLinesC, inLinesOffseted);
	medianFilter15x15_asm_test(width, outLinesAsm, inLines);
	RecordProperty("CyclePerPixel", medianFilter15x15CycleCount / width);
	
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(medianFilter15x15Test, TestRandomWidthRandomData)
{
	unsigned char **inLinesOffseted;
	width = randGen->GenerateUInt(8, 1921, 8);
	width = (width >> 3)<<3; //(line width have to be mutiple of 8)
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 15,0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 15, 8);

	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvMedianFilter15x15(width, outLinesC, inLinesOffseted);
	medianFilter15x15_asm_test(width, outLinesAsm, inLines);
	RecordProperty("CyclePerPixel", medianFilter15x15CycleCount / width);
	
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);


}
