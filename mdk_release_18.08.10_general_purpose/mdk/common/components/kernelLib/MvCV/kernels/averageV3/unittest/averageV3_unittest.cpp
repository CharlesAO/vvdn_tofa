//AverageV3 kernel test
//Asm function prototype:
//     void AverageV3_asm(half** in, half** out, u32 inWidth);
//
//Asm test function prototype:
//     void AverageV3_asm(half** in, half** out, unsigned int inWidth);
//
//C function prototype:
//     void AverageV3(half** in, half** out, u32 inWidth);
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
#include "averageV3.h"
#include "averageV3_asm_test.h"
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
half DELTA = 0.05f;//accepted tolerance between C and ASM results

class AverageV3KernelTest : public ::testing::TestWithParam< unsigned int > {
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

TEST_F(AverageV3KernelTest, TestAll255Width8)
{
	width = 8;
	half **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);


	mvcvAverageV3(inLinesOffseted, outLinesC, width);
	AverageV3_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", AverageV3CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(AverageV3KernelTest, TestAll0Width8)
{
	width = 8;
	half **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);


	mvcvAverageV3(inLinesOffseted, outLinesC, width);
	AverageV3_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", AverageV3CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(AverageV3KernelTest, TestRandomDataWidth32)
{
	width = 32;
	half **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 0,255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);


	mvcvAverageV3(inLinesOffseted, outLinesC, width);
	AverageV3_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", AverageV3CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(AverageV3KernelTest, TestRandomInputLinesRandomWidth)
{
	width = randGen->GenerateUInt(8, 1921, 8);
	half **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 0.0, 255.0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);


	mvcvAverageV3(inLinesOffseted, outLinesC, width);
	AverageV3_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", AverageV3CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(AverageV3KernelTest, TestRandomInputLinesRandomWidthBig)
{
	width = randGen->GenerateUInt(1280, 1921, 8);
	half **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 0.0, 255.0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);


	mvcvAverageV3(inLinesOffseted, outLinesC, width);
	AverageV3_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", AverageV3CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

