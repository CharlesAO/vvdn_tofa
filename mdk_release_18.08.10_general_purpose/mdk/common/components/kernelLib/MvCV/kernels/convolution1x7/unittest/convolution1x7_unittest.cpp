//convolution1x7 kernel test
//Asm function prototype:
//     void Convolution1x7_asm(u8** in, u8** out, half* conv, u32 inWidth);
//
//Asm test function prototype:
//     void Convolution1x7_asm(unsigned char** in, unsigned char** out, half* conv, unsigned int inWidth);
//
//C function prototype:
//     void Convolution1x7(u8** in, u8** out, float* conv, u32 inWidth);
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
#include "convolution1x7.h"
#include "convolution1x7_asm_test.h"
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
#define SIZE 1

class Convolution1x7KernelTest : public ::testing::TestWithParam< unsigned int > {
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


TEST_F(Convolution1x7KernelTest, TestUniformInputLinesWidthAll0)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 1, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half* convMat = inputGen.GetLineFloat16(7, 1.0);

	Convolution1x7_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x7CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x7(inLines, outLinesC, convMat, width);
	
	outputChecker.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 0);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(Convolution1x7KernelTest, TestUniformInputLinesAll255)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 1, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half* convMat = inputGen.GetLineFloat16(7, 1.0);

	Convolution1x7_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x7CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x7(inLines, outLinesC, convMat, width);
	
	outputChecker.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 0);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Convolution1x7KernelTest, TestIndividualValues)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 1, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	half convMat[7] = {-0.1, 1.5, -5.0, 14.0, -5.0, 1.5, -0.1};

	inLines[0][8] =   4;  inLines[0][12] =  23; inLines[0][16] =    0;
	inLines[0][9] =   8;  inLines[0][13] =  16; inLines[0][17] =   25;
	inLines[0][10] = 10;  inLines[0][14] = 255; inLines[0][18] =   16;
	inLines[0][11] =  0;  inLines[0][15] =   2; inLines[0][19] =    1;

	Convolution1x7_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x7CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x7(inLines, outLinesC, convMat, width);
	
	EXPECT_EQ(31, outLinesAsm[0][0]);
	EXPECT_EQ(39, outLinesAsm[0][1]);
	EXPECT_EQ(138, outLinesAsm[0][2]);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(Convolution1x7KernelTest, TestUniformInputDifferentFilter)
{
	width = 320;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 1, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half convMat[7] = {-0.1, 1.5, -5.0, 5.0, -5.0, 1.5, -0.1};

	Convolution1x7_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x7CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x7(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(Convolution1x7KernelTest, TestRandomInputDifferentFilter)
{
	width = 1200;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 1, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half convMat[7] = {-0.1, 1.5, -5.0, 5.0, -5.0, 1.5, -0.1};

	Convolution1x7_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x7CycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x7(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(Convolution1x7KernelTest, TestUniformInputRandomFilter)
{
	width = 800;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 1, 20);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	inputGen.SelectGenerator("random");
	half *convMat = inputGen.GetLineFloat16(7, -3.0f, 10.0f);
	
	Convolution1x7_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x7CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x7(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(Convolution1x7KernelTest, TestRandomInputMinimumWidthSize)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 1, 20);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half convMat[7] = {-0.1, 1.5, -5.0, 5.0, -5.0, 1.5, -0.1};

	Convolution1x7_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x7CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x7(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(Convolution1x7KernelTest, TestRandomInputRandomFilter)
{
	width = 24;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 1, 20);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	half *convMat = inputGen.GetLineFloat16(7, -3.0f, 10.0f);
	
	
	Convolution1x7_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x7CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x7(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


//-------------------- parameterized tests -------------------------------

INSTANTIATE_TEST_CASE_P(RandomInputs, Convolution1x7KernelTest,
		Values((unsigned int)32, (unsigned int)328, (unsigned int)640, (unsigned int)800, (unsigned int)1024, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution1x7KernelTest, TestRandomInputDifferentWidths)
{
	width = GetParam();
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 1, 20);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	half *convMat = inputGen.GetLineFloat16(7, -3.0f, 10.0f);
	
	Convolution1x7_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x7CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution1x7(inLines, outLinesC, convMat, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}
