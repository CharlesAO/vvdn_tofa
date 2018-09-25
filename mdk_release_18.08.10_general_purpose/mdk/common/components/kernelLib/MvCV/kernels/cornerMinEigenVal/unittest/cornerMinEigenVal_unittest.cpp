//boxFilter kernel test
//Asm function prototype:
//     void CornerMinEigenVal_asm(u8 **in_lines, u8 **out_line, u32 width);
//
//Asm test function prototype:
//     void CornerMinEigenVal_asm_test(u8 **in_lines, u8 **out_line, u32 width);
//
//C function prototype:
//     void mvcvCornerMinEigenVal(u8 **in_lines, u8 **out_line, u32 width);
//
//Parameter description:
// CornerMinEigenVal filter - is 5x5 kernel size
// input_lines   - pointer to input pixel
// output_line   - position on line   
// width         - width of line
//
// each line needs a padding o kernel_size - 1 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
///
#include "cornerMinEigenVal.h"
#include "cornerMinEigenVal_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 8
#define DELTA 1

class CornerMinEigenValKernelTest : public ::testing::TestWithParam< unsigned int > {
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
	unsigned char **outLinesC;
	unsigned char **outLinesAsm;
	unsigned char *buffer;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};




TEST_F(CornerMinEigenValKernelTest, TestUniformInputLines32)
{
	width = 32;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	inputGen.FillLine(inLines[0], width, 25);

	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	buffer  = inputGen.GetEmptyLine( (width + PADDING * 2) * 4 * 6);
	mvcvCornerMinEigenVal(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenValCycleCount / width);

	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING * 2, 5);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(CornerMinEigenValKernelTest, TestAllValuesZero)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	buffer  = inputGen.GetEmptyLine( (width + PADDING * 2) * 4 * 6);
	mvcvCornerMinEigenVal(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenValCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(CornerMinEigenValKernelTest, TestAllValues255)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	buffer  = inputGen.GetEmptyLine( (width + PADDING * 2) * 4 * 6);
	mvcvCornerMinEigenVal(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenValCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(CornerMinEigenValKernelTest, TestMinimumWidthSize)
{
	width = 16;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	inputGen.FillLine(inLines[0], width + PADDING * 2, 25);
	outLinesC = inputGen.GetEmptyLines(width, 5);
	outLinesAsm = inputGen.GetEmptyLines(width, 5);
	buffer  = inputGen.GetEmptyLine( (width + PADDING * 2) * 4 * 6);
	mvcvCornerMinEigenVal(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenValCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(CornerMinEigenValKernelTest, TestSimpleNotEqualInputLines)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 1);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width, 5);
	outLinesAsm = inputGen.GetEmptyLines(width, 5);
	for(int i = 1; i < 5; i++)
	{
		inputGen.FillLine(inLines[i], width + PADDING * 2, i + 1);
	}
	buffer  = inputGen.GetEmptyLine( (width + PADDING * 2) * 4 * 6);
	mvcvCornerMinEigenVal(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenValCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(CornerMinEigenValKernelTest, TestConsecutiveValues)
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
	outLinesC = inputGen.GetEmptyLines(width, 5);
	outLinesAsm = inputGen.GetEmptyLines(width, 5);
	buffer  = inputGen.GetEmptyLine( (width + PADDING * 2) * 4 * 6);
	mvcvCornerMinEigenVal(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenValCycleCount / width);

	outputChecker.CompareArrays( outLinesAsm[0], outLinesC[0], width, DELTA);
}


TEST_F(CornerMinEigenValKernelTest, TestRandomWidth)
{

	width = randGen->GenerateUInt(8, 1280, 8);
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	inputGen.FillLine(inLines[0], width + PADDING * 2, 25);
	outLinesC = inputGen.GetEmptyLines(width, 5);
	outLinesAsm = inputGen.GetEmptyLines(width, 5);

	buffer  = inputGen.GetEmptyLine( (width + PADDING * 2) * 4 * 6);
	mvcvCornerMinEigenVal(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenValCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(CornerMinEigenValKernelTest, TestRandomData)
{
	width = 320;
		unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width , 1);
		buffer  = inputGen.GetEmptyLine( (width + PADDING * 2) * 4 * 6);
	mvcvCornerMinEigenVal(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenValCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(CornerMinEigenValKernelTest, TestRandomWidthRandomData)
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
		
	outLinesC = inputGen.GetEmptyLines(width, 5);
	outLinesAsm = inputGen.GetEmptyLines(width, 5);
	buffer  = inputGen.GetEmptyLine( (width + PADDING * 2) * 4 * 6);
	mvcvCornerMinEigenVal(inLinesOffseted, outLinesC, buffer, width);
	CornerMinEigenVal_asm_test(inLines, outLinesAsm, buffer, width);
	RecordProperty("CyclePerPixel", CornerMinEigenValCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}
