//SAD5x5 kernel test

//Asm function prototype:
//	 void mvcvSumOfAbsDiff5x5_asm(u8** in1, u8** in2, u8** out, u32 width);

//Asm test function prototype:
//   void sumOfAbsDiff5x5_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned int width);

//C function prototype:
//   void mvcvSumOfAbsDiff5x5(u8** in1, u8** in2, u8** out, u32 width);

//width	- width of input line
//in1		- first image
//in2		- the second image
//out		- the output line

#include "gtest/gtest.h"
#include "sumOfAbsDiff5x5.h"
#include "sumOfAbsDiff5x5_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define PADDING 16
#define SIZE 5

class sumOfAbsDiff5x5Test : public ::testing::Test {
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
	unsigned char** outLineC;
	unsigned char** outLineAsm;
	unsigned char** outLineExpected;
	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(sumOfAbsDiff5x5Test, TestOnePixelDistinctPadding0)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0);
	outLineExpected = inputGen.GetLines(width, 1, 0);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	for(int i = 0; i < SIZE; i++)
	{
		inputGen.FillLineRange(inLine1[i], width, PADDING/2 - 1, width + PADDING/2 - 1, 215);
		inputGen.FillLineRange(inLine2[i], width, PADDING/2 - 1, width + PADDING/2 - 1, 215);
	}	
	inLine1[2][11] = 15;	
	
	sumOfAbsDiff5x5_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfAbsDiff5x5CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);	
	mvcvSumOfAbsDiff5x5(inLine1, inLine2, outLineC, width);
			
	outLineExpected[0][1] = 200;  
	outLineExpected[0][2] = 200;  
	outLineExpected[0][3] = 200;  
	outLineExpected[0][4] = 200;  
	outLineExpected[0][5] = 200;  	
	outputCheck.CompareArrays(outLineC[0], outLineExpected[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(sumOfAbsDiff5x5Test, TestSameInput77)
{
	width = 512;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 77);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 77);
	outLineExpected = inputGen.GetLines(width, 1, 0);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	

	sumOfAbsDiff5x5_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfAbsDiff5x5CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfAbsDiff5x5(inLine1, inLine2, outLineC, width);	
	
	outputCheck.CompareArrays(outLineC[0], outLineExpected[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(sumOfAbsDiff5x5Test, TestRandomInput)
{
	width = 128;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0, 3);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0, 3);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	

	sumOfAbsDiff5x5_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfAbsDiff5x5CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfAbsDiff5x5(inLine1, inLine2, outLineC, width);
		
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(sumOfAbsDiff5x5Test, TestRandomInputMaxWidth)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0, 2);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0, 2);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);
	
	sumOfAbsDiff5x5_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfAbsDiff5x5CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfAbsDiff5x5(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(sumOfAbsDiff5x5Test, TestRandomInputMinWidth)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0, 3);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0, 3);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	

	sumOfAbsDiff5x5_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfAbsDiff5x5CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfAbsDiff5x5(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(sumOfAbsDiff5x5Test, TestRandomInputPadding0)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	  
	inputGen.SelectGenerator("random");	
	for(int i = 0; i < SIZE; i++)
	{
		inputGen.FillLineRange(inLine1[i], width, PADDING/2 - 1, width + PADDING / 2 - 1, 0, 2, 1);
		inputGen.FillLineRange(inLine2[i], width, PADDING/2 - 1, width + PADDING / 2 - 1, 0, 2, 1);
	}
	
	sumOfAbsDiff5x5_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfAbsDiff5x5CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfAbsDiff5x5(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}