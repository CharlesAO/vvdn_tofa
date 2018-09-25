//BitwiseXor kernel test

//Asm function prototype:
//	 void bitwiseXor_asm(u8** in1, u8** in2, u8** out, u32 width);

//Asm test function prototype:
//    	void bitwiseXor_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned int width);

//C function prototype:
//  	 void bitwiseXor(u8** in1, u8** in2, u8** out, u32 width);

//width	- width of input line
//in1		- first image
//in2		- the second image
//out		- the output line

#include "gtest/gtest.h"
#include "bitwiseXor.h"
#include "bitwiseXor_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>


class bitwiseXorTest : public ::testing::Test {
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

TEST_F(bitwiseXorTest, TestSameInputOutput0)
{
	width = 720;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 78);
	inLine2 = inputGen.GetLines(width, 1, 78);
	outLineExpected = inputGen.GetLines(width, 1, 0); 
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseXor(inLine1, inLine2, outLineC, width);
	bitwiseXor_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", bitwiseXorCycleCount / width);
			
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseXorTest, TestUniformInput7and11eq15)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 7);
	inLine2 = inputGen.GetLines(width, 1, 11);
	outLineExpected = inputGen.GetLines(width, 1, 12); 
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseXor(inLine1, inLine2, outLineC, width);
	bitwiseXor_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", bitwiseXorCycleCount / width);
			
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseXorTest, TestUniformInputMinWidth8)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 255);
	inLine2 = inputGen.GetLines(width, 1, 0);
	outLineExpected = inputGen.GetLines(width, 1, 255);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseXor(inLine1, inLine2, outLineC, width);
	bitwiseXor_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", bitwiseXorCycleCount / width);
			
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseXorTest, TestRandomInputMaxWidth)
{
	width = 1920;
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	
	mvcvBitwiseXor(inLine1, inLine2, outLineC, width);
	bitwiseXor_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", bitwiseXorCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseXorTest, TestRandomInputRandomWidth)
{
	width = randGen->GenerateUInt(8, 1921, 8);
	width = (width >> 3)<<3; //(line width have to be mutiple of 8)
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseXor(inLine1, inLine2, outLineC, width);
	bitwiseXor_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", bitwiseXorCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}
