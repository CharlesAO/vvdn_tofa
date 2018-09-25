//bitwiseXorMask kernel test

//Asm function prototype:
//	 void bitwiseXorMask_asm(u8** in1, u8** in2, u8** out, u8** mask, u32 width);

//Asm test function prototype:
//    	void bitwiseXorMask_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned char **mask, unsigned int width);

//C function prototype:
//  	 void bitwiseXorMask(u8** in1, u8** in2, u8** out, u8** mask, u32 width);

//width	- width of input line
//in1		- first image
//in2		- the second image
//mask	- mask
//out		- the output line

#include "gtest/gtest.h"
#include "bitwiseXorMask.h"
#include "bitwiseXorMask_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

class bitwiseXorMaskTest : public ::testing::Test {
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
	unsigned char** mask;
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

TEST_F(bitwiseXorMaskTest, TestUniformInput7and11eq12Mask1)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 7);
	inLine2 = inputGen.GetLines(width, 1, 11);
	mask 	= inputGen.GetLines(width, 1, 1);	
	outLineExpected = inputGen.GetLines(width, 1, 12);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseXorMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseXorMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseXorMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseXorMaskTest, TestEqInputsMask1)
{
	width = 160;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 145);
	mask 	= inputGen.GetLines(width, 1, 1);	
	outLineExpected = inputGen.GetLines(width, 1, 0);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseXorMask(inLine1, inLine1, outLineC, mask, width);
	bitwiseXorMask_asm_test(inLine1, inLine1, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseXorMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseXorMaskTest, TestRandomInputMinWidthMask1)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	mask 	= inputGen.GetLines(width, 1, 1);	
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	
	
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	
	mvcvBitwiseXorMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseXorMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseXorMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseXorMaskTest, TestRandomInputMaxWidthMask1)
{
	width = 1920;
	inputGen.SelectGenerator("uniform");
	mask 	= inputGen.GetLines(width, 1, 1);	
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	
	
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	
	mvcvBitwiseXorMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseXorMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseXorMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseXorMaskTest, TestRandomInputMask0)
{
	width = 720;
	inputGen.SelectGenerator("uniform");
	mask 	= inputGen.GetLines(width, 1, 0);	
	outLineC = inputGen.GetLines(width, 1, 195);
	outLineAsm = inputGen.GetLines(width, 1, 195);		
	outLineExpected = inputGen.GetLines(width, 1, 195);
	
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 254);
	
	mvcvBitwiseXorMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseXorMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseXorMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineExpected[0], outLineAsm[0], width);
}

TEST_F(bitwiseXorMaskTest, TestRandomInputRandomMask)
{
	width = 720;
	inputGen.SelectGenerator("uniform");		
	outLineC = inputGen.GetLines(width, 1, 195);
	outLineAsm = inputGen.GetLines(width, 1, 195);		
	
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 254);
	mask 	= inputGen.GetLines(width, 1, 0, 2);
	
	mvcvBitwiseXorMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseXorMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseXorMaskCycleCount / width);			

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}