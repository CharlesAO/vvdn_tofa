//canny edge detector kernel test
//Asm function prototype:
//     void canny_asm(u8** srcAddr, u8** dstAddr,  float threshold1, float threshold2, u32 width);
//
//Asm test function prototype:
//     void canny_asm(unsigned char** srcAddr, unsigned char** dstAddr,
//                    float threshold1, float threshold2, unsigned int width);
//
//C function prototype:
//     void canny(u8** srcAddr, u8** dstAddr,  float threshold1, float threshold2, u32 width);
//
//Parameter description:
// srcAddr    - array of pointers to input lines
// dstAddr    - array of pointers for output lines
// threshold1 - lower threshold
// threshold2 - upper threshold
// width      - width of input line

#include "gtest/gtest.h"
#include "canny.h"
#include "canny_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define DELTA 0
#define PADDING 8

class CannyKernelTest : public ::testing::TestWithParam< unsigned int > {
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
	unsigned char *buffer;
	unsigned int width;
	unsigned char threshold1;
	unsigned char threshold2;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(CannyKernelTest, TestUniformInputLinesAll0)
{
	width = 128;
	threshold1 = 40;
	threshold2 = 70;
	unsigned char **inLinesOffseted;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 9, 0);
	buffer  = inputGen.GetEmptyLine( (width + PADDING * 2) * 16);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 9, 8);
	inputGen.FillLine(inLines[0], width + PADDING * 2, 9);

	outLinesC = inputGen.GetLines(width, 1, 0);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);


	mvcvCanny(inLinesOffseted, outLinesC, buffer, threshold1, threshold2, width);

	canny_asm_test(inLines, outLinesAsm, buffer, threshold1, threshold2, width);

	RecordProperty("CyclePerPixel", CannyCycleCount / width);

		outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(CannyKernelTest, TestUniformInputLinesAll255)
{
	width = 120;
	threshold1 = 40;
	threshold2 = 70;

	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 9, 255);
	buffer  = inputGen.GetEmptyLine( (width + PADDING * 2) * 16);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 9, 8);

	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvCanny(inLinesOffseted, outLinesC, buffer, threshold1, threshold2, width);
	canny_asm_test(inLines, outLinesAsm, buffer, threshold1, threshold2, width);
	RecordProperty("CyclePerPixel", CannyCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(CannyKernelTest, TestUniformInputLines)
{
	width = 320;
	threshold1 = 40;
	threshold2 = 70;

	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 9, 7);
	buffer  = inputGen.GetEmptyLine( (width + PADDING * 2) * 16);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 9, 8);

	outLinesC = inputGen.GetLines(width, 1, 0);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvCanny(inLinesOffseted, outLinesC, buffer, threshold1, threshold2, width);
	canny_asm_test(inLines, outLinesAsm, buffer, threshold1, threshold2, width);
	RecordProperty("CyclePerPixel", CannyCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(CannyKernelTest, TestRandomWithRandomInputLines)
{
	
	threshold1 = 40;
	threshold2 = 70;
	
	width = randGen->GenerateUInt(0, 1921, 8);
	width = (width >> 3)<<3; //(line width have to be mutiple of 8)
	
	float contorC = 0, contorAsm = 0, absdiff;
	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 9, 7);
	buffer  = inputGen.GetEmptyLine( (width + PADDING * 2) * 16);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 9, 8);

	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvCanny(inLinesOffseted, outLinesC, buffer, threshold1, threshold2, width);
	canny_asm_test(inLines, outLinesAsm, buffer, threshold1, threshold2, width);
	RecordProperty("CyclePerPixel", CannyCycleCount / width);

	for (unsigned int i=0; i<width; i++)
	{
		if (outLinesC[0][i] == 255) contorC++;
		if (outLinesAsm[0][i] == 255) contorAsm++;
	}
	
	absdiff = contorC - contorAsm;
	if (absdiff < 0 ) absdiff = -absdiff;

	
	EXPECT_NEAR(0,absdiff/width,0.2);
	

	
}
