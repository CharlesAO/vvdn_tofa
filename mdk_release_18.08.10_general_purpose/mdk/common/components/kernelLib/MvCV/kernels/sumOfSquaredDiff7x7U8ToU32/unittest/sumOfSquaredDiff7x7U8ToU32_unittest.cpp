//SAD7x7U8ToU32 kernel test

//Asm function prototype:
//	 void mvcvSumOfSquaredDiff7x7U8ToU32_asm(u8** in1, u8** in2, u32** out, u32 width);

//Asm test function prototype:
//    	void sumOfSquaredDiff7x7U8ToU32_asm_test(unsigned char **input1, unsigned char **input2, unsigned int **output, unsigned int width);

//C function prototype:
//	 void mvcvSumOfSquaredDiff7x7U8ToU32(u8** in1, u8** in2, u32** out, u32 width);

//width	- width of input line
//in1		- first image
//in2		- the second image
//out		- the output line

#include "gtest/gtest.h"
///
#include "sumOfSquaredDiff7x7U8ToU32.h"
#include "sumOfSquaredDiff7x7U8ToU32_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define PADDING 16
#define SIZE 7

class sumOfSquaredDiff7x7U8ToU32Test : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned char** inLine1;
	unsigned char** inLine2;
	unsigned int** outLineC;
	unsigned int** outLineAsm;
	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


TEST_F(sumOfSquaredDiff7x7U8ToU32Test, TestSameInput77)
{
	width = 512;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 77);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 77);
	outLineC = (unsigned int **)inputGen.GetEmptyLines(width * 4, 1);
	outLineAsm = (unsigned int **)inputGen.GetEmptyLines(width * 4, 1);

	sumOfSquaredDiff7x7U8ToU32_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfSquaredDiff7x7U8ToU32CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfSquaredDiff7x7U8ToU32(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(sumOfSquaredDiff7x7U8ToU32Test, TestMaxDiff)
{
	width = 320;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 255);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0);
	outLineC = (unsigned int **)inputGen.GetEmptyLines(width * 4, 1);
	outLineAsm = (unsigned int **)inputGen.GetEmptyLines(width * 4, 1);

	sumOfSquaredDiff7x7U8ToU32_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfSquaredDiff7x7U8ToU32CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfSquaredDiff7x7U8ToU32(inLine1, inLine2, outLineC, width);

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(sumOfSquaredDiff7x7U8ToU32Test, TestOnePixelDistinct)
{
	width = 160;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 123);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 123);
	outLineC = (unsigned int **)inputGen.GetEmptyLines(width * 4, 1);
	outLineAsm = (unsigned int **)inputGen.GetEmptyLines(width * 4, 1);

	sumOfSquaredDiff7x7U8ToU32_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfSquaredDiff7x7U8ToU32CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfSquaredDiff7x7U8ToU32(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(sumOfSquaredDiff7x7U8ToU32Test, TestRandomInput)
{
	width = 128;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0, 3);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0, 3);
	outLineC = (unsigned int **)inputGen.GetEmptyLines(width * 4, 1);
	outLineAsm = (unsigned int **)inputGen.GetEmptyLines(width * 4, 1);

	sumOfSquaredDiff7x7U8ToU32_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfSquaredDiff7x7U8ToU32CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfSquaredDiff7x7U8ToU32(inLine1, inLine2, outLineC, width);
		
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);	
}

TEST_F(sumOfSquaredDiff7x7U8ToU32Test, TestRandomInputMaxWidth)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0, 2);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0, 2);
	outLineC = (unsigned int **)inputGen.GetEmptyLines(width * 4, 1);
	outLineAsm = (unsigned int **)inputGen.GetEmptyLines(width * 4, 1);
	
	sumOfSquaredDiff7x7U8ToU32_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfSquaredDiff7x7U8ToU32CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfSquaredDiff7x7U8ToU32(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(sumOfSquaredDiff7x7U8ToU32Test, TestRandomInputMinWidth)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0, 3);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0, 3);
	outLineC = (unsigned int **)inputGen.GetEmptyLines(width * 4, 1);
	outLineAsm = (unsigned int **)inputGen.GetEmptyLines(width * 4, 1);

	sumOfSquaredDiff7x7U8ToU32_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfSquaredDiff7x7U8ToU32CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfSquaredDiff7x7U8ToU32(inLine1, inLine2, outLineC, width);

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(sumOfSquaredDiff7x7U8ToU32Test, TestRandomInputLines)
{
	width = 64;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width + PADDING, 7, 0, 255);
	inLine2 = inputGen.GetLines(width + PADDING, 7, 0, 255);
    outLineC   = (unsigned int**)inputGen.GetEmptyLines((width) * sizeof(unsigned int), 1);
    outLineAsm = (unsigned int**)inputGen.GetEmptyLines((width) * sizeof(unsigned int), 1);

    sumOfSquaredDiff7x7U8ToU32_asm_test(inLine1, inLine2, outLineAsm, width);
    RecordProperty("CyclePerPixel", sumOfSquaredDiff7x7U8ToU32CycleCount / width);

	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetOffsettedLines(inLine1, 7, 8);
	inLine2 = inputGen.GetOffsettedLines(inLine2, 7, 8);
	mvcvSumOfSquaredDiff7x7U8ToU32(inLine1, inLine2, outLineC, width);

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}
