//SAD 11x11 kernel test

//Asm function prototype:
//	 void mvcvSumOfSquaredDiff11x11_asm(u8** in1, u8** in2, u8** out, u32 width);

//Asm test function prototype:
//   void sumOfSquaredDiff11x11_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned int width);

//C function prototype:
//   void mvcvSumOfSquaredDiff11x11(u8** in1, u8** in2, u8** out, u32 width);

//width	- width of input line
//in1		- first image
//in2		- the second image
//out		- the output line

#include "gtest/gtest.h"
#include "sumOfSquaredDiff11x11.h"
#include "sumOfSquaredDiff11x11_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define PADDING 16
#define SIZE 11

class sumOfSquaredDiff11x11Test : public ::testing::Test {
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

TEST_F(sumOfSquaredDiff11x11Test, TestEqualInputs)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 127);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 127);
	outLineExpected = inputGen.GetLines(width, 1, 0);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	
		
	sumOfSquaredDiff11x11_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfSquaredDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfSquaredDiff11x11(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineExpected[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}


TEST_F(sumOfSquaredDiff11x11Test, TestSSD121)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 49);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 50);
	outLineExpected = inputGen.GetLines(width, 1, 121);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	
		
	sumOfSquaredDiff11x11_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfSquaredDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfSquaredDiff11x11(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineExpected[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}


TEST_F(sumOfSquaredDiff11x11Test, TestOnePixelDistinct)
{
	width = 720;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 215);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 215);
	outLineExpected = inputGen.GetLines(width, 1, 0);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	
	
	inLine1[2][11] = 213;	
		
	sumOfSquaredDiff11x11_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfSquaredDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfSquaredDiff11x11(inLine1, inLine2, outLineC, width);
	
	outLineExpected[0][0] = 4;  
	outLineExpected[0][1] = 4;  
	outLineExpected[0][2] = 4;  
	outLineExpected[0][3] = 4;  
	outLineExpected[0][4] = 4;  
	outLineExpected[0][5] = 4;  
	outLineExpected[0][6] = 4;  
	outLineExpected[0][7] = 4;  
	outLineExpected[0][8] = 4;  
	
	outputCheck.CompareArrays(outLineC[0], outLineExpected[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}


TEST_F(sumOfSquaredDiff11x11Test, TestRandomInput)
{
	width = 320;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	     
	
	sumOfSquaredDiff11x11_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfSquaredDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfSquaredDiff11x11(inLine1, inLine2, outLineC, width);				
		
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}


TEST_F(sumOfSquaredDiff11x11Test, TestRandomInputRandomWidth)
{
	width = randGen->GenerateUInt(0, 1921);
	width = (width >> 3)<<3; //(line width have to be mutiple of 8)
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	     
		
	sumOfSquaredDiff11x11_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfSquaredDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfSquaredDiff11x11(inLine1, inLine2, outLineC, width);		

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}


TEST_F(sumOfSquaredDiff11x11Test, TestRandomInputMinWidth)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	     
		
	sumOfSquaredDiff11x11_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfSquaredDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfSquaredDiff11x11(inLine1, inLine2, outLineC, width);		

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}


TEST_F(sumOfSquaredDiff11x11Test, TestRandomInputMaxWidth)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	     
		
	sumOfSquaredDiff11x11_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfSquaredDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfSquaredDiff11x11(inLine1, inLine2, outLineC, width);		

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}