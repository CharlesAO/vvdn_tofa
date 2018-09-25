//ssdPointLine7x7U8U32 kernel test
//Asm function prototype:
//     void ssdPointLine7x7U8U32_asm(u8** in, u8** in2, u32** out,  u32 inWidth);
//
//Asm test function prototype:
//     void ssdPointLine7x7U8U32_asm(unsigned char** in, unsigned char** in2, unsigned int** out, unsigned int inWidth);
//
//C function prototype:
//     void ssdPointLine7x7U8U32(u8** in, u8** in2, u32** out,  u32 inWidth);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - array of pointers to output lines
// conv       - array of values from convolution
// inWidth    - width of input line
//
// each line needs a padding o kernel_size - 1 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
#include <ssdPointLine7x7U8U32.h>
#include "ssdPointLine7x7U8U32_asm_test.h"
#include "ssdPointLine7x7U8U32_unittest.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include <stdio.h>
#include <stdlib.h>


using ::testing::TestWithParam;
using ::testing::Values;


#define PADDING 8
#define DELTA 2 //accepted tolerance between C and ASM results


class ssdPointLine7x7U8U32KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}


	unsigned char **inLines1;
	unsigned char **inLines2;
	unsigned int **outLinesC;
	unsigned int **outLinesAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(ssdPointLine7x7U8U32KernelTest, TestUniformInputLinesMinimumWidth)
{
	 width = 8;
	unsigned char **inLinesOffseted1;
	unsigned char **inLinesOffseted2;
	inputGen.SelectGenerator("uniform");
	inLines1 = inputGen.GetLines(7 + 16, 7, 3);
	inLines2 = inputGen.GetLines(width + 16, 7, 1);

	inLinesOffseted1 = inputGen.GetOffsettedLines(inLines1, 7, 8);
	inLinesOffseted2 = inputGen.GetOffsettedLines(inLines2, 7, 8);
	
	outLinesC   = (unsigned int**)inputGen.GetLines((width + 16) * sizeof(unsigned int), 1, 0);
	outLinesAsm = (unsigned int**)inputGen.GetLines((width + 16) * sizeof(unsigned int) , 1, 0);

	ssdPointLine7x7U8U32_asm_test(inLines1, inLines2, outLinesAsm, width);
	mvcvSsdPointLine7x7U8U32(inLinesOffseted1, inLinesOffseted2, outLinesC, width);
	
	RecordProperty("CyclePerPixel", ssdPointLine7x7U8U32CycleCount / width);

	CompareArrays(outLinesC[0], outLinesAsm[0], width + 16);
}

TEST_F(ssdPointLine7x7U8U32KernelTest, TestUniformInputLines1)
{
	width = 512;
	unsigned char **inLinesOffseted1;
	unsigned char **inLinesOffseted2;
	inputGen.SelectGenerator("uniform");
	inLines1 = inputGen.GetLines(7 + 16, 7, 255);
	inLines2 = inputGen.GetLines(width + 16, 7, 0);

	inLinesOffseted1 = inputGen.GetOffsettedLines(inLines1, 7, 8);
	inLinesOffseted2 = inputGen.GetOffsettedLines(inLines2, 7, 8);
	
	outLinesC   = (unsigned int**)inputGen.GetLines((width + 16) * sizeof(unsigned int), 1, 0);
	outLinesAsm = (unsigned int**)inputGen.GetLines((width + 16) * sizeof(unsigned int) , 1, 0);
	
	mvcvSsdPointLine7x7U8U32(inLinesOffseted1, inLinesOffseted2, outLinesC, width);
	ssdPointLine7x7U8U32_asm_test(inLines1, inLines2, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", ssdPointLine7x7U8U32CycleCount / width);

	CompareArrays(outLinesC[0], outLinesAsm[0], width + 16);
}

TEST_F(ssdPointLine7x7U8U32KernelTest, TestUniformInputLines2)
{
	width = 512;
	unsigned char **inLinesOffseted1;
	unsigned char **inLinesOffseted2;
	inputGen.SelectGenerator("uniform");
	inLines1 = inputGen.GetLines(7 + 16, 7, 0);
	inLines2 = inputGen.GetLines(width + 16, 7, 255);

	inLinesOffseted1 = inputGen.GetOffsettedLines(inLines1, 7, 8);
	inLinesOffseted2 = inputGen.GetOffsettedLines(inLines2, 7, 8);
	
	outLinesC   = (unsigned int**)inputGen.GetLines((width + 16) * sizeof(unsigned int), 1, 0);
	outLinesAsm = (unsigned int**)inputGen.GetLines((width + 16) * sizeof(unsigned int) , 1, 0);
	
	mvcvSsdPointLine7x7U8U32(inLinesOffseted1, inLinesOffseted2, outLinesC, width);
	ssdPointLine7x7U8U32_asm_test(inLines1, inLines2, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", ssdPointLine7x7U8U32CycleCount / width);

	CompareArrays(outLinesC[0], outLinesAsm[0], width + 16);
}


TEST_F(ssdPointLine7x7U8U32KernelTest, TestRandomInputLines)
{
	width = 1920;
	unsigned char **inLinesOffseted1;
	unsigned char **inLinesOffseted2;
	inputGen.SelectGenerator("random");
	inLines1 = inputGen.GetLines(7 + 16, 7, 0, 255);
	inLines2 = inputGen.GetLines(width + 16, 7, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	inLinesOffseted1 = inputGen.GetOffsettedLines(inLines1, 7, 8);
	inLinesOffseted2 = inputGen.GetOffsettedLines(inLines2, 7, 8);
	
	outLinesC   = (unsigned int**)inputGen.GetLines((width + 16) * sizeof(unsigned int), 1, 0);
	outLinesAsm = (unsigned int**)inputGen.GetLines((width + 16) * sizeof(unsigned int) , 1, 0);
	
	mvcvSsdPointLine7x7U8U32(inLinesOffseted1, inLinesOffseted2, outLinesC, width);
	ssdPointLine7x7U8U32_asm_test(inLines1, inLines2, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", ssdPointLine7x7U8U32CycleCount / width);

	CompareArrays(outLinesC[0], outLinesAsm[0], width + 16);
}

 INSTANTIATE_TEST_CASE_P(RandomInputs, ssdPointLine7x7U8U32KernelTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);

TEST_P(ssdPointLine7x7U8U32KernelTest , TestRandomData)
{
	unsigned char **inLinesOffseted1;
	unsigned char **inLinesOffseted2;
	inputGen.SelectGenerator("random");
	width = GetParam();
	inLines1 = inputGen.GetLines(7 + 16, 7, 0, 255);
	inLines2 = inputGen.GetLines(width + 16, 7, 0, 255);

	inputGen.SelectGenerator("uniform");
	inLinesOffseted1 = inputGen.GetOffsettedLines(inLines1, 7, 8);
	inLinesOffseted2 = inputGen.GetOffsettedLines(inLines2, 7, 8);
	
	unsigned int** outLinesC   = (unsigned int**)inputGen.GetLines((width + 16) * sizeof(unsigned int), 1, 0);
	unsigned int** outLinesAsm = (unsigned int**)inputGen.GetLines((width + 16) * sizeof(unsigned int) , 1, 0);
	
	mvcvSsdPointLine7x7U8U32(inLinesOffseted1, inLinesOffseted2, outLinesC, width);
	ssdPointLine7x7U8U32_asm_test(inLines1, inLines2, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", ssdPointLine7x7U8U32CycleCount / width);

	CompareArrays(outLinesC[0], outLinesAsm[0], width + 16);
	
}