//convolution1x5 kernel test
//Asm function prototype:
//     void Convolution1x5Fp16ToFp16_asm(half** in, half** out, half conv[5], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution1x5Fp16ToFp16_asm(half** in, half** out, half conv[5], unsigned int inWidth);
//
//C function prototype:
//     void Convolution1x5Fp16ToFp16(half** in, half** out, half conv[5], u32 inWidth);
//
//Parameter description:
// in         	- array of pointers to input lines
// out        	- array of pointers to output lines
// conv       	- array of values from convolution
// inWidth  	- width of input line
//
// each line needs a padding o kernel_size - 1 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
///
#include "convolution1x5Fp16ToFp16.h"
#include "convolution1x5Fp16ToFp16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;

#define MAT_SIZE 5
#define PADDING 16
#define KERNEL_WIDTH 5
#define KERNEL_HEIGHT 1
half DELTA = 2;//accepted tolerance between C and ASM results


class Convolution1x5KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	half **inLines;
	half **outLinesC;
	half **outLinesAsm;
	half **expectedOutput;
	half convMat[MAT_SIZE];
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


TEST_F(Convolution1x5KernelTest, TestUniformInput120UniformConvMat)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, KERNEL_HEIGHT, 120.0);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);
	inputGen.FillLine(convMat, MAT_SIZE, 0.2f);
	
	Convolution1x5Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x5Fp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, KERNEL_HEIGHT, PADDING / 2);
	mvcvConvolution1x5Fp16ToFp16(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);

}

TEST_F(Convolution1x5KernelTest, TestRandomInputLinesRandomConvMatrixMinWidth)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, KERNEL_HEIGHT, 0.0, 255.0);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);
	inputGen.FillLine(convMat, MAT_SIZE, 0.1f, 0.3f);
	
	Convolution1x5Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x5Fp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, KERNEL_HEIGHT, PADDING / 2);
	mvcvConvolution1x5Fp16ToFp16(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}


TEST_F(Convolution1x5KernelTest, TestRandomInputLinesRandomConvMatrixMaxWidth)
{
	width = 32;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, KERNEL_HEIGHT, 0.0, 255.0);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);
	inputGen.FillLine(convMat, MAT_SIZE, 0.1f, 0.3f);
	
	Convolution1x5Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x5Fp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, KERNEL_HEIGHT, PADDING / 2);
	mvcvConvolution1x5Fp16ToFp16(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}


//------------ parameterized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(UniformInputs, Convolution1x5KernelTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution1x5KernelTest, TestRandomInputParameterizedWidth)
{
	unsigned int width = GetParam();
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, KERNEL_HEIGHT, 0.0, 255.0);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);
	inputGen.FillLine(convMat, MAT_SIZE, 0.1f, 0.3f);
	
	Convolution1x5Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x5Fp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, KERNEL_HEIGHT, PADDING / 2);
	mvcvConvolution1x5Fp16ToFp16(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}
