//bitwiseOrMask kernel test

//Asm function prototype:
//	 void bitwiseOrMask_asm(u8** in1, u8** in2, u8** out, u8** mask, u32 width);

//Asm test function prototype:
//    	void bitwiseOrMask_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned char **mask, unsigned int width);

//C function prototype:
//  	 void bitwiseOrMask(u8** in1, u8** in2, u8** out, u8** mask, u32 width);

//width	- width of input line
//in1		- first image
//in2		- the second image
//mask	- mask
//out		- the output line

#include "gtest/gtest.h"
#include "bitwiseOrMask.h"
#include "bitwiseOrMask_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>


class bitwiseOrMaskTest : public ::testing::Test {
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

TEST_F(bitwiseOrMaskTest, TestUniformInput7and11eq15Mask1)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 7);
	inLine2 = inputGen.GetLines(width, 1, 11);
	mask 	= inputGen.GetLines(width, 1, 1);	
	outLineExpected = inputGen.GetLines(width, 1, 15);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseOrMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseOrMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseOrMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseOrMaskTest, TestUniformInput0and255eq255Mask1)
{
	width = 64;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 0);
	inLine2 = inputGen.GetLines(width, 1, 255);
	mask 	= inputGen.GetLines(width, 1, 1);	
	outLineExpected = inputGen.GetLines(width, 1, 255);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseOrMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseOrMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseOrMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseOrMaskTest, TestUniformInputMinWidthMask1)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 1);
	inLine2 = inputGen.GetLines(width, 1, 2);
	mask 	= inputGen.GetLines(width, 1, 1);	
	outLineExpected = inputGen.GetLines(width, 1, 3);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseOrMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseOrMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseOrMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseOrMaskTest, TestRandomInputMaxWidthMask1)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	inputGen.SelectGenerator("uniform");
	mask 	= inputGen.GetLines(width, 1, 1);	
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseOrMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseOrMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseOrMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseOrMaskTest, TestRandomInputMinWidthMask0)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	inputGen.SelectGenerator("uniform");
	mask 	= inputGen.GetLines(width, 1, 1);	
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	mvcvBitwiseOrMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseOrMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseOrMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseOrMaskTest, TestRandomInputMaxWidthMask0)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	inputGen.SelectGenerator("uniform");
	mask 	= inputGen.GetLines(width, 1, 1);	
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	mvcvBitwiseOrMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseOrMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseOrMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}


TEST_F(bitwiseOrMaskTest, TestRandomInpuMaxWidthtMask01)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	mask 	= inputGen.GetLines(width, 1, 0, 2);		
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	mvcvBitwiseOrMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseOrMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseOrMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseOrMaskTest, TestRandomInpuRandomWidthtMask01)
{
	inputGen.SelectGenerator("random");
	width = randGen->GenerateUInt(8, 1921, 8);
	width = (width >> 3)<<3; //(line width have to be mutiple of 8)
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	mask 	= inputGen.GetLines(width, 1, 0, 2);		
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	mvcvBitwiseOrMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseOrMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseOrMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseOrMaskTest, TestRandomInputMinWidthMask01)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	mask 	= inputGen.GetLines(width, 1, 0, 2);		
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	mvcvBitwiseOrMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseOrMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseOrMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}