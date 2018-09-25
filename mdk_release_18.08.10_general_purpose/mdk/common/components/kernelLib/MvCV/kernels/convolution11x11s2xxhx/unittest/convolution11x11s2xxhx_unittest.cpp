//Convolution11x11s2xxhx kernel test
//Asm function prototype:
//     void Convolution11x11s2xxhx_asm(u8** in, u8** out, u8 conv[][11], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution11x11s2xxhx_asm(unsigned char** in, unsigned char** out, u8 conv[][11], unsigned int inWidth);
//
//C function prototype:
//     void Convolution11x11s2xxhx(u8** in, u8** out, u8 conv[][11], u32 inWidth);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - array of pointers to output lines
// conv       - array of values from convolution
// inWidth    - width of input line


#include "gtest/gtest.h"
///
#include "convolution11x11s2xxhx.h"
#include "convolution11x11s2xxhx_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;


#define PADDING 16
#define DELTA 3 //accepted tolerance between C and ASM results
#define SIZE 11

class Convolution11x11s2xxhxKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	u8 convMat[121];
		
	unsigned char **inLines;
	unsigned char **outLinesC;
	unsigned char **outLinesAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(Convolution11x11s2xxhxKernelTest, TestUniformInputLinesMinimumWidth)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 11, 4);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inputGen.FillLine(convMat, 121, 1);
	

	Convolution11x11s2xxhx_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", Convolution11x11s2xxhxCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution11x11s2xxhx(inLines, outLinesC, convMat, width);
	
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
}

TEST_F(Convolution11x11s2xxhxKernelTest, TestUniformInputLinesAll0)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 11, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	
	inputGen.FillLine(convMat, 121, 1);

	Convolution11x11s2xxhx_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", Convolution11x11s2xxhxCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution11x11s2xxhx(inLines, outLinesC, convMat, width);
	
        outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2);
}




TEST_F(Convolution11x11s2xxhxKernelTest, TestUniformInputLinesRandomConvMatrix)
{
	width = 64;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 11, 3);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	
	inputGen.SelectGenerator("random");
        inputGen.FillLine(convMat, 121, 0, 10);
	
	Convolution11x11s2xxhx_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", Convolution11x11s2xxhxCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution11x11s2xxhx(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
}


TEST_F(Convolution11x11s2xxhxKernelTest, TestRandomInputLinesRandomConvMatrix)
{
	width = 320;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 11, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	
	u8 convMat[121];
	inputGen.FillLine(convMat, 121, 0);
	

	Convolution11x11s2xxhx_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", Convolution11x11s2xxhxCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution11x11s2xxhx(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
}


//------------ parameterized tests ----------------------------------------

INSTANTIATE_TEST_CASE_P(UniformInputs, Convolution11x11s2xxhxKernelTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution11x11s2xxhxKernelTest, TestRandomInputLinesBlurFilter)
{
	
	u8 convMat[121];

	inputGen.SelectGenerator("random");
	//inputGen.FillLine((unsigned short*)convMat, 121, 1.0f / 121);
	inputGen.FillLine(convMat, 121, 0, 10);

	unsigned int width = GetParam();

	
	inLines = inputGen.GetLines(width + PADDING, 11, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	Convolution11x11s2xxhx_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", Convolution11x11s2xxhxCycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution11x11s2xxhx(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
}
