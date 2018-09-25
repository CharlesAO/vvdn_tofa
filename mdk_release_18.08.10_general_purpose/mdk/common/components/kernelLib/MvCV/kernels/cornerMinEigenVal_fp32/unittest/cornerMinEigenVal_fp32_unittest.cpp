//boxFilter kernel test
//Asm function prototype:
//     void CornerMinEigenVal_fp32_asm(u8 **in_lines, u8 **out_line, u32 width);
//
//Asm test function prototype:
//     void CornerMinEigenVal_fp32_asm_test(u8 **in_lines, u8 **out_line, u32 width);
//
//C function prototype:
//     void mvcvCornerMinEigenVal_fp32(u8 **in_lines, u8 **out_line, u32 width);
//
//Parameter description:
// CornerMinEigenVal_fp32 filter - is 5x5 kernel size
// input_lines   - pointer to input pixel
// output_line   - position on line
// width         - width of line
//
// each line needs a padding o kernel_size - 1 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
///
#include "cornerMinEigenVal_fp32.h"
#include "cornerMinEigenVal_fp32_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 8
float DELTA = 0.75;

class CornerMinEigenVal_fp32KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char normalize;
	unsigned char **inLines;
	float **outLinesC;
	float **outLinesAsm;
    float *buffer;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(CornerMinEigenVal_fp32KernelTest, TestUniformInputLines32)
{
	width = 32;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	inputGen.FillLine(inLines[0], width + PADDING * 2, 25);

	outLinesC = inputGen.GetLinesFloat(width, 1, 0);
	outLinesAsm = inputGen.GetLinesFloat(width, 1, 0);
	buffer  = inputGen.GetLineFloat( (width + PADDING * 2) * 4 * 6, 0);
	mvcvCornerMinEigenVal_fp32(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_fp32_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenVal_fp32CycleCount / width);

	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING * 2, 5);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width,DELTA);
}


TEST_F(CornerMinEigenVal_fp32KernelTest, TestAllValuesZero)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetLinesFloat(width, 1, 0);
	outLinesAsm = inputGen.GetLinesFloat(width, 1, 0);
	buffer  = inputGen.GetLineFloat( (width + PADDING * 2) * 4 * 6, 0);
	mvcvCornerMinEigenVal_fp32(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_fp32_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenVal_fp32CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width,DELTA);
}


TEST_F(CornerMinEigenVal_fp32KernelTest, TestAllValues255)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetLinesFloat(width, 1, 0);
	outLinesAsm = inputGen.GetLinesFloat(width, 1, 0);
	buffer  = inputGen.GetLineFloat( (width + PADDING * 2) * 4 * 6, 0);
	mvcvCornerMinEigenVal_fp32(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_fp32_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenVal_fp32CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(CornerMinEigenVal_fp32KernelTest, TestMinimumWidthSize)
{
	width = 16;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	inputGen.FillLine(inLines[0], width + PADDING * 2, 25);
	outLinesC = inputGen.GetLinesFloat(width, 5, 0);
	outLinesAsm = inputGen.GetLinesFloat(width, 5, 0);
	buffer  = inputGen.GetLineFloat( (width + PADDING * 2) * 4 * 6, 0);
	mvcvCornerMinEigenVal_fp32(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_fp32_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenVal_fp32CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width,DELTA);
}

TEST_F(CornerMinEigenVal_fp32KernelTest, TestSimpleNotEqualInputLines)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 1);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetLinesFloat(width, 5, 0);
	outLinesAsm = inputGen.GetLinesFloat(width, 5, 0);
	for(int i = 1; i < 5; i++)
	{
		inputGen.FillLine(inLines[i], width + PADDING * 2, i + 1);
	}
	buffer  = inputGen.GetLineFloat( (width + PADDING * 2) * 4 * 6, 0);
	mvcvCornerMinEigenVal_fp32(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_fp32_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenVal_fp32CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width,DELTA);
}

TEST_F(CornerMinEigenVal_fp32KernelTest, TestConsecutiveValues)
{

	width = 40;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0, 100);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	inputGen.SelectGenerator("uniform");
	for(int i = 0; i < 5; i++) {
		for(int j = 0; j < (int)width; j++) {
			inLines[i][j] = j;
		}
	}
	outLinesC = inputGen.GetLinesFloat(width, 5, 0);
	outLinesAsm = inputGen.GetLinesFloat(width, 5, 0);
	buffer  = inputGen.GetLineFloat( (width + PADDING * 2) * 4 * 6, 0);
	mvcvCornerMinEigenVal_fp32(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_fp32_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenVal_fp32CycleCount / width);

	outputChecker.CompareArrays( outLinesAsm[0], outLinesC[0], width, DELTA);
}


TEST_F(CornerMinEigenVal_fp32KernelTest, TestRandomWidth)
{

	width = randGen->GenerateUInt(8, 1280, 8);
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	inputGen.FillLine(inLines[0], width + PADDING * 2, 25);
	outLinesC = inputGen.GetLinesFloat(width, 5, 0);
	outLinesAsm = inputGen.GetLinesFloat(width, 5, 0);

	buffer  = inputGen.GetLineFloat( (width + PADDING * 2) * 4 * 6, 0);
	mvcvCornerMinEigenVal_fp32(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_fp32_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenVal_fp32CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(CornerMinEigenVal_fp32KernelTest, TestRandomData)
{
	width = 320;
		unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetLinesFloat(width, 1, 0);
	outLinesAsm = inputGen.GetLinesFloat(width, 1, 0);
	buffer  = inputGen.GetLineFloat( (width + PADDING * 2) * 4 * 6, 0);
	mvcvCornerMinEigenVal_fp32(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_fp32_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenVal_fp32CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(CornerMinEigenVal_fp32KernelTest, TestRandomWidthRandomData)
{

	width = randGen->GenerateUInt(8, 1280, 8);
	inputGen.SelectGenerator("random");
		unsigned char **inLinesOffseted;
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0, 255);
		inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	inputGen.SelectGenerator("uniform");
	for(int i = 0; i < 5; i++) {
		inputGen.FillLineRange(inLines[i], width + PADDING,
				width, width + PADDING - 1, inLines[i][width - 1]);
	}

	outLinesC = inputGen.GetLinesFloat(width, 5, 0);
	outLinesAsm = inputGen.GetLinesFloat(width, 5, 0);
	buffer  = inputGen.GetLineFloat( (width + PADDING * 2) * 4 * 6, 0);
	mvcvCornerMinEigenVal_fp32(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_fp32_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenVal_fp32CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}
