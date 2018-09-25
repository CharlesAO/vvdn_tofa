/// This kernel performs a biliniar rescaling
/// @param[in] in          - Input lines, 16-bits floating point
/// @param[out] out        - Output line, 16-bits floating point
/// @param[in] inWidth    - Width of input line

#include "gtest/gtest.h"
#include "resizeAlphaFp16ToFp16.h"
#include "resizeAlphaFp16ToFp16_asm_test.h"
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
#define SIZE 2
half DELTA =0.3; //accepted tolerance between C and ASM results

class ResizeAlphaFp16ToFp16KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	//half convMatAsm[9];
	half **inLines;
	half *outLinesC;
	half *outLinesAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


TEST_F(ResizeAlphaFp16ToFp16KernelTest, TestRandomInputLinesRandomConvMatrix)
{

	width = randGen->GenerateUInt(8, 1600, 8);
	half **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2 + PADDING, 2, 0.0, 255.0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 2, 8);
	outLinesC = inputGen.GetLineFloat16(width + PADDING,5,5);
	outLinesAsm = inputGen.GetLineFloat16(width + PADDING,5,5);


	mvispResizeAlphaFp16ToFp16(inLinesOffseted, outLinesC, width);
	ResizeAlphaFp16ToFp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", ResizeAlphaFp16ToFp16CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width + PADDING, DELTA);
}


TEST_F(ResizeAlphaFp16ToFp16KernelTest, TestExperimental)
{
	width = 1600;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2 + PADDING, 3, 0, 255);
	outLinesC = inputGen.GetLineFloat16(width + PADDING,5,5);
	outLinesAsm = inputGen.GetLineFloat16(width + PADDING,5,5);

	ResizeAlphaFp16ToFp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", ResizeAlphaFp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvispResizeAlphaFp16ToFp16(inLines, outLinesC, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width + PADDING, DELTA);
}

TEST_F(ResizeAlphaFp16ToFp16KernelTest, TestUniformInputLinesWidth16)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width * 2 + PADDING, 3, 3);
	outLinesC = inputGen.GetLineFloat16(width + PADDING,5,5);
	outLinesAsm = inputGen.GetLineFloat16(width + PADDING,5,5);
	
	ResizeAlphaFp16ToFp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", ResizeAlphaFp16ToFp16CycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvispResizeAlphaFp16ToFp16(inLines, outLinesC, width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width + PADDING, DELTA);
}


TEST_F(ResizeAlphaFp16ToFp16KernelTest, TestUniformInputLinesAllValues255)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width * 2 + PADDING, 3, 255);
	outLinesC = inputGen.GetLineFloat16(width + PADDING,5,5);
	outLinesAsm = inputGen.GetLineFloat16(width + PADDING,5,5);

	ResizeAlphaFp16ToFp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", ResizeAlphaFp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvispResizeAlphaFp16ToFp16(inLines, outLinesC, width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width + PADDING, DELTA);
}

//------------ test different convolution matrices ------------------------

TEST_F(ResizeAlphaFp16ToFp16KernelTest, TestRandomLinesSobel)
{
	width =64;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2 + PADDING, 3, 0, 255);
	outLinesC = inputGen.GetLineFloat16(width + PADDING,5,5);
	outLinesAsm = inputGen.GetLineFloat16(width + PADDING,5,5);


	ResizeAlphaFp16ToFp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", ResizeAlphaFp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvispResizeAlphaFp16ToFp16(inLines, outLinesC, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width + PADDING, DELTA);
}

TEST_F(ResizeAlphaFp16ToFp16KernelTest, TestRandomLinesEdgeDetect)
{
	width = 640;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2 + PADDING, 3, 0, 255);
	outLinesC = inputGen.GetLineFloat16(width + PADDING,5,5);
	outLinesAsm = inputGen.GetLineFloat16(width + PADDING,5,5);

	ResizeAlphaFp16ToFp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", ResizeAlphaFp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvispResizeAlphaFp16ToFp16(inLines, outLinesC, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width + PADDING, DELTA);
}

TEST_F(ResizeAlphaFp16ToFp16KernelTest, TestRandomLinesSharpen)
{
	width = 640;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2 + PADDING, 3, 0, 255);
	outLinesC = inputGen.GetLineFloat16(width + PADDING,5,5);
	outLinesAsm = inputGen.GetLineFloat16(width + PADDING,5,5);

	ResizeAlphaFp16ToFp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", ResizeAlphaFp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvispResizeAlphaFp16ToFp16(inLines, outLinesC, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width + PADDING, DELTA);
}

TEST_F(ResizeAlphaFp16ToFp16KernelTest, TestRandomLinesRandomKernel)
{
	width = 640;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2 + PADDING, 3, 0, 255);
	outLinesC = inputGen.GetLineFloat16(width + PADDING,5,5);
	outLinesAsm = inputGen.GetLineFloat16(width + PADDING,5,5);


	ResizeAlphaFp16ToFp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", ResizeAlphaFp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvispResizeAlphaFp16ToFp16(inLines, outLinesC, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width + PADDING, DELTA);
}

//------------ parameterized tests ----------------------------------------

INSTANTIATE_TEST_CASE_P(UniformInputs, ResizeAlphaFp16ToFp16KernelTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1600);
);

TEST_P(ResizeAlphaFp16ToFp16KernelTest, TestRandomInputLinesEmbossFilter)
{

	width = GetParam();

	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2 + PADDING, 3, 0, 255);
	outLinesC = inputGen.GetLineFloat16(width + PADDING,5,5);
	outLinesAsm = inputGen.GetLineFloat16(width + PADDING,5,5);

	ResizeAlphaFp16ToFp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", ResizeAlphaFp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvispResizeAlphaFp16ToFp16(inLines, outLinesC, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width + PADDING, DELTA);
}
