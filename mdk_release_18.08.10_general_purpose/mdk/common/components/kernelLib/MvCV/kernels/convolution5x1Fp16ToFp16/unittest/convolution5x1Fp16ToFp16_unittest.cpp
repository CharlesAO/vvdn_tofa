//convolution5x1 kernel test
//Asm function prototype:
//     void Convolution5x1Fp16ToFp16_asm(half** in, half** out, half conv[5], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution5x1Fp16ToFp16_asm(half** in, half** out, half conv[5], unsigned int inWidth);
//
//C function prototype:
//     void Convolution5x1Fp16ToFp16(half** in, half** out, half conv[5], u32 inWidth);
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
#include "convolution5x1Fp16ToFp16.h"
#include "convolution5x1Fp16ToFp16_asm_test.h"
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
#define KERNEL_WIDTH 1
#define KERNEL_HEIGHT 5
half DELTA = 1;//accepted tolerance between C and ASM results


class Convolution5x1KernelTest : public ::testing::TestWithParam< unsigned int > {
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


TEST_F(Convolution5x1KernelTest, TestUniformInput120UniformConvMat)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines 	= inputGen.GetLinesFloat16(width, KERNEL_HEIGHT, 120.0);
	outLinesC 	= inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);
	inputGen.FillLine(convMat, MAT_SIZE, 0.02f);
	
	mvcvConvolution5x1Fp16ToFp16(inLines, outLinesC, convMat, width);
	Convolution5x1Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x1Fp16ToFp16CycleCount / width);	
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);

}


TEST_F(Convolution5x1KernelTest, TestWidth8ExpectedOutput)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines 	= inputGen.GetLinesFloat16(width, KERNEL_HEIGHT, 0.0);
	outLinesC 	= inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);
	expectedOutput = inputGen.GetEmptyLinesFp16(3, 1);
	expectedOutput[0][0] = 13.390625;
	expectedOutput[0][1] = 5.105469;
	expectedOutput[0][2] = 0.491455;
	
	
	inLines[0][0] = 123.67;		inLines[0][1] = 23.67;		inLines[0][2] = 3.67;
	inLines[1][0] = 155.884;    inLines[1][1] = 55.884;     inLines[1][2] = 5.884;
	inLines[2][0] = 173.67;     inLines[2][1] = 73.67;      inLines[2][2] = 3.67;
	inLines[3][0] = 167.153;    inLines[3][1] = 67.153;     inLines[3][2] = 7.153;
	inLines[4][0] = 188.258;    inLines[4][1] = 88.258;     inLines[4][2] = 8.258;
	
	half convMat[MAT_SIZE] = {0.013947, 0.023376, 0.011627, 0.018127, 0.015793};	
	
	mvcvConvolution5x1Fp16ToFp16(inLines, outLinesC, convMat, width);
	Convolution5x1Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x1Fp16ToFp16CycleCount / width);	
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);		
	outputChecker.CompareArrays(outLinesAsm, expectedOutput, 3, 1, DELTA);		
}


TEST_F(Convolution5x1KernelTest, TestRandomInputLinesRandomConvMatrixMinWidth)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width, KERNEL_HEIGHT, 0.0, 255.0);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);
	inputGen.FillLine(convMat, MAT_SIZE, 0.01f, 0.03f);
	
	mvcvConvolution5x1Fp16ToFp16(inLines, outLinesC, convMat, width);
	Convolution5x1Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x1Fp16ToFp16CycleCount / width);	
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}


TEST_F(Convolution5x1KernelTest, TestRandomInputLinesRandomConvMatrixMaxWidth)
{
	width = 32;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width, KERNEL_HEIGHT, 0.0, 255.0);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);
	inputGen.FillLine(convMat, MAT_SIZE, 0.01f, 0.03f);
	
	mvcvConvolution5x1Fp16ToFp16(inLines, outLinesC, convMat, width);
	Convolution5x1Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x1Fp16ToFp16CycleCount / width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}


//------------ parameterized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(UniformInputs, Convolution5x1KernelTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution5x1KernelTest, TestRandomInputParameterizedWidth)
{
	unsigned int width = GetParam();
	inputGen.SelectGenerator("random");
	inLines 	= inputGen.GetLinesFloat16(width, KERNEL_HEIGHT, 0.0, 255.0);
	outLinesC 	= inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);
	inputGen.FillLine(convMat, MAT_SIZE, 0.01f, 0.03f);
	
	mvcvConvolution5x1Fp16ToFp16(inLines, outLinesC, convMat, width);
	Convolution5x1Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x1Fp16ToFp16CycleCount / width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}
