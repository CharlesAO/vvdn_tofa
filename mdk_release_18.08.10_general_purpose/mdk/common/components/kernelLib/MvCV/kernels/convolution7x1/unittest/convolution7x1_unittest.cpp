//convolution7x1 kernel test
//Asm function prototype:
//     void Convolution7x1_asm(u8** in, u8** out, half* conv, u32 inWidth);
//
//Asm test function prototype:
//     void Convolution7x1_asm(unsigned char** in, unsigned char** out, half* conv, unsigned int inWidth);
//
//C function prototype:
//     void Convolution7x1(u8** in, u8** out, float* conv, u32 inWidth);
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
#include "convolution7x1.h"
#include "convolution7x1_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;

#define DELTA 3 //accepted tolerance between C and ASM results
#define KERNEL_SIZE 7

class Convolution7x1KernelTest : public ::testing::TestWithParam< unsigned int > {
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


TEST_F(Convolution7x1KernelTest, TestUniformInputLinesWidthAll0)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half* convMat = inputGen.GetLineFloat16(KERNEL_SIZE, 1.0);

	mvcvConvolution7x1(inLines, outLinesC, convMat, width);
	Convolution7x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution7x1CycleCount / width);

	outputChecker.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 0);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Convolution7x1KernelTest, TestUniformInputLinesAll255)
{
	width = 32;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half* convMat = inputGen.GetLineFloat16(KERNEL_SIZE, 1.0);

	mvcvConvolution7x1(inLines, outLinesC, convMat, width);
	Convolution7x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution7x1CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(Convolution7x1KernelTest, TestUniformInputDifferentFilter)
{
	width = 320;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 4);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half convMat[KERNEL_SIZE] = {-0.1, 1.5, -5.0, 14.0, -5.0, 1.5, -0.1};

	mvcvConvolution7x1(inLines, outLinesC, convMat, width);
	Convolution7x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution7x1CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(Convolution7x1KernelTest, TestRandomInputDifferentFilter)
{
	width = 16;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half convMat[KERNEL_SIZE] = {-0.1, 1.5, -5.0, 14.0, -5.0, 1.5, -0.1};

	mvcvConvolution7x1(inLines, outLinesC, convMat, width);
	Convolution7x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution7x1CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(Convolution7x1KernelTest, TestUniformInputRandomFilter)
{
	width = 800;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 25);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	inputGen.SelectGenerator("random");
	half *convMat = inputGen.GetLineFloat16(KERNEL_SIZE, -3.0f, 10.0f);
		
	mvcvConvolution7x1(inLines, outLinesC, convMat, width);
	Convolution7x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution7x1CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(Convolution7x1KernelTest, TestRandomInputMinimumWidthSize)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half convMat[KERNEL_SIZE] = {-0.1, 1.5, -5.0, 14.0, -5.0, 1.5, -0.1};

	mvcvConvolution7x1(inLines, outLinesC, convMat, width);
	Convolution7x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution7x1CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(Convolution7x1KernelTest, TestRandomInputRandomFilter)
{
	width = 24;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	half *convMat = inputGen.GetLineFloat16(KERNEL_SIZE, -3.0f, 10.0f);
		
	mvcvConvolution7x1(inLines, outLinesC, convMat, width);
	Convolution7x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution7x1CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

//-------------------- parameterized tests -------------------------------

INSTANTIATE_TEST_CASE_P(RandomInputs, Convolution7x1KernelTest,
		Values((unsigned int)24, (unsigned int)32, (unsigned int)328, (unsigned int)640, (unsigned int)800, (unsigned int)1024, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution7x1KernelTest, TestRandomInputDifferentWidths)
{
	width = GetParam();
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	half *convMat = inputGen.GetLineFloat16(KERNEL_SIZE, -3.0f, 10.0f);
	
	mvcvConvolution7x1(inLines, outLinesC, convMat, width);
	Convolution7x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution7x1CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}
