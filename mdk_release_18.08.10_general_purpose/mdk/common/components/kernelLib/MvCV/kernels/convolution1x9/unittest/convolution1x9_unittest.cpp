//convolution1x9 kernel test
//Asm function prototype:
//     void Convolution1x9_asm(u8** in, u8** out, half* conv, u32 inWidth);
//
//Asm test function prototype:
//     void Convolution1x9_asm(unsigned char** in, unsigned char** out, half* conv, unsigned int inWidth);
//
//C function prototype:
//     void mvcvConvolution1x9(u8** in, u8** out, float* conv, u32 inWidth);
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
#include "convolution1x9.h"
#include "convolution1x9_asm_test.h"
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
#define DELTA 4 //accepted tolerance between C and ASM results
#define SIZE 1

class Convolution1x9KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

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


TEST_F(Convolution1x9KernelTest, TestUniformInputLinesWidthAll0)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 1, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half* convMat = inputGen.GetLineFloat16(9, 1.0);

	Convolution1x9_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x9CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x9(inLines, outLinesC, convMat, width);
	
	//outputChecker.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 0);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(Convolution1x9KernelTest, TestUniformInputLinesAll255)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 1, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half* convMat = inputGen.GetLineFloat16(9, 1.0);

	Convolution1x9_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x9CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x9(inLines, outLinesC, convMat, width);
	
	//outputChecker.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 0);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Convolution1x9KernelTest, TestUniformInputDifferentFilter)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 1, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half convMat[9] = {0.1, -2.5, 4.0, -6.0, 18.0, -6.0, 4.0, -2.5, 0.1};

	Convolution1x9_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x9CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x9(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(Convolution1x9KernelTest, TestRandomInputDifferentFilter)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 1, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half convMat[9] = {0.1, -2.5, 4.0, -6.0, 18.0, -6.0, 4.0, -2.5, 0.1};

	Convolution1x9_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x9CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x9(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(Convolution1x9KernelTest, TestUniformInputRandomFilter)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 1, 20);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inputGen.SelectGenerator("random");
	half* convMat = inputGen.GetLineFloat16(9, -3.0f, 10.0f);
	
	Convolution1x9_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x9CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x9(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(Convolution1x9KernelTest, TestRandomInputMinimumWidthSize)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 1, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half convMat[9] = {0.1, -2.5, 4.0, -6.0, 18.0, -6.0, 4.0, -2.5, 0.1};

	Convolution1x9_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x9CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x9(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(Convolution1x9KernelTest, TestRandomInputRandomFilter)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 1, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half* convMat = inputGen.GetLineFloat16(9, -3.0f, 10.0f);
	
	Convolution1x9_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x9CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x9(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


//-------------------- parameterized tests -------------------------------

INSTANTIATE_TEST_CASE_P(RandomInputs, Convolution1x9KernelTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution1x9KernelTest, TestRandomInputDifferentWidths)
{
	width = GetParam();
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 1, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half* convMat = inputGen.GetLineFloat16(9, -3.0f, 10.0f);
	
	Convolution1x9_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x9CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x9(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}
