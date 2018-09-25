//convolution3x3 kernel test
//Asm function prototype:
//     void Convolution3x3_asm(u8** in, u8** out, half conv[][3], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution3x3_asm(unsigned char** in, unsigned char** out, half conv[][3], unsigned int inWidth);
//
//C function prototype:
//     void Convolution3x3(u8** in, u8** out, half conv[][3], u32 inWidth);
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
#include "convolution3x3s2xhhx.h"
///
#include "convolution3x3s2xhhx_asm_test.h"
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
#define DELTA 2 //accepted tolerance between C and ASM results
#define SIZE 3
class Convolution3x3s2xhhxKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	half convAsm[9];
	half convC[9];
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



TEST_F(Convolution3x3s2xhhxKernelTest, TestExperimental)
{
	width = 32;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 3, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width/2, 1);
	outLinesAsm = inputGen.GetEmptyLines(width/2, 1);

	half   convMatC[9] = {8.0 / 9, 9.0 / 9, 0.0 / 9, 55.0 / 9, 5.0 / 9, -1.0 / 9, 0.0 / 9, 9.0 / 9, 0.0 / 9};
	half  convMatAsm[9] = {8.0 / 9, 9.0 / 9, 0.0 / 9, 55.0 / 9, 5.0 / 9, -1.0 / 9, 0.0 / 9, 9.0 / 9, 0.0 / 9};

	Convolution3x3s2xhhx_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s2xhhxCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s2xhhx(inLines, outLinesC, convMatC, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
}


TEST_F(Convolution3x3s2xhhxKernelTest, TestUniformInputLinesWidth16)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 3, 3);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inputGen.FillLine(convC, 9, 1.0f / 9);
	inputGen.FillLine(convAsm, 9, 1.0f / 9);
	
	Convolution3x3s2xhhx_asm_test(inLines, outLinesAsm, convAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s2xhhxCycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s2xhhx(inLines, outLinesC, convC, width);
	
	outputChecker.ExpectAllInRange<unsigned char>(outLinesAsm[0], width/2, 2, 3);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
}


TEST_F(Convolution3x3s2xhhxKernelTest, TestUniformInputLinesAllValues0)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 3, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	half   convMatC[9] = {-18.0 / 9, 9.0 / 9, 0.0 / 9, -9.0 / 9, 9.0 / 9, 9.0 / 9, 0.0 / 9, 9.0 / 9, 18.0 / 9};
	half  convMatAsm[9] = {-18.0 / 9, -9.0 / 9, 0.0 / 9, -9.0 / 9, 9.0 / 9, 9.0 / 9, 0.0 / 9, 9.0 / 9, 18.0 / 9}; //emboss

	Convolution3x3s2xhhx_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s2xhhxCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s2xhhx(inLines, outLinesC, convMatC, width);
	
	outputChecker.ExpectAllEQ<unsigned char>(outLinesAsm[0], width/2, 0);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2);
}


TEST_F(Convolution3x3s2xhhxKernelTest, TestUniformInputLinesAllValues255)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 3, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	half   convMatC[9] = {1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9};
	half  convMatAsm[9] = {1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9}; //emboss

	Convolution3x3s2xhhx_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s2xhhxCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s2xhhx(inLines, outLinesC, convMatC, width);
	
	outputChecker.ExpectAllInRange<unsigned char>(outLinesAsm[0], width/2, 254, 255);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
}

//------------ test different convolution matrices ------------------------

TEST_F(Convolution3x3s2xhhxKernelTest, TestRandomLinesSobel)
{
	width = 640;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 3, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	half   convMatC[9] = {1.0 / 9, 0.0 / 9, 1.0 / 9, 2.0 / 9, 0.0 / 9, 2.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9};
	half  convMatAsm[9] = {1.0 / 9, 0.0 / 9, 1.0 / 9, 2.0 / 9, 0.0 / 9, 2.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9};

	Convolution3x3s2xhhx_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s2xhhxCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s2xhhx(inLines, outLinesC, convMatC, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
}

TEST_F(Convolution3x3s2xhhxKernelTest, TestRandomLinesEdgeDetect)
{
	width = 640;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 3, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	half   convMatC[9] = {0.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9, 4.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9, 0.0 / 9};
	half  convMatAsm[9] = {0.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9, 4.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9, 0.0 / 9};

	Convolution3x3s2xhhx_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s2xhhxCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s2xhhx(inLines, outLinesC, convMatC, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
}

TEST_F(Convolution3x3s2xhhxKernelTest, TestRandomLinesSharpen)
{
	width = 640;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 3, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	half   convMatC[9] = {0.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9, 5.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9, 0.0 / 9};
	half  convMatAsm[9] = {0.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9, 5.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9, 0.0 / 9};

	Convolution3x3s2xhhx_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s2xhhxCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s2xhhx(inLines, outLinesC, convMatC, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
}

TEST_F(Convolution3x3s2xhhxKernelTest, TestRandomLinesRandomKernel)
{
	width = 640;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 3, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	half   convMatC[9];
	half  convMatAsm[9];

	for(int i = 0; i < 9; i++)
	{
		convMatC[i] = randGen->GenerateFloat16(-20, 20) / 9;
		convMatAsm[i] = (half)convMatC[i];

	}

	Convolution3x3s2xhhx_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s2xhhxCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s2xhhx(inLines, outLinesC, convMatC, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
}

//------------ parameterized tests ----------------------------------------

INSTANTIATE_TEST_CASE_P(UniformInputs, Convolution3x3s2xhhxKernelTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution3x3s2xhhxKernelTest, TestRandomInputLinesEmbossFilter)
{
	half   convMatC[9] = {18.0 / 9, 9.0 / 9, 0.0 / 9, 9.0 / 9, 9.0 / 9, 9.0 / 9, 0.0 / 9, 9.0 / 9, 18.0 / 9};
	half  convMatAsm[9] = {18.0 / 9, 9.0 / 9, 0.0 / 9, 9.0 / 9, 9.0 / 9, 9.0 / 9, 0.0 / 9, 9.0 / 9, 18.0 / 9};

	width = GetParam();

	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 3, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	Convolution3x3s2xhhx_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s2xhhxCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s2xhhx(inLines, outLinesC, convMatC, width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
}
