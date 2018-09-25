//scharr_fp16Filter kernel test
//Asm function prototype:
//     void mvcvscharr_fp16_asm(u8** in, u8** out, u32 width);
//
//Asm test function prototype:
//     void scharr_fp16_asm_test(unsigned char** in, unsigned char** out, unsigned int width);
//
//C function prototype:
//     void mvcvscharr_fp16(u8** in, u8** out, u32 width)
//
//Parameter description:
// This kernel performs scharr edge detection operator
// @param[in] in     - Input lines
// @param[out] out   - Output line
// @param[in] width  - Width of input line

#include "gtest/gtest.h"
///
#include "scharr_fp16.h"
#include "scharr_fp16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 8
half DELTA = 2;
half DELTA2 = 0.09;

class scharr_fp16KernelTest : public ::testing::TestWithParam< unsigned int > {
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
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


TEST_F(scharr_fp16KernelTest, TestUniformInputLines)
{
	width = 320;
	half **inLinesOffseted;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 20);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvScharr_fp16(inLinesOffseted, outLinesC, width);
	scharr_fp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", scharr_fp16CycleCount / width);

	outputChecker.ExpectAllEQ<half>(outLinesAsm[0], width, 0);
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}


TEST_F(scharr_fp16KernelTest, TestUniformInputLinesAll0)
{
	width = 640;
	half **inLinesOffseted;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvScharr_fp16(inLinesOffseted, outLinesC, width);
	scharr_fp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", scharr_fp16CycleCount / width);

	outputChecker.ExpectAllEQ<half>(outLinesC[0], width, 0);
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(scharr_fp16KernelTest, TestUniformInputLinesAll255)
{
	width = 800;
	half **inLinesOffseted;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvScharr_fp16(inLinesOffseted, outLinesC, width);
	scharr_fp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", scharr_fp16CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(scharr_fp16KernelTest, TestDifferentUniformInputLines)
{
	width = 1280;
	half **inLinesOffseted;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 1);

	for(int i = 0; i < 3; i++) {
		inputGen.FillLine(inLines[i], width, i + 1);
	}
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvScharr_fp16(inLinesOffseted, outLinesC, width);
	scharr_fp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", scharr_fp16CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}


TEST_F(scharr_fp16KernelTest, TestMinimumLineWidth)
{
	width = 8;
	half **inLinesOffseted;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 1);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvScharr_fp16(inLinesOffseted, outLinesC, width);
	scharr_fp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", scharr_fp16CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}


//------------------- parameterized tests ------------------------
INSTANTIATE_TEST_CASE_P(RandomInputs, scharr_fp16KernelTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)40, (unsigned int)320, (unsigned int)648, (unsigned int)1024, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(scharr_fp16KernelTest, TestRandomInput)
{
	width = GetParam();
	half **inLinesOffseted;

	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 0, 1);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvScharr_fp16(inLinesOffseted, outLinesC, width);
	scharr_fp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", scharr_fp16CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA2);
}
