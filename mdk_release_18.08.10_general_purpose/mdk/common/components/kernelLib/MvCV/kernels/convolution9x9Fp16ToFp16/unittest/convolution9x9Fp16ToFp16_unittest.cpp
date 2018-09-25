//convolution9x9Fp16ToFp16 kernel test
//Asm function prototype:
//     void Convolution9x9Fp16ToFp16_asm(u8** in, u8** out, half conv[][7], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution9x9Fp16ToFp16_asm(unsigned char** in, unsigned char** out, half conv[][7], unsigned int inWidth);
//
//C function prototype:
//     void Convolution9x9Fp16ToFp16(u8** in, u8** out, float conv[][7], u32 inWidth);
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
///
#include "convolution9x9Fp16ToFp16.h"
#include "convolution9x9Fp16ToFp16_asm_test.h"
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
half  DELTA=1; //accepted tolerance between C and ASM results
#define SIZE 9

class Convolution9x9Fp16ToFp16KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	half convMat[81];
	half *cMat;
	
	half **inLines;
	half **outLinesC;
	half **outLinesAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(Convolution9x9Fp16ToFp16KernelTest, TestUniformInputLinesMinimumWidth)
{
	width = 32;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 9, 4);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inputGen.FillLine(convMat, 81, 0.012345);
	

	Convolution9x9Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution9x9Fp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution9x9Fp16ToFp16(inLines, outLinesC, convMat, width);
	
	//outputChecker.ExpectAllInRange<unsigned char>(outLinesAsm[0], width, 3, 4);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(Convolution9x9Fp16ToFp16KernelTest, TestUniformInputLinesAll0)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 9, 0);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inputGen.FillLine(convMat, 81, 0.012345);

	Convolution9x9Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution9x9Fp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution9x9Fp16ToFp16(inLines, outLinesC, convMat, width);
	
	//outputChecker.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 0);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(Convolution9x9Fp16ToFp16KernelTest, TestUniformInputLinesAll255)
{
	width = 160;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 9, 255);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inputGen.FillLine(convMat, 81, 0.012345);

	Convolution9x9Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution9x9Fp16ToFp16CycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution9x9Fp16ToFp16(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(Convolution9x9Fp16ToFp16KernelTest, TestUniformInputLinesRandomConvMatrix)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 9, 3);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inputGen.SelectGenerator("random");
	inputGen.FillLine(convMat, 81, 0.0f, 0.1f);

	Convolution9x9Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution9x9Fp16ToFp16CycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution9x9Fp16ToFp16(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(Convolution9x9Fp16ToFp16KernelTest, TestRandomInputLinesRandomConvMatrix)
{
	width = 320;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 9, 0, 255);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inputGen.FillLine(convMat, 81, 0.0f, 0.01f);
	
	Convolution9x9Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution9x9Fp16ToFp16CycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution9x9Fp16ToFp16(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


//------------ parameterized tests ----------------------------------------

INSTANTIATE_TEST_CASE_P(UniformInputs, Convolution9x9Fp16ToFp16KernelTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution9x9Fp16ToFp16KernelTest, TestRandomInputLinesBlurFilter)
{
	half   convMatC[81];
	half    convMatAsm[81];

	inputGen.SelectGenerator("uniform");
	
	inputGen.FillLine(convMat, 81, 1.0f / 81);

	unsigned int width = GetParam();

	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 9, 0, 255);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	Convolution9x9Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution9x9Fp16ToFp16CycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution9x9Fp16ToFp16(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

