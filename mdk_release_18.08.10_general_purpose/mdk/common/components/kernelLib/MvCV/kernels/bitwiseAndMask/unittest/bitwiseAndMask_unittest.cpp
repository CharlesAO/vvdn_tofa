//bitwiseAndMask kernel test

//Asm function prototype:
//	 void bitwiseAndMask_asm(u8** in1, u8** in2, u8** out, u8** mask, u32 width);

//Asm test function prototype:
//    	void bitwiseAndMask_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned char **mask, unsigned int width);

//C function prototype:
//  	 void bitwiseAndMask(u8** in1, u8** in2, u8** out, u8** mask, u32 width);

//width	- width of input line
//in1		- first image
//in2		- the second image
//mask	- mask
//out		- the output line

#include "gtest/gtest.h"
#include "bitwiseAndMask.h"
#include "bitwiseAndMask_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>



class bitwiseAndMaskTest : public ::testing::Test {
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

TEST_F(bitwiseAndMaskTest, TestUniformInput7and11eq3Mask1)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 7);
	inLine2 = inputGen.GetLines(width, 1, 11);
	mask 	= inputGen.GetLines(width, 1, 1);	
	outLineExpected = inputGen.GetLines(width, 1, 3);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseAndMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseAndMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseAndMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseAndMaskTest, TestRandomInputMinWidth8Mask1)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	inputGen.SelectGenerator("uniform");
	mask 	= inputGen.GetLines(width, 1, 1);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseAndMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseAndMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseAndMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseAndMaskTest, TestRandomInputWidth16Mask1)
{
	width = 16;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	inputGen.SelectGenerator("uniform");
	mask 	= inputGen.GetLines(width, 1, 1);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseAndMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseAndMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseAndMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseAndMaskTest, TestRandomInputMaxWidth1920Mask1)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	inputGen.SelectGenerator("uniform");
	mask 	= inputGen.GetLines(width, 1, 1);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseAndMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseAndMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseAndMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseAndMaskTest, TestRandomInputRandomWidthMask1)
{
	width = randGen->GenerateUInt(8, 1921, 8);
	width = (width >> 3)<<3; //(line width have to be mutiple of 8)
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	inputGen.SelectGenerator("uniform");
	mask 	= inputGen.GetLines(width, 1, 1);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvBitwiseAndMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseAndMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseAndMaskCycleCount / width);		
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseAndMaskTest, TestRandomInputWidth16Mask0)
{
	width = 16;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	inputGen.SelectGenerator("uniform");
	mask 	= inputGen.GetLines(width, 1, 0);
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	mvcvBitwiseAndMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseAndMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseAndMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseAndMaskTest, TestRandomInput1Mask01)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 255);
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);
	mask 	= inputGen.GetLines(width, 1, 0);
    for(unsigned int i = 0; i < width; i=i+2)
	{
		mask[0][i] = 1;
	}
	inputGen.SelectGenerator("random");
	inLine2 = inputGen.GetLines(width, 1, 0, 255);	
	
	mvcvBitwiseAndMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseAndMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseAndMaskCycleCount / width);
	
	outLineExpected = inLine2;
    for(unsigned int i = 1; i < width; i=i+2)
	{
		outLineExpected[0][i] = 0;
	}	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineExpected[0], width);
}

TEST_F(bitwiseAndMaskTest, TestRandomInputMinWidth8MaskRandom)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	mask 	= inputGen.GetLines(width, 1, 0, 2);
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	mvcvBitwiseAndMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseAndMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseAndMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(bitwiseAndMaskTest, TestRandomInputMaxWidth1920MaskRandom)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	mask 	= inputGen.GetLines(width, 1, 0, 50);
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	mvcvBitwiseAndMask(inLine1, inLine2, outLineC, mask, width);
	bitwiseAndMask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", bitwiseAndMaskCycleCount / width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
	
	//outputCheck.DumpArrays("inLine1", "inLine1.c", inLine1, width, 1);
	//outputCheck.DumpArrays("inLine2", "inLine2.c", inLine2, width, 1);
	//outputCheck.DumpArrays("mask", "mask.c", mask, width, 1);
	//outputCheck.DumpArrays("outLineC", "outLineC.c", outLineC, width, 1);
	//outputCheck.DumpArrays("outLineAsm", "outLineAsm.c", outLineAsm, width, 1);
}
