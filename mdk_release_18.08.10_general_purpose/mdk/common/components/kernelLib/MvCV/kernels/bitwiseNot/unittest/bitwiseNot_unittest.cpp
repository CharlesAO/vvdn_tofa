//BitwiseNot kernel test

//Asm function prototype:
//	 void bitwiseNot_asm(u8** in, u8** out, u32 width);

//Asm test function prototype:
//    	void bitwiseNot_asm_test(unsigned char **input, unsigned char **output, unsigned int width);

//C function prototype:
//  	 void bitwiseNot(u8** in, u8** out, u32 width);

//width	- width of input line
//in		- input line
//out		- the output line

#include "gtest/gtest.h"
#include "bitwiseNot.h"
#include "bitwiseNot_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>


class bitwiseNotTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned char** inLine;
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


TEST_F(bitwiseNotTest, TestUniformInput7Expected248)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLines(width, 1, 7);
	outLineExpected = inputGen.GetLines(width, 1, 248);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseNot(inLine, outLineC, width);
	bitwiseNot_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", bitwiseNotCycleCount / width);
			
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseNotTest, TestRandomInputMaxWidth)
{
	width = 1920;
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 1, 0, 255);
	
	mvcvBitwiseNot(inLine, outLineC, width);
	bitwiseNot_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", bitwiseNotCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseNotTest, TestRandomInputRandomWidth)
{
	width = randGen->GenerateUInt(8, 1921, 8);
	width = (width >> 3)<<3; //(line width have to be mutiple of 8)
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 1, 0, 255);
	
	mvcvBitwiseNot(inLine, outLineC, width);
	bitwiseNot_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", bitwiseNotCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseNotTest, TestRandomInputWidth8)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 1, 0, 255);
	
	mvcvBitwiseNot(inLine, outLineC, width);
	bitwiseNot_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", bitwiseNotCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseNotTest, TestRandomInputWidth56)
{
	width = 56;
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 1, 0, 255);
	
	mvcvBitwiseNot(inLine, outLineC, width);
	bitwiseNot_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", bitwiseNotCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseNotTest, TestRandomInputWidth64)
{
	width = 64;
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 1, 0, 255);
	
	mvcvBitwiseNot(inLine, outLineC, width);
	bitwiseNot_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", bitwiseNotCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseNotTest, TestRandomInputWidth72)
{
	width = 72;
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 1, 0, 255);
	
	mvcvBitwiseNot(inLine, outLineC, width);
	bitwiseNot_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", bitwiseNotCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}