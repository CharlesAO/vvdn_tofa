//boxFilter kernel test
//Asm function prototype:
//     void boxfilter7x7_asm(u8** in, u8** out, u32 normalize, u32 width);
//
//Asm test function prototype:
//     void boxfilter7x7_asm_test(u8** in, u8** out, u32 normalize, u32 width);
//
//C function prototype:
//     void boxfilter7x7(u8** in, u8** out, u32 normalize, u32 width);
//
//Parameter description:
// in        - array of pointers to input lines
// out       - array of pointers for output lines
// normalize - parameter to check if we want to do a normalize boxfilter or not 1 for normalized values , 0 in the other case
// width     - width of input line
//
// each line needs a padding o kernel_size - 1 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
///
#include "boxFilter7x7.h"
#include "boxFilter7x7_asm_test.h"
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

class BoxFilter7x7KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned int normalize;
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


// ------------- normalized variant -------------------

TEST_F(BoxFilter7x7KernelTest, TestUniformInputLines32)
{
	width = 32;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 0);
	inputGen.FillLine(inLines[0], width + PADDING, 49);

	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 1, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 1, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(BoxFilter7x7KernelTest, TestAllValuesZero)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 0);
	
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 1, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 1, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);

	outputChecker.ExpectAllEQ<unsigned char>(outLinesC[0], width, 0);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(BoxFilter7x7KernelTest, TestAllValues255)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 255);
	
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 1, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 1, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(BoxFilter7x7KernelTest, TestMinimumWidthSize)
{
	width = 16;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 0);
	inputGen.FillLine(inLines[0], width + PADDING, 25);
	
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 1, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 1, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(BoxFilter7x7KernelTest, TestSimpleNotEqualInputLines)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 1);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	for(int i = 1; i < 7; i++)
	{
		inputGen.FillLine(inLines[i], width + PADDING, i + 1);
	}

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 1, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 1, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(BoxFilter7x7KernelTest, TestRandomWidthRandomData)
{
	width = randGen->GenerateUInt(16, 1921, 8);
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING *2, 7, 0, 255);
	inputGen.SelectGenerator("uniform");
	for(int i = 0; i < 7; i++) {
		inputGen.FillLineRange(inLines[i], width + PADDING,
				width, width + PADDING - 1, inLines[i][width - 1]);
	}
	
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 1, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 1, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

// ----------------------- unnormalized variant ---------------------------
// the value of unnormalized output pixels can go beyond 8 bits, so
// they are stored on 16 bits values; because of this the size of the
// output buffer is twice as big than in the normalized case.

TEST_F(BoxFilter7x7KernelTest, TestUniformInputLines32Unnormalized)
{
	width = 32;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING*2, 7, 1);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width * 2, 1);
	outLinesAsm = inputGen.GetEmptyLines(width * 2, 1);

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 0, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 0, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);

	outputChecker.CompareArrays((unsigned short*)outLinesC[0], (unsigned short*)outLinesAsm[0], width);
}

TEST_F(BoxFilter7x7KernelTest, TestAllValuesZeroUnnormalized)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING*2, 7, 0);
	
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width * 2, 1);
	outLinesAsm = inputGen.GetEmptyLines(width * 2, 1);

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 0, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 0, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);

	outputChecker.CompareArrays((unsigned short*)outLinesC[0], (unsigned short*)outLinesAsm[0], width);
}


TEST_F(BoxFilter7x7KernelTest, TestAllValues255Unnormalized)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width * 2, 1);
	outLinesAsm = inputGen.GetEmptyLines(width * 2, 1);

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 0, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 0, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);

	outputChecker.CompareArrays((unsigned short*)outLinesC[0], (unsigned short*)outLinesAsm[0], width);
}


TEST_F(BoxFilter7x7KernelTest, TestMinimumWidthSizeUnnormalized)
{
	width = 16;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width * 2, 1);
	outLinesAsm = inputGen.GetEmptyLines(width * 2, 1);

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 0, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 0, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);

	outputChecker.CompareArrays((unsigned short*)outLinesC[0], (unsigned short*)outLinesAsm[0], width);
}

TEST_F(BoxFilter7x7KernelTest, TestSimpleNotEqualInputLinesUnnormalized)
{
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 1);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width * 2, 7);
	outLinesAsm = inputGen.GetEmptyLines(width * 2, 5);
	for(int i = 1; i < 7; i++)
	{
		inputGen.FillLine(inLines[i], width + PADDING, i + 1);
	}

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 0, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 0, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);

	outputChecker.CompareArrays((unsigned short*)outLinesC[0], (unsigned short*)outLinesAsm[0], width);
}

TEST_F(BoxFilter7x7KernelTest, TestRandomWidthRandomDataUnnormalized)
{

	width = randGen->GenerateUInt(16, 1921, 8);
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 0, 255);
	for(int i = 0; i < 7; i++) {
		inputGen.FillLineRange(inLines[i], width + PADDING,
				width, width + PADDING - 1, inLines[i][width - 1]);
	}
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width * 2, 1);
	outLinesAsm = inputGen.GetEmptyLines(width * 2, 1);

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 0, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 0, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

//---------------- parameterized tests -------------------------

INSTANTIATE_TEST_CASE_P(RandomInputsUnnormalized, BoxFilter7x7KernelTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(BoxFilter7x7KernelTest, TestWidthRandomData)
{
	width = GetParam();
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING*2, 7, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 1, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 1, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_P(BoxFilter7x7KernelTest, TestWidthRandomDataUnnormalized)
{
	width = GetParam();
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING*2, 7, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width * 2, 1);
	outLinesAsm = inputGen.GetEmptyLines(width * 2, 1);

	mvcvBoxfilter7x7(inLinesOffseted, outLinesC, 0, width);
	boxfilter7x7_asm_test(inLines, outLinesAsm, 0, width);
	RecordProperty("CyclePerPixel", boxFilter7x7CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

