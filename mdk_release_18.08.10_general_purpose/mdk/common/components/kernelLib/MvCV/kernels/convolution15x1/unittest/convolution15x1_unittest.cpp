//convolution15x1 kernel test
//Asm function prototype:
//     void Convolution15x1_asm(u8** in, u8** out, half* conv, u32 inWidth);
//
//Asm test function prototype:
//     void Convolution15x1_asm(unsigned char** in, unsigned char** out, half* conv, unsigned int inWidth);
//
//C function prototype:
//     void Convolution15x1(u8** in, u8** out, float* conv, u32 inWidth);
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
#include "convolution15x1.h"
#include "convolution15x1_asm_test.h"
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
#define KERNEL_SIZE 15
#define delta 5
class Convolution15x1KernelTest : public ::testing::TestWithParam< unsigned int > {
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


TEST_F(Convolution15x1KernelTest, TestUniformInputLinesWidthAll0)
{
	width = 64;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half* convMat = inputGen.GetLineFloat16(KERNEL_SIZE, 1.0);

	mvcvConvolution15x1(inLines, outLinesC, convMat, width);
	Convolution15x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution15x1CycleCount / width);

	outputChecker.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 0);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(Convolution15x1KernelTest, TestUniformInputLinesAll255)
{
	width = 32;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half* convMat = inputGen.GetLineFloat16(KERNEL_SIZE, 1.0);

	mvcvConvolution15x1(inLines, outLinesC, convMat, width);
	Convolution15x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution15x1CycleCount / width);

	outputChecker.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 255);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(Convolution15x1KernelTest, TestUniformInputDifferentFilter)
{
	width = 320;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 4);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half convMat[KERNEL_SIZE] = {0.1, 1.0, -3.0, 4.0, -6.5, 8.0, -10.0, 10.0, -10.0, 8.0, -6.5, 4.0, -3.0, 1.0, 0.1};

	mvcvConvolution15x1(inLines, outLinesC, convMat, width);
	Convolution15x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution15x1CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(Convolution15x1KernelTest, TestRandomInputDifferentFilter)
{
	width = 16;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 0, 100);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half convMat[KERNEL_SIZE] = {0.1, 1.0, -3.0, 4.0, -6.5, 8.0, -10.0, 10.0, -10.0, 8.0, -6.5, 4.0, -3.0, 1.0, 0.1};

	//outputChecker.DumpArrays("inline1",  "inLine1.c", inLines[0], width);	
	//outputChecker.DumpArrays("inline2",  "inLine2.c", inLines[1], width);	
	//outputChecker.DumpArrays("inline3",  "inLine3.c", inLines[2], width);
	//outputChecker.DumpArrays("inline4",  "inLine4.c", inLines[3], width);
	//outputChecker.DumpArrays("inline5",  "inLine5.c", inLines[4], width);
	//outputChecker.DumpArrays("inline6",  "inLine6.c", inLines[5], width);
	//outputChecker.DumpArrays("inline7",  "inLine7.c", inLines[6], width);
	//outputChecker.DumpArrays("inline8",  "inLine8.c", inLines[7], width);
	//outputChecker.DumpArrays("inline9",  "inLine9.c", inLines[8], width);
	//outputChecker.DumpArrays("inline10", "inLine10.c", inLines[9], width);
	//outputChecker.DumpArrays("inline11", "inLine11.c", inLines[10], width);
	//outputChecker.DumpArrays("inline12", "inLine12.c", inLines[11], width);
	//outputChecker.DumpArrays("inline13", "inLine13.c", inLines[12], width);
	//outputChecker.DumpArrays("inline14", "inLine14.c", inLines[13], width);
	//outputChecker.DumpArrays("inline15", "inLine15.c", inLines[14], width);
		
	mvcvConvolution15x1(inLines, outLinesC, convMat, width);
	Convolution15x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution15x1CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, delta);
	//outputChecker.DumpArrays("outLineAsm", "outLineAsm.c", outLinesAsm[0], width);
	//outputChecker.DumpArrays("outLineC", "outLineC.c", outLinesC[0], width);
}


TEST_F(Convolution15x1KernelTest, TestUniformInputRandomFilter)
{
	width = 800;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 25);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	inputGen.SelectGenerator("random");
	half *convMat = inputGen.GetLineFloat16(KERNEL_SIZE, -3.0f, 10.0f);
	
	
	mvcvConvolution15x1(inLines, outLinesC, convMat, width);
	Convolution15x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution15x1CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(Convolution15x1KernelTest, TestRandomInputMinimumWidthSize)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 0, 100);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half convMat[KERNEL_SIZE] = {0.1, 1.0, -3.0, 4.0, -6.5, 8.0, -10.0, 10.0, -10.0, 8.0, -6.5, 4.0, -3.0, 1.0, 0.1};

	mvcvConvolution15x1(inLines, outLinesC, convMat, width);
	Convolution15x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution15x1CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(Convolution15x1KernelTest, TestRandomInputRandomFilter)
{
	width = 24;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 0, 100);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	half *convMat = inputGen.GetLineFloat16(KERNEL_SIZE, -3.0f, 10.0f);
	
	
	mvcvConvolution15x1(inLines, outLinesC, convMat, width);
	Convolution15x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution15x1CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

//-------------------- parameterized tests -------------------------------

INSTANTIATE_TEST_CASE_P(RandomInputs, Convolution15x1KernelTest,
		Values((unsigned int)24, (unsigned int)32, (unsigned int)328, (unsigned int)640, (unsigned int)800, (unsigned int)1024, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution15x1KernelTest, TestRandomInputDifferentWidths)
{
	width = GetParam();
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width, KERNEL_SIZE, 0, 100);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	half *convMat = inputGen.GetLineFloat16(KERNEL_SIZE, -3.0f, 10.0f);
	
	
	mvcvConvolution15x1(inLines, outLinesC, convMat, width);
	Convolution15x1_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution15x1CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

