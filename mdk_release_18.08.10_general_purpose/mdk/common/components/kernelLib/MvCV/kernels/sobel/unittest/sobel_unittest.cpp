//sobelFilter kernel test
//Asm function prototype:
//     void mvcvSobel_asm(u8** in, u8** out, u32 width);
//
//Asm test function prototype:
//     void sobel_asm_test(unsigned char** in, unsigned char** out, unsigned int width);
//
//C function prototype:
//     void mvcvSobel(u8** in, u8** out, u32 width)
//
//Parameter description:
// sobel filter - Filter, calculates magnitude
// in     - array of pointers to input lines
// out    - array of pointers for output lines
// width  - width of input line

#include "gtest/gtest.h"
///
#include "sobel.h"
#include "sobel_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define DELTA 2
#define PADDING 8

class SobelFilterKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char **inLinesC;
	unsigned char **inLinesAsm;
	unsigned char **outLinesC;
	unsigned char **outLinesAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


TEST_F(SobelFilterKernelTest, TestUniformInputLines)
{
	width = 320;

	inputGen.SelectGenerator("uniform");
	inLinesAsm = inputGen.GetLines(width + PADDING * 2, 3, 1);
	inLinesC = inputGen.GetOffsettedLines(inLinesAsm, 3, 8);

	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvSobel(inLinesC, outLinesC, width);
	sobel_asm_test(inLinesAsm, outLinesAsm, width);
	RecordProperty("CyclePerPixel", SobelCycleCount / width);

	outputChecker.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 0);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(SobelFilterKernelTest, TestUniformInputLinesAll0)
{
	width = 640;

	inputGen.SelectGenerator("uniform");
	inLinesAsm = inputGen.GetLines(width + PADDING * 2, 3, 0);
	inLinesC = inputGen.GetOffsettedLines(inLinesAsm, 3, 8);

	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvSobel(inLinesC, outLinesC, width);
	sobel_asm_test(inLinesAsm, outLinesAsm, width);
	RecordProperty("CyclePerPixel", SobelCycleCount / width);

	outputChecker.ExpectAllEQ<unsigned char>(outLinesC[0], width, 0);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(SobelFilterKernelTest, TestUniformInputLinesAll255)
{
	width = 800;

	inputGen.SelectGenerator("uniform");
	inLinesAsm = inputGen.GetLines(width + PADDING * 2, 3, 255);
	inLinesC = inputGen.GetOffsettedLines(inLinesAsm, 3, 8);

	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvSobel(inLinesC, outLinesC, width);
	sobel_asm_test(inLinesAsm, outLinesAsm, width);
	RecordProperty("CyclePerPixel", SobelCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(SobelFilterKernelTest, TestDifferentUniformInputLines)
{
	width = 1280;

	inputGen.SelectGenerator("uniform");
	inLinesAsm = inputGen.GetLines(width + PADDING * 2, 3, 1);

	for(int i = 0; i < 3; i++) {
		inputGen.FillLine(inLinesAsm[i], width, i + 1);
	}
	inLinesC = inputGen.GetOffsettedLines(inLinesAsm, 3, 8);

	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvSobel(inLinesC, outLinesC, width);
	sobel_asm_test(inLinesAsm, outLinesAsm, width);
	RecordProperty("CyclePerPixel", SobelCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(SobelFilterKernelTest, TestMinimumLineWidth)
{
	width = 8;

	inputGen.SelectGenerator("uniform");
	inLinesAsm = inputGen.GetLines(width + PADDING * 2, 3, 1);
	inLinesC = inputGen.GetOffsettedLines(inLinesAsm, 3, 8);

	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvSobel(inLinesC, outLinesC, width);
	sobel_asm_test(inLinesAsm, outLinesAsm, width);
	RecordProperty("CyclePerPixel", SobelCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}




//------------------- parameterized tests ------------------------
INSTANTIATE_TEST_CASE_P(RandomInputs, SobelFilterKernelTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)40, (unsigned int)320, (unsigned int)648, (unsigned int)1024, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(SobelFilterKernelTest, TestRandomInput)
{
	width = GetParam();

	inputGen.SelectGenerator("random");
	inLinesAsm = inputGen.GetLines(width + PADDING * 2, 3, 0, 255);
	inLinesC = inputGen.GetOffsettedLines(inLinesAsm, 3, 8);

	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvSobel(inLinesC, outLinesC, width);
	sobel_asm_test(inLinesAsm, outLinesAsm, width);
	RecordProperty("CyclePerPixel", SobelCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

