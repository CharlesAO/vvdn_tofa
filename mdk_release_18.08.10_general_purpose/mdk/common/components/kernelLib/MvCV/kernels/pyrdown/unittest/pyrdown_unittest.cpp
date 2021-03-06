//pyrDown kernel test
//Asm function prototype:
//     void mvcvPyrdown_asm(u8 **inLine,u8 **outLine,int width);
//
//Asm test function prototype:
//     void sobel_asm_test(unsigned char** in, unsigned char** out, unsigned int width);
//
//C function prototype:
//     void mvcvPyrdown(u8 **inLine, u8 **outLine, int width);
//
//Parameter description:
// pyrdown filter - downsample even lines and even cols
// inLine  - array of pointers to input lines
// outLine - array of pointers for output lines
// width   - width of input line

#include "gtest/gtest.h"
///
#include "pyrdown.h"
#include "pyrdown_asm_test.h"
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

class PyrDownKernelTest : public ::testing::TestWithParam< unsigned int > {
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
	unsigned int outWidth;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(PyrDownKernelTest, TestUniformInputLines)
{
	width = 320;
	outWidth = (width - 1) / 2 + 1;

	inputGen.SelectGenerator("uniform");
	inLinesAsm = inputGen.GetLines(width + PADDING*2, 5, 7);
	inLinesC = inputGen.GetOffsettedLines(inLinesAsm, 5, 8);

	outLinesC = inputGen.GetLines(outWidth, 1, 0);
	outLinesAsm = inputGen.GetEmptyLines(outWidth, 1);

	mvcvPyrdown(inLinesC, outLinesC, width);
	pyrdown_asm_test(inLinesAsm, outLinesAsm, width);
	RecordProperty("CyclePerPixel", PyrDownCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], outWidth);
}


TEST_F(PyrDownKernelTest, TestUniformInputLinesAll0)
{
	width = 320;
	outWidth = (width - 1) / 2 + 1;

	inputGen.SelectGenerator("uniform");
	inLinesAsm = inputGen.GetLines(width + PADDING*2, 5, 0);
	inLinesC = inputGen.GetOffsettedLines(inLinesAsm, 5, 8);

	outLinesC = inputGen.GetEmptyLines(outWidth, 1);
	outLinesAsm = inputGen.GetEmptyLines(outWidth, 1);

	mvcvPyrdown(inLinesC, outLinesC, width);
	pyrdown_asm_test(inLinesAsm, outLinesAsm, width);
	RecordProperty("CyclePerPixel", PyrDownCycleCount / width);

	outputChecker.ExpectAllEQ<unsigned char>(outLinesAsm[0], outWidth, 0);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], outWidth);
}


TEST_F(PyrDownKernelTest, TestUniformInputLinesAll255)
{
	width = 64;
	outWidth = (width - 1) / 2 + 1;

	inputGen.SelectGenerator("uniform");
	inLinesAsm = inputGen.GetLines(width + PADDING*2, 5, 255);
	inLinesC = inputGen.GetOffsettedLines(inLinesAsm, 5, 8);

	outLinesC = inputGen.GetEmptyLines(outWidth, 1);
	outLinesAsm = inputGen.GetEmptyLines(outWidth, 1);

	mvcvPyrdown(inLinesC, outLinesC, width);
	pyrdown_asm_test(inLinesAsm, outLinesAsm, width);
	RecordProperty("CyclePerPixel", PyrDownCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], outWidth);
}

TEST_F(PyrDownKernelTest, TestRandomInputLines)
{
	width = 240;
	outWidth = (width - 1) / 2 + 1;

	inputGen.SelectGenerator("random");
	inLinesAsm = inputGen.GetLines(width + PADDING*2, 5, 0, 255);
	inLinesC = inputGen.GetOffsettedLines(inLinesAsm, 5, 8);

	outLinesC = inputGen.GetEmptyLines(outWidth, 1);
	outLinesAsm = inputGen.GetEmptyLines(outWidth, 1);

	mvcvPyrdown(inLinesC, outLinesC, width);
	pyrdown_asm_test(inLinesAsm, outLinesAsm, width);
	RecordProperty("CyclePerPixel", PyrDownCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], outWidth);
}



TEST_F(PyrDownKernelTest, TestRandomWidthRandomDataAndWidth)
{
	width = randGen->GenerateUInt(48, 1921,16);
	width = (width >> 3)<<3; //(line width have to be mutiple of 8)
	outWidth = (width - 1) / 2 + 1;

	inputGen.SelectGenerator("random");
	inLinesAsm = inputGen.GetLines(width + PADDING*2, 5, 0, 255);
	inLinesC = inputGen.GetOffsettedLines(inLinesAsm, 5, 8);

	outLinesC = inputGen.GetEmptyLines(outWidth, 1);
	outLinesAsm = inputGen.GetEmptyLines(outWidth, 1);

	mvcvPyrdown(inLinesC, outLinesC, width);
	pyrdown_asm_test(inLinesAsm, outLinesAsm, width);
	RecordProperty("CyclePerPixel", PyrDownCycleCount / width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], outWidth);
}
