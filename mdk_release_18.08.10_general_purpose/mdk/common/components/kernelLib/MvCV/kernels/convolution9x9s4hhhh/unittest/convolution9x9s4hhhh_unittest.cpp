//convolution9x9s4hhhh kernel test
//Asm function prototype:
//     void Convolution9x9s4hhhh_asm(u8** in, u8** out, half conv[81], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution9x9s4hhhh_asm(unsigned char** in, unsigned char** out, half conv[81], unsigned int inWidth);
//
//C function prototype:
//     void Convolution9x9s4hhhh(u8** in, u8** out, float conv[81], u32 inWidth);
//
//Parameter description:
// stride = 2(s2); input data type = half(h); coefficient data type = half(h); computation/accumulation datatype = half(h); output data type = half(h);
// @param[in] in         - Input lines
// @param[in] out        - Output line
// @param[in] conv       - Array of Fp16 values from 9x9 convolution matrix
// @param[in] inWidth    - Width of input line

#include "gtest/gtest.h"
///
#include "convolution9x9s4hhhh.h"
#include "convolution9x9s4hhhh_asm_test.h"
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

class Convolution9x9s4hhhhKernelTest : public ::testing::TestWithParam< unsigned int > {
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

TEST_F(Convolution9x9s4hhhhKernelTest, TestUniformInputLinesMinimumWidth)
{
	width = 32;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 9, 4);
	outLinesC = inputGen.GetLinesFloat16(width, 1, 1);
	outLinesAsm = inputGen.GetLinesFloat16(width, 1, 1);

	inputGen.FillLine(convMat, 81, 0.012345);
	

	Convolution9x9s4hhhh_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution9x9s4hhhhCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution9x9s4hhhh(inLines, outLinesC, convMat, width);
	
	//outputChecker.ExpectAllInRange<unsigned char>(outLinesAsm[0], width, 3, 4);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(Convolution9x9s4hhhhKernelTest, TestUniformInputLinesAll0)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 9, 0);
	outLinesC = inputGen.GetLinesFloat16(width, 1, 1);
	outLinesAsm = inputGen.GetLinesFloat16(width, 1, 1);

	inputGen.FillLine(convMat, 81, 0.012345);

	Convolution9x9s4hhhh_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution9x9s4hhhhCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution9x9s4hhhh(inLines, outLinesC, convMat, width);
	
	//outputChecker.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 0);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(Convolution9x9s4hhhhKernelTest, TestUniformInputLinesAll255)
{
	width = 160;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 9, 255);
	outLinesC = inputGen.GetLinesFloat16(width, 1, 1);
	outLinesAsm = inputGen.GetLinesFloat16(width, 1, 1);

	inputGen.FillLine(convMat, 81, 0.012345);

	Convolution9x9s4hhhh_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution9x9s4hhhhCycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution9x9s4hhhh(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(Convolution9x9s4hhhhKernelTest, TestUniformInputLinesRandomConvMatrix)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 9, 3);
	outLinesC = inputGen.GetLinesFloat16(width, 1, 1);
	outLinesAsm = inputGen.GetLinesFloat16(width, 1, 1);

	inputGen.SelectGenerator("random");
	inputGen.FillLine(convMat, 81, 0.0f, 0.1f);

	Convolution9x9s4hhhh_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution9x9s4hhhhCycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution9x9s4hhhh(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(Convolution9x9s4hhhhKernelTest, TestRandomInputLinesRandomConvMatrix)
{
	width = 320;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 9, 0, 255);
	outLinesC = inputGen.GetLinesFloat16(width, 1, 1);
	outLinesAsm = inputGen.GetLinesFloat16(width, 1, 1);

	inputGen.FillLine(convMat, 81, 0.0f, 0.01f);
	
	Convolution9x9s4hhhh_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution9x9s4hhhhCycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution9x9s4hhhh(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


//------------ parameterized tests ----------------------------------------

INSTANTIATE_TEST_CASE_P(UniformInputs, Convolution9x9s4hhhhKernelTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution9x9s4hhhhKernelTest, TestRandomInputLinesBlurFilter)
{
	unsigned int width = GetParam();
	inputGen.SelectGenerator("uniform");
	outLinesC = (half**)inputGen.GetLines(width*2, 1, 1);
	outLinesAsm = (half**)inputGen.GetLines(width*2, 1, 1);
	inputGen.FillLine(convMat, 81, 1.0f / 81);

	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 9, 0, 255);

	Convolution9x9s4hhhh_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution9x9s4hhhhCycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution9x9s4hhhh(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

